结构化绑定可以用一个对象的成员或元素初始化多个变量。比如
```cpp
struct MyStruct
{
    int i = 0;
    std::string s;
};

MyStruct ms;

// structured binding
auto [u, v] = ms;
```
某种程度上说，分解了某个对象来初始化其他变量。

下面两种初始化语法都是支持的。
```cpp
auto [u2, v2]{ms};
auto [u3, v3](ms);
```
当函数返回结构体或数组的时候，结构化绑定非常有用。
```cpp
MyStruct getStruct()
{
    return MyStruct{42, "hello"};
}

// id and val name i and s of returned struct
auto [id, val] = getStruct();

// using
if (id > 30)
{
    std::cout << val;
}
```
`id` `val` 是返回的结构体中的 `i` `s`。

使用结构化绑定，可以提高可读性。
```cpp
for (const auto &elem : mymap)
{
    std::cout << elem.first << ": " << elem.second << '\n';
}

for (const auto &[key, val] : mymap)
{
    std::cout << key << ": " << val << '\n';
}
```

## in Detail
#### Binding to an Anonymous Entity
结构化绑定的背后有一个匿名的对象，变量指向这个匿名对象的元素或成员。
```cpp
// initialization
auto [u, v] = ms;

auto e = ms;
aliasname u = e.i;
aliasname v = e.s;

// for return values
auto [u, v] = getStruct();

auto e = getStruct();
aliasname u = e.i;
aliasname v = e.s;
```
这里 `u` `v` 是 `ms` 对应成员变量的拷贝，不是引用类型，`decltype(u)` `decltype(v)` 分别是 `i` `s` 的类型。

匿名对象 `e` 的生命周期与结构化绑定生命周期一致，结构化绑定离开作用域，临时对象被销毁。

除非使用引用，否则修改结构化绑定变量的值不影响结构体的成员，反之亦然。
```cpp
MyStruct ms{42, "hello"};
auto [u, v] = ms;
ms.i = 77;
std::cout << u; // prints 42
u = 99;
std::cout << ms.i; // prints 77
```

#### Using Qualifiers
使用 `const` 和引用修饰作用于匿名对象 `e` 这个整体。
```cpp
const auto &[u, v] = ms; // a reference, so that u/v refer to ms.i/ms.s

ms.i = 77;      // affects the value of u
std::cout << u; // prints 77
```
非 `const` 的话，可以双向修改。
```cpp
MyStruct ms{42, "hello"};
auto &[u, v] = ms; // the initialized entity is a reference to ms
ms.i = 77;         // affects the value of u
std::cout << u;    // prints 77
u = 99;            // modifies ms.i
std::cout << ms.i; // prints 99
```
如果结构化绑定一个临时对象的引用，那么临时对象的生命周期扩展到结构化绑定的生命周期。
```cpp
MyStruct getStruct();

const auto &[a, b] = getStruct();
std::cout << "a: " << a << '\n'; // OK
```

#### Qualifiers Do Not Necessarily Apply to the Structured Bindings
下面的 `u` 和 `v` 并不是引用类型，只有匿名对象 `e` 是引用。不过是 `const` 的，无法修改。因此，`u` `v` 的类型分别是 `const int` 和 `const std::string`。
```cpp
const auto &[u, v] = ms; // a reference, so that u/v refer to ms.i/ms.s
```
对齐也是类似的，下面代码中对齐的匿名对象，也就是说第一个变量 `u` 按照 16 字节对齐了，但是 `v` 并没有。
```cpp
alignas(16) auto [u, v] = ms; // align the object, not v
```
即使使用 `auto` 进行结构化绑定，没有类型退化。因为 `auto` 作用于匿名对象整体，而整体不会类型退化。这与 `auto` 初始化不同。
```cpp
struct S
{
    const char x[6];
    const char y[3];
};

S s1{};
auto [a, b] = s1; // a and b get the exact member types: const char[6]

auto a2 = a; // a2 gets decayed type of a
```

#### Move Semantics
移动语义也符合上述规则。

