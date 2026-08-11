有时我们需要创建一个线程执行后台任务，但是创建线程的函数不等待线程执行完成而是把线程的所有权给调用者；或者相反，创建一个线程，然后把所有权给某个函数。这就涉及到线程的所有权转移。

`std::thread`对移动的支持就是为了解决这个问题。执行线程的所有权可以在不同`std::thread`对象之间转移。下面的例子就创建了两个线程和三个`std::thread`对象。
```cpp
void some_function();
void some_other_function();
std::thread t1(some_function);
std::thread t2 = std::move(t1);
t1 = std::thread(some_other_function);
std::thread t3;
t3 = std::move(t2);
t1 = std::move(t3);
```
首先创建一个线程，和`t1`关联。接着，在构造`t2`的时候，通过显式的`std::move()`把线程的所有权转移给`t2`。现在`t1`不再关联任何执行线程，`t2`和运行`some_function`的线程关联。

接着，启动新的线程，和临时`std::thread`对象关联。后续所有权转移给`t1`不需要调用`std::move()`，因为所有者是一个临时对象，从临时对象转移所有权是自动的（隐式的）。

使用默认构造函数构造`t3`，没有和任何线程关联。接着把`t2`关联的线程所有权转移给`t3`，这里是显式通过`std::move()`进行的，因为`t2`是具名变量。此时，`t1`关联运行`some_other_function`的线程，`t2`没有关联任何线程，`t3`关联运行`some_function`的线程。

最后，把运行`some_function`的线程的所有权转移回`t1`。但是`t1`已经关联一个线程了，那么会调用线程的析构函数，但是此线程没有`join()`或者`detach()`，那么析构函数会调用`std::terminate()`终止程序。赋值新的线程对象会导致丢弃旧的线程对象。

`std::thread`对移动的支持意味着我们可以轻松的把线程所有权转移出函数。
```cpp
std::thread f()
{
    void some_function();
    return std::thread(some_function);
}

std::thread g()
{
    void some_other_function(int);
    std::thread t(some_other_function, 42);
    return t;
}
```
类似的，如果一个函数接受`std::thread`示例作为参数，那么我们可以转移所有权到函数内。
```cpp
void f(std::thread t);
void g()
{
    void some_function();
    f(std::thread(some_function));
    std::thread t(some_function);
    f(std::move(t));
}
```
`std::thread`所有权可以转移使得我们可以转移线程所有权到`thread_guard`类内部，以免发生任何不期待的行为，因为外部没有人能够`join()`或者`detach()`这个线程了。我们重写一个类似的`scoped_thread`类，目标是在退出某个范围前线程完成。
```cpp
class scoped_thread
{
    std::thread t;

public:
    explicit scoped_thread(std::thread t_) : t(std::move(t_))
    {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }

    ~scoped_thread()
    {
        t.join();
    }

    scoped_thread(scoped_thread const &) = delete;
    scoped_thread &operator=(scoped_thread const &) = delete;
};

struct func;

void f()
{
    int some_local_state;
    scoped_thread t{std::thread(func(some_local_state))};
    do_something_in_current_thread();
}
```
和之前类似，不过新的线程对象直接接受新线程的所有权。当到`f`执行完的时候，`t`会被销毁，这时在析构函数中`join()`。`thread_guard`是在析构函数中检查`joinable()`，这里修改成了在构造函数里面检查。

C++17 有一个提案，`joining_thread`，当线程析构时自动`join()`，和上面的代码很类似，不过委员会未能达成共识。不过C++20 的时候，`std::jthread`是标准的一部分了。实现类似下面的代码。
```cpp
class joining_thread
{
    std::thread t;

public:
    joining_thread() noexcept = default;

    template <typename Callable, typename... Args>
    explicit joining_thread(Callable &&func, Args &&...args) : t(std::forward<Callable>(func), std::forward<Args>(args)...)
    {
    }

    explicit joining_thread(std::thread t_) noexcept : t(std::move(t_))
    {
    }

    joining_thread(joining_thread &&other) noexcept : t(std::move(other.t))
    {
    }

    joining_thread &operator=(joining_thread &&other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other.t);
        return *this;
    }

    joining_thread &operator=(std::thread other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other);
        return *this;
    }

    ~joining_thread() noexcept
    {
        if (joinable())
            join();
    }

    void swap(joining_thread &other) noexcept
    {
        t.swap(other.t);
    }

    std::thread::id get_id() const noexcept
    {
        return t.get_id();
    }

    bool joinable() const noexcept
    {
        return t.joinable();
    }

    void join()
    {
        t.join();
    }

    void detach()
    {
        t.detach();
    }

    std::thread &as_thread() noexcept
    {
        return t;
    }

    const std::thread &as_thread() const noexcept
    {
        return t;
    }
};
```
`std::thread`的移动支持使得支持移动操作的容器（比如`std::vector<>`）可以放`std::thread`对象。可以像下面的例子一样生成一堆线程放到容易内然后等待它们结束。
```cpp
void do_work(unsigned id);
void f()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.emplace_back(do_work, i);
    }

    for (auto &entry : threads)
        entry.join();
}
```
若用多线程来切分算法的工作，往往采用上述的结构。在函数返回前，这些线程必须完成。每个线程是自包含的，函数的副作用往往是操作共享数据作为结果。假如$f()`要向调用者返回值，而结果又依赖于这些线程完成工作来检查共享数据，那么只能等这些线程结束。第四章介绍其他传递运算结果的方法。

把`std::thread`对象放到`std::vector`里面而不是一个个的声明并等待，向自动管理线程又前进了一步，这些线程可以视为一组。
