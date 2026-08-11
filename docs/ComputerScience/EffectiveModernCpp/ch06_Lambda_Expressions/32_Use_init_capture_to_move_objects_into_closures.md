有时，按值捕获或者按引用捕获都不是我们想要的。如果有一个只能移动的对象，比如 `std::unique_ptr` `std::future`，想要捕获到闭包内，而 C++11 并没有提供这种能力。当一个对象的移动操作比拷贝轻量很多，比如大部分 STL 容器，想把这个对象捕获进闭包，我们或许更倾向于移动而不是拷贝。C++11 lambda 也没有提供这个能力。

但是 C++14 提供了将一个对象移动到闭包的能力。

C++11 的一个缺点就是没有移动捕获的能力，最直接的修复方式是在 C++14 中增加这个功能。但是标准委员会的方案是提供新的捕获机制，更灵活，而移动捕获只是能完成的事情之一。新的能力称为初始化捕获（`init capture`）。初始化捕获比 C++11 中的捕获能力更强大。唯一的问题是没有默认捕获模式，不过 [Item 31](./31_Avoid_default_capture_modes.md) 说过，应该避免这个问题。不过 C++11 中可以很好的处理情况，初始化捕获的代码显得略微有点冗余，所以前者的方式也是一个不错的选择。

使用初始化捕获需要指定

* 生成的闭包类的数据成员的名字
* 初始化这个数据成员的表达式

下面展示了如何使用初始化捕获移动一个 `std::unique_ptr` 到闭包。
```cpp
class Widget
{ // some useful type
public:
    bool isValidated() const;
    bool isProcessed() const;
    bool isArchived() const;

private:
};

// create Widget; see Item 21 for info on std::make_unique
auto pw = std::make_unique<Widget>();

// configure *pw

// init data mbr  in closure w/ std::move(pw)
auto func = [pw = std::move(pw)]
{ return pw->isValidated() && pw->isArchived(); };
```
`pw = std::move(pw)` 这个表达式就是初始化捕获。`=` 的左边是闭包类成员变量的名字，右边是初始化表达式。`=` 左右两个部分的作用范围是不同的。左边的范围是闭包类的作用范围。右边的范围是 lambda 定义的作用范围。就上面的例子而言，`=` 左边的 `pw` 是闭包类的成员变量，右边的 `pw` 是 lambda 对象定义的范围，是通过 `std::make_unique` 创建的对象。`pw = std::move(pw)` 的含义就是在闭包内创建一个名为 `pw` 的成员变量，用 `std::move` 作用于局部变量 `pw` 的结果来初始化这个成员变量。

lambda 内部的代码是在闭包类的作用范围内，因此内部的 `pw` 指的是闭包类的成员变量。

代码中 `// configure *pw` 是指在使用 `std::make_unique` 创建的 `Widget` 和指向 `Widget` 的 `std::unique_ptr` 被捕获之前，需要修改 `Widget`。如果不需要，比如 `std::make_unique` 创建的 `Widget` 就可以直接被 lambda 捕获，那么局部变量 `pw` 不是必须的，可以使用 ``std::make_unique` 初始化闭包类的成员变量。
```cpp
// init data mbr in closure w/ result of call to make_unique
auto func = [pw = std::make_unique<Widget>()]
{ return pw->isValidated() && pw->isArchived(); };
```
C++14 的捕获是 C++11 捕获模式的推广，因此初始化捕获也称为通用捕获（`generalized lambda capture`）。

后面作者大篇幅的介绍了如何在 C++11 中模拟初始化捕获，自己写一个闭包类，或者是 `std::bind`，这里都略去了。

## Things to Remember
* Use C++14's init capture to move objects into closures.
* In C++11, emulate init capture via hand-written classes or `std::bind`.
