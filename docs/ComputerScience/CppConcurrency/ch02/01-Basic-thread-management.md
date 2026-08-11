每个C++程序至少有一个线程：运行`main()`的线程。程序可以再启动以其他函数为入口的线程，这些线程和初始线程一起并行运行。`main()`返回那么程序退出，类似的指定的入口函数返回那么线程退出。

### Launching a thread
构造`std::thread`对象就会启动一个线程。最简单的任务没有参数，也没有返回值，运行结束自行退出。任务也可以很复杂，比如接受一些参数，同时需要通过消息系统触发执行一些操作，线程退出也需要特殊的消息。不管任务简单还是复杂，启动线程总是从构造`std::thread`开始。
```cpp
void do_some_work();
std::thread my_thread(do_some_work);
```
任何可执行类型都可以作为构造函数的参数。可以传递实现了函数调用操作的类的实例给`std::thread`构造函数。
```cpp
class background_task
{
public:
    void operator()() const
    {
        do_something();
        do_something_else();
    }
};
background_task f;
std::thread my_thread(f);
```
这种情况下，函数对象会被复制到线程对象的空间中然后执行。那么需要保证拷贝与原始对象行为等价，否则可能会出现非预期行为。

另外一个需要注意的事情是"C++'s most vexing parse"。如果传递一个临时变量，语法可能和函数声明一样，那么编译器不会认为它是一个函数对象。比如
```cpp
std::thread my_thread(background_task());
```
声明了一个函数`my_thread`，返回值是`std::thread`对象，参数是一个函数指针，无参，返回值是一个`background_task`对象。可以增加一对小括号，或者使用统一初始化列表的方式。
```cpp
std::thread my_thread((background_task()));
std::thread my_thread{background_task()};
```
一个避免上述问题的方法是使用 lambda 表达式，可以捕获局部变量以避免传递参数，前一个例子可以改写为
```cpp
std::thread my_thread([]
                      {
    do_something();
    do_something_else(); });
```

一旦启动一个线程，我们需要在`std::thread`对象销毁之前决定是等待其执行完（`join`）还是让其自己运行（`detach`），否则`std::thread`的析构函数`std::terminate()`终止程序。即使用异常，我们现在也必须做出这个决定。决定时间点是`std::thread`销毁之前，可能线程早都运行完了。如果选择`detach`且线程还在运行，那么线程会继续运行，`std::thread`被销毁了也可以继续运行（最终函数返回时线程退出）。

如果放任新的线程运行，那么需要保证线程访问的数据在结束前都是有效的。这个问题和是否多线程无关，不过多线程程序中遇到生命周期问题的可能性更大。

如果子线程持有主线程局部变量的引用或者指针，主线程退出，子线程还需要访问这些变量就会出问题。
```cpp
struct func
{
    int &i;
    func(int &i_) : i(i_) {}
    void operator()()
    {
        for (unsigned j = 0; j < 1000000; ++j)
        {
            do_something(i);
        }
    }
};

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();
}
```
因为显式调用了`detach()`，`oops`退出之后`my_thread`可能还在运行。再次调用`do_something(i)`的时候就会访问被销毁的变量。这和普通的单线程程序一样，可能会访问被销毁变量的指针或者引用。出现了这种情况之后，错误也不一定会立即发生。

一个常用的解决方法是自包含所有数据（复制一份），而不是使用共享数据。如果是可执行对象作为构造函数，这个对象本身会被复制到线程内部，所以这个对象立即销毁也没事。不过这个对象持有引用或者指针就需要小心了。所以在函数内创建线程，不要引用局部变量，除非能保证线程运行结束后函数才退出。

### Waiting for a thread to complete
调用`join()`方法可以等待新线程结束。这是一种简单且暴力的技术，要么等要么不等。如果需要更精细的控制，比如检查是否完成或者需要等待一段时间，需要其他技术，第四章会详细说明。调用`join()`会清除关联线程的存储空间，也就不会再关联任何线程了。这意味着你只能调用一次`join()`函数，调用过之后，那么对应`std::thread`对象的`joinable()`函数会返回`false`。

