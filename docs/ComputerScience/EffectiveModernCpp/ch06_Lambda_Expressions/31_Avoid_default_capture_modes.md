C++11 中有两种默认捕获模式：按值和按引用。默认按引用捕获会导致悬垂引用的问题。默认按值捕获会让你认为问题不存在了（其实并没有），还会导致你认为闭包是自包含的（其实也不是）。

这个条款就来讲解这些问题。首先，从按引用捕获开始。

按引用捕获使得一个闭包引用了一个局部变量或者参数，只在闭包作用域内有定义。如果闭包的声明周期比局部变量或者参数的长，那么就会有悬垂引用的问题。假定我们有一个过滤函数的容器，每一个过滤函数接受一个 `int` 参数返回 `bool` 类型表示是否满足过滤器的条件。
```cpp
using FilterContainer =                    // see Item 9 for "using",
    std::vector<std::function<bool(int)>>; // Item 5 for std::function

FilterContainer filters; // filtering funcs
```
我们添加一个过滤器得到 5 的倍数。
```cpp
filters.emplace_back( // see Item 42 for info on emplace_back
    [](int value)
    { return value % 5 == 0; });
```
被除数有可能是运行时计算出来的，不能在 lambda 中硬编码 5，代码看起来如下
```cpp
void addDivisorFilter()
{
    auto calc1 = computeSomeValue1();
    auto calc2 = computeSomeValue2();
    auto divisor = computeDivisor(calc1, calc2);

    filters.emplace_back( // danger! ref to divisor will dangle!
        [&](int value)
        { return value % divisor == 0; });
}
```
lambda 引用了局部变量 `divisor`，但是当函数 `addDivisorFilter` 退出时就不再有效了。添加到 `filters` 中的函数也不再有效了，接着使用过滤器就会导致未定义行为。

如果显式的按引用捕获，问题还是存在的。
```cpp
filters.emplace_back(
    [&divisor](int value) // danger! ref to divisor will still dangle!
    { return value % divisor == 0; });
```
但是显式捕获的好处是更容易知道 lambda 依赖于 `divisor` 的生命周期。明确的写出 `divisor`，提醒我们要确保 `divisor` 的生命周期长于 lambda 闭包。比起 `[&]`，更容易让人想起要注意悬垂引用这回事。

如果你知道一个闭包立即要使用，也不会被拷贝，那么没有闭包比引用的变量生命周期更长的问题。这种情况下，你可能会说，没有悬垂引用的风险，没有理由不使用默认按引用捕获模式。比如，我们的过滤 lambda 表示是用于 `std::all_of` 函数。
```cpp
template <typename C>
void workWithContainer(const C &container)
{
    auto calc1 = computeSomeValue1();            // as above
    auto calc2 = computeSomeValue2();            // as above
    auto divisor = computeDivisor(calc1, calc2); // as above

    // type of elements in container
    using ContElemT = typename C::value_type;

    using std::begin; // for genericity;
    using std::end;   // see Item 13

    if (std::all_of(                          // if all values
            begin(container), end(container), // in container
            [&](const ContElemT &value)       // are multiples
            { return value % divisor == 0; }) // of divisor...
    )
    {
        // they are...
    }
    else
    {
        // at least one isn't...
    }
}
```
这确实是安全的，不过也有不确定性。假定 lambda 在其他上下文中也很有用，比如加到 `filters` 中，然后拷贝这段 lambda 到其他上下文，可能会导致 `divisor` 的生命周期比闭包短，然后又出现悬垂引用的问题，而这次没有一个显式的声明提醒你要注意 `divisor` 的声明周期。

长期来看，指定 lambda 要捕获的变量，是更好的软件工程实践。

另外，C++14 允许在 lambda 中使用 `auto`，那么 `ContElemT` 也就不需要了。上面代码中的 `if` 可以简写为
```cpp
if (std::all_of(begin(container), end(container),
                [&](const auto &value) // C++14
                { return value % divisor == 0; }))
```
解决 `divisor` 的一种方式是默认按值传递。那么添加到 `filters` 中的 lambda 如下。
```cpp
filters.emplace_back( // now divisor can't dangle
    [=](int value)
    { return value % divisor == 0; });
```
对于这个例子而言，完全没有问题。但是默认按值传递并不会使悬垂问题得到解决。比如这里按值捕获指针，那么向闭包里面拷贝了一个指针，但是你无法阻止 lambda 外面 `delete` 这个指针导致拷贝的指针无效。

你也许会说，我读了[第四章](../ch04_Smart_Pointers/README.md)只使用智能指针不使用裸指针和 `delete`。这也许是对的，但是与当前问题无关。事实上，还是会涉及裸指针，并且会被 `delete`。只是由于现代 C++ 编程风格导致整个问题没有暴露在源代码中。

假定我们有一个 `Widget` 可以添加元素到过滤器的容器中。
```cpp
class Widget
{
public:
    // ctors, etc.

    void addFilter() const; // add an entry to filters
private:
    int divisor; // used in Widget's filter
};
```
`Widget::addFilter` 的实现如下
```cpp
void Widget::addFilter() const
{
    filters.emplace_back(
        [=](int value)
        { return value % divisor == 0; });
}
```
对于缺少经验的人看来，这段代码是安全的。lambda 依赖于 `divisor`，但是按值传递，拷贝了 `divisor` 到闭包中。

