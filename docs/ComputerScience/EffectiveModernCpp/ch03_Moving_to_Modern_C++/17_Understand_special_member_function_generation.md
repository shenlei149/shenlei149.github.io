C++ 特殊成员函数是指可以自动生成的成员函数。C++98 中有四个：默认构造函数，析构函数，拷贝构造函数和拷贝赋值运算符。只有需要时，这些函数才会被自动生成。一个类没有任何构造函数，才会生成默认构造函数。自动生成的特殊成员函数是 `public` 且是 `inline` 的。继承自有虚析构函数的基类，自动生成的析构函数是虚函数，初次智蛙，自动生成的函数默认都是非虚函数。

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

移动操作相关函数的生成规则和拷贝操作类似，只在必要时生成，如果生成了，那么行为是对所有非 `static` 成员变量进行逐字段的移动。移动构造函数对参数 `rhs` 中每一个非 `static` 成员进行移动构造，移动赋值运算符类似。如果存在基类部分，会对基类部分做同样的处理。

假定现在我更倾向于使用移动操作，比如移动构造或者移动赋值运算法，但是移动操作本身并不一定发生了。成员变量的移动操作更像是一个移动操作的请求，如果类型是不可移动的，比如 C++98 的对象，那么会使用拷贝替代移动。移动成员变量的核心是使用 `std::move`，运行时是会决定使用移动操作还是拷贝操作。详见 Item 23 TODO add link。能移动才会移动，不支持的话使用拷贝。

与拷贝类似，一旦我们自己定义了移动操作，那么编译器则不会生成。不过细节略有不同。

两个拷贝函数是独立的。如果只声明其中一个，不会影响编译器自动生成另外一个。比如声明了拷贝构造函数，但是又用到了拷贝赋值运算符，那么编译器会自动生成后者。反之亦然。

两个移动函数不是独立的。如果声明了其中一个，那么编译器不会自动生成另外一个。如果我们声明了其中一个，比如移动构造函数，这说明默认的行为和我们实现有不同，那么编译器就推断说无法生成一个合适的移动赋值运算符。即声明移动构造函数会抑制编译器自动合成移动赋值运算符。反之亦然。

类似的，如果一个类声明了拷贝函数，那么也不会生成移动函数。因为声明拷贝函数，那么意味着自动生成的行为——逐字段复制——不适合当前类，那么编译器会推断说既然逐字段复制不适合这个类，那么逐字段移动也可能不适合这个类，因此不会再自动生成了。

反方向也成立。声明了移动函数，那么编译器会使用 `delete` 禁止生成拷贝函数。理由也是类似的。这个新增的限制并不会破坏 C++98 的代码，因为旧代码没有移动函数。不过当你处理这些遗留代码，新增了移动函数以提升性能，那么自动生成特殊成员函数的规则就变了。

三原则（`Rule of Three`）是说一旦你声明了析构函数、拷贝构造函数和拷贝赋值运算符中的一个，那么应该声明这三个函数。这条原则是长期观察得到的。通常需要管理的资源就是内存，这也是为什么 STL 容器类都声明了析构函数、拷贝构造函数和拷贝赋值运算符。

三原则的一个重要结果就是如果用户自定义了析构函数，那么简单的逐个拷贝成员变量的方式可能不再试用了。在 C++98 时代，这个原则没有受到应有的重视，当有了析构函数，编译器还会是自动生成拷贝操作的函数。C++11 为了兼容，仍保持不变。

三原则背后的原因还是有效的，再加上之前关于移动操作的观察，在 C++11 引入移动操作时，为新的操作新增了限制。

因此，只有在下面三个条件都满足且必要时，才会生成移动操作。
* 没有声明拷贝操作
* 没有声明移动操作
* 没有生命析构函数

