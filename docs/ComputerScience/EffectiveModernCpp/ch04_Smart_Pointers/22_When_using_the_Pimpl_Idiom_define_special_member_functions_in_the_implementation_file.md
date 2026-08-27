如果和编译时间打过交道，那么可能会熟悉 Pimpl（`pointer to implementation`）惯用法。它本质上是用指向具体实现的指针取代数据成员，把这些成员放到实现类中，然后通过指针来访问它们。比如
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
由于 `Widget` 的成员类型有 `std::string`、`std::vector` 和 `Gadget`，因此要编译 `"widget.h"` 就必须包含头文件 `<string>`、`<vector>` 和 `gadget.h`，这会增加编译使用 `Widget` 的客户端代码所需的时间。一旦这些头文件变化，使用 `Widget` 的客户端代码也要重新编译。STL 不会经常修改，但是 `gadget.h` 可能会频繁修改。

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
由于 `Widget` 不再依赖于 `std::string`、`std::vector` 和 `Gadget`，因此也不需要包含这些头文件。这会缩短编译时间；另外，如果这些类型相关的头文件发生修改，也不会影响使用 `Widget` 的客户端代码编译。

只声明但未定义的类型称为不完整类型（`incomplete type`）。`Widget::Impl` 就是这样的类型。对于不完整类型，有效的操作很少，不过定义一个指向它的指针是合法的。

上述是 Pimpl 惯用法的第一部分。第二部分是在运行时创建并销毁一个对象，这个对象包含被替代的成员变量；这些代码都写在实现类中。
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
这个 `.cpp` 文件 `#include` 了 `<string>`、`<vector>` 和 `gadget.h`，这些头文件不需要出现在 `.h` 文件中，因此可以获得上述收益。

正如前面的章节所讨论的，C++11 推荐使用智能指针代替裸指针，下面使用 `std::unique_ptr` 替代裸指针。
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
报错信息依赖于编译器，不过大致就是说无法对一个不完整类型使用 `delete`。

实现 Pimpl 惯用法是 `std::unique_ptr` 最常见的用途之一。另外，`std::unique_ptr` 号称可以与不完整类型配合使用。我们这里只需稍作调整即可通过编译。

原因是当 `w` 离开其作用域时必须被销毁。但我们没有显式编写析构函数，所以编译器会自动生成一个。自动生成的析构函数会销毁成员 `pImpl`；`pImpl` 的类型是 `std::unique_ptr<Widget::Impl>`，而默认删除器会用 `delete` 销毁它指向的对象。C++11 默认删除器的实现通常会使用 `static_assert` 确保所指对象不是不完整类型。同时，编译器生成的析构函数等价于在头文件中定义，因此隐含 `inline`。此时，类型确实是不完整的。报错信息往往出现在定义 `w` 的这一行，因为此处需要实例化析构逻辑。

为了修复这个问题，必须保证销毁 `std::unique_ptr<Widget::Impl>` 时 `Widget::Impl` 是完整类型，而这要求将析构函数定义在 `widget.cpp` 中，并放在 `Widget::Impl` 定义之后。

这样，我们需要在头文件中声明析构函数，然后在 `.cpp` 中定义它，并放在 `Widget::Impl` 之后。
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
这就能正常工作了。如果想要明确表示使用编译器生成的析构函数，可以写作
```cpp
Widget::~Widget() = default; // same effect as above
```

使用 Pimpl 惯用法的类型自然支持移动操作，编译器自动生成的移动操作也能正常工作，因为它们只是移动 `std::unique_ptr` 对象。但我们声明了析构函数，这阻止了编译器自动生成移动操作。为了弥补这一点，我们需要手动添加它们。
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
这里遇到的问题与前面未声明析构函数时相同，原因也一样，不再赘述。解决方案同样是将声明和定义分离，并把定义放到 `Widget::Impl` 之后。

使用 Pimpl 惯用法改造一个类，并不应该改变其行为。原始的 `Widget` 类包含 `std::string`、`std::vector` 和 `Gadget` 类型的成员变量，也假设 `Gadget` 与 `std::string`、`std::vector` 都支持复制。我们仍然需要编写拷贝相关操作，因为 1）`std::unique_ptr` 只能移动，编译器不会再为我们生成可用的拷贝操作；2）Pimpl 的语义应当是深拷贝，而不是简单复制指针。

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
两个实现都是把源对象（`rhs`）中的 `Impl` 复制到目标对象（`*this`）。对于 `Impl`，编译器生成的拷贝操作就能逐字段复制，我们只需直接使用，不必再逐个字段手写拷贝。

为了实现 Pimpl 惯用法，这里使用智能指针 `std::unique_ptr` 替代裸指针。如果改用 `std::shared_ptr`，上述建议就不再适用，因为一切都能正常工作。
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

`std::shared_ptr` 和 `std::unique_ptr` 在实现 Pimpl 惯用法时有差异，原因是两者支持自定义删除器的方式不同。对于 `std::unique_ptr` 而言，删除器的类型是智能指针的一部分，好处是能够生成更小的运行时数据结构和更快的代码，但是缺点是编译器自动生成某些特殊成员函数时，所指类型必须是完整类型。对 `std::shared_ptr` 而言，删除器的类型不是智能指针的一部分，需要更大的运行时数据结构和稍慢的代码，好处是自动生成特殊成员函数时不要求所指类型是完整类型。

不过，使用 `std::shared_ptr` 会使编译器生成的拷贝操作共享同一个 `Impl` 对象，而不是原始 `Widget` 的深拷贝语义。若仍需要深拷贝，必须显式定义拷贝构造函数和拷贝赋值运算符。

对于实现 Pimpl 惯用法这个问题，在这两者之间没有完美折中。像这里 `Widget` 和 `Widget::Impl` 的例子属于独占所有权，因此使用 `std::unique_ptr` 更合适；如果某些场景需要共享所有权，那么 `std::shared_ptr` 就是更好的选择，也免去了手动声明和定义特殊成员函数的麻烦。

## Things to Remember
* The Pimpl Idiom decreases build times by reducing compilation dependencies between class clients and class implementations.
* For `std::unique_ptr` pImpl pointers, declare special member functions in the class header, but implement them in the implementation file. Do this even if the default function implementations are acceptable.
* The above advice applies to `std::unique_ptr`, but not to `std::shared_ptr`.
