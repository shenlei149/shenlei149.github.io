C++98 中的异常说明非常复杂。对于一个函数，需要列出所有可能抛出的异常；如果修改了函数，异常说明可能也需要修改。而客户端代码可能依赖这一说明，导致客户端代码也不得不修改。编译器不会保证二者的一致性。

C++11 中，大家一致认为一个函数是否会抛出异常是非常重要的信息。这一点构成了 C++11 异常说明的基础。在 C++11 中，无条件的 `noexcept` 表明函数不会抛出异常。

函数是否声明为 `noexcept` 是接口设计的一部分，也是调用者最关心的事情。调用者可以查看函数声明是否包含 `noexcept`，因为这关系到调用者的异常安全性和效率。因此，函数是否为 `noexcept` 与成员函数是否为 `noexcept` 同样重要。当你知道函数不会抛出异常却没有加上 `noexcept` 关键字时，这个接口设计得并不好。

使用 `noexcept` 的一个原因是它可以生成更好的目标代码。要理解其中的原因，不妨先了解 C++11 和 C++98 指定函数不抛出异常的差别。下面两个声明都表示函数 $f$ 不会向调用者传播异常。
```cpp
int f(int x) throw();  // no exceptions from f: C++98 style
int f(int x) noexcept; // no exceptions from f: C++11 style
```

如果运行时 $f$ 抛出了异常，就违背了 $f$ 的声明。在 C++98 中，调用栈可能会展开到 $f$ 的调用者；在处理一些额外事务后，程序将终止。而在 C++11 中，程序会在不展开调用栈的情况下终止。

这两者之间的差别会对生成的目标代码的性能产生很大影响。在 `noexcept` 函数中，编译器不需要保证运行时栈处于可展开的状态；发生异常时，也不需要确保其中的对象按构造顺序的逆序析构。用 `throw()` 标记的函数没有这种优化的灵活性；未标记为 `noexcept` 的函数也是如此。
```cpp
RetType function(params) noexcept; // most optimizable
RetType function(params) throw();  // less optimizable
RetType function(params);          // less optimizable
```
这个理由足以让人给不会抛出异常的函数添加 `noexcept` 关键字。

还有一些函数尤其符合这一点，比如移动操作。假设在 C++98 标准下写了如下代码，通过 `push_back` 逐个添加对象：
```cpp
std::vector<Widget> vw;
Widget w;
vw.push_back(w); // add w to vw
```

这些代码能够工作，因此不想改成 C++11 风格，但又想借助移动语义提高性能。首先，需要确保 `Widget` 支持移动：可以手写移动操作，也可以由编译器合成，参见 [Item 17](./17_Understand_special_member_function_generation.md)。

插入新元素时，`std::vector` 可能已经没有足够的容量。这时，`std::vector` 会分配更大的内存，将现有元素复制到新地址，再插入新元素。C++98 中，这个转移操作会复制每个元素，然后析构旧元素。该过程使得 `push_back` 具有异常安全性：即使复制过程中抛出异常，由于旧元素尚未析构，`std::vector` 也没有发生变化。

C++11 中可以使用移动替代复制来提高性能，但这可能违背 `push_back` 的异常安全性。假设已经移动了 $n$ 个元素，再移动第 $n+1$ 个元素时发生了异常，此时 `std::vector` 已不再是原来的 `std::vector`，也无法恢复，因为已有 $n$ 个元素被移走。

这个问题很严重，会使依赖 `push_back` 异常安全性的代码无法正常工作。因此，C++11 的实现不能悄无声息地用可能抛出异常的移动操作替代复制；通常只有当它知道移动操作不会抛出异常时，才能安全地进行这一优化。若元素不可复制，实现没有其他选择，只能使用移动操作，此时异常安全保证可能会降低。

除了 `std::vector::push_back` 可以通过“可以移动就移动，否则就复制”的策略提升性能外，许多 STL 函数（如 `std::vector::reserve` 和 `std::deque::insert`）也能从中受益。如何知道移动操作不会抛出异常呢？很简单：查看函数是否声明为 `noexcept`。

另一个绝佳的例子是 `swap`。STL 算法大量使用该函数，它也常用于拷贝赋值运算符的实现中，因此将其声明为 `noexcept` 非常必要。有趣的是，标准库 `swap` 是否为 `noexcept` 取决于用户定义的 `swap` 是否为 `noexcept`。例如，标准库中数组和 `std::pair` 的 `swap` 如下：
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
这些函数带有条件 `noexcept`。例如，给定两个 `Widget` 数组，数组的 `swap` 为 `noexcept` 的前提是 `Widget` 的 `swap` 为 `noexcept`。对于 `Widget` 的二维数组，其 `swap` 是否为 `noexcept` 又取决于一维数组的 `swap`，最终仍取决于 `Widget` 的 `swap` 是否为 `noexcept`。`pair::swap` 也类似：它取决于两个成员的 `swap` 是否为 `noexcept`。

