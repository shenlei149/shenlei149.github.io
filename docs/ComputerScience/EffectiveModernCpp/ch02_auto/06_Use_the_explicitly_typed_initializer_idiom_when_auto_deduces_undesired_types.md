[Item 5](./05_Prefer_auto_to_explicit_type_declarations.md) 列出了很多使用 `auto` 的优势。不过，有时 `auto` 推导出的类型并不是我们想要的。例如，有一个接受 `Widget` 并返回 `std::vector<bool>` 的函数，其中每个 `bool` 表示 `Widget` 是否提供某项特定能力。
```cpp
std::vector<bool> features(const Widget& w);
```

假定第 5 个 bit 表示传入的 `Widget` 是否具有高优先级。我们可能会写出如下代码：
```cpp
Widget w;
bool highPriority = features(w)[5];     // is w high priority?
processWidget(w, highPriority);         // process w in accord with its priority
```

上述代码工作得很好。若将 `highPriority` 的显式声明类型改为 `auto`：
```cpp
auto highPriority = features(w)[5];     // is w high priority?
```

代码能够通过编译，但是其行为变得不可预期：
```cpp
processWidget(w, highPriority);         // undefined behavior!
```

原因是使用 `auto` 后，`highPriority` 的类型不再是 `bool`，而是 `std::vector<bool>::reference`。通常，`std::vector<T>::operator[]` 返回 `T&`；但 `std::vector<bool>` 是个例外，因为单独的 bit 没有可供引用的地址。`std::vector<bool>::reference` 的行为很像 `bool&`，可以转换为 `bool`（而不是 `bool&`），所以显式地写出类型 `bool` 没有问题。

具体实现有所不同。`std::vector<bool>::reference` 往往包含指向存储该 bit 的字的指针，以及该 bit 的偏移量。`features(w)` 返回的临时 `std::vector<bool>` 在完整表达式结束时被销毁；`highPriority` 中的代理对象随之包含悬垂指针。

`std::vector<bool>::reference` 是代理类（`proxy class`）的例子。[第四章](../ch04_Smart_Pointers/README.md)介绍的智能指针也是代理类的例子。

`std::shared_ptr` 和 `std::unique_ptr` 对用户可见，而 `std::vector<bool>::reference` 在某种程度上是不可见的，这就导致了问题。

另一个常见的情况是表达式模板。考虑 `Matrix` 类的四个对象 `m1`、`m2`、`m3` 和 `m4`：
```cpp
Matrix sum = m1 + m2 + m3 + m4;
```

为了提高效率，`Matrix` 的 `operator+` 可能返回 `Sum<Matrix, Matrix>` 类型的对象。这是一种 `Matrix` 的代理类型，能够隐式转换为 `Matrix`。上面代码右侧表达式的类型可能形如 `Sum<Sum<Sum<Matrix, Matrix>, Matrix>, Matrix>`，随后隐式转换为 `Matrix`。

不可见的代理类型与 `auto` 配合时可能出现问题。这类对象的生命周期通常应限制在单个语句内；使用 `auto` 创建变量可能会违反这一限制。

现在有两个问题：第一，如何识别是否使用了代理类型；第二，如何修复这个问题。

先看第一个问题。尽管代理类型在设计时会尽量不让程序员感知，但所使用的类库通常仍会在文档中记录其用法。对类库越熟悉，就越不会对其中的代理类型感到束手无策。

如果没有文档，头文件是很好的信息来源。很少有类库会完全向客户端隐藏代理类型。函数会返回代理类型供用户使用，因此可以从函数签名得知代理类型的存在。例如，`std::vector<bool>::operator[]` 的函数签名为：
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

假设我们知道 `std::vector<T>::operator[]` 通常返回 `T&`，那么这个不遵循惯例的返回类型就是一个有效提示。关注接口往往能发现代理类型的存在。实践中，程序员通常是在处理诡异的编译错误，或调试代码和单元测试时发现代理类型的存在。

一旦发现代理类型，并且 `auto` 推导出的类型是代理类型而非被代理的类型，该如何修复呢？解决方法不只是弃用 `auto`，因为错不在 `auto`，而在于需要让它进行不同的类型推导。作者将此称为显式类型化初始化惯用法（`the explicitly typed initializer idiom`）。

这种方法仍然使用 `auto` 声明变量，不过先将初始化表达式转换为希望 `auto` 推导出的类型。例如：
```cpp
auto highPriority = static_cast<bool>(features(w)[5]);
```

`features(w)[5]` 返回 `std::vector<bool>::reference` 类型的对象，不过 `static_cast` 将其转换为 `bool`，因此 `auto` 推导出的变量 `highPriority` 的类型也是 `bool`。运行时，`std::vector<bool>::reference` 可以转换为 `bool`；在转换过程中，它的指针仍然有效，指向 `std::vector<bool>` 的内部存储单元，并被解引用以获取 `bool` 值。这样就避免了前面所说的未定义行为，随后该 `bool` 值用于初始化变量 `highPriority`。

前面的 `Matrix` 示例可以写作：
```cpp
auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
```

这种方法还能够强调程序员的意图：有意将初始化表达式转换为不同的变量类型。例如，一个计算容差的函数：
```cpp
double calcEpsilon(); // return tolerance value
```

其返回类型是 `double`。如果程序中认为 `float` 的精度已经足够，可能会写出如下代码：
```cpp
float ep = calcEpsilon(); // implicitly convert double -> float
```

但是，这没有表明我们有意降低精度。若使用显式类型化初始化惯用法，则写作：
```cpp
auto ep = static_cast<float>(calcEpsilon());
```

类似地，我们有时会将浮点数存入整型变量。例如，计算一个支持随机访问的容器的下标时，有一个范围为 0 到 1.0 的浮点数，它表示相对于第 0 个元素的位置。计算结果是浮点数，但对于下标而言，将其存入整型变量已经足够。
```cpp
int index = d * (c.size() - 1);
```

这种写法略显晦涩，没有显式表达我们的意图：将 `double` 类型的值存入 `int` 类型的变量。下面的写法更好：
```cpp
auto index = static_cast<int>(d * (c.size() - 1));
```

## Things to Remember
* "Invisible" proxy types can cause `auto` to deduce the "wrong" type for an initializing expression.
* The explicitly typed initializer idiom forces `auto` to deduce the type you want it to have.