### Waiting in exceptional circumstances
线程启动之后立即调用`detach()`是没有问题的。但是调用`join()`的时机需要谨慎考虑。如果在`join()`之前有异常抛出，那么就不会执行`join()`了。

在程序不会由于异常的抛出就异常退出的情况下，如果在非异常路径上调用了`join()`，那么需要在异常发生的时候也能调用到`join()`。下面是一个示例
```cpp
struct func;
void f()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        do_something_in_current_thread();
    }
    catch (...)
    {
        t.join();
        throw;
    }
    t.join();
}
```
使用`try/catch`块确保不管是异常或者正常结束函数`f`，在此之前线程`t`都能访问局部变量。这种方式略微繁琐一点。不管是由于访问局部变量还是其他原因，需要确保函数正常退出或者异常退出前线程完成，有简洁的方式做这个事情——RAII，把`join()`放到一个类的析构函数里面。
```cpp
class thread_guard
{
    std::thread &t;

public:
    explicit thread_guard(std::thread &t_) : t(t_) {}
    ~thread_guard()
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;
};
struct func;
void f()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
}
```
当函数`f`退出时，局部对象会按照构建的逆序析构。首先析构`thread_guard`对象`g`，线程在析构函数中`join()`。不管`do_something_in_current_thread()`是否抛出异常，析构函数一定会调用到。

`thread_guard`析构函数中在`join()`之前判断`joinable()`，这点非常重要，因为`join()`只能调用一次！

复制和赋值构造函数被标记成`=delete`使得编译器不会为我们合成默认的相应构造访问。复制或者赋值的新对象的生命周期可能比引用的线程对象更长，那么再调用析构函数就会出错。

如果 detach 线程就不用考虑异常问题了。由于分离了`std::thread`对象和线程本身，那么`std::thread`对象销毁的时候不会调用`std::terminate()`来终止程序，而对应的线程很可能仍旧在运行中。

###  Running threads in the background
通过调用`std::thread`的`detach()`方法使线程在后台运行，不可以再和其通信。也无法再获取与之关联的`std::thread`对象。后台运行的线程所有者是C++运行时（`C++ Runtime Library`），线程退出后，它负责回收相应的资源。

对应 UNIX 守护进程（`daemon process`）的概念，分离的线程称为守护进程（`daemon threads`）。这些线程和程序生命周期一样长，执行后台任务，比如清理缓存、监控文件变化等等。可以通过分离线程建立机制来确认线程是否完成任务，或者用于启动后不用管的任务。

调用`detach()`分离线程之后，那么线程和`std::thread`对象不再有联系，也不再能`join`。
```cpp
std::thread t(do_background_work);
t.detach();
assert(!t.joinable());
```
调用`detach()`条件和调用`join()`的条件一样，必须`t.joinable() `返回`true`，原因也是一样的，`std::thread`对象关联一个线程的时候才能`join()`或者`detach()`。

考虑一个文本编辑程序，可以同时打开多个文件。UI 层面和内部实现都可以有多种选择。每打开一个文件，有一个独立的窗口，是一个独立的线程，都在一个应用进程里面。内部实现可以很直接简单，每个线程完全独立，代码是共用的，但是处理完全不同的数据，很适合使用分离线程。当对应的窗口关闭的时候，线程自己退出就好了。下面是代码示例
```cpp
void edit_document(std::string const &filename)
{
    open_document_and_display_gui(filename);
    while (!done_editing())
    {
        user_command cmd = get_user_input();
        if (cmd.type == open_new_document)
        {
            std::string const new_name = get_filename_from_user();
            std::thread t(edit_document, new_name);
            t.detach();
        }
        else
        {
            process_user_input(cmd);
        }
    }
}
```
这个例子也展示了如何传递参数`new_name`到新的线程：通过`std::thread`的构造函数，这是一种最直接简单的方式。
