`auto` 类型推导和模板类型推导类似，只有一个例外。

在 [Item 1](./01_Understand_template_type_deduction.md) 中，使用下面这样通用的模板函数进行描述。
```cpp
template<typename T>
void f(ParamType param);

f(expr);        // call f with some expression
```

通过 `expr` 的类型推导 `T` 和 `ParamType` 的类型。

`auto` 类型推导中，`auto` 扮演了 `T` 的角色，而类型说明符整体和 `ParamType` 一样。

下面三个语句的类型说明符非别是 `auto` 自身，`const auto` 和 `const auto&`。
```cpp
auto x = 27;
const auto cx = x;
const auto& rx = x;
```

此时，编译器推导规则和推导下面模板类型一样。
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

根据 `ParamType` 的不同特征，[Item 1](./01_Understand_template_type_deduction.md) 讨论时分成了三种情况，这里也是类似的：

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

[Item 1](./01_Understand_template_type_deduction.md) 中讨论了对于非引用类型，数组和函数退化成了指针，`auto` 类型推导也是类似的：
```cpp
const char name[] = "R. N. Briggs";     // name's type is const char[13]

auto arr1 = name;       // arr1's type is const char*
auto& arr2 = name;      // arr2's type is const char (&)[13]

void someFunc(int, double);     // someFunc is a function; type is void(int, double)

auto func1 = someFunc;      // func1's type is void (*)(int, double)
auto& func2 = someFunc;     // func2's type is void (&)(int, double)
```

现在讨论两者唯一的差异。C++98 中可以用下面两种方式声明并初始化一个 `int` 值：
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

这些代码可以正常编译，不过意义不同。前两个会推导成 `int` 类型，初始化成 27。后面两个会推导成 `std::initializer_list<int>`，包含一个元素——27！
```cpp
auto x1 = 27;       // type is int, value is 27
auto x2(27);        // ditto
auto x3 = { 27 };   // type is std::initializer_list<int>, value is { 27 }
auto x4{ 27 };      // ditto
```

这就是对 `auto` 进行类型推导的特殊规则。当使用括号对 `auto` 变量初始化的时候，类型被推导成 `std::initializer_list`。如果不能进行推导，比如初始化列表的值的类型不同，无法编译。如：
```cpp
auto x5 = { 1, 2, 3.0 };    // error! can't deduce T for std::initializer_list<T>
```
上面的语句中有两类推导。第一个是 `auto x5`，由于使用了括号形式的统一初始化，那么 `x5` 类型是 `std::initializer_list`，而后者是模板类。为了实例化 `std::initializer_list<T>`，需要推导出 `T` 的类型。但是里面有两种不同类型的数值导致 `T` 推导失败。

这是 `auto` 类型推导和模板推导的不同。如果向模板函数传入初始化列表，类型推导会失败。
```cpp
auto x = { 11, 23, 9 };     // x's type is std::initializer_list<int>

template<typename T>        // template with parameter declaration
void f(T param);            // equivalent to x's declaration

f({ 11, 23, 9 });           // error! can't deduce type for T
```

如果我们把模板参数 `param` 设成 `std::initializer_list<T>`，这样就能推导类型 `T` 了。
```cpp
template<typename T>
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 });   // T deduced as int, and initList's
                    // type is std::initializer_list<int>
```

所以唯一的区别就是 `auto` 假设使用统一初始化的变量类型是 `std::initializer_list` 而不模板类型推导没有这个假设。

本书作者也没有搞明白为什么要有这个规则。不过规则就是规则，记住就好了。正是由于有这个陷阱，很多开发者只有在必要的时候才加大括号。参见 [Item 7](../ch03_Moving_to_Modern_C++/07_Distinguish_between_parenthesis_and_brace_when_creating_objects.md)。

C++14 中函数的返回值允许是 `auto`，lambda 表达式的参数也可以使用 `auto`，不过此时类型推导使用的模板类型推导规则而不是 `auto` 类型推导规则。也就是说，函数返回或者是向 lambda 传入一个大括号写作的统一初始化语句，会编译失败。
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
* `auto` type deduction is usually the same as template type deduction, but `auto` type deduction assumes that a braced initializer represents a `std::initializer_list`, and template type deduction doesn't.
* `auto` in a function return type or a lambda parameter implies template type deduction, not `auto` type deduction.
