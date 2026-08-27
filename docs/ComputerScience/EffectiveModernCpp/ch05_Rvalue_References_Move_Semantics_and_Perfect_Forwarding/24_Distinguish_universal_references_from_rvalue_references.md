要声明 `T` 的右值引用，写作 `T&&`。但并不是所有的 `T&&` 都表示右值引用。
```cpp
void f(Widget &&param); // rvalue reference

Widget &&var1 = Widget(); // rvalue reference

auto &&var2 = var1; // not rvalue reference

template <typename T>
void f(std::vector<T> &&param); // rvalue reference

template <typename T>
void f(T &&param); // not rvalue reference
```

`T&&` 有两种语义。一个是右值引用，绑定到右值，表示对象可以被移动。

另一个语义是通用引用：它既可能是左值引用，也可能是右值引用。虽然看起来像是右值引用，但实际上可能会折叠成左值引用。因此，既可以绑定右值，也可以绑定左值。进一步，可以绑定 `const` 或者非 `const` 对象，也可以绑定 `volatile` 或者非 `volatile` 对象。甚至可以绑定到 `const volatile` 对象上。几乎可以绑定到任意对象上。因此，称为通用引用（`universal references`）。

通用引用往往出现在两个上下文中。第一个是模板函数参数。
```cpp
template <typename T>
void f(T &&param);
```
第二个是 `auto` 声明。
```cpp
auto &&var2 = var1;
```
两者的共同点是有类型推导。而下面的两个例子不涉及类型推导，那么就不是通用引用而是右值引用。
```cpp
void f(Widget &&param); // no type deduction; param is an rvalue reference

Widget &&var1 = Widget(); // no type deduction; var1 is an rvalue reference
```
通用引用也是引用，所以需要初始化。初始化的时候，绑定左值或者右值决定了通用引用是左值引用还是右值引用。对于模板函数，在调用的时候进行初始化。
```cpp
template <typename T>
void f(T &&param); // param is a universal reference

Widget w;

// lvalue passed to f; param's type is Widget& (i.e., an lvalue reference)
f(w);

// rvalue passed to f; param's type is Widget&& (i.e., an rvalue reference)
f(std::move(w));
```
对于通用引用而言，类型推导必要但不充分。其形式必须精确地是 `T&&`。
```cpp
template <typename T>
void f(std::vector<T> &&param); // param is an rvalue reference
```
除非我们指定模板函数的模板参数，否则 `T` 需要被推导。但是 `param` 的类型是 `std::vector<T> &&` 而不是 `T&&`，因此 `param` 是右值引用。如果绑定左值，编译器会报错。
```cpp
std::vector<int> v;
f(v); // error! can't bind lvalue to rvalue reference
```
即使简单地加上 `const` 修饰，也会使其成为非通用引用。
```cpp
template <typename T>
void f(const T &&param); // param is an rvalue reference
```
当看到一个参数类型是 `T&&` 的模板函数，也不能假设它就是通用引用。因为不能假定一定会出现类型推导。考虑 `std::vector` 中的 `push_back` 函数。
```cpp
// from C++ Standards
template <class T, class Allocator = allocator<T>>
class vector
{
public:
    void push_back(T &&x);
};
```
`push_back` 的参数类型是符合要求的，但是这里不涉及类型推导。因为如果 `vector` 没有被实例化，`push_back` 就不存在；而一旦 `vector` 被实例化，`push_back` 的类型就完全确定了。比如
```cpp
std::vector<Widget> v;
```
实例化后的 `std::vector` 如下所示：
```cpp
class vector<Widget, allocator<Widget>>
{
public:
    void push_back(Widget &&x); // rvalue reference
};
```
因此，`push_back` 不涉及类型推导，这里总是右值引用。

与之相对，`std::vector` 的成员函数 `emplace_back` 就涉及类型推导。
```cpp
// still from C++ Standards
template <class T, class Allocator = allocator<T>>
class vector
{
public:
    template <class... Args>
    void emplace_back(Args &&...args);
};
```
这里的 `Args` 与 `std::vector` 的参数类型 `T` 无关，因此每次调用 `emplace_back` 的时候都会涉及类型推导。

这里参数名字是 `Args`，形式也是 `Args&&`。当然，名字本身无关紧要，所以精确地说通用引用的形式是 `type&&`，且 `param` 需要类型推导。
```cpp
template <typename MyTemplateType>     // param is a
void someFunc(MyTemplateType &&param); // universal reference
```
另一处常见的通用引用是 `auto&&`，也是 `T&&` 形式，因此也涉及类型推导。假定在 C++14 中，我们要实现一个函数，统计任意函数调用的时间开销。
```cpp
auto timeFuncInvocation =
    [](auto &&func, auto &&...params) // C++14
{
    // start timer;

    std::forward<decltype(func)>(func)(           // invoke func
        std::forward<decltype(params)>(params)... // on params
    );

    // stop timer and record elapsed time;
};
```
`std::forward<decltype(xxx)>` 略微有点复杂，[Item 33](../ch06_Lambda_Expressions/33_Use_decltype_on_auto_parameters_to_std_forward_them.md) 会解释。这里的重点是 lambda 中的参数类型是 `auto&&`。所以 `func` 是通用引用，可以绑定任意可调用对象，左值或者右值都行。`params` 是零个或多个通用引用，可以绑定任意多个、任意类型的参数。有了 `auto` 通用引用，`timeFuncInvocation` 几乎可以测量任意函数的执行时间（[Item 30](./30_Familiarize_yourself_with_perfect_forwarding_failure_cases.md) 会说明为什么是“几乎任意”而不是“任意函数”）。

通用引用只是一个抽象，实际机制是引用折叠（`reference collapsing`），详见 Item 28。事实上，引用折叠并不会让通用引用变得无用。区分通用引用和右值引用，帮助我们更好地理解代码，也帮助我们更好地与他人沟通。[Item 25](./25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md) 和 [Item 26](./26_Avoid_overloading_on_universal_references.md) 也是基于这一点进行深入分析的，所以区分二者很重要。

## Things to Remember
* If a function template parameter has type `T&&` for a deduced type `T`, or if an object is declared using `auto&&`, the parameter or object is a universal reference.
* If the form of the type declaration isn't precisely `type&&`, or if type deduction does not occur, `type&&` denotes an rvalue reference.
* Universal references correspond to rvalue references if they're initialized with rvalues. They correspond to lvalue references if they're initialized with lvalues.
