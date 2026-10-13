

## 折叠表达式
变参模板是一个强大的工具，它允许我们定义接受可变数量模板参数的类或函数。不过当我们需要实现递归的时候，需要写一些额外的代码，比如下面的求和函数：
```cpp
auto Sum() { return 0; }

template<typename T, typename... Ts>
auto Sum(T head, Ts... tail)
{
	return head + Sum(tail...);
}
```
使用折叠表达式，我们可以将上面的递归实现简化为一行：
```cpp
template<typename... Args>
auto Sum(Args... args)
{
	return (args + ... + 0);
}
```
折叠表达式有以下几个变种（以参数包 `arg1, arg2, ..., argN` 为例）：

- 一元左折叠（`unary left fold`）：`(... op args)`，从左到右依次折叠，展开形式为 `(((arg1 op arg2) op ...) op argN)`
- 一元右折叠（`unary right fold`）：`(args op ...)`，从右到左依次折叠，展开形式为 `(arg1 op (... op (argN-1 op argN)))`
- 二元左折叠（`binary left fold`）：`(init op ... op args)`，从左到右依次折叠，展开形式为 `(((init op arg1) op ...) op argN)`
- 二元右折叠（`binary right fold`）：`(args op ... op init)`，从右到左依次折叠，展开形式为 `(arg1 op (... op (argN op init)))`

其中 `op` 表示操作符，可以是以下操作符中的一个 `+` `-` `*` `/` `%` `^` `&` `|` `=` `<` `>` `<<` `>>` `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=` `<<=` `>>=` `==` `!=` `<=` `>=` `&&` `||` `,` `.*` `->*`。

当我们写 `Sum(1, 2, 3)` 时，编译器会将其展开为 `(1 + (2 + (3 + 0)))`，从而实现对可变参数的求和。

对于空的参数包，`||` 的默认值是 `false`，`&&` 的默认值是 `true`，`,` 的默认值是 `void()`，其余运算符没有默认值。因此下面的 `Sum()` 实现不能一个参数都不传递。
```cpp
template<typename... Args>
auto Sum(Args... args)
{
	return (args + ...);
}
```
下面的函数会打印传入的各个参数。
```cpp
template<typename... Args>
void FoldPrint(Args &&...args)
{
	(std::cout << ... << std::forward<Args>(args)) << '\n';
}
```
但上述代码打印出的各参数之间没有分隔符。如果需要分隔符，可以借助 `,` 运算符，下面使用一元右折叠实现带分隔符的打印：
```cpp
template<typename... Args>
void FoldPrintWithComma(Args &&...args)
{
	((std::cout << std::forward<Args>(args) << ", ") , ...) << '\n';
}
```
对于 `,` 而言，使用左折叠和右折叠对结果没有影响。

下面是依次 `push_back` 元素到 `std::vector` 的示例：
```cpp
template<typename T, typename... Args>
void PushBackVec(std::vector<T> &v, Args &&...args)
{
	(v.push_back(std::forward<Args>(args)), ...);
}

// example
std::vector<float> vf;
PushBackVec(vf, 10.5f, 0.7f, 1.1f, 0.89f);
```
