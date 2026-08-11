C++14 最令人兴奋的功能是泛型 lambda（`generic lambda`），参数能够使用 `auto`。这个功能的实现很直接，闭包类的 `operator()` 是一个模板。比如
```cpp
auto f = [](auto x)
{ return func(normalize(x)); };
```
那么闭包类类似下面的实现
```cpp
class SomeCompilerGeneratedClassName
{
public:
    template <typename T>      // see Item 3 for
    auto operator()(T x) const // auto return type
    {
        return func(normalize(x));
    }

    // other closure class functionality
};
```
在这个例子中，lambda 唯一需要做的事情就是将参数 `x` 转发给 `normalize`。如果 `normalize` 对左值和右值处理不同，那么这个 lambda 没有实现对，因为总是将左值传递给 `normalize`，即使传递给 lambda 的是右值。

正确的方式是将 `x` 完美转发给 `normalize`。需要做两处修改，第一个将 `x` 声明为通用引用（参见 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)），第二个使用 `std::forward` 将参数传递给 `normalize`（参见 [Item 25](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md)）。
```cpp
auto f = [](auto &&x)
{ return func(normalize(std::forward<???>(x))); };
```
这里还有一个问题需要处理，`std::forward` 的模板参数是什么？也就是上面的 `???` 应该怎么写。

一般使用完美转发的情况是在一个模板函数内部，有一个类型参数 `T`，那么写作 `std::forward<T>` 即可。但是在泛型 lambda 中，没有类型 `T` 可用。闭包类中有一个模板参数 `T`，但是 lambda 中又用不了。

[Item 28](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/28_Understand_reference_collapsing.md) 解释了，如果左值传递给通用引用参数，参数类型是左值引用。如果传递右值，参数是右值引用。这意味在 lambda 中可以通过检查 `x` 类型来区分两者。`decltype` 就是做这个事情的（参见 [Item 3](../ch01_Deducing_Types/03_Understand_decltype.md)）。如果传递的是左值，`decltype(x)` 返回的是左值引用，如果传递的是右值，`decltype(x)` 返回的是右值引用。

[Item 28](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/28_Understand_reference_collapsing.md) 还解释了，当调用 `std::forward` 时，左值引用意味着传递左值，非引用意味着传递右值。在 lambda 中，如果 `x` 绑定的是左值，那么 `decltype(x)` 返回左值引用，这与之前的要求一致。但是如果 `x` 绑定的是右值，那么 `decltype(x)` 返回的右值引用，而不是非引用。

但是再来看一下 [Item 28](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/28_Understand_reference_collapsing.md) 中提到的 `std::forward` 实现。
```cpp
template <typename T>                     // in namespace
T &&forward(remove_reference_t<T> &param) // std
{
    return static_cast<T &&>(param);
}
```
假定客户端想要完美转发 `Widget` 的右值，使用类型 `Widget` 实例化 `std::forward`，那么 `std::forward` 模板生成的函数如下。
```cpp
Widget &&forward(Widget &param)           // instantiation of
{                                         // std::forward when
    return static_cast<Widget &&>(param); // T is Widget
}
```
现在考虑，想要完美转发 `Widget`，但是 `T` 不是非引用，而是右值引用，即 `Widget&&`，那么实例化 `std::forward` 之后且应用了 `std::remove_reference_t`，但是没有引用折叠时，实例化的 `std::forward` 如下。
```cpp
Widget && &&forward(Widget &param)           // instantiation of std::forward
{                                           // when T is Widget&&
    return static_cast<Widget && &&>(param); // (before reference- collapsing)
}
```
右值引用的右值引用，引用折叠的结果是右值引用，因此，实例化代码如下
```cpp
Widget &&forward(Widget &param)           // instantiation of std::forward
{                                         // when T is Widget&&
    return static_cast<Widget &&>(param); // (after reference- collapsing)
}
```
对比将 `T` 设置为 `Widget` 的 `std::forward` 实例化结果可以看出，使用右值引用和非引用实例化 `std::forward` 的结果是一样的。

这是一个好消息，传递右值，`decltype(x)` 返回右值引用，将其作为 `std::forward` 的模板参数，结果与使用非引用结果一致。前面已经论证，传递左值 `decltype(x)` 返回左值引用是满足需要的。那么将 `decltype(x)` 作为 `std::forward` 的模板参数是我们想要的结果。那么完美转发的 lambda 如下。
```cpp
auto f =
    [](auto &&param)
{
    return func(normalize(std::forward<decltype(param)>(param)));
};
```
再加上六个点，那么这个 lambda 将能够转发任意多的参数。
```cpp
auto f =
    [](auto &&...params)
{
    return func(normalize(std::forward<decltype(params)>(params)...));
};
```

## Things to Remember
* Use `decltype` on `auto&&` parameters to `std::forward` them.
