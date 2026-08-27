C++ 特殊成员函数是指编译器可以自动生成的成员函数。C++98 中有四个：默认构造函数、析构函数、拷贝构造函数和拷贝赋值运算符。隐式声明的特殊成员函数会在需要时由编译器定义。只有当一个类没有用户声明的构造函数时，编译器才会生成默认构造函数。自动生成的特殊成员函数是 `public` 且是 `inline` 的。若基类的析构函数是虚函数，自动生成的派生类析构函数也是虚函数；除此之外，自动生成的特殊成员函数默认都是非虚函数。

随着时间流逝，C++ 中生成特殊成员函数的规则也发生了变化。

C++11 中，有两个新的特殊成员函数：移动构造函数和移动赋值运算符。签名如下：
```cpp
class Widget
{
public:
    Widget(Widget &&rhs);            // move constructor
    Widget &operator=(Widget &&rhs); // move assignment operator
};
```

移动操作相关函数的生成规则和拷贝操作类似：只有在满足特定条件时才会隐式声明；如果被定义，其行为是逐个移动所有非 `static` 成员变量。移动构造函数会对参数 `rhs` 中的每个非 `static` 成员进行移动构造，移动赋值运算符也是如此。如果存在基类子对象，也会以同样的方式处理。

即使使用了移动构造或移动赋值，移动操作本身也不一定会发生。对成员变量的“移动”更像是一次移动请求：如果该类型不可移动（例如某个 C++98 类型），则会以拷贝代替移动。编译器会将源对象的成员视为右值，并在编译期通过重载决议选择移动或拷贝操作；并不是在运行时决定的。详见 [Item 23](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/23_Understand_std_move_and_std_forward.md)。

与拷贝类似，一旦我们自己声明了移动操作，编译器就不会自动生成另一个移动操作。不过细节略有不同。

两个拷贝函数是独立的。如果只声明其中一个，不会阻止编译器自动生成另一个。例如，声明了拷贝构造函数但又使用了拷贝赋值运算符时，编译器仍会自动生成后者；反之亦然。在 C++11 中，这种自动生成会被弃用。

两个移动函数不是独立的。如果声明了其中一个，编译器就不会自动生成另一个。如果我们声明了其中一个，例如移动构造函数，说明默认行为与所需行为不同；因此，编译器无法推断出合适的移动赋值运算符。也就是说，声明移动构造函数会抑制编译器自动生成移动赋值运算符；反之亦然。

类似地，如果一个类声明了拷贝函数，也不会自动生成移动函数。声明拷贝函数意味着自动生成的逐字段复制不适合当前类；因此，编译器会保守地认为逐字段移动也可能不适合该类。

反方向也成立。声明了移动函数后，隐式声明的拷贝函数会被定义为 `delete`。理由也类似。这个新增限制不会破坏 C++98 代码，因为旧代码没有移动函数。不过，若在遗留代码中为提升性能而新增移动函数，自动生成特殊成员函数的规则就会改变。

三原则（`Rule of Three`）是说，一旦声明了析构函数、拷贝构造函数和拷贝赋值运算符中的一个，通常也应声明另外两个。这条原则源自长期观察：需要自行管理资源的类往往需要自定义全部三个函数。几乎所有 STL 容器都显式声明了这三个函数中的至少一个。

三原则的一个重要结论是：如果用户声明了析构函数，简单的逐个拷贝成员变量可能不再适用。在 C++98 时代，这个原则没有受到应有的重视：即使有用户声明的析构函数，编译器仍会自动生成拷贝操作。为了兼容性，C++11 仍然保留了这一行为。

三原则背后的原因还是有效的，再加上之前关于移动操作的观察，在 C++11 引入移动操作时，为新的操作新增了限制。

因此，只有在下面三个条件都满足时，编译器才会自动生成移动操作：
* 没有用户声明的拷贝操作；
* 没有用户声明的移动操作；
* 没有用户声明的析构函数。

类似的规则有时也适用于拷贝操作：在 C++11 中，如果声明了析构函数或另一个拷贝操作，自动生成相应拷贝操作的行为会被弃用。如果现有类声明了析构函数或一个拷贝操作，却依赖自动生成另一个拷贝操作，就需要更新代码。若自动生成的行为符合预期，只需添加 `= default`。
```cpp
class Widget
{
public:
    ~Widget();                                   // user-declared dtor
    Widget(const Widget &) = default;            // default copy ctor behavior is OK
    Widget &operator=(const Widget &) = default; // default copy assign behavior is OK
};
```

