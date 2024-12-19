`std::variant<>` 提供了一种新的 `union` 类，是一种处理异构集合多态的新方法，无需基类和指针就能处理不同数据类型。

## Motivation for `std::variant<>`
继承自 C，C++ 支持 `union` 类型，保存一系列可能的类型中的一个，但是有些缺陷：

* 对象不知道当前值的类型
* 无法保存非普通的值，比如 `std::string`
* 无法继承 `union`

C++ 提供了 `std::variant<>`，一种可区分的 `union`（`closed discriminated union`）

* 知道当前值的类型
* 能够存任意指定类型
* 可以继承

`std::variant<>` 保存一系列候选中的一个值，通常是不同类型。当然，两个候选也可以有相同的类型，当它们有不同的语义时很有用。

`std::variant<>` 的大小是最大的类型的大小加上一个固定开销，表示当前存储的是哪个候选的值。无堆分配。

一般情况下，`std::variant<>` 不能为空，除非指定一个候选表示这个信息。不过有很少的场景，比如设置一个不同类型的值时抛出了一场，此时没有值。

`std::variant<>` 和 `std::optional` `std::any` 一样，是值语义，拷贝是深拷贝，构造一个新的对象，拷贝当前存储的值。开销比直接拷贝当前值类型开销略大，需要找到要拷贝哪个值。支持移动语义。

## Using `std::variant<>`
下面是使用 `std::variant<>` 的例子。`index()`，返回当前哪一个候选被设置了，从零开始计数。初始化和赋值都会选择类型最匹配的候选类型，如果没有精准匹配，那么可能会出现意料之外的结果。
```cpp
#include <variant>
#include <iostream>

int main()
{
    std::variant<int, std::string> var{"hi"}; // initialized with string alternative
    std::cout << var.index() << '\n';         // prints 1
    var = 42;                                 // now holds int alternative
    std::cout << var.index() << '\n';         // prints 0

    try
    {
        int i = std::get<0>(var);                   // access by index
        std::string s = std::get<std::string>(var); // access by type (throws exception here)
        ...
    }
    catch (const std::bad_variant_access &e)
    { // in case a wrong type/index is used
        std::cerr << "EXCEPTION: " << e.what() << '\n';
        ...
    }
}
```
没有空的 `variant`，`variant` 也不能有引用类型、C 风格数据类型、不完整的类型（比如 `void`）。

`variant` 没有空状态，这意味至少调用了一次构造函数。默认构造函数是调用第一个类型的默认构造函数，如果第一个类型没有默认构造函数，会出错。
```cpp
std::variant<std::string, int> var; // => var.index() == 0, value == ""

struct NoDefConstr
{
    NoDefConstr(int i)
    {
        std::cout << "NoDefConstr::NoDefConstr(int) called\n";
    }
};

std::variant<NoDefConstr, int> v1; // ERROR: can't default construct first type
```

#### `std::monostate`
为了解决上述第一个参数没有默认构造函数的问题，C++ 标准库提供了 `std::monostate`，所有类型为 `std::monostate` 的对象状态都一样，比较结果是相等。可以作为 `variant` 的一个候选类型，表示其他类型都没有值。

`std::monostate` 可以作为第一个参数类型，解决上述问题。
```cpp
std::variant<std::monostate, NoDefConstr, int> v2; // OK
std::cout << "index: " << v2.index() << '\n';      // prints 0
```

某种意义上，它也可以表示空状态。

下面是几种检查 `monostate` 的方法，也展示了 `variant` 支持的操作。`get_if<>()` 函数使用指向 `variant` 的指针，如果类型 `T` 正确或者 `index` 正确，返回指向当前值的指针，否则返回 `nullptr`。`get<T>()` 不一样，接受的是 `variant` 的引用，按值返回，如果模板参数不正确，抛出异常。
```cpp
if (v2.index() == 0)
{
    std::cout << "has monostate\n";
}

if (!v2.index())
{
    std::cout << "has monostate\n";
}

if (std::holds_alternative<std::monostate>(v2))
{
    std::cout << "has monostate\n";
}

if (std::get_if<0>(&v2))
{
    std::cout << "has monostate\n";
}

if (std::get_if<std::monostate>(&v2))
{
    std::cout << "has monostate\n";
}
```

和赋另外一个可选类型的值一样，可以赋值为 `monostate` 对象。
```cpp
v2 = 42;
std::cout << "index: " << v2.index() << '\n'; // index: 1

v2 = std::monostate{};
std::cout << "index: " << v2.index() << '\n'; // index: 0
```

#### Deriving from Variants
可以从 `std::variant<>` 继承，比如下面的聚合体。
```cpp
class Derived : public std::variant<int, std::string>
{
};

Derived d = {{"hello"}};
std::cout << d.index() << '\n';      // prints: 1
std::cout << std::get<1>(d) << '\n'; // prints: hello
d.emplace<0>(77);                    // initializes int, destroys string
std::cout << std::get<0>(d) << '\n'; // prints: 77
```

