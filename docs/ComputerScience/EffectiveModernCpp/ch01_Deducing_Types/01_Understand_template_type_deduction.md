我们有一个函数模板
```cpp
template <typename T>
void f(ParamType param);
```
当调用它的时候
```cpp
f(expr);
```
编译器会自动推断 `T` 和 `ParamType` 的类型。而这两者往往不同，因为后者会有 `const` 或者引用等修饰。从 `expr` 中需要同时推导 `T` 和 `ParamType`，我们需要考虑如下三种场景：

* `ParamType` 是指针或者引用，但不是通用引用（`universal reference`）（参考 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)）
* `ParamType` 是通用引用
* `ParamType` 既不是指针也不是引用

## Case 1: ParamType is a Reference or Pointer, but not a Universal Reference
类型推导规则是
1. `expr` 是引用的话把引用修饰忽略
2. 以 `ParamType` 为背景对 `expr` 进行模式匹配来确定 `T`

假定函数模板如下：
```cpp
template <typename T>
void f(T &param); // param is a reference
```
有如下三个变量
```cpp
int x = 27;        // x is an int
const int cx = x;  // cx is a const int
const int &rx = x; // rx is a reference to x as a const int
```
那么调用函数模板得到以下推导类型
```cpp
// 显然
f(x);  // T is int, param's type is int&

// cx rx 是 const 的，所以 T 也是 const 修饰的
// param 是对应的 const int 的引用
// const 表示不能修改，类型推导也包含 const，这样才能保证函数 f 不修改传入的对象
// 所以将 const 对象传入 T& 为参数的函数模板是安全的
f(cx); // T is const int,
       // param's type is const int&

// rx 类型是引用，但是类型推导第一步就忽略了引用
// 所以 T 是 const int 而不是引用
f(rx); // T is const int,
       // param's type is const int&
```
上面的例子是左值引用，右值引用的类型推导是类似的。

如果把 `f` 的参数的类型从 `T &` 变成 `const T &`，会有略微不同。因为函数声明就是 `const` 对象的引用，那么类型 `T` 不再有 `const` 修饰。
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
如果参数 `param` 是指针或者指向 `const` 的指针，类型推导基本原则是一样的。
```cpp
template <typename T>
void f(T *param); // param is now a pointer

int x = 27;         // as before
const int *px = &x; // px is a ptr to x as a const in

f(&x); // T is int, param's type is int*
f(px); // T is const int,
       // param's type is const int*
```
至此，C++的类型推导基本符合直觉。

## Case 2: ParamType is a Universal Reference
如果是通用引用的话，事情就不这么明显了。参数声明像右值引用，但是当左值传入的时候，行为却不一样。具体参见 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md)，这里先大致介绍一下。

* 如果 `expr` 是左值，那么 `T` 和 `ParamType` 都被推导成了左值引用。这是唯一一个场景使得 `T` 是引用类型，其次 `ParamType` 语法是右值引用，但是被推导成左值引用。
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
[Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md) 会详细解释。这里关键点是对于通用引用参数的类型推导规则和参数类型是左值引用或者右值引用是不同的。当使用通用引用时，类型推导会区别对待左值引用和右值引用，对于非通用引用不会有这样的不同。

## Case 3: ParamType is Neither a Pointer nor a Reference
这里就是按值传递了。
```cpp
template <typename T>
void f(T param); // param is now passed by value
```
`param` 按值传递，会拷贝一个新的对象，所以推导规则如下：
1. `expr` 如果是引用类型，忽略引用
2. 在 1 的基础上，如果有 `const` 修饰，忽略 `const`。如果是 `volatile`，也忽略。（`volatile` 不常见，参加 Item 40（TODO 贴链接））

```cpp
int x = 27;        // as before
const int cx = x;  // as before
const int &rx = x; // as before

f(x);  // T's and param's types are both int

// cx 和 rx 是 const 的，但是 param 不是
// param 是与 cx rx 无关的另一个拷贝副本，cx rx 是否能修改与 param 能够修改无关
f(cx); // T's and param's types are again both int
f(rx); // T's and param's types are still both int
```
意识到只有按值传递时 `const` `volatile` 才能被忽略是非常重要的事情。如前所述，如果参数是指向 `const` 的指针或者引用，`expr` 的 `const` 属性会被保留。这里考虑如下场景，参数是指向 `const` 的 `const` 的指针，`param` 按值传递。
```cpp
template <typename T>
void f(T param);        // param is still passed by value
const char *const ptr = // ptr is const pointer to const object
    "Fun with pointers";
f(ptr); // pass arg of type const char * const
```
星号右边的 `const` 表示指针不能指向其他位置，包括 `null`。`ptr` 传递给 `f` 的时候，`ptr` 自身拷贝了一次，按值传递。类型推导根据按值传递的规则，`ptr` 自身的 `const` 属性被移除，所以 `param` 类型是 `const char *`，指针本身可以修改，指向内存不能修改。

## Array Arguments
还有一个场景没有讨论到，源于数组可以退化成指针。
```cpp
const char name[] = "J. P. Briggs"; // name's type is const char[13]
const char *ptrToName = name;       // array decays to pointer
```
`name` 的类型是 `const char[13]`，与 `ptrToName` 的类型是 `const char*` 是不同的，不过存在退化，所以可以编译。

把数组传入按值传递的函数模板会推导成什么类型呢？
```cpp
template <typename T>
void f(T param); // template with by-value parameter
f(name);         // what types are deduced for T and param?
```
函数参数是数组会被视为指针，所以下面两种声明是等价的。
```cpp
void myFunc(int param[]);
void myFunc(int *param); // same function as above
```
这一点源自 C 语言，不过会让人以为两者是一样的。

由于退化，所以类型推导结果为 `T` 的类型是 `const char *`。
```cpp
f(name); // name is array, but T deduced as const char*
```
还有另外一种情况，类型不能是真的数组类型，但是能声明成数组引用。
```cpp
template <typename T>
void f(T &param); // template with by-reference parameter
f(name);          // pass array to f
```
此时，`T` 的类型是真实的数组 `const char [13]`，那么 `f` 参数类型是 `const char (&)[13]`。

这使得我们可以创建一个函数模板，编译器计算数组的大小。
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
`constexpr` 使得可以编译器得到结果（参见 [Item 15](../ch03_Moving_to_Modern_C++/15_Use_constexpr_whenever_possible.md)）。`noexcept` 可以帮助编译器更好的优化（参见 [Item 14](../ch03_Moving_to_Modern_C++/14_Declare_functions_noexcept_if_they_wont_emit_exceptions.md)）。

使用这个函数可以容易的创建处大小一样的数组。
```cpp
int keyVals[] = {1, 3, 7, 9, 11, 22, 35}; // keyVals has 7 elements
int mappedVals[arraySize(keyVals)];       // so does mappedVals

// better
std::array<int, arraySize(keyVals)> mappedVals; // mappedVals' size is 7
```

## Function Arguments
函数类型和数组类型类似，也会退化成指针，那么推导行为和上述描述的一致。
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
* During template type deduction, arguments that are references are treated as non-references, i.e., their reference-ness is ignored.
* When deducing types for universal reference parameters, lvalue arguments get special treatment.
* When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
* During template type deduction, arguments that are array or function names decay to pointers, unless they're used to initialize references.