`u` `v` 指向匿名对象，它是 `ms` 的右值引用。此时 `ms` 还持有原来的值。
```cpp
MyStruct ms = {42, "Jim"};
auto &&[v, n] = std::move(ms); // entity is rvalue reference to ms

std::cout << "ms.s: " << ms.s << '\n'; // prints "Jim"
```
不过可以对 `n` 进行移动赋值，此时会将 `ms.s` 移走，这样，被移动的数据处于未定义的状态。
```cpp
std::string s = std::move(n);          // moves ms.s to s
std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints unspecified value
std::cout << "s: " << s << '\n';       // prints "Jim"
```
注意，这与通过移动来初始化结构化绑定不同。这里匿名对象是用 `ms` 移动的值来初始化，`ms` 已经不再持有原来的值了。
```cpp
MyStruct ms = {42, "Jim"};
auto [v, n] = std::move(ms); // new entity with moved-from values from ms

std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints "Jim"
```
后续可以给 `n` 赋予新值，或者移动赋值给其他变量，但是这都与 `ms.s` 无关了。
```cpp
std::string s = std::move(n); // moves n to s
n = "Lara";
std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints "Lara"
std::cout << "s: " << s << '\n';       // prints "Jim"
```

## Where Can Be Used
数据绑定可以用于都是 `public` 的结构体、C 风格数组和类似元组（tuple-like）的对象。

* 非 `static` 的成员都是 `public` 的结构体：每个字段可以给一个名字。
* 原生数组：每一个元素可以给给一个名字。
* 类元组：需要提供以下三个 API。任意类型可以通过提供 API 实现数据绑定。
    * `std::tuple_size<type>::value` 返回元素的个数
    * `std::tuple_element<idx,type>::type` 返回第 `idx` 个元素的类型
    * `get<idx>()` 返回第 `idx` 个元素的值

`std::pair<>`, `std::tuple<>`, `std::array<>` 都提供了这些 API，所以可以进行数据绑定。

必须绑定制定个数的元素，不能跳过某个元素，名字也不能重复。下面使用两次 `_` 这个名字是非法的。
```cpp
auto [_, val1] = getStruct(); // OK
auto [_, val2] = getStruct(); // ERROR: name _ already used
```

当前还不支持嵌套的结构化绑定。

### Structures and Classes
继承的使用会导致无法结构化绑定。所有的数据必须来自同一个类。
```cpp
struct B
{
    int a = 1;
    int b = 2;
};
struct D1 : B
{
};
auto [x, y] = D1{}; // OK
struct D2 : B
{
    int c = 3;
};
auto [i, j, k] = D2{}; // compile-time ERROR
```
还有一个要求是数据的顺序必须稳定，如果 `B` 中的 `a` `b` 交换了顺序，那么 `x` `y` 的值也会发生变化。

`union` 不支持结构化绑定。

### 原生数组
非常直观，不过有两点需要注意，首先，这里少数使用原始数组会发生按值拷贝的场景。第二，入参由于退化成了指针，无法结构化绑定，不过返回类型可以通过引用指定大小信息，所以可以结构化绑定。
```cpp
int arr[] = {47, 11};
auto [x, y] = arr; // x and y are the int elems of a copy of arr
auto [z] = arr;    // ERROR: number of elements doesn't fit

auto getArr() -> int (&)[2]; // getArr() returns reference to raw int array
auto [x, y] = getArr();      // x and y are the int elems of a copy of the returned array
```

### `std::pair`, `std::tuple`, and `std::array`
对 `std::array` 的结构化绑定亦相当直观。
```cpp
std::array<int, 4> getArray();
auto [a, b, c, d] = getArray(); // a,b,c,d name the 4 elements of the copied return value

std::array<int, 4> stdarr{1, 2, 3, 4};
auto &[a, b, c, d] = stdarr;
a += 10; // OK: modifies stdarr[0]

const auto &[e, f, g, h] = stdarr;
e += 10; // ERROR: reference to constant object

auto &&[i, j, k, l] = stdarr;
i += 10; // OK: modifies stdarr[0]

auto [m, n, o, p] = stdarr;
m += 10; // OK: but modifies copy of stdarr[0]

// cannot initialize a non-const reference from a temporary object
auto &[a, b, c, d] = getArray(); // ERROR
```

下面是 `std::tuple` 的结构化绑定例子。
```cpp
std::tuple<char, float, std::string> getTuple();

// a,b,c have types and values of returned tuple
// a: char, b: float, c: std::string
auto [a, b, c] = getTuple();
```

