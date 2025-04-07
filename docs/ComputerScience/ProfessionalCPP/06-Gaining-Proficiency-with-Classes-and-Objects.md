## 类的定义与实现
下面是写在头文件中类的声明。尽可能的使用内类初始化，比如这里就初始化了成员变量 `value_`。
```cpp
#include <string>

class SpreadsheetCell
{
public:
	void SetValue(double value);
	double GetValue() const;

	void SetString(std::string_view value);
	std::string GetString() const;

private:
	std::string DoubleToString(double value) const;
	double StringToDouble(std::string_view value) const;

private:
	double value_ { 0 };
};
```

下面是类的实现，成员函数的定义。
```cpp
#include "SpreadsheetCell.h"

#include <charconv>

void
SpreadsheetCell::SetValue(double value)
{
	value_ = value;
}

double
SpreadsheetCell::GetValue() const
{
	return value_;
}

void
SpreadsheetCell::SetString(std::string_view value)
{
	value_ = StringToDouble(value);
}

std::string
SpreadsheetCell::GetString() const
{
	return DoubleToString(value_);
}

std::string
SpreadsheetCell::DoubleToString(double value) const
{
	return std::to_string(value);
}

double
SpreadsheetCell::StringToDouble(std::string_view value) const
{
	double number { 0.0 };
	std::from_chars(value.data(), value.data() + value.size(), number);
	return number;
}
```

C++23 引入了显式对象参数（`explicit object parameter`），因此对于成员函数，可以显式传入 `this`，那么成员函数定义如下。
```cpp
void
SpreadsheetCell::SetValue(this SpreadsheetCell &self, double value)
{
	self.value_ = value;
}
```
对于这个例子而言，这么写没有任何好处。但是在显式指定引用修饰符、成员模板函数将对象参数作为模板（简化 `const` 和非 `const` 两个版本）、递归 lambda 表达式时是有用的。

## 类的生命周期
对象的生命周期涉及三件事：创建、销毁和赋值。

首先看下创建。默认情况下，编译期会生成一个默认构造函数，另个参数，由于前面写了内类初始化，因此 `value_` 会初始化成 0。

也可以写我们自己的构造函数，比如下面是两个构造函数的声明和定义。这里使用了列表初始化，推荐使用这种方式。对于 `const` 成员变量、引用成员变量、没有默认构造函数的成员变量、基类没有默认构造函数，那么必须使用。这里还体现了函数重载，即参数不同但是函数名相同。
```cpp
SpreadsheetCell(double initValue);
SpreadsheetCell(std::string_view initValue);

SpreadsheetCell::SpreadsheetCell(double initValue)
	: value_ { initValue }
{
}

SpreadsheetCell::SpreadsheetCell(std::string_view initValue)
	: value_ { StringToDouble(initValue) }
{
}
```
一旦我们实现了自己的构造函数，那么编译器就不会再自动生成默认构造函数，那么可能会影响使用的便利性，比如定义对象的数组，比如继承类的构造。是否提供默认构造函数，更多的应该考虑语义是否必要，便利性次之。对于当前类而言，可以有一个，而且无需额外操作，自动生成的就满足需求，那么可以如下给出定义。
```cpp
SpreadsheetCell() = default;
```

和显式默认定义默认构造函数相对的是也可以显式删除默认构造函数。如果一个类有删除了默认构造函数的的数据成员，那么这个类的默认构造函数也会被删除。
```cpp
SpreadsheetCell() = delete;
```

这里有个事情需要强调一下，C++ 初始化成员的顺序是在类内定义的顺序，不是构造函数的初始化列表的顺序，因此，初始化列表顺序应当与定义顺序一致，否则可能会出现非预期行为。

还有一种构造函数是拷贝构造函数，参数是 `const` 引用类型，比如这里的类的构造函数的声明和实现如下。
```cpp
SpreadsheetCell(const SpreadsheetCell &src);

SpreadsheetCell::SpreadsheetCell(const SpreadsheetCell &src)
	: value_ { src.value_ }
{
}
```
这样的实现没有太大的意义，因此也可以直接使用 `default`。
```cpp
SpreadsheetCell(const SpreadsheetCell &src) = default;
```
注意，拷贝构造函数也是构造函数，因此一旦定义了拷贝构造函数，编译器就不会再生成默认构造函数了。

