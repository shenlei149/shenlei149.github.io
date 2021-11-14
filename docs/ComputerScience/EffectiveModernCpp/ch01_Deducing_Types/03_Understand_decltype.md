给一个名字或者表达式，`decltype` 返回其类型。大部分时候，它按照预期工作，不过极个别场合能让你头疼。

我们先从不会让我们感到惊讶的例子开始。`decltype` 近乎于鹦鹉学舌。
```cpp
const int i = 0;            // decltype(i) is const int
bool f(const Widget& w);    // decltype(w) is const Widget&
                            // decltype(f) is bool(const Widget&)
struct Point {
 int x, y;                  // decltype(Point::x) is int
};                          // decltype(Point::y) is int
Widget w;                   // decltype(w) is Widget
if (f(w))                   // decltype(f(w)) is bool
template<typename T>        // simplified version of std::vector
class vector {
public:
 T& operator[](std::size_t index);
};
vector<int> v;              // decltype(v) is vector<int>

if (v[0] == 0)              // decltype(v[0]) is int&
```

C++11 中，`decltype` 的一个常用的地方是写模板函数的返回类型，其依赖于函数参数的类型。比如一个函数接受一个容器对象，其支持索引 `operator[]` 运算符，返回的就是某个索引对应的对象，那么返回类型应该是该容器索引 `operator[]` 运算符的返回类型。

通常情况下，类型为 `T` 的容器的 `operator[]` 运算符返回类型是 `T&`。比如 `std::deque`，和大部分的 `std::vector`。对于 `std::vector<bool>` 而言，`operator[]` 返回的不是 `bool&` 而是一个新的对象。[Item 6](../ch02_auto/06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md) 会探究其原因，不过，这里的重点是容器的 `operator[]` 返回类型依赖于容器。

这种情况下，`decltype` 能够简化表达。下面是第一个版本，展示了如何使用 `decltype` 表达返回类型。
```cpp
template<typename Container, typename Index>    // works, but
auto authAndAccess(Container& c, Index i)       // requires
 -> decltype(c[i])                              // refinement
{
    authenticateUser();
    return c[i];
}
```

函数开头的 `auto` 并不表示类型推导。而是 C++11 引入的新语法，返回类型后置到参数列表后的 `->` 之后。这样，在写返回类型的时候，可以使用函数参数。`authAndAccess` 的返回类型就用到了 `c` 和 `i`，如果前置，那么这两个参数不可用。

这样，函数的返回类型和容器 `operator[]` 运算符返回类型一致了。

C++14 允许对任意函数或 lambda 使用 `auto` 来推导返回类型，所以上面的例子可以去掉后置返回类型。此时，`auto` 就是表示类型推导的意思。
```cpp
template<typename Container, typename Index>    // C++14;
auto authAndAccess(Container& c, Index i)       // not quite correct
{
    authenticateUser();
    return c[i]; // return type deduced from c[i]
}
```

[Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 解释此时 `auto` 使用的是模板函数的推导规则。这里就存在这样一个问题。`T` 类型的容器的 `operator[]` 往往返回引用类型 `T&`，而 [Item 1](../ch01_Deducing_Types/01_Understand_template_type_deduction.md) 中说过，模板函数推导往往会忽略引用修饰。这样，如下代码将无法编译。
```cpp
std::deque<int> d;
authAndAccess(d, 5) = 10;   // authenticate user, return d[5],
                            // then assign 10 to it;
                            // this won't compile!
```
`d[5]` 返回 `int&` 类型，但是 `authAndAccess` 的返回类型被推导为 `int`，那么函数返回值是一个右值，尝试给右值赋值为 10 是不允许的。

我们使用 `decltype` 来修复这个问题。C++14 中引入了 `decltype(auto)`。`auto` 是说类型是被推导得到的，`decltype` 的含义是按照 `decltype` 的规则推导。代码改写如下。
```cpp
template<typename Container, typename Index>    // C++14; works,
decltype(auto)                                  // but still
authAndAccess(Container& c, Index i)            // requires refinement
{
    authenticateUser();
    return c[i];
}
```
现在不管 `c[i]` 返回类型是 `T&` 还是新的对象的类型，那么函数 `authAndAccess` 返回类型就都和 `c[i]` 一致了。

`decltype(auto)` 不限于只在函数返回值时使用，想使用 `decltype` 推导规则来声明变量时也可以用。
```cpp
Widget w;
const Widget& cw = w;
auto myWidget1 = cw;        // auto type deduction:
                            // myWidget1's type is Widget

decltype(auto) myWidget2 = cw;      // decltype type deduction:
                                    // myWidget2's type is
                                    // const Widget&
```

现在接着优化之前的函数。其函数签名是
```cpp
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container& c, Index i);
```
容器通过左值非 `const` 引用传递，返回结果可以被修改，但是无法绑定右值，除非是左值 `const` 引用方式传递。

当然，传递右值引用是一个不常见的情况。一个右值容器，是一个临时对象，当函数调用结束之后会被销毁，那么拿到一个被销毁的容器的某个对象的引用是很危险的。不过，`authAndAccess` 可以返回其拷贝。
```cpp
std::deque<std::string> makeStringDeque();      // factory function

// make copy of 5th element of deque returned from makeStringDeque
auto s = authAndAccess(makeStringDeque(), 5);
```

一个解决方案是使用函数重载，但是这样需要维护两份代码。另一个解决方案是使用 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md) 阐述的通用引用，那么函数签名如下
```cpp
template<typename Container, typename Index>            // c is now a
decltype(auto) authAndAccess(Container&& c, Index i);   // universal reference
```

由于不知道容器类型，也就是不知道具体返回对象的类型。如果值传递的话，可能有复制带来的性能问题。不过有的 STL 容器类，比如 `vector, string` 的 `operator[]` 也会返回值类型，所以我们也选择这么做。

不过，这里根据 [Item 25](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md) 的规则对通用引用使用 `std::forward`。
```cpp
template<typename Container, typename Index>    // final
decltype(auto)                                  // C++14
authAndAccess(Container&& c, Index i)           // version
{
    authenticateUser();
    return std::forward<Container>(c)[i];
}
```

回到开头的问题，大部分情况 `decltype` 会返回我们希望的类型，但是偶尔会给出一个令人吃惊的结果。为了全面理解 `decltype`，我们再看几个特殊情况。

`decltype` 作用于一个变量名，其是左值，没有问题。不过如果作用于一个表达式而不是变量名，总是返回左值引用。这几乎没啥影响。左值表达式自身包含了左值引用的修饰符。

不过有些情况会有些许差异。比如
```cpp
int x = 0;
```

`x` 是一个变量名，那么 `decltype(x)` 返回 `int`。如果给 `x` 加上括号 `(x)` 这就是个表达式而不是变量名了。`x` 是左值，`(x)` 是左值表达式，那么 `decltype((x))` 返回类型是 `int&`。

结合 C++14 的 `decltype(auto)` 后，情况会更复杂一些。
```cpp
decltype(auto) f1()
{
    int x = 0;
    return x;      // decltype(x) is int, so f1 returns int
}
decltype(auto) f2()
{
    int x = 0;
    return (x);    // decltype((x)) is int&, so f2 returns int&
}
```

注意，`f2` 和 `f1` 的返回类型不同，后者是 `int&`，而且是局部变量的引用，后续使用完全是未定义行为。

这个例子告诉我们要特别注意 `decltype(auto)` 的使用。看似无关紧要的细节会影响 `decltype(auto)` 的结果。可以使用 [Item 4](./04_Know_how_to_view_deduced_types.md) 中描述的技术确保结果是我们期望的。

同时，不要忽视全局。大部分情况下，不管是否和 `auto` 配合使用，`decltype` 只有很多的情况会给出不符合预期的结果。特别是作用于变量名的时候，就是简单返回其类型。

## Things to Remember
* `decltype` almost always yields the type of a variable or expression without any modifications.
* For lvalue expressions of type `T` other than names, `decltype` always reports a type of `T&`.
* C++14 supports `decltype(auto)`, which, like `auto`, deduces a type from its initializer, but it performs the type deduction using the `decltype` rules.
