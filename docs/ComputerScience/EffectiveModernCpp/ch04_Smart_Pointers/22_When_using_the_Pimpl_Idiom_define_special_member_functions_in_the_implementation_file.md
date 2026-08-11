如果和编译时间打过交道，那么可能会熟悉 Pimpl（`pointer to implementation`）惯用法。原来就是用指向具体实现的指针取代数据字段，将这些字段放到实现类中，然后通过指针来放着这些字段。比如
```cpp
class Widget // in header "widget.h"
{
public:
    Widget();

private:
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3; // Gadget is some user-defined type
};
```
由于 `Widget` 的成员类型有 `std::string, std::vector, Gadget`，那么为了能够编译 `"widget.h"` 这个文件必须包含头文件 `<string>, <vector>, gadget.h`，这增加了编译使用 `Widget` 的客户端代码的时间。一旦头文件变化了，那么这些客户端代码也要重新编译。STL 不会经常修改，但是 `gadget.h` 可能会频繁修改。

我们先使用 C++98 的方式实现 Pimpl。成员变量被替换成指向一个只声明但是没有定义的结构体的指针。
```cpp
class Widget // still in header "widget.h"
{
public:
    Widget();
    ~Widget(); // dtor is needed—see below

private:
    struct Impl; // declare implementation struct
    Impl *pImpl; // and pointer to it
};
```
由于 `Widget` 不再依赖于 `std::string, std::vector, Gadget`，那么也不需要添加这些头文件。这会缩短时间，另外，如果涉及这些类型的头文件修改了，那么也不会影响使用 `Widget` 的客户端编译。

只声明没定义的类型称为不完整类型（`incomplete type`）。`Widget::Impl` 就是这样的类型。对于不完整类型，有效的操作很少，不过定义一个指向它的指针是合法的。

上述是 Pimpl 惯用法的第一部分。第二部分是动态的创建和析构一个对象，这个对象包含被替代的成员变量，这些代码都写在实现类中。
```cpp
// file "widget.cpp"
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl // definition of Widget::Impl
{                   // with data members formerly in Widget
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget()      // allocate data members for
    : pImpl(new Impl) // this Widget object
{
}

Widget::~Widget() // destroy data members for this object
{
    delete pImpl;
}
```
这个 `.cpp` 文件 `#include` 的头文件 `<string>, <vector>, gadget.h`，这些没有被包含在 `.h` 文件中，所以能够得到上述收益。

正如前面的章节讨论的，C++11 中推荐使用智能指针代替裸指针，下面，使用 `std::unique_ptr` 替代裸指针。
```cpp
// file "widget.h"
class Widget
{
public:
    Widget();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl; // use smart pointer instead of raw pointer
};

// file "widget.cpp"
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl
{ // as before
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget()                      // per Item 21, create std::unique_ptr
    : pImpl(std::make_unique<Impl>()) // via std::make_unique
{
}
```
`Widget` 的析构函数不见了，因为当 `std::unique_ptr` 析构时，会自动析构指向的对象。这样就无需我们自己管理资源了。

这些代码能够编译，但是客户端代码无法编译。
```cpp
#include "widget.h"
Widget w; // error!
```
报错信息依赖于编译器，不过差不多就是说无法将 `sizeof` `delete` 作用于一个不完整类型。

实现 Pimpl 惯用法，是 `std::unique_ptr` 最常见的场景。另外，`std::unique_ptr` 宣传说可以与不完整类型配合使用的。我们这里需要稍作调整即可通过编译。

原因是当 `w` 离开其作用域时需要被销毁。但是我们没有写析构函数，那么编译器会自动生成一个。自动生成的析构函数会销毁字段 `pImpl`，其类型是 `std::unique_ptr<Widget::Impl>` 使用默认的删除器，使用 `delete` 析构 `std::unique_ptr` 指向的对象。C++11 的默认删除器的实现往往会使用 `static_assert` 确保裸指针指向的对象不是一个不完成类型的对象。同时，编译器生成的析构函数相当于实现在了头文件中，隐含着 `inline`。此时，类型确实是不完整的。报错信息往往出现在定义 `w` 的这一行，因为它的创建导致在析构的时候出现这一系列问题。

为了修复这个问题，需要在销毁 `std::unique_ptr<Widget::Impl>` 时使得 `Widget::Impl` 是一个完整的类型，而 `Widget::Impl` 定义在 `widget.cpp`。那么需要在 `.cpp` 文件中定义析构函数，并在 `Widget::Impl` 之后。

