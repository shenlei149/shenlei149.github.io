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
子类的函数返回类型可以是协变的（`covariant`），也就是说，在返回类类型的指针或引用时，子类函数可以返回父类函数返回类型的派生类。C++ 允许这种协变返回类型，比如下面这个例子。
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

有的人认为子类存在违背 LSP 的风险，基类无法起到抽象的作用，原因是调用方的稳定性最终依然要依赖于子类是否有错误行为。这是一种因噎废食的想法，有点本末倒置。基类绝对是一种抽象，调用方代码应该并且仅仅应该依赖于抽象的预期行为，违反了 LSP 本质上属于程序的 bug，需要修复而不是使用一些其他方法规避问题。比如下面的代码就是一个错误的示例，应该想办法重新设计抽象而不是在调用方使用 `dynamic_cast` 来规避问题。
```cpp
class Base
{};

class Derived : public Base
{};

class Special : public Base
{};

// ... Potentially more derived classes
void f(const Base &b)
{
	if (dynamic_cast<const Special *>(&b))
	{
		// ... do something "special," knowing that 'Special' behaves differently
	}
	else
	{}
}
```

为了实现解耦，我们应该信任我们的抽象，如果我们没有能够完全被理解、有实质意义的抽象，很难写出可靠、稳定的代码。为了达成这个目的，坚持 LSP 是非常重要的。对于抽象的预期行为，必须清晰且没有二义性，代码自解释或者是有规范的文档。

## 基类与概念（`concept`）的异曲同工之妙
这一小节的内容和 C++ 有点强绑定。

LSP 不仅仅适用于运行期的多态和继承结构，也可以用于编译期的多态和模板。看下面两段代码。
```cpp
//==== Code Snippet 1 ====
class Document
{
public:
	// ...
	virtual ~Document() = default;
	virtual void ExportToJSON(/*...*/) const = 0;
	virtual void Serialize(ByteStream &, /*...*/) const = 0;
	// ...
};

void UseDocument(const Document &doc)
{
	// ...
	doc.ExportToJSON(/*...*/);
	// ...
}

//==== Code Snippet 2 ====
template<typename T>
concept Document = requires(const T &t, ByteStream b) {
	t.ExportToJSON(/*...*/);
	t.Serialize(b, /*...*/);
};

template<Document T>
void UseDocument(const T &doc)
{
	// ...
	doc.ExportToJSON(/*...*/);
	// ...
}
```
第一个是运行时多态的实现，第二个是编译时多态的实现。实现方式不同，但是两者的语义相同。第一段代码中，传入 `UseDocument` 的对象必须是 `Document` 的子类，并满足基类 `Document` 抽象的预期行为；第二段代码中，用于实例化 `UseDocument` 的类型必须满足 `Document` 概念的约束条件。两者都要求满足一定的预期行为，都体现了类似的语义。

基类和概念都描述了一组预期行为，基类通过接口和文档表达需求，概念则表达可由编译器检查的语法要求。因此，概念在描述模板参数需求时与基类发挥着相似作用，但两者并不等价。概念本身无法表达语义约束。从预期行为的角度看，LSP 同样适用于概念和模板代码。我们再看一个例子。
```cpp
template<typename InputIt, typename OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
	while (first != last)
	{
		*d_first++ = *first++;
	}
	return d_first;
}
```
`copy` 函数的预期行为是将 `[first, last)` 的元素复制到以 `d_first` 开头的范围内。`InputIt` 表示输入迭代器，要求迭代器能够进行等值比较，支持前缀和后缀自增遍历，并能通过解引用访问元素。`OutputIt` 表示输出迭代器，也有自己的要求。满足这些要求的类型才能正确地用来实例化 `copy` 函数模板。若不满足语法要求，通常会导致编译错误，若不满足未由概念表达的语义要求，则可能导致未定义行为或错误结果。因此，由 `InputIt` 和 `OutputIt` 的要求构成了 LSP 意义上的抽象。

## 理解重载（`overloading`）的语义要求
每一个抽象都表示一组需求、要求。函数的重载，特别是自由函数（`free function`）的重载，也能表示一个抽象的概念。基于给定的类型，编译器从一组函数名相同的函数中选择一个最合适的函数来调用。这些称为重载集（`overload set`）。这是一种灵活且强大的抽象机制。

