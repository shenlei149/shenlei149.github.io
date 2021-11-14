## 运算符重载概述
C++ 允许我们重载运算符，比如 `+` `<` `*` `<<`，常见的运算符，`->` `*` 解引用的运算符，`[]` `()` 这样数组索引、函数调用运算符，还有转化运算符、分配回收内存的运算符等等。

重载运算符的好处：1）使我们的自定义类型更像是内置类型，也更容易理解；2）更好的控制程序，比如重载内存相关的运算符。重载运算符不是为了类的开发者，而是为了使用者！

重载运算符有一些限制：不能新增运算符；`.` `.*` 对象成员访问、`::` 范围限定、`?:` 条件运算符不能重载；除了 `()` 函数调用、`new` `delete`、C++23 引入的 `[]` 之外，不能修改运算符的参数个数；不能修改运算符的优先级和结合性；不能用于内置类型，至少有一个参数是用户自定义类型。另外，有些运算符有不同的含义，比如 `operator-` 是负号还是减号、`*` 是乘号也可以解引用指针，`<<` 流插入运算符也是左移运算符，那么可以重载所有的含义。

当决定重载一个运算符，有许多点需要决策。

第一点是成员函数还是全局函数。有些算子必须是成员函数，有些算子必须是全局函数，有些算子两者皆可。如果两者都行，推荐是成员函数，因为可以标记为 `virtual`，可以利用继承体系的优势，还可以直接访问私有变量而无需引入 `friend` 函数。如果实现为全局函数，那么应该与自定义类在一个命名空间。

第二个点是参数类型。首先参数个数有很大的限制。虽然类型可以选择的余地很大，但是合理的选择并不多，比如对 `T` 类型实现 `+`，明显两个 `std::string` 不是合理的选择。

第三点是返回类型。返回类型并不影响重载决议。可以写任意类型，但是这不意味着应该做，有意义才是第一位要考虑的。

最后一点是行为，一定要有意义，语义直观。

有几个运算符可以被重载，但是不应该重载。首先是 `&` 取地址，会导致语义不明确；其次是 `&&` `||`，因为运算前两个操作数都必须先求值，丢失了短路效果；最后是 `,` 序列操作符，从左往右求值，很少有合理的原因需要重载这个。

下表总结了所有可以被重载的运算符，并且给出了相关的建议。`T` 是当前实现运算符重载的类型，`E` 是其他类型。这里没有给出全部可能的组合。

| OPERATOR | NAME OR CATEGORY | MEMBER FUNCTION OR GLOBAL FUNCTION | WHEN TO OVERLOAD | SAMPLE PROTOTYPES |
|--|--|--|--|--|
| `operator+` <br> `operator-` <br> `operator*` <br> `operator/` <br> `operator%` | Binary arithmetic | Global function recommended | Whenever you want to provide these operations for your class | `T operator+(const T&, const T&);` <br> `T operator+(const T&,const E&);` |
| `operator-` <br> `operator+` <br> `operator~` | Unary arithmetic and bitwise operators | Member function recommended | Whenever you want to provide these operations for your class | `T operator-() const;` |
| `operator++` <br> `operator--` | Pre-increment and pre-decrement | Member function recommended | Whenever you overload `+=` and `-=` taking an arithmetic argument (`int`, `long`, ...) | `T& operator++();` |
| `operator++` <br> `operator--` | Post-increment and post-decrement | Member function recommended Whenever you overload `+=` and `-=` taking an arithmetic argument (`int`, `long`, ...) | `T operator++(int);` |
| `operator=` | Assignment operator | Member function required | Whenever your class has dynamically allocated resources, or members that are references |`T& operator=(const T&);` |
| `operator+=` <br> `operator-=` <br> `operator*=` <br> `operator/=` `operator%=` | Shorthand / compound arithmetic assignment operator | Member function recommended | Whenever you overload the binary arithmetic operators and your class is not designed to be immutable | `T& operator+=(const T&);` <br> `T& operator+=(const E&);` |
| `operator<<` <br> `operator>>` <br> `operator&` <br> `operator\|` <br> `operator^` | Binary bitwise operators | Global function recommended | Whenever you want to provide these operations | `T operator<<(const T&, const T&);` <br> `T operator<<(const T&, const E&);` |
| `operator<<=` <br> `operator>>=` <br> `operator&=` <br> `operator\|=` <br> `operator^=` | Shorthand / compound bitwise assignment operator | Member function recommended | Whenever you overload the binary bitwise operators and your class is not designed to be immutable | `T& operator<<=(const T&);` <br> `T& operator<<=(const E&);` |
| `operator<=>` | Three-way comparison operator | Member function recommended | Whenever you want to provide comparison support for your class; if possible, this should be defaulted using `=default` | `auto operator<=>(const T&) const = default;` <br> `partial_ordering operator<=>(const E&) const;` |
| `operator==` | Binary equality operator | Post-C++20: member function recommended <br> Pre-C++20: global function recommended | Whenever you want to provide comparison support for your class, and you cannot default the three-way comparison operator | `bool operator==(const T&) const;` <br> `bool operator==(const E&) const;` <br> `bool operator==(const T&, const T&);` <br> `bool operator==(const T&, const E&);` |
| `operator!=` | Binary inequality operator | Post-C++20: member function recommended <br> Pre-C++20: global function recommended | Post-C++20: not needed as the compiler automatically provides `!=` when `==` is supported <br> Pre-C++20: Whenever you want to provide comparison support for your class | `bool operator!=(const T&) const;` <br> `bool operator!=(const E&) const;` <br> `bool operator!=(const T&, const T&);` <br> `bool operator!=(const T&, const E&);` |
| `operator<` <br> `operator>` <br> `operator<=` <br> `operator>=` | Binary comparison operators | Global function recommended | Whenever you want to provide these operations; not needed when `<=>` is provided | `bool operator<(const T&, const T&);` <br> `bool operator<(const T&, const E&);` |
| `operator<<` <br> `operator>>` | I/O stream operators (insertion and extraction) | Global function required | Whenever you want to provide these operations | `ostream& operator<<(ostream&, const T&);` <br> `istream& operator>>(istream&, T&);` |
| `operator!` | Boolean negation operator | Member function recommended | Rarely; use `bool` or `void*` conversion instead | `bool operator!() const;` |
| `operator&&` <br> `operator||` | Binary Boolean operators | Global function recommended | Rarely, if ever, because you lose short-circuiting; it's better to overload `&` and `|` instead, as these never short-circuit | `bool operator&&(const T&, const T&);` |
| `operator[]` | Subscripting (array index) operator | Member function required | When you want to support subscripting | `E& operator[] (size_t);` <br> `const E& operator[] (size_t) const;` |
| `operator()` | Function call operator | Member function required | When you want objects to behave like function pointers | Return type and parameters can vary; see later examples in this chapter |
| `operator type()` | Conversion, or cast, operators (separate operator for each type) | Member function required When you want to provide conversions from your class to other types | `operator double() const;` |
| `operator ""_x` | User-defined literal operator | Global function required | When you want to support user defined literals | `T operator""_i(long double d);` |
| `operator new` <br> `operator new[]` | Memory allocation routines | Member function recommended | When you want to control memory allocation for your classes (rarely) | `void* operator new(size_t size);` <br> `void* operator new[](size_t size);` |
| `operator delete` <br> `operator delete[]` | Memory deallocation routines | Member function recommended | Whenever you overload the memory allocation routines (rarely) | `void operator delete(void* ptr) noexcept;` <br> `void operator delete[](void* ptr) noexcept;` |
| `operator*` <br> `operator->` | Dereferencing operators | Member function recommended for `operator*` <br> Member function required for `operator->` | Useful for smart pointers | `E& operator*() const;` <br> `E* operator->() const;` |
| `operator&` | Address-of operator | N/A | Never | N/A |
| `operator->*` | Dereference pointer-to-member | N/A | Never | N/A |
| `operator,` | Comma operator | N/A | Never | N/A |

