
## 聚合初始化
聚合体（`aggregate`）是一个数组或一个类（`class`、`struct` 或 `union`）。如果一个类满足如下属性，则支持聚合初始化（`aggregate initialization`）：

- 没有 `private` 或 `protected` 的非静态数据成员。
- 没有用户自定义或者继承的构造函数。
- 没有虚基类、`private` 或 `protected` 的基类。
- 没有虚函数。

聚合初始化就是依次、按序初始化聚合体的非静态数据成员，或者数组的元素。
```cpp
struct Point3D
{
	int x;
	int y;
	int z;
};

Point3D p1 { 1, 2, 3 };
```

指定初始化（`designated initialization`）是 C++20 新增的特性，允许在聚合初始化中指定成员变量的名称来进行初始化，可以跳过某些成员变量的初始化，未指定的成员变量会被默认初始化，但是仍旧不能乱序初始化。同时，指定初始化会检查窄化转换（`narrowing conversion`），如果存在窄化转换，则会报错。
```cpp
Point3D p2 { .x = 1, .y = 2, .z = 3 };

Point3D p3 { .x = 1, .z = 3 };

// designator order for field ‘Point3D::x’ does not match declaration order in ‘Point3D’
Point3D p4 { .y = 2, .z = 3, .x = 1 };

// narrowing conversion of ‘3.5e+0’ from ‘double’ to ‘int’ [-Wnarrowing]
Point3D p5 { .x = 1, .y = 2, .z = 3.5 };
```

## 三路比较
`<=>` 运算符是 C++20 新增的三路比较运算符，也称为飞船运算符（`spaceship operator`）。它的出现是为了解决之前自定义类型中要重载六个比较运算符（`<`, `>`, `<=`, `>=`, `==`, `!=`）的问题。

C++20 中，等值比较的重载 `operator==` 自动会生成 `operator!=`，同时可以写 `= default`，简化实现。

重载 `operator<=>` 会自动生成六个比较运算符，这个重载也可以写 `= default`。成员函数必须是 `const`，参数是 `const` 左值引用，返回值类型是 `std::strong_ordering`、`std::weak_ordering` 或者 `std::partial_ordering`，也可以是 `auto`，编译器会根据成员变量的类型自动推导返回值类型。

### 比较类别
C++20 定义了三种比较类别（`comparison categories`）：

- `std::strong_ordering`：强序。
- `std::weak_ordering`：弱序。
- `std::partial_ordering`：偏序。

下面三种属性用于区别这些比较类别。

- 关系运算符：`T` 必须支持所有的六种关系运算符 `<`, `>`, `<=`, `>=`, `==`, `!=`。
- 等值：所有相等的对象都无法区分了。
- 可比较性：对于任意两个对象 `a` 和 `b`，`a < b` `a == b` `a > b` 这三者必有其一为 `true`。

强序满足这三个属性，比如 `int`、`std::string` 等类型。弱序不满足等价性，比如字符串忽视大小写的比较，`hello` 和 `Hello` 是相等的，但是它们还可以被区分。偏序仅满足第一个属性，支持六种关系运算符，比如浮点数的比较，`NaN` 和 一个数值比较，三种比较方式结果都是 `false`，同时 `+0.0` 和 `-0.0` 是相等的，但是它们可以被区分，符号位不同。因此，强序隐含着弱序，弱序隐含着偏序。

这三类比较都提供了几个枚举值表示比较的结果。`std::strong_ordering` 提供了 `less`、`equal`、`greater` 三个枚举值，`std::weak_ordering` 提供了 `less`、`equivalent`、`greater` 三个枚举值，`std::partial_ordering` 提供了 `less`、`equivalent`、`greater`、`unordered` 四个枚举值。`unordered` 表示无法比较，也就是说其他三个结果都是 `false` 的情况。

### 使用
可以把结果与 0 进行比较，也可以和枚举值进行比较。
```cpp
int main()
{
	int a = 2026;
	int b = 1970;
	auto cmp = a <=> b;
	if (cmp < 0)
	{
		std::print("{} < {}\n", a, b);
	}
	else if (cmp > 0)
	{
		std::print("{} > {}\n", a, b);
	}
	else
	{
		std::print("{} == {}\n", a, b);
	}

	if (cmp == std::strong_ordering::less)
	{
		std::print("{} < {}\n", a, b);
	}
	else if (cmp == std::strong_ordering::greater)
	{
		std::print("{} > {}\n", a, b);
	}
	else
	{
		std::print("{} == {}\n", a, b);
	}

	return 0;
}
```

编译器生成的比较运算符隐含着 `constexpr` 和 `noexcept`，因此可以在编译期进行比较，同时会默认生成的函数比较逻辑是逐个对非 `static` 成员变量进行比较，对于容器也是类似的。因此对于 `string`、`vector` 等类型可以手动优化，因为可以先考虑容器的大小。标准委员会已经有提案来解决这个问题了。
```cpp
struct Basics
{
	int i;
	char c;
	float f;
	double d;

	auto operator<=>(const Basics &) const = default;
};

struct Arrays
{
	int ai[1];
	char ac[2];
	float af[3];
	double ad[2][2];

	auto operator<=>(const Arrays &) const = default;
};

struct Bases
	: Basics
	, Arrays
{
	auto operator<=>(const Bases &) const = default;
};

int main()
{
	constexpr Bases a = {
		{ 0,	 'c',		  1.f,			   1.						  },
		{ { 1 }, { 'a', 'b' }, { 1.f, 2.f, 3.f }, { { 1., 2. }, { 3., 4. } } }
	};
	constexpr Bases b = {
		{ 0,	 'c',		  1.f,			   1.						  },
		{ { 1 }, { 'a', 'b' }, { 1.f, 2.f, 3.f }, { { 1., 2. }, { 3., 4. } } }
	};

	static_assert(a == b);
	static_assert(!(a != b));
	static_assert(!(a < b));
	static_assert(a <= b);
	static_assert(!(a > b));
	static_assert(a >= b);

	return 0;
}
```

### 表达式重写
编译器会对比较运算的表达式进行重写，比如 `a < b` 会被重写为 `(a <=> b) < 0`，也就是说 `a OP b` 会被重写为 `(a <=> b) OP 0`，如果 `a` 和 `b` 的类型是不同的并且 `a` 的类型不能转换为 `b` 的类型，那么编译器还会尝试改写成 `0 OP (b <=> a)`。这样做的好处是可以减少比较运算符的重载数量，同时应用范围能够很广。比如下面的例子，默认实现自己和自己的比较，自行实现与 `int` 的比较，那么 `MyInt OP MyInt` 和 `MyInt OP int` `int OP MyInt` 都可以使用。
```cpp
class MyInt
{
public:
	explicit MyInt(int value)
		: value_(value)
	{}

	auto operator<=>(const MyInt &other) const = default;

	constexpr auto operator<=>(const int& other) const { return value_ <=> other; }

private:
	int value_;
};

int main()
{
	MyInt a(5);
	int b = 10;
	std::print("a < b: {}\n", a < b);
	std::print("a > b: {}\n", a > b);

	return 0;
}
```

如果定义了 `operator==`，那么 `operator!=` 会自动生成，反之定义了 `operator!=`，那么不会自动合成 `operator==`，而是从 `operator<=>` 中推导出 `operator==`。如果自定义了某种比较运算符，比如 `operator<`，那么使用自定义的函数而不是 `operator<=>`。
