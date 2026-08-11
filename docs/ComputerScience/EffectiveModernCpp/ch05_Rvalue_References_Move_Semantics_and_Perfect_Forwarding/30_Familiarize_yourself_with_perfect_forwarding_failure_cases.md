这一小节会讨论几种完美转发失败的情况。

在开始之前，我们首先讨论下完美转发的含义。转发的意思是说将传入一个函数的参数传入——转发——给另一个函数，目的是第二个函数收到与第一个函数收到的对象相同。这不适用于按值传递，因为对象是拷贝进来的。我们希望转发到的函数基于原始传入的对象执行。这也不适用于指针类型，因为我们不想强迫调用者一定要传递指针。那么通常情况下转发指的是转发引用参数。

完美转发意味着我们不仅转发对象，也转发它们的特征：它们的类型，是左值还是右值，是 `const` 或是 `volatile`。结合我们需要转发引用，那么需要使用通用引用，因为只有通用引用才能在传递参数时确定是左值还是右值。

假定我们有一个函数 `f`，我们下面实现一个模板函数转发参数给它。
```cpp
template <typename T>
void fwd(T &&param) // accept any argument
{
    f(std::forward<T>(param)); // forward it to f
}
```
就转发函数本质而言，它们是通用的。接受任意类型的参数，转发得到的参数。拓展这种想法，模板函数应该是可变参数模板，接受任意多的参数。那么这个函数的实现如下
```cpp
template <typename... Ts>
void fwd(Ts &&...params) // accept any arguments
{
    f(std::forward<Ts>(params)...); // forward them to f
}
```
这种形式在标准库中很常见，比如 `emplace` 函数（见 [Item 42](../ch08_Tweaks/42_Consider_emplacement_instead_of_insertion.md)）和智能指针的 `std::make_shared` 和 `std::make_unique`。

在给定 `f` 和转发函数 `fwd` 的前提下，完美转发失败指的是给定参数 `f` 所作的事情与 `fwd` 转发相同参数时做的事情不同。
```cpp
f(expression);   // if this does one thing,
fwd(expression); // but this does something else, fwd fails
                 // to perfectly forward expression to f
```
不同的参数会导致不同的错误。知道是什么错误和如何避免是很重要的。

## Braced initializers
假定 `f` 声明如下
```cpp
void f(const std::vector<int> &v);
```
使用大括号初始化是能够编译成功的
```cpp
f({1, 2, 3}); // fine, "{1, 2, 3}" implicitly converted to std::vector<int>
```
但是传入 `fdw` 就无法编译。
```cpp
fwd({1, 2, 3}); // error! doesn't compile
```
这就是大括号初始化导致完美转发失败的例子。

所有这样的失败的原因都是一样的。当直接调用 `f` 的时候，比如 `f({1, 2, 3})`，编译器会查看函数声明的参数类型和传递的参数类型，判断是否匹配，必要时会进行隐式转换。`{1, 2, 3}` 生成了一个临时的 `std::vector<int>` 使得 `f` 的参数 `v` 能够绑定到 `std::vector<int>` 对象上。

当通过 `fwd` 间接调用 `f` 的时候，编译器不再检查传递的参数与 `f` 声明类型的匹配度。而是推导传入 `fwd` 的参数类型，拿推导的参数类型与 `f` 声明的参数类型比较。当以下情况发生的时候，完美转发就会失败。

* 编译器无法推导出 `fwd` 的一个或多个参数类型。这种情况下会编译失败。
* 编译器推导错了 `fwd` 的一个或多个参数类型。错误意味着无法使用推导出来的类型编译 `fwd`，也可能意味着使用 `fwd` 的推导类型调用 `f`，会导致与直接使用相同参数调用 `f` 的行为不一致。这可能是由于 `f` 有多个重载，而推导类型调用的与直接调用的 `f` 函数不同。