`std::pair` 可以参考本章开头的例子即可。

由于结构化绑定的例子通常是一起声明的，但是不能一起用，所以无法对这些变量做修改。不过如果被赋的值可以赋值给 `std::pair` 或 `std::tuple`，那么可以使用 `std::tie()` 一次性更新所有的值。这在处理有多个返回值的循环的时候非常有用。
```cpp
std::tuple<char, float, std::string> getTuple();
auto [a, b, c] = getTuple();    // a,b,c have types and values of returned tuple
std::tie(a, b, c) = getTuple(); // a,b,c get values of next returned tuple

std::boyer_moore_searcher bmsearch{sub.begin(), sub.end()};
for (auto [beg, end] = bmsearch(text.begin(), text.end());
     beg != text.end();
     std::tie(beg, end) = bmsearch(end, text.end()))
{
    //
}
```

## Providing a Tuple-Like API for Structured Bindings
与 `std::pair` `std::tuple` `std::array` 类似，我们可以为自定义的类型提供类元组 API，使得可以进行结构化绑定的操作。

首先看下只读的实现。假定需要给下面的类提供类元组 API。
```cpp
#include <string>
#include <utility> // for std::move()
class Customer
{
private:
    std::string first;
    std::string last;
    long val;

public:
    Customer(std::string f, std::string l, long v)
        : first{std::move(f)}, last{std::move(l)}, val{v}
    {
    }

    std::string getFirst() const
    {
        return first;
    }

    std::string getLast() const
    {
        return last;
    }

    long getValue() const
    {
        return val;
    }
};
```

那么类元组 API 实现如下。
```cpp
#include "customer1.hpp"
#include <utility> // for tuple-like API

// provide a tuple-like API for class Customer for structured bindings:

// std::tuple_size
template <>
struct std::tuple_size<Customer>
{
    static constexpr int value = 3; // we have 3 attributes
};

// std::tuple_element
// a full specialization
template <>
struct std::tuple_element<2, Customer>
{
    using type = long; // last attribute is a long
};
// a partial specialization
template <std::size_t Idx>
struct std::tuple_element<Idx, Customer>
{
    using type = std::string; // the other attributes are strings
};

// define specific getters:
// a primary function template declaration
template <std::size_t>
auto get(const Customer &c);
// full specializations
template <>
auto get<0>(const Customer &c) { return c.getFirst(); }
template <>
auto get<1>(const Customer &c) { return c.getLast(); }
template <>
auto get<2>(const Customer &c) { return c.getValue(); }
```

完全特化的函数签名需要与声明一致，包括返回类型，所以如下代码无法通过编译。
```cpp
template <std::size_t>
auto get(const Customer &c);
template <>
std::string get<0>(const Customer &c) { return c.getFirst(); }
template <>
std::string get<1>(const Customer &c) { return c.getLast(); }
template <>
long get<2>(const Customer &c) { return c.getValue(); }
```
如果使用 `if constexpr` 上述模板可以改写为
```cpp
template <std::size_t I>
auto get(const Customer &c)
{
    static_assert(I < 3);
    if constexpr (I == 0)
    {
        return c.getFirst();
    }
    else if constexpr (I == 1)
    {
        return c.getLast();
    }
    else
    { // I == 2
        return c.getValue();
    }
}
```
使用结构化绑定
```cpp
#include "structbind1.hpp"
#include <iostream>
int main()
{
    Customer c{"Tim", "Starr", 42};

    auto [f, l, v] = c;

    std::cout << "f/l/v: " << f << ' ' << l << ' ' << v << '\n';

    // modify structured bindings:
    std::string s{std::move(f)};
    l = "Waters";
    v += 10;
    std::cout << "f/l/v: " << f << ' ' << l << ' ' << v << '\n';
    std::cout << "c: " << c.getFirst() << ' '
              << c.getLast() << ' ' << c.getValue() << '\n';
    std::cout << "s: " << s << '\n';
}
```
会调用对应的 `get<>` 函数初始化 `f` `l` `v`。当输出的时候，不会再次调用 `get<>` 函数了。

同时，由于返回的是值，拷贝了一次，所以新的变量与 `c` 完全无关，因此秀爱 `c` 并不会影响 `f` `l` `v`，反之亦然。

