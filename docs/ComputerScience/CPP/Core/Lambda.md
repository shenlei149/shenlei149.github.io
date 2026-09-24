
## C++20 Lambda 新特性
### Lambda 模板参数
在 C++20 之前，如果要用 lambda 表达式表示泛型，就不得不使用 `auto`，但这存在弊端，比如下面的例子中，使用 `auto` 会允许两个参数类型不同，但在某些场景下（从语义上讲）我们希望约束它们必须是同类型的。C++20 的 lambda 引入了模板参数，就能很好地解决这个问题。
```cpp
auto sumGeneric = [](auto a, auto b) { return a + b; };
auto sumTemplate = []<typename T>(T a, T b) { return a + b; };

int main()
{
	std::cout << sumGeneric(1, 2) << std::endl;	 // 3
	std::cout << sumTemplate(1, 2) << std::endl; // 3

	std::cout << sumGeneric(1, true) << std::endl;	// 2
	// std::cout << sumTemplate(1, true) << std::endl; // no match for call to ‘(<lambda(T, T)>) (int, bool)’
}
```

lambda 模板参数的另一个有用的场景是当 lambda 参数的类型是容器的时候。比如下面的例子，第一个可以接受任何容器类型，只要求有 `size()` 方法就好，第二个只能接受 `std::vector` 作为参数，第三个使用了概念（concept）来约束容器类型，只能是存放整数类型的 `std::vector`。
```cpp
auto sizeGeneric = [](auto container) { return container.size(); };
auto sizeVector = []<typename T>(std::vector<T> v) { return v.size(); };
auto sizeConcept = []<std::integral T>(std::vector<T> v) { return v.size(); };
```

### 非求值上下文和无状态 lambda
C++20 允许 lambda 表达式出现在非求值上下文中，比如 `decltype` 中，同时对于无状态（`stateless`）的 lambda，即不捕获任何外部变量的 lambda，可以默认构造和拷贝。这两项新特性极大简化了相关用法。
```cpp
// C++17
auto cmp = [](int a, int b) { return a > b; };
std::set<int, decltype(cmp)> mySet1(cmp); // pass cmp instance explicitly

// C++20
std::set<int, decltype([](int a, int b) { return a > b; })> mySet2;
```

### 与 `consteval` 结合
C++20 允许将 lambda 与 `consteval` 结合使用，从而在编译期进行求值。下面是一个简单的例子：
```cpp
// C++20 consteval lambda example
auto constevalLambda = []() consteval
{
	std::vector myVec = { 1, 2, 4, 3 };
	std::sort(myVec.begin(), myVec.end());
	return myVec.back();
};
```

### 初始化捕捉与包展开
C++20 增强了 lambda 的表达能力，在初始化捕捉（`init capture`）中支持包展开（`pack expansion`），从而可以更方便地捕获参数包。下面是一个简单的例子：
```cpp
#include <iostream>
#include <memory>
#include <utility>

template<typename Callable, typename... Args>
auto PackExpansion(Callable call, Args... args)
{
	// C++20: init capture with pack expansion
	return [call, ... args = std::move(args)]() mutable { return call(std::move(args)...); };
}

void ProcessResource(std::unique_ptr<int> ptr, const std::string &name)
{
	std::cout << name << " holds value: " << *ptr << '\n';
}

int main()
{
	auto ptr = std::make_unique<int>(42);

	// move unique_ptr into the closure
	auto task = PackExpansion(ProcessResource, std::move(ptr), "Task A");

	task(); // Task A holds value: 42
}
```
