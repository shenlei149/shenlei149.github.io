本章介绍三种查看类型推导结果的方法：编辑器、编译期和运行时。

## IDE Editors
只要编辑器具备编译状态信息，就可以将鼠标悬停在变量上，或让编辑器直接显示推导结果。对于简单类型，编辑器通常处理得很好；对于复杂类型，结果可能不准确，甚至无法显示。
```cpp
const int theAnswer = 42;
auto x = theAnswer;
auto y = &theAnswer;
```
编辑器很可能会推导出：`x` 的类型是 `int`，`y` 的类型是 `const int*`。

## Compiler Diagnostics
让编译器告知类型推导结果的一种方法是故意让代码编译失败，并通过错误消息显示类型。

例如，对于上面的例子，我们需要显示 `x` 和 `y` 的类型。可以声明一个只有声明、没有定义的类：
```cpp
template<typename T>    // declaration only for TD;
class TD;               // TD == "Type Displayer"
```

由于该模板类没有定义，实例化它会出错。为了查看 `x` 和 `y` 的类型，可以分别以它们的类型实例化 `TD` 类。
```cpp
TD<decltype(x)> xType;      // elicit errors containing
TD<decltype(y)> yType;      // x's and y's types
```

下面是两个编译器给出的错误信息。`TD` 模板类的具体实例化类型就是推导结果。
```cpp
error: aggregate 'TD<int> xType' has incomplete type and cannot be defined
error: aggregate 'TD<const int *> yType' has incomplete type and cannot be defined

error: 'xType' uses undefined class 'TD<int>'
error: 'yType' uses undefined class 'TD<const int *>'
```

## Runtime Output
用 `printf` 输出通常简单高效，但难点在于选择合适的输出格式。先来看看 `typeid` 和 `std::type_info::name` 的效果，仍然使用上面的例子。
```cpp
std::cout << typeid(x).name() << '\n';      // display types for
std::cout << typeid(y).name() << '\n';      // x and 
```

`std::type_info::name` 返回的字符串是实现定义的，因此不保证易读。GNU 和 Clang 的返回结果有点像加密文本：`x` 对应的类型是 `i`，`y` 对应的类型是 `PKi`。`i` 表示 `int`，`PK` 表示 `pointer to const`。这两个编译器都支持 `c++filt`，可将其显示为更友好的名称。MSVC 的输出是 `int` 和 `int const *`，友好得多。

下面考虑一个更复杂的示例。
```cpp
template<typename T>                // template function to
void f(const T& param);             // be called
std::vector<Widget> createVec();    // factory function
const auto vw = createVec();        // init vw w/factory return

if (!vw.empty()) {
    f(&vw[0]);      // call f
}
```

这个示例涉及自定义类型 `Widget`、STL 容器 `std::vector` 和 `auto` 变量 `vw`，更符合实际场景。

使用 `typeid` 显示类型推导结果。
```cpp
template<typename T>
void f(const T& param)
{
    using std::cout;
    cout << "T = " << typeid(T).name() << '\n';             // show T
    cout << "param = " << typeid(param).name() << '\n';     // show  param's type
```

GNU 和 Clang 的输出如下：
```
T = PK6Widget
param = PK6Widget
```

`PK` 后面的数字 `6` 是类名 `Widget` 的长度。因此，`typeid` 显示 `T` 和 `param` 的类型都是 `const Widget*`。

MSVC 的输出是：
```
T = class Widget const *
param = class Widget const *
```

三个编译器给出了相同的结果，看起来似乎很准确。但仔细思考，`T` 和 `param` 的类型会相同吗？考虑一个简单的例子：若 `T` 的类型是 `int`，则 `param` 的类型是 `const int&`，两者并不相同。

事实上，`std::type_info::name` 的结果并不可靠；在这个示例中，它显示的 `param` 类型就是错误的。对表达式应用 `typeid` 时，所得的 `std::type_info` 不保留引用和顶层 `const` 属性，因此不能指望该函数始终显示完整的类型信息。正如 [Item 1](./01_Understand_template_type_deduction.md) 所解释的，引用和顶层 `const` 会被忽略。这就是为什么 `param` 的实际类型是 `const Widget * const &`，而输出却是 `const Widget*`：先忽略引用，再忽略顶层 `const`。

这个示例说明 `std::type_info::name` 并不可靠。随后作者又给出了两个示例，说明编辑器的结果也未必可靠；实际使用 clangd 时，特别复杂的类型推导结果同样往往不可靠。

Boost TypeIndex 库能够正确地完成这项工作。继续使用之前的示例：
```cpp
#include <boost/type_index.hpp>
template<typename T>
void f(const T& param)
{
    using std::cout;
    using boost::typeindex::type_id_with_cvr;

    // show T
    cout << "T = "
         << type_id_with_cvr<T>().pretty_name()
         << '\n';

    // show param's type
    cout << "param = "
         << type_id_with_cvr<decltype(param)>().pretty_name()
         << '\n';
}
```

模板函数 `boost::typeindex::type_id_with_cvr` 的模板实参就是要查看类型信息的类型。后缀 `cvr` 表示 `const`、`volatile` 和 `reference`，也就是说这些信息不会被移除。`pretty_name` 成员函数返回包含人类可读类型信息的 `std::string`。

下面测试上述函数 `f`：
```cpp
std::vector<Widget> createVec();    // factory function
const auto vw = createVec();        // init vw w/factory return
if (!vw.empty()) {
    f(&vw[0]);          // call f
}
```

在 GNU 和 Clang 编译器上，Boost 库的输出如下：
```
T = Widget const*
param = Widget const* const&
```

在 MSVC 编译器上，Boost 库的输出如下：
```
T = class Widget const *
param = class Widget const * const &
```

编辑器、编译器错误信息以及 Boost TypeIndex 这样的类库都是查看类型信息的工具，能够提供帮助，但无法替代对本章前面三个 Item 所阐述的类型推导机制的理解。

## Things to Remember
* Deduced types can often be seen using IDE editors, compiler error messages, and the Boost TypeIndex library.
* The results of some tools may be neither helpful nor accurate, so an understanding of C++'s type deduction rules remains essential.
