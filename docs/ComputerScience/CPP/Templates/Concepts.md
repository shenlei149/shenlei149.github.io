如果写过大量模板代码，你可能会遇到一些编译错误信息非常长，大海捞针般的查找错误原因。C++20 引入了 Concepts，能够解救于此。大致说来，Concepts 有如下优势：

- 对模板参数的要求和约束是接口的一部分。
- Concepts 是可被编译器执行的文档。Concepts 指定了限制条件，编译器会在编译时检查条件是否成立。
- 函数的重载和模板的特化也可以基于 Concepts 来实现。
- Concepts 可以用于函数模板、类模板、模板成员函数，还可以用于变量模板（`variable templates`）和别名模板（`alias templates`）。
- 更友好的报错信息，编译器会直接将模板参数的要求与模板实参进行对比。
- 可以使用预定义（`predefined`）的 Concepts，也可以自定义（`custom`）Concepts。
- `auto` 与 Concepts 的用法统一了，使用 `auto` 的地方都能使用 Concepts。
- 如果一个函数使用了 Concepts，那么自动编程了一个函数模板。因此编写函数模板就和编写普通函数一样简单。

## 用法

有四种方式使用 Concepts。
```cpp
template<typename T>
requires std::integral<T>
auto gcd1(T a, T b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd1(b, a % b);
}

template<typename T>
auto gcd2(T a, T b)
requires std::integral<T>
{
	if (b == 0)
	{
		return a;
	}

	return gcd2(b, a % b);
}

template<std::integral T>
auto gcd3(T a, T b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd3(b, a % b);
}

auto gcd4(std::integral auto a, std::integral auto b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd4(b, a % b);
}
```
这四种写法分别是
- requires 子句（`requires clause`）
- 尾部 requires 子句（`trailing requires clause`）
- 受约束的模板参数（`constrained template parameter`）
- 简写函数模板（`abbreviated function template`）

上面写法有些许差异，`gcd4` 中参数 `a` 和 `b` 是两个独立的模板参数，类型可以不同，其他三种写法必须相同的类型。

这里面用到了预定义的 Concepts `std::integral`，它要求模板参数是整型（`integral`）类型。这个预定义 Concepts 实现很简单。
```cpp
template<class T>
concept integral = std::is_integral_v<T>;
```

requires 字句很强大，可以对模板参数或者函数声明进行约束。关键字 `requires` 后面是一个编译期的谓词，也可以是一个 Concepts，还可以是 `requires` 表达式（`requires expression`）。当然，也可以结合使用。

下面例子中，谓词表达式约束的是非类型 `i`。
```cpp
template<uint32_t i>
requires(i <= 20)
uint32_t sum(int j)
{
	return i + j;
}
```
如果违反了约束，比如 `sum<100>(20)`，编译器会给出明确的错误信息：
```
template<unsigned int i>  requires  i <= 20 uint32_t sum(int)
```

> [!TIP]
> 一般情况下应该给 Concepts 起一个有意义的名字，便于理解和使用，而不是直接使用谓词表达式。

Concepts 可以用于函数的返回类型。
```cpp
template<typename T>
requires std::integral<T>
std::integral auto gcd1(T a, T b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd1(b, a % b);
}

std::integral auto gcd4(std::integral auto a, std::integral auto b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd4(b, a % b);
}
```

Concepts 是一个编译期的谓词，是一个在编译期执行并返回布尔值的函数。因此可以用于 `static_assert`，也可以用于 `if constexpr`。`std::three_way_comparable<T>` 是一个预定义的 Concepts，后面会介绍。
```cpp
if constexpr (std::three_way_comparable<double>)
{
	// do something
}

static_assert(std::three_way_comparable<double>);
```

Concepts 可以作为类的模板参数。比如
```cpp
template<std::regular T>
class MyVector;
```
这里 `std::regular` 是一个预定义的 Concepts，要求 `T` 是一个常规类型（`regular type`），可以拷贝（`copyable`）、可以默认构造（`default constructible`）、可以比较相等（`equality comparable`）。类似于内置类型 `int` 的行为，可以使用 `==` 比较。`MyVector<int>` 是合法的，而 `MyVector<int &>` 会导致编译错误。

Concepts 可以用于模板类的成员函数，也可以用于普通类的模板成员函数。下面是一个普通类的模板成员函数的例子。
```cpp
class Printer
{
public:
	void PrintInt(std::integral auto i) { std::cout << i << std::endl; }
};

Printer p;
p.PrintInt(42);

// no instance of function template "Printer::PrintInt" matches the argument list
p.PrintInt(3.14);
```

