对一个名字或表达式使用 `decltype`，它会返回其类型。大多数时候，它的行为符合预期，不过在极少数情况下也会令人困惑。

我们先从不会令人惊讶的例子开始。`decltype` 几乎会原样报告类型。
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

C++11 中，`decltype` 的一个常见用途是指定依赖于函数参数类型的模板函数返回类型。例如，一个函数接受支持下标 `operator[]` 的容器对象，并返回指定下标处的元素；其返回类型应当与该容器的 `operator[]` 返回类型相同。

通常，元素类型为 `T` 的容器的 `operator[]` 会返回 `T&`，例如 `std::deque` 和除 `std::vector<bool>` 外的 `std::vector`。对于 `std::vector<bool>`，`operator[]` 返回的不是 `bool&`，而是一个代理对象。[Item 6](../ch02_auto/06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md) 会探究其中的原因；此处的重点是，容器的 `operator[]` 返回类型取决于容器的具体类型。

在这种情况下，`decltype` 能简化返回类型的表达。下面的第一个版本展示了如何用 `decltype` 指定返回类型。
```cpp
template<typename Container, typename Index>    // works, but
auto authAndAccess(Container& c, Index i)       // requires
 -> decltype(c[i])                              // refinement
{
    authenticateUser();
    return c[i];
}
```

函数开头的 `auto` 并不表示类型推导，而是 C++11 引入的后置返回类型语法的一部分：返回类型写在参数列表之后的 `->` 后面。这样指定返回类型时便可以使用函数参数。`authAndAccess` 的返回类型用到了 `c` 和 `i`；若将返回类型前置，这两个参数尚不可用。

这样，函数的返回类型便与容器 `operator[]` 的返回类型一致。

C++14 允许任意函数或 lambda 使用 `auto` 推导返回类型，所以上面的例子可以省去后置返回类型。此时，`auto` 确实表示类型推导。
```cpp
template<typename Container, typename Index>    // C++14;
auto authAndAccess(Container& c, Index i)       // not quite correct
{
    authenticateUser();
    return c[i]; // return type deduced from c[i]
}
```

[Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 解释了此处 `auto` 采用模板类型推导规则。问题在于，元素类型为 `T` 的容器的 `operator[]` 往往返回引用类型 `T&`，而 [Item 1](../ch01_Deducing_Types/01_Understand_template_type_deduction.md) 说明了模板类型推导通常会忽略引用修饰。因此，下面的代码无法编译。
```cpp
std::deque<int> d;
authAndAccess(d, 5) = 10;   // authenticate user, return d[5],
                            // then assign 10 to it;
                            // this won't compile!
```
`d[5]` 返回 `int&`，但 `authAndAccess` 的返回类型被推导为 `int`。因此，函数返回值是一个右值，不能对其赋值。

可以用 `decltype` 修复这个问题。C++14 引入了 `decltype(auto)`：`auto` 表示类型需要推导，`decltype` 表示推导遵循 `decltype` 的规则。代码改写如下。
```cpp
template<typename Container, typename Index>    // C++14; works,
decltype(auto)                                  // but still
authAndAccess(Container& c, Index i)            // requires refinement
{
    authenticateUser();
    return c[i];
}
```
现在，无论 `c[i]` 返回 `T&` 还是一个代理对象，`authAndAccess` 的返回类型都会与 `c[i]` 的类型一致。

`decltype(auto)` 不仅可用于函数返回类型；声明变量时，若希望使用 `decltype` 推导规则，也可以使用它。
```cpp
Widget w;
const Widget& cw = w;
auto myWidget1 = cw;        // auto type deduction:
                            // myWidget1's type is Widget

decltype(auto) myWidget2 = cw;      // decltype type deduction:
                                    // myWidget2's type is
                                    // const Widget&
```

现在继续改进前面的函数。它的函数签名是：
```cpp
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container& c, Index i);
```
容器通过非常量左值引用传递，因此返回结果可以被修改；但该函数无法接受右值容器。

当然，传入右值容器并不常见。右值容器通常是临时对象，会在完整表达式结束时销毁，因此保留其元素的引用很危险。不过，调用者可以立即复制该元素：
```cpp
std::deque<std::string> makeStringDeque();      // factory function

// make copy of 5th element of deque returned from makeStringDeque
auto s = authAndAccess(makeStringDeque(), 5);
```

一种解决方案是使用函数重载，但这需要维护两份代码。另一种解决方案是使用 [Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md) 阐述的通用引用，函数签名如下：
```cpp
template<typename Container, typename Index>            // c is now a
decltype(auto) authAndAccess(Container&& c, Index i);   // universal reference
```

由于容器类型未知，无法显式写出具体的返回类型。若一律按值返回，可能引入不必要的复制开销；因此，应保留 `operator[]` 的实际返回类型。

不过，这里应当根据 [Item 25](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md) 的规则，对通用引用使用 `std::forward`。
```cpp
template<typename Container, typename Index>    // final
decltype(auto)                                  // C++14
authAndAccess(Container&& c, Index i)           // version
{
    authenticateUser();
    return std::forward<Container>(c)[i];
}
```

回到开头的话题：大多数情况下，`decltype` 会返回我们期望的类型，但偶尔会给出令人惊讶的结果。为了全面理解 `decltype`，再来看几个特殊情况。

对未加括号的标识表达式或类成员访问表达式使用 `decltype` 时，它会直接报告所指实体的声明类型。对于其他表达式，`decltype` 的结果取决于表达式的值类别：左值表达式类型为 `T` 时结果为 `T&`，将亡值表达式类型为 `T` 时结果为 `T&&`，纯右值表达式类型为 `T` 时结果为 `T`。

不过，在某些看似无关紧要的情况下，结果会有所不同。例如：
```cpp
int x = 0;
```

`x` 是未加括号的标识表达式，因此 `decltype(x)` 返回 `int`。给 `x` 加上括号后，`(x)` 仍是左值表达式，但不再是未加括号的标识表达式，所以 `decltype((x))` 返回 `int&`。

结合 C++14 的 `decltype(auto)` 后，情况会更微妙一些。
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

注意，`f2` 和 `f1` 的返回类型不同：`f2` 返回 `int&`，且该引用指向局部变量；在函数返回后使用它会导致未定义行为。

这个例子说明，使用 `decltype(auto)` 时必须格外小心。看似无关紧要的细节会影响其推导结果。可以使用 [Item 4](./04_Know_how_to_view_deduced_types.md) 中描述的技术，确认结果符合预期。

同时，不要忽略“几乎”二字。无论是否与 `auto` 配合使用，`decltype` 在绝大多数情况下都会按预期工作；尤其是作用于未加括号的标识表达式时，它只是简单地返回所指实体的声明类型。

## Things to Remember
* `decltype` almost always yields the type of a variable or expression without any modifications.
* For lvalue expressions of type `T` other than unparenthesized id-expressions and unparenthesized class member access expressions, `decltype` always reports `T&`.
* C++14 supports `decltype(auto)`, which, like `auto`, deduces a type from its initializer, but it performs the type deduction using the `decltype` rules.
