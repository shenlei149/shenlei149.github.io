先看一个非常简单的例子。
```cpp
int x;
```
忘记初始化了。

再看下面的例子，解引用一个迭代器来初始化一个局部变量。
```cpp
template<typename It>       // algorithm to dwim ("do what I mean")
void dwim(It b, It e)       // for all elements in range from
{ // b to e
    while (b != e) {
        typename std::iterator_traits<It>::value_type currValue = *b;
    }
}
```

类型 `typename std::iterator_traits<It>::value_type` 之长，让写代码的乐趣少了很多。

第三个例子是想声明一个类型为闭包的局部变量，但是闭包的类型只有编译器知道，写不出来。。。这也是为什么闭包和 `auto` 同时出现在 C++11 的原因，只有闭包而不引入 `auto` 就没法写代码了。

`auto` 能够很好的解决这三个问题。

首先 `auto` 从初始化语句推导类型，这样就不会再忘记初始化变量了。
```cpp
int x1;         // potentially uninitialized
auto x2;        // error! initializer required
auto x3 = 0;    // fine, x's value is well-defined
```

`auto` 可以替代长长的类型名。
```cpp
template<typename It>       // as before
void dwim(It b, It e)
{
    while (b != e) {
        auto currValue = *b;
    }
}
```

由于 `auto` 使用 [Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 中解释的推导原则，可以表示只有编译器知道的类型，所以可以写闭包了。
```cpp
auto derefUPLess =                          // comparison func.
    [](const std::unique_ptr<Widget>& p1,   // for Widgets
       const std::unique_ptr<Widget>& p2)   // pointed to by
    { return *p1 < *p2; };                  // std::unique_ptrs
```

C++14 中，允许闭包参数用 `auto`，那么可以进一步简化写法。
```cpp
auto derefLess =            // C++14 comparison
    [](const auto& p1,      // function for
       const auto& p2)      // values pointed
    { return *p1 < *p2; };  // to by anything
                            // pointer-like
```

有些人可能会有疑问，这里并不一定要使用闭包，可以用 `std::function ` 替代。我们下面就解释一下两者之间的区别和联系。

C++11 引入的 `std::function` 可以理解成函数指针，不过函数指针只能指向函数，但是 `std::function` 可以存一些可以调用的对象。就像使用函数指针必须指定类型一样，使用 `std::function` 也必须指定类型。比如为了定义一个 `std::function` 对象，其指向如下签名的可调用对象。
```cpp
bool(const std::unique_ptr<Widget>&,    // C++11 signature for std::unique_ptr<Widget>
     const std::unique_ptr<Widget>&)    // comparison function
```

其变量名是 `func`
```cpp
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)> func;
```

因为 lambda 返回的是可调用对象，所以闭包可以存到 `std::function` 对象中。如果不用 `auto`，就必须写成
```cpp
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)>
    derefUPLess = [](const std::unique_ptr<Widget>& p1,
                     const std::unique_ptr<Widget>& p2)
                    { return *p1 < *p2; };
```

除了语法上的冗余以外，`std::function` 和 `auto` 并不完全一样。`auto` 声明的闭包对象和闭包同类型，内存占用也和闭包所需内存一样。`std::function` 模板类实例化的过程，会占用固定大小的内存，如果无法放下闭包，那么会从堆上分配内存。所以通常 `std::function` 比 `auto` 占用内存要高。由于内联和间接调用函数返回等限制，``std::function` 比 `auto` 要慢。再加上 `auto` 写法简洁，`auto` 往往是更好的选择。（类似的，为了存储函数返回的结果，lambda 也比 `std::bind` 要好很多，详见 [Item 34](../ch06_Lambda_Expressions/34_Prefer_lambdas_to_std_bind.md)。

`auto` 还能避免类型错误的问题。比如
```cpp
std::vector<int> v;
unsigned sz = v.size();
```

很少有开发者意识到 `v.size()` 的返回类型是 `std::vector<int>::size_type`。这里赋值的类型是 `unsigned`，对于大部分情况，这足够用了。但是 64 为系统上存在大小被限制的情况。使用 `auto` 就解决了。
```cpp
auto sz = v.size();     // sz's type is std::vector<int>::size_type
```

下面再看一个例子。
```cpp
std::unordered_map<std::string, int> m;
for (const std::pair<std::string, int>& p : m)
{
    // do something with p
}
```

看起来很好，但是也有问题。

`std::unordered_map ` 的键的类型是 `const` 的，所以其中的 `std::pair` 类型是 `std::pair<const std::string, int>` 而不是 `std::pair<std::string, int>`。上述写法会导致调用拷贝构造函数构造一个新的对象，然后 `p` 是指向新对象的引用，`for` 结束后这个临时对象被销毁。这明显不符合预期，我们只是想简单的让 `p` 绑定到 `m` 中的每一个对象上。使用 `auto` 就没有这个问题了。
```cpp
for (const auto& p : m)
{
    // as before
}
```

新的代码更高效也更简洁。同时，如果获取 `p` 的指针，那么就是指向 `m` 内的某一个元素的。

有很多使用 `auto` 的理由，不过它也不是完美的。`auto` 根据初始化语句进行推导，有的时候这个结果并不符合预期，也不是我们想要的。Items [2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) and [6](./06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md) 有描述，这里不再赘述。

`auto` 是可选的而不是强制的，出于可维护性或者代码整洁等原因，可以显式地写类型。类型推导并不是 C++ 原创，很多语言都有类似功能，同时，社区也积攒了很多经验，已经证实类型推导和大型软件工程的可维护性不矛盾。

使用 `auto` 可能无法直接获取其真实类型，这一点可以借助 IDE 的帮助。我们很多时候并不需要知道详细类型，只要知道是容器、智能指针等基本信息就足够了，配合好的命名规则，这些抽象类型信息也能得到。

显式地写类型可能会有潜在的问题，比如执行效率和正确性。另外，使用 `auto` 能够方便的重构。比如一个函数返回 `int`，我们需要修改成 `long`，如果使用 `auto` 声明用函数返回值初始化的变量，那么就不需要修改调用的地方了。

## Things to Remember
* `auto` variables must be initialized, are generally immune to type mismatches that can lead to portability or efficiency problems, can ease the process ofrefactoring, and typically require less typing than variables with explicitly specified types.
* `auto`-typed variables are subject to the pitfalls described in Items [2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) and [6](./06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md).

