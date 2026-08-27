`auto` 类型推导和模板类型推导类似，只有一个例外。

在 [Item 1](./01_Understand_template_type_deduction.md) 中，使用下面这样通用的模板函数进行描述。
```cpp
template<typename T>
void f(ParamType param);

f(expr);        // call f with some expression
```

根据 `expr` 的类型推导出 `T` 和 `ParamType` 的类型。

在 `auto` 类型推导中，`auto` 扮演了 `T` 的角色，而整个类型说明符相当于 `ParamType`。

下面三个语句的类型说明符分别是 `auto`、`const auto` 和 `const auto&`。
```cpp
auto x = 27;
const auto cx = x;
const auto& rx = x;
```

此时，编译器的推导规则与从下面的模板进行类型推导时相同。
```cpp
template<typename T>        // conceptual template for
void func_for_x(T param);   // deducing x's type

func_for_x(27);     // conceptual call: param's deduced type is x's type

template<typename T>                // conceptual template for
void func_for_cx(const T param);    // deducing cx's type

func_for_cx(x);     // conceptual call: param's deduced type is cx's type

template<typename T>                // conceptual template for
void func_for_rx(const T& param);   // deducing rx's type

func_for_rx(x);     // conceptual call: param's deduced type is rx's type
```

根据 `ParamType` 的不同特征，[Item 1](./01_Understand_template_type_deduction.md) 将模板类型推导分成了三种情况；这里也类似：

* 类型说明符是指针或者引用，但不是通用引用
* 类型说明符是通用引用
* 类型说明符既不是指针也不是引用

下面是第一种和第三种情况：
```cpp
auto x = 27;            // case 3 (x is neither ptr nor reference)
const auto cx = x;      // case 3 (cx isn't either)
const auto& rx = x;     // case 1 (rx is a non-universal ref.)
```

下面是第二种情况：
```cpp
auto&& uref1 = x;       // x is int and lvalue, so uref1's type is int&
auto&& uref2 = cx;      // cx is const int and lvalue, so uref2's type is const int&
auto&& uref3 = 27;      // 27 is int and rvalue, so uref3's type is int&&
```

[Item 1](./01_Understand_template_type_deduction.md) 讨论过：当类型说明符不是引用时，数组和函数会退化为指针。`auto` 类型推导也是如此：
```cpp
const char name[] = "R. N. Briggs";     // name's type is const char[13]

auto arr1 = name;       // arr1's type is const char*
auto& arr2 = name;      // arr2's type is const char (&)[13]

void someFunc(int, double);     // someFunc is a function; type is void(int, double)

auto func1 = someFunc;      // func1's type is void (*)(int, double)
auto& func2 = someFunc;     // func2's type is void (&)(int, double)
```

现在讨论两者唯一的差异：大括号初始化。C++98 中可以用下面两种方式声明并初始化一个 `int` 值：
```cpp
int x1 = 27;
int x2(27);
```

C++11 引入了统一初始化：
```cpp
int x3 = { 27 };
int x4{ 27 };
```

[Item 5](../ch02_auto/05_Prefer_auto_to_explicit_type_declarations.md) 阐述了使用 `auto` 替代固定类型有很多好处，所以这里直接把 `int` 替换成 `auto` 得到如下代码。
```cpp
auto x1 = 27;
auto x2(27);
auto x3 = { 27 };
auto x4{ 27 };
```

在 C++11 和 C++14 中，这些代码都可以正常编译，但推导结果不同：前两个会推导为 `int` 类型，值为 27；后两个会推导为包含一个元素 27 的 `std::initializer_list<int>`。
```cpp
auto x1 = 27;       // type is int, value is 27
auto x2(27);        // ditto
auto x3 = { 27 };   // type is std::initializer_list<int>, value is { 27 }
auto x4{ 27 };      // ditto
```

这就是 `auto` 类型推导的特殊规则：在 C++11 和 C++14 中，当使用大括号初始化 `auto` 变量时，类型会被推导为 `std::initializer_list`。如果无法完成这种推导，例如初始化列表中的元素类型不同，代码便无法编译：
```cpp
auto x5 = { 1, 2, 3.0 };    // error! can't deduce T for std::initializer_list<T>
```
上面的语句涉及两个层面的类型推导。对于 `auto x5`，由于使用了大括号形式的统一初始化，`x5` 的类型必须是 `std::initializer_list<T>`。为了实例化模板类 `std::initializer_list<T>`，还需要推导出 `T` 的类型；但列表中同时有 `int` 和 `double`，因此 `T` 的推导失败。

这正是 `auto` 类型推导与模板类型推导的不同之处。如果向模板函数传入初始化列表，类型推导会失败。
```cpp
auto x = { 11, 23, 9 };     // x's type is std::initializer_list<int>

template<typename T>        // template with parameter declaration
void f(T param);            // equivalent to x's declaration

f({ 11, 23, 9 });           // error! can't deduce type for T
```

如果将模板参数 `param` 的类型设为 `std::initializer_list<T>`，就能推导出 `T` 的类型。
```cpp
template<typename T>
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 });   // T deduced as int, and initList's
                    // type is std::initializer_list<int>
```

因此，在 C++11 和 C++14 中，唯一的区别在于：`auto` 会将大括号初始化的变量视为 `std::initializer_list`，而模板类型推导没有这种假设。

自 C++17 起，直接列表初始化的规则有所变化：`auto x{27};` 推导为 `int`，而包含多个元素的直接列表初始化（如 `auto x{1, 2};`）不合法；复制列表初始化（如 `auto x = {27};`）仍会推导为 `std::initializer_list<int>`。本节前面的 `x4` 示例及其注释适用于 C++11 和 C++14。

本书作者也不清楚为什么会有这条规则。不过规则就是规则，记住即可。正是由于这个陷阱，许多开发者只在必要时才使用大括号。参见 [Item 7](../ch03_Moving_to_Modern_C++/07_Distinguish_between_parenthesis_and_brace_when_creating_objects.md)。

C++14 允许函数返回类型为 `auto`，也允许 lambda 表达式的参数使用 `auto`。不过，此时使用的是模板类型推导规则，而不是 `auto` 类型推导规则。也就是说，函数返回大括号初始化列表，或向 lambda 传入大括号初始化列表，都会导致编译失败。
```cpp
auto createInitList()
{
 return { 1, 2, 3 };    // error: can't deduce type for { 1, 2, 3 }
}

std::vector<int> v;
auto resetV =
 [&v](const auto& newValue) { v = newValue; };      // C++14

resetV({ 1, 2, 3 });    // error! can't deduce type for { 1, 2, 3 }
```


## Things to Remember
* `auto` type deduction is usually the same as template type deduction, but it has special rules for braced initializers that template type deduction lacks.
* `auto` in a function return type or a lambda parameter implies template type deduction, not `auto` type deduction.
