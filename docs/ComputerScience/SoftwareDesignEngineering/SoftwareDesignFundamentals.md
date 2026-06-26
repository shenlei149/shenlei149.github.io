什么是软件设计（`Software Design`）？为什么软件设计很重要？软件设计的原则是什么？软件设计的模式有哪些？这些都是软件设计的基础问题，这里及后续章节将逐一解答这些问题。

## 软件设计的重要性
代码质量有很多评价指标，比如可读性、可测试性、可维护性、可扩展性、可重用性等等。对于一个项目来说，至关重要的是软件的整体结构（`structure`）。好的结构最终会影响到上述的各种质量指标。软件设计的目标就是为了构建一个好的结构，从而提高软件的质量。好的软件设计能够容忍某些糟糕的实现决定，反之则不行，差的软件设计不能靠堆砌新的语言特性来弥补。

变化是软件的常态，是所有问题的根源。软件设计的目标就是为了更好地应对变化。

大部分的开发者应该遇到过这样的情况：当需要修改一个很小的功能或者修复一个 bug 时，却需要修改很多文件，涉及整个系统的多个模块。这就是依赖（`dependency`）问题。不管项目大小，依赖都是最核心的问题。因此，软件设计是一门管理组件之间依赖关系的艺术，目的是最小化人为的、技术性的依赖，并引入必要的抽象与折中。软件设计，或者说软件工程（`engineering`），更像是一门艺术，或者说是一种工艺（`craft`），与科学相结合。有时，软件架构（`software architecture`）这个术语表达的也是类似的意思，两者可以互换使用。也有的人将架构和设计区分开来，认为架构是更高层次的设计。

> Dependency is the key problem in software development at all scales.
>
> Kent Beck, Test-Driven Development: By Example (Addison-Wesley, 2002).

> The goal of software architecture is to minimize the human resources required to build and maintain the required system.
>
> Robert C. Martin, Clean Architecture (Addison-Wesley, 2017).

我们可以将软件开发分成两个大层，第一层是设计层（`design level`），包含架构和设计，第二层是代码层（`code level`）。最底层是实现细节（`implementation details`），这一层有一些常见的实现模式（`implementation patterns`），比如 RAII、Pimpl、SFINAE（`Substitution Failure Is Not An Error`）等等，这些模式是 C++ 特有的，其他语言可能没有或者有不同的实现方式。再往上是软件设计，这一层包含一些设计模式（`design patterns`），比如装饰者、策略等等，这些就是语言无关的了。再往上是软件架构，如前所述，这里的共识不多，这里通常涉及关键决策。这一层包含一些架构模式（`architectural patterns`），比如微服务、客户端-服务器等。两者的共同点都是解决整体结构和相互依赖的问题。架构是更高层次的设计，解决的是模块、组件级别的，而设计则是更细粒度的，解决的是类、函数级别的。

惯用法（`idiom`）的出现使得实现和设计两者的界限更加模糊一些。惯用法表示某种模式（`pattern`），在某种语言中被广泛使用，比如前面提到的 RAII 就是 C++ 中的一种惯用法，还有 copy-and-swap 也是 C++ 中的一种惯用法。大部分的 C++ 惯用法都是实现层次的，设计层次的惯用法相对较少一些，比如 NVI（`Non-Virtual Interface`）和 Pimpl 就是一种设计层次的惯用法，分别脱胎于设计模式中的模板方法（`Template Method`）和桥接模式（`Bridge Pattern`）。这里涉及到一个问题，就是掌握编程语言的细节和掌握软件设计的原则哪个更重要？答案是两者都重要。没有必要厚此薄彼。

> Design and programming are human activities; forget that and all is lost.
>
> Bjarne Stroustrup, The C++ Programming Language, 3rd Edition (Addison-Wesley, 2000).

## 拥抱变化的设计
变化是问题的根源，因此分离关注点（`separation of concerns`）是应对变化的重要原则，核心思想就是对各个功能进行拆分、隔离或者抽取，将整个系统分解为更小的、命名清晰的、易于理解的小单元，能够提升效率，更好地应对变化。

如何衡量拆分是否合理呢？内聚（`cohesion`）是一个衡量模块之间关系紧密程度的指标。一个高内聚的集合，各个语句、数据块之间联系紧密，被视为一个不可分割的整体。任何试图将它们拆分的做法，都会增加耦合程度并降低代码的可读性。