整个代码输出如下
```cpp
f/l/v:    Tim Starr 42
f/l/v:     Waters 52
c:        Tim Starr 42
s:        Tim
```

我们也可以对 `std::vector<Customer>` 进行迭代。
```cpp
std::vector<Customer> coll;

for (const auto &[first, last, val] : coll)
{
    std::cout << first << ' ' << last << ": " << val << '\n';
}
```
由于使用了 `const auto&`，所以不会拷贝 `Customer`，但是由于 `first` `last` 按值返回，所以会拷贝字符串。当每一次迭代结束，拷贝的字符串会销毁。

使用 `decltype` 推导是变量自身的类型，与匿名对象修饰无关，因此 `decltype(first)` `decltype(last)` 是 `const std::string` 而不是引用。

现在分析如何实现提供可写的类元组 API。首先，自定义类需要提供可以修改成员变量的 API，即提供返回 `const` 和非 `const` 引用的 API。
```cpp
#include <string>
#include <utility> // for std::move()
class Customer
{
private:
    std::string first;
    std::string last;
    long val;

public:
    Customer(std::string f, std::string l, long v)
        : first{std::move(f)}, last{std::move(l)}, val{v}
    {
    }

    const std::string &firstname() const
    {
        return first;
    }

    std::string &firstname()
    {
        return first;
    }

    const std::string &lastname() const
    {
        return last;
    }

    std::string &lastname()
    {
        return last;
    }

    long value() const
    {
        return val;
    }

    long &value()
    {
        return val;
    }
};
```
下面是类元组 API 的实现。首先 `get<>` 有三个重载，分别用于处理 `const` 对象、非 `const` 对象和可移动对象。另外，为了让接口返回引用，返回类型需要写成 `decltype(auto)`，否则会丢掉引用修饰。
```cpp
#include "customer2.hpp"
#include <utility> // for tuple-like API

// provide a tuple-like API for class Customer for structured bindings:
template <>
struct std::tuple_size<Customer>
{
    static constexpr int value = 3; // we have 3 attributes
};
template <>

struct std::tuple_element<2, Customer>
{
    using type = long; // last attribute is a long
};

template <std::size_t Idx>
struct std::tuple_element<Idx, Customer>
{
    using type = std::string; // the other attributes are strings
};

// define specific getters:
template <std::size_t I>
decltype(auto) get(Customer &c)
{
    static_assert(I < 3);
    if constexpr (I == 0)
    {
        return c.firstname();
    }
    else if constexpr (I == 1)
    {
        return c.lastname();
    }
    else
    { // I == 2
        return c.value();
    }
}

template <std::size_t I>
decltype(auto) get(const Customer &c)
{
    static_assert(I < 3);
    if constexpr (I == 0)
    {
        return c.firstname();
    }
    else if constexpr (I == 1)
    {
        return c.lastname();
    }
    else
    { // I == 2
        return c.value();
    }
}

template <std::size_t I>
decltype(auto) get(Customer &&c)
{
    static_assert(I < 3);
    if constexpr (I == 0)
    {
        return std::move(c.firstname());
    }
    else if constexpr (I == 1)
    {
        return std::move(c.lastname());
    }
    else
    { // I == 2
        return c.value();
    }
}
```

下面是对使用结构化绑定的代码，现在可以真的修改成员变量的值了。
```cpp
#include "structbind2.hpp"
#include <iostream>
int main()
{
    Customer c{"Tim", "Starr", 42};

    auto [f, l, v] = c;

    std::cout << "f/l/v: " << f << ' ' << l << ' ' << v << '\n';

    // modify structured bindings via references:
    auto &&[f2, l2, v2] = c;
    std::string s{std::move(f2)};
    f2 = "Ringo";
    v2 += 10;

    std::cout << "f2/l2/v2: " << f2 << ' ' << l2 << ' ' << v2 << '\n';
    std::cout << "c: " << c.firstname() << ' '
              << c.lastname() << ' ' << c.value() << '\n';
    std::cout << "s: " << s << '\n';
}
```
输出如下
```cpp
f/l/v:    Tim Starr 42
f2/l2/v2: Ringo Starr 52
c:        Ringo Starr 52
s:        Tim
```
