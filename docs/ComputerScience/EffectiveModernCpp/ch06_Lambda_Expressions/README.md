lambda 表达式是 C++ 中改变游戏规则的事务。因为它并没有带来新的表达能力，所以这种说法让人惊讶。所以 lambda 能做的事情，使用其他方式也能完成。但是 lambda 可以很方便的创建一个函数对象，影响着程序员的日常开发工作。有了 lambda 表达式，标准库中 `_if` 算法，比如 `std::find_if` `std::remove_if` `std::count_if`，用起来就相当简便了。对于需要自定义比较函数的地方，比如 `std::sort` `std::nth_element` `std::lower_bound`，也是类似的。lambda 还能方便的自定义 `std::shared_ptr` `std::unique_ptr` 的删除器（参见 Item [18](../ch04_Smart_Pointers/18_Use_std_unique_ptr_for_exclusive_ownership_resource_management.md) 和 [19](../ch04_Smart_Pointers/19_Use_std_shared_ptr_for_shared_ownership_resource_management.md)），还可以方便的指定线程 API 中的条件变量（参见 Item 39 TODO link）。除了标准库之外，lambda 表达式还能方便的用于回调函数、接口适配函数、一次性调用的上下文中。lambda 使得 C++ 成为了一种更令人愉快的编程语言。

下面简要回顾 lambda 相关的术语。

lambda 表达式（`lambda expression`）是一个表达式，是代码的一部分，下面高亮的两行就是 lambda 表达式。
```cpp hl_lines="2 3"
std::find_if(container.begin(), container.end(),
             [](int val)
             { return 0 < val && val < 10; });
```
闭包（`closure`）是 lambda 创建的运行时对象。依赖于捕获模式，闭包持有捕获数据的拷贝或者引用。在上面的例子中，闭包是运行时传入 `std::find_if` 的对象。

闭包类（`closure`）是闭包实例化的类。每一个 lambda 使得编译器生成唯一的闭包类。lambda 内的语句是对应闭包类的成员函数的可执行指令。

lambda 通常创建一个闭包，作为函数的参数使用一次。比如上面 `std::find_if` 的例子。不过，闭包可以被拷贝，一个 lambda 表达式就对应了多个闭包。比如下面的代码。
```cpp
{
    int x; // x is local variable

    auto c1 = [x](int y)    // c1 is copy of the closure
    { return x * y > 55; }; // produced by the lambda

    auto c2 = c1; // c2 is copy of c1
    auto c3 = c2; // c3 is copy of c2
}
```
这里 `c1` `c2` `c3` 都是 lambda 创建出来的闭包。

非正式的讲，模糊 lambda、闭包和闭包类的边界是可以接受的。不过在本章涉及的条款中，区分编译期（lambda 和闭包类）与运行时（闭包）并理解它们之间的关系很重要。
