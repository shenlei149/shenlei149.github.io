
## 拷贝省略
假定有一个类 `Widget` 和如下工厂函数。使用时（比如 `Widget w = CreateWidget();`）会发生拷贝省略，`Widget` 直接在 `w` 中构造，不会先构造临时对象，再把它移动或拷贝到 `w`，然后析构临时对象。
```cpp
Widget CreateWidget()
{
    return Widget();
}
```
编译器还可以做更多优化。下面的例子是具名返回值优化（`Named Return Value Optimization`, `NRVO`）。注意，这一场景并非 C++ 标准强制要求。
```cpp
Widget CreateWidget()
{
    Widget w;
    // do some operations on w before returning it
    return w;
}
```
以下三种场景允许省略拷贝或移动：

- 用临时对象初始化另一个对象
- 返回或抛出一个即将离开作用域的对象
- 按值捕获异常

其中，用纯右值初始化对象，以及从函数返回纯右值，自 C++17 起必须省略。具名局部对象的返回（`NRVO`）、抛出和按值捕获只是允许省略，并不保证发生。强制省略有如下收益：

- 性能提升
- 可以确定地按值返回，而不必写成出参
- 行为在不同编译器之间可移植
- 可以返回既不能拷贝也不能移动的对象，因为这些操作根本不会发生

注意，不要写成如下形式。`std::move` 会抑制 NRVO，迫使编译器执行一次拷贝或移动。
```cpp
Widget CreateWidget()
{
    Widget w;
    // do some operations on w before returning it
    return std::move(w);
}
```
