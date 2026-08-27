我们有一个函数模板
```cpp
template <typename T>
void f(ParamType param);
```
当调用它时：
```cpp
f(expr);
```
编译器会根据 `expr` 推导 `T`，并据此确定 `param` 的实际类型。`ParamType` 表示形参的类型模式，它通常会包含 `const` 或引用等修饰。推导规则取决于以下三种情形：

* `ParamType` 是指针或引用，但不是通用引用（`universal reference`）（参见 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)）
* `ParamType` 是通用引用
* `ParamType` 既不是指针也不是引用

## Case 1: ParamType is a Reference or Pointer, but not a Universal Reference
类型推导规则如下：
1. 如果 `expr` 是引用，忽略其引用修饰
2. 以 `ParamType` 为模式对 `expr` 进行匹配，以确定 `T`

假定函数模板如下：
```cpp
template <typename T>
void f(T &param); // param is a reference
```
有如下三个变量：
```cpp
int x = 27;        // x is an int
const int cx = x;  // cx is a const int
const int &rx = x; // rx is a reference to const int
```
调用该函数模板时，推导结果如下：
```cpp
// 显然
f(x);  // T is int, param's type is int&

// cx 和 rx 是 const 的，所以 T 也带有 const 修饰
// param 是对应的 const int 的引用
// const 表示不能修改，类型推导也包含 const，这样才能保证函数 f 不修改传入的对象
// 因此可以将 const 对象传给参数类型为 T& 的函数模板
f(cx); // T is const int,
       // param's type is const int&

// rx 类型是引用，但是类型推导第一步就忽略了引用
// 所以 T 是 const int 而不是引用
f(rx); // T is const int,
       // param's type is const int&
```
上面的例子使用左值引用；非通用引用的右值引用形参也遵循类似的推导规则。

如果把 `f` 的参数类型从 `T &` 改为 `const T &`，推导结果会略有不同。由于函数参数本身是对 `const` 对象的引用，`T` 不再带有 `const` 修饰。
```cpp
template <typename T>
void f(const T &param); // param is now a ref-to-const

int x = 27;        // as before
const int cx = x;  // as before
const int &rx = x; // as before

f(x);  // T is int, param's type is const int&
f(cx); // T is int, param's type is const int&
f(rx); // T is int, param's type is const int&
```
如果 `param` 的类型是指针（包括指向 `const` 的指针），类型推导的基本原则也是一样的。
```cpp
template <typename T>
void f(T *param); // param is now a pointer

int x = 27;         // as before
const int *px = &x; // px is a pointer to const int

f(&x); // T is int, param's type is int*
f(px); // T is const int,
       // param's type is const int*
```
至此，C++ 的类型推导基本符合直觉。

## Case 2: ParamType is a Universal Reference
对于通用引用，情况就不那么直观了。参数声明在语法上像右值引用，但传入左值时，其推导行为不同。具体参见 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)，这里先作简要介绍。

* 如果 `expr` 是左值，那么 `T` 会被推导为左值引用；经引用折叠后，`param` 的类型也是左值引用。这是唯一会使 `T` 成为引用类型的场景：`ParamType` 的语法形式虽然是右值引用，但最终会成为左值引用。
* 如果 `expr` 是右值，那么一切都回到了直观的场景（参见 Case 1）。

比如
```cpp
template <typename T>
void f(T &&param); // param is now a universal reference

int x = 27;        // as before
const int cx = x;  // as before
const int &rx = x; // as before

f(x);  // x is lvalue, so T is int&,
       // param's type is also int&
f(cx); // cx is lvalue, so T is const int&,
       // param's type is also const int&
f(rx); // rx is lvalue, so T is const int&,
       // param's type is also const int&
f(27); // 27 is rvalue, so T is int,
       // param's type is therefore int&&
```
[Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md) 会作详细解释。关键在于，通用引用参数的类型推导规则不同于左值引用或右值引用参数的规则。使用通用引用时，类型推导会区别对待左值和右值；非通用引用参数则没有这种区别。

## Case 3: ParamType is Neither a Pointer nor a Reference
这里讨论按值传递的情形。
```cpp
template <typename T>
void f(T param); // param is now passed by value
```
`param` 按值传递，会创建实参的副本，因此推导规则如下：
1. 如果 `expr` 是引用类型，忽略引用
2. 在第 1 步的基础上，忽略顶层的 `const` 和 `volatile` 修饰。（`volatile` 不常见，参见 [Item 40](../ch07_The_Concurrency_API/40_Use_std_atomic_for_concurrency_volatile_for_special_memory.md)）

