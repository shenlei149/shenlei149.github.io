C++11 引入了 lambda 表达式，C++14 引入了泛型 lambda，都是非常成功的。C++17 新增了如下应用场景。

* 编译期常量表达式
* 需要拷贝当前对象的情况

## `constexpr` Lambdas
C++17 开始，lambda 表达式默认是 `constexpr` 的。也就是说，如果上下文都是编译期确定的，比如没有静态变量，没有 `try/catch`，没有虚函数，没有 `new/delete`，那么 lambda 表达式可以用于编译期。

比如下面的例子，可以用于编译期指定 `std::array<>` 的大小。
```cpp
auto squared = [](auto val) { // implicitly constexpr since C++17
    return val * val;
};

std::array<int, squared(5)> a; // OK since C++17 => std::array<int,25>
```
如果不满足编译期的条件，那么就失去了 `constexpr` 属性，不过仍旧可以运行时使用。
```cpp
auto squared2 = [](auto val) { // implicitly constexpr since C++17
    static int calls = 0;      // OK, but disables lambda for constexpr contexts

    return val * val;
};

std::array<int, squared2(5)> a;   // ERROR: static variable in compile-time context
std::cout << squared2(5) << '\n'; // OK
```
如果想确定是编译期合法的，可以加 `constexpr` 关键字，如果不满足条件，无法编译。另外，lambda 表达式也可以指定返回类型。
```cpp
auto squared3 = [](auto val) constexpr { // OK since C++17
    return val * val;
};

auto squared3i = [](int val) constexpr -> int { // OK since C++17
    return val * val;
};

auto squared4 = [](auto val) constexpr
{
    static int calls = 0; // ERROR: static variable in compile-time context

    return val * val;
};
```
和其他 `constexpr` 函数一样，如果 lambda 是在运行上下文中的，那么也是运行时调用其函数体。

对于显式或者隐式的 `constexpr` lambda 表达式，对应闭包类型的 `operator()` 也是 `constexpr`。
```cpp
auto squared = [](auto val) { // implicitly constexpr since C++17
    return val * val;
};

class CompilerSpecificName
{
public:
    template <typename T>
    constexpr auto operator()(T val) const
    {
        return val * val;
    }
};
```
注意下面两个定义是不同的。
```cpp
auto squared1 = [](auto val) constexpr { // compile-time lambda calls
    return val * val;
};

constexpr auto squared2 = [](auto val) { // compile-time initialization
    return val * val;
};
```
第一个只有 lambda 表达式是 `constexpr`，可以用于编译期的地方，但是 `squared1` 可以直到运行时才初始化。如果依赖于静态初始化顺序，可能就会出问题。

### Using `constexpr` Lambdas
下面是一个使用 `constexpr` lambda 表达式的例子。lambda 表达式用于计算字符串的哈希值。
```cpp
auto hashed = [](const char *str)
{
    std::size_t hash = 5381; // initial hash value
    while (*str != '\0')
    {
        hash = hash * 33 ^ *str++; // combine hash value with next character
    }
    return hash;
};
```
我们可以在编译器初始化枚举值，或者是用于 `switch` 的 `case` 分支，注意 `switch` 条件还是运行时调用。
```cpp
enum Hashed
{
    beer = hashed("beer"), // OK, compile-time hashing
    wine = hashed("wine"),
    water = hashed("water"),
};

switch (hashed(argv[1])) // runtime hashing
{
case hashed("beer"): // OK, compile-time hashing

    break;
case hashed("wine"):

    break;
}
```
可以用编译期 lambda 初始化一个容器，那么编译器优化时可以在编译期初始化这个容器的值。
```cpp
std::array arr{hashed("beer"),
               hashed("wine"),
               hashed("water")};
```
一个 `constexpr` lambda 可以使用另一个 `constexpr` lambda 表达式。下面两个哈希值都是运行时计算的，但是使用了不同的逻辑，所以值不同。
```cpp
auto hashed = [](const char *str, auto combine)
{
    std::size_t hash = 5381; // initial hash value
    while (*str != '\0')
    {
        hash = combine(hash, *str++); // combine hash value with next character
    }
    return hash;
};

constexpr std::size_t hv1{hashed("wine",
                                 [](auto h, char c)
                                 { return h * 33 + c; })};
constexpr std::size_t hv2{hashed("wine",
                                 [](auto h, char c)
                                 { return h * 33 ^ c; })};
```

## Passing Copies of this to Lambdas
在一个非静态函数中，如果不捕获 `this`，那么无法使用成员变量，显式的写 `this->` 也不行。所以想要使用成员变量，必须显式捕获，按值或按引用都行，前者也只是指针的副本罢了。
```cpp
class C
{
private:
    std::string name;

public:
    void foo()
    {
        auto l1 = []
        { std::cout << name << '\n'; }; // ERROR
        auto l2 = []
        { std::cout << this->name << '\n'; }; // ERROR
    }
};

class C
{
private:
    std::string name;

public:
    void foo()
    {
        auto l1 = [this]
        { std::cout << name << '\n'; }; // OK
        auto l2 = [=]
        { std::cout << name << '\n'; }; // OK
        auto l3 = [&]
        { std::cout << name << '\n'; }; // OK
    }
};
```
不过，这样会有一个问题，如果 lambda 的生命周期比传入的对象生命要长，那么后续使用 `this` 的成员变量就会出错。此时，可能需要捕获对象的拷贝。C++14 中可以如下解决。
```cpp
class C
{
private:
    std::string name;

public:
    void foo()
    {
        auto l1 = [thisCopy = *this]
        { std::cout << thisCopy.name << '\n'; };
    }
};
```
不过，如果有其他捕获方式的话，可能还是会不经意间使用 `this` 指针的指向的变量。
```cpp
auto l1 = [&, thisCopy = *this]
{
    thisCopy.name = "new name";
    std::cout << name << '\n'; // OOPS: still the old name
};
```
C++17 可以用 `*this` 捕获拷贝。闭包类型会保存一份副本。
```cpp
class C
{
private:
    std::string name;

public:
    void foo()
    {
        auto l1 = [*this]
        { std::cout << name << '\n'; };
    }
};
```
此时，还是可以使用其他捕获方式，不过 `this` 不能重复。
```cpp
auto l2 = [&, *this] {};    // OK
auto l3 = [this, *this] {}; // ERROR
```

