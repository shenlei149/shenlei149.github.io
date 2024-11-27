## Nested Namespaces
嵌套命名空间。下面两种写法是等价的。
```cpp
namespace A::B::C
{
}

namespace A
{
    namespace B
    {
        namespace C
        {
        }
    }
}
```
目前还不支持嵌套 `inline` 命名空间，因为不能确定 `inline` 作用于最后一个命名空间还是全部的命名空间。

## Defined Expression Evaluation Order
很多代码直觉上看似乎正确，但是其实行为是未定义的。比如下面的代码。
```cpp
std::string s = "I heard it even works if you don't believe";

s.replace(0, 8, "").replace(s.find("even"), 4, "sometimes").replace(s.find("you don't"), 9, "I");
```
通常会假设这段代码先将前八个字符删除，然后将 `even` 替换成 `sometimes`，最后将 `you don't` 替换成 `I`。结果如下
```
it sometimes works if I believe
```
但是其结果是没有保证的。原因在于 `find()` 被调用的时机，只要在需要其返回值的之前调用即可，可能再第一个 `replace` 调用之前就全部调用了，也可能其他时机，所以结果可能是下面几种情况
```
it sometimes works if I believe
it sometimes workIdon't believe
it even worsometiIdon't believe
it even worsometimesf youIlieve
```
另一个例子如下：
```cpp
std::cout << f() << g() << h();
```
一般会假设先调用 `f()` 再调用 `g()`，最后调用 `g()`，不过这个假设是不正确的。它们可以被任意顺序调用，因此如果返回结果依赖于调用顺序的话，结果可能有变化。

在 C++17 之前，下面代码的行为是未定义的。
```cpp
i = 0;
std::cout << ++i << ' ' << --i << '\n';
```
或许会输出 1 0，也可能结果是 0 -1 或者 0 0。这与使用 `int` 或者自定义类型无关。

为了消除这些不确定性行为，对于某些操作定义其求值顺序。

下面这些操作，会在 `e2` 求值之前完成对 `e1` 的求值。即顺序是从左向右的。
```
e1 [ e2 ]
e1 . e2
e1 .* e2
e1 ->* e2
e1 << e2
e1 >> e2
```
不过同一个函数的不同参数之间求值顺序仍然是不确定的。下面的例子中，`e1` 会在 `a1` `a2` `a3` 求值之前完成求值，但是 `a1` `a2` `a3` 的求值顺序是不确定的。
```cpp
e1.f(a1,a2,a3)
```

下面这些操作，会在 `e2` 求值之前完成对右边 `e1` 的求值。
```
e2 = e1
e2 += e1
e2 *= e1
```

对于 `new` 操作，分配内存的操作会在 `e` 求值之前完成。新的对象的初始化操作会在第一次使用它之前完成。
```cpp
new Type(e)
```

这些保证对基本类型和自定义类型都试用。

所以从 C++17 开始，下面代码的输出是确定的。
```cpp
std::string s = "I heard it even works if you don't believe";
s.replace(0, 8, "").replace(s.find("even"), 4, "always").replace(s.find("don't believe"), 13, "use C++17");
```
输出是
```cpp
it always works if you use C++17
```
下面的输出一定是 1 0。
```cpp
i = 0;
std::cout << ++i << ' ' << --i << '\n';
```
不过很多其他操作的行为还是未定义的。比如下面最右侧的 `i` 的值可能是自增前或者是自增后的值。
```cpp
i = i++ + i; // still undefined behavior
```

#### 向后不兼容
新的求值顺序可能会影响现有程序的输出。比如下面的例子。
```cpp
#include <iostream>
#include <vector>

void print10elems(const std::vector<int> &v)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "value: " << v.at(i) << '\n';
    }
}

int main()
{
    try
    {
        std::vector<int> vec{7, 14, 21, 28};
        print10elems(vec);
    }
    catch (const std::exception &e)
    { // handle standard exception
        std::cerr << "EXCEPTION: " << e.what() << '\n';
    }
    catch (...)
    { // handle any other exception
        std::cerr << "EXCEPTION of unknown type\n";
    }
}
```
由于 `vector<>` 只有 4 个元素，`print10elems` 中 `at()` 会越界访问抛出异常。

在 C++17 之前，输出可能如下
```
value: 7
value: 14
value: 21
value: 28
EXCEPTION: ...
```
这是因为允许在 `std::cout << "value: "` 之前先调用 `at()` 抛出异常。不过 C++17 开始输出就是确定的了。
```
value: 7
value: 14
value: 21
value: 28
value: EXCEPTION: ...
```

