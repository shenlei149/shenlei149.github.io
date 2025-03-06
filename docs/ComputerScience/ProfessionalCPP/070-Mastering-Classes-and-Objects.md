友元，`friend`，能够让一个类、类成员函数、非类成员函数能够访问一个类的 `protect` `private` 成员，容易滥用，因为会破坏封装原则，因此要有限度的使用。

这一章构造了一个略微有点假的例子（裸指针动态管理内存）来说明析构函数、拷贝构造、拷贝赋值、移动构造、移动赋值等概念。

C++ 中对指针进行拷贝、赋值操作，是浅拷贝（`shallow`），仅仅复制指针的地址而不是内存，对于拷贝构造和拷贝赋值而言，需要深拷贝（`deep`），即拷贝内容。

下面是一种经典实现拷贝赋值运算符的方法，拷贝交换（`copy-and-swap`），确保异常安全。完整代码参考[这里](https://github.com/shenlei149/learn_professional_cpp/blob/3e4b2d50fa6dcbd13b60f0538c3892b0e18c0736/src/Spreadsheet/v0/Spreadsheet.cc#L30)。
```cpp
Spreadsheet &
Spreadsheet::operator=(const Spreadsheet &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	Spreadsheet temp { rhs };
	Swap(temp);

	return *this;
}
```

如果拷贝操作非常耗时或者其他原因不希望用户拷贝对象，我们可以删除拷贝构造函数和拷贝赋值运算符。这样就无法拷贝了，也没有办法按值传递这个对象给某个函数。

能够取地址的是左值（`lvalue`），比如有名的变量。不是左值的是右值（`rvalue`），比如临时变量、字面量等。下面代码中 `a` 是左值，`4 * 2` 是右值。
```cpp
int a { 4 * 2 };
```
右值引用（`rvalue reference`），顾名思义引用一个右值，这个概念应用于右值是临时对象或者是 `std::move()` 的返回对象。语法是 `type&&`。

下面是两个重载函数，一个接受左值引用，一个接受右值引用。
```cpp
void HandleMessage(std::string &message);  // lvalue reference parameter
void HandleMessage(std::string &&message); // rvalue reference parameter

std::string a { "Hello " };
HandleMessage(a); // Calls HandleMessage(string&)

std::string b { "World" };
HandleMessage(a + b); // Calls HandleMessage(string&&)

HandleMessage("Hello World"); // Calls HandleMessage(string&&)
HandleMessage(std::move(b));  // Calls HandleMessage(string&&)
```

注意，对于参数 `type&& value` 而言，`value` 的类型是右值引用，但自身是左值，因为是一个有名变量。如果需要将 `value` 作为右值使用，需要使用 `std::move()` 进行转换。

`std::move` 这个名字略微有点迷惑性，这个函数仅仅是做了一个类型转化，并没有做任何移动相关的操作，因此感觉这个函数名字叫 `as_rvalue` 可能更好点。

C++23 引入了衰退拷贝（`decay copy`），语法是 `auto{x}` 或 `auto(x)`，拷贝了一个临时对象，这个临时对象可以作为右值传递给某个函数。这里重复下上面的例子，`b` 使用 `std::move()`，此时不能确保 `b` 还能再正常使用，但是如果 `auto { b }` 这么用，不管 `HandleMessage` 如何实现，是否移动参数，都不会影响 `b` 本身。
```cpp
HandleMessage(std::move(b));

HandleMessage(auto { b });
```

移动语义（`move semantics`）需要类支持移动构造函数（`move constructor`）和移动赋值运算符（`move assignment operator`）。移动的是所有权，即将内存等资源从一个对象移动到另一个对象。

移动构造和移动赋值的声明和实现如下。声明 `noexcept` 的好处是标准库可以在条件满足的情况下使用移动而不是拷贝以提升性能。
```cpp
Spreadsheet(Spreadsheet &&src) noexcept;
Spreadsheet &operator=(Spreadsheet &&rhs) noexcept;

Spreadsheet::Spreadsheet(Spreadsheet &&src) noexcept
{
	Swap(src);
}

Spreadsheet &
Spreadsheet::operator=(Spreadsheet &&rhs) noexcept
{
	Swap(rhs);
	return *this;
}
```

这里直接使用 `Swap()` 函数将新对象和旧对象交换，方便简洁，不过这也有一个问题，移动赋值的左边变量的内存给了右边变量，那么存活时间可能更长。因为标准也没有规定如何实现，只是说被移动的对象不再保证可用罢了。如果希望立即释放资源，那么可以使用直接朴素的实现，先释放自己的资源，然后接管被移动对象的资源，给被移动对象赋值适合的值，比如 `nullptr`。

如果一个函数 `return` 一个无名临时对象，C++17 开始强制要求省略拷贝或移动操作（`mandatory elision of copy/move operations`）。如果返回的对象是局部变量而不是参数，不强制要求省略拷贝或移动操作（`non-mandatory elision of copy/move operations`），不过优化器往往会做 NRVO（`named return value optimization`）优化。因此，不要写 `return std::move(object)` 这样的代码，会导致编译器无法优化，如果不支持移动操作，那么会进行拷贝，性能损失就比较大了。

条件表达式并不会被优化，因此，下面第二种写法更好，能优化。
```cpp
return condition ? obj1 : obj2;

if (condition)
{
	return obj1;
}
else
{
	return obj2;
}
```
什么时候使用 `std::move()` 返回呢？一个例子是返回成员变量但是不再需要使用时，可以使用替代拷贝以提升性能。

本书还讨论了一个问题，就是有了左值引用和右值引用，有些函数，比如 `SetData` 这种成员函数，可能需要实现两个重载以实现性能最大化，但是两个重载不好维护。一个更好的写法是直接按值传递，此时只比性能最好的实现多一次额外移动。因此总是要拷贝且移动比拷贝开销小很多的时候，可以考虑按值传递，不过对于多态场景不使用。[这里](https://guozi149.me/ComputerScience/EffectiveModernCpp/ch08_Tweaks/41_Consider_pass_by_value_for_copyable_parameters_that_are_cheap_to_move_and_always_copied/)有详细讨论。

移动构造和移动赋值和拷贝类似，也可以给一个默认实现 `=default` 或者删除 `=delete`。移动构造和移动赋值也可以由编译器自动合成，不过一旦自己声明了析构函数、拷贝构造或拷贝赋值，编译器就不再自动生成了。

从上述分析可以看出，析构函数、拷贝和移动构造、拷贝和移动赋值这五个函数比较特殊，应该要么都提供自己的实现，要么都用默认实现，或者都删除。现代 C++ 最好的设计是都使用默认实现，比如上面的例子可以用 `std::vector` 自行管理内存。如果类有自己的资源需要管理，那么就需要为这五个函数提供自定义的版本。

类中能包含 `static` 函数，如果一个函数不需要访问类实例相关的函数或者变量，就可以也应该声明为 `static` 的。和普通的函数没有太大区别，主要就是属于类而不是直接属于某个命名空间。

如果一个函数不修改任何成员变量，可以也应该声明为 `const`。有的时候，逻辑上是 `const`，对用户而言是不变的，但是实际上修改了某个变量，比如统计信息，那么可以在需要修改的变量前面加上 `mutable`。

成员函数也可以重载，即函数名一样，但是参数类型不同或者个数不同。这里需要强调一种重载，就是加不加 `const`，这类函数很有用，比如 `const` 对象可以获取内部 `const` 的某个数据。由于两者的实现可能会近乎一样，所以可以使用下面这种方式来避免代码 重复：可以的话让非 `const` 版本调用 `const` 版本。
```cpp
T& ClassName::FunctionName()
{
	return const_cast<T&>(std::as_const(*this).FunctionName());
}
```
另一中解决方法就是抽取一个私有的公共函数，但是上述本质上就是让 `const` 版本就是这个函数了。

重载函数也可以显式删除，目的是不能使用某种类型的参数。

引用也可以修饰成员函数，`&` 放到函数签名最后，左值对象可以调用，`&&` 放到最后，右值对象可以调用。这么做可以提升性能。另外一个用途是可以禁止右值对象调用，比如一个耗时操作修改内部，但是右值对象很快就要销毁，这么做完全是浪费。

对于引用修饰的写法，使用显式对象参数语法更繁琐，但是引用修饰更清晰一点。如下所示。
```cpp
const std::string &
GetText(this const TextHolder &self)
{
	return self.text_;
}

std::string &&
GetText(this TextHolder &&self)
{
	return move(self.text_);
}

const std::string &
GetText() const &
{
	return text_;
}

std::string &&
GetText() &&
{
	return std::move(text_);
}
```

`inline` 仅仅是给编译器一个提示，但是编译器觉得有损性能的话，会忽略。反之，如果不写，编译器如果觉得有必要，那么也会 `inline`。现在 `inline` 更多的用于头文件中的一些函数，这样只在一个编译单元内生效。

C++ 支持默认参数，从最右边的参数开始给定一个默认值，这样无需写多个重载，但是由于 C++ 不支持命名参数，所以有时还是要增加一些重载以跳过一个参数。

C++ 支持常量表达式，很多时候也需要常量表达式，比如数组的大小。一个函数，加上 `constexpr`，表示可以用于编译期求值，当然也可以像普通函数一样运行时使用。
```cpp
constexpr int
getArraySize()
{
	return 32;
}

int myArray[getArraySize()];
```
`constexpr` 函数不能抛出异常，不能有副作用。可以随意调用 `constpexr` 函数，但是编译期求值的路径上不能调用普通函数。C++23 开始，`constexpr` 函数可以使用 `goto`，`static` 和 `static constexpr` 变量。

`consteval` 类似，不过只能用于编译期求值。C++ 23 开始，可以在 `constexpr` 函数里面调用，可以通过 `if consteval` 检查是否在进行常量求值。

`constexpr` 和 `consteval` 可以用于类的构造函数和其他类成员函数，这样就能编译期构造自定义对象了。两者的区别和作用于函数是一致的。对于默认函数，不管是隐式还是显式通过 `=default` 指定，都隐式的添加了 `constexpr`，除非有成员变量包含非 `constexpr` 构造函数或成员函数。
```cpp
class Matrix
{
public:
	Matrix() = default; // implicitly constexpr

	constexpr explicit Matrix(unsigned rows, unsigned columns)
		: rows_ { rows }
		, columns_ { columns }
	{
	}

	constexpr unsigned
	getSize() const
	{
		return rows_ * columns_;
	}

private:
	unsigned rows_ { 0 };
	unsigned columns_ { 0 };
};

constexpr Matrix matrix { 8, 2 };
constexpr Matrix matrixDefault;

int linearizedMatrix[matrix.getSize()];
```

除了普通的成员变量之外，还支持 `static` 的成员变量，属于类而不是类的实例。`static T Name` 这样只是声明了变量，在实现的文件中还需要定义一下 `T ClassName::Name { value }`，目的是分配空间。不过现在支持 `inline` 方式声明，就无需额外定义了。

如果不想修改 `static`，还可以添加 `const` `constexpr` 修饰，比如定义一些类级别的常量，用于默认参数或者其他场景。

在类的外部，使用 `ClassName::StaticVariable` 的方式访问。
