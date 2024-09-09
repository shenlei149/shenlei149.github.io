有一些参数是需要被拷贝的。比如下面的例子，`addName` 可能需要拷贝参数到私有容器中。从效率上讲，应该拷贝左值移动右值。
```cpp
class Widget
{
public:
    void addName(const std::string &newName) // take lvalue;
    {
        // copy it
        names.push_back(newName);
    }

    void addName(std::string &&newName) // take rvalue;
    {
        // move it; see Item 25 for use of std::move
        names.push_back(std::move(newName));
    }

private:
    std::vector<std::string> names;
};
```
上述代码能够工作，但是需要写两份一模一样的代码，比如声明、定义、文档，也需要维护两份代码。

如果介意编译结果大小的话，在对象代码中也有两个函数。如果两个函数能够 `inline` 的话，代码膨胀会更大，如果不 `inline` 的话，就会看到两个函数。

一个可选方案是使用模板函数，参数是通用引用([Item 24](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/24_Distinguish_universal_references_from_rvalue_references.md))。
```cpp
class Widget
{
public:
    template <typename T>     // take lvalues and rvalues;
    void addName(T &&newName) // copy lvalues, move rvalues;
    {                         // see Item 25 for use of std::forward
        names.push_back(std::forward<T>(newName));
    }
};
```
这虽然减少了源代码的维护工作，但是引入了很多新的更复杂的问题。`addName` 作为模板函数，一般需要实现在头文件。会生成多份目标大吗，因为除了 `std::string` 左值和右值能实例化这个模板函数之外，能够构造 `std::string` 的类型也可以实例化这个模板函数。有的类型不能传递给通用模板参数，参见 [Item 30](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/30_Familiarize_yourself_with_perfect_forwarding_failure_cases.md)，如果传递了不合适的参数，那么会有相当多的报错信息。

有没有更好的实现？有。不过需要放弃作为 C++ 程序员的第一个原则，避免按值传递用户的自定义类型。对于函数 `addName` 而言，按值传递是非常合理的策略。

在说明为什么合理之前，先看下其实现。
```cpp
class Widget
{
public:
    void addName(std::string newName) // take lvalue or rvalue; move it
    {
        names.push_back(std::move(newName));
    }
};
```
这里需要解释的一点是对 `newName` 使用 `std::move`。一般情况下，`std::move` 用于右值引用。我们知道不管调用者传入的是什么，`newName` 都是一个独立的对象，并且这里是最后一次使用 `newName`，所以移动对后续没有影响。

这个实现解决了之前的问题，不过按值传递参数，会不会代价比较大？

在 C++98，确实会有这个问题。不管调用者传入了什么，都会调用拷贝构造函数来创建参数 `newName`。不过到了 C++11，对于左值，拷贝构造了 `newName`。对于右值，其是被移动构造的。
```cpp
Widget w;

std::string name("Bart");
w.addName(name); // call addName with lvalue

w.addName(name + "Jenne"); // call addName with rvalue
```
第一次调用 `addName`，参数 `newName` 由左值初始化，所以是拷贝构造。第二次调用 `newName` 是由 `std::string` 的 `operator+` 的结果来初始化，这个结果是右值，所以 `newName` 是移动构造的。

左值拷贝右值引用，这正是我们所需要的。不过也有一些警示需要牢记。

下面分析上述三个版本的开销。这里忽略编译器对拷贝和移动的优化，这些优化都是上下文和编译器相关。实际上，这些不会影响分析的本质。

* 重载方案：不管是左值还是右值，都是绑定引用到变量 `newName` 上，从拷贝和移动的角度看，这一点零开销。对于左值重载，一次拷贝开销，对于右值重在，一次移动开销。
* 通用引用：和重载方案一样，绑定引用，那么对于 `std::string` 类型的参数，和之前分析一样，对于左值，一次拷贝，对于右值，一次移动。如果参数不是 `std::string` 类型的参数，完美转发参数构造一个 `std::string`，也没有 `std::string` 的拷贝或者移动，因此不影响分析。
* 按值传递：不管是左值还是右值，都必须构造 `newName` 对象。左值初始化，一次拷贝，右值初始化，一次移动。当添加到 `Widget::names` 中时，还有一次移动开销。所以对于左值，一次拷贝一次移动开销，对于右值，两次移动开销。

这一节的标题略微有点复杂，因为包含了四个要素：

**对于可拷贝参数，如果其总是要被拷贝且移动开销低，那么考虑按值传递。**

首先，