## Relaxed Enum Initialization from Integral Values
从 C++17 开始，对于有底层类型的枚举，可以直接使用整数初始化。如果有范围限定的枚举，有无底层类型都行，因为有默认类型；对于无范围限定的枚举，必须指定底层类型。
```cpp
// unscoped enum with underlying type:
enum MyInt : char
{
};
MyInt i1{42};    // OK since C++17 (ERROR before C++17)
MyInt i2 = 42;   // still ERROR
MyInt i3(42);    // still ERROR
MyInt i4 = {42}; // still ERROR

// scoped enum with default underlying type:
enum class Weekday
{
    mon,
    tue,
    wed,
    thu,
    fri,
    sat,
    sun
};
Weekday s1{0};    // OK since C++17 (ERROR before C++17)
Weekday s2 = 0;   // still ERROR
Weekday s3(0);    // still ERROR
Weekday s4 = {0}; // still ERROR

// scoped enum with specified underlying type:
enum class Weekday : char
{
    mon,
    tue,
    wed,
    thu,
    fri,
    sat,
    sun
};
Weekday s1{0};    // OK since C++17 (ERROR before C++17)
Weekday s2 = 0;   // still ERROR
Weekday s3(0);    // still ERROR
Weekday s4 = {0}; // still ERROR

// unscoped enum without underlying type:
enum Flag
{
    bit1 = 1,
    bit2 = 2,
    bit3 = 4
};
Flag f1{0}; // still ERROR
```

这里不能有窄化值发生，所以不能用浮点数去初始化。
```cpp
enum MyInt : char
{
};
MyInt i5{42.2}; // still ERROR
```
这个功能的动机是可以自定义一个正数类型，类似上面的 `MyInt`，指定顶层类型的枚举。没有这个功能，初始化自定义对象就必须使用转换。

C++17 的 `std::byte` 就使用了这个功能。

## Fixed Direct List Initialization with `auto`
C++11 引入的大括号统一初始化在配合 `auto` 的时候，会有不符合直接的地方。
```cpp
int x{42};       // initializes an int
int y{1, 2, 3};  // ERROR
auto a{42};      // initializes a std::initializer_list<int>
auto b{1, 2, 3}; // OK: initializes a std::initializer_list<int>
```

直接初始化（`direct list initialization`），不使用等号 `=`，的行为得到了修复。
```cpp
int x{42};       // initializes an int
int y{1, 2, 3};  // ERROR
auto a{42};      // initializes an int now
auto b{1, 2, 3}; // ERROR now
```
注意，这是一个破坏性修改（`breaking change`）。

所以现在拷贝初始化（`copy list initialization`），使用等号 `=`，的行为还是初始化为 `std::initializer_list<>`，这与直接初始化有了很大的不同，这也是推荐使用直接初始化的原因。
```cpp
auto c = {42};      // still initializes a std::initializer_list<int>
auto d = {1, 2, 3}; // still OK: initializes a std::initializer_list<int>

auto a{42};    // initializes an int now
auto c = {42}; // still initializes a std::initializer_list<int>
```

## Hexadecimal Floating-Point Literals
C++17 允许使用十六进制描述浮点数字面量，有的时候使用十进制不能精确表示，那么这个时候此功能就很有用了。
```cpp
#include <iostream>
#include <iomanip>
int main()
{
    // init list of floating-point values:
    std::initializer_list<double> values{
        0x1p4,       // 16
        0xA,         // 10
        0xAp2,       // 40
        5e0,         // 5
        0x1.4p+2,    // 5
        1e5,         // 100000
        0x1.86Ap+16, // 100000
        0xC.68p+2,   // 49.625
    };

    // print all values both as decimal and hexadecimal value:
    for (double d : values)
    {
        std::cout << "dec: " << std::setw(6) << std::defaultfloat << d
                  << " hex: " << std::hexfloat << d << '\n';
    }
}
```
新的表示方法是

* 有效数字/尾数使用十六进制表示
* 指数使用十进制表示，基底为 2

`0xAp2` 表示十进制的 $40=10\times 2^2$。`0x1.4p+5`，是 $1.25\times 2^5$，其中 `0.4` 是十六进制，对应十进制的 `0.25`。

上述代码输出结果是
```
dec: 16 hex: 0x1p+4
dec: 10 hex: 0x1.4p+3
dec: 40 hex: 0x1.4p+5
dec: 5 hex: 0x1.4p+2
dec: 5 hex: 0x1.4p+2
dec: 100000 hex: 0x1.86ap+16
dec: 100000 hex: 0x1.86ap+16
dec: 49.625 hex: 0x1.8dp+5
```

## UTF-8 Character Literals
C++11 引入了以 `u8` 为前缀的 UTF-8 字符串字面量，但是不能作用于字符字面量，C++17 弥补这一空白。
```cpp
auto c = u8'6'; // character 6 with UTF-8 encoding value
```
`u8` 只适用于单个字符且这个字符占一个字节，下面的例子中，`¨o` 是德文中的字符，两个字节，是不允许的。
```cpp
char c = u8'¨o';
```

