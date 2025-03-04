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