Concepts 可以用于可变参数模板（`variadic template`）。下面的例子要求所有的模板参数都是整型。
```cpp
template<std::integral... Args>
bool All(Args... args)
{
	return (... && args);
}

template<std::integral... Args>
bool Any(Args... args)
{
	return (... || args);
}
```

标准模板库中的 `std::advance` 接受不同类型的迭代器 `iter` 和整数 `n`，并将迭代器前进 `n` 个位置。对于不同的迭代器行为不一样，比如 `std::forward_list` 只支持单向前进，`std::list` 支持双向前进，`std::vector` 支持随机访问。前面的两种对象的迭代器只能一步一步向前，时间复杂度是 $O(n)$，而 `std::vector` 的迭代器可以直接跳到目标位置，时间复杂度是 $O(1)$。因此 `std::advance` 需要根据迭代器类型来选择不同的实现。C++20 Concepts 可以很方便地实现重载能力，不同容器的迭代器类型不同，调用不同的函数模板实现。
```cpp
template<std::forward_iterator I>
void advance(I &it, int n)
{}

template<std::bidirectional_iterator I>
void advance(I &it, int n)
{}

template<std::random_access_iterator I>
void advance(I &it, int n)
{}
```

Concepts 可以用于模板的特化。下面例子中，如果是一个 regular 类型，那么会选择特化版本的 `MyVector`，否则选择普通版本的 `MyVector`。
```cpp
template<typename T>
class MyVector
{};

template<std::regular T>
class MyVector<T>
{};
```

Concepts 可以组合起来使用，比如下面的 `find`，要求迭代器 `Iter` 是一个输入迭代器（`input iterator`），并且要求迭代器的值类型 `value_type` 是一个可以和 `Val` 比较相等的类型（`equality comparable`）。
```cpp
template<typename Iter, typename Val>
requires std::input_iterator<Iter> && std::equality_comparable_with<std::iter_value_t<Iter>, Val>
Iter find(Iter b, Iter e, Val v)
{}

template<std::input_iterator Iter, typename Val>
requires std::equality_comparable_with<typename std::iterator_traits<Iter>::value_type, Val>
Iter find(Iter b, Iter e, Val v)
{}
```

## 受约束和不受约束的占位符
`auto` 是不受约束的占位符（`unconstrained placeholder`），而 Concepts 是受约束的占位符（`constrained placeholder`）。在任何使用 `auto` 的地方都可以使用 Concepts。比如下面的示例。
```cpp
std::integral auto GetIntegral(int val) { return val; }

int main()
{
	std::vector<int> vec { 1, 2, 3, 4, 5 };
	for (std::integral auto i : vec)
	{
		std::cout << i << " ";
	}
	std::cout << '\n';

	std::cout << std::boolalpha << '\n';

	std::integral auto b1 = true;
	std::cout << b1 << '\n';

	auto b2 = true;
	std::cout << b2 << '\n';

	std::integral auto integ1 = GetIntegral(10);
	std::cout << integ1 << '\n';

	auto integ2 = GetIntegral(10);
	std::cout << integ2 << '\n';

	return 0;
}
```

在 C++14 中 `auto` 只能用于 lambda 表达式的参数类型，在 C++20 中 `auto` 可以用于函数参数类型，此时函数自动变成了模板函数，当然这里也可以使用 Concepts 来约束参数类型。下面是之前的例子和对应的不受约束的 `auto` 版本。之前提到过 `gcd4` 中参数 `a` 和 `b` 是两个独立的模板参数，类型可以不同，类似的 `gcd5` 中参数 `a` 和 `b` 也是两个独立的模板参数，类型可以不同。
```cpp
std::integral auto gcd4(std::integral auto a, std::integral auto b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd4(b, a % b);
}

auto gcd5(auto a, auto b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd5(b, a % b);
}
```
作为模板函数，可以显式的指定参数类型，比如 `gcd5<uint32_t, uint32_t>(10, 20);`。

这种简写函数模板也可以重载，比如下面的例子。
```cpp
void Overload(auto t) { std::cout << "auto" << std::endl; }

void Overload(std::integral auto t) { std::cout << "std::integral auto" << std::endl; }

void Overload(long l) { std::cout << "long" << std::endl; }

int main()
{
	Overload(3.14);
	Overload(2026);
	Overload(2026L);

	return 0;
}
```

