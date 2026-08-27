用于对象时，`constexpr` 是 `const` 的加强版；用于函数时，其意义就大不相同了。本节主要讲解它的含义。

`constexpr` 表明一个对象是 `const` 的，且用在其初始化中的值在编译期已知。当应用于函数时，不能想当然地认为函数会返回 `const` 对象，也不能认为其返回值一定能在编译期确定。注意，这是一个特性。

我们先讨论 `constexpr` 对象，它们的值在编译期确定。

编译期确定的值享有特权。它们可以被放入只读存储空间，这对于嵌入式系统非常有用。更广泛的应用是，编译期确定的常量可以用于需要整型常量表达式（`integral constant expression`）的上下文中，比如数组大小、整型模板参数（如 `std::array` 的长度）、枚举器初始值和对齐说明符等。如果需要在这些上下文中使用，通常应将变量声明为 `constexpr`，这样编译器会保证其值在编译期确定。
```cpp
int sz;                            // non-constexpr variable
constexpr auto arraySize1 = sz;    // error! sz's value not known at compilation
std::array<int, sz> data1;         // error! same problem
constexpr auto arraySize2 = 10;    // fine, 10 is a compile-time constant
std::array<int, arraySize2> data2; // fine, arraySize2 is constexpr
```
使用 `const` 不能保证同样的事情，因为 `const` 变量不要求在编译期初始化。
```cpp
int sz;                          // as before
const auto arraySize = sz;       // fine, arraySize is const copy of sz
std::array<int, arraySize> data; // error! arraySize's value not known at compilation
```
简而言之，`constexpr` 对象都是 `const` 的，但是反之则不然。`const` 对象未必在编译期初始化；如果一个值应在编译期初始化并用于需要编译期常量的上下文，就应使用 `constexpr`。

将 `constexpr` 作用于函数时，如果实参的值在编译期确定，那么调用结果也可在编译期确定；如果实参的值只能在运行时得知，函数调用就会在运行时求值。
* `constexpr` 函数可以用于需要编译期常量的地方。如果传入的实参都是编译期确定的常量，调用结果就是编译期常量；否则，在这种上下文中代码无法通过编译。
* 当传入 `constexpr` 函数的一个或多个参数无法在编译期确定时，其行为与调用普通函数相同。这意味着不需要分别编写一个用于编译期求值的函数和一个用于运行时求值的函数：一个 `constexpr` 函数就够了。

假定有这样一个需求：一次实验有 $n$ 个变量，每个变量有三种可能性，那么共有 $3^n$ 种结果。假定 $n$ 在编译期可知，那么 $3^n$ 理论上也在编译期可知，使用 `std::array` 就是合理的选择。为此，我们需要一个可以在编译期计算 $3^n$ 的函数。C++ 标准库提供了可用于计算幂的 `std::pow`，不过它有两个问题：其返回值是浮点数，而我们需要整数；并且它不是 `constexpr` 函数，无法在编译期求值。

我们可以自己实现一个。在此之前，我们看一下它是如何声明和使用的。
```cpp
constexpr int                   // pow's a constexpr func
pow(int base, int exp) noexcept // that never throws
{
    // impl is below
}
constexpr auto numConds = 5;               // # of conditions
std::array<int, pow(3, numConds)> results; // results has 3^numConds elements
```
`pow` 前面的 `constexpr` 不是说 `pow` 返回一个 `const` 值，而是说如果 `base` 和 `exp` 都是编译期常量，那么 `pow` 的结果也可以当作编译期常量使用；如果其中任一参数不是编译期常量，那么 `pow` 会在运行时计算结果。这意味着 `pow` 不仅可以用于类似 `std::array` 大小这类需要编译期常量的地方，还可以用于运行时环境。
```cpp
auto base = readFromDB("base");    // get these values
auto exp = readFromDB("exponent"); // at runtime
auto baseToExp = pow(base, exp);   // call pow function at runtime
```
`constexpr` 函数可在编译期被调用并返回结果，因此对此有一些限制；C++11 和 C++14 的限制不同。

