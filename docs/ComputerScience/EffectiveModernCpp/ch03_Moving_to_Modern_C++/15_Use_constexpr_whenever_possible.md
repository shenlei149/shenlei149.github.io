`constexpr` 用于对象时，是 `const` 的加强版，当用于函数时，意义就大不相同了。这一节主要讲解其含义。

`constexpr` 表明一个对象是 `const` 的，且在编译期确定。当应用于函数时，不能想当然认为函数会返回 `const` 对象，也不能再编译期确定其返回值。注意这是一个特性。

我们先讨论 `constexpr` 对象，它的值是在编译期确定的。

编译期确定的值享有特权。可以放到只读存储空间，这对于嵌入式而言是非常有用的。更广泛的应用是编译期确定的常量可以应用于需要整型常量表达式（`integral constant expression`）的上下文中，比如数组的大小，整数模版参数（`std::array` 的长度），枚举，对齐修饰符等等。如果需要在这些上下文中使用，那么就需要将变量声明为 `constexpr`，编译器保证它们在编译期确定其值。
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
简而言之，`constexpr` 对象都是 `const` 的，但是反之则不一定。如果在编译期初始化一个值，且能放到需要编译期常量的上下文中，那么就需要使用 `constexpr`。

将 `constexpr` 作用于函数时，如果实参是编译期确定的值，那么返回值是编译期常量；实参是运行时才能知道，那么函数返回一个运行时的值。
* `constexpr` 函数可以用于需要编译期常量的地方。如果你传入的实参是编译期确定的常量，那么函数返回一个编译期常量，否则，代码无法通过编译。
* 当传入 `constexpr` 函数的参数中有一个或者多个无法在编译期确定，那么行为和调用普通函数一样。这意味着我们不需要两个函数，一个为了编译期求值，一个为了运行时求值。一个 `constexpr` 函数就够了。

假定有这样一个需求。一个实验，有 $n$ 个变量，每个变量有三种可能性，那么共有 $3^n$ 种结果。假定 $n$ 编译期可知，那么 $3^n$ 理论上也是编译期可知的，那么使用 `std::array` 是一个合理的选择。为此，我们需要一个可以在编译期计算 $3^n$ 的函数。C++ 标准库提供了 `std::pow` 可以用于计算指数，不过有两个问题，第一个是返回值是浮点数而我们需要整数，第二个是这个函数不是 `constexpr` 的，无法在编译期求值。

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
`pow` 前面的 `constexpr` 不是说 `pow` 返回一个 `const` 值，而是说如果 `base, exp` 都是编译期常量，那么 `pow` 的结果也可以当做编译期常量使用，如果不都是活着都不是编译期常量，那么 `pow` 会在运行时计算结果。这意味 `pow` 不仅可以用于类似 `std::array` 的大小这种需要编译期常量的地方，还可以用于运行时环境。
```cpp
auto base = readFromDB("base");    // get these values
auto exp = readFromDB("exponent"); // at runtime
auto baseToExp = pow(base, exp);   // call pow function at runtime
```
`constexpr` 函数必须能够在编译期被调用且返回结果，因此对此有一些限制，且 C++11 和 C++14 的限制不同。

C++11 中要求函数不能多余一行，即一个 `return` 语句。不过可以使用三目运算符 `?:`，还可以使用递归，那么表达能力没有特别弱。C++11 中可以这样实现 `pow`。
```cpp
constexpr int pow(int base, int exp) noexcept
{
    return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
```
除了函数式程序员以外，很难有人认为这种风格很好。C++14 中对 `constexpr` 函数的限制宽松了需要，可以这么写：
```cpp
constexpr int pow(int base, int exp) noexcept // C++14
{
    auto result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}
```
`constexpr` 函数的参数类型和返回类型必须要是字面类型（`literal type`），意思是可以在编译期确定值的类型。C++11 中除了 `void` 之外的内置类型是字面类型，用户自定义类型也可以是字面类型，因为构造函数和其他函数可以是 `constexpr` 函数。
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
`Point` 的构造函数是 `constexpr` 函数，如果参数是编译期确定的，那么 `Point` 对象的值也就在编译期确定了，那么这个对象也就可能可以 `constexpr` 修饰。
```cpp
constexpr Point p1(9.4, 27.7); // fine, "runs" constexpr ctor during compilation
constexpr Point p2(28.8, 5.3); // also fine
```
类似的，`xValue, yValue` 两个函数可以是 `constexpr` 函数。如果是一个编译期可知的 `Point` 对象，那么其成员变量 `x, y` 也是编译期确定的值。那么我们可以写一个 `constexpr` 函数，调用这两个成员函数，用这个函数的结果初始化 `constexpr` 对象。
```cpp
constexpr Point midpoint(const Point &p1, const Point &p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2,  // call constexpr
            (p1.yValue() + p2.yValue()) / 2}; // member funcs
}
constexpr auto mid = midpoint(p1, p2); // init constexpr object
                                       // w/result of constexpr function
```
这意味着我们可以通过调用构造函数、成员函数和非成员函数构造 `mid` 对象，其放在只读存储中。我们还可以将 `mid.xValue() * 10` 用于模板参数或者需要枚举值的表达式里面。更进一步，这模糊了运行时计算和编译时计算，很多以前必须运行时的计算可以放到编译期，这样的迁移越多，程序运行的越快，不过编译时间也就越长。

`Point` 的 `setX, setY` 在 C++11 中不能是 `constexpr` 函数，因为 1）C++11 中默认 `constexpr` 默认是 `const` 的而这两个成员函数修改了对象的状态；2）C++11 中 `void` 不是字面类型。C++14 中放开了这两个限制，所以也可以这样写：
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
如果可能，尽可能的使用 `constexpr` 关键字。因为 `constexpr` 对象和 `constexpr` 函数都比非 `constexpr` 对象和函数的应用场景药广泛。

需要注意的是，`constexpr` 是对象和函数接口的一部分。加上 `constexpr` 意味着能被用于编译期常量的上下文中。如果之后又觉得使用 `constexpr` 是一个错误并移除的话，可能会导致客户端代码无法通过编译。（为了调试或者优化性能而添加一个 I/O 操作就可能会导致这样的问题，因为 I/O 语句往往不能加到 `constexpr` 函数中。）尽可能的使用 `constexpr` 表示承诺长期对对象或函数有这样的约束。

## Things to Remember
* `constexpr` objects are `const` and are initialized with values known during compilation.
* `constexpr` functions can produce compile-time results when called with arguments whose values are known during compilation.
* `constexpr` objects and functions may be used in a wider range of contexts than non-`constexpr` objects and functions.
* `constexpr` is part of an object's or function's interface.