## 预定义 Concepts
不要重复发明轮子，C++20 提供了很多预定义的 Concepts，方便使用。下面是一些常用的预定义 Concepts。

首先讨论一下之前使用的 `std::three_way_comparable<T>`，它要求类型 `T` 支持三向比较（`three-way comparison`），假定 `a` `b` 都是 `T` 类型的对象，那么这个 Concepts 等价于

- `(a <=> b == 0) == bool(a == b)` 为 `true`
- `(a <=> b != 0) == bool(a != b)` 为 `true`
- `((a <=> b) <=> 0)` 和 `(0 <=> (a <=> b))` 是相当的
- `(a <=> b < 0) == bool(a < b)` 为 `true`
- `(a <=> b > 0) == bool(a > b)` 为 `true`
- `(a <=> b <= 0) == bool(a <= b)` 为 `true`
- `(a <=> b >= 0) == bool(a >= b)` 为 `true`

下面是几个和类型、类型之间的关系、类型分类相关的 Concepts。

- `std::default_initializable<T>`：类型 `T` 可以默认构造（`default constructible`）。
- `std::same_as<T, U>`：类型 `T` 和类型 `U` 是相同的类型。
- `std::derived_from<T, U>`：类型 `T` 是类型 `U` 的派生类。
- `std::convertible_to<T, U>`：类型 `T` 可以隐式或显式地转换为类型 `U`。
- `std::common_reference_with<T, U>`：类型 `T` 和类型 `U` 有一个公共的引用类型 `C`，这个类型 `C` 是编译器通过 `std::common_reference_t<T, U>` 推导出来的。
- `std::common_with<T, U>`：和 `std::common_reference_with<T, U>` 类似，不过 `C` 是编译器通过 `std::common_type_t<T, U>` 推导出来的，可能不是引用类型。
- `std::assignable_from<T, U>`：类型 `T` 可以被类型 `U` 赋值，其中 `T` 必须是一个左值引用类型（`lvalue reference type`），因此 `std::assignable_from<int, int>` 结果是 `false`，而 `std::assignable_from<int&, int>` 结果是 `true`。
- `std::swappable<T>`：类型 `T` 可以交换（`swappable`）。

下面一组是算术相关的 Concepts。

- `std::integral<T>`：`std::is_integral_v<T>`。
- `std::signed_integral<T>`：`std::integral<T> && std::is_signed_v<T>`。
- `std::unsigned_integral<T>`：`std::integral<T> && !std::is_signed_v<T>`。
- `std::floating_point<T>`：`std::is_floating_point_v<T>`。

下面是一组和声明周期相关的 Concepts。

- `std::destructible<T>`：类型 `T` 可以被销毁（`destructible`）。
- `std::constructible_from<T, Args...>`：类型 `T` 可以用类型 `Args...` 的参数构造（`constructible`）。
- `std::default_constructible<T>`：类型 `T` 可以默认构造（`default constructible`）。
- `std::move_constructible<T>`：类型 `T` 可以移动构造（`move constructible`）。
- `std::copy_constructible<T>`：类型 `T` 可以拷贝构造（`copy constructible`）。

下面是两个比较相关的 Concepts。

- `std::equality_comparable<T>`：类型 `T` 可以比较相等（`equality comparable`）。
- `std::total_ordering<T>`：类型 `T` 可以进行全序比较（`total ordering`）。
  - 这里假定 `a` `b` `c` 都是类型 `T` 的对象，那么这个 Concepts 等价于
    - `bool(a < b)` `bool(a > b)` `bool(a == b)` 只有一个是 `true`
    - 如果 `bool(a < b)` `bool(b < c)` 都是 `true`，那么 `bool(a < c)` 也是 `true`
    - `bool(a > b) == bool(b < a)` 为 `true`
    - `bool(a <=b) == !bool(b < a)` 为 `true`
    - `bool(a >= b) == !bool(a < b)` 为 `true`

下面几个和对象相关的 Concepts 稍微复杂一点，下面是它们的定义。`std::is_object_v<T>` 是一个类型 trait，判断类型 `T` 是否是对象类型（`object type`），要求是标量、数组、`union` 或者 `class` 类型。
```cpp
template<class T>
concept movable =
	std::is_object_v<T> && std::move_constructible<T> && std::assignable_from<T &, T> && std::swappable<T>;

template<class T>
concept copyable = std::copy_constructible<T> && std::movable<T> && std::assignable_from<T &, T &> &&
				   std::assignable_from<T &, const T &> && std::assignable_from<T &, const T>;

template<class T>
concept semiregular = std::copyable<T> && std::default_initializable<T>;

template<class T>
concept regular = std::semiregular<T> && std::equality_comparable<T>;
```

