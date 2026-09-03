这一节围绕着访问者（`visitor`）设计模式展开。

## 扩展类型或操作
回到传统的绘制图形的例子，我们先使用过程式解决方案。假定有一个 `Shape` 基类，其中存储 `ShapeType` 枚举值。
```cpp
//---- <Shape.h> ----------------
enum class ShapeType
{
	Circle,
	Square
};

class Shape
{
protected:
	explicit Shape(ShapeType type)
		: type_(type)
	{}

public:
	virtual ~Shape() = default;

	ShapeType Type() const { return type_; }

private:
	ShapeType type_;
};
```
下面我们继承出 `Circle` 和 `Square`，并为它们添加 `Draw()` 函数。为了避免与具体类型细节耦合，`Draw()` 在 `DrawCircle.h` 中声明，并在相应的源文件中定义，也就是说，`Draw()` 是自由函数。
```cpp
//---- <Circle.h> ----------------
class Circle : public Shape
{
public:
	explicit Circle(double radius)
		: Shape(ShapeType::Circle)
		, radius_(radius)
	{
		/* 检查传入的半径是否合法 */
	}

	double Radius() const { return radius_; }

	Point Center() const { return center_; }

private:
	double radius_;
	Point center_ {};
};

//---- <DrawCircle.h> ----------------
class Circle;
void Draw(const Circle &);

//---- <DrawCircle.cpp> ----------------
#include <Circle.h>
#include <DrawCircle.h>

void Draw(const Circle &c)
{
	// ... Implementing the logic for drawing a circle
}

//---- <Square.h> ----------------
#include <Shape.h>

class Square : public Shape
{
public:
	explicit Square(double side)
		: Shape(ShapeType::Square)
		, side_(side)
	{
		/* 检查传入的边长是否合法 */
	}

	double Side() const { return side_; }

	Point Center() const { return center_; }

private:
	double side_;
	Point center_ {}; // 如果你愿意，也可以表示任意顶点
};

//---- <DrawSquare.h> ----------------
class Square;
void Draw(const Square &);

//---- <DrawSquare.cpp> ----------------
#include <DrawSquare.h>
#include <Square.h>

void Draw(const Square &s)
{
	// ... Implementing the logic for drawing a square
}
```
有了这些类之后，我们可以创建一个 `Shape` 的集合，然后遍历它们并绘制每一个图形。
```cpp
//---- <DrawShapes.h> ----------------
#include <vector>
#include <memory>

class Shape;
void DrawShapes(const std::vector<std::unique_ptr<Shape>> &shapes);

//--- <DrawShapes.cpp> ----------------
#include <DrawShapes.h>
#include <Circle.h>
#include <DrawCircle.h>
#include <Square.h>
#include <DrawSquare.h>

void DrawShapes(const std::vector<std::unique_ptr<Shape>> &shapes)
{
    for (const auto &shape : shapes)
    {
        switch (shape->Type())
        {
        case ShapeType::Circle:
            Draw(static_cast<const Circle &>(*shape));
            break;
        case ShapeType::Square:
            Draw(static_cast<const Square &>(*shape));
            break;
        default:
            throw std::runtime_error("Unknown shape type");
        }
    }
}
```
`main()` 函数中，我们可以创建一个 `Shape` 的集合，然后调用 `DrawShapes()` 来绘制所有的图形。
```cpp
//---- <main.cpp> ----------------
#include <DrawShapes.h>
#include <Circle.h>
#include <Square.h>

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Square>(3.0));

    DrawShapes(shapes);

    return 0;
}
```
这个解决方案明显有很多缺点。首先，如果需要扩展类型，就需要修改枚举类 `ShapeType`，并在 `DrawShapes()` 中添加新的 `case` 分支。问题的核心在于它违反了开放封闭原则（`open-closed principle`）。Scott Meyers 对此评价是：

> This kind of type-based programming has a long history in C, and one of the things we know about it is that it yields programs that are essentially unmaintainable.