### 单一职责
SOLID 原则是最基础的设计原则，分别是单一职责原则（`Single Responsibility Principle`, `SRP`）、开放封闭原则（`Open-Closed Principle`, `OCP`）、里氏替换原则（`Liskov Substitution Principle`, `LSP`）、接口隔离原则（`Interface Segregation Principle`, `ISP`）和依赖倒置原则（`Dependency Inversion Principle`, `DIP`）。这里先介绍 SRP，后续章节会介绍其他的设计原则。

SRP 是说一个类应该只有一个职责（`responsibility`），或者说一个类应该只有一个引起它变化的原因。但是问题在于什么是职责？什么是单一职责？有时，这个原则也被解释为只做一件事情，但是这个描述并没有提供更多的信息，描述依旧非常模糊。忽略名字，这个原则想描述的事情是将真正属于同类的东西聚集在一起，而将不属于同类的东西分离开来，如果有不同原因发生变化，那么应该将这些变化分类开。理想情况下，想要修改软件的某个方面，只需要修改一处即可。

下面分析一个违反 SRP 的例子。
```cpp
// #include <some_json_library.h> // Potential physical dependency

class Document
{
public:
	// ...
	virtual ~Document() = default;

	virtual void ExportToJSON(/*...*/) const = 0;
	virtual void Serialize(ByteStream &, /*...*/) const = 0;
	// ...
};
```
上面这个基类的定义看似很好，能够导出 JSON 这种通用的格式，能够序列化成二进制（`ByteStream`）这种更高效的格式，可能是 Word、PDF 等等。但是这个类引入了三类依赖。

第一类是 `ExportToJSON` 方法引入了对 JSON 库的依赖，这个库可能是第三方的，也可能是我们自己写的。为了一致性，所有的子类不得不依赖同一个类库，因此这些子类并没有真正的独立，而是耦合到了一起。同时，由于依赖了 JSON 类库，类本身变得不够轻量，后续重用也是一个问题。`Serialize` 很可能会依赖于 `protobuf` 这种二进制序列化的类库，这个依赖于 JSON 类库正交，但是可能会引发连锁反应。

第二类是 `ExportToJSON` 的参数很可能暴露了一些 JSON 类库的细节，导致一旦需要变更 JSON 类库，需要修改函数签名，那么所有的子类都需要修改，调用者也需要修改，这就是一个典型的依赖问题。

第三个问题是由 `Serialize` 方法引入的，要序列化成什么格式呢？常见做法是给一个枚举类，比如
```cpp
enum class DocumentType
{
	Word,
	PDF,
	// ...
};
```
如果这种低级别的文档表示形式被直接用在各个子类的实现内部，就会在不知不觉中把不同种类的文档强行耦合在了一起。新增一个类型，可能会影响到各个子类，这明显是一个设计缺陷。

总结一下，以下原因都会导致这个类以及相关子类的修改：

- 直接依赖的 JSON 类库变化了，那么需要修改 `ExportToJSON` 的实现细节。
- 由于底层实现变了，导致 `ExportToJSON` 的函数签名变了。
- 由于直接依赖的 `ByteStream` 类库变化，导致 `Serialize` 的修改。
- 由于底层实现细节变了而需要修改 `Serialize` 的函数。
- 由于 `DocumentType` 枚举类的修改，导致所有 `Document` 类需要修改。

如果大家都把所谓正交（独立）的功能都耦合到 `Document` 类，进一步推高了复杂度，那么修改就更困难了。并且一旦涉及到各个子类的修改，改动范围就会变大，带来维护风险。

现在看一下使用这个 `Document` 类的情况。假定有一个 `User` 类要使用 `Document` 类，这个依赖是必然的，我们不必关心。但是 `Document` 对所选择的 JSON 和 `ByteStream` 类库的依赖导致 `User` 类也间接受到了影响，但是 `User` 不应该依赖 JSON 或者序列化这些事情。依赖关系如下：
```
            User-------------
              |             |
              v             | transitive dependency
          Document          |
         /        \         |
        v          v        v
    JSON Library   ByteStream Library
```

