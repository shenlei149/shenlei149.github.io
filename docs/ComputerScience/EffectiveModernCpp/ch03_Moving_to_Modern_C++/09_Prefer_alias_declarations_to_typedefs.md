在工作中会大量使用 STL。[Item 18](../ch04_Smart_Pointers/18_Use_std_unique_ptr_for_exclusive_ownership_resource_management.md) 会解释为什么使用 `std::unique_ptr` 是个好主意，但我们不想一遍又一遍地重复 `std::unique_ptr<std::unordered_map<std::string, std::string>>`。

为避免这种重复，可以使用 `typedef`：
```cpp
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
```

在 C++11 中，可以使用别名声明：
```cpp
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

两者完全等价。不过，下面将说明别名声明优于 `typedef`，因此应优先使用前者。首先需要指出的是，许多人认为在处理函数指针时，别名声明更易读。
```cpp
// FP is a synonym for a pointer to a function taking an int and
// a const std::string& and returning nothing.
typedef void (*FP)(int, const std::string &);  // typedef with the same meaning as above
using FP = void (*)(int, const std::string &); // alias declaration
```

当然，我们并不总是需要处理函数指针，因此这并不是一个充分的理由。

一个更有说服力的理由是模板。C++11 可以轻松定义别名模板；在此之前，必须将 `struct` 模板化才能达到同样的目的。假设需要为使用自定义分配器 `MyAlloc` 的链表定义别名，下面是两种实现方式：
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

后者的麻烦在于：如果需要在模板类中使用模板参数创建这样的链表，就不得不使用与 `typedef` 配套的 `typename`：
```cpp
template <typename T>
class Widget
{                                       // Widget<T> contains
private:                                // a MyAllocList<T>
    typename MyAllocList<T>::type list; // as a data member
};
```

这里的 `MyAllocList<T>::type` 是依赖于模板参数 `T` 的限定名称。编译器在模板定义阶段无法确定它是否表示类型，因此 C++ 规定必须以 `typename` 指明它是类型。

如果使用别名模板，情况就简单得多。
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

对我们来说，`MyAllocList<T>` 仿佛和 `MyAllocList<T>::type` 一样，都依赖于模板参数 `T`，不过编译器的处理方式不同。编译器知道 `MyAllocList` 是别名模板，因此 `MyAllocList<T>` 在语法上只能表示类型；此处既不需要，也不允许使用 `typename`。

但 `typedef` 并非如此。当编译器看到 `MyAllocList<T>::type` 时，无法确定它是否为类型：某个 `MyAllocList<T>` 特化中，`MyAllocList<T>::type` 可能表示的并不是类型。这种情况虽不常见，却是编译器必须考虑的可能性，因此必须写上 `typename`。

这种不常见的情况如下：
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

如果用 `Wine` 实例化模板类 `Widget`，`MyAllocList<T>::type` 表示的是成员变量而不是类型。`Widget` 中该限定名称的含义依赖于模板参数 `T`，因此编译器在解析模板时要求用 `typename` 将其标记为类型；当以 `Wine` 实例化时，则会因它实际并非类型而出错。

在模板元编程中，我们往往需要处理由类型 `T` 推导得到的类型，例如添加或移除 `const`、引用等修饰。`<type_traits>` 提供了许多工具来实现这些转换，例如：
```cpp
std::remove_const<T>::type                  // yields T from const T
std::remove_reference<T>::type              // yields T from T& and T&&
std::add_lvalue_reference<T>::type          // yields T& from T
```

上述注释只是非常简单的介绍；如果要在实际工作中使用，务必仔细阅读文档。

这里不是模板元编程教程；需要注意的是名称中的 `::type` 后缀。如果要在模板中使用它们，就不得不加上 `typename`。这是因为 C++11 标准库通过类模板中的 `typedef` 来实现这些类型萃取。虽然这里说明了别名声明更好，但标准库出于历史原因没有采用它们。C++14 为这些 C++11 类型萃取提供了更方便的别名模板，以 `_t` 后缀加以区分。
```cpp
std::remove_const<T>::type          // C++11: const T → T
std::remove_const_t<T>              // C++14 equivalent
std::remove_reference<T>::type      // C++11: T&/T&& → T
std::remove_reference_t<T>          // C++14 equivalent
std::add_lvalue_reference<T>::type  // C++11: T → T&
std::add_lvalue_reference_t<T>      // C++14 equivalent
```

使用 C++11 引入的技术，很容易实现这些 C++14 对应形式：
```cpp
template <class T>
using remove_const_t = typename remove_const<T>::type;
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```

## Things to Remember
* A `typedef` cannot declare a template alias, but an alias declaration can.
* Alias templates avoid the `::type` suffix and, in templates, the `typename` prefix often required to refer to `typedef`-defined types.
* C++14 provides alias templates for all C++11 type-trait transformations.
