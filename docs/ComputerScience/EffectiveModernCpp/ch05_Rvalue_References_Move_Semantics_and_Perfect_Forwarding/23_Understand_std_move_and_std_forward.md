从 `std::move` 和 `std::forward` 什么都不做的方面学习，有助于理解其本质。`std::move` 什么也不移动。`std::forward` 什么也不转发。它们不会编译成任何可执行代码。

`std::move` 和 `std::forward` 是转换（`cast`）函数。`std::move` 无条件将参数转成右值。`std::forward` 仅在条件满足的情况下做转换。这个解释可能带来新的问题，但是这就是这两个函数的全部内容了。

下面详细解释 `std::move`。下面是 C++11 中 `std::move` 的实现，与标准库不同，但是很接近。
```cpp
template <typename T> // in namespace std
typename remove_reference<T>::type &&
move(T &&param)
{
    using ReturnType = typename remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}
```
首先函数名是 `move`。其次，最核心的就是类型转换。给定一个对象的引用（准确的说是通用引用，详见 [Item 24](./24_Distinguish_universal_references_from_rvalue_references.md)），返回的也是该对象的引用。

函数返回类型中 `&&` 表示 `std::move` 返回右值引用，但是如 [Item 28](./28_Understand_reference_collapsing.md) 所说，如果 `T` 是左值引用，那么 `T&&` 是左值引用。为了正确性，我们使用 type traits（[Item 9](../ch03_Moving_to_Modern_C++/09_Prefer_alias_declarations_to_typedefs.md)）`std::remove_reference` 确保 `&&` 应用于非引用类型。`std::move` 最后返回右值引用，从函数中返回右值引用，那么结果类型是右值。这就是 `std::move` 所做的全部了：将参数转换成右值。

C++14 可以简化实现
```cpp
template <typename T>          // C++14; still in
decltype(auto) move(T &&param) // namespace std
{
    using ReturnType = remove_reference_t<T> &&;
    return static_cast<ReturnType>(param);
}
```

因为 `std::move` 只是将类型转化成右值，所以有人提议将名字改为 `rvalue_cast` 或者类似的东西。但是这都不重要哦，重要的是现在它的名字就是 `std::move`，记住其所作的事情就好。

右值是移动操作的候选，所以 `std::move` 就是告诉编译器这个对象更适合移动。所以叫 `std::move`。

但是，右值往往是移动操作的候选。假定有一个类，表示一段注解，其构造函数接受 `std::string` 类型参数，然后复制到类成员变量。假定使用 [Item 41](../ch08_Tweaks/41_Consider_pass_by_value_for_copyable_parameters_that_are_cheap_to_move_and_always_copied.md) 的建议，使用值传递的方式。
```cpp
class Annotation
{
public:
    // param to be copied,so per Item 41, pass by value
    explicit Annotation(std::string text);
};
```
由于我们只需要读取 `text` 的内容而不需要修改，根据传统的 `const` 应加尽加的原则，函数签名如下
```cpp
class Annotation
{
public:
    explicit Annotation(const std::string text);
};
```
为了避免一次拷贝成本，根据 [Item 41](../ch08_Tweaks/41_Consider_pass_by_value_for_copyable_parameters_that_are_cheap_to_move_and_always_copied.md) 的建议，使用 `std::move`
```cpp
class Annotation
{
public:
    explicit Annotation(const std::string text)
        : value(std::move(text)) // "move" text into value; this code
    {                            // doesn't do what it seems to!
    }

private:
    std::string value;
};
```
这段代码能编译、链接、运行。但是和预期不一致的地方是，`text` 是拷贝到 `value` 中的，而不是移动。`text` 被 `std::move` 转换成了右值，但是一开始 `text` 的类型是 `const std::string`，结果类型是 `const std::string` 的右值。`const` 属性被保留了下来。

考虑一下当编译器必须确定要调用哪个 `std::string` 构造函数时所产生的影响。
```cpp
class string
{ // std::string is actually a typedef for std::basic_string<char>
public:
    string(const string &rhs); // copy ctor
    string(string &&rhs);      // move ctor
};
```

在 `Annotation` 构造函数中，调用 `std::string` 的构造函数参数是 `std::move(text)`，是 `const std::string` 的右值。这无法使用 `std::string` 的移动构造函数，因为后者的参数是非 `const` 的 `std::string` 的右值引用。但是这个右值可以传递给拷贝构造函数，因为 `const` 的左值引用允许绑定一个 `const` 右值作为参数。即使 `text` 已经被转换成了右值，但是调用的仍旧是拷贝构造函数。这是为了符合 `const` 语义，因为移动某个对象往往意味着修改这个对象，而语言不允许传递 `const` 对象给一个可能修改它们的函数。