## `std::variant<>` Types and Operations
### `std::variant<>` Types
在头文件 `<variant>` 中定义如下，是一个可变参数模板。
```cpp
namespace std
{
    template <typename... Types>
    class variant;
}
```
除此之外，还定义了

* 类模板 `std::variant_size`
* 类模板 `std::variant_alternative`
* 值 `std::variant_npos`
* 类型 `std::monostate`
* 异常 `std::bad_variant_access`，继承自 `std::exception`。

还有两个变量模板 `std::in_place_type<>` `std::in_place_index<>`，类型分别是 `std::in_place_type_t` `std::in_place_index_t`，定义在头文件 `<utility>`。

### `std::variant<>` Operations
支持以下操作。

| Operation | Effect |
|--|--|
| constructors | Creates a variant object (might call constructor for underlying type) |
| destructor | Destroys a variant object |
| `=` | Assigns a new value |
| `emplace<T>()` | Destroys the old value and assigns a new value to the alternative with type `T` |
| `emplace<Idx>()` | Destroys the old value and assigns a new value to the alternative with index `Idx` |
| `valueless_by_exception()` | Returns whether the variant has no value due to an exception |
| `index()` | Returns the index of the current alternative |
| `swap()` | Swaps values between two objects |
| `==`, `!=`, `<`, `<=`, `>`, `>=` | Compares variant objects |
| `hash<>` | Function object type for computing hash values |
| `holds_alternative<T>()` | Returns whether there is a value for type `T` |
| `get<T>()` | Returns the value for the alternative with type `T` |
| `get<Idx>()` Returns the value for the alternative with index `Idx` |
| `get_if<T>()` | Returns a pointer to the value for the alternative with type `T` or `nullptr` |
| `get_if<Idx>()` | Returns a pointer to the value for the alternative with index Idx or nullptr |
| `visit()` | Performs an operation for the current alternative |

#### Construction
默认情况下，`variant` 调用第一个可选类型的默认构造函数。使用默认值值初始化，因此用 `0` `false` `nullptr` 初始化。
```cpp
std::variant<int, int, std::string> v1; // sets first int to 0, index()==0
```
如果传递了值，选择最匹配的类型。如果有两个类型优先级相同，有歧义，出错。
```cpp
std::variant<long, int> v2{42};
std::cout << v2.index() << '\n'; // prints 1

std::variant<long, long> v3{42};         // ERROR: ambiguous
std::variant<int, float> v4{42.3};       // ERROR: ambiguous
std::variant<int, double> v5{42.3};      // OK
std::variant<int, long double> v6{42.3}; // ERROR: ambiguous

std::variant<std::string, std::string_view> v7{"hello"};               // ERROR: ambiguous
std::variant<std::string, std::string_view, const char *> v8{"hello"}; // OK
std::cout << v8.index() << '\n';                                       // prints 2
```
如果用超过一个值进行初始化，需要使用 `in_place_type` `in_place_index`。使用这个可以解决前面歧义的问题。
```cpp
std::variant<std::complex<double>> v9{3.0, 4.0};    // ERROR
std::variant<std::complex<double>> v10{{3.0, 4.0}}; // ERROR
std::variant<std::complex<double>> v11{std::in_place_type<std::complex<double>>,
                                       3.0, 4.0};
std::variant<std::complex<double>> v12{std::in_place_index<0>, 3.0, 4.0};

std::variant<int, int> v13{std::in_place_index<1>, 77};  // init 2nd int
std::variant<int, long> v14{std::in_place_index<1>, 77}; // init long, not int
std::cout << v14.index() << '\n';                        // prints 1
```
可以传递一个初始化列表，再加上一个额外参数。如果初始化列表类型不同，需要显式的写出来。
```cpp
// initialize variant with a set with lambda as sorting criterion:
auto sc = [](int x, int y)
{
    return std::abs(x) < std::abs(y);
};
std::variant<std::vector<int>,
             std::set<int, decltype(sc)>>
    v15{std::in_place_index<1>,
        {4, 8, -7, -2, 0, 5},
        sc};

std::variant<std::vector<int>,
             std::set<int, decltype(sc)>>
    v15{std::in_place_index<1>,
        std::initializer_list<int>{4, 5L},
        sc};
```

`std::variant<>` 不支持类型推导，也就没有 `make_variant<>()`。由于 `std::variant<>` 的目的处理多个候选值的一个，这个推导没有意义，也没有需求。

如果所有类型都支持拷贝，那么 `variant` 支持拷贝。
```cpp
struct NoCopy
{
    NoCopy() = default;
    NoCopy(const NoCopy &) = delete;
};

std::variant<int, NoCopy> v1;
std::variant<int, NoCopy> v2{v1}; // ERROR
```