```cpp
int x = 27;        // as before
const int cx = x;  // as before
const int &rx = x; // as before

f(x);  // T's and param's types are both int

// cx 和 rx 是 const 的，但是 param 不是
// param 是与 cx 和 rx 无关的另一个拷贝副本；cx 和 rx 是否可修改与 param 是否可修改无关
f(cx); // T's and param's types are again both int
f(rx); // T's and param's types are still both int
```
认识到只有按值传递时，顶层 `const` 和 `volatile` 才会被忽略非常重要。如前所述，如果参数是指向 `const` 的指针或引用，`expr` 的 `const` 属性会被保留。下面考虑一种情形：`param` 按值传递，实参是指向 `const` 对象的常量指针。
```cpp
template <typename T>
void f(T param);        // param is still passed by value
const char *const ptr = // ptr is const pointer to const object
    "Fun with pointers";
f(ptr); // pass arg of type const char * const
```
`*` 右侧的 `const` 表示指针本身不能被重新赋值，也不能改为 `nullptr`。将 `ptr` 按值传递给 `f` 时，会创建 `ptr` 的副本。根据按值传递的类型推导规则，`ptr` 的顶层 `const` 被移除，因此 `param` 的类型是 `const char *`：指针本身可以修改，但不能通过它修改所指向的对象。

## Array Arguments
还有一个场景没有讨论到，源于数组可以退化成指针。
```cpp
const char name[] = "J. P. Briggs"; // name's type is const char[14]
const char *ptrToName = name;       // array decays to pointer
```
`name` 的类型是 `const char[14]`，与 `ptrToName` 的类型 `const char*` 不同；不过数组可以转换为指针，因此该初始化可以通过编译。

把数组传入按值传递的函数模板会推导成什么类型呢？
```cpp
template <typename T>
void f(T param); // template with by-value parameter
f(name);         // what types are deduced for T and param?
```
在函数声明中，数组类型的形参会被调整为指针类型，所以下面两种声明是等价的。
```cpp
void myFunc(int param[]);
void myFunc(int *param); // same function as above
```
这一规则源自 C 语言，但容易让人误以为数组和指针是同一种类型。

由于数组到指针的转换，推导得到的 `T` 是 `const char *`。
```cpp
f(name); // name is array, but T deduced as const char*
```
不过，函数形参不能具有真正的数组类型，但可以声明为数组引用。
```cpp
template <typename T>
void f(T &param); // template with by-reference parameter
f(name);          // pass array to f
```
此时，`T` 的类型是真正的数组类型 `const char[14]`，那么 `f` 的参数类型是 `const char (&)[14]`。

这使得我们可以创建一个函数模板，让编译器在编译期计算数组的大小。
```cpp
// return size of an array as a compile-time constant. (The
// array parameter has no name, because we care only about
// the number of elements it contains.)
template <typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}
```
`constexpr` 使得在满足常量表达式要求时，调用可以在编译期求值（参见 [Item 15](../ch03_Moving_to_Modern_C++/15_Use_constexpr_whenever_possible.md)）。`noexcept` 可以帮助编译器进行更好的优化（参见 [Item 14](../ch03_Moving_to_Modern_C++/14_Declare_functions_noexcept_if_they_wont_emit_exceptions.md)）。

使用这个函数可以容易地创建出大小相同的数组。
```cpp
int keyVals[] = {1, 3, 7, 9, 11, 22, 35}; // keyVals has 7 elements
int mappedVals[arraySize(keyVals)];       // so does mappedVals

// better
std::array<int, arraySize(keyVals)> mappedVals; // mappedVals' size is 7
```

## Function Arguments
函数类型与数组类型类似：用于按值形参的类型推导时，函数会转换为函数指针，因此推导行为与上述描述一致。
```cpp
void someFunc(int, double); // someFunc is a function; type is void(int, double)

template <typename T>
void f1(T param); // in f1, param passed by value

template <typename T>
void f2(T &param); // in f2, param passed by ref

f1(someFunc); // param deduced as ptr-to-func; type is void (*)(int, double)
f2(someFunc); // param deduced as ref-to-func; type is void (&)(int, double)
```

## Things to Remember
* During template type deduction for non-universal-reference parameters, reference arguments are treated as non-references, i.e., their reference-ness is ignored.
* When deducing types for universal reference parameters, lvalue arguments get special treatment.
* When deducing types for by-value parameters, top-level const and/or volatile qualifiers are ignored.
* During template type deduction for non-reference parameters, array and function arguments decay to pointers.