SRP 的思想就是分离这些关注点，将不同理由的导致的变化的部分隔离到不同的变化点（`variation point`）。比如将 JSON 和序列化的功能抽取到不同的类中，这样就能够将这些变化点隔离开来，降低耦合，提高内聚。下面是一种可能的依赖关系。后续章节会介绍一些设计模式、如何实现这些分离的设计。
```
    JSON        User      Serialization
    |    \        |        /      |
    |     \       |       /       |
    |      \      |      /        |
    |       \     |     /         |
    |        \    |    /          |
    |         \   |   /           |
    v          v  v  v            v
JSON Library   Document      ByteStream Library
```

### Don't Repeat Yourself
我们先看下面一个例子，下面有一个 `Item` 类，`Price()` 函数能够返回这个 `Item` 的价格，形式是 `Money` 类的对象。
```cpp
//---- <Item.h> ----------------

#include <Money.h>

class Item
{
public:
	virtual ~Item() = default;
	virtual Money Price() const = 0;
};
```
假定我们有两个子类，分别是 `CppBook` 和 `ConferenceTicket`，它们都继承自 `Item` 类，并且都实现了 `Price()` 函数。
```cpp
//---- <CppBook.h> ----------------

#include <Item.h>
#include <Money.h>

#include <string>

class CppBook : public Item
{
public:
	explicit CppBook(std::string title, std::string author, Money price)
		: title_(std::move(title))
		, author_(std::move(author))
		, priceWithTax_(price * 1.15) // 15% tax rate
	{}

	const std::string &Title() const { return title_; }

	const std::string &Author() const { return author_; }

	Money Price() const override { return priceWithTax_; }

private:
	std::string title_;
	std::string author_;
	Money priceWithTax_;
};

//---- <ConferenceTicket.h> ----------------

#include <Item.h>
#include <Money.h>

#include <string>

class ConferenceTicket : public Item
{
public:
	explicit ConferenceTicket(std::string name, Money price)
		: name_(std::move(name))
		, priceWithTax_(price * 1.15) // 15% tax rate
	{}

	const std::string &Name() const { return name_; }

	Money Price() const override { return priceWithTax_; }

private:
	std::string name_;
	Money priceWithTax_;
};
```
上述设计看起来很好，分离了关注点，满足前面说的 SRP，但是它违反了 DRY（Don't Repeat Yourself）原则。`CppBook` 和 `ConferenceTicket` 类中都包含了计算价格的逻辑，如果税率发生了变化，那么我们就需要修改这两个类的代码，甚至更多的子类。

SRP 告诉我们每个事物只能有一个职责（一个变化的理由），DRY 原则告诉我们每个职责也应该有且只有一份。这个原则建议就是不要在多处重复同样的信息，而是要通过合理的设计，确保只需要在一个地方做出修改就足够了。理想情况下，税率这个信息应该只在一处定义即可。通常情况下，DRY 原则和 SRP 是相辅相成的，SRP 能够帮助我们更好地实现 DRY 原则，反之亦然。不过在一些特殊的情况下，要想保证满足这两个原则需要一些额外的设计。后续章节会介绍一些设计模式，阐述如何实现以满足这些原则。

### 不要过早进行关注点分离
过早进行关注点分离（`premature separation of concerns`）会导致过度设计（`over-engineering`），为了分离而分离会增加复杂性，引入不必要的胶水代码和间接层。

我们的目标是可维护性，SOLID 原则是工具，工具是来帮助我们达到我们的目标的，而不是要达成 SOLID！SRP 和 DRY 是很好的工具但不是目的。

> Don’t try to achieve SOLID, use SOLID to achieve maintainability.
>
> Katerina Trajchevska, "Becoming a Better Developer by Using the SOLID Design Principles", Laracon EU, August 30–31, 2018.

如果知道哪里会变化，使用这些工具让未来的变化更容易，如果不知道，那就等到变化出现的时候再重构，让代码变得更好。

