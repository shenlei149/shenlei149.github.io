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

## Things to Remember
* Use C++14's init capture to move objects into closures.
* In C++11, emulate init capture via hand-written classes or `std::bind`.