和之前类似，也能删除拷贝构造函数。
```cpp
SpreadsheetCell(const SpreadsheetCell &src) = delete;
```
当函数参数按值传递、显式的通过一个实例构造另一个实例，就会用到拷贝构造函数。

构造函数也可以接受 `initializer_list<T>` 作为参数类型，使用 `size()` 可以知道其个数，可以使用 `range for` 来遍历元素。很多时候能够方便使用，比如初始化容器的时候。

如果类支持 `initializer_list<T>` 构造对象，那么需要小心，使用大括号构造的时候，一定会使用初始化列表的构造函数，而使用小括号构造的时候可能会走到其他构造函数。比如下面 `std::vector` 的例子，两者差别很大。
```cpp
vector<int> v1 { 6 }; // Constructs a vector with a single element, 6.
vector<int> v2(6);	  // Constructs a vector with 6 default-nitialized elements.
```

构造函数可以调用其他构造函数，但是不要相互依赖。比如接受 `std::string_view` 的构造函数，可以直接初始化成员变量，也可以调用另一个构造函数来初始化。
```cpp
SpreadsheetCell::SpreadsheetCell(std::string_view initValue)
	: SpreadsheetCell { StringToDouble(initValue) }
{
}
```

由于类有一参版本的构造函数，可能会将其他类型的对象转化成其他类型的对象。比如下面的 `double` 数可以直接给 `SpreadsheetCell` 类型赋值，这里就有转化。
```
SpreadsheetCell cell = 5;
```
这种转化可能是无意的，可以在构造函数前面加上 `explicit` 避免这种情况。推荐至少给一参版本的构造函数加上 `explicit` 确保不会有无意的类型转化。对于多参版本的构造函数，也可以加，不过使用时候至少需要大括号括起来，这样代码看起来是在构造新的类型，因此也可以不加，问题比一参版本的少很多。

当对象被销毁的时候，比如智能指针被销毁了，会调用析构函数（`destructor`），然后释放内存。析构函数中可以执行清理工作，释放动态分配的内存、关闭文件等。析构函数是在构造函数前面加一个 `~` 来声明和定义。成员变量构造顺序是成员变量声明的顺序，而析构顺序恰恰相反。

我们希望能够像 `int` 赋值一样可以给自定义类型（类）赋值，此时就需要重载赋值运算符（`assignment operator`）`operator=`，参数类型是 `const` 引用。
```cpp
SpreadsheetCell &operator=(const SpreadsheetCell &rhs);
```
返回类型是引用（可以返回其他类型甚至是 `void`，但是使用者会期待返回的是引用）的话可以链式调用。
```cpp
myCell = anotherCell = aThirdCell;

myCell.operator=(anotherCell.operator=(aThirdCell));
```
实现的话和拷贝构造函数类似，不过由于可以自己给自己赋值，所以当出现这种情况时，最好什么也不做直接返回，正确且高效。避免的方式如下。
```cpp
if (this == &ths)
{
	return *this;
}
```
和拷贝构造函数一样，可以给一个默认实现或删除赋值运算符。

C++11 开始，如果有自定义的析构函数或赋值运算符，会放弃自动生成拷贝构造函数，如果有自定义的析构函数或拷贝构造函数，也会放弃生成赋值运算符。这是因为这几个函数要么都需要特殊实现，要么默认生成的就足够了。

拷贝构造和赋值运算符有一个核心区别，前者是构造，说明还没有对象，后者已经有了对象再次赋值。因此下面前面三行都是构造，特别是第三行，虽然是 `=`，但是仅仅是第二行的另一种写法罢了，只有最后一个是赋值。
```cpp
SpreadsheetCell myCell { 5 };
SpreadsheetCell anotherCell { myCell };
SpreadsheetCell aThirdCell = myCell;

anotherCell = myCell; // Calls operator= for anotherCell
```
在实现构造函数的时候，如果在函数体中实现，调用的是赋值运算符，因为此时对象已经存在并且初始化好了，使用初始化列表的写法，对象还没有构造，因此调用的是拷贝构造函数。
