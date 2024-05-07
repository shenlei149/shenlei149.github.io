本章会介绍三种方法获取类型推导的结果：编辑器、编译期和运行时。

## IDE Editors
主要是编辑器有编译状态信息，这样的话鼠标放到变量上或者编辑器会直接显示推导结果。如果是简单类型，往往能处理很好；如果是复杂类型，可能会不太准或者无法推导。
```cpp
const int theAnswer = 42;
auto x = theAnswer;
auto y = &theAnswer;
```
编辑器大概率会推导出结果：`x` 类型是 `int`，`y` 类型是 `const int*`。

## Compiler Diagnostics
让编译器告诉我们关于类型推导的结果，方法是让代码编译出错，通过错误消息来显示类型。

比如上面的例子，我们需要显式 `x, y` 的类型。我们声明一个没有定义过的类，比如
```cpp
template<typename T>    // declaration only for TD;
class TD;               // TD == "Type Displayer"
```

由于模板类没有定义，实例化会出错。为了查看 `x, y` 的类型，我们以它们的类型来实例化 `TD` 类。
```cpp
TD<decltype(x)> xType;      // elicit errors containing
TD<decltype(y)> yType;      // x's and y's types
```

作者给出了两个编译器的出错信息。`TD` 模板类的具体类型就是推导结果。
```cpp
error: aggregate 'TD<int> xType' has incomplete type and cannot be defined
error: aggregate 'TD<const int *> yType' has incomplete type and cannot be defined

error: 'xType' uses undefined class 'TD<int>'
error: 'yType' uses undefined class 'TD<const int *>'
```

## Runtime Output
`printf` 打印大法往往简单高效。这里的一个挑战是输出合适的格式。我们先看下 `typeid` `std::type_info::name` 的效果。仍旧使用上面的例子。
```cpp
std::cout << typeid(x).name() << '\n';      // display types for
std::cout << typeid(y).name() << '\n';      // x and 
```

`std::type_info::name` 不保证返回的内容一定非常合理，但其实现会尽可能使其有效。GNU 和 Clang 返回结果有点像加密内容，`x` 对应类型是 `i`，`y` 对应类型是 `PKi`。`i` 表示 `int`，`PK` 表示 `pointer to const`。这两个编译器都支持 c++filt，以显式更友好的名字。MSVC 的输出是 `int` 和 `int const *`，友好很多。

下面考虑一个更复杂的例子。
```cpp
template<typename T>                // template function to
void f(const T& param);             // be called
std::vector<Widget> createVec();    // factory function
const auto vw = createVec();        // init vw w/factory return

if (!vw.empty()) {
    f(&vw[0]);      // call f
}
```

这个例子涉及自定义类型 `Widget`，STL 容器 `std::vector` 和 `auto` 变量 `vw`。这更符合实际的场景。

使用 `typedid` 来显式类型推导结果。
```cpp
template<typename T>
void f(const T& param)
{
    using std::cout;
    cout << "T = " << typeid(T).name() << '\n';             // show T
    cout << "param = " << typeid(param).name() << '\n';     // show  param's type
```

GNU 和 Clang 的输出如下
```
T = PK6Widget
param = PK6Widget
```

PK 后面的数字 6 是类的长度。所以编译器认为 `T` 和 `param` 的类型都是 `const Widget*`。

MSVC 的输出是
```
T = class Widget const *
param = class Widget const *
```

三个编译器给出了相同的结果，那么应该是准确的。但是仔细思考，`T` 和 `param` 的类型会一样吗？考察一个简单的例子，`T` 的类型是 `int`，那么 `param` 的类型是 `const int&`，两者不同。

事实上，`std::type_info::name` 的结果并不可行，具体到这个例子，`param` 的类型是不对的。`std::type_info::name` 的规范要求它们将类型看做是按值传递的情况，所以不应该要求这些函数一定能返回正确结果。正如 [Item 1](/EffectiveModernCpp/ch01_Deducing_Types/01_Understand_template_type_deduction.md) 中的解释，引用和 `const` 这两个属性会被忽略。这就是为什么 `param` 的真实类型是 `const Widget * const &`，但是输出是 `const Widget*`。首先引用被移除了，接着 `const` 也被移除了。

这个例子告诉我们 `std::type_info::name` 不可靠。后续给出了两个例子，说明编辑器的结果也不完全可靠，实际使用 clangd 的经验也是如此，特别复杂的类型推导不太靠谱。

Boost TypeIndex library 能够正确的做这个事情。继续之前的例子。
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

模板函数 `boost::typeindex::type_id_with_cvr` 需要的模板类型就是想要查看信息的类型，后缀 `cvr` 的意思是 `const, volatile, reference`，也就是说不会移除这些信息。`pretty_name` 成员函数返回一个 `std::string`，包含人类友好的类型信息。

下面测试一下上面实现的函数 `f`。
```cpp
std::vector<Widget> createVec();    // factory function
const auto vw = createVec();        // init vw w/factory return
if (!vw.empty()) {
    f(&vw[0]);          // call f
}
```

GNU 和 Clang 编译程序，Boost 库输出如下：
```
T = Widget const*
param = Widget const* const&
```

MSVC 编译程序，Boost 库输出如下：
```
T = class Widget const *
param = class Widget const * const &
```

编辑器、编译时报错或者诸如 Boost TypeIndex 这样的类库，都是查看信息的工具，能给我们一些帮助，但是替代不了本章前面三个 Item 所阐述的对类型推导的理解。

## Things to Remember
* Deduced types can often be seen using IDE editors, compiler error messages, and the Boost TypeIndex library.
* The results of some tools may be neither helpful nor accurate, so an understanding of C++'s type deduction rules remains essential.