为了克服上述问题，我们换成面向对象的解决方案。在 `Shape` 类中添加一个纯虚函数 `Draw()`，然后在 `Circle` 和 `Square` 中实现它，`DrawShapes()` 函数中直接调用 `shape->Draw()`，这样就不需要使用 `switch` 语句了。
```cpp
//---- <Shape.h> ----------------
class Shape
{
public:
	Shape() = default;
	virtual ~Shape() = default;

	virtual void Draw() const = 0;
};

//---- <Circle.h> ----------------
#include <Shape.h>

class Circle : public Shape
{
public:
	explicit Circle(double radius)
		: radius_(radius)
	{}

	double Radius() const { return radius_; }

	void Draw() const override;

private:
	double radius_;
};

//---- <Circle.cpp> ----------------
#include <Circle.h>

void Circle::Draw() const
{
	// Implementation for drawing a circle
}

//---- <Square.h> ----------------
#include <Shape.h>

class Square : public Shape
{
public:
	explicit Square(double side)
		: side_(side)
	{}

	double Side() const { return side_; }

	void Draw() const override;

private:
	double side_;
};

//---- <Square.cpp> ----------------
#include <Square.h>

void Square::Draw() const
{
	// Implementation for drawing a square
}

//---- <DrawShapes.cpp> ----------------
#include <Shape.h>
#include <memory>
#include <vector>

void DrawShapes(const std::vector<std::unique_ptr<Shape>> &shapes)
{
	for (const auto &shape : shapes)
	{
		shape->Draw();
	}
}
```
这个方案看起来顺眼多了，不过它也有缺点。面向对象的方法使得我们能够轻松地添加新类型，但是如果我们想要添加新操作，比如 `Resize()` 或者 `Move()`，就需要修改 `Shape` 类并在所有派生类中实现这些新操作，这同样违反了开闭原则。

总而言之，面向对象的解决方法在添加类型方面满足 OCP，但在添加操作方面不满足 OCP；过程式解决方法则相反。在过程式解决方法中，添加新操作很容易，新操作可以以自由函数或独立类的形式添加，完全没有必要修改 `Shape` 类或其派生类。因此，这种方式在添加操作方面满足 OCP，但在添加类型方面不满足 OCP。

应根据对代码库演进方向的预判，选择合适的扩展方案。不要忽视方案的弱点，以免最终陷入维护地狱。

## 使用访问者模式扩展操作
访问者（`visitor`）模式旨在支持操作扩展，解决上述问题。

> Intent: "Represent an operation to be performed on the elements of an object structure. Visitor lets you define a new operation without changing the classes of the elements on which it operates."

### 解决方案
整个架构图如下所示，除了 `Shape` 类和它的派生类之外，我们还定义了 `ShapeVisitor` 基类，代表了对图形操作的抽象。
```
        Client -------------------> ObjectStructure
            |                               |
            |                               |
            v                               v
        ShapeVisitor                      Shape
            ^                               ^
           / \                             / \
          /   \                           /   \
         /     \                         /     \
        |       |                       |       |
     Rotate   Draw                    Circle   Square
```
首先 `ShapeVisitor` 基类为 `Shape` 的每一个派生类定义了一个纯虚函数。
```cpp
class ShapeVisitor
{
public:
	virtual ~ShapeVisitor() = default;
	virtual void visit(const Circle &, /*...*/) const = 0;
	virtual void visit(const Square &, /*...*/) const = 0;

	// Possibly more visit() functions, one for each concrete shape
};
```
有了 `ShapeVisitor` 基类之后，我们就可以轻松添加新操作，即添加一个新的派生类。比如，为了支持绘制图形，我们可以定义一个 `DrawVisitor` 类。也就是说，我们在添加操作这一维度满足了 OCP。
```cpp
class DrawVisitor : public ShapeVisitor
{
public:
    void visit(const Circle &c, /*...*/) const override;
    void visit(const Square &s, /*...*/) const override;

    // Possibly more visit() functions, one for each concrete shape
};
```
最初的问题在于，每添加一个新操作，都需要修改基类 `Shape`，并在所有派生类中实现该操作。访问者模式将新增操作作为一个变动点。通过将这个变动点抽取到独立的类中，遵循单一职责原则（`single responsibility principle`），`Shape` 不再因新增操作而修改。这避免了对 `Shape` 继承体系的频繁变更，并使添加操作变得简单。

