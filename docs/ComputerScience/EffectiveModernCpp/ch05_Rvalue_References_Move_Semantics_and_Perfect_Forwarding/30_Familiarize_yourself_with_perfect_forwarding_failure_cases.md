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
这种形式在标准库中很常见，比如 `emplace` 函数（见 Item 42 TODO）和智能指针的 `std::make_shared` 和 `std::make_unique`。

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

