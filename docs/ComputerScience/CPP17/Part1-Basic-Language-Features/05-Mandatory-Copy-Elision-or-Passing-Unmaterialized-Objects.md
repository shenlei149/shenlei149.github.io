本章主题可以从下面两个角度看：

* C++17 引入了一个新的规则：当按值传递或者返回参数时，必须省略对临时对象的拷贝
* 效果是使用未实质化的对象来初始化

## Motivation for Mandatory Copy Elision for Temporaries
C++ 一开始就允许在某些情况下省略拷贝，即使会影响程序的运行结果（比如拷贝构造函数中的输出语句没有执行）。用一个临时对象初始化一个新对象就会出现这种情况，特别是函数按值传递参数或者返回临时对象的场景。
```cpp
class MyClass
{
};

void foo(MyClass param)
{ // param is initialized by passed argument
}

MyClass bar()
{
    return MyClass{}; // returns temporary
}

int main()
{
    foo(MyClass{});    // pass temporary to initialize param
    MyClass x = bar(); // use returned temporary to initialize x
    foo(bar());        // use returned temporary to initialize param
}
```
不过这不是强制的，所以上面的例子中仍旧要求有隐式或者显式的拷贝和移动构造函数。尽管优化之后可能不需要调用这些函数，但是这些函数必须存在。下面删除了这些函数，会编译报错。
```cpp
class MyClass
{
public:
    // no copy/move constructor defined:
    MyClass(const MyClass &) = delete;
    MyClass(MyClass &&) = delete;
};
```

从 C++17 起，临时对象初始化对象时必须省略拷贝。那么之前的例子中 `MyClass` 不需要拷贝，所以删除拷贝和移动构造函数也能编译通过。

不过，其他一些可选优化的场景，还是需要这些构造函数。比如下面 `foo()` 函数中 `obj` 是一个有名的对象，使用的时候是左值。即使使用了具名返回值优化（`named return value optimization`），也还是需要这些构造函数。`obj` 作为参数也是一样的。传递一个临时对象（右值）到函数不再需要拷贝或移动，但是从函数返回，是具名的，所以需要拷贝或移动。
```cpp
MyClass foo()
{
    MyClass obj;

    return obj; // still requires copy/move support
}

MyClass bar(MyClass obj) // copy elision for passed temporaries
{
    return obj; // still requires copy/move support
}
```

## Benefit of Mandatory Copy Elision for Temporaries
这个特性的有点就是省略了拷贝，提升了性能。尽管编译器很早就做了类似的优化，但是现在有了标准的保证。尽管移动语义可以减少拷贝成本，但是不拷贝还是能带来性能提升，特别是很多基本字段，移动也还是需要逐个拷贝。另一个优势是可以减少一个出参，在 `return` 语句直接创建要返回的对象。

还有一个优势是可以实现一个工厂，能够返回没有拷贝和移动构造函数的类型。比如下面的工厂实现，可以用于没有拷贝和移动构造函数的 `std::atomic<>`。
```cpp
#include <utility>
template <typename T, typename... Args>
T create(Args &&...args)
{
    return T{std::forward<Args>(args)...};
}

#include "factory.hpp"
#include <memory>
#include <atomic>

int main()
{
    int i = create<int>(42);
    std::unique_ptr<int> up = create<std::unique_ptr<int>>(new int{42});
    std::atomic<int> ai = create<std::atomic<int>>(42);
}
```

还有一个影响是对于显式删除移动构造函数的类，现在也可以返回临时对象来初始化新的对象。
```cpp
class CopyOnly
{
public:
    CopyOnly() {}

    CopyOnly(int) {}

    CopyOnly(const CopyOnly &) = default;
    CopyOnly(CopyOnly &&) = delete; // explicitly deleted
};

CopyOnly ret()
{
    return CopyOnly{}; // OK since C++17
}

CopyOnly x = 42; // OK since C++17
```
在 C++17 之前，无法给 `x` 赋值。因为这里会使用拷贝初始化（使用 `=`），将 42 转换成临时对象，然后临时对象移动赋值给 `x`，原则上需要，但是如果优化了的话就不会调用。注意，只有移动构造函数不是用户声明的情况下，拷贝构造函数才会作为移动构造函数的，所以这里有拷贝构造函数但是不会被使用。
