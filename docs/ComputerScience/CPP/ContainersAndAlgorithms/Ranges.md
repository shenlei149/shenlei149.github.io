得益于 C++20 引入的 Range 库，使用 STL 更加方便，更加强大。Range 库的算法具有延迟求值（`lazy evaluation`）的特性，同时可以直接用于容器，且极易组合使用。Range 库蕴含着函数式编程的思想。

下面是一个简单的例子，将容器中偶数的元素筛选出来乘以 2。
```cpp
std::vector<int> vec = { 1, 2, 3, 4, 5, 6 };
auto result = vec | std::views::filter([](int x) { return x % 2 == 0; })
			  | std::views::transform([](int x) { return x * 2; });

for (int x : result)
{
	std::cout << x << " ";
}
```

## Ranges
Range 是一对迭代器，我们可以遍历其范围内的元素。STL 中的容器是 Range 但不是 View。参考 Concepts 中的 Range 概念。

从 C++20 开始，`end()` 哨兵的类型可以和 `begin()` 迭代器的类型不同。针对不同的 Range，我们可以自定义哨兵，当然前提是要直观地理解 Range 的边界。下面这个例子对 C 字符串展示了自定义哨兵的用法。`std::ranges::for_each` 支持传入迭代器和哨兵来指定范围；也可以直接传入 Range，默认以其 `begin()` 和 `end()` 作为范围，和普通的 range-based `for` 循环效果相同。
```cpp
#include <algorithm>
#include <iostream>
#include <ranges>

struct Space
{
	bool operator==(auto pos) const { return *pos == ' '; }
};

int main()
{
	const char *str = "Hello World";
	// print "Hello"
	std::ranges::for_each(str, Space {}, [](char c) { std::cout << c; });
	std::cout << std::endl;

	std::ranges::subrange hello { str, Space {} };
	std::ranges::for_each(hello, [](char c) { std::cout << c; });
	std::cout << std::endl;
	for (auto c : hello)
	{
		std::cout << c;
	}
	std::cout << std::endl;

	return 0;
}
```

C++20 还新增了两个迭代器和三个特殊的哨兵，如下表所示。

| Iterator | Description |
|----------|-------------|
| `std::counted_iterator(it, count)` | 使用 `count` 来限制迭代器的范围 |
| `std::common_iterator(it, sent)` | 将迭代器和哨兵封装为一个通用迭代器 |
| `std::default_sentinel` | 用作默认的哨兵，表示 Range 的结束 |
| `std::unreachable_sentinel` | 表示一个永远不可达的哨兵 |
| `std::move_sentinel` | 为了将拷贝映射为移动语义而设计的哨兵 |

下面是使用 `std::counted_iterator` 只输出 `std::vector` 中前 4 个元素的例子。
```cpp
std::vector<int> vec = { 1, 2, 3, 4, 5, 6 };

std::ranges::copy(vec.begin(),
				  vec.begin() + 4,
				  std::ostream_iterator<int>(std::cout, " "));
std::cout << std::endl;

std::ranges::copy(std::counted_iterator(vec.begin(), 4),
				  std::default_sentinel,
				  std::ostream_iterator<int>(std::cout, " "));
std::cout << std::endl;
```
