假定有一个容器存放 `std::string`，当通过插入函数（`insert` `push_front` `push_back` `insert_after`）插入一个新的元素，传给函数的数据类型是 `std::string`。

不过这也不总是成立的。
```cpp
std::vector<std::string> vs; // container of std::string
vs.push_back("xyzzy");       // add string literal
```
容器存放的是 `std::string`，不过传入 `push_back` 的是字符串字面量，不是 `std::string`，即传入参数类型不是容器持有的元素的类型。

`std::vector` 的 `push_back` 对左值和右值有重载：
```cpp
template <class T,
          class Allocator = allocator<T>>
class vector
{
public:
    void push_back(const T &x); // insert lvalue
    void push_back(T &&x);      // insert rvalue
};
```
当调用
```cpp
vs.push_back("xyzzy");
```
编译器发现实际类型（`const char[6]`）和 `push_back` 的参数类型（`std::string` 的引用）不同。通过从字符串字面量创建一个临时 `std::string` 可以解决这个问题，然后将这个临时对象传递给 `push_back`。也就是说，在编译器看来这个调用是
```cpp
vs.push_back(std::string("xyzzy")); // create temp. std::string and pass it to push_back
```
代码正常工作，但是性能并不是最佳的。

为了在 `std::string` 中创建一个新的元素，必须调用 `std::string` 的构造函数。但是上述代码调用了两次，还调用了一次 `std::string` 的析构函数。下面是调用 `push_back` 运行时发生的事情。

1. 从字符串字面量 `"xyzzy"` 构造一个临时的 `std::string` 对象，该对象没有名字，这里称为 `temp` 好了。构造 `temp` 是第一次调用 `std::string` 的构造函数。因为这是一个临时对象，所以是右值。
2. `temp` 传递给接受右值引用的 `push_back` 函数，绑定到右值引用参数 `x` 上。在 `std::vector` 内存中构造一个 `x` 的副本。第二次构造是创建一个位于 `std::vector` 内的对象。由于 `x` 是右值引用，所以调用的是移动构造函数。
3. 当 `push_back` 返回，`temp` 被销毁，调用 `std::string` 的析构函数。

如果能直接将字符串字面量传入 `push_back` 然后类似第二步这样在 `std::vector` 内构造一个 `std::string`，这样就可以避免构造和销毁 `temp` 对象。这样就能性能最大化了。

这里需要调用 `emplace_back` 而不是 `push_back`。

`emplace_back` 使用传入的参数在 `std::vector` 内构造一个 `std::string`。这个过程不涉及临时对象。
```cpp
vs.emplace_back("xyzzy"); // construct std::string inside vs directly from "xyzzy"
```
`emplace_back` 使用完美转发，只要不是遇到其限制（参考 [Item](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/30_Familiarize_yourself_with_perfect_forwarding_failure_cases.md)），可以向 `emplace_back` 传入任意个数的任意类型的参数。比如可以用接受一个字符和重复次数的 `std::string` 在 `vs` 内部构造一个 `std::string` 对象。
```cpp
vs.emplace_back(50, 'x'); // insert std::string consisting of 50 'x' characters
```

支持 `push_back` 的容器都支持 `emplace_back`。类似的，有 `push_front` 的容易都支持 `emplace_front`，支持 `insert` 的容器也支持 `emplace`。关联容器提供 `emplace_hint` 作为接受 `hint` 迭代器的 `insert` 函数的补充。`std::forward_list` 有 `emplace_after`，对应于 `insert_after`。

`emplace` 接口比插入接口强的一点是更灵活。必须传入要插入的对象给插入函数，但是可以传入可以构造要插入对象的参数给 `emplace` 函数。这使得 `emplace` 可以避免构造和销毁临时对象，而插入函数无法避免这一点。

由于传入 `emplace` 函数的参数类型可以是容器持有对象的类型（函数通过调用拷贝或移动构造函数实现）,`emplace` 也可以用于对于插入函数而言也没有临时对象的场景。这种情况下，插入和 `emplace` 几乎做同样的事情。比如
```cpp
std::string queenOfDisco("Donna Summer");
```
下面的调用是合法的，并且效果也是一样的。
```cpp
vs.push_back(queenOfDisco);    // copy-construct queenOfDisco at end of vs
vs.emplace_back(queenOfDisco); // ditto
```
`emplace` 能够完成所有插入函数能做的事情。有的时候 `emplace` 更高效，理论上至少不会更低效。为什么不总是使用它呢？

因为理论上是这样的，但是实际上是有区别的。在当前标准库的实现下，是有些场景使用 `emplace` 更高效，不过也有一些场景插入函数速度更快。这些场景不容易描述，因为这取决于传递的参数类型，使用的容器类型，要插入或 `emplace` 元素的位置，元素构造的异常安全性，对不允许插入重复值的容器（`std::set` `std::mao` `std::unordered_set` `std::unordered_map`）值是否在容器中。一般建议是说通过基准测试来确定使用插入函数还是 `emplace`。

