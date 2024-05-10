假定现在需要实现一个函数，接受一个 `name` 参数，打印当前时间，然后把 `name` 放到某个数据结构中。一种可能的实现如下
```cpp
std::multiset<std::string> names; // global data structure
void logAndAdd(const std::string &name)
{
    // get current time
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd"); // make log entry
    names.emplace(name);   // add name to global data  structure;
                           // see Item 42 for info on emplace
}
```
这段代码没有问题，但是性能不足够好。考虑下面几种调用情况
```cpp
std::string petName("Darla");
logAndAdd(petName); // pass lvalue std::string

logAndAdd(std::string("Persephone")); // pass rvalue std::string

logAndAdd("Patty Dog"); // pass string literal
```
第一个调用，`name` 绑定到 `petName`，在内部，`name` 传递给 `names.emplace`。由于 `name` 是左值，所有不得不拷贝到 `names`。没有办法避免拷贝。

第二个调用，`name` 绑定到了右值，这个右值是通过 `"Persephone"` 显式构造的 `std::string` 对象。`name` 自己是一个左值，所以必须拷贝到 `names` 里面。这里，我们其实可以移动而不是拷贝，加上 `move`，可能可以实现。

第三个调用，`name` 绑定了了右值，不过这次是通过 `""Patty Dog"` 隐式构造的 `std::string` 对象。和第二个调用一样，也需要拷贝。不过和上次不同的是，这次传递的是字符串字面量。如果将字面量直接传递给 `emplace` 函数，那么就不需要创建 `std::string` 的临时对象了。`emplace` 会在 `std::multiset` 创建要存放的 `std::string` 对象。这里我们付出了拷贝字符串的代价，但是这里本无需付出移动的代价。

可以使用 [Item 24](./24_Distinguish_universal_references_from_rvalue_references.md) 中谈及的通用引用来提升第二个和第三个调用的性能。根据 [Item 25](./25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md)，需要使用 `std::forward` 通用引用。
```cpp
template <typename T>
void logAndAdd(T &&name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}
std::string petName("Darla"); // as before
logAndAdd(petName);           // as before, copy lvalue into multiset

logAndAdd(std::string("Persephone")); // move rvalue instead of copying it

logAndAdd("Patty Dog"); // create std::string in multiset instead
                        // of copying a temporary std::string
```
现在增加一个需求，有的时候客户端无法访问 `name` 而只知道 `idx`，`logAndAdd` 内部根据 `idx` 搜索 `name`。为了实现这一点，需要重载 `logAndAdd`。
```cpp
std::string nameFromIdx(int idx); // return name corresponding to idx

void logAndAdd(int idx) // new overload
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(nameFromIdx(idx));
}
```
有了两个重载的版本，下面的调用都能如预期工作。
```cpp
std::string petName("Darla"); // as before

logAndAdd(petName);                   // as before, these
logAndAdd(std::string("Persephone")); // calls all invoke
logAndAdd("Patty Dog");               // the T&& overload

logAndAdd(22); // calls int overload
```
但是假定定义了 `short` 变量，然后调用 `logAndAdd` 方法，就会出错。
```cpp
short nameIdx;

// give nameIdx a value

logAndAdd(nameIdx); // error!
```
`logAndAdd` 有两个重载。对于通用引用版本，`T` 推导为 `short`，精确匹配。`int` 版本的函数也可以匹配 `short`，但是有一个隐式转换。根据重载匹配规则，所有调用的参数类型为通用引用的版本。

`name` 绑定了 `short` 类型，一步步的传递给 `emplace`，由于 `names` 类型是 `std::multiset<std::string>`，所以会构造 `std::string` 对象。但是 `std::string` 没有接受 `short` 参数的构造函数，所以调用 `logAndAdd` 失败。根源就在于 `short` 类型的参数优先匹配了通用引用的版本而不是接受 `int` 参数的版本。

接受通用引用参数的函数时 C++ 中最贪婪的函数。对于绝大部分参数，都能实例化为精确匹配的函数。很少几种类型无法匹配，在 Item 30 TODO link 中有描述。这就是为什么重载通用引用类型的函数不是一个好主意：它会匹配比开发者预期的多的多参数类型。

下面是一个完美转发构造函数与重载的接受 `int` 为参数的类。这个例子包含了 `logAndAdd` 的问题，还有更多的新问题。
```cpp
class Person
{
public:
    template <typename T>
    explicit Person(T &&n)         // perfect forwarding ctor;
        : name(std::forward<T>(n)) // initializes data member
    {
    }

    explicit Person(int idx) // int ctor
        : name(nameFromIdx(idx))
    {
    }

private:
    std::string name;
};
```
与 `logAndAdd` 的例子一样，传入非 `int` 的整数类习惯，比如 `short` `long` `std::size_t` 等，都会调用完美转发的构造函数而不是 `int` 版本的重载，这会导致编译错误。

[Item 17](../ch03_Moving_to_Modern_C++/17_Understand_special_member_function_generation.md) 告诉我们，在合适的情况下，即使一个类包含了模板构造函数，在某些情况下实例化之后的签名与拷贝构造和移动构造函数一样，C++ 还是会自动合成这两个构造函数。因此，`Person` 类实际看起来是下面这个样子。
```cpp
class Person
{
public:
    template <typename T> // perfect forwarding ctor
    explicit Person(T &&n)
        : name(std::forward<T>(n))
    {
    }

    explicit Person(int idx); // int ctor

    Person(const Person &rhs); // copy ctor (compiler-generated)

    Person(Person &&rhs); // move ctor (compiler-generated)
};
```
会出现下面这个反直觉的错误：
```cpp
Person p("Nancy");
auto cloneOfP(p); // create new Person from p; this won't compile!
```
这里使用一个 `Person` 对象构造另外一个，很明显，调用拷贝构造函数。注意，这 `p` 是左值，不能通过移动的方式完成拷贝。

## Things to Remember
* Overloading on universal references almost always leads to the universal reference overload being called more frequently than expected.
* Perfect-forwarding constructors are especially problematic, because they're typically better matches than copy constructors for non-const lvalues, and they can hijack derived class calls to base class copy and move constructors.
