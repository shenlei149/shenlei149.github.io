C++ 面向对象编程的基础是类、继承和虚函数。一个基本概念是：派生类通过实现某个虚函数来覆盖（`override`，下文以“覆盖”和“重写”表示这一概念）基类的实现。C++ 的设计使得编写重写函数时很容易出问题。

重写函数与函数重载（`overload`）没有任何关系。重写使得可以通过基类接口调用派生类的函数。
```cpp
class Base
{
public:
    virtual void doWork(); // base class virtual function
};

class Derived : public Base
{
public:
    virtual void doWork(); // overrides Base::doWork
                           // ("virtual" is optional here)
};

std::unique_ptr<Base> upb =      // create base class pointer to derived class object;
    std::make_unique<Derived>(); // see Item 21 for info on std::make_unique
upb->doWork();                   // call doWork through base class ptr;
                                 // derived class function is invoked
```

重写函数有以下要求：
* 基类函数必须是虚函数
* 函数名必须相同（析构函数除外）
* 参数类型必须相同
* `const` 修饰必须相同
* 返回类型必须相同，或者满足协变返回类型规则
* 异常规范必须兼容（派生类的异常规范不能比基类更宽松）

此外，C++11 还要求：
* 引用修饰符必须相同。这是一项较少使用的特性，用于限制成员函数只能由左值或右值对象调用。

```cpp
class Widget
{
public:
    void doWork() &;  // this version of doWork applies  only when *this is an lvalue
    void doWork() &&; // this version of doWork applies only when *this is an rvalue
};

Widget makeWidget(); // factory function (returns rvalue)
Widget w;            // normal object (an lvalue)

w.doWork();            // calls Widget::doWork for lvalues (i.e., Widget::doWork &)
makeWidget().doWork(); // calls Widget::doWork for rvalues (i.e., Widget::doWork &&)
```

稍后会详细介绍引用修饰符。这里只需要注意：如果派生类和基类的引用修饰符不同，派生类的函数就是独立的函数，并没有覆盖基类的实现。

如果没有遵循上述要求，就会出现问题。下面示例中的各个函数都没有覆盖基类的虚函数。
```cpp
class Base
{
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    void mf4() const;
};
class Derived : public Base
{
public:
    virtual void mf1();
    virtual void mf2(unsigned int x);
    virtual void mf3() &&;
    void mf4() const;
};
```

上面代码的问题是：
* `mf1`：基类函数带有 `const` 修饰符，派生类函数没有
* `mf2`：基类函数的参数类型是 `int`，而派生类函数的参数类型是 `unsigned int`
* `mf3`：基类函数带有左值引用修饰符，而派生类函数带有右值引用修饰符
* `mf4`：基类函数没有声明为虚函数

你可能会说编译器会给出警告，但编译器并不总会给出警告信息。另外，有些开发者可能会忽略这些警告。

C++11 引入了新的关键字：`override`。当需要覆盖基类函数时，只需在函数声明末尾添加 `override`。如下所示：
```cpp
class Derived : public Base
{
public:
    virtual void mf1() override;
    virtual void mf2(unsigned int x) override;
    virtual void mf3() && override;
    virtual void mf4() const override;
};
```

这些代码无法编译。函数标记为 `override` 后，编译器会检查所有与重写相关的条件；如果不满足，就会报错。

下面是修复错误后的代码：
```cpp
class Base
{
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    virtual void mf4() const;
};

class Derived : public Base
{
public:
    virtual void mf1() const override;
    virtual void mf2(int x) override;
    virtual void mf3() & override;
    void mf4() const override; // adding "virtual" is OK, but not necessary
};
```

多数问题需要在派生类中修复，但也不排除像最后一个例子那样修改基类。

此外，`override` 还可以帮助我们评估修改基类函数签名的后果。如果所有重写函数都标记了 `override`，修改基类函数签名时就会出现许多编译错误，从而可以评估是否值得进行这一修改。如果没有标记 `override`，编译器可能不会给出任何错误信息，但此时程序行为已经不符合预期；但愿完善的测试能够捕捉到 bug。

准确地说，`override` 和 `final` 是两个上下文相关关键字（`contextual keywords`）。以 `override` 为例，只有出现在函数声明末尾时，它才是关键字。如果遗留代码中将 `override` 用作名称，升级到 C++11 时也无需修改。
```cpp
class Warning // potential legacy class from C++98
{
public:
    void override(); // legal in both C++98 and C++11 (with the same meaning)
};
```

最后，我们再回过头解释一下引用修饰符。

如果一个函数只接受左值作为参数，可以将参数类型声明为非 `const` 左值引用。右值的情况类似。
```cpp
void doSomething(Widget &w);  // accepts only lvalue Widgets
void doSomething(Widget &&w); // accepts only rvalue Widgets
```

对成员函数使用引用修饰符的目的，是区分调用该成员函数的对象，即 `*this`。这与使用 `const` 修饰成员函数的作用类似。

带引用修饰符的成员函数并不常见，但确实可能出现。例如，`Widget` 有一个类型为 `std::vector<double>` 的数据成员，并向客户端提供访问函数。
```cpp
class Widget
{
public:
    using DataType = std::vector<double>;
    DataType &data() { return values; }

private:
    DataType values;
};
```

这种封装可能并不理想，但先看看调用这个函数时会发生什么。
```cpp
Widget w;
auto vals1 = w.data(); // copy w.values into vals1
```

`data()` 返回左值引用，因此函数调用表达式是左值。用它初始化 `vals1` 时会发生拷贝，如注释所言。

假定我们有一个返回 `Widget` 对象的工厂函数：
```cpp
Widget makeWidget();
```

现在用 `makeWidget` 返回对象中的 `values` 初始化一个 `std::vector`。
```cpp
auto vals2 = makeWidget().data(); // copy values inside the Widget into vals2
```

分析过程与上面相同，这里仍然需要拷贝数据。`makeWidget` 返回的是一个即将销毁的临时对象，此时拷贝 `std::vector` 完全是浪费，移动会更好。不过，`data()` 返回左值，因此会调用拷贝构造函数。（某些场景下，编译器或许能够优化，但我们不应该依赖这种优化。）

正确的做法是：当右值 `Widget` 对象调用 `data()` 时，让 `data()` 按值返回，从而使调用表达式成为右值。引用修饰符正是为此而设。
```cpp
class Widget
{
public:
    using DataType = std::vector<double>;

    DataType &data() & // for lvalue Widgets, return lvalue
    {
        return values;
    }
    DataType data() && // for rvalue Widgets, return rvalue
    {
        return std::move(values);
    }

private:
    DataType values;
};
```

这两个重载的返回类型不同。因此，前述两类对象调用 `data()` 函数时会有不同的行为。
```cpp
auto vals1 = w.data();            // calls lvalue overload for
                                  // Widget::data, copy-constructs vals1
auto vals2 = makeWidget().data(); // calls rvalue overload for
                                  // Widget::data, move-constructs vals2
```

## Things to Remember
* Declare overriding functions `override`.
* Member function reference qualifiers make it possible to treat lvalue and rvalue objects (`*this`) differently.