这样，我们需要在头文件声明析构函数。然后在 `.cpp` 中定义它且在 `Widget::Impl` 之后。
```cpp
// file "widget.h"
class Widget
{
public:
    Widget();
    ~Widget(); // declaration only

private: // as before
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// file "widget.cpp"
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl
{ // as before
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() // as before
    : pImpl(std::make_unique<Impl>())
{
}

Widget::~Widget() // ~Widget definition
{
}
```
这就能正常工作了。如果想要强调我们使用编译器生成的析构函数，可以写作
```cpp
Widget::~Widget() = default; // same effect as above
```

使用 Pimpl 惯用法的类型自然支持移动操作，编译器自动生成的移动操作也能正常工作，移动 `std::unique_ptr` 对象。但是我们声明定义了析构函数，这组织了编译器自动生成移动操作。为了弥补这一点，我们可以手动添加上。
```cpp
// file "widget.h"
class Widget
{
public:
    Widget();
    ~Widget();

    Widget(Widget &&rhs) = default;            // right idea,
    Widget &operator=(Widget &&rhs) = default; // wrong code!

private: // as before
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
```
这种方法遇到的问题和之前没有声明定义析构函数遇到的问题一样，原因也是一样的，不再赘述。解决方案也一样，分离声明和定义，且将定义放到 `Widget::Impl` 定义之后。

使用 Pimlp 惯用法改造一个类，并不应该改变其行为。原始的 `Widget` 类包含 `std::string, std::vector, Gadget` 类型的成员变量，也假设 `Gadget` 和 `std::string, std::vector` 支持复制。我们不得不写拷贝相关操作的函数，因为 1）有了 `std::unique_ptr` 是一个仅能移动的对象，编译器不再帮我们生成拷贝操作了；2）默认生成的实现对 `std::unique_ptr` 进行浅拷贝，但是我们期望是深拷贝。

声明和定义如下。
```cpp
// file "widget.h"
class Widget
{
public:
    // other funcs, as before

    Widget(const Widget &rhs);            // declarations
    Widget &operator=(const Widget &rhs); // only

private: // as before
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// file "widget.cpp"
#include "widget.h"

struct Widget::Impl // as before
{
};
Widget::~Widget() = default; // other funcs, as before

Widget::Widget(const Widget &rhs) // copy ctor
    : pImpl(std::make_unique<Impl>(*rhs.pImpl))
{
}

Widget &Widget::operator=(const Widget &rhs) // copy operator=
{
    *pImpl = *rhs.pImpl;
    return *this;
}
```
两个实现都是从源对象（`rhs`）拷贝 `Impl` 到目标对象（`*this`）。对于 `Impl` 对象，编译器生成的拷贝操作就能对 `Impl` 进行逐字段的拷贝，我们只需要使用而不必再一个一个字段的拷贝。

为了实现 Pimpl 惯用法，这里使用了智能指针 `std::unique_ptr` 
替代裸指针。如果使用 `std::shared_ptr` 的话，以上建议都不再使用。因为一切都工作的很好！
```cpp
class Widget
{
public:
    Widget();
    // no declarations for dtor
    // or move operations

private:
    struct Impl;
    std::shared_ptr<Impl> pImpl; // std::shared_ptr
};
```

`#include "widget.h"` 的客户端代码如下。
```cpp
Widget w1;
auto w2(std::move(w1)); // move-construct w2
w1 = std::move(w2);     // move-assign w1
```
一切都和我们期望的一致：默认构造 `w1`，`w1` 移动构造了 `w2`，`w2` 又移动赋值给 `w1`，最后两个对象都正确销毁。

`std::shared_ptr` 和 `std::unique_ptr` 在实现 Pimpl 惯用法时有差异的原因两者支持自定义删除器的方式不同。对于 `std::unique_ptr` 而言，删除器的类型是智能指针的一部分，好处是能够生成更小的运行时数据结构和运行更快的代码，但是缺点就是要求编译器自动生成特殊成员函数时，指向的类型必须是完整类型。对 `std::shared_ptr` 而言，删除器的类型不是智能指针的一部分，需要更大的运行时数据结构和运行稍慢的代码，好处是自动生成特殊成员函数时不需要指向的类型是完整类型。

对于实现 Pimpl 惯用法这个问题，在这两者之间没有一个好的折中。像这里 `Widget` 和 `Widget::Impl` 的例子，是独占所有权，那么使用 `std::unique_ptr` 更合适一些。如果一些场景，共享所有权，此时 `std::shared_ptr` 就是很好的选择了，还摆脱了自己声明定义特殊成员函数的麻烦。

## Things to Remember
* The Pimpl Idiom decreases build times by reducing compilation dependencies between class clients and class implementations.
* For `std::unique_ptr` pImpl pointers, declare special member functions in the class header, but implement them in the implementation file. Do this even if the default function implementations are acceptable.
* The above advice applies to `std::unique_ptr`, but not to `std::shared_ptr`.