上面的例子中，错误的原因是将大括号初始化传递给了一个没有声明 `std::initializer_list` 为参数的模板函数。这是一个非推导上下文。也就是说，由于模板函数的参数不是 `std::initializer_list`，那么编译器被禁止从 `{1, 2, 3}` 推导类型。由于被阻止推导类型，那么编译器只能报错处理了。

[Item 2](../ch01_Deducing_Types/01_Understand_template_type_deduction.md) 告诉我们能够正确推导使用大括号初始化的 `auto` 变量。这个变量被视为 `std::initializer_list` 对象，而我们转发就是需要这么一个 `std::initializer_list` 类型。因此一个简单的变通方式就是先使用 `auto` 声明一个变量，然后调用 `fwd` 函数。
```cpp
auto il = {1, 2, 3}; // il's type deduced to be std::initializer_list<int>
fwd(il);             // fine, perfect-forwards il to f
```

## 0 or NULL as null pointers
[Item 8](../ch03_Moving_to_Modern_C++/08_Prefer_nullptr_to_0_and_NULL.md) 告诉我们如果使用 0 或者 `NULL` 作为空指针传入模板函数，会导致推导出错误的类型，期望是指针类型，但是实际是整数类型。这种情况下，不能期待会完美转发一个空指针。解决方案也很简单，传入 `nullptr`。

## Declaration-only integral static const data members
一般而言，没有必要定义 `static const` 的成员变量，因为编译器会进行常量传播（`const propagation`），消除了需要内存存放的需求。
```cpp
class Widget
{
public:
    static const std::size_t MinVals = 28; // MinVals' declaration no defn. for MinVals
};

std::vector<int> widgetData;
widgetData.reserve(Widget::MinVals); // use of MinVals
```
这里 `Widget::MinVals` 没有定义，但仍可以使用它来初始化 `widgetData` 的容量。编译器会使用 28 替换所有使用 `Widget::MinVals` 的地方。所以没有内存存储这个值也是可行的。但是如果需要取地址，比如有某个指针指向这个变量，那么上述代码虽然能编译通过，但是链接的时候会报错。

考虑之前的 `f` 与 `fwd` 函数，假定 `f` 的声明如下
```cpp
void f(std::size_t val);
```
那么如下调用函数 `f` 是合法的
```cpp
f(Widget::MinVals); // fine, treated as "f(28)"
```
但是无法通过 `fwd` 调用 `f`
```cpp
fwd(Widget::MinVals); // error! shouldn't link
```
链接的时候会出错，原因与之前解释一样。

尽管这里并没有取 `MinVals` 的地址，但是 `fwd` 的参数是通用引用，而引用在编译器看来和指针一样。在某种程度上，引用就是自动解引用的指针。传递 `MinVals` 的引用与传递指针一样高效，但是也要求有某个内存地址，使得指针能够指向这个位置。通过引用传递 `static const` 的成员变量，一般也就要求其有定义，否则会导致完美转发失败。

根据标准，传递 `static const` 整数的引用，要求有定义。但是并不是所有的实现都强制要求这一点。所以，具体问题可能依赖于编译器和链接器。你或许发现没有定义也能完美转发 `static const` 的整型成员变量，但是这并不是能这么做的理由，因为这涉及可移植性。为了提高可移植性，应该给 `static const` 变量一个定义。对于 `MinVals`，定义如下
```cpp
const std::size_t Widget::MinVals; // in Widget's .cpp file
```
注意，这里并没有使用 28 进行初始化。因为按照要求只能初始化一次，如果在两个地方都初始化，编译器会报错。

## Overloaded function names and template names
假定函数 `f` 接受一个函数作为参数，以自定义行为。假定这个函数参数与返回值类型都是 `int`，那么 `f` 的声明如下
```cpp
void f(int (*pf)(int)); // pf = "processing function"
```
也可以使用非指针的语法
```cpp
void f(int pf(int)); // declares same f as above
```
现在，有两个重载版本的 `processVal` 函数
```cpp
int processVal(int value);
int processVal(int value, int priority);
```
我们可以将 `processVal` 传递给 `f` 函数
```cpp
f(processVal); // fine
```
编译器知道需要哪一个 `processVal`，即匹配 `f` 参数的那一个。于是乎，将接受一个 `int` 参数的 `processVal` 函数地址传递给 `f`。

