对于模板函数
```cpp
template <typename T>
void func(T &&param);
```
而言，不管 `param` 是左值还是右值，推导模板参数 `T` 都会编码（`encode`）。

编码的机制很简单。当左值被传入的时候，`T` 被推导为左值引用，当右值被传入的时候，`T` 推导为非引用。注意，这里并不是对称的。因此
```cpp
Widget widgetFactory(); // function returning rvalue
Widget w;               // a variable (an lvalue)

func(w);               // call func with lvalue; T deduced to be Widget&
func(widgetFactory()); // call func with rvalue; T deduced to be Widget
```
将 `Widget` 传入 `func`，依赖于其是左值还是右值，推导的类型不同。我们很快将看到，这决定了通用引用是左值引用还是右值引用，这也是 `std::forward` 正确工作的机制。

在继续之前，我们首先要明确在 C++ 中引用的引用是不合法的。
```cpp
int x;
auto& & rx = x; // error! can't declare reference to reference
```
不过再看看当左值传入模板函数的时候发生了什么：
```cpp
template <typename T>
void func(T &&param); // as before

func(w); // invoke func with lvalue; T deduced as Widget&
```
当 `T` 被推导成引用 `Widget&`，那么函数参数看起来如下
```cpp
void func(Widget& &&param);
```
引用的引用。编译器自己生成了引用的引用，但是没有报错。[Item 24](./24_Distinguish_universal_references_from_rvalue_references.md) 说过，当 `param` 用左值初始化，那么其类型是左值引用，编译器是如何使得最后的签名如下的呢？
```cpp
void func(Widget &param);
```
答案是引用折叠（`reference collapsing`）。当编译器遇到引用的引用时（比如这里的模板实例化），引用折叠指示下一步需要做什么。

有左值和右值两种引用，那么引用的引用有四种，左值的左值，左值的右值，右值的左值，右值的右值。如果出现了引用的引用，根据如下规则折叠成单一引用。

> 如果任意一层引用是左值引用，结果是左值引用；否则，即两层都是右值引用，结果是右值引用。

上面将 `Widget&` 替换 `func` 的例子中，产生了左值引用的右值引用，那么结果就是左值引用。

引用折叠是 `std::forward` 能够工作的关键之一。`std::forward` 的一个常见场景是有通用引用的模板函数。
```cpp
template <typename T>
void f(T &&fParam)
{
    // do some work

    someFunc(std::forward<T>(fParam)); // forward fParam to someFunc
}
```
`fParam` 是通用引用，那么不管传入 `f` 的是左值还是右值，`T` 都会编码。当 `T` 编码说传入 `f` 的是右值时，`std::forward` 的就是要将 `fParam` 这个左值转化成右值。

`std::forward` 实现如下
```cpp
template <typename T>                         // in
T &&forward(typename                          // namespace
            remove_reference<T>::type &param) // std
{
    return static_cast<T &&>(param);
}
```
这并不是标准实现，但是差异不影响理解 `std::forward` 是如何工作的。

假定这里传入 `f` 的是 `Widget` 的左值，`T` 被推导为 `Widget&`，那么对 `std::forward` 的调用被实例化为 `std::forward<Widget&>`。将 `Widget&` 代入 `std::forward` 的实现
```cpp
Widget & &&forward(typename remove_reference<Widget &>::type &param)
{
    return static_cast<Widget & &&>(param);
}
```
`std::remove_reference<Widget&>::type` 结果是 `Widget`，因此代码变成了
```cpp
Widget & &&forward(Widget &param)
{
    return static_cast<Widget & &&>(param);
}
```
对类型转换和返回类型应用引用折叠，最终版本的 `std::forward` 如下。
```cpp
Widget &forward(Widget &param)
{
    return static_cast<Widget &>(param);
}
```
当左值传入 `f`，`std::forward` 被实例化成接受左值引用并返回左值引用，`std::forward` 内部什么也没有做，因为传入参数已经是 `Widget&` 类型了。左值传入 `std::forward`，返回了左值引用，而后者依旧是左值，因此 `std::forward` 返回的还是左值，符合预期。