为了让访问者模式工作，我们需要在 `Shape` 类中添加一个纯虚函数 `Accept()`，它接受 `ShapeVisitor` 的常量引用作为参数。然后在每个派生类中实现这个函数，调用访问者相应的 `visit()` 函数。
```cpp
class Shape
{
public:
    virtual ~Shape() = default;
    virtual void Accept(const ShapeVisitor &visitor) = 0;
};
```
`Accept()` 的函数实现很简单，调用 `visit()` 函数。虽然实现看起来一样，但是 `this` 类型不同，会在传入的访问者中调用不同的 `visit()` 函数重载。因此，`Shape` 无法在基类提供默认实现。
```cpp
class Circle : public Shape
{
public:
	explicit Circle(double radius)
		: radius_(radius)
	{}

	void Accept(const ShapeVisitor &v) override { v.visit(*this); }

	double Radius() const { return radius_; }

private:
	double radius_;
};

class Square : public Shape
{
public:
	explicit Square(double side)
		: side_(side)
	{}

	void Accept(const ShapeVisitor &v) override { v.visit(*this); }

	double Side() const { return side_; }

private:
	double side_;
};
```
最后，在需要执行某种操作的地方使用 `Accept()` 函数。下面是 `DrawShapes()` 函数的实现。
```cpp
void DrawShapes(const std::vector<std::unique_ptr<Shape>> &shapes)
{
    DrawVisitor drawVisitor;
    for (const auto &shape : shapes)
    {
        shape->Accept(drawVisitor);
    }
}
```
但是软件工程没有银弹，访问者模式也有缺点。

### 缺点
访问者模式是对 OOP 弱点的一种折中，因此存在许多固有问题。

第一个缺点是实现灵活性较低。如果添加一个平移操作 `Translate()`，需要实现 `TranslateVisitor` 类，并在每个 `visit()` 函数中实现平移逻辑。可以想象，这些 `visit()` 函数的实现大同小异，甚至完全相同，但我们仍不得不编写所有的 `visit()` 函数。当然可以抽取相同的逻辑以符合 DRY 原则，但基类施加的严格要求使我们仍不得不编写一些样板代码。同时，局限性还体现在 `visit()` 函数的返回类型上，返回什么类型由基类 `ShapeVisitor` 决定。

第二个缺点是添加新类型会变得非常困难。访问者模式要求一个封闭的类型集，并在此基础上提供开放的操作集。如果这个假设不成立，比如需要添加新的 `Shape` 派生类，那么整个 `ShapeVisitor` 继承体系都需要修改，基类要添加新的纯虚函数，所有派生类都要实现该函数。这就会带来我们之前讨论过的问题。导致这个问题的根本原因是 `ShapeVisitor` 基类依赖于具体图形类，因为它需要为每种图形提供一个 `visit()` 函数，具体图形类依赖于 `Shape` 基类，`Shape` 基类的 `Accept()` 函数又依赖于 `ShapeVisitor` 基类，从而形成循环依赖。添加类型需要修改这一依赖环，而添加操作与该依赖环无关。出于这个原因，经典的访问者模式也称为有环访问者（`cyclic visitor`）。

第三个缺点是访问者的侵入性。访问者模式要求在 `Shape` 类中添加一个纯虚函数 `Accept()`，这就意味着我们必须修改 `Shape` 类的定义。如果 `Shape` 类是第三方库的一部分，我们就无法修改它，访问者模式也就行不通了。后面会介绍使用 `std::variant` 实现非侵入式的访问者模式。

第四个缺点是，具体图形类的 `Accept()` 会被其派生类继承。如果有人（很可能就是未来的自己）在具体图形类下添加新的派生层次，却忘记重写（`override`）`Accept()` 函数，那么 `Accept()` 会以基类的静态类型调用 `visit()`，导致访问者模式失效，因为访问操作会被应用到错误的类型上。这也说明添加新类型较为困难，维护成本较高。

第五个缺点是每个操作都需要调用两次虚函数。起初，我们既不知道操作的类型，也不知道图形的类型。第一次调用虚函数是 `Accept()`，此时能够解析出具体的图形类型。第二次调用虚函数是 `visit()`，它接收具体图形类型的参数，此时能够解析出具体的操作类型。这种双重虚函数调用的开销可能会影响性能。

第六个缺点是，如果访问者和图形对象都动态分配，这些小的内存分配长远看来可能会导致内存碎片化。此外，这样的内存布局对缓存很不友好。通常会使用指针来操作动态分配的访问者和图形对象，因此指针间接寻址使编译器极难进行任何形式的优化。不过，这些问题不是访问者模式独有的，整个 OOP 都存在这两方面的问题。

最后一个缺点是这个模式被证明难以完全理解和维护。这看起来是一个主观缺点，不过两个继承体系之间错综复杂的相互作用所带来的复杂度确实会成为一种负担。

