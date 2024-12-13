编程中，我们经常会遇到一个场景，可能会传递、返回、使用一个确定类型的值，有可能是没有值的。对于指针类型，`nullptr` 表示没有值。其他类型需要返回一个确定类型和一个 `bool` 值表示是否有值。`std::optional<>` 就是类型安全的实现。

可选对象内部包含了一个对象和一个布尔变量，因此多占一个字节，考虑到对其等因素，额外占用空间会略大。可选对象无需堆分配，对其方式与包含的对象一致。

可选对象的实现并不等于一个布尔变量加一个值对象。如果没有值，不会调用包含类型的构造函数，没有默认构造函数的对象也能处于一个默认状态。

`std::variant<>` `std::any` 一样，结果对象是值类型。拷贝的时候是深拷贝，创建了一个完全独立的对象。如果没有包含值，那么拷贝 `std::optional<>` 开销很低，如果有包含值，那么拷贝 `std::optional<>` 的成本和直接拷贝对象一样。`std::optional<>` 支持移动语义。

## Using `std::optional<>`
`std::optional<>` 表示的是可空的任意类型。实例可以是成员、参数和返回值。

### Optional Return Values
下面是使用 `std::optional<>` 作为返回类型的例子。`asInt()` 将传入的字符串转换成整数。由于可能不成功，因此使用 `std::optional<>` 作为返回类型，这样可以表示没有整数这个语义，而不是返回一个特殊约定的整数或者抛出异常。
```cpp
#include <optional>
#include <string>
#include <iostream>

// convert string to int if possible:
std::optional<int> asInt(const std::string &s)
{
    try
    {
        return std::stoi(s);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

int main()
{
    for (auto s : {"42", " 077", "hello", "0x33"})
    {
        // try to convert s to int and print the result if possible:
        std::optional<int> oi = asInt(s);
        if (oi)
        {
            std::cout << "convert '" << s << "' to int: " << *oi << "\n";
        }
        else
        {
            std::cout << "can't convert '" << s << "' to int\n";
        }
    }
}
```
还可以如下实现这个函数
```cpp
std::optional<int> asInt(const std::string &s)
{
    std::optional<int> ret; // initially no value
    try
    {
        ret = std::stoi(s);
    }
    catch (...)
    {
    }

    return ret;
}
```
`oi` 的类型是 `std::optional<int>`，直接使用对象可以作为一个布尔表达式，通过解引用的方式访问数据。
```cpp
if (oi)
{
    std::cout << "convert '" << s << "' to int: " << *oi << "\n";
}
```
下面是另一种使用返回结果，`has_value()` 表示是否有值，`value()` 访问这个值。`value()` 比解引用 `*` 更安全，如果没有值会抛出异常。`*` 仅仅用于确定有一个值，否则是未定义行为，依赖于当前内存内容。
```cpp
std::optional<int> oi = asInt(s);
if (oi.has_value())
{
    std::cout << "convert '" << s << "' to int: " << oi.value() << "\n";
}
```

### Optional Arguments and Data Members
下面这个例子展示的如何使用 `std::optional<>` 传递参数和作为成员使用。
```cpp
#include <string>
#include <optional>
#include <iostream>

class Name
{
private:
    std::string first;
    std::optional<std::string> middle;
    std::string last;

public:
    Name(std::string f,
         std::optional<std::string> m,
         std::string l)
        : first{std::move(f)}, middle{std::move(m)}, last{std::move(l)}
    {
    }

    friend std::ostream &operator<<(std::ostream &strm, const Name &n)
    {
        strm << n.first << ' ';
        if (n.middle)
        {
            strm << *n.middle << ' ';
        }
        return strm << n.last;
    }
};

int main()
{
    Name n{"Jim", std::nullopt, "Knopf"};
    std::cout << n << '\n';

    Name m{"Donald", "Ervin", "Knuth"};
    std::cout << m << '\n';
}
```
类 `Name` 表示一个名字，由名字、可选的中间名字和姓组成。如果没有中间名，传递 `std::nullopt` 给 `middle`，这和使用空字符串表示没有中间名是不一样的。

这里有一个最佳实践，对于值语义，构造函数应该声明为按值传递，然后移动到成员变量。

`std::optional<>` 也改变了使用 `middle` 的语义，使用 `n.middle` 作为布尔表达式检查是否有中间名，如果有，使用 `*n.middle` 访问数据。

另一种访问数据的方式是使用函数 `value_or()`，可以指定一个默认值，当没有值时，返回默认值，比如可以这样使用
```cpp
std::cout << middle.value_or(""); // print middle name or nothing
```

## `std::optional<>` Types and Operations
### `std::optional<>` Types
在头文件 `<optional>`，C++ 标准库如下定义了 `std::optional<>`。
```cpp
namespace std
{
    template <typename T>
    class optional;
}
```
除此之外，还定义了

* `std::nullopt_t` 类型的 `nullopt` 可以作为没有值的可选对象的“值”。
* 直接继承自 `std::exception` 的 `std::bad_optional_access`，会在没有值的但是访问了情况下抛出。

### `std::optional<>` Operations
`std::optional<>` 支持的操作如下所示。

