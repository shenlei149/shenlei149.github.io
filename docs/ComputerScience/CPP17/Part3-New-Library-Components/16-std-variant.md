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

#### Accessing the Value
通常，我们使用 `get<>()` `get_if<>` 获取数据。 传入索引或者只出现一次的类型。对于前者而言，如果索引无效、类型无效或有歧义，编译出错。如果索引或者类型指定的值不是当前值，会抛出 `std::bad_variant_exception` 异常。`get_if<>()` 会检查当前值是否被设置，如果没有会返回 `nullptr`，否则会返回指向当前值的指针。
```cpp
std::variant<int, int, std::string> var; // sets first int to 0, index()==0

auto a = std::get<double>(var); // compile-time ERROR: no double
auto b = std::get<4>(var);      // compile-time ERROR: no 4th alternative
auto c = std::get<int>(var);    // compile-time ERROR: int twice

try
{
    auto s = std::get<std::string>(var); // throws exception (first int currently set)
    auto i = std::get<0>(var);           // OK, i==0
    auto j = std::get<1>(var);           // throws exception (other int currently set)
}
catch (const std::bad_variant_access &e)
{ // in case of an invalid access
    std::cout << "Exception: " << e.what() << '\n';
}

if (auto ip = std::get_if<1>(&var); ip != nullptr)
{
    std::cout << *ip << '\n';
}
else
{
    std::cout << "alternative with index 1 not set\n";
}
```

#### Changing the Value
赋值操作和 `emplace()` 都可以修改值。如果有匹配的类型的话，赋值操作直接赋新值，使用 `emplace()` 的话，总是先销毁旧址，再赋新值。
```cpp
std::variant<int, int, std::string> var; // sets first int to 0, index()==0

var = "hello";      // sets string, index()==2
var.emplace<1>(42); // sets second int, index()==1
```
使用 `get<>()` 或 `get_if<>()` 也可以修改值。
```cpp
std::variant<int, int, std::string> var; // sets first int to 0, index()==0

std::get<0>(var) = 77; // OK, because first int already set
std::get<1>(var) = 99; // throws exception (other int currently set)

// if second int set modify it
if (auto p = std::get_if<1>(&var); p)
{
    *p = 42;
}
```

#### Comparisons
对于两个类型相同的 `variant`，候选类型相同且同续，可以使用比较运算符。

* 当前值的索引小的 `variant` 比较小
* 如果索引相同，使用具体类型的比较。`std::monostate` 的值全都相同。
* 两个处于特殊状态 `valueless_by_exception()` 的 `variant` 相同，如果只有一个处于特殊状态，那么特殊状态的比较小。

```cpp
std::variant<std::monostate, int, std::string> v1, v2{"hello"}, v3{42};
std::variant<std::monostate, std::string, int> v4;

v1 == v4    // COMPILE-TIME ERROR
v1 == v2    // yields false
v1 < v2     // yields true
v1 < v3     // yields true
v2 < v3     // yields false

v1 = "hello";
v1 == v2    // yields true

v2 = 41;
v2 < v3     // yields true
```

#### Move Semantics
如果 `std::variant<>` 所有候选类型都支持移动，那么对象本身也支持移动。

如果移动 `std::variant<>` 对象整体，那么状态是拷贝，当前值是移动。被移动的对象的状态不变，但是值未指定。

可以移入或者移出当前值。

#### Hashing
当且仅当所有类型都支持哈希的时候 `variant` 支持哈希。哈希值可能和当前值的哈希值一致，也可能不一致，取决于平台。

### Visitors
可以使用访问器（`visitor`）处理值。访问器必须为每一种候选类型提供无歧义的函数来处理对应类型的值。当访问一个 `variant` 时，调用与 `variant` 当前值最匹配的函数。

