上述的阻塞都是不确定时间的，对于很多场景这无所谓。但是有时我们需要告诉客户或者其他线程这个线程还活着，或者由于时间太长或者用户点了取消键需要提前结束任务。

通常有两种方式指定超时：一个时间周期，比如 30s，或者到某个时间点，比如 2023-10-11 00:00:00。前者的函数后缀为`_for`，后者的函数后缀是`_until`。

比如`std::condition_variable`就有额外两个版本的`wait()`重载：`wait_for()`和`wait_until()`。它们和`wait()`类似，挂起线程被通知或者超时了。

进入细节之前，先来看看在C++中如何表示时间。

### Clocks
就C++标准库而言，时钟（`clock`）是时间信息的来源，包含以下四个部分。
- 现在的时间`now`
- 用于表示从时钟获取的时间的类型
- 时钟 tick 周期
- 时钟的 tick 是否是一致的速率，是否能被认为是稳定的（`steady`）的时钟

`clock`类的`now()`用于返回当前时间，比如`std::chrono::system_clock::now()`返回系统时钟的当前时间。类型可以通过`typedef`的成员`time_point`获取，`some_clock::now()`的返回类型`some_clock::time_point`。

时钟 tick 周期表示秒的一个部分，通过`typedef`的成员`period`获得。比如一秒 25 次 tick 的周期是`std::ratio<1,25>`，每 2.5 秒一次 tick 的周期是`std::ratio<5,2>`。如果周期要到运行时才能知道，或者不用应用运行时有变化，那么`period`可以是最小周期，平均周期或者类库编写者认为合适的值。所以不保证应用观察到的周期和`period`给定的周期一致。

如果时钟 tick 是稳定的速率且不能被调整，我们就说该时钟是稳定的（`steady`）。静态方法`is_steady`会返回表示该时钟是否是稳定的。`std::chrono::system_clock`不是稳定的，可能会被自动调整本地时间偏移量。不稳定意味着调用`now()`可能会返回一个比之前调用`now()`返回的时间还早的时间。后续我们会看到稳定时钟对超时是很重要的，所以C++标准提供了`std::chrono::steady_clock`。除此之外，还提供`std::chrono::system_clock`表示当前“真实时间”，可以和`time_t`类型互转，还提供了`std::chrono::high_resolution_clock`保证了最小 tick 周期。它很可能是其他时钟的别名（通过`typedef`定义）。

### Durations
持续时长（`Duration`）用模板类`std::chrono::duration<>`表示。第一个模板参数底层表示这个`Duration`的类型，比如`int` `long` `double`等，第二个参数是分数，表示第一个参数对应的每单元是多少秒。比如用`short`存储分钟数，那么具体类型就是`std::chrono::duration<short,std::ratio<60,1>>`，第二个参数的含义是每 60 秒一次。使用`double`存储毫秒数，那么类型就是`std::chrono::duration<double,std::ratio<1,1000>>`。

标准库提供了一系列的别名，`anoseconds`, `microseconds`, `milliseconds`, `seconds`, `minutes`, `hours`. 使用足够 bits 的整数存储。足够 bits 的意思是可以表示 500 年的`Duration`。标准库还提供了从`std::atto`（$10^{-18}$）到`std::exa`（$10^{18}$）各种`ratio`定义，用于自定义类型。比如可以用`double`存储百分之一秒，那么具体类型就是`std::duration<double,std::centi>`。

`std::chrono_literals`命名空间给出了一些字面值常量后缀操作符，方便使用。
```cpp
using namespace std::chrono_literals;
auto one_day = 24h;
auto half_an_hour = 30min;
auto max_time_between_messages = 30ms;
```
如果和整数字面量一起用，那么和使用前面介绍的提供的别名是等价的，比如`15ns`和`std::chrono::nanoseconds(15)`是一样的。如果配合浮点数，那么是合适的浮点类型作为表示数据的类型，比如`2.5min`的浮点类型是不确定的`std::chrono::duration<some-floating-point-type, std::ratio<60,1>>`。如果你对具体类型比较关心，那么应该显式的定义具体类型而不是使用这些字面量。

如果不需要截断数值，那么`Duration`是可以隐式转化的，比如小时到秒，但是反之不行。我们可以通过`std::chrono::duration_cast<>`进行显式的转化：
```cpp
std::chrono::milliseconds ms(54802);
std::chrono::seconds s =
    std::chrono::duration_cast<std::chrono::seconds>(ms);
```
结果是截断而不是四舍不入，所以结果是 54 秒。

`Duration`可以加减或者乘除一个常数得到新的`Duration`。比如`5*seconds(1)`和`seconds(5)` `minutes(1) - seconds(55)`是一样的。`count()`获取当前`Duration`单位数，比如`std::chrono::milliseconds(1234).count()`是 1234。

需要`Duration`的`wait()`函数可以通过`std::chrono::duration<>`的实例实现。下面的例子是等 35 秒。
```cpp
std::future<int> f = std::async(some_task);
if (f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready)
    do_something_with(f.get());
```
`wait`函数返回一个状态，以此区别是超时还是等待的事件发生了。在这个例子中，我们等待一个`future`，`std::future_status::timeout`表示超时，`std::future_status::ready`和`std::future_status::deferred`分别表示`future`状态是`ready`和任务被推迟了。`wait`内部是使用稳定时钟来计算时间，所以不管系统时间在这个过程中怎么调整（提前或者推迟）都不影响等待时长。不过由于系统的调度和 OS 时钟的精度，或许比 35 秒稍长一点点。

