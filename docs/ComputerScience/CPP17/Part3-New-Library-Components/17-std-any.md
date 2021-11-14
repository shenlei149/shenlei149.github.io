一般情况下，C++ 是类型绑定、类型安全的语言。值对象的类型是确定的，支持什么操作，行为是什么也都是确定的。对象不能改变它的类型。

`std::any` 是值类型，能够改变类型，同时保证类型安全。该对象可以持有任意类型的值，同时知道正在保存的值的类型。当声明对象时无需指定可能得类型。

关键就是保存包含的值的同时保存包含值的类型。因为值可能任意大，因此可能需要堆分配。实现对于很小的类型，比如 `int`，要避免堆分配。

如果用字符串赋值，那么会分配一段内存，然后复制到新的内存中，同时保存当前持有对象是一个字符串。运行时可以检查其包含值的类型。当要使用的时候必须用 `any_cast<>` 指定类型。

`std::any` 和 `std::optional<>` `std::variant<>` 一样，是值语义。因此拷贝是深拷贝，会创建一个独立的对象。因为可能会涉及堆分配，因此拷贝 `std::any` 通常比较开销比较大。推荐引用传递或者移动。支持部分移动语义。

## Using `std::any`
下面是使用 `std::any` 的例子。`std::any` 可以为空，也可以用不同类型进行初始化。如果有初始化的话，赋值的值的类型是 `std::any` 包含值的类型。通过 `type()` 成员函数，可以检查包含值的类型和某个类型的 ID 是否一致。如果为空，类型 ID 是 `typeid(void)`。
```cpp
std::any a;            // a is empty
std::any b = 4.3;      // b has value 4.3 of type double
a = 42;                // a has value 42 of type int
b = std::string{"hi"}; // b has value "hi" of type std::string

if (a.type() == typeid(std::string))
{
    std::string s = std::any_cast<std::string>(a);
    useString(s);
}
```
必须使用 `any_cast<>` 转化类型。如果对象为空或者包含值的类型不匹配导致转换失败会抛出异常 `std::bad_any_cast`。如果不知道具体类型，可以像下面这样使用。
```cpp
try
{
    auto s = std::any_cast<std::string>(a);
}
catch (std::bad_any_cast &e)
{
    std::cerr << "EXCEPTION: " << e.what() << '\n';
}
```
`any_cast<>` 会构造一个传入的类型的对象。比如这里模板类型是 `std::string`，因此会创建一个临时对象，然后用这个临时对象初始化 `s`。如果不要这样的初始化，可以转成引用类型避免创建临时对象。
```cpp
std::cout << std::any_cast<const std::string &>(a);
```
通过引用还可以修改值。
```cpp
std::any_cast<std::string &>(a) = "world";
```

`any_cast<>` 接受 `std::any` 对象的指针，如果类型匹配，返回指向包含值的指针，否则返回 `nullptr`。
```cpp
auto p = std::any_cast<std::string>(&a);
if (p)
{
}

if (auto p = std::any_cast<std::string>(&a); p != nullptr)
{
}
```
可以通过以下几种方法清空一个 `std::any` 对象。
```cpp
a.reset(); // makes it empty

a = std::any{};

a = {};
```
也可以检查是否为空。
```cpp
if (a.has_value())
{
}
```
会使用包含值的退化类型（数组变成指针，顶层的引用和 `const` 会被忽略）。字符串字面量赋值的话类型是 `const char *`，因此要像下面这样使用。
```cpp
std::any a = "hello"; // type() is const char*

if (a.type() == typeid(const char *)) // true
{
}

if (a.type() == typeid(std::string)) // false
{
}

std::cout << std::any_cast<const char *>(a) << '\n'; // OK
std::cout << std::any_cast<std::string>(a) << '\n';  // EXCEPTION
```
这基本上就是所有支持的操作了。没有哈希，没有比较。没有 `value()` 获取当前值。因为运行时才知道类型，不能用泛型 lambda 来独立于类型来处理值。只能使用 `std::any_cast<>` 类处理。

不过可以将 `std::any` 放到 `std::vector` 中。这里只能使用 `if-else` 链，不能使用 `switch`。
```cpp
std::vector<std::any> v;

v.push_back(42);

std::string s = "hello";
v.push_back(s);

for (const auto &a : v)
{
    if (auto pa = std::any_cast<const std::string>(&a); pa != nullptr)
    {
        std::cout << "string: " << *pa << '\n';
    }
    else if (auto pa = std::any_cast<const int>(&a); pa != nullptr)
    {
        std::cout << "int: " << *pa << '\n';
    }
}
```

## `std::any` Types and Operations
### Any Types
下面是头文件 `<any>` 中 `std::any` 的定义，可见这不是一个模板类。
```cpp
namespace std
{
    class any;
}
```
除此之外，还定义了异常 `std::bad_any_cast`，继承自 `std::bad_cast`，后者继承自 `std::exception`。当类型转换失败的时候会抛出这个异常。

