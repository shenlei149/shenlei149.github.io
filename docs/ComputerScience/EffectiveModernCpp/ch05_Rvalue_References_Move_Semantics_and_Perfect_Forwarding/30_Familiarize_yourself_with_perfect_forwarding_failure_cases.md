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

## Braced initializers

## 0 or NULL as null pointers