这种方式通常在多态场景中很有用。基类通常需要虚析构函数，否则通过基类指针执行某些操作（例如 `delete`）会出错。声明析构函数后，编译器就不会自动生成移动操作。如果默认移动操作正确，应显式声明移动操作并用 `= default` 实现。C++11 中，声明移动操作会使隐式声明的拷贝操作被定义为删除；如果默认拷贝操作也正确，同样用 `= default` 显式声明即可。
```cpp
class Base
{
public:
    virtual ~Base() = default; // make dtor virtual
    Base(Base &&) = default;   // support moving
    Base &operator=(Base &&) = default;
    Base(const Base &) = default; // support copying
    Base &operator=(const Base &) = default;
};
```
即使编译器可以自动生成移动操作和拷贝操作，且默认行为符合要求，也应考虑手动声明这些函数并使用 `= default`。这能更好地表明意图；更重要的是，能避免一些潜在的 bug。例如，有一个存储字符串的类，可以通过整数快速查找所需的字符串。
```cpp
class StringTable
{
public:
    StringTable() {}
    // functions for insertion, erasure, lookup,
    // etc., but no copy/move/dtor functionality
private:
    std::map<int, std::string> values;
};
```
这个类没有显式声明析构函数、拷贝操作或移动操作，编译器会自动生成它们。

后来，因为需要，在构造函数和析构函数中打印了日志：
```cpp
class StringTable
{
public:
    StringTable() // added
    {
        makeLogEntry("Creating StringTable object");
    }

    ~StringTable() // also added
    {
        makeLogEntry("Destroying StringTable object");
    }

    // other funcs as before

private:
    std::map<int, std::string> values; // as before
};
```
这看起来很合理，但添加析构函数后，编译器无法再自动生成移动操作。拷贝操作不受影响。此时，本应调用移动操作的表达式会调用拷贝操作。拷贝 `std::map<int, std::string>` 的速度可能比移动它慢几个数量级。仅仅增加一个析构函数就可能导致性能问题；若显式声明拷贝操作和移动操作，并使用 `= default`，即可避免这个问题。

对于默认构造函数和析构函数，C++11 中的规则和 C++98 中的规则基本一致。

C++11 中特殊成员函数的处理规则如下：
* 默认构造函数：与 C++98 一致。只有没有用户声明的构造函数时，才会自动生成默认构造函数。
* 析构函数：与 C++98 基本一致。其异常规范由基类和成员的析构函数推导得出。与 C++98 一致，只有当某个基类的析构函数是虚函数时，该类的析构函数才是虚函数。
* 拷贝构造函数：行为与 C++98 一致，对非 `static` 成员变量逐个拷贝。只有在未用户声明拷贝构造函数时，才会自动生成。若声明了移动操作，该函数会被定义为删除。若声明了析构函数或拷贝赋值运算符，自动生成该函数的行为会被弃用。
* 拷贝赋值运算符：行为与 C++98 一致，对非 `static` 成员变量逐个执行拷贝赋值。只有在未用户声明拷贝赋值运算符时，才会自动生成。若声明了移动操作，该函数会被定义为删除。若声明了析构函数或拷贝构造函数，自动生成该函数的行为会被弃用。
* 移动构造函数和移动赋值运算符：对非 `static` 成员变量逐个移动。只有没有用户声明的拷贝操作、移动操作和析构函数时，才会自动生成。

成员函数模板不会阻止特殊成员函数的生成。如果 `Widget` 类如下：
```cpp
class Widget
{
    template <typename T>            // construct Widget
    Widget(const T &rhs);            // from anything
    template <typename T>            // assign Widget
    Widget &operator=(const T &rhs); // from anything
};
```
如果满足条件，编译器仍会生成拷贝操作；即使 `T` 是 `Widget`，成员函数模板也不会抑制其生成。这是一种边缘情况，[Item 26](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/26_Avoid_overloading_on_universal_references.md) 会解释它的影响。

## Things to Remember
* The special member functions are those that compilers may generate on their own: default constructor, destructor, copy operations, and move operations.
* Move operations are generated only for classes lacking explicitly declared move operations, copy operations, and a destructor.
* The copy constructor is generated only for classes lacking an explicitly declared copy constructor, and it's deleted if a move operation is declared. The copy assignment operator is generated only for classes lacking an explicitly declared copy assignment operator, and it's deleted if a move operation is declared. Generation of the copy operations in classes with an explicitly declared destructor is deprecated.
* Member function templates never suppress generation of special member functions.
