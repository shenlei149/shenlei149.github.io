右值引用只绑定可以移动的对象。如果参数是右值引用，那么绑定的对象可能会被移动。
```cpp
class Widget
{
    // rhs definitely refers to an object eligible for moving
    Widget(Widget &&rhs);
};
```
会给这样的函数传递一个具有右值性被函数使用的对象。那么需要将绑定的对象转换成右值。[Item 23](./23_Understand_std_move_and_std_forward.md) 告诉我们，`std::move` 就是为了这个目的而设计的。
```cpp
class Widget
{
public:
    Widget(Widget &&rhs) // rhs is rvalue reference
        : name(std::move(rhs.name)),
          p(std::move(rhs.p))
    {
    }

private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};
```
[Item 24](./24_Distinguish_universal_references_from_rvalue_references.md) 告诉我们，通用引用也可以绑定一个可以移动的对象。仅当右值绑定到通用引用上的时候，通用引用需要转换为右值。[Item 23](./23_Understand_std_move_and_std_forward.md) 告诉我们，`std::forward` 就是做这个事情的。
```cpp
class Widget
{
public:
    template <typename T>
    void setName(T &&newName) // newName is universal reference
    {
        name = std::forward<T>(newName);
    }
};
```
当把右值引用传递给其他函数的时候，因为右值引用总是绑定的右值，所以使用 `std::move` 无条件的将其转换为右值；当把通用引用传递给其他函数的时候，使用 `std::forward` 在其绑定了右值的时候转换成右值。

[Item 23](./23_Understand_std_move_and_std_forward.md) 解释了使用在右值引用上可以使用 `std::forward`，但是代码冗长，容易出错，所以应该避免在右值引用上使用 `std::forward`。不过在通用引用上使用 `std::move` 问题更大，可能会不经意的改变左值（比如局部变量）。
```cpp
class Widget
{
public:
    template <typename T>
    void setName(T &&newName) // universal reference
    {                         // compiles, but is bad, bad, bad!
        name = std::move(newName);
    }

private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

std::string getWidgetName(); // factory function

Widget w;
auto n = getWidgetName(); // n is local variable
w.setName(n);             // moves n into w!
                          // n's value now unknown
```
临时变量 `n` 传入了 `w.setName`，调用者可能会认为这个函数是只读的。但是 `setName` 的内部使用 `std::move` 无条件的把引用参数转成了右值，`n` 的值被移动到了 `w.name`，在 `w.setName` 完成之后，`n` 的值不确定，这完全出乎调用者的意料。

或许你会认为 `setName` 的参数不应该声明为通用引用。根据 [Item 24](./24_Distinguish_universal_references_from_rvalue_references.md)，通用引用不应该是 `const` 的。那么可能的方案是重载，一个是 `const` 左值，一个是右值。
```cpp
class Widget
{
public:
    void setName(const std::string &newName) // set from const lvalue
    {
        name = newName;
    }
    void setName(std::string &&newName) // set from rvalue
    {
        name = std::move(newName);
    }
};
```
这可以工作，但是有缺陷。首先，维护两份代码的成本比维护一个模板要高。第二，可能会有性能问题。比如如下使用 `setName`
```cpp
w.setName("Adela Novak");
```
对于通用引用的版本而言，字面量 `"Adela Novak"` 可以传递给 `setName`，也可以传递给 `w` 内部的 `std::string` 的赋值运算符。`w` 的 `name` 字段直接通过字面量赋值，没有 `std::string` 的临时对象会被创建。但是对于后面重载版本，必须创建一个 `std::string` 临时对象，绑定到 `setName` 的参数上，这个临时对象被移动给 `w` 的字段。这就涉及 `std::string` 的构造、移动复制操作和析构。这比调用接受 `const char*` 参数类型的 `std::string` 赋值运算符耗时的多。事实就是，将通用引用参数的模板函数改成接受左值和右值引用的函数重载，某些情况下会有性能损失。如果 `Widget` 成员类型是任意类型，额外开销可能会更大，因为不是所有类型的移动操作都和 `std::string` 一样小。参考 [Item 29](./29_Assume_that_move_operations_are_not_present_not_cheap_and_not_used.md)。

重载版本引入的最重要的问题是扩展性问题。现在 `Widget::setName` 只有一个参数，那么只需要两个重载版本，如果是 $n$ 个参数呢？$2^n$ 个重载函数，指数级爆炸，不现实。更甚，一些函数有不限个参数，每一个可能是左值也可能是右值。比如 `std::make_shared` 和 `std::make_unique`：
```cpp
template <class T, class... Args>
shared_ptr<T> make_shared(Args &&...args);

template <class T, class... Args>
unique_ptr<T> make_unique(Args &&...args);
```
对于这种情况，通用引用是唯一的选择。这时，要使用 `std::forward` 传递通用引用参数给其他函数。

