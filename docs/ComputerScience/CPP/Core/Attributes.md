这里单独罗列和讲解 C++11/14/17/20 中的各种属性及其语法与使用方法，可能在其他章节会重复出现。

## [[nodiscard]]
`[[nodiscard]]` 属性可以用于函数、枚举、类的声明。如果函数的返回值被标记为 `[[nodiscard]]`，编译器会在调用该函数但忽略其返回值时发出警告。如果函数返回的是标记了 `[[nodiscard]]` 的枚举值或类的对象，调用者也应当使用返回值，否则同样会收到警告。C++20 还支持填写原因。具体参考下面的例子。
```cpp
#include <utility>

struct MyType
{
	[[nodiscard("Implicit destroying of temporary MyType.")]]
	MyType(int, bool)
	{}
};

template<typename T, typename... Args>
[[nodiscard("You have a memory leak.")]]
T *Create(Args &&...args)
{
	return new T(std::forward<Args>(args)...);
}

enum class [[nodiscard("Don't ignore the error code.")]] ErrorCode
{
	Okay,
	Warning,
	Critical,
	Fatal
};

ErrorCode ErrorProneFunction() { return ErrorCode::Fatal; }

int main()
{
	// no warning
	int *val = Create<int>(5);
	delete val;

	// declared with attribute ‘nodiscard’: ‘You have a memory leak.’
	Create<int>(5);

	// declared with attribute ‘nodiscard’: ‘Don't ignore the error code.’
	ErrorProneFunction();

	// declared with attribute ‘nodiscard’: ‘Implicit destroying of temporary MyType.’
	MyType { 5, true };

	return 0;
}
```

## [[likely]] [[unlikely]]
`[[likely]]` 和 `[[unlikely]]` 属性用于指示编译器某个分支的可能性，从而帮助优化分支预测。标准指出，标记了 `[[likely]]` 的分支比其他分支具备任意更高的可能性（`arbitrarily more likely`），而标记了 `[[unlikely]]` 的分支则相反。任意高说明不是高一点，比如 51% vs. 49%，而是趋于 100%，即几乎确定会发生或几乎确定不会发生。过度使用这两个属性可能会导致性能下降。
```cpp
for (size_t i = 0; i < v.size(); i++)
{
	if (v[i] < 0) [[likely]]
	{
		sum -= sqrt(-v[i]);
	}
	else
	{
		sum += sqrt(v[i]);
	}
}
```

## [[no_unique_address]]
`[[no_unique_address]]` 属性表示类的非静态数据成员不需要独占唯一的地址，这个属性通常用于空类型的成员变量，以减少内存的使用。
```cpp
#include <iostream>

struct Empty
{};

struct NoUniqueAddress
{
	int d {};
	[[no_unique_address]] Empty e {};
};

struct UniqueAddress
{
	int d {};
	Empty e {};
};

// main function
int main()
{
	std::cout << std::boolalpha;

	// true
	std::cout << "sizeof(int) == sizeof(NoUniqueAddress): " << (sizeof(int) == sizeof(NoUniqueAddress)) << '\n';

	// false
	std::cout << "sizeof(int) == sizeof(UniqueAddress): " << (sizeof(int) == sizeof(UniqueAddress)) << '\n';

	return 0;
}
```