## 接口分离
回到 `Document` 类的例子。假定用户写了一个函数来处理 `Document` 类的对象，将其导出为 JSON 格式。
```cpp
void ExportDocumentToJSON(const Document &doc)
{
	// ...
	doc.ExportToJSON(/*...*/);
	// ...
}
```
这个函数只需要 JSON 相关的功能，但是由于 `Document` 类的设计，这个函数不得不依赖了 `Document` 类的其他功能，比如序列化相关的功能。如果 `ByteStream` 类发生了变化，或者 `Serialize` 函数签名发生了变化，会影响到所有使用 `Document` 类的代码，包括这个 `ExportDocumentToJSON` 函数，需要重新编译、测试。如果 `Document` 类耦合了更多正交的功能，这个问题就会更严重了。

这里违反了接口隔离原则（`Interface Segregation Principle`, `ISP`），这就是 SOLID 中的 I。ISP 是说一个类不应该依赖于它不使用的方法。ISP 的思想是通过分离接口来分离关注点。

这个例子中，我们应该引入两个接口 `JSONExportable` 和 `Serializable`，`Document` 类分别实现这两个接口，这样就能够将这些功能分离开来。
```cpp
class JSONExportable
{
public:
	// ...
	virtual ~JSONExportable() = default;
	virtual void ExportToJSON(/*...*/) const = 0;
	// ...
};

class Serializable
{
public:
	// ...
	virtual ~Serializable() = default;
	virtual void Serialize(ByteStream &bs, /*...*/) const = 0;
	// ...
};

class Document
	: public JSONExportable
	, public Serializable
{
public:
	// ...
};
```
使用的地方仅仅需要依赖 `JSONExportable` 接口即可，即仅仅依赖它所需要的东西。
```cpp
void ExportDocumentToJSON(const JSONExportable &exportable)
{
	// ...
	exportable.ExportToJSON(/* pass necessary arguments */);
	// ...
}
```

有些人可能会质疑，这不就是 SRP 吗？社区在这一点上也达成了共识，认为 ISP 是 SRP 的一个特例，做到了 SRP 就做到了 ISP。但是这个原则还是非常有价值的，大家常常在后期维护的时候，有意无意地往同一个接口中加入彼此正交的功能，结果就是接口变得非常臃肿，使用方还是会依赖其并不需要的东西，同时接口一旦发布，后续的修改就会变得非常困难了。因此，在设计、重构的时候，ISP 是一个非常重要的原则，能够帮助我们设计出更好的接口。

ISP 的理念还可以应用于模板参数的设计，比如 `std::copy` 这个函数。
```cpp
template<std::input_iterator InputIt, std::output_iterator OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first);
```
这保证了约束的最小化，`std::input_iterator` 这个概念约束有只读、单向、单次遍历，`std::output_iterator` 这个概念约束有只写、单向、单次遍历。如果我们违反 ISP 的原则，使用一个更宽泛的概念，比如 `std::forward_iterator`，这个概念约束有可读可写、单向、可多次遍历，那么就会导致一些不必要的约束，导致有些场景无法使用这个 `copy` 函数了，比如无法将输入流（`input stream`）作为输入迭代器来使用，因为不支持写入操作和多次遍历。

## 面向可测试性的设计
软件一直在变化，这是本质问题。在修改的时候，可能会有意无意地引入问题，如何防止这一点呢？测试！完整的测试会保护我们不受修改的影响。但是前提是我们写测试，如果我们不写测试，那么我们就无法保护自己了。因此，我们的设计应该是面向可测试性（`testability`）的设计，简单说就是要容易写测试。

先来看看下面这个“矫揉造作”的例子。假定我们有一个 `Widget` 类，这个类有一个私有函数 `UpdateCollection()`，如何测试这个函数呢？换句话说，如果修改了这个函数，如何确保修改没有引入问题呢？
```cpp
class Widget
{
	// ...

private:
	void UpdateCollection(/* some arguments needed to update the collection */);

	std::vector<Blob> blobs_;
	/* Potentially other data members */
};
```
你可能会说，或许有 `AddBlob()` 这个 `public` 函数会调用 `UpdateCollection()`，我们可以通过测试 `AddBlob()` 来间接测试 `UpdateCollection()`，但是如果 `AddBlob()` 的实现发生了变化，不再调用 `UpdateCollection()` 了，那么我们就无法测试 `UpdateCollection()` 了。这其实是一种白盒测试（`white-box test`）的想法，需要了解被测试函数的实现细节。这里需要的是黑盒测试（`black-box test`），只需要测试预期行为。

