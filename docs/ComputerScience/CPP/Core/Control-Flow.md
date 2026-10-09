
## `for` 循环
传统 `for` 循环的语法如下：
```cpp
for (init-expression; condition; iteration-expression)
{
    // loop body
}
```
range-based `for` 循环的语法如下：
```cpp
for (declaration : range)
{
    // loop body
}
```
比如遍历一个 `std::vector<int>`：
```cpp
std::vector<int> vec = { 1, 2, 3, 4, 5 };
for (int x : vec)
{
	// loop body
}
```
C++ 规定将右值绑定到通用引用 `auto &&` 时，需要延长临时对象的生命周期。比如上面例子中 `std::vector<int>` 如果是通过 `GetVector()` 这样的函数返回的临时对象，需要延长其生命周期到整个 `for` 循环的作用域。
```cpp
for (auto &&x : GetVector())
{
    // loop body
}
```
但是这会产生一个新的问题，如果是链式调用，中间产生了临时对象，可能会导致未定义行为（`undefined behavior`）。比如下面的例子中，`GetItems()` 返回了其内部成员的引用，那么 `for` 循环可能会访问已经被销毁的对象：因为 `CreateFactory()` 返回的临时对象 `Factory` 在完整表达式结束后、`for` 循环开始前就已经被销毁了，`GetItems()` 返回的引用所指向的内存区域已经失效。
```cpp
struct Factory
{
	std::vector<int> items = { 1, 2, 3, 4, 5 };
	const std::vector<int> &GetItems() const { return items; }
};

Factory CreateFactory() { return Factory {}; }

for (auto &&item : CreateFactory().GetItems())
{
	// loop body
}
```
C++20 对 range-based `for` 循环进行了改进，引入了初始化语句，可以很好地解决这个问题，通过初始化语句显式地确定 `Factory` 对象的生命周期。
```cpp
for (Factory factory = CreateFactory(); auto &&item : factory.GetItems())
{
	// loop body
}
```

对于 range-based `for` 循环，`begin` 和 `end` 可以是不同类型。编译器展开后的代码大致如下。
```cpp
{
	auto &&__range = for_range_initializer;
	auto __begin = begin_expr;
	auto __end = end_expr;
	for (; __begin != __end; ++__begin)
	{
		for_range_declaration = *__begin;
		statement;
	}
}
```
假定我们定义一个遍历 C 风格字符串的范围类型：`begin()` 返回指针，`end()` 返回哨兵。
```cpp
#include <iostream>

struct NullSentinel
{};

// overload the != operator for the iterator and sentinel comparison
inline bool operator!=(const char *ptr, NullSentinel) { return *ptr != '\0'; }

struct CStringRange
{
	const char *str;

	// begin() returns char*
	const char *begin() const { return str; }

	// end() returns a completely different type, NullSentinel!
	NullSentinel end() const { return NullSentinel {}; }
};

int main()
{
	CStringRange str { "Hello C++!" };

	// for loop using the custom range and sentinel
	for (char c : str)
	{
		std::cout << c << ' ';
	}
}
```
