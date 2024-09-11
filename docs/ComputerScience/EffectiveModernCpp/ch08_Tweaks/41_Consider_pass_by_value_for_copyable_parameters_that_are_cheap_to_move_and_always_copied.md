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

首先，仅仅是“考虑”按值传递。只用写一份代码，只有一份目标代码，没有通用引用传递的问题。不过，毕竟比可选方案要多一点开销。下面还会接着讨论一些其他开销。

其次，仅对“可拷贝”的类型考虑按值传递。只可移动的类型不满足条件，如果无法拷贝，而函数一定会为参数创建一个副本，那么会使用移动构造。按值传递的好处是只用写一个函数。但是对于只可移动的参数，没有必要实现一个左参版本的重载，因为这涉及到拷贝，但是只可移动的类型不支持拷贝。这就意味着只需要支持右值版本：“重载”一个右值版本就足够了。

看下面的例子，需要设置类型为 `std::unique_ptr<std::string>` 的成员变量，只要一个函数，即对右值类型做处理就足够了。
```cpp
class Widget
{
public:
    void setPtr(std::unique_ptr<std::string> &&ptr)
    {
        p = std::move(ptr);
    }

private:
    std::unique_ptr<std::string> p;
};
```
可能的调用如下：
```cpp
Widget w;

w.setPtr(std::make_unique<std::string>("Modern C++"));
```
`std::make_unique` 返回的右值 `std::make_unique<std::string>` 通过右值引用传递给 `setPtr`，然后移动到 `p` 中。全程只有一次移动开销。

如果使用按值传递，代码如下
```cpp
class Widget
{
public:
    void setPtr(std::unique_ptr<std::string> ptr)
    {
        p = std::move(ptr);
    }
};
```
同样的调用，会先移动构造 `ptr`，然后再移动赋值给 `p`，两次移动开销，比之前的方案多了一倍。

其次，按值传递只考虑“移动成本小”的类型。如果移动成本很低，那么额外一次移动是可以接受的，反之移动开销不低，那么就应该和避免不必要拷贝一样，尽可能避免不必要移动。

最后，按值传递的参数“总是被拷贝”。这很重要。假定在拷贝到 `names` 之前，`addName` 先检查长度，满足要求再添加到 `names`。代码可能如下。
```cpp
class Widget
{
public:
    void addName(std::string newName)
    {
        if ((newName.length() >= minLen) &&
            (newName.length() <= maxLen))
        {
            names.push_back(std::move(newName));
        }
    }

private:
    std::vector<std::string> names;
};
```
付出了构造和析构 `newName` 的代价，但是也有可能什么也不做，并没有往 `names` 中添加任何元素。如果使用引用就不会有这个问题。

即使上述条件都满足，有的时候按值传递也不是一个好的选择。这是因为函数实现可以有两种方式拷贝参数：构造和赋值。之前的 `addName` 是使用了构造，`newName` 传递给 `vector::push_back`，在函数内部 `newName` 构造了一个新的对象，其位于容器末尾。对于使用构造的函数，上述分析是成立：使用按值传递多一次移动成本。

当函数使用赋值来拷贝参数，情况就更复杂了。假设我们有一个类存放密码，因为密码可以更新，所以有一个 `changeTo` 来实现这个功能。我们使用按值传递，所以这个类大致如下。
```cpp
class Password
{
public:
    explicit Password(std::string pwd) // pass by value construct text
        : text(std::move(pwd))
    {
    }

    void changeTo(std::string newPwd) // pass by value assign text
    {
        text = std::move(newPwd);
    }

private:
    std::string text; // text of password
};
```
客户端代码如下
```cpp
std::string initPwd("Supercalifragilisticexpialidocious");
Password p(initPwd);
```
用给定的密码初始化 `p.text`，按值传递，在构造函数内部会多一次 `std::string` 移动开销，使用重载方式或者通用引用可以消除这个问题。

现在用户需要更新密码。
```cpp
std::string newPassword = "Beware the Jabberwock";
p.changeTo(newPassword);
```
现在 `changeTo` 使用赋值操作来拷贝参数 `newPwd`，会导致按值传递策略的代价提升相当大。