某些情况下，可能一开始不是这样的，需要在一个函数内多次使用绑定到右值引用或通用引用的对象，那么需要确保在其他操作完成前，不会移动这个对象。因此，最后一次使用的时候，可以使用 `std::move` 或 `std::forward`。比如
```cpp
template <typename T> // text is univ. reference
void setSignText(T &&text)
{
    sign.setText(text); // use text, but don't modify it

    // get current time
    auto now = std::chrono::system_clock::now();

    // conditionally cast text to rvalue
    signHistory.add(now, std::forward<T>(text));
}
```
我们想要确保 `text` 的内容不会被 `sign.setText` 修改，原因是这个值需要再 `signHistory.add` 中再次被使用。因此只能再最后使用 `std::forward`。

对于 `std::move` 而言，规则是一样的。不过在一些情况下，需要使用 `std::move_if_noexcept`。

在一个按值返回的函数中，返回的是右值引用或者通用引用的对象，需要对返回的引用调用 `std::move` 或 `std::forward`。考虑如下两个矩阵相加的 `operator+` 操作，左边的矩阵是右值，用于保存结果。
```cpp
Matrix // by-value return
operator+(Matrix && lhs, const Matrix & rhs)
{
    lhs += rhs;

    return std::move(lhs); // move lhs into  return value
}
```
`lhs` 转化成了右值，那么可以移动到返回值的内存中。如果不调用 `std::move`
```cpp
Matrix // as above
operator+(Matrix && lhs, const Matrix & rhs)
{
    lhs += rhs;

    return lhs; // copy lhs into return value
}
```
此时，`lhs` 是左值，那么会拷贝到返回值的内存中。如果 `Matrix` 支持移动操作，比拷贝操作更轻量，那么使用 `std::move` 就更高效。

如果 `Matrix` 不支持移动，那么也不会更差，因为右值也能用于 `Matrix` 的拷贝构造函数（[Item 23](./23_Understand_std_move_and_std_forward.md)）。如果随后 `Matrix` 支持了移动操作，那么 `operator+` 就会受益。

通用引用和 `std::forward` 也要类似使用。考虑函数模板 `reduceAndCopy` 接受一个未约化的对象 `Fraction`，约化，然后返回一个拷贝对象。如果原始对象是右值，那么可以移动到返回值中，避免拷贝，如果是左值，就不得不拷贝了。
```cpp
template <typename T>
Fraction                // by-value return
reduceAndCopy(T &&frac) // universal reference param
{
    frac.reduce();
    return std::forward<T>(frac); // move rvalue into return
}
```
如果忽略 `std::forward`，那么会无条件地拷贝返回值。

注意，上述优化仅适用于返回参数是右值引用参数或者通用引用参数，不可以用于按值返回局部变量的情况。比如
```cpp
Widget makeWidget() // "Copying" version of makeWidget
{
    Widget w; // local variable

    // configure w

    return w; // "copy" w into return value
}
```
不应该优化为
```cpp
Widget makeWidget() // Moving version of makeWidget
{
    Widget w;

    return std::move(w); // move w into return value (don't do this!)
}
```
标准委员会的人很早就意识到对于第一个版本的 `makeWidget`，可以避免拷贝局部变量 `w`，而直接在返回值的内存处构造这个对象。这就是 RVO（`return value optimization`）。

对于这种拷贝消除只在不影响软件行为的地方才可以实施。要满足两个条件：1）局部对象与函数返回值类型相同；2）局部对象就是要返回的东西。

第一个版本的 `makeWidget` 满足这两个条件，所以编译器会进行 RVO 从而避免拷贝。

而第二个版本的 `makeWidget` 不满足第二个条件，返回的已经不是局部对象 `w` 了而是对 `w` 的引用（`std::move` 的返回结果）。开发者试图帮助编译器优化，反而限制了编译器的优化。

RVO 仅仅是一个优化。万一编译器没有优化呢？那么就会拷贝吗？是不是此时就应该使用 `std::move` 呢？答案依旧是否定的。

C++ 标准规定如果满足 RVO 条件时，没有进行 RVO 优化，那么就必须视返回值为右值。因此，要么实现了 RVO 优化，避免了拷贝，要么代码与显式地写了 `std::move` 一样。不管发生那种情况，结果要么比显式地写了 `std::move` 好，要么与显式地写了 `std::move` 一样。因此，不要画蛇添足。

这种情况与按值传递参数的函数很像。不过这里无法对返回参数做拷贝消除的优化，但是如果返回的是按值传递的参数，编译器应该视为右值。因此，如下代码
```cpp
// by-value parameter of same type as function's return
Widget makeWidget(Widget w)
{
    return w;
}
```
在编译器看来与下面代码一样
```cpp
Widget makeWidget(Widget w)
{
    return std::move(w); // treat w as rvalue
}
```

## Things to Remember
* Apply `std::move` to rvalue references and `std::forward` to universal references the last time each is used.
* Do the same thing for rvalue references and universal references being returned from functions that return by value.
* Never apply `std::move` or `std::forward` to local objects if they would otherwise be eligible for the return value optimization.