下面是一些启发式的方法来帮助确定如何选择。
1. **新增的值是构造到容器，而不是赋值** 本节开头的例子满足这一点。值本身不存在，因此新值被构造到 `std::vector`。如果考虑新的 `std::string` 被插入的位置，事情会发生变化。比如
```cpp
std::vector<std::string> vs; // as before

// add elements to vs

vs.emplace(vs.begin(), "xyzzy"); // add "xyzzy" to beginning of vs
```
对于这种情况，没有实现会在 `vs[0]` 的内存处构造被添加 `std::string`。通常会移动赋值把值放到对应位置。不过移动赋值需要一个被移动的对象，这意味需要创建一个临时对象。由于 `emplace` 主要优势就是没有临时对象的创建和销毁，因此当通过赋值来添加值的时候，`emplace` 就没有优势了。

通过构造还是赋值添加新的元素取决于实现，不过启发式的想法还是有帮助的。基于节点的容器通常使用构造的方式添加新的值，并且大部分容器都是基于节点的。只有 `std::vector` `std::deque` `std::string` 不是。`std::array` 也不是，但是不支持插入或 `emplace`，与这里讨论无关。对于不是基于节点的容器，使用 `emplace_back` 来使用构造而不是赋值向容器添加元素，对于 `std::deque`，使用 `emplace_front` 也是一样的。

2. **传入的参数类型与容器持有的类型不同** 如果需要新添的对象类型是 `T`，容器是 `container<T>`，没有理由期望 `emplace` 比插入更快，因为使用插入接口也没有临时对象的创建。
3. **容器大概率不需要拒绝插入重复值** 这意味容器要么允许重复值的插入，要么大部分值都是唯一的。为了检测值是否在容器内，`emplace` 实现往往是创建一个新的节点，然后与容器内已有的节点进行比较。如果不存在则添加新节点。否则，`emplace` 取消操作，销毁新节点，这意味创建和销毁新节点的开销都浪费了。相比插入函数而言，这类节点的创建更多是为了 `emplace`。

之前的两个例子都满足上述条件，所以 `emplace_back` 都会运行的更快。
```cpp
vs.emplace_back("xyzzy"); // construct new value at end of container; don't pass the type in
                          // container; don't use container rejecting duplicates
vs.emplace_back(50, 'x'); // ditto
```
当决定是否使用 `emplace` 函数，还有两个点需要考虑。第一个是资源管理。假定有持有 `std::shared_ptr<Widget>` 的容器
```cpp
std::list<std::shared_ptr<Widget>> ptrs;
```
并且我们希望通过 `std::shared_ptr` 自定义删除器。一旦有了自定义删除器，我们无法通过 `make_shared` 来创建 `std::shared_ptr`，只能先 `new` 一个裸指针然后交给 `std::shared_ptr` 管理。

假定自定义删除器是下面的函数
```cpp
void killWidget(Widget *pWidget);
```
那么使用插入函数的代码如下
```cpp
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({new Widget, killWidget});
```
两种方式都可以，都有一个临时的 `std::shared_ptr` 的对象在调用 `push_back` 之前被创建。因为 `push_back` 的参数是 `std::shared_ptr` 的引用，所以必须要有一个 `std::shared_ptr` 引用指向的实际对象。

这个例子中，使用 `emplace_back` 可以避免临时 `std::shared_ptr` 被创建出来。不过这个例子中，临时对象创建的意义远大于其开销。考虑如下场景。

1. `new Widget` 创建了一个裸指针指向的对象，然后创建临时对象 `std::shared_ptr<Widget>` 持有这个裸指针。这个临时对象称为 `temp`，
2. `push_back` 接受 `temp` 的引用。在分配一个临时节点来持有 `temp` 副本的时候，OOM 了。
3. 异常从 `push_back` 中出来，`temp` 对象被销毁。唯一管理 `Widget` 的 `std::shared_ptr` 对象会负责调用 `killWidget` 销毁这个 `Widget` 对象。

异常发生了，但是没有泄露任何资源。如果使用 `emplace_back` 替代 `push_back`，就会有问题。
```cpp
ptrs.emplace_back(new Widget, killWidget);
```

1. `new Widget` 创建的指针被完美转发，然后创建一个 `list` 节点准备持有该裸指针，但此时 OOM 了。
2. 异常从 `emplace_back` 中出来，那个在栈上的裸指针是唯一访问 `Widget` 对象的途径，此时消失了，那个 `Widget` 及其持有的资源泄露了。

