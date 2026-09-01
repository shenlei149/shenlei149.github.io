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
这个方案看起来顺眼多了，不过它也有缺点。