下面是几个调用相关的 Concepts。

- `std::invocable<F, Args...>`：类型 `F` 可以被调用（`invocable`），并且参数类型是 `Args...`。
- `std::regular_invocable<F, Args...>`：类似于 `std::invocable`，不过要求不修改函数参数，同样的输入保持相同的输出。
- `std::predicate<F, Args...>`：类型 `F` 可以被调用，并且返回值可以转换为 `bool`。

下面是一组迭代器相关的 Concepts，定义在 `<iterator>` 头文件中。

- `std::input_iterator<I>`
- `std::output_iterator<I, T>`
- `std::forward_iterator<I>`: 继承自 `std::input_iterator<I>`，要求迭代器可以多次遍历。支持 `++it` 和 `it++`，并且 `*it` 是一个左值引用类型，同时支持等于（`==`）和不等于（`!=`）比较。
- `std::bidirectional_iterator<I>`: 继承自 `std::forward_iterator<I>`，要求迭代器可以前进和后退。支持 `--it` 和 `it--`。
- `std::random_access_iterator<I>`: 继承自 `std::bidirectional_iterator<I>`，要求迭代器可以随机访问。支持 `it + n` `it - n` `it[n]` `it1 - it2` `it1 < it2` 等操作。
- `std::contiguous_iterator<I>`: 继承自 `std::random_access_iterator<I>`，要求迭代器指向的元素在内存中是连续的。

下面是三个与算法相关的 Concepts。

- `std::permutable<T>`：能够对元素进行就地重排（`in-place reordering`）。
- `std::mergeable`：能够将多个已排序的序列合并为一个输出序列。
- `std::sortable`：能够对元素进行排序。

### ranges
下面是几个与 range 相关的 Concepts。

- `std::ranges::range<T>`：range 表示一组元素，可以通过迭代器访问，提供了开始迭代器和结束哨兵。STL 的容器都是 range。
```cpp
template<class T>
concept range = requires(T &t) {
	ranges::begin(t); // equality-preserving for forward iterators
	ranges::end(t);
};
```

`std::ranges::input_range` `std::ranges::output_range` `std::ranges::forward_range` `std::ranges::bidirectional_range` `std::ranges::random_access_range` `std::ranges::contiguous_range` 分别对应迭代器的 Concepts，不再赘述。

下面是几个特殊的 `range`。

- `std::ranges::borrowed_range`：保证迭代器的生命周期与 range 的生命周期不绑定，range 销毁之后迭代器仍然有效。
- `std::ranges::common_range`：保证 `std::ranges::begin(t)` 和 `std::ranges::end(t)` 返回的类型相同。传统的 C++ 迭代器都是 `common_range`。
- `std::ranges::sized_range`：保证在 $O(1)$ 时间复杂度内通过迭代器差值获取 range 的大小。
- `std::ranges::viewable_range`：保证 range 可以通过 `std::views::all` 转换为一个 view。

`std::ranges::view` 是一个 range，能够常量时间拷贝、移动和赋值。
```cpp
template<class T>
concept view = ranges::range<T> && std::movable<T> && ranges::enable_view<T>;

template<class T>
constexpr bool enable_view = std::derived_from<T, view_base> || /*is-derived-from-view-interface*/<T>;

struct view_base
{};
```
视图（`view`）通常作用于某个 range 执行某种操作。视图不拥有（`own`）数据。视图应该是只读的、无状态的，具备相等性保持（`equality-preserving`）。STL 的容器时 range 但不是视图。

## 定义 Concepts
如果标准库没有预定义我们需要的 Concepts，我们可以自定义 Concepts。定义一个 Concepts 的语法如下：
```cpp
template<template-parameter-list>
concept concept-name = constraint-expression;
```
定义开始是 `template` 关键字，后面是模板参数列表（`template parameter list`），然后是 `concept` 关键字，后面是 Concepts 的名字，最后是一个约束表达式（`constraint expression`）。约束表达式可以是一个编译期的谓词，可以是以下两种形式之一：

- 其他 Concepts 或编译期谓词的逻辑组合
  - 逻辑组合可以使用析取（`||`）、合取（`&&`）、非（`!`）等逻辑运算符。
  - 编译期谓词是一个在编译期执行并返回布尔值的函数