我们可以给任意类型，比如 `int`、`std::string`，或者是用户定义的类型，添加一个重载函数，这种方式是非入侵的。但是如果是成员函数，就必须修改类的定义，这种方式是入侵的，甚至做不到，比如对 `int` 类型添加成员函数，或者无法修改的第三方类库。自由函数践行了 OCP，单纯的新添代码实现扩展，完全不需要修改任何现有的代码。下面给出一个例子来阐述自由函数的好处。
```cpp
template<typename Range>
void TraverseRange(const Range &range)
{
	for (auto pos = range.begin(); pos != range.end(); ++pos)
	{
		// ...
	}
}
```
这段代码无法用于 C 风格数组，因为 C 风格数组没有成员函数 `begin` 和 `end`。这里不讨论 C++ 中通常更推荐使用 `std::array` 或 `std::vector` 代替 C 风格数组的情形。若使用自由函数 `std::begin` 和 `std::end`，就可以支持 C 风格数组。
```cpp
template<typename Range>
void TraverseRange(const Range &range)
{
	using std::begin;
	using std::end;
	for (auto pos = begin(range); pos != end(range); ++pos)
	{
		// ...
	}
}
```
STL 体现了许多设计原则，不同功能组件之间松耦合、分离关注点，并尽可能地提供自由函数以最大化代码复用。容器和算法是两个独立组件，两者不直接依赖，而是通过迭代器连接。`std::string` 是后来添加的，其中可能仍存在耦合或重复之处。

> There was never any question that the [standard template] library represented a breakthrough in efficient and extensible design.
>
> Scott Meyers

如果想要发挥上述的威力，需要遵循规则和约定，遵循 LSP。考虑下面这个例子，你实现了自定义的 `swap()`。
```cpp
struct Widget
{
	int i;
	int j;
};

void swap(Widget &w1, Widget &w2)
{
	using std::swap;
	swap(w1.i, w2.i);
}
```
你的同事使用这些函数得到了一个诡异的结果。
```cpp
Widget w1 { 1, 11 };
Widget w2 { 2, 22 };

swap(w1, w2);
// Widget w1 contains (2,11)
// Widget w2 contains (1,22)
```

原因在于 `swap` 的实现并没有符合人们的预期，大家期望对象的所有状态都被交换了。如果你重载了某个函数，要忠实的履行其预期行为，满足用户的直觉预期。否则就会违反 LSP。

你可能会保证遵守 LSP，但是在庞大的代码库中，各个角落都散落着重载，无法知晓它们的预期行为和隐藏的细节，因此即使自己很小心避开陷阱，但是最终做出来的可能仍旧不是“正确”的事情。系统赋予了开发者太大的能力和自由。

事物有利有弊。重载集的威力很大，但也很难用好。只有当重载函数具有相同的核心语义，且差异能由参数类型自然表达时，才适合使用重载，不能仅因功能大致等价就重载。如果重载函数之间有细微差别或隐藏细节，就应使用不同的函数名来表达不同的预期行为。`find` 函数是线性查找，不要求传入范围内的元素有序。`begin()` 和 `end()` 返回一对迭代器，没人会认为它们应该启动或终止某个过程，后者应该交给 `start()` 和 `stop()`。

如果是虚函数，这里的建议也是适用的。软件设计的目标是减少依赖关系，而虚函数往往又会带来一些耦合，如何将虚函数解救（`free`）出来，也是一个值得思考的问题。后续章节会讨论如何将虚函数以自由函数的形式分离出来。

## 抽象的所有权
Robert Martin 说过最灵活的系统，源码依赖仅仅指向抽象而不是具体实现。这就是依赖倒置原则（`Dependency Inversion Principle`, `DIP`），SOLID 中的 D。这个建议针对的是管理依赖关系，你应该依赖抽象，而不是去依赖具体的类型或者底层的实现细节。这个表述仅仅是广义上的抽象，不涉及继承或者接口等具体的抽象机制。

假定有一个 ATM 的例子，因为和钱相关，因此需要引入事务，我们有一个抽象类 `Transaction`，它有一个纯虚函数 `Execute()`，三个子类分别是 `Deposit`、`Withdraw` 和 `Transfer`。这三个类需要的信息是通过 `UI` 类来获取的，`UI` 类有 `RequestDepositAmount()`、`RequestWithdrawAmount()` `RequestTransferAmount()` `InformInsufficientFunds()` 等函数。这里事务类和 `UI` 类在架构层面是分开的。

现在有了新的需求，需要为 VIP 客户提供 `SpeedTransfer` 功能，或许 `UI` 类需要增加 `RequestSpeedTransferAmount()` `RequestVIPNumber()` 等函数。由于所有事务类都依赖于 `UI` 类，因此 `UI` 类的修改会影响所有事务类。最好情况下，这些事务类只需要重新编译和测试，最坏情况下，可能需要重新部署。这是因为稳定的高层事务依赖于不稳定的底层 `UI` 类，这个依赖需要翻转过来。

我们引入三个抽象类 `DepositUI`、`WithdrawUI` 和 `TransferUI`

- `DepositUI` 包含 `RequestDepositAmount()` 函数
- `WithdrawUI` 包含 `RequestWithdrawAmount()` `InformInsufficientFunds()` 函数
- `TransferUI` 包含 `RequestTransferAmount()` `InformInsufficientFunds()` 函数