## 使用 `std::variant` 实现访问者模式
OOP 的继承体系存在问题，我们应该回到过程式的解决方案吗？显然不是，毕竟这种方案的问题依然存在，并且还容易出错。下面介绍一种使用 `std::variant` 实现访问者模式的方式，它可以解决其中的一些问题。

### `std::variant` 的基本用法
`std::variant` 是 C++17 引入的一个类型安全的联合体（`type-safe union`），它可以存储多种类型中的一种。`std::variant` 代表了若干可能的备选类型中的某一种。下面的示例中，定义了一个 `std::variant`，它可以存储 `int`、`double` 或 `std::string` 类型的值，但是不会同时包含多种类型的值，正常情况下也不会不包含任何值。
```cpp
#include <cstdlib>
#include <iostream>
#include <string>
#include <variant>

struct Print
{
	void operator()(int value) const { std::cout << "int: " << value << '\n'; }

	void operator()(double value) const { std::cout << "double: " << value << '\n'; }

	void operator()(const std::string &value) const { std::cout << "string: " << value << '\n'; }
};

int main()
{
	// Creates a default variant that contains an 'int' initialized to 0
	std::variant<int, double, std::string> v {};

	v = 42;		  // Assigns the 'int' 42 to the variant
	v = 3.14;	  // Assigns the 'double' 3.14 to the variant
	v = 2.71F;	  // Assigns a 'float', which is promoted to 'double'
	v = "Bjarne"; // Assigns the string literal 'Bjarne' to the variant
	v = 43;		  // Assigns the 'int' 43 to the variant

	const int i = std::get<int>(v);		  // Direct access to the value
	int *const pi = std::get_if<int>(&v); // Direct access to the value

	std::visit(Print {}, v); // Applying the Print visitor

	return 0;
}
```
默认情况下，`std::variant` 会存储第一个类型的默认值，也就是 `int` 的 0。赋值非常简单，使用 `=` 运算符即可。如果尝试赋值一个不属于备选类型之一的值，则会按常规的类型转换规则进行转换。比如上面尝试赋值一个 `float`，它会被提升为 `double`。

为了存储这些备选类型，`std::variant` 提供了刚好存储大小最大的备选类型的内部缓冲区。上面的例子中，最大的备选类型是 `std::string`，通常需要 32B（依赖于实现）。当赋值 `Bjarne` 时，清理前一个值（`double` 类型，没有什么事情需要做），然后在内部缓冲区中就地构造一个 `std::string` 对象。当又赋值成 `int` 时，`std::variant` 调用 `std::string` 的析构函数销毁对象，然后存储 `int` 值。`std::variant` 是类型安全的。

除了写入，我们还需要能够读取。我们不能直接把一个 `std::variant` 赋值给其他类型的变量，例如 `int`。访问内部存储的值有三种方式。`std::get()` 直接访问存储的值，如果类型不匹配会抛出异常。`std::get_if()` 返回一个指针，如果类型不匹配则返回 `nullptr`。第三种方式就是使用 `std::visit()`，它允许我们对存储的值进行任意操作。这里我们传递的是 `Print` 访问者，它必须为每一个可能的备选类型提供一个函数调用，否则会编译报错。这里我们提供了三个 `operator()`，分别处理 `int`、`double` 和 `std::string`。`std::visit()` 会根据存储的值的类型调用相应的函数。值得注意的是，`Print` 无需继承任何基类，也没有任何虚函数。因此也不存在因特定要求而产生的强耦合。