#### Using Function Objects as Visitors
如果 `operator()` 不支持所有类型或者有歧义，那么调用 `visit()` 会编译出错。下面的例子能编译成功的原因是 `long double` 比 `int` 更匹配 `double`。
```cpp
#include <variant>
#include <string>
#include <iostream>

struct MyVisitor
{
    void operator()(int i) const
    {
        std::cout << "int: " << i << '\n';
    }

    void operator()(std::string s) const
    {
        std::cout << "string: " << s << '\n';
    }

    void operator()(long double d) const
    {
        std::cout << "double: " << d << '\n';
    }
};

int main()
{
    std::variant<int, std::string, double> var(42);
    std::visit(MyVisitor(), var); // calls operator() for int
    var = "hello";
    std::visit(MyVisitor(), var); // calls operator() for string
    var = 42.7;
    std::visit(MyVisitor(), var); // calls operator() for long double
}
```

访问器也可以修改值，不过只能修改当前值，不能赋值给其他候选类型。另外，这里只区分类型，因此无法对同一个类型有不同的行为。另外，下面的函数应该标记为 `const`，因为是无状态的。
```cpp
struct Twice
{
    void operator()(double &d) const
    {
        d *= 2;
    }

    void operator()(int &i) const
    {
        i *= 2;
    }

    void operator()(std::string &s) const
    {
        s = s + s;
    }
};

std::visit(Twice(), var); // calls operator() for matching type
```

#### Using Generic Lambdas as Visitors
最简单的使用这个功能的方法是传入泛型 lambda，其可以处理任意类型。
```cpp
auto printvariant = [](const auto &val)
{
    std::cout << val << '\n';
};

std::visit(printvariant, var)
```
泛型 lambda 定义了一个闭包类型，如下所示，一个模板成员函数。如果调用是合法的，这里是输出 `<<`，那么 lambda 就可以传递给 `std::visit()`。
```cpp
class CompilerSpecificClosureTypeName
{
public:
    template <typename T>
    auto operator()(const T &val) const
    {
        std::cout << val << '\n';
    }
};
```
也可以使用 lambda 修改值。
```cpp
// double the value of the current alternative:
std::visit([](auto &val)
           { val = val + val; },
           var);

// restore to the default value of the current alternative:
std::visit([](auto &val)
           { val = std::remove_reference_t<decltype(val)>{}; },
           var);
```
还可以使用编译期的 `if` 对不同类型进行处理。下面例子中，`string` 类型使用 `+` 加倍 `string`，其余类型使用 `*=`。
```cpp
auto dblvar = [](auto &val)
{
    if constexpr (std::is_convertible_v<decltype(val),
                                        std::string>)
    {
        val = val + val;
    }
    else
    {
        val *= 2;
    }
};

std::visit(dblvar, var);
```
注意，如果判断条件如下的话不会如期工作，因为参数 `val` 的类型只可能是引用 `int&` `std::string&` `long double&`。
```cpp
if constexpr (std::is_same_v<decltype(val), std::string>)
```

#### Return Values in Visitors
访问器的函数可以返回值，不过要求所有的返回类型都要相同。如果下面的函数不显式声明返回类型，那么无法编译，因为返回类型不同。
```cpp
using IntOrDouble = std::variant<int, double>;

std::vector<IntOrDouble> coll{42, 7.7, 0, -0.7};

double sum{0};
for (const auto &elem : coll)
{
    sum += std::visit([](const auto &val) -> double
                      { return val; },
                      elem);
}
```

#### Using Overloaded Lambdas as Visitors
通过重载器（`overloader`），可以定义一系列的 lambda 来处理不同的类型。

重载器定义如下
```cpp
// ”inherit” all function call operators of passed base types:
template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

// base types are deduced from passed arguments:
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;
```
现在通过 `overload` 定义一系列 lambda 来处理不同可选类型。
```cpp
std::variant<int, std::string> var(42);

std::visit(overload{
               // calls best matching lambda for current alternative
               [](int i)
               { std::cout << "int: " << i << '\n'; },
               [](const std::string &s)
               { std::cout << "string: " << s << '\n'; },
           },
           var);
```
类似的，我们可以修改值。
```cpp
auto twice = overload{
    [](std::string &s)
    { s += s; },
    [](auto &i)
    { i *= 2; },
};

std::variant<int, std::string> var(42);
std::visit(twice, var); // value 42 becomes 84

var = "hi";
std::visit(twice, var); // value "hi" becomes "hihi"
```

