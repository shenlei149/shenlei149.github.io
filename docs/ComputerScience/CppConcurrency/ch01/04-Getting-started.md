多线程C++程序和一般的C++程序很像，除了有的函数可能会同时运行罢了。为了让函数并发执行，需要使用一些特殊的函数和对象来控制不同的线程。

### Hello, Concurrent World
从改写经典的Hello World程序开始：
```cpp
#include <iostream>
int main()
{
    std::cout << "Hello World\n";
}
```
使用不同的线程来输出信息：
```cpp
#include <iostream>
#include <thread>

void hello()
{
    std::cout << "Hello Concurrent World\n";
}

int main()
{
    std::thread t(hello);
    t.join();
}
```
第一处不同是增加了`#include <thread>`，管理线程的函数和类都在这个头文件里面。

第二个不同是输出放在了一个单独的函数里面，这是因为每个线程需要一个初始函数（`initial function`）。初始线程的初始函数是`main()`，其他线程需要显式地在构造时指定入口函数。名为`t`的`std::thread`对象的初始函数是`hello()`。

第三处不同是如果直接在`main()`里面调用`hello()`，那么从始至终只有一个线程。现在有两个线程：分别是`main()`作为入口的初始线程和以`hello()`为入口的新线程。

新线程启动之后，原来的线程继续执行。在上面的例子，如果`main()`不等待新的线程结束就一路执行下去，那么新线程可能没有执行机会就运行到了最后，程序退出了。这正是调用`join()`的原因，令当前线程等待关联的线程，这里是`t`线程。
