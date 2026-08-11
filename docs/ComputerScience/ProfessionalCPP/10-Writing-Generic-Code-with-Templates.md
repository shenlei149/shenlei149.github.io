## 类模板
类是对象的蓝图，类模板是类的蓝图。类模板主要用于容器类和数据结构，用于存储对象。

如果我们需要写一个通用的棋盘类，但是不使用模版，就不得不使用多态。棋盘类的容器，比如是 `std::vector`，持有基类的指针，实际存放具体类。这种方法有很多弊端。首先不能按值存放，只能存放指针。更严重的问题是类型不安全，可以创建两个不同派生类的对象的指针放到棋盘类容器中。另外，需要向下做类型转换。最后，不能放一些原始类型。

使用模版可以解决上述问题。下面是模板解决方案：类模板。
```cpp
template<typename T>
class Grid
{
public:
	explicit Grid(std::size_t width = DefaultWidth, std::size_t height = DefaultHeight);
	virtual ~Grid() = default;

	Grid(const Grid &src) = default;
	Grid &operator=(const Grid &rhs) = default;
	Grid(Grid &&src) = default;
	Grid &operator=(Grid &&rhs) = default;

	std::optional<T> &At(std::size_t x, std::size_t y);
	const std::optional<T> &At(std::size_t x, std::size_t y) const;

	std::size_t
	GetWidth() const
	{
		return width_;
	}

	std::size_t
	GetHeight() const
	{
		return height_;
	}

public:
	static constexpr std::size_t DefaultWidth { 10 };
	static constexpr std::size_t DefaultHeight { 10 };

private:
	void VerifyCoordinate(std::size_t x, std::size_t y) const;

private:
	std::vector<std::optional<T>> cells_;
	std::size_t width_ { 0 };
	std::size_t height_ { 0 };
};
```
由于类模板要求声明和定义对用户都可见，因此习惯在一个文件，不过仍旧推荐声接口和实现分开，更易读。
```cpp
template<typename T>
Grid<T>::Grid(std::size_t width, std::size_t height)
	: width_ { width }
	, height_ { height }
{
	cells_.resize(width_ * height_);
}

template<typename T>
std::optional<T> &
Grid<T>::At(std::size_t x, std::size_t y)
{
	return const_cast<std::optional<T> &>(std::as_const(*this).At(x, y));
}

template<typename T>
const std::optional<T> &
Grid<T>::At(std::size_t x, std::size_t y) const
{
	VerifyCoordinate(x, y);
	return cells_[x + y * width_];
}

template<typename T>
void
Grid<T>::VerifyCoordinate(std::size_t x, std::size_t y) const
{
	if (x >= width_)
	{
		throw std::out_of_range { std::format("x ({}) must be less than width ({}).", x, width_) };
	}

	if (y >= height_)
	{
		throw std::out_of_range { std::format("y ({}) must be less than height ({}).", y, height_) };
	}
}
```
类模板需要实例化，如下所示。`Grid<T>` 就是一个具体的类，和类一样去使用即可。
```cpp
Grid<int>
Grid<double>
Grid<SpreadsheetCell>
Grid<const char *>
Grid<vector<int>>
```

模板的编译分成两步，第一步是语法检查，此时不知道 `T` 的具体类型，第二步是实例化，比如 `Grid<int>`。实例化分为隐式实例化和显式实例化。`Grid<int> xxx` 是隐式实例化，编译器生成 `virtual` 函数和使用到的非 `virtual` 函数，这称为选择性实例化（`selective instantiation`）。`template class Grid<string>` 是显式实例化，会生成所有的函数，可以帮助找到一些错误，即使这些函数没有被用到。当对 `T` 的对象进行操作时，相当于对 `T` 有要求，如果不满足，编译器会报错，不过信息相当晦涩，后面阐述的概念（`concept`）会改善这一点。