### 使用 `std::variant` 重构图形示例
首先我们重构 `Circle` 和 `Square`，相当简单，不再有 `Shape` 基类，也不再有虚函数 `Accept()`。因此这种实现方式是非侵入式的，可以应用于现有类型，并且完全不需要为了未来的操作提前修改这些类。
```cpp
//---- <Circle.h> ----------------
class Circle
{
public:
	explicit Circle(double radius)
		: radius_(radius)
	{}

	double Radius() const { return radius_; }

	Point Center() const { return center_; }

private:
	double radius_;
	Point center_ {};
};

//---- <Square.h> ----------------
class Square
{
public:
	explicit Square(double side)
		: side_(side)
	{}

	double Side() const { return side_; }

	Point Center() const { return center_; }

private:
	double side_;
	Point center_ {};
};
```
下面使用 `std::variant` 表示这个由 `Circle` 和 `Square` 组成的封闭类型集合。代表图形类型的这一集合抽象显而易见应该叫 `Shape`。也就是说，`std::variant` 取代了 `Shape` 基类的职责。至此，我们也不再需要智能指针了，因为之前使用智能指针是为了让同一个 `std::vector` 能够存储不同类型的图形对象。现在 `std::variant` 就可以做到这一点，因此直接将 `std::variant` 存储在 `std::vector` 中即可。
```cpp
//---- <Shape.h> ----------------
#include <Circle.h>
#include <Square.h>
#include <variant>

using Shape = std::variant<Circle, Square>;

//---- <Shapes.h> ----------------
#include <Shape.h>
#include <vector>

using Shapes = std::vector<Shape>;
```
下面实现操作 `Draw()`，它是一个访问者。遵循之前的规定，为每一个备选类型实现一个 `operator()`。和之前不同的是，这里没有基类，因而不需要重复地编写任何虚函数。也没有约束要求一定要实现成两个函数，我们也可以选择将两个 `operator()` 合并成一个函数。在返回值方面，我们也不受任何限制，可以返回任意类型的值，摆脱了基类的束缚。
```cpp
//---- <Draw.h> ----------------
#include <Circle.h>
#include <Square.h>

struct Draw
{
	void operator()(const Circle &c) const { /* ... Implementing the logic for drawing a circle ... */ }

	void operator()(const Square &s) const { /* ... Implementing the logic for drawing a square ... */ }
};
```
最后我们来实现 `DrawShapes()` 函数。它使用 `std::visit()` 来完成类型分发工作，它会访问存储在 `std::variant` 中的值，并调用访问者的相应函数。
```cpp
//---- <DrawShapes.h> ----------------
#include <Shapes.h>

void DrawShapes(const Shapes &shapes);

//---- <DrawShapes.cpp> ----------------
#include <Draw.h>
#include <DrawShapes.h>

void DrawShapes(const Shapes &shapes)
{
	for (const auto &shape : shapes)
	{
		std::visit(Draw {}, shape);
	}
}
```
我们也可以选择手动实现相同的分发工作。这可能会回到之前枚举类型方案的维护问题上，不过这么做的理由可能是考虑了性能问题，稍后讨论这个问题。
```cpp
void DrawShapes(const Shapes &shapes)
{
	for (const auto &shape : shapes)
	{
		if (const Circle *circle = std::get_if<Circle>(&shape))
		{
			//  ... Drawing a circle
		}
		else if (const Square *square = std::get_if<Square>(&shape))
		{
			// ... Drawing a square
		}
	}
}
```
有了上面的实现，我们就可以在 `main()` 函数中创建一个 `Shapes` 的集合，然后调用 `DrawShapes()` 来绘制所有的图形。我们无需再通过 `std::make_unique()` 创建对象，而是直接创建对象并添加到 `std::vector` 中即可。得益于 `std::variant` 的非 `explicit` 构造函数，可以将任何备选类型隐式转换为 `std::variant`。
```cpp
int main()
{
	Shapes shapes;
	shapes.emplace_back(Circle { 2.3 });
	shapes.emplace_back(Square { 1.2 });
	shapes.emplace_back(Circle { 4.1 });

	DrawShapes(shapes);

	return 0;
}
```
新的基于 `std::variant` 的访问者模式实现方式效果相当好。没有任何地方使用基类，没有虚函数，没有指针，没有手动内存分配，一切都很直观，样板代码很少。这个方案看起来和之前的方案大相径庭，但是核心是一致的：任何人都能在不修改现有代码的情况下添加新的操作。我们在扩展操作的维度上满足了开闭原则。下面是新的架构图，`std::variant` 与备选类型之间不存在环形依赖。
```
        Circle   Square
            ^       ^
             \     /
              \   /
               \ /
                v
           std::variant
                ^
                |
                v
               Draw
```
除了上述优势，这个架构甚至还可以构建其他抽象，比如将所有的圆形或所有的多边形组织起来，创建一个抽象的图形集合。下面是使用 `std::variant` 实现这个抽象的例子，原本的 `Shape` 抽象也不受影响。这就是 `std::variant` 这种非侵入式方案的价值！
```cpp
//---- <Shape.h> ----------------
#include <Circle.h>
#include <Square.h>
#include <variant>

using Shape = std::variant<Circle, Square>;

//---- <RoundShapes.h> ----------------
#include <Circle.h>
#include <variant>

using RoundShapes = std::variant<Circle>;

//---- <AngularShapes.h> ----------------
#include <Square.h>
#include <variant>

using AngularShapes = std::variant<Square>;
```

### 性能测试
