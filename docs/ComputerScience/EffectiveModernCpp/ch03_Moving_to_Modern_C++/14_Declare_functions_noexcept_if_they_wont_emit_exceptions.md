C++98 中异常详细说明异常复杂。对于一个函数，需要列出所有可能的异常，如果修改了函数，那么异常详细说明可能也需要修改。而客户端代码可能依赖这一点，导致客户端代码也不得不改变。而编译器不会保证一致性的。

C++11 时，大家一致认为一个函数是否会抛出异常是非常重要的信息。这一点构成了 C++11 异常说明的基础。在 C++11 中，无条件的 `noexcept` 确保函数不会抛出异常。

一个函数是否声明为 `noexcept` 是设计接口的事情。这也是调用者最关心的事情。调用者可以查看函数声明是否包含 `noexcept`，这关系到调用者异常安全的行为和效率。因此函数是否是 `noexcept` 和成员函数是否是 `noexcept` 一样重要。当你知道函数不会抛出异常而没有加 `noexcept` 关键字，这个接口设计的不好。

使用 `noexcept` 的一个原因是可以生成更好的目标代码。为了理解为什么，先从了解 C++11 和 C++98 指定函数不抛出异常的差别开始是个切入点。下面两个声明函数 $f$，确保用户永远不会收到一个异常。
```cpp
int f(int x) throw();  // no exceptions from f: C++98 style
int f(int x) noexcept; // no exceptions from f: C++11 style
```

如果运行时 $f$ 抛出了异常，那么违背了 $f$ 的声明。在 C++98 中，调用栈会展开到 $f$ 的调用者，然后处理一些无关事情之后退出程序。而在 C++11 中，略有差别，在程序退出前可能会展开调用栈。

这两者之前会对生成的目标代码的性能有很大的影响。在 noexcept 函数中，编译器不需要保证运行时栈处于可以展开的状态，也不需要确保 `noexcept` 函数中的对象按照构造的顺序的逆序析构。`throw()` 标记的函数没有这种优化的灵活性，没有标记 `noexcept` 的函数也一样。
```cpp
RetType function(params) noexcept; // most optimizable
RetType function(params) throw();  // less optimizable
RetType function(params);          // less optimizable
```
这个理由足够让人给不抛出异常的函数添加 `noexcept` 关键字。

还有一些函数更符合这一点，比如移动操作。假设在 C++98 标准下写了如下代码，通过 `push_back` 一遍遍的添加对象
```cpp
std::vector<Widget> vw;
Widget w;
vw.push_back(w); // add w to vw
```

这些代码能工作，所以没想着改成 C++11 的风格，但是又想借助移动语义来提高性能。首先，需要确保 `Widget` 支持移动，可以手写也可以由编译器自己合成，参见 [Item 17](./17_Understand_special_member_function_generation.md)。

当新的元素插入到 `std::vector` 的时候，`std::vector` 可能已经满了，这个时候 `std::vector` 会分配更大的内存，然后把现有元素存到新的地址，然后插入新的元素。C++98 中，这个转移操作是复制每一个元素到新的地方，然后析构旧的元素，这个过程使得 `push_back` 操作是异常安全的，即使在复制过程中抛出了异常，由于旧的元素没有被析构，`std::vector` 并没有发生变化。

C++11 中可以使用移动替代复制来提高性能，但是这违背了 `push_back` 的异常安全性。假设已经移动了 $n$ 个元素，再移动第 $n+1$ 个元素的时候发生了一场，此时 `std::vector` 已经不是原来的 `std::vector`，了，也无法恢复，因为已经有 $n$ 个元素被移动走了。

这个问题很严重，使得依赖 `push_back` 异常安全的代码无法正常工作了。所以 C++11 的实现不能悄无声息的使用移动替代复制，只有当它知道移动操作不会抛出异常的时候，才能做这个优化。

除了 `std::vector::push_back` 可以得益于“可以移动就移动，否则就复制”的策略提升性能外，很多 STL 库函数，比如 `std::vector::reserve` `std::deque::insert`，都可以提升性能。如何知道不会抛出异常呢？很简单：查看函数是否声明了 `noexcept`。

另一个绝佳的例子是 `swap`。STL 算法中大量的使用了这个函数，也常用于拷贝运算符重载中，所以将其声明为 `noexcept` 是非常必要的。有趣的是，标准库的 `swap` 是否是 `noexcept` 依赖于用户定义的 `swap` 是否是 `noexcept` 的。比如标准库中数组和 `std::pair` 的 `swap` 如下
```cpp
template <class T, size_t N>
void swap(T (&a)[N],
          T (&b)[N]) noexcept(noexcept(swap(*a, *b)));

template <class T1, class T2>
struct pair
{
    void swap(pair &p) noexcept(noexcept(swap(first, p.first)) && noexcept(swap(second, p.second)));
};
```
这个函数是有条件的 `noexcept`。比如，给定两个 `Widget` 的数组，那么 `swap` 是 `noexcept` 的前提是 `Widget` 的 `swap` 函数是 `noexcept` 的。向上一层，`Widget` 的数组的数组的 `swap` 是否是 `noexcept` 依赖于 `Widget` 的数组的 `swap`，最终依赖于`Widget` 的 `swap` 函数是 `noexcept` 的。`pair::swap` 也类似，依赖于对象是否是 `noexcept` 的。