这完全是错误认识。

闭包只能捕获 lambda 创建上下文中的非 `static` 的局部变量和参数。在 `Widget::addFilter` 内，`divisor` 不是局部变量，是 `Widget` 类的成员变量。不能被捕捉。如果将默认按值捕获的代码去掉，会编译出错。
```cpp
void Widget::addFilter() const
{
    filters.emplace_back( // error! divisor
        [](int value)     // not available
        { return value % divisor == 0; });
}
```
如果显式捕获 `divisor`，不管是按值或是按引用，也无法编译，因为 `divisor` 不是局部变量或参数。
```cpp
void Widget::addFilter() const
{
    filters.emplace_back(    // error! no local
        [divisor](int value) // divisor to capture
        { return value % divisor == 0; });
}
```
如果默认按值无法捕获 `divisor`，lambda 不按值捕获就无法编译。究竟是怎么回事呢？

这里隐含了一个裸指针：`this`！每一个非 `static` 的成员函数都有一个 `this` 指针，当使用成员变量的时候，就在使用这个指针。在 `Widget` 成员函数内部，编译器就将 `divisor` 替换成 `this->divisor`，那么默认按值捕获的版本实际上捕获了 `Widget` 的 `this` 指针，而不是 `divisor`。也就是说，在编译器看来，实际代码大致如下
```cpp
void Widget::addFilter() const
{
    auto currentObjectPtr = this;

    filters.emplace_back(
        [currentObjectPtr](int value)
        { return value % currentObjectPtr->divisor == 0; });
}
```
理解了这一点，就会明白闭包的生命周期与 `Widget` 对象的关系，闭包内包含一个指向 `Widget` 的 `this` 指针的拷贝。下面代码只是用智能指针。
```cpp
using FilterContainer = // as before
    std::vector<std::function<bool(int)>>;
FilterContainer filters; // as before

void doSomeWork()
{ // create Widget; see Item 21 for std::make_unique
    auto pw = std::make_unique<Widget>();

    pw->addFilter(); // add filter that uses Widget::divisor

} // destroy Widget; filters now holds dangling pointer!
```
当 `doSomeWork` 被调用，创建了一个依赖于 `Widget` 的过滤器，`Widget` 由 `std::make_unique` 创建出来的，而过滤器有一个指向 `Widget` 对象的裸指针的拷贝。当 `doSomeWork` 结束的时候，由 `std::make_unique` 管理的 `Widget` 对象会被销毁，那么 `filters` 中有一个元素就包含了悬垂指针。

这个特殊的问题可以拷贝一份成员变量，然后 lambda 捕获这个拷贝。
```cpp
void Widget::addFilter() const
{
    auto divisorCopy = divisor; // copy data member
    filters.emplace_back(
        [divisorCopy](int value)             // capture the copy
        { return value % divisorCopy == 0; } // use the copy
    );
}
```
此时，使用默认按值捕获也是可以的。
```cpp
void Widget::addFilter() const
{
    auto divisorCopy = divisor; // copy data member
    filters.emplace_back(
        [=](int value)                       // capture the copy
        { return value % divisorCopy == 0; } // use the copy
    );
}
```
但是何必冒险呢？一开始我们认为按值捕获了 `divisor`，但是不小心捕获的是 `this`。

C++14 中，更好的做法是采用通用 lambda 捕获（参见 [Item 32](./32_Use_init_capture_to_move_objects_into_closures.md)）。
```cpp
void Widget::addFilter() const
{
    filters.emplace_back(                // C++14:
        [divisor = divisor](int value)   // copy divisor to closure
        { return value % divisor == 0; } // use the copy
    );
}
```
对通用捕获而言，就没有默认捕获模式的问题了。

按值捕获的另一个缺点是让人以为闭包是自包含的，与闭包外的值更新无关。一般而言这个说法是不对的，因为 lambda 不仅仅依赖于局部变量和参数，也会依赖静态存储周期（`static storage duration`）的对象。这些对象是定义在全局或命名空间下，亦或者是在内类、函数内、文件内部声明为 `static` 的变量。lambda 能用这些变量，但是不能被捕获。按值捕获会误导以为捕获了这些变量。看下面的例子。
```cpp
void addDivisorFilter()
{
    static auto calc1 = computeSomeValue1();            // now static
    static auto calc2 = computeSomeValue2();            // now static
    static auto divisor = computeDivisor(calc1, calc2); // now static

    filters.emplace_back(
        [=](int value)                   // captures nothing!
        { return value % divisor == 0; } // refers to above static
    );

    ++divisor; // modify divisor
}
```
`[=]` 会让人以为闭包已经捕获了需要的变量的拷贝，是自包含的了。其实不然。这个 lambda 没有使用任何非 `static` 的局部变量，所以什么也没有捕获，而是引用了 `static` 变量 `divisor`。每次调用 `addDivisorFilter` 都会使 `divisor` 自增，那么添加到 `filters` 的过滤器行为一直在变化，因为相应的 `divisor` 变化了。事实上，lambda 捕获的是 `divisor` 的引用，这与默认按值捕获矛盾。如果一开始就避免使用默认按值捕获，就能避免这些陷阱。

## Things to Remember
* Default by-reference capture can lead to dangling references.
* Default by-value capture is susceptible to dangling pointers (especially `this`), and it misleadingly suggests that lambdas are self-contained.