但是 `fwd` 是一个模板函数，并不包含任何关于需要什么类型的参数的信息，那么编译器就无法知道该使用哪一个重载。
```cpp
fwd(processVal); // error! which processVal?
```
只有 `processVal` 是没有类型的，那么无法进行类型推导，完美转发失败。

如果我们使用模板函数而不是重载，也会有同样的问题。一个模板函数不是一个函数，而是需要函数
```cpp
template <typename T>
T workOnVal(T param) // template for processing values
{
}

fwd(workOnVal); // error! which workOnVal instantiation?
```
如果想要完美转发接受一个重载函数或者是模板函数，需要手动的指定是哪一个重载或者模板实例化的函数。比如，声明一个与 `f` 参数相同的函数指针类型，使用 `processVal` 或者 `workOnVal` 初始化，这就选择了合适的重载或模板实例，然后将指针传递给 `fwd`
```cpp
using ProcessFuncType = int (*)(int); // make typedef; see Item 9

// specify needed signature for processVal
ProcessFuncType processValPtr = processVal;

fwd(processValPtr);                           // fine
fwd(static_cast<ProcessFuncType>(workOnVal)); // also fine
```
这就要求我们知道完美转发的函数指针的类型。但是没有理由一个完美转发的函数会有文档记录这些事情。毕竟，完美转发设计初衷是接受任意类型，所以如果没有文档记录这些的话，我们又怎么能知道具体类型呢？

## Bitfields
最后一种无法完美转发的情况是使用了位域作为函数的参数。假定 IPv4 头的定义如下
```cpp
struct IPv4Header
{
    std::uint32_t version : 4,
        IHL : 4,
        DSCP : 6,
        ECN : 2,
        totalLength : 16;
};
```
假定 `f` 的参数是 `std::size_t`，我们传入 `IPv4Header` 的 `totalLength` 来调用 `f` 函数。
```cpp
void f(std::size_t sz); // function to call
IPv4Header h;

f(h.totalLength); // fine
```
但是无法以同样的方式调用 `fwd` 函数
```cpp
fwd(h.totalLength); // error!
```
原因是 `fwd` 的参数是引用，而 `h.totalLength` 是非 `const` 位域。C++ 标准禁止这么做：非 `const` 引用不能绑定到位域。原因很简单，因为位域可以从任意比特开始，而指针或引用不能指向任意比特，C++ 寻址最小单元是 `char`，也就是一个字节而不是比特。

如果意识到接受位域作为参数的函数，函数收到的一定是拷贝的话，那么这么问题就很容易解决了。如前所述，没有指针或引用能够指向位域，那么函数不能是指针参数或者引用参数。所以结果只能是按值传递，或者是 `const` 引用。按值传递的话函数收到的是位域内容的拷贝，如果参数是 `const` 引用，标准是要求引用绑定到存放位域的的整数类型（比如 `int`）的拷贝对象上。因此，并不是绑定到位域而是包含位域的对象的拷贝。

解决这个问题的关键就是函数接受的是拷贝。在完美转发之前，我们可以先行拷贝一遍。在当前 `IPv4Header` 例子中，代码如下
```cpp
// copy bitfield value; see Item 6 for info on init. form
auto length = static_cast<std::uint16_t>(h.totalLength);

fwd(length); // forward the copy
```

## Upshot
大部分时候，完美转发都工作的很好，偶尔会出错。可能是编译错误，更严重的参数是能编译但是不能按照预期行为工作。知道完美转发不完美的情况是很重要的，还需要知道如何绕过这些问题。

## Things to Remember
* Perfect forwarding fails when template type deduction fails or when it deduces the wrong type.
* The kinds of arguments that lead to perfect forwarding failure are braced initializers, null pointers expressed as `0` or `NULL`, declaration-only integral `const static` data members, template and overloaded function names, and bitfields.
