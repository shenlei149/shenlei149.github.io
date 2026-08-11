## 模版参数
当使用容器或其他模板类型作为模板参数的时候，这就涉及了模板模板参数（`template template parameter`）。比如之前实现的 `Grid` 类，现在修改为可以指定使用什么容器来存放数据。
```cpp
template<typename T,
		 template<typename E, typename Allocator = std::allocator<E>> class Container = std::vector>
class Grid
{
public:
	std::optional<T> &at(std::size_t x, std::size_t y);
	const std::optional<T> &at(std::size_t x, std::size_t y) const;

private:
	Container<std::optional<T>> m_cells;
};
```
语法如下
```cpp
template<..., template<TemplateTypeParams> class ParameterName, ...>

template<..., template<TemplateTypeParams> typename ParameterName, ...>
```

利用非类型模版参数，可以给存储在 `Grid` 里的元素给一个默认值。不过对于非类型模板参数，仅支持很少的类型，那么这个 `Grid` 的适用范围也就小了很多。
```cpp
template<typename T, T DEFAULT = T {}>
class Grid
{
};
```

## 类模板偏特化
与完全的模板特化不同，偏特化仅仅特化一些模板参数。比如下面的例子，对存储 `const char *` 类型做了特殊处理。
```cpp
template<typename T, std::size_t WIDTH, std::size_t HEIGHT>
class Grid
{
};

template<std::size_t WIDTH, std::size_t HEIGHT>
class Grid<const char *, WIDTH, HEIGHT>
{
};
```
偏特化的威力在于能够为一类子类型进行特化，比如下面的例子，为所有的指针类型特化处理。
```cpp
template<typename T>
class Grid
{
};

template<typename U>
class Grid<U *>
{
};
```

模板函数不支持偏特化，但是可以通过函数重载写另一个模板函数来模拟偏特化。

## 模板递归
类似于函数递归，模板也可以递归。假定我们需要一个三维棋盘，一种做法是写一个一维棋盘，然后二维棋盘的模板参数是一维棋盘，三维棋盘的模板参数是一个二维棋盘，类似于 `std::vector<std::vector<std::vector<int>>>`。还有一种做法是棋盘 `Grid` 支持一个整数参数，指定维度，内部容器的元素是一个第一个维度的 `Grid`，最后和函数递归一样，特化 `size = 1` 作为终止条件。
```cpp
template<typename T, std::size_t N>
class NDGrid final
{
private:
	std::vector<NDGrid<T, N - 1>> elements_;
};

template<typename T>
class NDGrid<T, 1> final
{
private:
	std::vector<T> elements_;
};
```

## 可变参数模板
可变参数模板（`variadic template`）可以支持任意多的模板参数，包括零个。`typename...` 是一个参数包（`parameter pack`）。
```cpp
template<typename... Types>
class VariadicTemplate
{
};

VariadicTemplate<int> instance1;
VariadicTemplate<std::string, double, std::vector<int>> instance2;
VariadicTemplate<> instance3;
```

不能直接遍历可变参数模板的各个参数，需要使用模板递归或者折叠表达式。

可变模板参数创建了一个类型安全的可变长度（`type-safe variable-length`）的参数列表。下面的例子是依次处理各个参数。`std::forward<Tn>(args)...` 是 `...` 的另一个用途，是参数包展开（`parameter pack expansion`），它依次将 `args` 的参数传入函数，以逗号分隔，等价于 `std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), ...`。这里 `std::forward` 是完美转发，保持参数以左值或右值的形式传递给函数。类型安全是说，如果传递了 `HandleValue` 无法处理的类型的参数，那么会编译报错。`sizeof...` 是一个运算符，计算参数包参数的个数。
```cpp
void
HandleValue(int value)
{
}

void
HandleValue(double value)
{
}

void
HandleValue(const std::string &value)
{
}

template<typename T1, typename... Tn>
void
ProcessValues(T1 &&arg1, Tn &&...args)
{
	HandleValue(std::forward<T1>(arg1));
	if constexpr (sizeof...(args) > 0)
	{
		ProcessValues(std::forward<Tn>(args)...);
	}
}
```

