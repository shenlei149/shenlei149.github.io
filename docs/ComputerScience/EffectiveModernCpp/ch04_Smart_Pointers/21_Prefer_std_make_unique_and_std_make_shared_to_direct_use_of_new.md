`std::make_shared` 是 C++11 标准引入的，不过 `std::make_unique` 是 C++14 标准引入的。实现一个不支持数组和自定义删除器的 `std::make_unique` 是很容的。
```cpp
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&...params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
```
`std::make_unique` `std::make_shared` 是三个 `make` 函数中的两个：接受任意参数，完美转发给构造函数动态的创建一个对象，然后返回指向这个对象的智能指针。和 `std::make_shared` 类似，`std::allocate_shared` 接受一个分配器。

下面通过使用和不使用 `make` 给出了使用 `make` 函数的第一个原因。
```cpp
auto upw1(std::make_unique<Widget>());    // with make func
std::unique_ptr<Widget> upw2(new Widget); // without make func
auto spw1(std::make_shared<Widget>());    // with make func
std::shared_ptr<Widget> spw2(new Widget); // without make func
```
原书高亮了 `Widget`。不使用 `make`，`Widget` 重复了两次，使用 `make` 就避免了重复。好处有避免了代码冗余。代码冗余会使得编译时间更长，目标代码冗余，并且使代码库使用更加困难。通常会演进成不一致的代码，而不一致的代码往往会出现 bug。

第二个使用 `make` 的理由是异常安全。考虑有如下函数，根据优先级来处理 `Widget`
```cpp
void processWidget(std::shared_ptr<Widget> spw, int priority);
```
按值传递 `std::shared_ptr` 看起来有点奇怪，不过 [Item 41](../ch08_Tweaks/41_Consider_pass_by_value_for_copyable_parameters_that_are_cheap_to_move_and_always_copied.md) 给出了合理的理由，如果 `processWidget` 总是复制 `std::shared_ptr`。

假定我们有一个计算优先级的函数
```cpp
int computePriority();
```
下面使用 `new` 而不是 `make` 来使用这个函数。
```cpp
processWidget(std::shared_ptr<Widget>(new Widget), // potential resource leak!
              computePriority());
```
注释中解释说，可能产生内存泄露，但是这又是如何发生的呢？

在运行时，参数必须要在调用函数之前完成求值，所以在调用 `processWidget` 之前，一定要完成三件事：
* `new Widget` 求值，在对上创建一个 `Widget` 对象
* 构造 `std::shared_ptr<Widget>` 来管理 `new` 出来的对象
* `computePriority` 必须被执行

编译器没有必要按照上述顺序进行操作。考虑下面一种情况：
1. 执行 `new Widget`
2. 执行 `computePriority`
3. 构造 `std::shared_ptr`

如果运行时 `computePriority` 抛出一个异常，而第一步 `new` 出来的对象还没有被 `std::shared_ptr` 管理，那么就会泄露内存。

如果使用 `make` 就不会有这个问题。
```cpp
processWidget(std::make_shared<Widget>(), // no potential resource leak
              computePriority());
```
运行时 `std::make_shared` 和 `computePriority` 总有一个先执行。`std::make_shared` 先执行，那么裸指针安全的存储在 `std::shared_ptr` 中，如果 `computePriority` 抛出异常，`std::shared_ptr` 的析构函数释放 `Widget` 的内存资源。如果 `computePriority` 先执行且抛出异常，`std::make_shared` 还没有调用，也就不用担心动态创建 `Widget` 而导致的内存泄露问题。

`std::shared_ptr` `std::make_shared` 替换成 `std::unique_ptr` `std::make_unique`，上述分析同样成立。

`std::make_shared` 的一个特性是相比原始的 `new` 效率提升。使用 `std::make_shared` 能够生成更小更快的代码，并使用更精简的数据结构。考虑如下直接使用 `new` 的代码
```cpp
std::shared_ptr<Widget> spw(new Widget);
```
这段代码涉及一次内存分配，但是执行了两次。第一次分配是 `new` `Widget`，第二次是 `std::shared_ptr` 的构造函数中分配内存存放控制块。

如果使用 `make`
```cpp
auto spw = std::make_shared<Widget>();
```
那么只进行一次内存分配，存放 `Widget` 和控制块，程序执行的更快。另外，`std::make_shared` 可以消除额外分配控制块的记录信息，使得占用内存更小。

针对 `std::make_shared` 的分析对 `std::allocate_shared` 也有效。

对使用 `new` 还是 `make` 的争论一直都有。这里是更倾向于使用 `make`，对于一些场景，不得不使用 `new`。

比如，`make` 函数不支持自定义删除器，但 `std::unique_ptr` `std::shared_ptr` 的构造函数支持。假定有一个自定义的删除器
```cpp
auto widgetDeleter = [](Widget *pw) {};
```
我们就不得不 `new` 对象然后构造智能指针。
```cpp
std::unique_ptr<Widget, decltype(widgetDeleter)>
    upw(new Widget, widgetDeleter);
std::shared_ptr<Widget> spw(new Widget, widgetDeleter);
```
第二个限制是由于语法限制导致的。之前分析过，如果构造函数有带 `std::initializer_list` 的，也有不带的，那么当使用大括号构造对象时，倾向于使用带 `std::initializer_list` 的构造函数，而使用小括号构造对象的时候，倾向于使用不带 `std::initializer_list` 的构造函数。`make` 函数完美转发参数给构造函数，但是不知道该使用哪一种括号构造对象。对于 `std::vector`，这个差异很大。
```cpp
auto upv = std::make_unique<std::vector<int>>(10, 20);
auto spv = std::make_shared<std::vector<int>>(10, 20);
```
是 10 个元素，每个都是 20？还是只有两个元素，分别是 10,20？