| Operation | Effect |
|--|--|
| constructors | Creates an optional object (might or  |might not call constructor for contained type) |
| `make_optional<>()` | Creates an optional object initialized by the passed value(s) |
| destructor | Destroys an optional object |
| `=` | Assigns a new value |
| `emplace()` | Assigns a new value to the contained type |
| `reset()` | Destroys any value (makes the object empty) |
| `has_value()` | Returns whether the object has a value |
| conversion to `bool` | Returns whether the object has a value |
| `*` | Value access (undefined behavior if no value) |
| `->` | Access to member of the value (undefined behavior if no value) |
| `value()` | Value access (exception if no value) |
| `value_or()` | Value access (fallback argument if no value) |
| `swap()` | Swaps values between two objects |
| `==`, `!=`, `<`, `<=`, `>`, `>=` | Compares optional objects |
| `hash<>` | Function object type to compute hash values |

#### Construction
可以构造一个没有值的可选对象，此时必须指定包含的类型，不会调用包含类型的任何构造函数。
```cpp
std::optional<int> o1;
std::optional<int> o2(std::nullopt);
```

可以传入值来初始化一个可选对象。由于有推导规则，可以不指定包含类型。
```cpp
std::optional o3{42};      // deduces optional<int>
std::optional o4{"hello"}; // deduces optional<const char*>

using namespace std::string_literals;
std::optional o5{"hello"s}; // deduces optional<string>
```

初始化多个参数的可选对象，可以传入已经存在的对象或者是以 `std::in_place` 开始的一系列参数（此时无法做类型推导）
```cpp
std::optional o6{std::complex{3.0, 4.0}};
std::optional<std::complex<double>> o7{std::in_place, 3.0, 4.0};
```
后者的好处是省略了临时对象的构造。通过这种形式，可以传递一个初始化列表外加额外的参数，如果类型不一样，需要使用 `std::initializer_list<>`。
```cpp
// initialize set with lambda as sorting criterion:
auto sc = [](int x, int y)
{
    return std::abs(x) < std::abs(y);
};
std::optional<std::set<int, decltype(sc)>> o8{std::in_place,
                                              {4, 8, -7, -2, 0, 5},
                                              sc};

std::optional<std::set<int, decltype(sc)>> o8{std::in_place,
                                              std::initializer_list<int>{4, 5L},
                                              sc};
```

如果包含的类型支持拷贝，那么可选对象也支持拷贝。不过，如果包含类型可以用可选对象构造的话，此时是用这个可选对象构造一个新对象而不是拷贝可选对象。
```cpp
std::optional o9{"hello"};          // deduces optional<const char*>
std::optional<std::string> o10{o9}; // OK

std::optional<int> o11;
std::optional<std::any> o12{o11}; // o12 contains an any object of an empty optional int
```

标准库提供了便利的 `make_optional<>()` 函数，可以用一个或多个（无需 `std::in_place`）参数初始化一个可选对象。和其他 `make` 类的函数一样，参数会退化。
```cpp
auto o13 = std::make_optional(3.0);     // optional<double>
auto o14 = std::make_optional("hello"); // optional<const char*>
auto o15 = std::make_optional<std::complex<double>>(3.0, 4.0);
```
没有构造函数接受一个参数，然后根据值确定用 `nullopt` 或者一个有效值来构造可选对象，此时可以用 `?:` 表达式。根据类模板参数推导规则，`pos->second` 的类型决定了 `o16` 的类型是 `std::optional<std::string>`。对于 `std::nullopt` 而言，类型推导是不适用的，但是 `:` 前后的类型要一致，所以 `std::nullopt` 转换成了 `std::optional<std::string>`。
```cpp
std::multimap<std::string, std::string> englishToGerman;

auto pos = englishToGerman.find("wisdom");
auto o16 = pos != englishToGerman.end()
               ? std::optional{pos->second}
               : std::nullopt;
```

#### Accessing the Value
检查可选对象是否有值，可以调用 `has_value()` 或将其自身作为布尔表达式。
```cpp
std::optional o{42};

if (o) // true
if (!o) // false
if (o.has_value()) // true
```
没有 I/O 操作，因为没有值时，不确定要输出是什么。
```cpp
std::cout << o; // ERROR
```
解引用 `operator*` 可以访问底层的对象，`->` 可以访问对象的成员。
```cpp
std::optional o{std::pair{42, "hello"}};

auto p = *o;           // initializes p as pair<int,string>
std::cout << o->first; // prints 42
```
这些操作要求有值，如果没有值的话，行为未定义。下面第二次输出是未定义行为。可能还是会输出 `hello`，因为内存没有变化，但是不应该依赖于这一点。在不确定的时候，先检查再使用。或者使用之前提到的 `value()` 函数，当没有值的时候会抛异常 `std::bad_optional_access`。
```cpp
std::optional<std::string> o{"hello"};

std::cout << *o; // OK: prints ”hello”

o = std::nullopt;
std::cout << *o; // undefined behavior

if (o)
    std::cout << *o; // OK (might output nothing)

std::cout << o.value(); // OK (throws if no value)
```