### Valueless by Exception
当在赋予新值或者修改旧值的时候发生了异常，`variant` 进入了一种特殊状态，失去了旧值但是还没有获取新值。比如
```cpp
struct S
{
    operator int() { throw "EXCEPTION"; } // any conversion to int throws
};

std::variant<double, int> var{12.2}; // initialized as double
var.emplace<1>(S{});                 // OOPS: throws while set as int
```
此时

* `var.valueless_by_exception()` 返回 `true`
* `var.index()` 返回 `std::variant_npos`

这意味着 `variant` 不再包含值了。

这种情况下有如下保证

* 如果 `emplace()` 抛出异常，那么 `valueless_by_exception()` 可能会返回 `false`。
* `operator=()` 抛出异常，但是修改不改变当前候选项，那么 `index()` 和 `valueless_by_exception()` 保持不变。值的状态依赖于这个类型的异常保证。
* `operator=()` 抛出异常，修改当前候选项，那么或许没有值，`valueless_by_exception()` 或许返回 `true`。这取决于异常发生的时间点。如果发生在实际修改值之前，那么 `variant` 还是有值的。

一般情况下，此时就不应该再继续使用 `variant` 对象了。如果要继续使用，需要判断其状态。
```cpp
std::variant<double, int> var{12.2}; // initialized as double
try
{
    var.emplace<1>(S{}); // OOPS: throws while set as int
}
catch (...)
{
    if (!var.valueless_by_exception())
    {
        ...
    }
}
```

## Polymorphism and Heterogeneous Collections with `std::variant`
`std::variant` 提供了一种新的处理异构集合的多态方法，是一种在封闭类型集合上的运行时多态。

关键在于 `variant<>` 可以持有多个候选类型的值。通过构造 `variant` 的集合，这个集合就是异构的。每一个 `variant` 知道自己持有的值，外加访问器接口，那么就可以在运行时处理不同类型的数据。由于 `variant` 是值语义，因此不需要指针（内存管理）或者虚函数。

### Geometric Objects with `std::variant`
下面是使用 `variant` 管理几何对象的例子。首先为所有可能得类型创建公共类型 `GeoObj`，候选的三个类型可以没有任何关系，无需共同基类，无需虚函数，接口也可以各异。
```cpp
#include <iostream>
#include <variant>
#include <vector>

#include "coord.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "rectangle.hpp"

// common type of all geometric object types:
using GeoObj = std::variant<Line, Circle, Rectangle>;

// create and initialize a collection of geometric objects:
std::vector<GeoObj> createFigure()
{
    std::vector<GeoObj> f;
    f.push_back(Line{Coord{1, 2}, Coord{3, 4}});
    f.push_back(Circle{Coord{5, 5}, 2});
    f.push_back(Rectangle{Coord{3, 3}, Coord{6, 4}});
    return f;
}

int main()
{
    std::vector<GeoObj> figure = createFigure();
    for (const GeoObj &geoobj : figure)
    {
        std::visit([](const auto &obj)
                   {
                       obj.draw(); // polymorphic call of draw()
                   },
                   geoobj);
    }
}

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "coord.hpp"

#include <iostream>

class Circle
{
private:
    Coord center;
    int rad;

public:
    Circle(Coord c, int r)
        : center{c}, rad{r}
    {
    }

    void move(const Coord &c)
    {
        center += c;
    }

    void draw() const
    {
        std::cout << "circle at " << center
                  << " with radius " << rad << '\n';
    }
};

#endif
```
我们在 `createFigure` 中构造了不同的类型，然后放到 `GeoObj` 的 `std::vector` 中。在以前，没有继承和多态是无法做到这一点的。以前的做法是所有的类以 `GeoObj` 为公共基类，然后 `std::vector` 存放基类指针，这就需要管理内存，考虑释放内存的问题，也可以使用智能指针。但是现在不需要这些了。

