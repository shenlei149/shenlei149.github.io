`using` 现在支持了逗号表达式声明一系列名字，也可以用于参数包。

在 C++17 之前，下面的例子中需要写三行 `using`。
```cpp
class Base
{
public:
    void a();
    void b();
    void c();
};

class Derived : private Base
{
public:
    using Base::a, Base::b, Base::c;
};
```

## Using Variadic Using Declarations
逗号分割的 `using` 使得我们可以继承可变数量的基类的同一个运算。

这个技术很酷的一个应用是创建一个重载 lambda 的集合。
```cpp
// "inherit" all function call operators of passed base types:
template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

// base types are deduced from passed arguments:
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

auto twice = overload{
    [](std::string &s)
    { s += s; },

    [](auto &v)
    { v *= 2; }};
```
这里创建了一个类型为 `overload` 的对象，其定义中使用推导规则根据 lambda 的类型推导出 `overload` 基类的类型。这里还是用了聚合体初始化，调用用每个 lambda 的闭包类型的拷贝构造函数来初始化基类对象。

`using` 的使用使得 `overload` 的两个函数调用操作符都有效；否则，两个基类对 `operator()` 运算符有不同的重载，这会导致歧义。

下面使用 `twice` 的例子。如果传入的是字符串类型，调用第一个重载，其他类型（前提是有 `*=` 运算符）调用第二个重载。
```cpp
int i = 42;
twice(i);
std::cout << "i: " << i << '\n'; // prints: 84

std::string s = "hi";
twice(s);
std::cout << "s: " << s << '\n'; // prints: hihi
```

`std::variant` 访问器也使用了这种模式。

## Variadic Using Declarations for Inheriting Constructors
使用 `using`，可以继承一系列基类的构造函数，而无需一个一个声明。下面是一个可变参数模板类 `Multi`，继承了每个指定参数的基类。
```cpp
template <typename T>
class Base
{
    T value{};

public:
    Base()
    {
    }

    Base(T v) : value{v}
    {
    }
};

template <typename... Types>
class Multi : private Base<Types>...
{
public:
    // derive all constructors:
    using Base<Types>::Base...;
};
```
下面定义一个三个不同类型的 `Multi<>` 类型，并使用不同类型的值来定义一些对象。根据类型不同，会调用其中一个带参数的基类构造函数和其余基类的默认构造函数。比如 `m2` 会调用`Base<bool>` 的默认构造函数，`Base<std::string>` 的字符串构造函数和 `Base<int>` 的默认构造函数。
```cpp
using MultiISB = Multi<int, std::string, bool>;

MultiISB m1 = 42;
MultiISB m2 = std::string("hello");
MultiISB m3 = true;
```
原则上我们可以继承所有赋值函数。
```cpp
template <typename... Types>
class Multi : private Base<Types>...
{
    // derive all assignment operators:
    using Base<Types>::operator=...;
};
```