结果是确定的。由于 `make` 实现使用小括号构造对象，所以是 10 个 20。如果想使用 `std::initializer_list` 构造两个元素的对象，那么不得不使用 `new`。`make` 函数需要有完美转发初始化列表的能力，但是正如 [Item 30](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/30_Familiarize_yourself_with_perfect_forwarding_failure_cases.md) 所说，初始化列表不能被完美转发。不过 Item 30 也给出了一个变通办法：使用 `auto` 类型推导构造一个 `std::initializer_list`，然后把这个对象传递给 `make`。
```cpp
// create std::initializer_list
auto initList = {10, 20};

// create std::vector using std::initializer_list ctor
auto spv = std::make_shared<std::vector<int>>(initList);
```
对于 `std::unique_ptr`，其 `make` 函数只有这两个问题。不过对于 `std::shared_ptr` 和它的 `make` 函数，还有两个问题。

一些类自定义了 `operator new` 和 `operator delete`，那么全局性的分配和释放内存的机制对这些就不再有效了。通常，设计这些的目的是更精准的控制内存大小。比如 `Widget` 自定义了 `operator new` 和 `operator delete`，精确地分配和释放 `sizeof(Widget)` 大小的内存。此时，通过 `std::allocate_shared` 提供的 `std::shared_ptr` 对自定义分配器的支持就不能正常工作了，因为 `std::allocate_shared` 需要分配的大小比 `Widget` 的大小少大一些（控制块）。

`std::make_shared` 的速度优势在于只分配一次内存。当引用计数为零的时候，对象应该被销毁。但是直到控制块也不再需要的时候，才能释放整块内存。

控制块还包括 `std::weak_ptr` 要用到的次级引用计数，表示有几个 `std::weak_ptr` 还在指向当前对象。`std::weak_ptr` 的 `expired()` 函数需要检查引用计数来确定是否还有 `std::shared_ptr` 指向当前对象。

只要 `std::weak_ptr` 还存在，那么控制块就必须要存在，那么整块内存就不能被释放。因此，通过 `make` 分配的内存，需要在最后一个 `std::shared_ptr` 和最后一个 `std::weak_ptr` 都不再指向当前对象时才能被释放。

如果对象很大，而且最后一个 `std::shared_ptr` 和最后一个 `std::weak_ptr` 被销毁之间的时间差很大，那么销毁对象和释放内存之间就会有延迟。
```cpp
class ReallyBigType
{
};

auto pBigObj =                         // create very large
    std::make_shared<ReallyBigType>(); // object via std::make_shared

// create std::shared_ptrs and std::weak_ptrs to
// large object, use them to work with it

// final std::shared_ptr to object destroyed here,
// but std::weak_ptrs to it remain

// during this period, memory formerly occupied
// by large object remains allocated

// final std::weak_ptr to object destroyed here;
// memory for control block and object is released
```

如果使用 `new` 构造对象，那么当最后一个 ` std::shared_ptr` 被析构的时候，对象会被析构，同时，内存也会被释放。
```cpp
class ReallyBigType
{
};

// create very large object via new
std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);

// as before, create std::shared_ptrs and
// std::weak_ptrs to object, use them with it

// final std::shared_ptr to object destroyed here,
// but std::weak_ptrs to it remain;
// memory for object is deallocated

// during this period, only memory for the
// control block remains allocated

// final std::weak_ptr to object destroyed here;
// memory for control block is released
```

如果无法使用 `std::make_shared` 又想异常安全，最好的方式是在一个不做其他事情的语句中，使用 `new` 构造对象然后立即传递给智能指针的构造函数。这样编译器无法在 `new` 对象和构造智能指针之间插入可能抛出异常的语句。
```cpp
void processWidget(std::shared_ptr<Widget> spw, // as before
                   int priority);
void cusDel(Widget *ptr); // custom deleter

processWidget(                                   // as before, potential
    std::shared_ptr<Widget>(new Widget, cusDel), // resource leak!
    computePriority());

std::shared_ptr<Widget> spw(new Widget, cusDel);
processWidget(spw, computePriority()); // correct, but not optimal; see below
```
`std::shared_ptr` 获取了传递给它的裸指针的所有权，即使其构造函数抛出异常（比如无法分配控制块的内存），还是可以保证在 `new Widget` 上调用 `cusDel` 释放内存。

这里有一个很小的性能问题，在异常不安全的版本，我们传递了右值，但是在异常安全的版本中，我们传递了左值。
```cpp
processWidget(
    std::shared_ptr<Widget>(new Widget, cusDel), // arg is rvalue
    computePriority());

processWidget(spw, computePriority()); // arg is lvalue
```
`processWidget` 的 `std::shared_ptr` 参数是按值传递的。如果传递的是右值，涉及移动操作；如果传递左值，只能拷贝。对于 `std::shared_ptr` 而言，这两者是有差距的，拷贝需要原子操作来自增引用计数，而移动就不需要了。解决这个性能问题的方式是对 `spw` 使用 `std::move` 将其变成右值（参考 [Item 23](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/23_Understand_std_move_and_std_forward.md)）。
```cpp
processWidget(std::move(spw),     // both efficient and
              computePriority()); // exception safe
```

## Things to Remember
* Compared to direct use of `new`, `make` functions eliminate source code duplication, improve exception safety, and, for `std::make_shared` and `std::allocate_shared`, generate code that's smaller and faster.
* Situations where use of `make` functions is inappropriate include the need to specify custom deleters and a desire to pass braced initializers.
* For `std::shared_ptr`, additional situations where `make` functions may be ill-advised include (1) classes with custom memory management and (2) systems with memory concerns, very large objects, and `std::weak_ptr` that outlive the corresponding `std::shared_ptr`.
