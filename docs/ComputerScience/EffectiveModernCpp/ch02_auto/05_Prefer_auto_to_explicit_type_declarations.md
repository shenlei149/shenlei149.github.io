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

第三个例子是想声明一个闭包类型的局部变量，但是闭包类型只有编译器知道，无法写出来。这也是为什么闭包和 `auto` 同时出现在 C++11 中：只有闭包而不引入 `auto`，就很难使用闭包类型编写代码。

`auto` 能够很好地解决这三个问题。

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

由于 `auto` 使用 [Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 中解释的推导规则，可以表示只有编译器知道的类型，所以可以声明闭包对象。
```cpp
auto derefUPLess =                          // comparison func.
    [](const std::unique_ptr<Widget>& p1,   // for Widgets
       const std::unique_ptr<Widget>& p2)   // pointed to by
    { return *p1 < *p2; };                  // std::unique_ptrs
```

C++14 允许在 lambda 的参数中使用 `auto`，因此可以进一步简化写法。
```cpp
auto derefLess =            // C++14 comparison
    [](const auto& p1,      // function for
       const auto& p2)      // values pointed
    { return *p1 < *p2; };  // to by anything
                            // pointer-like
```

有些人可能会有疑问：这里并不一定要使用闭包对象，也可以用 `std::function` 替代。下面解释一下两者之间的区别和联系。

C++11 引入的 `std::function` 是一种通用的、类型擦除的可调用对象包装器。函数指针只能指向函数，而 `std::function` 可以存储满足要求的各种可调用对象。和函数指针一样，使用 `std::function` 时也必须指定调用签名。例如，要定义一个调用签名如下的 `std::function` 对象：
```cpp
bool(const std::unique_ptr<Widget>&,    // C++11 signature for std::unique_ptr<Widget>
     const std::unique_ptr<Widget>&)    // comparison function
```

变量名为 `func`：
```cpp
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)> func;
```

lambda 表达式会生成可调用的闭包对象，因此闭包对象可以存储在 `std::function` 对象中。如果不用 `auto`，就必须写成
```cpp
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)>
    derefUPLess = [](const std::unique_ptr<Widget>& p1,
                     const std::unique_ptr<Widget>& p2)
                    { return *p1 < *p2; };
```

除了语法上的冗余以外，`std::function` 和 `auto` 并不完全一样。用 `auto` 声明的闭包对象具有闭包类型，其内存占用就是该闭包对象所需的内存。`std::function` 对象还需要存储类型擦除所需的信息；实现通常会为较小的可调用对象提供小对象优化，而对于放不下的对象则在堆上分配内存。因此，`std::function` 通常比闭包对象占用更多内存。由于难以内联且调用通常需要间接跳转，`std::function` 往往比 `auto` 声明的闭包对象慢。再加上 `auto` 写法简洁，`auto` 往往是更好的选择。（类似地，若要存储 lambda 表达式或 `std::bind` 调用产生的可调用对象，lambda 往往比 `std::bind` 更好，详见 [Item 34](../ch06_Lambda_Expressions/34_Prefer_lambdas_to_std_bind.md)。）

`auto` 还能避免类型错误的问题。比如
```cpp
std::vector<int> v;
unsigned sz = v.size();
```

很少有开发者意识到 `v.size()` 的返回类型是 `std::vector<int>::size_type`。这里使用的类型是 `unsigned`，在大多数情况下能够正常工作；但是在 64 位系统上，它可能无法表示容器允许的全部大小。使用 `auto` 就可以避免这种问题。
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

`std::unordered_map` 的键是 `const` 的，所以其中元素的 `std::pair` 类型是 `std::pair<const std::string, int>`，而不是 `std::pair<std::string, int>`。上述写法会构造一个新的临时对象，然后让 `p` 引用该对象；每次循环迭代结束后，这个临时对象都会被销毁。这明显不符合预期：我们只是想让 `p` 绑定到 `m` 中的每个元素。使用 `auto` 就没有这个问题了。
```cpp
for (const auto& p : m)
{
    // as before
}
```

新的代码更高效也更简洁。同时，如果获取 `p` 的地址，得到的就是指向 `m` 中某个元素的指针。

有很多使用 `auto` 的理由，不过它也不是完美的。`auto` 根据初始化表达式进行推导，有时推导结果并不符合预期，也不是我们想要的。[Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 和 [Item 6](./06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md) 对此有所描述，这里不再赘述。

`auto` 是可选的而不是强制的，出于可维护性或代码整洁等原因，可以显式地指定类型。类型推导并不是 C++ 的原创特性，很多语言都有类似功能；同时，社区也积累了很多经验，已经证实类型推导与大型软件工程的可维护性并不矛盾。

使用 `auto` 时，可能无法直接看出变量的实际类型，这一点可以借助 IDE 来解决。很多时候我们并不需要知道详细类型；只要知道它是容器、智能指针等，就足够了。配合良好的命名规则，也能传达这类抽象的类型信息。

显式地写类型可能会带来潜在问题，例如影响执行效率和正确性。另外，使用 `auto` 能够方便重构。例如，一个函数原本返回 `int`，后来需要改为返回 `long`；如果使用 `auto` 声明由函数返回值初始化的变量，就不需要修改调用处。

## Things to Remember
* `auto` variables must be initialized, are generally immune to type mismatches that can lead to portability or efficiency problems, can ease the process of refactoring, and typically require less typing than variables with explicitly specified types.
* `auto`-typed variables are subject to the pitfalls described in Items [2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) and [6](./06_Use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types.md).
