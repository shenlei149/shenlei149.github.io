有几种方式实现一个线程等待另一个线程完成工作这种场景。第一种是等待线程通过不停的轮询共享变量（通过互斥保护）来得知这个事情，工作线程一旦完成了，就设置这个变量。这有两个方面的浪费：第一个等待线程不停的轮询，浪费资源，可能使得工作线程工作的更慢；如果正在轮询（加了锁），那么工作线程将不得不等待等待线程释放之后再获得锁修改变量。

第二种方式是利用`std::this_thread::sleep_for()`等待一段时间再轮询。
```cpp
bool flag;
std::mutex m;
void wait_for_flag()
{
    std::unique_lock<std::mutex> lk(m);
    while (!flag)
    {
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
}
```
睡眠之前释放锁，线程唤醒之后再加锁轮询。

这会有很大的性能提升，因为睡眠的时候不会有资源浪费。睡眠时长是一个问题，如果太短，还是会浪费资源，如果太长，工作线程完成之后会有延迟才检查变量。大部分场景延迟问题不大，不过实时性要求高的场景就会是一个弊端。

更好的做法是使用c++标准库提供的条件变量（`condition variable`）让工作线程通知等待线程。一个条件变量可以看做是事件或者条件（`condition`），一个或多个线程可以等待这个条件被满足，一个线程一旦使之成立之后就可以通知（`notify`）等待在这个条件变量上的线程，这些等待线程继续它们的工作。

### Waiting for a condition with condition variables
C++标准库提供的条件变量有两个：`std::condition_variable`和`std::condition_variable_any`。后者更通用，需要的仅是`metux-like`对象即可，有潜在的附加开销，如果没有额外需要，使用`std::condition_variable`即可。

下面的示例展示了如何使用条件变量。
```cpp
std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;
void data_preparation_thread()
{
    while (more_data_to_prepare())
    {
        data_chunk const data = prepare_data();
        {
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(data);
        }

        data_cond.notify_one();
    }
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(
            lk, []
            { return !data_queue.empty(); });

        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);

        if (is_last_chunk(data))
            break;
    }
}
```
有一个队列用于在两个线程之间传递数据。生产者使用`std::lock_guard`加锁然后添加数据，解锁后调用`notify_one()`通知等待在条件变量`std::condition_variable`实例上的线程。我们是在解锁后再通知，好处是一旦唤醒了某个线程，不需要再次被`metux`阻塞住。

另外一侧，使用`std::unique_lock`加锁。然后调用`std::condition_variable`实例的`wait()`方法等待第二个参数（lambda 表达式）成立，即队列中有数据。

`wait()`的实现是说检查条件返回值。如果返回`false`，那么释放锁，进入睡眠等待通知。当生产者`notify_one()`会唤醒该线程，首先加锁，然后检查条件，如果`false`，释放锁继续等待，如果是`true`，那么已经获得了锁，执行后续的代码。因为`wait()`需要加锁和解锁，所以这里需要使用`std::unique_lock`而不是`std::lock_guard`，后者没有提供这种灵活性。如果`wait()`进入睡眠时不释放锁，那么生产者也无法获取锁把数据放入队列，进一步也就无法调用`notify_one()`来唤醒等待线程。

上面的例子中我们传递了一个 lambda 表达式，传递一个函数也是可以的。如果已经有现成的函数了，那么无需包裹成 lambda 表达式。`wait()`被调用期间，可能会多次检查条件变量（重新获取锁），一旦条件是`true`，会立即返回。等待线程重新获取锁检查条件，但是没有响应唤醒的线程，那么是虚假唤醒（`spurious wake`）。虚假唤醒次数和频率是无法确定的，所以检查条件的函数最好没有副作用，因为副作用会出现若干次。

`std::condition_variable::wait`是对忙等待的优化。一个非优化的实现可能就是一个简单的循环。
```cpp
template <typename Predicate>
void minimal_wait(std::unique_lock<std::mutex> &lk, Predicate pred)
{
    while (!pred())
    {
        lk.unlock();
        lk.lock();
    }
}
```
更好的实现是仅在`notify_one()`和`notify_all()`被调用的时候唤醒。

`std::unique_lock`的灵活性不仅体现在和`wait()`配合。上面的例子中我们在处理数据之前就释放锁，而处理数据可能需要点时间，我们遵守上一章提到的使锁的时间尽可能短的原则。

