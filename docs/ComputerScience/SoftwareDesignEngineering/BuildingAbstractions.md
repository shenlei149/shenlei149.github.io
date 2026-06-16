抽象在整个计算机领域都扮演着重要的角色，在软件设计中也不例外。好的抽象是控制复杂性的关键，想要构建好抽象、用好抽象需要我们掌握一定的技巧，这个过程有很多微妙的细节，正因如此，给我们感觉更像是一门艺术而不是科学。

## 遵循抽象的预期行为
先看一个经典的例子，首先给出 `Rectangle` 类的定义：
```cpp
class Rectangle
{
public:
	virtual ~Rectangle() = default;

	int GetWidth() const { return width_; }

	int GetHeight() const { return height_; }

	virtual void SetWidth(int width) { width_ = width; }

	virtual void SetHeight(int height) { height_ = height; }

	virtual int GetArea() const { return width_ * height_; }

private:
	int width_ = 0;
	int height_ = 0;
};
```
实现很简单，也非常直接。下面继承出一个 `Square` 类，从数学角度看，正方形是矩形的一种特殊情况。由于正方形的宽和高必须相等，所以我们重写了 `SetWidth` 和 `SetHeight` 方法以保证这一点：
```cpp
class Square : public Rectangle
{
public:
	void SetWidth(int width) override
	{
		Rectangle::SetWidth(width);
		Rectangle::SetHeight(width);
	}

	void SetHeight(int height) override
	{
		Rectangle::SetWidth(height);
		Rectangle::SetHeight(height);
	}
};
```
假定有一个 `Transform` 函数如下
```cpp
void Transform(Rectangle &rectangle)
{
	rectangle.SetWidth(7);
	rectangle.SetHeight(4);

	assert(rectangle.GetArea() == 28);
}
```
如果我们传入一个正方形，面积是 16，而不是 28，会 `assert` 失败。这里 `Square` 类违反了 `Rectangle` 类的预期行为，深层次原因是违反了 LSP（里氏替换原则）。

### 里氏替换原则
里氏替换原则（`Liskov Substitution Principle`, `LSP`）是 SOLID 原则中的 L，关心的是行为子类型化（`behavioral subtyping`）。正式定义是

类型 `T` 的对象 `x` 满足性质 $\phi(x)$，如果 `S` 是 `T` 的子类，那么类型 `S` 的对象 `y` 也满足性质 $\phi(y)$。

这里描述的是 `IS-A` 的关系，这种关系要求子类中必须严格遵循某个抽象的预期行为。这里所说的性质有以下几点。

如果有前置条件约束，那么子类的前置条件必须比父类的前置条件更弱，也就是说，子类的输入条件不能超出父类的条件范围。否则就违背了调用者对该抽象所抱有的行为预期。比如下面的例子就是一个错误的示范。
```cpp
struct X
{
	virtual ~X() = default;

	// Precondition: the function accepts all 'i' greater than 0
	virtual void Func(int i) const
	{
		assert(i > 0);
		// ...
	}
};

struct Y : public X
{
	// Precondition: the function accepts all 'i' greater than 10.
	// This would strengthen the precondition; numbers between 1 and 10
	// would no longer be allowed. This is a LSP violation!
	void Func(int i) const override
	{
		assert(i > 10);
		// ...
	}
};
```
与之相反的是子类的后置约束条件必须比父类的后置约束条件更强。下面又是一个错误的例子。
```cpp
struct X
{
	virtual ~X() = default;

	// Postcondition: the function will only return values larger than 0
	virtual int Func() const
	{
		int i;
		// ...
		assert(i > 0);
		return i;
	}
};

struct Y : public X
{
	// Postcondition: the function may return any value.
	// This would weaken the postcondition; negative numbers and 0 would
	// be allowed. This is a LSP violation!
	int Func() const override
	{
		int i;
		// ...
		return i;
	}
};
```
子类的函数返回类型要求是协变的（`covariant`），也就是说，子类的函数返回类型必须是父类型函数返回类型的子类。C++ 允许协变返回类型，比如下面这个例子。
```cpp
struct Base
{ /*...some virtual functions, including destructor...*/
};

struct Derived : public Base
{};

struct X
{
	virtual ~X() = default;
	virtual Base *Func();
};

struct Y : public X
{
	Derived *Func() override; // Covariant return type
};
```
子类的函数的参数类型是逆变的（`contravariant`），子类的函数参数类型是父类对应函数参数的父类。C++ 不支持参数逆变，因此下面的代码无法通过编译，子类的函数参数必须与父类对应函数的参数一致。
```cpp
struct Base
{ /*...some virtual functions, including destructor...*/
};

struct Derived : public Base
{};

struct X
{
	virtual ~X() = default;
	virtual void Func(Derived *);
};

struct Y : public X
{
	void Func(Base *) override; // Contravariant function parameter; Not
							 // supported in C++. Therefore the function
							 // does not override, but fails to compile.
};
```
父类的不变性（`invariant`）必须在子类中保持不变，即父类内部状态的行为预期，在所有成员函数调用结束后必须保持不变，这里成员函数包含子类的成员函数。下面仍旧是错误的例子。
```cpp
struct X
{
	explicit X(int v = 1)
		: value_(v)
	{
		if (v < 1 || v > 10)
		{
			throw std::invalid_argument("Value must be within the range [1..10]");
		}
	}

	virtual ~X() = default;

	int Get() const { return value_; }

protected:
	int value_; // Invariant: must be within the range [1..10]
};

struct Y : public X
{
public:
	Y()
		: X()
	{
		value_ = 11; // Broken invariant: After the constructor, 'value_'
					 // is out of expected range. One good reason to
					 // properly encapsulate invariants and to follow
					 // Core Guideline C.133: Avoid protected data.
	}
};
```

介绍完 LSP 再回到之前的例子。`Rectangle` 有一个预期行为是可以单独设置宽和高，而 `Square` 违反了这个预期行为，它引入了更强的不变性约束条件——宽和高必须相等。因此 `Square` 类并不满足 `Rectangle` 类的预期行为，这里并不满足 `IS-A` 的关系，无法在需要 `Rectangle` 的地方使用 `Square`。正方形是一个长方形，数学上是 `IS-A` 的关系，但不是里氏替换中所说的 `IS-A` 关系。

因此继承绝不是一个凭借直觉能使用的特性，需要深思熟虑才能驾驭。当使用继承的时候，要确保子类满足父类的预期行为，且子类符合用户的直观、自然的预期。
