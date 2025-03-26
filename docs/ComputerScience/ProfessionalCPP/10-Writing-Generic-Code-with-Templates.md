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