- 一个 requires 表达式（`requires expression`）
  - 简单 requires（`simple requirement`）
  - 类型 requires（`type requirement`）
  - 组合 requires（`compound requirement`）
  - 嵌套 requires（`nested requirement`）

使用逻辑运算符组合 Concepts 或编译期谓词时要遵循短路原则，如果表达式的结果已经确定了，那么会自动停止求值。

类型萃取（`type trait`）是编译期的谓词，因此我们已经有了很多工具可以构建自己的 Concepts。
```cpp
template<typename T>
concept Integral = std::is_integral<T>::value;

template<typename T>
concept SignedIntegral = Integral<T> && std::is_signed<T>::value;

template<typename T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

template<typename T>
concept Arithmetic = std::is_integral<T>::value || std::is_floating_point<T>::value;
```

> [!WARNING]
> 定义 Concepts 的时候，不能再使用 requires 做约束了。下面是一个错误的例子。
> ```cpp
> template<typename T>
> concept AlwaysTrue = true;
>
> template<typename T>
> requires AlwaysTrue<T>
> concept Error = true;
> ```

requires 表达式的语法形式如下：
```cpp
requires (parameter-list[optional]) { requirement-seq }
```
`parameter-list` 是一个可选的参数列表，和函数的参数列表类似，`requirement-seq` 是一系列 requires 语句，可以是简单 requires、类型 requires、组合 requires 或嵌套 requires。

下面是一个简单 requires 的例子，要求类型 `T` 可以执行 `+` 操作。
```cpp
template<typename T>
concept Addable = requires(T a, T b) { a + b; };
```

对于类型 requires，不得不使用 `typnname` 关键字。比如下面的 Concepts 要求类型 `T` 有一个嵌套类型 `value_type`，并且可以用 `T` 来实例化 `Other` 模板类。
```cpp
template<typename T>
concept TypeRequirement = requires {
	typename T::value_type;
	typename Other<T>;
};
```

组合 requires 的语法如下。和简单 requires 类似，组合 requires 允许使用 `noexcept` 关键字，并且有一个函数类型。
```cpp
template<typename T>
concept Equal = requires(T a, T b) {
	{ a == b } -> std::convertible_to<bool>;
	{ != b } -> std::convertible_to<bool>;
};
```

嵌套 requires 的语法如下。
```cpp
requires constraint-expression;
```
下面这个例子给了简单写法和故意使用嵌套 requires 的写法。
```cpp
template<typename T>
concept UnsignedIntegral = Integral<T> && !SignedIntergral<T>;

template<typename T>
concept UnsignedIntegral = Integral<T> && requires(T) { requires !SignedIntegral<T>; };
```

### 更多示例
使用前面定义的 `Equal` Concepts 和嵌套 requires 的写法，可以定义 `Ordering` Concepts，要求类型 `T` 可以进行全序比较（`total ordering`）。
```cpp
template<typename T>
concept Ordering = Equal<T> && requires(T a, T b) {
	{ a <= b } -> std::convertible_to<bool>;
	{ a < b } -> std::convertible_to<bool>;
	{ a > b } -> std::convertible_to<bool>;
	{ a >= b } -> std::convertible_to<bool>;
};
```
C++20 有了三路比较运算符（`<=>`），更推荐使用 `std::three_way_comparable<T>` 预定义的 Concepts。


## requires 表达式
requires 表达式可以单独使用在需要一个编译期谓词的地方，比如 `static_assert` 或者 `if constexpr`，甚至是一个 requires 字句。先看 requires 字句的例子，第一个 `requires` 是字句的关键字，这里表示需要一个约束，第二个 `requires` 是一个 requires 表达式，表示约束的内容，是一个匿名的、临时的 Concepts。
```cpp
template<typename T>
requires requires(T x) { x + x; }
T add(T a, T b)
{
	return a + b;
}
```
下面是使用 `static_assert` 和 `if constexpr` 的例子。
```cpp
struct First
{
	int Count() const { return 1; }
};

struct Second
{
	int Size() const { return 2; }
};

template<typename T>
int GetCount(const T &obj)
{
	if constexpr (requires { obj.Count(); })
	{
		return obj.Count();
	}
	else if constexpr (requires { obj.Size(); })
	{
		return obj.Size();
	}
	else
	{
		return 0;
	}
}

int main()
{
	First first;
	Second second;

	static_assert(requires(First f) {
		{ f.Count() } -> std::convertible_to<int>;
	});

	// failed
	static_assert(requires(Second s) {
		{ s.Count() } -> std::convertible_to<int>;
	});

	return 0;
}
```