模板参数除了可以是类型之外，还能是整数类型、枚举、指针、引用等。整数和枚举都比较常见。比如前面的 `Grid` 类模板可以实现成 3 个模板参数的类模板 `template <typename T, std::size_t WIDTH, std::size_t HEIGHT>`，这样实例化的时候需要提供三个参数，由于 `WIDTH` `HEIGHT` 编译期确定，因此可以使用数组而不是 `std::vector` 保存数据。

类模板参数和普通参数一样，可以有默认值。

类模板参数推导能够简化代码。有的时候推导可能出现我们不期望的类型，此时可以通过用户自定义的推导规则来得到我们想要的结果。一般写法是
```cpp
template<...>
explicit TemplateName(Parameters)->DeducedTemplate<...>;
```
常见的是将传入的类型 `const char *` 修正成 `std::string`，比如 `ClassName(const char*) -> ClassName<std::string>`。详情可以参考[这里](../CPP17/Part2-Template-Features/09-Class-Template-Argument-Deduction.md#deduction-guides)

成员函数模板可以出现在类模板中，也可以在普通类中。这个函数自身也有模板参数。

对于类模板，`Grid<int>` 和 `Grid<double>` 是完全不同的类，因此不能将一个前者的对象赋值给后者，也不能用后者的对象构造前者，反之亦然。因为默认的拷贝构造和拷贝赋值定义默认类型是 `T`，类型必须一致。
```cpp
Grid(const Grid &src) = default;
Grid &operator=(const Grid &rhs) = default;
```
我们可以通过添加成员函数模板解决这个问题，函数模板参数类型可以和类模板类型不一样，这样就能在不同类型间进行拷贝和赋值。
```cpp
template<typename E>
Grid(const Grid<E> &src);
template<typename E>
Grid &operator=(const Grid<E> &rhs);

void Swap(Grid &other) noexcept;

template<typename T>
template<typename E>
Grid<T>::Grid(const Grid<E> &src)
	: Grid { src.GetWidth(), src.GetHeight() }
{
	for (size_t i = 0; i < width_; i++)
	{
		for (size_t j = 0; j < height_; j++)
		{
			At(i, j) = src.At(i, j);
		}
	}
}

template<typename T>
template<typename E>
Grid<T> &
Grid<T>::operator=(const Grid<E> &rhs)
{
	Grid<T> temp { rhs };
	Swap(temp);
	return *this;
}

template<typename T>
void
Grid<T>::Swap(Grid &other) noexcept
{
	std::swap(width_, other.width_);
	std::swap(height_, other.height_);
	std::swap(cells_, other.cells_);
}
```

与类模板一样，成员函数模板也支持非类型的参数。

C++23 引入了显式对象参数，配合函数模板，可以消除代码冗余。之前的实现有两个 `At`，尽管使用了 Meyers 提出的 `const_cast()` 模式避免重复代码，但是仍旧需要定义两个版本。下面使用成员函数模板解决这个问题。核心是 `this` 模板化。
```cpp
template<typename Self>
auto &&At(this Self &&self, std::size_t x, std::size_t y);

template<typename T>
template<typename Self>
auto &&
Grid<T>::At(this Self &&self, std::size_t x, std::size_t y)
{
	self.VerifyCoordinate(x, y);
	return std::forward_like<Self>(self.cells_[x + y * self.width_]);
}
```
这里有个中澳概念：`Self &&`。这本书称为转发引用，Meyers 称为通用引用，可以参考[这里](../EffectiveModernCpp/ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)。如果 `Self` 不是函数模板参数而是类模板参数，由于类模板参数编译时就确定好了，那么就是右值引用类型。转发引用可以绑定到引用、`const` 引用和右值引用，这取决于参数自身。`std::forward_like` 的使用使得返回类型也依赖于参数。比如 `Self &&` 是 `const Grid<T> &`，那么返回类型就是 `const std::optional<T> &`。

类模板特化是针对某些类型有特殊的实现。比如上面 `Grid<T>` 的例子，如果 `T` 是 `const char *`，明显不能仅仅存指针而不是需要存内容，那么可能就需要一个特殊的实现。
```cpp
template<>
class Grid<const char *>
```

一个类也可以继承一个类模板，它自身也是类模板。需要注意的是，在继承类模板中访问基类的成员时，需要使用 `this` 或者 `Base<T>::`。
```cpp
template<typename T>
class Derived : public Base<T>
```

对于类模板，给一个别名往往用起来更方便。比如
```cpp
template<typename T1, typename T2>
class MyClassTemplate
{
};

using OtherName = MyClassTemplate<int, double>;

template<typename T1>
using OtherName = MyClassTemplate<T1, double>;
```

## 函数模板
函数模板与类版本类似，比如下面实现了一个适用于任意类型的 `Find` 函数。调用时，可以用尖括号指定类型，或者让编译器自动推导。
```cpp
template<typename T>
std::optional<size_t>
Find(const T &value, const T *arr, size_t size)
{
	for (size_t i { 0 }; i < size; ++i)
	{
		if (arr[i] == value)
		{
			return i; // Found it; return the index.
		}
	}

	return {}; // Failed to find it; return empty optional.
}
```
有时，数组的大小在调用 `Find` 的上下文是包含在数组中而不是指针，那么我们可以额外提供一个版本的 `Find`，更易用。
```cpp
template<typename T, size_t N>
optional<size_t>
Find(const T &value, const T (&arr)[N])
{
	return Find(value, arr, N);
}
```
函数模板也可以特化，但是不常用，不如写一个重载函数，因为特化不参与重载解析，可能导致不期望的行为。当模板函数和非模板函数都满足时，倾向于调用非模板函数。回到前面的例子，对于 `const char *` 的查找需要使用 `std::strcmp` 而不是 `==`。
```cpp
std::optional<size_t>
Find(const char *value, const char **arr, size_t size)
{
	for (size_t i { 0 }; i < size; ++i)
	{
		if (std::strcmp(arr[i], value) == 0)
		{
			return i; // Found it; return the index.
		}
	}

	return {}; // Failed to find it; return empty optional.
}
```
类似普通函数，函数模板也可以是某个类的友元。

函数模板类型推导很方便，不过如果返回类型是模板参数类型，那么返回类型就必须指定，无法推导，这个可以让。可以从第一个开始往后部分指定模板参数类型。如何让编译器帮忙推导返回类型呢？返回类型那里不要使用模板参数，而是写 `auto` 或 `decltype(auto)`，前面会去掉 `const` 和引用修饰，更推荐后者。

问题来了，既然返回类型可以用 `auto` 自己推导，参数类型也可以推导，那能不能写 `auto` 呢？可以！下面就是函数模板的缩写语法糖版本。
```cpp
decltype(auto)
Add(const auto &t1, const auto &t2)
{
	return t1 + t2;
}
```
这种写法，每个类型都可能推导成不同类型，同时函数的实现无法做类型推导的功能，因为 `auto` 是匿名而不是 `T` 这样有名类型。如果想要类型一致，或想类型推导，那么还要写成复杂的形式。

## 变量模板
除此之外，还有变量模板，语法如下，这样可以得到与请求类型最接近的值。
```cpp
template<typename T>
constexpr T PI { T { 3.141592653589793238462643383279502884 } };
```

## 概念
概念（`concept`），C++20 引入的全新概念，为了对模版进行约束，报错信息更友好。

概念定义的语法如下。
```cpp
template<parameter-list>
concept concept-name = constraints-expression;
```
表达式必须是一个编译期确定的表达式，结果必须是布尔类型。`concept-name<argument-list>` 是一个概念表达式（`concept expression`），可以用于 `static_assert`。比如
```cpp
template<typename T>
concept Big = sizeof(T) > 4;

static_assert(!Big<char>);
static_assert(!Big<short>);
static_assert(Big<long double>);
```

概念的约束表达式引入了要求表达式（`requires expression`）这个概念。定义的语法如下
```cpp
requires (parameter-list) { requirements; }
```
参数列表是可选的，但是不能有默认参数。类似函数体内的表达式不能引入新的局部变量。`requirements;` 是一系列要求，以分号结尾。要求分为以下四类。

简单要求（`simple requirement`）。任意表达式，不是以 `requires` 开头，不能有变量声明，循环，条件表达式等等。下面要求 `T` 类型满足自增。
```cpp
template<typename T>
concept Incrementable = requires(T x) {
	x++;
	++x;
};
```

类型要求（`type requirement`）。要求类型满足某种要求，以 `typename` 开头。下面是两个实例，第一个要求类型有 `value_type` 字段，第二个要求能够实例化 `SomeTemplate` 类模板。
```cpp
template<typename T>
concept C = requires { typename T::value_type; };

template<typename T>
concept C = requires { typename SomeTemplate<T>; };
```

组合要求（`compound requirement`）。验证函数是否有异常、返回类型是否满足要求。语法如下
```cpp
{ expression } noexcept -> type-constraint;
```
`noexcept` `type-constraint` 均为可选。下面的例子要求类型 `T` 有不抛出异常的析构函数和 `Swap` 函数。
```cpp
template<typename T>
concept C = requires(T x, T y) {
	{ x.~T() } noexcept;
	{ x.Swap(y) } noexcept;
};
```
`->` 后面的类型约束接受零个或多个模板类型参数。默认情况将返回值类型作为第一个参数传递，因此写的时候类型约束会少一个模板参数。比如下面要求 `size()` 函数返回类型可以转换到 `size_t` 类型。
```cpp
template<typename T>
concept C = requires(const T x) {
	{ x.size() } -> convertible_to<size_t>;
};
```
`std::convertible_to<From, To>` 是标准库自带的概念，由于第一个模板参数默认是表达式的凡是类型，因此上面只写了 `To` 的模板类型为 `size_t`。

嵌套要求（`nested requirements`）。比如
```cpp
template<typename T>
concept C = requires(T t) {
	++t;
	--t;
	t++;
	t--;
	requires sizeof(t) == 4;
};
```

概念可以使用 `&&` `||` 组合起来。比如
```cpp
template<typename T>
concept IncrementableAndDecrementable = Incrementable<T> && Decrementable<T>;
```

标准库在 `<concept>` 头文件中提供了大量已经定义好的概念，均在 `std` 命名空间下。和语言相关的有 `same_as`, `derived_from`, `convertible_to`, `integral`,
`floating_point`, `copy_constructible`。比较相关的概念 `equality_comparable`, `totally_ordered`。对象相关的 `movable`, `copyable`。调用对象 `invocable`, `predicate`。迭代器 `<iterator>` 相关 `random_access_iterator`, `forward_iterator`, `incrementable`, `indirectly_copyable`, `indirectly_swappable`。范围（`range`）相关的稍后在对应章节会体积。推荐使用标准库提供的概念或者组合起来使用。

在 `auto` 前面写想要约束的概念名就可以约束自动推导的类型，报错更容易理解，代码也更容易读。

对于模版函数的参数类型约束有下面几种形式。不过对于参数名字做约束，或者是类模板内直接定义的成员函数，必须使用后缀 `requires` 的语法。
```cpp
template<typename T>
	requires std::convertible_to<T, bool>
void process(const T &t);

template<convertible_to<bool> T>
void process(const T &t);

void process(const Incrementable auto &t);
```

类模板约束类似。
```cpp
template<typename T>
	requires std::derived_from<T, GamePiece>
class GameBoard : public Grid<T>
{
};

template<std::derived_from<GamePiece> T>
class GameBoard : public Grid<T>
```

类的成员函数可以有附加的约束条件，比如 `GameBoard` 的移动函数要求 `T` 能移动。
```cpp
void move(std::size_t xSrc, std::size_t ySrc, std::size_t xDest, std::size_t yDest)
	requires std::movable<T>;
```

约束可以用于函数模版重载、类模板的特化。比如
```cpp
template<typename T>
optional<size_t>
Find(const T &value, const T *arr, size_t size)
{
}

template<std::floating_point T>
optional<size_t>
Find(const T &value, const T *arr, size_t size)
{
}
```