这种情况下，对象的生命周期处理问题，但是不是 `std::shared_ptr` 的错。有自定义删除器的 `std::unique_ptr` 也会有这个问题。类似 `std::shared_ptr` `std::unique_ptr` 资源管理的有效性的基础是自愿（比如 `new` 的裸指针）要立即传给资源管理对象。这也是 `std::make_shared` `std::make_unique` 这类函数自动做这些事情的原因。

利用资源管理对象的容器（比如 `std::list<std::shared_ptr<Widget>>)`）插入函数，函数的参数确保了在获取资源（比如 `new`）和构造管理资源的对象之间没有其他操作。在 `emplace` 中，完美转发延迟了资源管理对象的创建，即当容器中有内存的时候再构造，这就为发生异常导致内存泄露开了一个窗。所有的标准容器都会有这个问题。所有当使用资源管理对象的容器，选择 `emplace` 而不是插入函数，要确保不能为了性能而损失了异常安全。

直白地说，我们不应该传入诸如 `new Widget` 到 `emplace_back` 或 `push_back` 或其他函数，这可能会导致异常安全问题。解决方法是使用独立的语句将 `new Widget` 的指针放到资源管理对象中，然后传递右值到你想传入 `new Widget` 的函数。下面是 `push_back` 和 `emplace_back` 的例子。
```cpp
std::shared_ptr<Widget> spw(new Widget,  // create Widget and
                            killWidget); // have spw manage it
ptrs.push_back(std::move(spw));          // add spw as rvalue

std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.emplace_back(std::move(spw));
```
这两种方法都会有创建和销毁 `spw` 的成本。为了保证资源创建和转移给资源管理对象之间没有任何操作，这个开销是必须付出的。因此 `emplace` 就不会比插入快了。

第二个值得讨论的是 `emplace` 与 `explicit` 构造函数交互。C++11 已经支持了正则表达式，假设创建了一个包含正则表达式的容器
```cpp
std::vector<std::regex> regexes;
```
由于分心写了如下代码
```cpp
regexes.emplace_back(nullptr); // add nullptr to container of regexes?
```
编译器不会报错。不小心创建了一个 `nullptr` 指针表示的正在表达式，这是如何成功的呢？尝试写如下代码
```ccp
std::regex r = nullptr; // error! won't compile
```
如果使用 `push_back` 而不是 `emplace_back`，编译器也会报错。
```cpp
regexes.push_back(nullptr); // error! won't compile
```
可能的原因是可以使用字符串来创建 `std::regex` 对象。如下代码是合法的。
```cpp
std::regex upperCaseWord("[A-Z]+");
```
从字符串构造 `std::regex` 是相对耗时的事情，为了降低意外出现这种开销的可能性，接受 `const char *` 的 `std::regex` 构造函数是 `explicit`。这就是什么下面的代码无法编译。
```cpp
std::regex r = nullptr;     // error! won't compile
regexes.push_back(nullptr); // error! won't compile
```
这两种情况都需要将指针转化为 `std::regex`，但是 `explicit` 使得这种隐式转化被禁止。

当调用 `emplace_back` 的时候，没有声明要传入 `std::regex` 对象，传入的参数是能够构造 `std::regex` 的参数。无需考虑隐式转化。下面的代码是合法的。
```cpp
std::regex r(nullptr); // compiles
```
上述代码能够编译，但是是未定义行为。接受 `const char *` 的 `std::regex` 构造函数需要指向的字符串是一个合法的正则表达式，但是空指针不是。

我们先讨论如下类似的代码有着不同的结果。
```cpp
std::regex r1 = nullptr; // error! won't compile
std::regex r2(nullptr);  // compiles
```
使用标准术语说，用于 `r1` 的初始化语法是拷贝初始化。初始化 `r2` 的语法（小括号，大括号也适用）是直接初始化。拷贝初始化不允许使用 `explicit` 构造函数，但是直接初始化可以用。这就是为什么第一个不能编译而第二个可以。

回到 `push_back` 和 `emplace_back`，`emplace` 使用直接初始化，可以用 `explicit` 构造函数，而插入函数使用拷贝初始化，不能用。因此
```cpp
regexes.emplace_back(nullptr); // compiles. Direct init permits use of explicit std::regex
                               // ctor taking a pointer

regexes.push_back(nullptr); // error! copy init forbids use of that ctor
```
这里想说明的是如果使用 `emplace` 函数，要小心确保传入正确的参数，因为编译器会考虑 `explicit` 来寻找一种合理的方式解释代码。

## Things to Remember
* In principle, emplacement functions should sometimes be more efficient than their insertion counterparts, and they should never be less efficient.
* In practice, they're most likely to be faster when (1) the value being added is constructed into the container, not assigned; (2) the argument type(s) passed differ from the type held by the container; and (3) the container won't reject the value being added due to it being a duplicate.
* Emplacement functions may perform type conversions that would be rejected by insertion functions.
