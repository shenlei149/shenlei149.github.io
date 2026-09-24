
## 类模板推导
函数模板推导能够简化代码，C++17 为类模板也引入了类型推导（`class template argument deduction`, `CTAD`），从而在实例化类模板时可以省略模板参数，和使用函数模板类似。
```cpp
auto myPair = std::make_pair(1, "Hello");

using namespace std::string_literals;
std::pair myOtherPair = { 2, "World"s };
```
更复杂的例子如下：
```cpp
// lock guard:
std::shared_timed_mutex mut;
std::lock_guard<std::shared_timed_mutex> lck(mut);

// array:
std::array<int32_t, 3> arr { 1, 2, 3 };

// simplified:
std::shared_timed_mutex mut;
std::lock_guard lck(mut);

std::array arr { 1, 2, 3 };
```
但是不支持部分参数推导，例如：
```cpp
std::tuple<int32_t, int32_t, int32_t> t1(1, 2, 3);
std::tuple t2(4, 5, 6);
std::tuple<int32_t> t3(7, 8, 9); // bad!
```
编译器会自动生成推导规则（隐式），也允许用户定义规则。

大多数情况下，我们可以依赖编译器自动生成的推导规则。编译器会根据主类模板（`primary class template`）的构造函数（包含拷贝构造函数和移动构造函数）自动创建规则。不过特化和偏特化不适用于这个机制。

有的时候，我们需要自定义推导规则，比如下面的例子，如果没有自定义推导规则，默认情况下 `T` 会被推导为 `const char*`。
```cpp
template<typename T>
struct MyWrapper
{
	T value;
};

MyWrapper(const char *) -> MyWrapper<std::string>;

MyWrapper w = { "Hello" };
```
另一个自定义规则有用的例子是 `overload`，它没有构造函数，但是我们希望能够自动推导类型。
```cpp
template<class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

template<class... Ts>
overload(Ts...) -> overload<Ts...>; // deduction guide
```