`noexcept` 能够提升性能，但正确性更重要。如前所述，`noexcept` 是接口的一部分，因此其实现必须满足这一承诺。如果后续实现无法再保证不抛出异常，可以移除 `noexcept`，但这会影响调用该函数的代码；也可以在不修改签名的情况下避免异常（自行处理异常往往不是好实践）；还可以放任不管，不过一旦抛出异常，程序就会终止。无论哪种方法都不理想。

出现这种情况的原因是，大部分函数都是异常中立（`exception-neutral`）的。这些函数本身不抛出异常，但它们调用的函数可能抛出异常。此时，异常中立函数通常会让异常继续传播，由更上层处理。这些异常中立函数不会是 `noexcept`，也不适合设计成 `noexcept`。

但是，有一些函数天然适合成为 `noexcept`，比如移动操作和 `swap`。这时，我们应尽力将其实现为 `noexcept`，并添加该关键字。

这里“天然”的意思是，不要为了 `noexcept` 而让函数的实现变得非常不自然。例如，函数可能抛出异常（如调用了可能抛出异常的函数），若为了讨好调用者而将其实现为 `noexcept`，例如通过捕获异常并转换为错误码或特殊返回值，就会使实现变得复杂，调用者也需要检查错误码等。这些复杂代码导致的运行时开销可能抵消 `noexcept` 的好处，同时也会降低可维护性，得不偿失。

对于一些函数，不抛出异常非常重要，它们默认就是 `noexcept`。C++98 中，内存释放函数（`operator delete` 和 `operator delete[]`）以及析构函数都不能抛出异常。C++11 从函数签名上加强了这一点：无论是用户定义还是编译器合成的内存释放函数和析构函数，默认都是 `noexcept`，无需手动添加该关键字（添加也无妨，只是不符合惯例）。析构函数不会隐式为 `noexcept` 的情形是：类的成员、基类成员及其内部成员明确将析构函数声明为非 `noexcept`（`noexcept(false)`）。这种情况非常罕见，STL 中没有。如果这些类的实例被传给 STL（放入容器或用于算法）且析构函数抛出异常，程序的行为将是未定义的。

一些类库设计者会区分宽契约（`wide contract`）和窄契约（`narrow contract`）。宽契约函数没有前置条件；无论程序状态或调用者传入的参数如何，都可以调用这些函数，且不会因此产生未定义行为。

反之，窄契约函数预设了前置条件；如果违反这些前置条件，函数的行为就是未定义的。

如果实现了一个宽契约函数，且它不会抛出异常，那么很自然，应加上 `noexcept` 关键字。但是，对窄契约函数则不能直接这么做。例如，函数 `f` 接受 `std::string` 类型的参数，其自然实现不会抛出异常，按照前述建议应将其声明为 `noexcept`。

现在假定 `f` 有一个前置条件：`std::string` 的长度不超过 32 个字节。如果超过该长度，函数的行为就是未定义的。`f` 没有检查这个前置条件的义务；它假定参数满足条件（由调用者保证）。即使如此，将其声明为 `noexcept` 仍是合理的。
```cpp
void f(const std::string &s) noexcept; // precondition: s.length() <= 32
```

假定现在 `f` 开始检查这个前置条件。检查并非必须，但也没人禁止这样做，而且这样做很有用：例如，调试抛出异常的程序比调试出现未定义行为的程序容易得多。如何进行检查呢？最简单的方式就是抛出异常。不过，这与先前的 `noexcept` 声明相冲突。基于这个原因，设计者往往只将宽契约函数声明为 `noexcept`。

最后，举例说明编译器不会检查实现与异常说明的一致性。下面的函数完全正确。
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

`doWork` 声明为 `noexcept`，但调用了未声明为 `noexcept` 的函数 `setup` 和 `cleanup`。这看似有些矛盾，但或许文档已说明它们不会抛出异常。不抛出异常的函数未声明为 `noexcept` 也有合理的原因。例如，它可能是 C 库函数；即使一些 C 标准库函数被置于 `std` 命名空间中，它们仍可能未声明为 `noexcept`，如 `std::strlen`。又或者，它是按 C++98 标准实现的函数，尚未来得及更新到新标准。

正是由于这些合理的原因，`noexcept` 函数可能调用未声明为 `noexcept` 的函数。C++ 因而允许这样的代码存在，编译器通常也不会报告警告。

## Things to Remember
* `noexcept` is part of a function's interface, and that means that callers may depend on it.
* `noexcept` functions are more optimizable than non-`noexcept` functions.
* `noexcept` is particularly valuable for the move operations, `swap`, memory deallocation functions, and destructors.
* Most functions are exception-neutral rather than `noexcept`.