三个事务类不再依赖具体的 `UI` 类，而是依赖于抽象的 `DepositUI`、`WithdrawUI` 和 `TransferUI`，这三个事务之间的间接依赖也被消除了。这样，如果需要添加新事务 `SpeedTransfer`，只需添加 `SpeedTransfer` 及其抽象接口 `SpeedTransferUI`，并修改 `UI` 类来实现该接口，不会影响其他事务类。

这里遵循了 ISP，消除了不同事务之间的间接耦合，同时让应该在一起的函数分到了不同的接口，这就是 SRP。引入抽象使得事务类和 `UI` 类之间的依赖关系翻转了过来，这是 DIP 吗？至此我们还没有真正实现 DIP。但是这三个抽象类仍然和 `UI` 类在一起，在架构上又与事务类分离。从架构上看，事务还是依赖 `UI` 模块，这仍然只是局部（`local`）依赖倒置。因此这里引出一个问题，引入的抽象类应该放在哪里？应与事务放在一起！

把抽象接口放到稳定的高层事务模块中，让它拥有这个抽象，才真正遵循了 DIP。这不仅仅是一个概念上的变换。在工程上，抽象接口的头文件要从 `UI` 模块移动到事务模块，重新组织包含关系。这里本质上是所有权的转移，抽象接口的所有权转移到了事务模块中。现在事务模块依赖于抽象接口，而 `UI` 模块中的具体实现依赖于抽象接口，这样就真正实现了 DIP。

但是这里有这么一个问题，抽象接口的所有权转移到了事务模块中，和更具体的 `UI` 类分离了，这不违反 SRP 吗？这里真正应该在一起的是事务类和它们所依赖的 `UI` 抽象接口。

这样，才真正实现了 DIP，接口抽象由高层掌握，拥有其所有权。这也符合 Robert Martin 的建议：

> 1. High-level modules should not depend on low-level modules. Both should depend on abstractions.
> 2. Abstractions should not depend on details. Details should depend on abstractions.
>
> Robert Martin

再来看一个编辑器和插件的例子。`Editor` 类属于高层模块，插件实现属于低层模块。`Editor` 类依赖插件的抽象接口 `IPlugin`，而插件实现依赖于 `IPlugin` 接口。`IPlugin` 的所有权归 `Editor` 模块。代码如下：
```cpp
// Editor module
// IPlugin.h
class IPlugin
{};

// Editor.h
#include "IPlugin.h"

class Editor
{};

// thirdparty module
// XXXPlugin.h
#include "IPlugin.h"

class XXXPlugin : public IPlugin
{};
```
这样核心的编辑器由某个团队开发，插件可以由其他团队甚至是社区开发。插件的开发者只需要依赖于 `IPlugin` 的抽象接口，而不需要依赖于 `Editor` 类的实现细节。这样就实现了模块之间的解耦，遵循了 DIP。

之前讨论过，模板和基类能表达同样的事情，因此模板也可以用于实现类似的抽象和依赖倒置。回到 `std::copy_if` 的例子。`InputIt` `OutputIt` 和 `UnaryPredicate` 都是抽象的概念，满足这些概念的类型才能被用来实例化 `copy_if` 函数模板，这些是对于使用方的硬性约束，也就是说，这些约束的所有权归 `copy_if` 函数模板所有。
```cpp
template<typename InputIt, typename OutputIt, typename UnaryPredicate>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred);
```

前面说过，`ADL` 和重载函数也能表达抽象，因此这里也能体现 DIP。比如前面提到的 `Widget` 和自定义的 `swap` 函数。假定为 `Widget` 定义的 `swap` 函数符合标准库的行为预期，它便可通过 ADL 加入重载集。这种形式与基类、派生类之间的关系类似，调用方依赖于抽象的预期行为，而非某个具体实现，这种依赖关系符合 DIP。

## 写一份架构设计文档
就像有 CI、自动化测试、静态检查工具一样，写一份架构设计文档也是非常重要的。

架构是团队就代码库达成的一种共识，是全局视角下的设计。一份文档能帮助大家站在统一视角看待问题，理解系统的设计和架构，避免误解。文档也承载着设计思想、愿景和关键决策等信息，如果没有文档，这些信息仅存在于代码和某个人的脑海中，那么当这个人离开团队或忘记这些信息时，团队就会失去这些信息，代码也会僵化，沦为“遗留代码”。因此，在架构文档上花费一些时间是值得的。

架构文档不应该包含经常变动的细节，应该专注于全局结构、核心关键组件之间的交互、重大的技术决策。

写架构文档可能是一项繁重的任务。可以从最核心的决策开始，逐步扩展以展现整个架构的全貌。架构文档应该是一个活文档，随着系统演进不断更新。一般而言，架构文档不会频繁变化，因此可以每两周或更长时间召开一次会议，讨论架构是否发生演进、如何演进，然后根据需要更新文档。
