C++ 有很多方法初始化一个对象，其中一个叫做聚合初始化（`aggregate initialization`），初始化聚合体。这是由 C 引入的用大括号来初始化一组值。
```cpp
struct Data
{
    std::string name;
    double value;
};

Data x = {"test1", 6.778};
```
从 C++11 开始不再需要等号了。
```cpp
Data x{"test1", 6.778};
```
从 C++17 开始，聚合体可以有基类，甚至可以省略基类的括号。初始化哪个成员依赖于传递参数的顺序。
```cpp
struct MoreData : Data
{
    bool done;
};

MoreData y{{"test1", 6.778}, false};

MoreData y{"test1", 6.778, false};
```

## Motivation for Extended Aggregate Initialization
如果没有这个特性，那么对派生类就无法聚合体初始化，必须像下面这样写一个构造函数。
```cpp
struct Cpp14Data : Data
{
    bool done;
    Cpp14Data(const std::string &s, double d, bool b)
        : Data{s, d}, done{b}
    {
    }
};

Cpp14Data y{"test1", 6.778, false};
```
而现在就无需再写构造函数了。如果内层的所需的参数都传递的话，可以省略括号。
```cpp
MoreData x{{"test1", 6.778}, false}; // OK since C++17
MoreData y{"test1", 6.778, false};   // OK
```
现在派生类也是一个聚合体了，所以也可以用下面的初始化方法。
```cpp
MoreData u;   // OOPS: value/done are uninitialized
MoreData z{}; // OK: value/done have values 0/false
```

## Using Extended Aggregate Initialization
聚合初始化一个典型场景就是初始化继承 C 结构体又添加了新的数据成员的类的实例。
```cpp
struct Data
{
    const char *name;
    double value;
};

struct CppData : Data
{
    bool critical;
    void print() const
    {
        std::cout << '[' << name << ',' << value << "]\n";
    }
};

CppData y{{"test1", 6.778}, false};
y.print();
```
可以跳过一些成员不赋值，这些成员会使用默认值。注意，写大括号不传参 `{}` 和什么也不写（不初始化）差别很大。
```cpp
CppData x1{};         // zero-initialize all elements
CppData x2{{"msg"}};  // same as {{"msg",0.0},false}
CppData x3{{}, true}; // same as {{nullptr,0.0},true}
CppData x4;           // values of fundamental types are unspecified
```
也可以从非聚合体继承得到聚合体，下面的例子并不是多态的 `public` 继承，因为 `std::string` 没有虚函数。
```cpp
struct MyString : std::string
{
    void print() const
    {
        if (empty())
        {
            std::cout << "<undefined>\n";
        }
        else
        {
            std::cout << c_str() << '\n';
        }
    }
};

MyString x{{"hello"}};
MyString y{"world"};
```
也可以多继承。嵌套的初始化列表按照基类声明顺序依次赋值。
```cpp
template <typename T>
struct D : std::string, std::complex<T>
{
    std::string data;
};

D<float> s{{"hello"}, {4.5, 6.7}, "world"};       // OK since C++17
D<float> t{"hello", {4.5, 6.7}, "world"};         // OK since C++17
std::cout << s.data;                              // outputs: "world"
std::cout << static_cast<std::string>(s);         // outputs: "hello"
std::cout << static_cast<std::complex<float>>(s); // outputs: (4.5,6.7)
```

## Definition of Aggregates
C++17 中聚合体的定义是

* 一个数组或
* 一个类类型
    * 没有用户定义或 `explicit` 构造函数
    * 没有通过 `using` 声明继承的构造函数
    * 没有 `private` `protected` 的非静态数据
    * 没有 `virtual` 函数
    * 没有 `virtual` `private` `protected` 的基类

如果想初始化一个聚合体，还需要满足下面两个约束条件。

* 基类没有 `private` `protected` 的成员
* 没有 `private` `protected` 构造函数

C++17 引入新的 type traits `is_aggregate<>` 测试类型是否是聚合体。
```cpp
template <typename T>
struct D : std::string, std::complex<T>
{
    std::string data;
};

D<float> s{{"hello"}, {4.5, 6.7}, "world"};         // OK since C++17
std::cout << std::is_aggregate<decltype(s)>::value; // outputs: 1 (true)
```

## Backward Incompatibilities
下面是一个无法在新标准下编译的代码。
```cpp
struct Derived;
struct Base
{
    friend struct Derived;

private:
    Base()
    {
    }
};

struct Derived : Base
{
};

int main()
{
    Derived d1{}; // ERROR since C++17
    Derived d2;   // still OK (but might not initialize)
}
```
在 C++17 之前，`Derived` 不是聚合体，那么 `Derived d1{}` 会调用默认构造函数，其调用基类的私有构造函数，不过 `Derived` 是 `Base` 的友元，所以可以访问私有构造函数，一切正常。

但是到了 C++17，`Derived` 满足上面所述的条件，是一个聚合体
```cpp
std::is_aggregate<Derived>::value
```
会返回 `true`。但是基类有 `private` 构造函数，违背了上面描述的初始化条件，所以无法编译。注意，这里与是否友元无关。