你可能会说，把这个函数修改成 `public` 的，技术上可行，但绝不是一个好主意。

你可能会说，使用友元测试类（`friend test class`）的方式来测试这个函数，`Widget` 里面加上 `friend class WidgetTest;`，这个也是技术上可行的，但也不是一个好主意。这样，测试类就和被测试类耦合在了一起了，增加了被测试类对测试类的依赖。

你可能会说，把这个函数修改成 `protected` 的，测试类继承 `Widget` 类。这绝对是滥用继承！

这里的正确解法是将这个函数抽取为一个类外的函数，或者说一个独立的类的成员函数，这样就能够直接测试这个函数了，同时也不会引入不必要的依赖了。
```cpp
void UpdateCollection(std::vector<Blob> &blobs, /* some arguments needed to update the collection */);
```
或者
```cpp
namespace widgetDetails
{

class BlobCollection
{
public:
	void UpdateCollection(/* some arguments needed to update the collection */);

private:
	std::vector<Blob> blobs_;
};

} // namespace widgetDetails
```
这样把放到一起的东西分离开是否违背了 SRP 呢？恰恰相反。`UpdateCollection()` 这个函数需要被测试，这就表明之前的设计不足够合理。我们有理由单独修改这个函数，那么说明 `Widget` 类有两个理由被修改，承担了一个以上的职责。换句话说，如果一个函数一定要被测试，那么它就应该是一个独立的单元，能够直接测试它。

这样是不是违反了封装呢？不。相反，封装是分离关注点的理由。Scott Meyers 在《Effective C++》中提到过，更推荐非成员、非友元函数（`non-member, non-friend function`）而不是成员函数，从类中抽取函数反而是更好的封装。这是因为成员函数天生就能访问所有的数据成员，因此很容易就会访问到不相关的数据成员，导致耦合度增加。抽取的 `UpdateCollection()` 函数只能通过 `Widget` 类提供的接口来访问数据成员，结果是更好的访问控制，更好的内聚。

通过上述的设计，我们做到了以下三点：（1）能够直接测试 `UpdateCollection()` 函数；（2）更好的封装；（3）独立的 `UpdateCollection()`，与 `Widget` 类解耦，甚至可以用于其他类。

这里并不是鼓励把所有函数都设计成独立的函数甚至是单独抽取到一个类。我们要仔细的分析哪些明明需要测试但是被塞进了 `private` 里面的函数。如果这个函数是虚函数该怎么办呢？后续章节会讨论这个问题。

## 面向扩展性的设计
扩展性（`extensibility`）是软件设计的重要目标之一，驱使我们做出好的设计。除非软件已经到生命周期尽头了，否则就需要添加新的功能，扩展现有代码是基本操作。扩展性并不是天上掉馅饼，而是需要我们在软件设计的时候就考虑到这一点。

### 开放封闭原则
回到之前 `Document` 类的例子，它有一个纯虚函数 `Serialize()`，假定有一个子类 `PDF` 继承了 `Document` 类，并且实现了 `Serialize()` 函数。
```cpp
class PDF : public Document
{
public:
	// ...
	void Serialize(ByteStream &bs, /*...*/) const override;
	// ...
};
```
之前提到过，为了区别不同类型的文档，我们可能会引入类
```cpp
enum class DocumentType
{
	Word,
	PDF,
	// ...
};
```
在序列化的时候，可能第一个字节就是这个 `DocumentType`，用来区分不同类型的文档。那么 `PDF` 类就会知道 `Word` 这个类的存在，这就导致了耦合。整个结构如下所示。
```
                   Document
                      ^
                      |
                      |
         +------------+--------------+
         |            |              |
         |            |              |
        PDF ----> DocumentType <-- Word
```
如果我们想要添加一个子类，比如 `XML`，我们就需要修改 `DocumentType` 类。这至少会导致 `PDF` 和 `Word` 这两个类需要重新编译。更糟糕的是，这限制了其他人对代码进行扩展的能力，因为可能这是一个类库而很多人没有权限修改 `DocumentType` 类。从设计的角度来说，`PDF` 和 `Word` 这两个类本不应该知道新增的类 `XML` 的存在。

