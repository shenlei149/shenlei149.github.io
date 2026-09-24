
## 非类型模板参数
C++ 支持非类型参数（`non-type`）作为模板参数，比如整数和枚举值，指向对象、函数、类属性（成员变量）的指针，左值引用，`std::nullptr_t` 等。

C++20 支持使用浮点数（`float` `double`）作为非类型模板参数。

C++20 支持字面量类型（`literal type`）作为非类型模板参数，前提是满足条件：

- 所有基类和非 `static` 的数据成员是 `public` 且非可变的
- 所有基类和非 `static` 的数据成员的类型是结构化类型（`structural type`）或者这些类型的数组
- 有一个 `constexpr` 构造函数

下面是一个示例：
```cpp
struct Point
{
	int x; // public, non-mutable, int is structural type
	int y;

	constexpr Point(int x, int y)
		: x(x)
		, y(y)
	{} // constexpr constructor
};

template<Point P>
struct Canvas
{};

Canvas<Point { 1, 2 }> c; // ok for C++20

struct BadPoint
{
private:
	int x; // private member

public:
	mutable int y; // mutable member

	constexpr BadPoint(int x, int y)
		: x(x)
		, y(y)
	{}
};
```

C++20 开始，编译期的字符串字面量（`string literal`）也可以作为非类型模板参数。
```cpp
template<size_t N>
struct FixedString
{
	char buf[N] {};

	constexpr FixedString(const char (&str)[N]) { std::copy_n(str, N, buf); }
};

template<FixedString Str>
void PrintTag()
{
	std::cout << "Tag: " << Str.buf << "\n";
}

PrintTag<"HTTP_200_OK">();
```