现在假定传入 `f` 的是 `Widget` 的右值。这种情况下，`T` 被推导为 `Widget`。`f` 内部 `std::forward` 被实例化为 `std::forward<Widget>`，使用 `Widget` 替换 `T` 得到 `std::forward` 的实现。
```cpp
Widget &&forward(typename remove_reference<Widget>::type &param)
{
    return static_cast<Widget &&>(param);
}
```
`std::remove_reference` 作用于 `Widget` 类型，结果还是 `Widget` 类型，所以 `std::forward` 代码是
```cpp
Widget &&forward(Widget &param)
{
    return static_cast<Widget &&>(param);
}
```
这里没有引用的应用，也就没有引用折叠，即这就是 `std::forward` 的最终版本。

从函数返回的右值引用被定义为右值。`std::forward` 将传入 `f` 的 `fParam` 这个左值转换为了右值。从最终效果来看，传入 `f` 的右值被右值的形式转发给了 `someFunc`，这就是我们所期望的。

C++14 中的 `std::remove_reference_t` 能够简化 `std::forward` 的实现。
```cpp
template <typename T> // C++14
T &&forward(remove_reference_t<T> &param)
{
    return static_cast<T &&>(param);
}
```
引用折叠会出现在四个地方。第一处就是上面描述的模板实例化。第二处是使用 `auto` 声明变量。细节与模板基本一致，因为 [Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 告诉我们 `auto` 的类型推导与模板基本一致。考虑之前的例子
```cpp
template <typename T>
void func(T &&param);

Widget widgetFactory(); // function returning rvalue
Widget w;               // a variable (an lvalue)

func(w);               // call func with lvalue; T deduced to be Widget&
func(widgetFactory()); // call func with rvalue; T deduced to be Widget
```
在 `auto` 上下文中，规则是类似的。
```cpp
auto&& w1 = w;
```
使用左值 `w` 初始化，`auto` 推导的类型是 `Widget&`，将其代回 `auto` 声明得到了引用的引用。
```cpp
Widget& && w1 = w;
```
引用折叠
```cpp
Widget& w1 = w;
```
所以 `w1` 是左值引用。

另一方面，我们看下面这个声明
```cpp
auto&& w2 = widgetFactory();
```
使用右值初始化 `w2`，`auto` 的推导结果是没有引用的 `Widget`，代回 `auto` 声明
```cpp
Widget&& w2 = widgetFactory();
```
这里没有引用的引用，就是最后结果了。因此 `w2` 是右值引用。

至此，基本完全理解了通用引用。它不是一种新的引用类型，当下面两个条件满足的时候是右值引用。

* 类型推导区分左值和右值。`T` 类型是左值，那么推导成 `T&`，如果是右值，还是 `T` 本身。
* 发生引用折叠。

通用引用的概念很有用。它使得我们可以不必知道引用折叠的存在，不必推导左值和右值不同的类型，然后再使用引用折叠的规则。

第三个出现引用折叠的地方是使用 `typedef` 或声明别名，如果在这个过程出现了引用的引用，就会发生引用折叠。比如我们有 `Widget` 模板类，内部使用 `typedef` 定义一个右值引用类型。
```cpp
template <typename T>
class Widget
{
public:
    typedef T &&RvalueRefToT;
};
```
假定实例化 `Widget` 的是左值引用
```cpp
Widget<int&> w;
```
`T` 是 `int&`，代入 `typedef` 语句得到
```cpp
typedef int& &&RvalueRefToT;
```
引用折叠完结果是
```cpp
typedef int &RvalueRefToT;
```
`typedef` 的名字可能并不能描述实际情况，当使用左值引用实例化 `Widget` 时，`RvalueRefToT` 是左值引用。

最后一处是使用 `decltype` 声明类型时，如果出现了引用的引用，就会发生引用折叠。

## Things to Remember
* Reference collapsing occurs in four contexts: template instantiation, `auto` type generation, creation and use of `typedef` and alias declarations, and `decltype`.
* When compilers generate a reference to a reference in a reference collapsing context, the result becomes a single reference. If either of the original references is an lvalue reference, the result is an lvalue reference. Otherwise it's an rvalue reference.
* Universal references are rvalue references in contexts where type deduction distinguishes lvalues from rvalues and where reference collapsing occurs.