上述的例子违反了开放封闭原则（`Open-Closed Principle`, `OCP`），这就是 SOLID 中的 O。OCP 是说软件实体（类、模块、函数等等）应该对扩展开放，对修改关闭。也就是说，理想状态下，我们应该能够通过添加新代码来扩展软件的功能，而无需修改现有的代码。

幸运的是我们之前提到的分离关注点的方法就能解决这个问题。调整之后，只有序列化 `Serialization` 模块需要知道 `DocumentType` 的存在，不同的 `Document` 子类都不知道 `DocumentType`，进而彼此是解耦的。
```
                        Document
                           ^
                           |
            +--------------+-------------+
            |                            |
            |                            |
           PDF                         Word
            ^                            ^
            |                            |
            +----------------------------+
            |                            |
            |                            |
           JSON                  Serialization(DocumentType)
```
序列化模块知道 `DocumentType` 的存在并不违反 OCP，我们并不需要修改更高层次或者同级的其他模块。从功能设计的角度看，这个依赖是必须的，而序列化模块也必须适配每一个 `Document` 的子类，因此这个依赖是合理的。

从分离关注点的角度说，真正的解决方案就是坚守 SRP，因此业内也有人认为也不应将 OCP 看作是独立的原则，它和 SRP 是一样的。大部分情况下，做到了分离关注点也就带来了扩展性。因此，将二者看作是紧密相关的甚至是融合在一起的原则也没有问题。

不过单独讨论 OCP，给了我们从另一个视角来审视问题本身。当我们显式的考虑扩展性的时候，会影响我们应用 SRP 的方式。相比于 SRP，OCP 更注重于扩展性，影响在扩展性方面做出的决策。因此，它或许不是 SRP 的副产物，完全取决于我们看待事物的角度。

### 编译期扩展性
日常开发中，常常会使用 C++ 标准库，它的设计考虑了扩展性，因此我们可以容易的使用它来实现我们自己的功能。先看第一个例子 `std::swap`，它的实现如下所示。
```cpp
template<class T>
void swap(T &a, T &b)
{
	T temp = std::move(a);
	a = std::move(b);
	b = std::move(temp);
}
```
这个函数的实现非常简单，使用了 move 语义来避免不必要的复制操作。这个函数的设计考虑了扩展性，能够适用于任何类型 `T`。同时，如果自定义类型没有提供高效的移动操作，可以提供自己的 `swap` 函数，那么也能够正确的调用自定义的 `swap` 函数，而不是使用默认的实现。
```cpp
namespace custom
{

class CustomType
{
	/* Implementation that requires a special form of swap */
};

void swap(CustomType &a, CustomType &b) { /* Special implementation for swapping two instances of type 'CustomType' */ }

} // namespace custom
```
使用的时候采用非限定调用（`unqualified call`）的方式，结合 ADL（`Argument-Dependent Lookup`），就能调用到合适的 `swap` 函数。
```cpp
using std::swap;

swap(a, b); // Calls std::swap for built-in types, custom::swap for CustomType
```
下面是第二个例子。
```cpp
template<typename InputIt, typename T>
constexpr InputIt find(InputIt first, InputIt last, const T &value);

template<typename InputIt, typename UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p);
```
`std::find` 和 `std::find_if` 通过模板参数支持任意类型的查找，`std::find_if` 还支持通过谓词（`predicate`）来定义查找条件，可以自定义比较对象的方式。

最后一个定制方式是模板特化（`template specialization`），比如 `std::hash`。
```cpp
template<>
struct std::hash<CustomType>
{
	std::size_t operator()(const CustomType &v) const noexcept { return /*...*/; }
};
```
`std::hash` 的设计使得我们能够为自定义类型提供哈希函数的实现，完全不需要修改任何现有的代码。通过模板特化来定制我们的需求。

### 避免过早进行扩展性设计
扩展性很重要，但是这不意味着应该不加思考地在实现细节层面广泛使用多态、模板来确保未来的扩展性。和不要过早地优化程序、过早地关注分离点一样，我们也不应该过早地关注扩展性。如果还不知道哪些地方会如何演进，那么就等到需要扩展的时候再进行设计和实现就好，不要预判一个可能永远不会扩展的点。未来的需求会指导我们，给出未来演进的方向，此时再重构代码，使得后续更容易扩展。
