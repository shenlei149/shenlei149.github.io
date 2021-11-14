从 C++17 开始，可以使用占位符 `auto` `decltype(auto)` 作为非模板类型的类型，这样可以写出不同类型的非模板类型的泛型代码。

## Using `auto` for Template Parameters
可以使用 `auto` 声明非类型模板参数。比如如下声明，可以用不同类型来初始化非类型模板参数 `N`，但是不能使用不支持的类型来实例化模板。
```cpp
template <auto N>
class S
{
};

S<42> s1;  // OK: type of N in S is int
S<'a'> s2; // OK: type of N in S is char

S<2.5> s3; // ERROR: template parameter type still cannot be double
```
还可以支持类模板类型推到。比如下面的例子，这里可以推导出 `N` 的类型和值。
```cpp
template <typename T, auto N>
class A
{
public:
    A(const std::array<T, N> &)
    {
    }

    A(T (&)[N])
    {
    }
};

A a2{"hello"}; // OK, deduces A<const char, 6> with N being std::size_t

std::array<double, 10> sa1;
A a1{sa1}; // OK, deduces A<double, 10> with N being std::size_t
```
可以修饰 `auto`，比如下面要求参数模板必须是指针。
```cpp
template <const auto *P>
struct S;
```
也可以使用可变参数模板，这样可以用多个参数类型相同或者不同的模板参数来实例化。
```cpp
template <auto... VS>
class HeteroValueList
{
};

template <auto V1, decltype(V1)... VS>
class HomoValueList
{
};

HeteroValueList<1, 2, 3> vals1;      // OK
HeteroValueList<1, 'a', true> vals2; // OK
HomoValueList<1, 2, 3> vals3;        // OK
HomoValueList<1, 'a', true> vals4;   // ERROR
```

### Parameterizing Templates for Characters and Strings
这个特性的一个应用就是可以定义一个模板参数，其可能是一个字符，也可能是一个字符串。比如可以改造使用折叠表达式打印任意个参数的例子，增加一个分隔符，默认是空格。
```cpp
#include <iostream>

template <auto Sep = ' ', typename First, typename... Args>
void print(const First &first, const Args &...args)
{
    std::cout << first;
    auto outWithSep = [](const auto &arg)
    {
        std::cout << Sep << arg;
    };

    (..., outWithSep(args));
    std::cout << '\n';
}

```
下面是一些使用的示例。可以和之前一样调用，默认使用空格。也可以指定分割字符。还可以指定无链接的字符串字面量。最后一个例子是使用其他可以打印的其他类型做模板参数。
```cpp
std::string s{"world"};
print(7.5, "hello", s); // prints: 7.5 hello world

print<'-'>(7.5, "hello", s); // prints: 7.5-hello-world

static const char sep[] = ", ";
print<sep>(7.5, "hello", s); // prints: 7.5, hello, world

print<-11>(7.5, "hello", s); // prints: 7.5-11hello-11world
```

### Defining Metaprogramming Constants
`auto` 模版参数的另一个应用是简化编译期常量的定义。下面是简化的例子。
```cpp
template <typename T, T v>
struct constant
{
    static constexpr T value = v;
};
using i = constant<int, 42>;
using c = constant<char, 'x'>;
using b = constant<bool, true>;

// now
template <auto v>
struct constant
{
    static constexpr auto value = v;
};
using i = constant<42>;
using c = constant<'x'>;
using b = constant<true>;
```

也可以应用于可变参数模版，甚至声明一系列类型不同的常量，类似一个 `tuple`。
```cpp
template <typename T, T... Elements>
struct sequence
{
};
using indexes = sequence<int, 0, 3, 4>;

// now
template <auto... Elements>
struct sequence
{
};
using indexes = sequence<0, 3, 4>;
using tuple = sequence<0, 'h', true>;
```

## Using `auto` as Variable Template Parameter
使用 `auto` 可以实现变量模板（`variable template`）。

比如下面的例子，变量模板 `arr` 的定义，其出现在头文件中。在每一个编译单元中，所有的 `arr<int,10>` 都指向同一个对象，与 `arr<long,10>` 和 `arr<int,10u>` 指向不同的对象。

下面是头文件中的定义。
```cpp
#ifndef VARTMPLAUTO_HPP
#define VARTMPLAUTO_HPP

#include <array>

template <typename T, auto N>
std::array<T, N> arr{};

void printArr();

#endif // VARTMPLAUTO_HPP
```
包含 `main` 的文件如下
```cpp
#include "vartmplauto.hpp"

int main()
{
    arr<int, 5>[0] = 17;
    arr<int, 5>[3] = 42;
    arr<int, 5u>[1] = 11;
    arr<int, 5u>[3] = 33;
    printArr();
}
```

实现 `printArr` 的文件如下
```cpp
#include "vartmplauto.hpp"
#include <iostream>

void printArr()
{
    std::cout << "arr<int,5>: ";
    for (const auto &elem : arr<int, 5>)
    {
        std::cout << elem << ' ';
    }

    std::cout << "\narr<int,5u>: ";
    for (const auto &elem : arr<int, 5u>)
    {
        std::cout << elem << ' ';
    }

    std::cout << '\n';
}
```
两个编译单元指向同一个对象，因此输出如下
```
arr<int,5>: 17 0 0 42 0
arr<int,5u>: 0 11 0 33 0
```
使用同样的方法，我们可以定义任意类型的常量，类型可以通过初始值推导出来。
```cpp
template <auto N>
constexpr auto val = N; // OK since C++17

auto v1 = val<5>;    // v1 == 5, v1 is int
auto v2 = val<true>; // v2 == true, v2 is bool
auto v3 = val<'a'>;  // v3 == 'a', v3 is char

std::is_same_v<decltype(val<5>), int>;       // yields false
std::is_same_v<decltype(val<5>), const int>; // yields true
std::is_same_v<decltype(v1), int>;           // yields true (because auto decays)
```

## Using `decltype(auto)` as Template Parameter
我们还可以使用占位符 `decltype(auto)` 作为模板参数。不过 `decltype` 推导类型有特殊的规则。对于表达式而言，`decltype(auto)` 的结果依赖于表达式的类型。

* prvalue，比如临时值，结果是 `type`。
* lvalue，比如有名的对象，结果是 `type&`。
* xvalue，比如 `std::move` 的对象，结果是 `type&&`。

也就是说，结果很容易是引用类型，因此，可能有意想不到的结果。
```cpp
#include <iostream>

template <decltype(auto) N>
struct S
{
    void printN() const
    {
        std::cout << "N: " << N << '\n';
    }
};

static const int c = 42;
static int v = 42;

int main()
{
    S<c> s1;   // deduces N as const int 42
    S<(c)> s2; // deduces N as const int& referring to c
    s1.printN();
    s2.printN();

    S<(v)> s3; // deduces N as int& referring to v
    v = 77;
    s3.printN(); // prints: N: 77
}
```