当希望能够移动对象的时候，不要声明为 `const`，因为移动 `const` 对象会变成拷贝操作。`std::move` 没有移动任何东西，也不保证它转换后的对象一定被移动。唯一能确定的事情是得到一个右值。

`std::forward` 与 `std::move` 类似，不过后者是是无条件转换，而前者只在条件满足的情况下转换。我们看一个 `std::forward` 的典型场景，一个模板函数，接受通用引用作为参数，然后转发给另一个函数。
```cpp
void process(const Widget &lvalArg); // process lvalues
void process(Widget &&rvalArg);      // process rvalues

template <typename T>         // template that passes
void logAndProcess(T &&param) // param to process
{
    // get current time
    auto now = std::chrono::system_clock::now();
    makeLogEntry("Calling 'process'", now);
    process(std::forward<T>(param));
}
```
下面使用这个函数，第一次参数是左值，第二次参数是右值。
```cpp
Widget w;
logAndProcess(w);            // call with lvalue
logAndProcess(std::move(w)); // call with rvalue
```
在 `logAndProcess` 函数内部，参数 `param` 传递给了函数 `process`。`process` 重载了两个实现，一个参数是左值，一个参数是右值。当调用 `logAndProcess` 时，如果传递了左值，我们期望 `process` 的左值版本被调用；如果传递了右值，很明显，希望 `process` 的右值版本被调用。

与所有函数的参数一样，`param` 是左值，那么 `logAndProcess` 总是调用 `process` 的左值版本。为了阻止此事，当传入 `logAndProcess` 的参数 `param` 是右值时，`std::forward` 将 `param` 转换成右值，调用 `process` 的右值版本。所以说 `std::forward` 是有条件转换：当参数是右值时，将其转换成右值。

`std::forward` 是如何知道参数是左值还是右值的呢？也就是它怎么判断 `param` 是左值还是右值的？简单的回答，`logAndProcess` 函数的模板参数 `T` 包含了这些信息。详见 [Item 28](./28_Understand_reference_collapsing.md)。

这两个函数都可以归纳为转换，那么是不是只是用 `std::forward` 呢？从技术角度看，回答是肯定的。`std::forward` 可以胜任所有的工作，而无需使用 `std::move`。这两个函数都不是必须的，我们可以自己手写转换，但这相当恶心。

`std::move` 的好处是方便、不易出错、更清晰。假定我们需要统计移动构造函数调用的次数，我们只需要添加一个 `static` 变量，然后在移动构造函数中自增。常见的实现如下。
```cpp
class Widget
{
public:
    Widget(Widget &&rhs)
        : s(std::move(rhs.s))
    {
        ++moveCtorCalls;
    }

private:
    static std::size_t moveCtorCalls;
    std::string s;
};
```
使用 `std::forward` 的版本如下
```cpp
class Widget
{
public:
    Widget(Widget &&rhs)                      // unconventional,
        : s(std::forward<std::string>(rhs.s)) // undesirable implementation
    {
        ++moveCtorCalls;
    }
};
```

首先，`std::move` 只需要一个函数参数 `rhs.s`，而 `std::forward` 除了函数参数 `rhs.s` 之外，还需要模板类型参数 `std::string`。其次，我们传递给 `std::forward` 的参数应该是一个右值（详见 [Item 28](./28_Understand_reference_collapsing.md)），是非引用类型。这意味 1）使用 `std::move` 少打字，不用传递右值参数的类型参数。2）减少传了不正确类型的风险，比如写成了 `std::string&` 就会导致初始化 `s` 使用拷贝构造而不是移动构造。

更重要的是，使用 `std::move` 意味着无条件转换为右值，而使用 `std::forward` 意味着只对绑定了右值的引用转换成右值，这完全是不同的操作。如它们应用场景那样，前者为了移动场景，而后者为了转发参数。正因如此，使用不同的函数（函数名）以示区别。

## Things to Remember
* `std::move` performs an unconditional cast to an rvalue. In and of itself, it doesn't move anything.
* `std::forward` casts its argument to an rvalue only if that argument is bound to an rvalue.
* Neither `std::move` nor `std::forward` do anything at runtime.
