C++ 面向对象的基础是类、继承、虚函数。一个基本概念就是继承类实现某个虚函数覆盖（`override`，下面用覆盖、重写表示这个词）基类的实现。C++ 的设计使得重写函数很容易出问题。

重写函数和函数重载（`overload`）没有任何关系。它的作用是通过基类的接口，可以调用子类的函数。
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
* 函数名必须一样（除了析构函数）
* 参数类型必须一样
* `const` 修饰必须一样
* 返回类型和异常类型是兼容的

除此之外，C++11 还要求
* 引用修饰符必须一样。非常不常见的特性，作用是约束只能左值或者右值调用。

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

稍后会详解引用修饰符，这里只需要注意，如果子类和基类的修饰符不一样，子类的函数是独立的函数，并没有覆盖基类的实现。

如果没有遵循上述要求，那么就会出现问题，下面例子的各个函数并不是覆盖基类的虚函数。
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

上面代码的问题是
* `mf1`：基类中用 `const` 修饰，继承类没有
* `mf2`：基类中的参数类型是 `int`，但是继承类类型是 `unsigned int`
* `mf3`：基类中是左值引用修饰，但是继承类是右值引用修饰
* `mf4`：基类中没有声明为虚函数

你可能会说编译器会给出警告。但是编译器不总是会给出警告信息。另外，有些开发者可能不注意这些警告。

C++11 引入新的关键字：`override`。当需要覆盖基类函数时，在函数最后面添加 `override` 即可。如下所示：
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

这些代码无法编译。当函数标记 `override` 之后，编译器会检查和重写相关的条件，如果不满足就会报错。

下面是修复错误后的代码
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

多数问题需要在继承类中修复，但是也不排除像最后一个例子一样在基类做某些修改。

除此之外，`override` 还可以帮助我们评估修改基类签名的后果。如果到处都写了 `override`，那么基类签名修改了，会出现很多编译错误，然后评估是否值得做。如果没有写 `override`，编译器不会有任何错误信息，但是此时行为已经不符合预期了，但愿有完善的测试能够捕捉 bug。

准确地说，`override` 和 `final` 是两个上下文相关的关键字（`contextual keyword`）。比如 `override`，只有出现在函数尾部它才是关键字。如果历史遗留代码中用 `override` 作为名字，升级到 C++11 也无需修改。
```cpp
class Warning // potential legacy class from C++98
{
public:
    void override(); // legal in both C++98 and C++11 (with the same meaning)
};
```

最后，我们再过回头解释一下引用修饰。

如果一个函数只接受左值作为参数，可以将参数类型声明为非 `const` 的左值引用。右值类似。
```cpp
void doSomething(Widget &w);  // accepts only lvalue Widgets
void doSomething(Widget &&w); // accepts only rvalue Widgets
```

对成员函数使用引用修饰符的目的就是区分调用这个成员函数的对象，即 `*this`。这和使用 `const` 修饰成员函数是一样的。

引用修饰成员函数不常见，但是是有可能出现的。比如 `Widget` 有一个类型为 `std::vector` 的成员函数，提供访问函数给客户端使用。
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

这个封装可能不太好，没关系，我们先看下使用这个函数会发生什么事。
```cpp
Widget w;
auto vals1 = w.data(); // copy w.values into vals1
```

`data()` 返回的是左值引用，即左值，那么用左值初始化左值会发生拷贝，如注释所言。

假定我们有一个工厂方法返回 `Widget` 对象
```cpp
Widget makeWidget();
```

现在我们用 `makeWidget` 返回的对象中的 `values` 初始化一个 `std::vector`。
```cpp
auto vals2 = makeWidget().data(); // copy values inside the Widget into vals2
```

和上面分析一样，这里还是要拷贝数据。`makeWidget` 返回的是一个临时对象，马上就要销毁了，这时候拷贝 `std::vector` 的话完全是浪费，此时移动更好。不过 `data()` 返回的是左值，编译器应该生成拷贝的函数。（可能在某些场景下，一些编译器能优化，但是我们不应该依赖编译器。）

正确的解法是 `Widget` 是右值对象调用 `data()` 时，返回也是右值。引用修饰就是这个目的。
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

两个重载的返回类型是不同的。那么之前两种不同类型的对象调用 `data()` 函数会有不一样的行为。
```cpp
auto vals1 = w.data();            // calls lvalue overload for
                                  // Widget::data, copy-constructs vals1
auto vals2 = makeWidget().data(); // calls rvalue overload for
                                  // Widget::data, move-constructs vals2
```

## Things to Remember
* Declare overriding functions `override`.
* Member function reference qualifiers make it possible to treat lvalue and rvalue objects (`*this`) differently.
