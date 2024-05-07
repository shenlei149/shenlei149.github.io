[Item 5](/EffectiveModernCpp/ch02_auto/05_Prefer_auto_to_explicit_type_declarations.md) 列出了很多使用 `auto` 的优势。不过，有的时候 `auto` 的推导的类型并不是我们想要的。比如有一个接受 `Widget` 返回 `std::vector<bool>` 的函数，每一个 `bool` 表示 `Widget` 是否提供特定的能力。
```cpp
std::vector<bool> features(const Widget& w);
```

假定 bit 5 表示传入的 `Widget` 是否是高优先级的。我们可能会写如下代码：
```cpp
Widget w;
bool highPriority = features(w)[5];     // is w high priority?
processWidget(w, highPriority);         // process w in accord with its priority
```

上述代码工作的很好，如果将 `highPriority` 显式声明的类型改为 `auto`：
```cpp
auto highPriority = features(w)[5];     // is w high priority?
```

代码能够通过编译，但是行为不可预期了。
```cpp
processWidget(w, highPriority);         // undefined behavior!
```

原因是当使用 `auto` 之后，`highPriority` 类型不再是 `bool` 而是 `std::vector<bool>::reference`。`std::vector::operator[]` 返回的类型一般情况是 `T&`，但是 `std::vector<bool>` 不是，因为 C++ 的规范使得无法引用一个具体的 bit。`std::vector<bool>::reference` 的行为很像 `bool&`，可以转化成 `bool`（不是 `bool&`），所以显式地写类型 `bool` 是没有问题的。

具体行为依赖于实现。`std::vector<bool>::reference` 往往包含一个指向某个包含该 bit 的字，外加一个偏移量。这是一个临时对象。这个临时对象拷贝给了 `highPriority`，等语句结束的时候，这个临时对象就销毁了。`highPriority` 包含了一个悬垂指针。

`std::vector<bool>::reference` 是代理类（`proxy class`）的例子。第四章（TODO link）介绍的智能指针也是代理类的例子。

`std::shared_ptr` 和 `std::unique_ptr` 对用户可见，而 `std::vector<bool>::reference` 某种程度上是不可见的，这就导致了问题。

另一个常见的情况是表达式模板。考虑类 `Matrix` 的四个对象 `m1, m2, m3, m4`
```cpp
Matrix sum = m1 + m2 + m3 + m4;
```

为了效率，`Matrix` 的 `operator+` 可能返回的类型是 `Sum<Matrix, Matrix>`，这是 `Matrix` 的代理类，能够隐式地转化为 `Matrix` 类型。上面代码右边的类型很可能是 `Sum<Sum<Sum<Matrix, Matrix>, Matrix>, Matrix>` 然后能够隐式地转化成 `Matrix` 类型。

看不见的代理类和 `auto` 配合会有问题。这些类型的对象的生命周期应该局限于一个语句，但是创建这些类型的对象会违反这一点。

那么现在有两个问题，第一个如何识别用到了代理类，第二个是如何修复。

首先看第一个问题。尽管代理类在设计的时候会尽可能不让程序员感知，但使用的类库还是会记录下用法。对类库越熟悉，那么就不会对类库的代理类感到束手无策。

如果没有文档，头文件是一个很好的信息来源。很少有类库会完全对客户端隐藏代理类。函数返回类型是代理类，然后让用户使用，所以可以从函数签名得知代理类的存在。比如 `std::vector<bool>::operator[]` 的函数签名
```cpp
namespace std           // from C++ Standards
{
    template <class Allocator>
    class vector<bool, Allocator>
    {
    public:
        class reference
        {
        };

        reference operator[](size_type n);
    };
}
```

假设我们知道 `operator[] for std::vector<T>` 通常返回类型是 `T&`，那么这个不遵循惯例的返回类型就是有效提示。关注接口往往会发现代理类的存在。实践中，程序员是在处理诡异的编译错误或者 debug 单元测试时发现代理类的存在。

一旦发现了代理类，且 `auto` 推导的结果是代理类类型而不是被代理的类型，如何修复呢？解决方法不是简单的不用 `auto`，因为这不是 `auto` 的错，而是强行进行不同的类型推导。作者称为显式类型化初始化惯用法（`the explicitly typed initializer idiom`）。

这种方法仍旧使用 `auto` 声明变量，不过将类型转化为希望 `auto` 推到的类型。比如
```cpp
auto highPriority = static_cast<bool>(features(w)[5]);
```

`features(w)[5]` 返回 `std::vector<bool>::reference` 类型的对象，不过 `static_cast` 把类型转化成了 `bool`，`auto` 推断变量 `highPriority` 的类型也是 `bool`。运行时，`std::vector<bool>::reference` 能够转化成 `bool` 类型，在转化的过程中，其指针有效，也就是指向 `std::vector<bool>` 内部字段，然后解引用，获取 `bool` 类型的值。这样就避免了前面说的未定义行为。最后用这个 `bool` 值来初始化变量 `highPriority`。

前面 `Matrix` 的例子可以写作
```cpp
auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
```

这种方法还能够强调程序员的意图：故意从初始化表达式的类型转化到一个不同的变量类型。比如一个计算容忍度的函数
```cpp
double calcEpsilon(); // return tolerance value
```

返回类型是 `double`。如果我们程序中觉得 `float` 的精度足够了，可能会写如下代码
```cpp
float ep = calcEpsilon(); // impliclitly convert double -> float
```

但是这并没有表明我们有意减少精度。如果使用显示类型化初始化惯用法的话，写作
```cpp
auto ep = static_cast<float>(calcEpsilon());
```

类似的，我们有时将一个浮点数存到一个整型变量中。比如计算一个能够随机访问的容器的下标，我们有一个浮点数，范围是 0 到 1.0，表示距离零号元素的距离，计算结果是浮点数，但是对于下标而言，放到整型变量里面足够了。
```cpp
int index = d * (c.size() - 1);
```

这种写法略显晦涩，没有显示出我们的意图，就是想将 `double` 类型的值存到 `int` 类型的变量中。下面的写法就好多了。
```cpp
auto index = static_cast<int>(d * (c.size() - 1));
```

## Things to Remember
* "Invisible" proxy types can cause `auto` to deduce the "wrong" type for an initializing expression.
* The explicitly typed initializer idiom forces `auto` to deduce the type you want it to have.