这里的 `visit()` 使用了一个泛型 lambda 表达式，会为 `GeoObj` 的每一个可能的类型实例化一次，当编译 `visit()` 时，会生成如下三个函数。如果有任何一个无法编译，那么 `visit()` 编译失败。编译成功后，对每个类型调用相应的函数。这不是 `if-else` 链。标准库保证了性能不会依赖于候选类型的个数。
```cpp
[](const Line &obj)
{
    obj.draw(); // call of Line::draw()
}

    [](const Circle &obj)
{
    obj.draw(); // call of Circle::draw()
}

[](const Rectangle &obj)
{
    obj.draw(); // call of Rectangle::draw()
}
```
这里行为和虚函数表一致，可以看作是在 `visit()` 局部有一个虚函数表。这里 `draw()` 不是虚函数。如果不同类型接口不同，可以使用编译期 `if` 或者重载器来处理。

### Other Heterogeneous Collections with `std::variant`
下面是另一个例子。这里能够使用异构元素初始化 `std::vector` 的原因是这些都可以转成 `variant`，如果我们使用 `long` 类型的值，编译器不知道转成 `int` 还是 `double`，就会报错。
```cpp
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <type_traits>

int main()
{
    using Var = std::variant<int, double, std::string>;

    std::vector<Var> values{42, 0.19, "hello world", 0.815};

    for (const Var &val : values)
    {
        std::visit([](const auto &v)
                   {
                    if constexpr(std::is_same_v<decltype(v), const std::string&>) 
                    {
                        std::cout << '"' << v << "\" ";
                    }
                    else
                    {
                        std::cout << v << ' ';
                    } },
                   val);
    }
}
```

迭代每一个元素，使用访问器调用回调函数处理数据。这里使用了编译期 `if` 针对 `string` 类型做了特殊处理。因此输出是
```
42 0.19 "hello world" 0.815
```
通过访问器重载，也能达到相同的效果。不过这里有一个小的陷阱，泛型 lambda 可能比隐式类型转换的优先级要高，导致调用了错误的重载。
```cpp
for (const auto &val : values)
{
    std::visit(overload{[](const auto &v)
                        {
                            std::cout << v << ' ';
                        },
                        [](const std::string &v)
                        {
                            std::cout << '"' << v << "\" ";
                        }},
               val);
}
```

### Comparing variant Polymorphism
下面总结一下 `std::variant<>` 实现异构集合和多态的优缺点。

优点

* 可以用于任意类型，不需要有公共基类（非入侵式）
* 不需要使用指针来实现异构集合
* 不需要有虚函数
* 值语义，无需担心内存问题
* 元素在 `std::vector` 中是连续存放的（使用指针会分布在堆上）

缺点

* 类型的封闭集合，需要在编译期知道所有候选类型
* 所以元素的大小都是体积最大的类型大小，大小差距比较大的时候这是个问题
* 拷贝开销更大一些

一般来说，并不推荐默认使用 `variant` 实现多态。优缺点都很明显。性能也是一个考量，差别很大。没有 `new` `delete` 可以节约时间，但是值传递也会耗费很多时间，所以要以测试为准。

## Special Cases with `std::variant<>`
### Having Both `bool` and `std::string` Alternatives
如果 `std::variant<>` 同时有 `bool` 和 `std::string` 两个候选类型，当使用字符串字面量赋值的时候，可能有意想不到的结果。
```cpp
std::variant<bool, std::string> v;
v = "hi"; // OOPS: sets the bool alternative
std::cout << "index: " << v.index() << '\n';
std::visit([](const auto &val)
           { std::cout << "value: " << val << '\n'; },
           v);
```
一种可能的输出如下。字符串字面量是给 `bool` 类型赋值而不是 `std::string` 类型。不过我使用 g++ 测试了一下，是给 `std::string` 赋值。
```
index: 0
value: true
```
可以使用下面的方式修复这个问题。
```cpp
v.emplace<1>("hello"); // explicitly assign to second alternative

v.emplace<std::string>("hello"); // explicitly assign to string alternative

v = std::string{"hello"}; // make sure a string is assigned

using namespace std::literals; // make sure a string is assigned
v = "hello"s;
```