有时，类似上面规则可以扩展到拷贝操作，因为 C++11 中，如果声明了析构函数或拷贝操作，那么废弃自动生成拷贝操作的函数。如果现有的类有析构函数或者声明了拷贝操作，但又依赖于自动生成拷贝操作，那么需要升级代码。如果自动生成的符合预期，那么只需要添加上 `= default` 即可。
```cpp
class Widget
{
public:
    ~Widget();                                   // user-declared dtor
    Widget(const Widget &) = default;            // default copy ctor behavior is OK
    Widget &operator=(const Widget &) = default; // default copy assign behavior is OK
};
```

这种方式通常在多态场景中很有用。基类通常有一个虚析构函数，否则只有基类指针调用某些操作（比如 `delete`）会出错。所以我们需要声明一个虚析构函数但是我们一旦声明了析构函数，那么就不会自动生成移动操作了。如果默认移动操作是正确的，那么应该声明移动操作并用 `= default` 实现。C++11 中声明了移动操作，会抑制拷贝操作的生成。如果默认拷贝操作也是正确的，那么同样用 `= default` 实现即可。
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
在满足编译器自动生成移动操作、拷贝操作，且默认行为符合要求的情况下，也应该手动声明这些函数并使用 `= default` 实现。因为这能更好的表明你的意图，更重要的是，能避免一些潜在的 bug。比如，有一个标识字符串的类，可以通过整数快速查找需要的字符串。
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
这个类声明析构函数、拷贝操作、移动操作，编译器自动生成了它们。

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
这看起来很合理，但是析构函数的添加导致无法自动生成移动操作了。拷贝操作不受影响。这时，所有的移动操作指定的都是拷贝操作。而拷贝 `std::map<int, std::string>` 的速度要比移动 `std::map<int, std::string>` 慢几个数量级。增加一个析构函数，导致了性能问题，如果声明了拷贝操作和移动操作，并且使用 `= default` 实现，就能避免这个问题。

对于默认构造函数和析构函数，C++11 中的规则和 C++98 中的规则基本一致。

C++11 中特殊成员函数的处理规则如下：
* 默认构造函数：与 C++98 一致。没有用户自定义的构造函数时才会生成默认构造函数。
* 析构函数：与 C++98 基本一致，唯一的区别是默认是 `noexcept`。与 C++98 一致，只有当基类的析构是虚函数是，这个类的析构函数才是虚函数。
* 拷贝构造函数：运行时行为与 C++98 一致，对非 `static` 成员变量逐个拷贝。用户没有定义拷贝构造时，才会自动生成。如果声明了移动操作，该函数是 `delete` 的。当声明了析构函数或拷贝赋值运算符，自动生成的函数被废弃。
* 拷贝赋值运算符：运行时行为与 C++98 一致，对非 `static` 成员变量逐个拷贝赋值。用户没有定义拷贝赋值运算符时，才会自动生成。如果声明了移动操作，该函数是 `delete` 的。当声明了析构函数或拷贝构造函数，自动生成的函数被废弃。
* 移动构造函数和移动赋值运算符：对非 `static` 成员变量逐个移动。当没有用户声明的拷贝操作、移动操作和析构函数时，才会自动生成。

这里没有成员模版函数阻止特殊成员函数的生成。如果 `Widget` 类如下
```cpp
class Widget
{
    template <typename T>            // construct Widget
    Widget(const T &rhs);            // from anything
    template <typename T>            // assign Widget
    Widget &operator=(const T &rhs); // from anything
};
```
如果满足条件，编译器还是会生成拷贝操作，即使当 `T` 是 `Widget` 时，模板函数签名与拷贝操作一致。这是一种边缘情况，Item 26 TODO link 会解释它的影响。

## Things to Remember
* The special member functions are those compilers may generate on their own: default constructor, destructor, copy operations, and move operations.
* Move operations are generated only for classes lacking explicitly declared move operations, copy operations, and a destructor.
* The copy constructor is generated only for classes lacking an explicitly declared copy constructor, and it’s deleted if a move operation is declared. The copy assignment operator is generated only for classes lacking an explicitly declared copy assignment operator, and it’s deleted if a move operation is declared. Generation of the copy operations in classes with an explicitly declared destructor is deprecated.
* Member function templates never suppress generation of special member functions.
