出于移植和集成容易的目的，头文件提供完整的定义是很重要的。但是在 C++17 之前，只有没有全局变量的时候才可以这样做。

从 C++17 开始，可以通过 `inline` 在头文件定义全局变量。要求是在一个编译单元内没有重复定义。如果有多个编译单元使用，它们会指向同一个对象。
```cpp
class MyClass
{
    inline static std::string msg{"OK"}; // OK since C++17
    ...
};
inline MyClass myGlobalObj; // OK even if included/defined by multiple CPP files
```

## Motivation for Inline Variables
C++ 中不允许初始化非常量的静态成员。
```cpp
class MyClass
{
    static std::string msg{"OK"}; // compile-time ERROR
};
```

可以定义在类的外面，但是如果被多个 CPP 文件包含，会出现链接错误。根据定义一次原则（`one definition rule`），除非是 `inline` 对象，否则只能定义一次。
```cpp
class MyClass
{
    static std::string msg;
    ...
};
std::string MyClass::msg{"OK"}; // Link ERROR if included by multiple CPP files
```

使用宏保护也无法解决这个问题。
```cpp
#ifndef MYHEADER_HPP
#define MYHEADER_HPP
class MyClass
{
    static std::string msg;
};
std::string MyClass::msg{"OK"}; // Link ERROR if included by multiple CPP files
#endif
```

问题的本质不是头文件被包含多次，而不不用的两个引用该头文件的 CPP 文件里面重复定义了 `MyClass::msg`。

和之前类似，定义一个类的对象，也会遇到链接错误。
```cpp
class MyClass
{
};
MyClass myGlobalObject; // Link ERROR if included by multiple CPP files
```

#### Workarounds
针对不同的场景，有一些方法可以绕开这个限制。

比以在类内初始化数字或者枚举类型的常量静态成员。不过这种方法限制很多，只能用字面类型初始化，比如基本的整数类型、浮点数、指针类型，或者是常量初始化了非静态成员的类且这个类不能有自定义的或虚析构函数。如果想获取地址，比如引用，那么必须定义一次，且多个编译单元不能重复定义。
```cpp
class MyClass
{
    static const bool trace = false; // OK, literal type
};
```

可以定义一个返回该值的 `static` 成员函数。
```cpp
class MyClass
{
    static std::string &getMsg()
    {
        static std::string msg{"OK"};
        return msg;
    }
};
```

从 C++14 开始，可以使用可变模板
```cpp
template <typename T = std::string>
T myGlobalMsg{"OK"};
```

还可以为这个静态成员定义一个模板类，然后继承这个类。
```cpp
template <typename = void>
class MyClassStatics
{
    static std::string msg;
};

template <typename T>
std::string MyClassStatics<T>::msg{"OK"};

class MyClass : public MyClassStatics<>
{
}
```
这些方法会导致签名重载，可读性变差，使用这个全局变量的方式也不同。而且有的方法会延迟初始化，使得我们无法在程序启动的时候就初始化好。

## Using Inline Variables
现在的通过 `inline` 就能在头文件定义一个全局变量，可以被多个 CPP 文件使用。
```cpp
class MyClass
{
    inline static std::string msg{"OK"}; // OK since C++17
};
inline MyClass myGlobalObj; // OK even if included/defined by multiple CPP files
```

`inline` 语义与函数定义时相同。

* 可以在多个编译单元定义，前提是所有定义都是相同的。
* 必须在每个使用它的编译单元都定义。

在头文件中定义，由多个 CPP 文件包含，就满足上述两点，程序的行为就好像只有一个变量一样。

原子定义也是可以的。
```cpp
inline std::atomic<bool> ready{false};
```

这里需要保证初始化 `inline` 变量的类型是完整的，如果一个类包含一个自身类型的成员，那么必须完整的声明之后再定义。
```cpp
struct MyType
{
    int value;
    MyType(int i) : value{i}
    {
    }
    // one static object to hold the maximum value of this type:
    static MyType max; // can only be declared here
};

inline MyType MyType::max{0};
```

## `constexpr` Now Implies `inline` For Static Members
对于 `static` 成员，`constexpr` 修饰符隐含了 `inline`。所以在 C++17 中，下面的代码定义了 `n`。
```cpp
struct D
{
    static constexpr int n = 5; // C++11/C++14: declaration
                                // since C++17: definition
};
```
等价于
```cpp
struct D
{
    inline static constexpr int n = 5;
};
```
在 C++17 之前，可以只声明，不定义。比如按值传递就是合法的。但是如果以引用传递到一个没有被优化掉且没有 `inline` 的函数，会出错。
```cpp
struct D
{
    static constexpr int n = 5;
};

std::cout << D::n; // OK (ostream::operator<<(int) gets D::n by value)

int twice(const int &i);
std::cout << twice(D::n); // was often an ERROR
```
上面的代码违背了定义一次原则。如果编译器有优化，那么可能出错，可能没问题。但是不优化，会由于缺少 `D::n` 的定义而报错。如果创建一个指向它的指针，大概率有问题
```cpp
const int *p = &D::n; // was usually an ERROR
```
正确的做法是补充一个定义。
```cpp
constexpr int D::n; // C++11/C++14: definition
                    // since C++17: redundant declaration (deprecated)
```
自 C++17 开始，已经无需这么做了。

## Inline Variables and `thread_local`
通过 `thread_local` 可以为每个线程创建一个 `inline` 变量。
```cpp
struct ThreadData
{
    inline static thread_local std::string name; // unique name per thread
};
inline thread_local std::vector<std::string> cache; // one cache per thread
```
下面是一个完整的例子。头文件如下
```cpp
#include <string>
#include <iostream>
struct MyData
{
    inline static std::string gName = "global";           // unique in program
    inline static thread_local std::string tName = "tls"; // unique per thread
    std::string lName = "local";                          // for each object

    void print(const std::string &msg) const
    {
        std::cout << msg << '\n';
        std::cout << "- gName: " << gName << '\n';
        std::cout << "- tName: " << tName << '\n';
        std::cout << "- lName: " << lName << '\n';
    }
};
inline thread_local MyData myThreadData; // one object per thread
```
我们可以在 `main` 函数所在 CPP 文件使用这个头文件。
```cpp
#include "inlinethreadlocal.hpp"
#include <thread>
void foo();
int main()
{
    myThreadData.print("main() begin:");
    myThreadData.gName = "thread1 name";
    myThreadData.tName = "thread1 name";
    myThreadData.lName = "thread1 name";
    myThreadData.print("main() later:");

    std::thread t(foo);
    t.join();
    myThreadData.print("main() end:");
}
```
也可以在定义了 `foo()` 的 CPP 文件使用这个头文件。
```cpp
#include "inlinethreadlocal.hpp"

void foo()
{
    myThreadData.print("foo() begin:");
    myThreadData.gName = "thread2 name";
    myThreadData.tName = "thread2 name";
    myThreadData.lName = "thread2 name";
    myThreadData.print("foo() end:");
}
```
程序的输出如下
```
main() begin:
- gName: global
- tName: tls
- lName: local
main() later:
- gName: thread1 name
- tName: thread1 name
- lName: thread1 name
foo() begin:
- gName: thread1 name
- tName: tls
- lName: local
foo() end:
- gName: thread2 name
- tName: thread2 name
- lName: thread2 name
main() end:
- gName: thread2 name
- tName: thread1 name
- lName: thread1 name
```