## Exception Specifications as Part of the Type
从 C++17 开始，`noexcept` 是函数类型的一部分，所以下面两个函数类型不同。
```cpp
void fMightThrow();
void fNoexcept() noexcept; // different type
```
在此之前，可以用可能会抛出异常的函数给定义为不能抛出异常的函数指针赋值，这样可能会有运行时的问题，现在不能这么做了。
```cpp
void (*fp)() noexcept; // pointer to function that doesn't throw
fp = fNoexcept;        // OK
fp = fMightThrow;      // ERROR since C++17
```
不过反之是可行的，即不抛出异常的函数给可能抛出异常的函数指针赋值。
```cpp
void (*fp2)();     // pointer to function that might throw
fp2 = fNoexcept;   // OK
fp2 = fMightThrow; // OK
```
只有 `noexcept` 不同的重载是不允许的，这和只有返回类型不同类似。
```cpp
void f3();
void f3() noexcept; // ERROR
```
其他规则不受影响。比如不能忽略基类的 `noexcept`。下面这个例子中，不写 `override` 也无法编译，因为覆盖一个函数不能异常更宽松。
```cpp
class Base
{
public:
    virtual void foo() noexcept;
};

class Derived : public Base
{
public:
    void foo() override; // ERROR: does not override
};
```

#### Using Conditional Exception Specifications
可以使用带条件的 `noexcept`，那么函数是否会抛出异常取决于条件是否满足。
```cpp
void f1();
void f2() noexcept;
void f3() noexcept(sizeof(int) < 4);  // same type as either f1() or f2()
void f4() noexcept(sizeof(int) >= 4); // different type to f3()
```
`f3()` 是否抛出异常取决于 `sizeof(int)` 与 4 那个大，如果前者小，条件为真，与 `f2()` 类型一样，否则和 `f1()` 的类型一样。

不过 `f3()` 和 `f4()` 的条件恰好相反，所以他俩的类型总是不一样。

旧的不抛出异常的写法仍旧有效但是被废弃了。
```cpp
void f5() throw(); // same as void f5() noexcept but deprecated
```
带参数的动态异常声明不再有效。
```cpp
void f6() throw(std::bad_alloc); // ERROR: invalid since C++17
```

#### Consequences for Generic Libraries
`noexcept` 作为函数声明的一部分这个修改对泛型库有影响。比如下面的代码 C++14 合法但是 C++17 就无法编译了。由于 `f1()` 和 `f2()` 不是一个类型，导致 `call` 的模板参数 `T` 无法推导出合理的类型了。
```cpp
#include <iostream>

template <typename T>
void call(T op1, T op2)
{
    op1();
    op2();
}

void f1()
{
    std::cout << "f1()\n";
}

void f2() noexcept
{
    std::cout << "f2()\n";
}

int main()
{
    call(f1, f2); // ERROR since C++17
}
```
修复方式是声明两个模板参数。
```cpp
template <typename T1, typename T2>
void call(T1 op1, T2 op2)
{
    op1();
    op2();
}
```
现在如果想重载所有的函数类型，数量需要多一倍！比如标准库 type traits `std::is_function<>`，主模版定义如下，表示 `T` 不是函数
```cpp
// primary template (in general type T is no function):
template <typename T>
struct is_function : std::false_type
{
};
```
对于函数类型，有一系列的偏特化。C++17 有 24 个，因为参数可以加 `const` 和 `volatile` 修饰，还需要处理左值和右值引用，另外还需要处理可变参数。
```cpp
// partial specializations for all function types:
template <typename Ret, typename... Params>
struct is_function<Ret(Params...)> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) const> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) &> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) const &> : std::true_type
{
};
```
现在又多了 24 个，因为需要考虑 `noexcept`。
```cpp
// partial specializations for all function types with noexcept:
template <typename Ret, typename... Params>
struct is_function<Ret(Params...) noexcept> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) const noexcept> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) & noexcept> : std::true_type
{
};

template <typename Ret, typename... Params>
struct is_function<Ret(Params...) const & noexcept> : std::true_type
{
};
```

如果类库没有针对 `noexcept` 实现重载，那么如果传入带 `noexcept` 的函数或者函数指针，将无法编译。

## Single-Argument `static_assert`
从 C++17 开始 `static_assert()` 的错误信息是可选参数，这样错误信息完全依赖于实现。
```cpp
#include <type_traits>
template <typename T>
class C
{
    // OK since C++11:
    static_assert(std::is_default_constructible<T>::value,
                  "class C: elements must be default-constructible");

    // OK since C++17:
    static_assert(std::is_default_constructible_v<T>);
};
```

## Preprocessor Condition `__has_include`
C++17 扩展了预处理指令，`__has_include` 测试一个头文件是否能被包含。当相应的 `#include` 有效时，`__has_include(...)` 的条件求值结果是 1（`true`）。其他因素不会影响结果，比如该头文件是否被包含。
```cpp
#if __has_include(<filesystem>)
#include <filesystem>
#define HAS_FILESYSTEM 1
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
#define HAS_FILESYSTEM 1
#define FILESYSTEM_IS_EXPERIMENTAL 1
#elif __has_include("filesystem.hpp")
#include "filesystem.hpp"
#define HAS_FILESYSTEM 1
#define FILESYSTEM_IS_EXPERIMENTAL 1
#else
#define HAS_FILESYSTEM 0
#endif
```

这里只能验证文件存在，但是不能保证内存是符合预期的，可以是空，或者是无效内容。

`__has_include` 是预处理指令，不能用于代码中的条件表达式。
```cpp
if (__has_include(<filesystem>) { // ERROR
}
```