这个例子中，同步仅限于队列本身，可以有效的减少同步问题和条件竞争的可能性。

### Building a thread-safe queue with condition variables
3.2 小节介绍了线程安全的栈，这里我们考虑设计一个队列。先看下C++标准库是怎么设计接口的。
```cpp
template <class T, class Container = std::deque<T>>
class queue
{
public:
    explicit queue(const Container &);
    explicit queue(Container && = Container());
    template <class Alloc>
    explicit queue(const Alloc &);
    template <class Alloc>
    queue(const Container &, const Alloc &);
    template <class Alloc>
    queue(Container &&, const Alloc &);
    template <class Alloc>
    queue(queue &&, const Alloc &);
    void swap(queue &q);
    bool empty() const;
    size_type size() const;
    T &front();
    const T &front() const;
    T &back();
    const T &back() const;
    void push(const T &x);
    void push(T &&x);
    void pop();
    template <class... Args>
    void emplace(Args &&...args);
};
```
如果忽略构造函数、复制等无关函数，那么其他 APIs 可以分成三组。第一组是查询队列状态的，`empty()`和`size()`；第二组是获取队列元素`front()`和`back()`；第三组是修改队列，`push()`, `pop()`和`emplace()`。和栈类似，接口天生会引入竞争，所以我们需要把`front()`和`pop()`合并成一个接口。上个小节的例子中我们发现多线程使用队列往往是一个生产一个消费。这里我们提供两个`pop`的变种：`try_pop()`立即返回，告诉调用者是否取回了队列头的数据；`wait_and_pop()`阻塞式的，直到有数据再返回。下面是接口设计。
```cpp
#include <memory>
template <typename T>
class threadsafe_queue
{
public:
    threadsafe_queue();
    threadsafe_queue(const threadsafe_queue &);
    threadsafe_queue &operator=(
        const threadsafe_queue &) = delete;
    void push(T new_value);
    bool try_pop(T &value);
    std::shared_ptr<T> try_pop();
    void wait_and_pop(T &value);
    std::shared_ptr<T> wait_and_pop();
    bool empty() const;
};
```
和栈的设计一样，我们简化了构造函数的设计。`try_pop()`两个重载略微不同，由调用者提供`T`的引用的版本可以利用返回值`bool`表明是否有数据，无参版本（返回智能指针）只能通过指针是否为`nullptr`来判断是否取回了值。

这和上一个小节的生产者消费者是什么关系呢？我们把之前的代码拿过来实现`push()`和`wait_and_pop()`，并且给出用法示例。
```cpp
#include <queue>
#include <mutex>
#include <condition_variable>
template <typename T>
class threadsafe_queue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }
};

threadsafe_queue<data_chunk> data_queue;
void data_preparation_thread()
{
    while (more_data_to_prepare())
    {
        data_chunk const data = prepare_data();
        data_queue.push(data);
    }
}

void data_processing_thread()
{
    while (true)
    {
        data_chunk data;
        data_queue.wait_and_pop(data);
        process(data);
        if (is_last_chunk(data))
            break;
    }
}
```
这样，互斥和条件变量都被封装到了`threadsafe_queue`里面。

至此，实现其他函数就成为相对容易的事情了。
```cpp
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
template <typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue()
    {

    threadsafe_queue(threadsafe_queue const &other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};
```
即使`empty()`是`const`成员函数，拷贝构造函数参数`other`是`const`引用，但是调用的对象可能是非`const`的，还是可以调用修改队列的函数，所以需要加锁。加锁是修改`mutex`的操作，为了能在`const`成员函数中做修改操作，所以需要`mutable std::mutex mut;`这样声明成员变量。

多个线程等待在同一个条件变量上也是可以的。如果线程处理划分好的任务，那么代码和之前的示例一样，唯一的区别是可能会有多个处理数据的线程。当生产一个数据后，`notify_one()`会触发所有`wait()`的线程中的一个来检查条件变量。任意一个在等待状态的线程都有可能被唤醒。

另外一种场景是多个线程等待在同一个条件变量上但是都需要响应通知事件。比如第三章使用`std::call_once`的初始化问题（当然还是推荐使用`std::call_once`），或者是周期性的初始化共享数据。这时只需要调用`notify_all()`即可。所有等待的线程都会唤醒然后检查条件变量。
