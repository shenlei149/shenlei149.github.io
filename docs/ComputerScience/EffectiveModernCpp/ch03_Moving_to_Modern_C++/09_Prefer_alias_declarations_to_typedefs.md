工作中会大量使用 STL，[Item 18](../ch04_Smart_Pointers/18_Use_std_unique_ptr_for_exclusive_ownership_resource_management.md) 会解释使用 `std::unique_ptr` 是一个好主意，但是我们不会想一遍遍的重复 `td::unique_ptr<std::unordered_map<std::string, std::string>>`。

为了避免这个事情，我们使用 `typedef`
```cpp
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
```

在 C++11 中，可以声明别名
```cpp
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

两者完全是等价的。不过下面要阐述别名比 `typedef` 要好，更倾向于使用前者。在开始之前需要提一下，很多人都觉得在处理函数指针时使用别名更容易理解。
```cpp
// FP is a synonym for a pointer to a function taking an int and
// a const std::string& and returning nothing
typedef void (*FP)(int, const std::string &);  // typedef same meaning as above
using FP = void (*)(int, const std::string &); // alias declaration
```

当然，我们并不总是需要和函数指针打交道，所以这并不是合适的理由。

一个令人信服的理由是：模板。对于模板，C++11 可以容易的定义模板的别名，而之前不得不模板化 `struct` 来达到同样的目的。我们需要定义类型——使用自定义分配器 `MyAlloc` 的链表——的别名。下面是两种实现的代码。
```cpp
template <typename T>                         // MyAllocList<T> is synonym for
using MyAllocList = std::list<T, MyAlloc<T>>; // std::list<T, MyAlloc<T>>
MyAllocList<Widget> lw;                       // client code

template <typename T>
struct MyAllocList                         // MyAllocList<T>::type
{                                          // is synonym for
    typedef std::list<T, MyAlloc<T>> type; // std::list<T, MyAlloc<T>>
};
MyAllocList<Widget>::type lw;              // client code
```

后者的麻烦在于如果需要在一个模板类里面使用模板参数来创建这么一个链表，那么不得不使用与 `typedef` 配套的 `typename`：
```cpp
template <typename T>
class Widget
{                                       // Widget<T> contains
private:                                // a MyAllocList<T>
    typename MyAllocList<T>::type list; // as a data member
};
```

这里 `MyAllocList<T>::type` 是一个依赖于模板类型 `T` 的类型，因此它是一个依赖类型（`dependent type`），C++ 规则之一就是依赖类型必须以 `typename` 开头。

如果使用模板别名，那么就简单许多。
```cpp
template <typename T>
using MyAllocList = std::list<T, MyAlloc<T>>; // as before
template <typename T>
class Widget
{
private:
    MyAllocList<T> list;        // no "typename", no "::type"
};
```

对我们来说，`MyAllocList<T>` 仿佛和 `MyAllocList<T>::type` 一样，也依赖月模板参数 `T`，不过实际上编译器不是这么处理的。当遇到 `MyAllocList` 时知道这是一个模板别名，那么 `MyAllocList<T>` 只能是一个类型，不是依赖类型，也就不需要 `typename`，当然也不允许 `typename` 出现。

但是 `typedef` 不是这样的。当编译器看到 `MyAllocList<T>::type` 的时候，它不能确定其就是一个类型，说不定某个 `MyAllocList<T>` 的特化实现中，`MyAllocList<T>::type` 表示其他东西而不是一个类型。这不常见，不过对编译器而言，是有可能发生的事情，所以不得不写上 `typename`。

这个不常见的可能性如下：
```cpp
class Wine
{
};
template <> // MyAllocList specialization for when T is Wine
class MyAllocList<Wine>
{
private:
    enum class WineType // see Item 10 for info on "enum class"
    {
        White,
        Red,
        Rose
    };
    WineType type; // in this class, type is a data member!
};
```

如果用 `Wine` 实例化模板类 `Widget`，那么 `MyAllocList<T>::type` 是一个成员变量而不是类型。在 `Widget` 中，`MyAllocList<T>::type` 表示什么是依赖于模板参数 `T` 的，所以称为依赖类型，编译器就要求必须写 `typename`。

在模板元编程中，我们往往需要处理类型 `T` 编程需要的类型，比如加上或者去掉 `const` 或引用修饰。`<type_traits>` 提供了很多工具以实现这些转化。比如
```cpp
std::remove_const<T>::type                  // yields T from const T
std::remove_reference<T>::type              // yields T from T& and T&&
std::add_lvalue_reference<T>::type          // yields T& from T
```

上述注释只是非常简单的介绍，如果要在工作中使用，务必仔细阅读文档。

这里不是要做模板元编程的教程，而是要注意后缀 `::type`。如果要在模板类里面使用它们，不得不加上 `typename`。这是因为 C++11 类库使用 `typedef` 加 `struct` 实现的它们。是的，这里要说别名更好，但是标准库没这么做。这是因为某些历史遗留问题。C++ 委员会的人在 C++14 才意识到模板别名有更好的方式来实现，所以 C++14 给出了模板别名的实现，后缀 `_t` 以示区别。
```cpp
std::remove_const<T>::type          // C++11: const T → T
std::remove_const_t<T>              // C++14 equivalent
std::remove_reference<T>::type      // C++11: T&/T&& → T
std::remove_reference_t<T>          // C++14 equivalent
std::add_lvalue_reference<T>::type  // C++11: T → T&
std::add_lvalue_reference_t<T>      // C++14 equivalent
```

使用 C++11 引入的技术，很容易实现 C++14 这些功能
```cpp
template <class T>
using remove_const_t = typename remove_const<T>::type;
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```

## Things to Remember
* `typedef` don't support templatization, but alias declarations do.
* Alias templates avoid the `::type` suffix and, in templates, the `typename` prefix often required to refer to typedefs.
* C++14 offers alias templates for all the C++11 type traits transformations.