`noexcept` 能够带来性能的提升，但是正确性更重要。如前所述，`noexcept` 是接口的一部分，那么其实现是必须要达成这一点。如果后期实现不再能保证不抛出异常，那么可以移除 `noexcept`，这会影响调用这个函数的代码，或者就让这个异常避免出现（自己处理异常往往不是好的实践）而不修改签名，亦或者是放任不管，不过这样一旦有异常抛出，程序就终止了。不管怎么，都不是好方法。

出现这种情况的原因是大部分函数都是异常中立（`exception-neutral`）的。这些函数自己不抛出异常，但是调用的函数可能抛出异常。此时，异常中立函数往往会继续抛出异常让更上层处理。这些异常中立函数不会是 `noexcept` 的，也不适合设计成 `noexcept` 的。

但是有一些函数天然的是 `noexcept` 的，比如移动操作、`swap`，这时，我们要尽力实现成 `noexcept` 的且添加关键字。

这里天然的意思是不要为了 `noexcept` 而使得函数的实现非常不自然。比如一个函数可能抛出异常（比如调用了可能抛出异常的函数），为了讨好调用者实现成 `noexcept` 的，比如处理异常转换成错误码，或者特殊返回值，这会导致实现变得复杂，而调用者使用起来也更复杂，需要检查错误码等等。这些复杂的代码导致的运行时开销可能会抵消 `noexcept` 的好处。同时，实现的复杂降低了维护性。得不偿失。

对于一些函数，不抛异常是非常重要的，默认就是 `noexcept`。C++98 中释放内存（`operator delete` `operator delete[]`）和析构函数都不能抛出异常。C++11 从函数签名上加强了这一点，不管是用户定义或者是编译器合成的释放内存和析构函数默认都是 `noexcept` 的，不需要手动添加关键字（加了也没啥，不符合约定罢了）。析构函数不能隐式的是 `noexcept` 的情况就是类的成员变量（继承的成员变量以及它们内部的成员变量）明确的声明它们的析构函数不是 `noexcept` 的（`noexcept(false)`），非常罕见。STL 里面没有。如果这些类的实例传入 STL（放到容器或者使用算法），又抛出了异常，程序的行为是未定义的。

一些类库设计者会区别宽契约（`wide contract`）和窄契约（`narrow contract`）。宽契约函数没有前置条件，不管程序状态如何，都能调用这些函数，对调用者传来的参数不做检查，同时也不会有未定义行为。

反之，函数是窄契约。这些函数会预设前置条件，如果违反了这些前置条件，函数的行为是未定义的。

如果实现了一个宽契约函数，且没有抛出异常，那么很自然的，应该加上 `noexcept` 关键字。但是对窄契约函数，就不能直接这么做了。比如一个函数 `f` 接受 `std::string` 类型的参数，自然实现下不抛出异常，那么按照建议应该声明为 `noexcept`。

现在假定 `f` 有一个前置条件：`std::string` 的长度不超过 32 个字节，如果超过了这个长度，函数的行为是未定义的。`f` 没有检查这个预设条件的义务，它假定参数是满足条件的（这个由调用者来保证）。即使这样，声明为 `noexcept` 也是合理的。
```cpp
void f(const std::string &s) noexcept; // precondition: s.length() <= 32
```

假定现在 `f` 开始检查这个前置条件了。检查不是必须的，但是也没人禁止我们这么做，并且这么做很有用，比如 debug 一个抛出异常的程序比 debug 出现未定义行为的程序要容易得多。如果做检查呢？最简单的方式就是抛出异常。不过这和之前的声明 `noexcept` 相冲突。基于这个原因，设计者往往只将宽契约函数声明为 `noexcept`。

最后，举例说明编译器检查实现和异常说明的一致性。下面的函数完全是正确的。
```cpp
void setup(); // functions defined elsewhere
void cleanup();
void doWork() noexcept
{
    setup();   // set up work to be done
               // do the actual work
    cleanup(); // perform cleanup actions
}
```

`doWork` 声明为 `noexcept`，但是调用了非 `noexcept` 的函数 `setup, cleanup`。这一点看似有点矛盾，或许在文档中写明了它们不会抛出异常。不抛出异常的函数没有声明为 `noexcept` 也有其合理性。比如是一个 C 库函数（即使一些 C 标准库函数移动到了 `std` 空间下，但是仍旧没有声明为 `noexcept`，比如 `std::strlen`，或者是 C++98 标准实现的函数，还没来得及更新到新标准。

正是由于这些合理的原因，一个 `noexcept` 可能调用非 `noexcept` 的函数，C++ 也就允许这样的代码存在，进而编译器通常也不会报告警告信息。

## Things to Remember
* `noexcept` is part of a function's interface, and that means that callers may depend on it.
* `noexcept` functions are more optimizable than non-`noexcept` functions.
* `noexcept` is particularly valuable for the move operations, `swap`, memory deallocation functions, and destructors.
* Most functions are exception-neutral rather than `noexcept`.

