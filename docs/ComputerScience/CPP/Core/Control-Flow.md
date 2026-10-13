
## `if` 语句
C++17 中可以在 `if` 语句中引入初始化语句，从而在条件判断之前初始化变量。语法如下：
```cpp
if (init-statement; condition)
{
	// if body
}
```
这样做的好处是可以将变量的作用域限制在 `if` 语句内部，避免污染外部作用域，同时也使代码更加简洁和易读。

### `if constexpr`
C++17 引入了 `if constexpr`，允许在编译期根据条件选择性地编译代码。语法如下：
```cpp
if constexpr (condition)
{
	// Discarded if condition is false
}
else
{
	// Discarded if condition is true
}
```
在 C++17 之前，如果某个算法存在针对不同类型要求的多个版本，需要借助 SFINAE 或标签分发（`tag dispatching`）技术来写一组函数。
```cpp
// SFINAE
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T> SimpleTypeInfo(T t)
{
	std::cout << "foo<integral T> " << t << '\n';
	return t;
}

template<typename T>
std::enable_if_t<!std::is_integral_v<T>, T> SimpleTypeInfo(T t)
{
	std::cout << "not integral \n";
	return t;
}

// Tag dispatching
template<typename T>
T SimpleTypeInfoTagImpl(T t, std::true_type)
{
	std::cout << "foo<integral T> " << t << '\n';
	return t;
}

template<typename T>
T SimpleTypeInfoTagImpl(T t, std::false_type)
{
	std::cout << "not integral \n";
	return t;
}

template<typename T>
T SimpleTypeInfoTag(T t)
{
	return SimpleTypeInfoTagImpl(t, std::is_integral<T> {});
}
```
使用 `if constexpr` 的写法如下，更自然，无需任何技巧。
```cpp
template<typename T>
auto SimpleTypeInfoIfConstexpr(T t)
{
	if constexpr (std::is_integral_v<T>)
	{
		std::cout << "foo<integral T> " << t << '\n';
		return t;
	}
	else
	{
		std::cout << "not integral \n";
		return t;
	}
}
```

## `switch` 语句
C++17 中可以在 `switch` 语句中引入初始化语句，效果和语法与 `if` 语句类似。

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