`newPassword` 是左值，所以构造参数 `newPwd` 的时候，会调用 `std::string` 的拷贝构造函数，构造函数会为新的密码分配内存。`newPwd` 移动给 `text`，会导致 `text` 原来持有的内存被释放掉。因此 `changeTo` 会有两次内存分配操作。

在当前这个例子中，旧密码比新密码长很多，所以无需分配和释放内存。如果使用重载方案就可以避免这个问题。
```cpp
class Password
{
public:
    void changeTo(const std::string &newPwd) // the overload for lvalues
    {
        // can reuse text's memory if text.capacity() >= newPwd.size()
        text = newPwd;
    }

private:
    std::string text; // as above
};
```
在这个场景中，按值传递包含了额外的分配内存和释放内存的操作，其开销比 `std::string` 的移动开销大多了。

如果旧密码比新密码要短，那么赋值的时候无法避免分配内存和释放内存，那么传值就和传引用速度一样了。也就是说，基于赋值的拷贝参数性能依赖于参数本身。上述分析仅适用于动态分配内存的参数类型，并不是所有类型都是这样的，不过也有很多满足，比如 `std::string` `std::vector`。

只有传递左值的时候才会有这个潜在的额外开销，因为只有此时才会执行拷贝然后有内存的分配和释放。对于传递右值，都是移动操作。

结论就是使用赋值进行拷贝的话，按值传递的额外开销取决于传递左值和右值的比例，类型是否动态分配内存，类型的赋值操作，旧值至少和新值占用内存一样多的可能性。对于 `std::string`，还需要考虑是否有 SSO，有的话还要考虑新值能否放进 SSO buffer。

所以，想要分析赋值拷贝情况下按值传递的开销是非常复杂的事情。实践中可以采取有罪推定的原则，除非能够确定按值传递的额外开销很小，否则就使用重载方案或者通用引用的方案。

对于尽可能快的软件来说，由于避免轻量的移动操作也很重要，所以按值传递不太可行。此外，很多时候不确定额外的移动操作有多少次。比如在 `Widget::addName` 例子中，如果其实现又调用了 `Widget::validateName`，也是按值传递。假定后者又调用了按值传递的函数，以此下去。

每一个函数都多了一次额外移动开销，但是累计开销可能就无法接受了。此时，如果按引用传递，就可以避免此类累计开销。

一个和性能无关，但是也是按照传递可能会导致的问题也需要讨论一下。这就是切片问题（`the slicing problem`）。如果一个函数可以接受基类及其派生类，那么不应该按值传递，否则派生类特有的数据会被切掉。
```cpp
// base class
class Widget
{
};

// derived class
class SpecialWidget : public Widget
{
};
void processWidget(Widget w); // func for any kind of Widget,
                              // including derived types;
                              // suffers from slicing problem

SpecialWidget sw;

processWidget(sw); // processWidget sees a Widget, not a SpecialWidget!
```
这也是为什么 C++98 要避免按值传递的另一个原因，不是效率问题，但是更严重。这也是为什么 C++ 编程首先学习的一个原则就是避免按值传递用户自定义的类型。

C++11 并没有改变这一点。按值传递仍旧有希望避免的性能问题，也会有切片问题。C++11 区别左值和右值参数。实现利用可拷贝对象的右值移动语义的函数，需要用重载方式或者通用引用，但是两者都有缺陷。对于特殊的可拷贝类型，移动代价很低，函数又总是需要拷贝参数，并且无需考虑切片的情况下，按值传递是一个性能近似接近的可选方案，能够避免其他方案的缺陷。

## Things to Remember
* For copyable, cheap-to-move parameters that are always copied, pass by value may be nearly as efficient as pass by reference, it's easier to implement, and it
can generate less object code.
* Copying parameters via construction may be significantly more expensive than copying them via assignment.
* Pass by value is subject to the slicing problem, so it's typically inappropriate for base class parameter types.