### Time points
时钟的时间点（`time point`）用`std::chrono::time_point<>`表示。第一个模板参数是时钟类型，第二个是测量的单位（`Duration`的一种特化）。时间点的值是从某个称为 epoch （一般情况下是 1970年1月1日零点）的特殊点开始的长度。尽管无法得到具体的 epoch，但是可以通过`time_since_epoch()`得到一个`Duration`，是从 epoch 开始到指定的时间点的`Duration`。

比如有`std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>`类型的时间点，是用分钟为单位，这比系统时钟本身的精度要低很多。

在`std::chrono::time_point<>`实例上加减一个`Duration`，得到一个新的时间点，比如`std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(500)`表示未来 500 纳秒那个时间点。这常常用于知道了代码块最大超时时间来计算绝对时间点。

两个共享同一个时钟的时间点相减，会得到一个`Duration`。下面的示例是计算代码耗时
```cpp
auto start = std::chrono::high_resolution_clock::now();
do_something();
auto stop = std::chrono::high_resolution_clock::now();
std::cout <<"do_something() took "
          << std::chrono::duration<double, std::chrono::seconds>(stop - start).count()
          <<" seconds" << std::endl;
```
如果把时间点传递给`wait()`函数，时间点的时钟参数用于测量时间。时钟如果更改了，那么会影响程序行为，因为`wait`跟踪时钟的更新直到`now()`返回比指定之间更晚的值。如果时钟往前调整，会少等一会，反之会多等一会。

时间点可以用于`_until`系列的`wait`函数。惯用法是在某个固定点获取`some-clock::now()`之后加一个偏移量。比如，我们在某个条件变量上最多等待 500 毫秒，可以这么实现
```cpp
#include <condition_variable>
#include <mutex>
#include <chrono>
std::condition_variable cv;
bool done;
std::mutex m;
bool wait_loop()
{
    auto const timeout = std::chrono::steady_clock::now() +
                         std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lk(m);

    while (!done)
    {
        if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
            break;
    }

    return done;
}
```
这是没有传入谓词而在条件变量上等待限定时间的推荐做法，这样，循环时间是有限的。不传入谓词是为了处理虚假唤醒，但是使用`wait_for()`，那么每次会重新计时，可能会循环无限次，总时间就不是有限的了。

### Functions that accept timeouts
超时的最简单用途是为特定线程的处理添加延迟，这样当它无所事事时就不会占用其他线程的处理时间。当线程完成任务之后可以调用`std::this_thread::sleep_for()`和`std::this_thread::sleep_until()`等待，这样可以释放 CPU 资源。这就像一个闹铃一样：线程睡眠一段时间（`sleep_for()`）或者直到一个时间点（`sleep_until()`）然后被叫醒。周期性的睡眠然后检查某些东西，前者比较合适；后者主要用于调度某个线程在指定时间干一些事情，比如在午夜进行备份，或者在早晨打印点材料。

除了睡眠，之前也讲过条件变量和`future`可以用。除此之外，如果`metux`支持的话可以传入超时。`std::mutex`和`std::recursive_mutex`不支持，不过`std::timed_mutex`和`std::recursive_timed_mutex`支持，含义是在超时之内获取锁或者放弃获取锁。下表是支持超时相关的函数列表。其中*duration*和*time_point*需要传入`std::duration<>`和`std::time_point<>`的实例。

| Class/Namespace | Functions | Return Values |
|--|--|--|
| `std::this_thread` namespace | `sleep_for`(*duration*)<br> `sleep_until`(*time_point*) | N/A |
| `std::condition_variable` <br> `std::condition_variable_anywait` |`std::cv_status::timeout` <br> `std::cv_status::no_timeout` <br> `wait_for`(*lock*, *duration*, *predicate*) <br> `wait_until`(*lock*, *time_point*, *predicate*) | `bool` - the return value of the *predicate* when woken |
| `std::timed_mutex`, `std::recursive_timed_mutex` <br> `std::shared_timed_mutex` | `try_lock_for`(*duration*) <br> `try_lock_until`(*time_point*) | `bool` - `true` if the lock was acquired, `false` otherwise |
| `std::shared_timed_mutex` | `try_lock_shared_for`(*duration*) <br> `try_lock_shared_until`(*time_point*) | `bool` - `true` if the lock was acquired, `false` otherwise |
| `std::unique_lock<TimedLockable>` <br> `unique_lock`(*lockable*, *duration*) <br> `unique_lock`(*lockable*, *time_point*) | N/A - `owns_lock()` on the newly constructed object returns `true` if the lock was acquired, `false` otherwise <br> `try_lock_for`(*duration*) <br> `try_lock_until`(*time_point*) | <br> `bool` - `true` if the lock was acquired, `false` otherwise |
| `std::shared_lock`<SharedTimedLockable> <br> `shared_lock`(*lockable*, *duration*) <br> `shared_lock`(*lockable*, *time_point*) | N/A - `owns_lock()` on the newly constructed object returns `true` if the lock was acquired, `false` otherwise <br> `try_lock_for`(*duration*) <br> `try_lock_until`(*time_point*) | <br> `bool` - `true` if the lock was acquired, `false` otherwise |
| `std::future<ValueType>` <br> `std::shared_future<ValueType>` | `wait_for`(*duration*) <br> `wait_until`(*time_point*) | `std::future_status::timeout` if the wait timed out, <br> `std::future_status::ready` if the future is ready, <br> `std::future_status::deferred` if the future holds a deferred function that hasn't yet started |