上面的表中的例子并没有右值引用的例子。对于大部分运算符，普通的左值引用和右值引用都是有意义的，当然实际要依赖于类本身。比如 `std::string` 的 `operator+` 有四种重载，对于右值的重载，由于传入对象不再使用了，有可能可以使用对应的内存而提升性能。
```cpp
std::string operator+(const std::string &lhs, const std::string &rhs); // No memory reuse.
std::string operator+(std::string &&lhs, const std::string &rhs);	   // Can reuse memory of lhs.
std::string operator+(const std::string &lhs, std::string &&rhs);	   // Can reuse memory of rhs.
std::string operator+(std::string &&lhs, std::string &&rhs); // Can reuse memory of lhs or rhs.
```

运算符有优先级，个人经验是记住乘除大于加减，逻辑运算、比较运算比算数运算低，其他时候如果不确定，加括号。结合性指的是先处理哪一个操作数，大部分是从左往右的。只有类似 `+=` `++x` `*x` `&x` `(T)` 这些是从右往左。

## 重载算数运算符
这里需要指出一点，运算符前缀 `++` 和 后缀 `++` 有些许区别，后缀版本需要传入参数，同时返回旧值，这样就无法返回对象的引用而是一个旧的对象。

## 重载流的输入输出 `<<` `>>`
一般使用全局函数，并且返回流的引用，这样可以链式调用。

## 重载下标运算符 `[]`
一般而言，需要提供两个版本，返回 `T &` 和 `const T &`。

C++23 开始支持多个参数，那么想实现多维数据类、矩阵类等很非常方便。

一般情况下 `[]` 的参数类型是整数，不过也可以是其他类型，比如 `std::map` 中的 `[]` 运算符参数就是 `TKey`。

C++23 还支持 `[]` 标记为 `static`，这里给出一个例子，将枚举转成 `std::string`。
```cpp
enum class Figure
{
	Diamond,
	Heart,
	Spade,
	Club
};

class FigureEnumToString
{
public:
	static constexpr string_view
	operator[](Figure figure) noexcept
	{
		switch (figure)
		{
		case Figure::Diamond:
			return "Diamond";
		case Figure::Heart:
			return "Heart";
		case Figure::Spade:
			return "Spade";
		case Figure::Club:
			return "Club";
		}
	}
};

int
main()
{
	Figure f { Figure::Spade };
	FigureEnumToString converter;
	println("{}", converter[f]);
	println("{}", FigureEnumToString {}[f]);
}
```

## 重载函数运算符 `()`
参数个数和类型都可以随意，不过实际操作中有意义更重要。

C++23 也允许 `()` 重载是 `static` 的。

## 重载转化运算符
一个方便的写法是使用 `auto`，如果想返回引用类型，使用 `auto&`。
```cpp
operator auto() const
{ /* ... */
}

operator const auto &() const
{ /* ... */
}
```

C++11 之后推荐写 `explicit` 在关键字 `operator` 之前，好处是可以在某些场景下避免歧义。

由于 C++ 会自动将 `bool` 转成 `int`，进而可以转成很多整数类型。因此很多场景下一般会实现 `operator void*()` 而不是 `operator bool()`。