使用参数包可以继承若干个类，也就是说，将它们混合起来。
```cpp
class Mixin1
{
public:
	explicit Mixin1(int i)
		: value_ { i }
	{
	}

	virtual void
	Mixin1Func()
	{
		std::println("Mixin1: {}", value_);
	}

private:
	int value_;
};

class Mixin2
{
public:
	explicit Mixin2(int i)
		: value_ { i }
	{
	}

	virtual void
	Mixin2Func()
	{
		std::println("Mixin2: {}", value_);
	}

private:
	int value_;
};

template<typename... Mixins>
class MyClass : public Mixins...
{
public:
	explicit MyClass(const Mixins &...mixins)
		: Mixins { mixins }...
	{
	}

	virtual ~MyClass() = default;
};

MyClass<Mixin1, Mixin2> a { Mixin1 { 11 }, Mixin2 { 22 } };
a.Mixin1Func();
a.Mixin2Func();

MyClass<Mixin1> b { Mixin1 { 33 } };
b.Mixin1Func();
// b.mixin2Func(); // Error: does not compile.

MyClass<> c;
// c.mixin1Func(); // Error: does not compile.
// c.mixin2Func(); // Error: does not compile.
```

折叠表达式可以方便的处理参数包。折叠表达式分为一元和二元，区别在于是否有参数包以外的元素参与运算，还可以分为左折叠和右折叠，这里表示从哪边开始结合。

比如前面的 `ProcessValues` 可以不使用递归处理。
```cpp
template<typename... Tn>
void
ProcessValues(Tn &&...args)
{
	(HandleValue(std::forward<Tn>(args)), ...);
}
```

这里是对 `,` 逗号进行一元右折叠，展开形式是
```cpp
(HandleValue(std::forward<A1>(a1)),
 HandleValue(std::forward<A2>(a2)),
 HandleValue(std::forward<A3>(a3)))
```

下面是对 `+` 进行左折叠的例子。
```cpp
template<typename... Values>
auto
SumValues(const Values &...values)
{
	return (... + values);
}

template<typename T, typename... Values>
auto
SumValues(const T &init, const Values &...values)
{
	return (init + ... + values);
}
```

折叠表达式还可以用于概念，比如下面就要求求和的类型都是相同的。
```cpp
template<typename T, typename... Us>
concept SameTypes = (std::same_as<T, Us> && ...);

template<typename T, typename... Values>
	requires SameTypes<T, Values...>
auto
SumValues(const T &init, const Values &...values)
{
	return (init + ... + values);
}
```

详细信息可以参考[这里](../CPP17/Part2-Template-Features/11-Fold-Expressions.md)。

## 元编程
首先，我们看几个元编程的例子。

第一个是在编译期求斐波那契数列，这里用到了模板递归。如果仅仅是追求编译期求值，那么 `consteval` 函数也
```cpp
template<int f>
class Factorial
{
public:
	static constexpr unsigned long long value { f * Factorial<f - 1>::value };
};

template<>
class Factorial<0>
{
public:
	static constexpr unsigned long long value { 1 };
};

int
main()
{
	std::println("{}", Factorial<6>::value);
}
```

下面看一个循环展开的例子，不过这里仅仅是示例，循环展开最好还是通过编程技巧让编译器帮我们做。
```cpp
template<int i>
class Loop
{
public:
	template<typename FuncType>
	static void
	Run(FuncType func)
	{
		Loop<i - 1>::Run(func);
		func(i);
	}
};

template<>
class Loop<0>
{
public:
	template<typename FuncType>
	static void
	Run(FuncType /* func */)
	{
	}
};

void
DoWork(int i)
{
	std::println("DoWork({})", i);
}

int
main()
{
	Loop<3>::Run(DoWork);
}
```

## 类型萃取
类型萃取（`type traits`）可以得到很多与类型相关的信息，并且提供了很多功能可以方便的使用。

第一类是判断类型的分类，比如 `is_integral<>` `is_class<>`，可以通过 `::value` 获取编译期的布尔值。对应的有一组 `_v` 结尾的定义，可以直接返回布尔值。

第二类是判断两个类型的关系，比如 `is_same` `is_base_of` `is_convertible` 等。

条件判断 `std::conditional`，恰当的使用我们可以做出很多事情，比如下面这个例子，`is_nothrow_move_assignable_v` 可以知道移动赋值是不是不抛出异常，不同条件得到不同的类型，那么 `a = move_assign_if_noexcept(b)` 会根据类型提供的方法选择调用移动或者是赋值。
```cpp
template<typename T>
constexpr std::conditional<std::is_nothrow_move_assignable_v<T>, T &&, const T &>::type
move_assign_if_noexcept(T &t) noexcept
{
	return std::move(t);
}
```

修改类型，比如 `add_const` `remove_pointer_t`，这里说明一下，后缀 `_t` 直接返回类型而无需使用 `::type`。

`enable_if` 的使用是基于 `substitution failure is not an error (SFINAE)` 的，也可以使用 `if constexpr` 解决问题。C++20 之后，模板函数可以使用 `requires` 解决。

逻辑运算，合取、析取、取反分别是 `conjunction` `disjunction` `negation`。

`static_assert` 也很有用，编译期可以检查一些事情。