C++11 对 `constexpr` 函数的函数体限制很严格：它只能包含一个 `return` 语句。不过可以使用条件运算符 `?:`，也可以使用递归，因此表达能力并不特别弱。C++11 中可以这样实现 `pow`。
```cpp
constexpr int pow(int base, int exp) noexcept
{
    return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
```
除函数式程序员外，很难有人认为这种风格很好。C++14 中对 `constexpr` 函数的限制宽松了很多，可以这么写：
```cpp
constexpr int pow(int base, int exp) noexcept // C++14
{
    auto result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}
```
`constexpr` 函数的参数类型和返回类型必须是字面类型（`literal type`）；这类类型可以参与常量表达式求值。C++11 中，除 `void` 外的内置类型都是字面类型；用户自定义类型在满足相应要求时也可以是字面类型，例如类需要具有 `constexpr` 构造函数，且所有非静态数据成员都必须是字面类型（C++11 还要求析构函数平凡）。
```cpp
class Point
{
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
    {
    }
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    void setX(double newX) noexcept { x = newX; }
    void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};
```
`Point` 的构造函数是 `constexpr` 函数。如果参数在编译期确定，那么 `Point` 对象的值也能在编译期确定，这个对象就可以用 `constexpr` 修饰。
```cpp
constexpr Point p1(9.4, 27.7); // fine, "runs" constexpr ctor during compilation
constexpr Point p2(28.8, 5.3); // also fine
```
类似地，`xValue` 和 `yValue` 两个函数可以是 `constexpr` 函数。如果一个 `Point` 对象在编译期可知，那么对其调用这些成员函数也可在编译期求值。我们可以编写一个 `constexpr` 函数，调用这两个成员函数，并用该函数的结果初始化 `constexpr` 对象。
```cpp
constexpr Point midpoint(const Point &p1, const Point &p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2,  // call constexpr
            (p1.yValue() + p2.yValue()) / 2}; // member funcs
}
constexpr auto mid = midpoint(p1, p2); // init constexpr object
                                       // w/result of constexpr function
```
这意味着我们可以通过调用构造函数、成员函数和非成员函数构造 `mid` 对象；实现可以将其放入只读存储空间。`mid.xValue() * 10` 也可用于要求常量表达式的场景；经适当的整型转换后，还可用于模板实参或枚举器初始值。更进一步，这模糊了运行时计算和编译期计算之间的界限。许多以前必须在运行时进行的计算可以移到编译期；迁移得越多，程序运行得越快，但编译时间也可能越长。

`Point` 的 `setX` 和 `setY` 在 C++11 中不能是 `constexpr` 函数，因为：1）C++11 中，`constexpr` 非静态成员函数隐式为 `const`，而这两个成员函数会修改对象状态；2）C++11 中 `void` 不是字面类型。C++14 放开了这两个限制，所以也可以这样写：
```cpp
class Point
{
public:
    constexpr void setX(double newX) noexcept // C++14
    {
        x = newX;
    }

    constexpr void setY(double newY) noexcept // C++14
    {
        y = newY;
    }
};
```
那么我们可以利用这个性质来实现如下函数：
```cpp
// return reflection of p with respect to the origin (C++14)
constexpr Point reflection(const Point &p) noexcept
{
    Point result;             // create non-const Point
    result.setX(-p.xValue()); // set its x and y values
    result.setY(-p.yValue());
    return result; // return copy of it
}
```
客户端代码可以这样用这些函数：
```cpp
constexpr Point p1(9.4, 27.7); // as above
constexpr Point p2(28.8, 5.3);
constexpr auto mid = midpoint(p1, p2);

// reflectedMid's value is (-19.1 -16.5) and known during compilation
constexpr auto reflectedMid = reflection(mid);
```
如果可能，尽可能使用 `constexpr` 关键字。因为 `constexpr` 对象和 `constexpr` 函数都比非 `constexpr` 对象和函数的应用场景更广泛。

需要注意的是，`constexpr` 是对象和函数接口的一部分。加上 `constexpr` 意味着它们能用于要求编译期常量的上下文。如果之后认为使用 `constexpr` 是一个错误并将其移除，可能会导致客户端代码无法通过编译。（为了调试或优化性能而添加一个 I/O 操作，就可能导致这种问题，因为 I/O 语句通常不能放入 `constexpr` 函数中。）尽可能使用 `constexpr`，表示承诺长期为对象或函数提供这样的约束。

## Things to Remember
* `constexpr` objects are `const` and are initialized with values known at compile time.
* `constexpr` functions can produce compile-time results when called with arguments whose values are known at compile time.
* `constexpr` objects and functions may be used in a wider range of contexts than non-`constexpr` objects and functions.
* `constexpr` is part of an object's or function's interface.