`std::any` 也使用 `<utility>` 中定义的 `std::in_place_type`，其类型是 `std::in_place_type_t`。

### Any Operations

| Operation | Effect |
|--|--|
| constructors | Creates an any object (might call constructor for underlying type) |
| `make_any()` | Creates an any object (passing value(s) to initialize it) |
| destructor | Destroys an any object |
| `=` | Assigns a new value |
| `emplace<T>()` | Assigns a new value with the type `T` |
| `reset()` | Destroys any value (makes the object empty) |
| `has_value()` | Returns whether the object has a value |
| `type()` | Returns the current type as std::type_info object |
| `any_cast<T>()` | Uses the current value as value of type `T` (exception/`nullptr` if other type) |
| `swap()` | Swaps values between two objects |

#### Construction
默认情况下，构造一个空对象。也可以传递值进行初始化，不过这里类型会发生退化。如果想要类型与初始值是不同类型，需要使用 `in_place_type`，需要注意的是，使用 `in_place_type` 也会有类型退化问题。
```cpp
std::any a1; // a1 is empty

std::any a2 = 42;      // a2 contains value of type int
std::any a3 = "hello"; // a2 contains value of type const char*

std::any a4{std::in_place_type<long>, 42};
std::any a5{std::in_place_type<std::string>, "hello"};

std::any a5b{std::in_place_type<const char[6]>, "hello"};
```
因为包含类型无法被推导，因此如果需要使用多个参数初始化，可以先构造一个对象赋值，或者使用 `std::in_place_type`。
```cpp
std::any a6{std::complex{3.0, 4.0}};
std::any a7{std::in_place_type<std::complex<double>>, 3.0, 4.0};
```
还可以使用初始化列表，也可以有附加参数。
```cpp
// initialize a std::any with a set with lambda as sorting criterion:
auto sc = [](int x, int y)
{
    return std::abs(x) < std::abs(y);
};

std::any a8{std::in_place_type<std::set<int, decltype(sc)>>,
            {4, 8, -7, -2, 0, 5},
            sc};
```
可以使用 `make_any<>()` 构造，此时可以传递一个或多个参数。由于没有类型推导，即使只有一个参数，也需要指明类型。
```cpp
auto a10 = std::make_any<float>(3.0);
auto a11 = std::make_any<std::string>("hello");
auto a13 = std::make_any<std::complex<double>>(3.0, 4.0);
auto a14 = std::make_any<std::set<int, decltype(sc)>>({4, 8, -7, -2, 0, 5},
                                                      sc);
```

#### Changing the Value
赋值和 `emplace()` 可以修改已经存在的值。
```cpp
std::any a;

a = 42;      // a contains value of type int
a = "hello"; // a contains value of type const char*

a.emplace<std::string>("hello");           // a contains value of type std::string
a.emplace<std::complex<double>>(4.4, 5.5); // a contains value of type std::complex<double>
```

#### Accessing the Value
需要使用 `std::any_cast<>` 来转化类型。如果去掉顶层引用和 `const` 和指定类型兼容，那么可以转换成功，否则会抛出异常。
```cpp
std::any_cast<std::string>(a);         // yield copy of the value
std::any_cast<std::string &>(a);       // write access by reference
std::any_cast<const std::string &>(a); // read access by reference
```
为了避免异常，可以传递一个 `std::any` 的指针，如果转化失败，会返回 `nullptr`。注意，这样不能转成引用类型。
```cpp
if (auto sp{std::any_cast<std::string>(&a)}; sp != nullptr)
{
    // use *sp for write access to the value of a
}

if (auto sp{std::any_cast<const std::string>(&a)}; sp != nullptr)
{
    // use *sp for read access to the value of a
}

std::any_cast<std::string &>(&a); // RUNTIME ERROR
```

#### Move Semantics
仅对于支持拷贝语义的底层对象，`std::any` 支持移动语义。也就是说，对于仅可移动的类型作为包含类型是不支持移动的。

对于移动操作而言，做法稍微有点不直观。
```cpp
std::string s("hello, world!");

std::any a;
a = std::move(s); // move s into a

s = std::move(std::any_cast<std::string &>(a)); // move assign string in a to s
```
和移动对象类似，`a` 仍旧有效，但是不要对其中包含的字符串做任何假设。下面的代码是不会输出 `NIL` 的。
```cpp
std::cout << (a.has_value() ? std::any_cast<std::string>(a) : std::string("NIL"));
```
下面的方法也奏效，不过多了一次移动。
```cpp
s = std::any_cast<std::string>(std::move(a));
```
不要转成右值引用，而是使用 `std::move()`。
```cpp
s = std::any_cast<std::string &&>(a); // compile-time error

a = std::move(s); // move s into a
```
注意，下面的代码仅在已经包含 `std::string` 类型的值是才能正常工作。
```cpp
std::any_cast<string &>(a) = std::move(s); // OOPS: a must hold a string
```
