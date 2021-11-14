即使只有一个互斥，也需要确保所有操作都在互斥保护之内，同时还要考虑接口的设计不要引入固有的竞争。如果使用不同互斥保护不同的数据，那么问题会变得更复杂，如果需要多个互斥保护同一个数据，那么还要考虑死锁的问题。

下面从第三章介绍的最简单的线程安全的栈开始。

### A thread-safe stack using locks
我们把第三章的线程安全的栈的代码贴在下面，然后解释是否安全。
```cpp
#include <exception>

struct empty_stack : std::exception
{
    const char *what() const throw();
};

template <typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;

public:
    threadsafe_stack() {}
    threadsafe_stack(const threadsafe_stack &other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_stack &operator=(const threadsafe_stack &) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();

        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(data.top())));
        data.pop();

        return res;
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();

        value = std::move(data.top());
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
```
首先，每一个成员函数都通过锁住互斥 `m` 保护数据，那么基本安全是有的。这样一次只能有一个线程访问数据，每一个成员函数维护不变量，那么没有线程能破坏不变量。

在调用 `empty()` 和 `pop()` 之间有潜在的竞争风险，不过在 `pop()` 拿到锁之后，又检查了一下是否为空，那么这个问题就不存在了。`pop()` 将 `std::stack<>` 的 `pop(), top()` 结合起来，那么由于接口带来的竞争也不存在了。

加锁本身可能会抛出异常，这非常罕见，往往是系统资源导致的，不过这时我们还没有修改数据，所以是安全的。解锁不会失败，没问题。使用 `std::lock_guard<>` 的好处是不会使锁始终处于加锁的状态。

`data.push()` 可能会由于拷贝、移动数据时抛出异常、没有足够内存而出错，不过 `std::stack<>` 会保证不会出错。

第一个重载版本的 `pop()` 可能会抛出一个空栈的异常，不过这时还没有修改数据，所以没有问题。创建 `res` 的时候也可能抛出异常，可能是构造 `std::make_shared` 时没有空间，也有可能是移动、构造数据对象的时候抛出异常。不过不管是什么情况，C++ 运行时和标准库都会确保没有内存泄露。而此时还没有修改栈，所以也没有问题。`data.pop()` 不会有异常，能够正常返回。所以这个版本的 `pop()` 是异常安全的。

第二个版本的 `pop()` 类似，移动或者构造数据对象可能会抛出异常，不过在调用 `data.pop()` 之前都不会修改数据结构，所以也是异常安全的。

最后，`empty()` 没有修改数据，所以也是异常安全的。

这里有死锁的风险。因为我们在移动、构造数据对象的时候，本质上是调用了用户的代码，而这时是持有锁的。如果用户的代码又调用了线程安全栈的成员函数，那么就会死锁。这是可以理解的，用户应该有责任避免这种情况。

所有的成员函数都使用了 `std::lock_guard<>`，所以任意线程访问都是安全的。不过构造函数和析构函数是例外。但这不是问题，因为对象只能构造和析构一次，不管是否是线程安全的对象，对一个没有完全构造或者开始部分析构的对象调用其他成员函数，都是不明智的行为。用户自己必须确保这一点。

尽管这个实现是安全的，但是每一次只能有一个线程修改数据。线程的串行化会导致性能比较差，特别是多个线程竞争的时候，等待的线程什么也做不了。另外对于要消费数据的线程而言，没有很好的等待机制，等待的线程只能频繁调用 `empty()` 或者调用 `pop()` 并捕获 `empty_stack`。下面线程安全的队列很好地解决了这个问题。

### A thread-safe queue using locks and condition variables
下面分析第四章介绍的线程安全的队里。和线程安全的栈类似，我们的实现封装了 `std::queue<>`，修改接口以避免竞争。
```cpp
template <typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() {}

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;

        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();

        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
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
除了 `push()` 中调用了 `data_cond.notify_one()` 和 `wait_and_pop()` 函数之外，和上一个小节栈都一样。`try_pop()` 函数也基本类似，不过不会抛出空容器异常，而是返回 `bool` 表示是否成功了，或者是是否是空指针。所以除了 `wait_and_pop()` 之外，上一节中其他函数的分析也适用于这里。

`wait_and_pop()` 解决了上一节描述的忙等待的问题，`data_cond.wait()` 直到有数据了才会返回，所以不用担心队列为空，同时数据仍旧处于互斥的保护之下。所以这个函数不会增加新的条件竞争、死锁等问题，不变量也不会被破坏。

不过有一个问题值得关注，当添加了一个新的数据，`data_cond.notify_one()` 被调用，一个线程被唤醒，但是构造 `std::shared_ptr<>` 的时候抛出了一场，那么其他线程也不会被唤醒。如果这不可以接受的话，可以调用 `data_cond.notify_all()` 唤醒所有的进行，不过也只有一个会处理数据，其他会再次睡眠，浪费资源。第二个可选的方法是如果 `wait_and_pop()` 中有异常，继续调用`notify_one()` 唤醒其他线程处理。第三种方式是把构造 `std::shared_ptr<>` 放到 `push()` 中，也就是保存的是持有数据的智能指针而不是数据本身。从 `std::queue<>` 复制 `std::shared_ptr<>` 不会有异常，所以 `wait_and_pop()` 是安全的。下面是基于这种思想改写的线程安全队列。
```cpp
template <typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() {}

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = std::move(*data_queue.front());
        data_queue.pop();
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;

        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();

        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    void push(T new_value)
    {
        std::shared_ptr<T> data(
            std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};
```
使用 `std::shared_ptr<>` 存储数据有一个额外的好处：现在构造数据可以在 `push()` 的锁外面，而之前必须在 `pop()` 的锁里面。分配内存需要一点时间，现在就减少了互斥锁的范围，潜在提高了并发能力。

和线程安全的栈一样，由于只有一个 `std::queue<>` 要么保护起来，要么没有保护，所以多个线程的操作需要串行化，只有一个线程能访问数据。

### A thread-safe queue using fine-grained locks and condition variables
如果我们要使用细粒度的锁，那么我们需要仔细考察一下队列这个数据结构。

最简单的队列是一个单链表。有一个 head 指针指向队列的第一个元素，删除的时候把 `head` 往后移动一个即可。还有一个 `tail` 指针指向队列的最后一个元素，新增的时候构造一个新节点，最后一个节点指向新节点，然后移动 `tail` 指针。

下面是一个简单的实现，只有 `try_pop()` 没有 `wait_and_pop()` 的原因是这个队列只能单线程运行。
```cpp
template <typename T>
class queue
{
private:
    struct node
    {
        T data;
        std::unique_ptr<node> next;
        node(T data_) : data(std::move(data_))
        {
        }
    };
    std::unique_ptr<node> head;
    node *tail;

public:
    queue() : tail(nullptr) {}
    queue(const queue &other) = delete;
    queue &operator=(const queue &other) = delete;

    std::shared_ptr<T> try_pop()
    {
        if (!head)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(head->data)));
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        if (!head)
            tail = nullptr;

        return res;
    }

    void push(T new_value)
    {
        std::unique_ptr<node> p(new node(std::move(new_value)));
        node *const new_tail = p.get();
        if (tail)
        {
            tail->next = std::move(p);
        }
        else
        {
            head = std::move(p);
        }

        tail = new_tail;
    }
};
```
首先我们使用 `std::unique_ptr<node>` 来保存数据，当不再使用的时候，会自动释放。所有权由 `head` 节点串成一个链，所以 `tail` 是裸指针类型。

如果在这个实现上使用细粒度的锁以支持多线程访问会有些问题。我们有两个数据项（`head` 和 `tail`），那么理论上可以用两个互斥分别保护这两个数据。

最明显的问题是 `push()` 同时修改 `head, tail`，那么需要同时加两个锁。这一点问题不大，可以做到。更严重的问题是，`push(), pop()` 可能会访问同一个节点的 `next` 指针：如果 `tail == head`，也就是链表只有一个节点，`push()` 访问 `tail->next` `try_pop()` 访问 `head->next`，这两个 `next` 是同一个对象。那么在这两个函数里面也不得不加两个锁，那么细粒度锁就无效了。

#### ENABLING CONCURRENCY BY SEPARATING DATA
利用一个假的节点即可解决这个问题。对于空队列，`head, tail` 指向假的节点而不是 `NULL`。这样 `try_pop()` 就不需要在空的时候访问 `head->next`。负面影响是不得不再套一层指针来访问数据。这里本质是构造假的节点需要用到 `T` 的无参构造函数，而这一点无法保证。
```cpp
template <typename T>
class queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head;
    node *tail;

public:
    queue() : head(new node), tail(head.get()) {}
    queue(const queue &other) = delete;
    queue &operator=(const queue &other) = delete;

    std::shared_ptr<T> try_pop()
    {
        // 由于存在假节点，这里修改成比较 head 和 tail 是否指向同一个节点
        if (head.get() == tail)
        {
            return std::shared_ptr<T>();
        }

        // 由于存的就是智能指针，这里可以直接获取不用构造
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }

    void push(T new_value)
    {
        // 这里不得不先构造一个 shared_ptr
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));

        // 构造一个新的假节点
        std::unique_ptr<node> p(new node);

        // 把值赋给旧的假节点
        tail->data = new_data;
        node *const new_tail = p.get();

        // 指向新的假节点
        tail->next = std::move(p);
        tail = new_tail;
    }
};
```
现在，`push()` 只需要访问 `tail` 而不再需要访问 `head`。`try_pop()` 需要访问 `head` 和 `tail`，不过后者只用于比较，那么加锁范围更少。更重要的是，`push()` 和 `pop()` 不会访问同一个节点，那么不再需要一个互斥负责所有的事情。

`push()` 加锁的范围比较容易确定，我们目的是保护 `tail`，那么从修改 `tail` 开始之前就加锁，到不再使用 `tail` 的时候释放锁，即到函数结束。

`try_pop()` 稍微复杂一点。我们先看 `head`，拿到锁的线程能够修改数据，所以在使用 `head` 之前加锁。一旦修改完成，也就是在返回结果之前，就能释放锁。修改 `head` 封装成一个函数会比较简洁，不用手动释放。对于 `tail` 而言，我们只需要访问一次，即读之前加锁，读完之后即可释放锁。也可以封装成一个函数。
```cpp
template <typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node *tail;

    node *get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);

        if (head.get() == get_tail())
        {
            return nullptr;
        }

        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

public:
    threadsafe_queue() : head(new node), tail(head.get()) {}
    threadsafe_queue(const threadsafe_queue &other) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node *const new_tail = p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};
```
开始分析之前先看下关于 `list` 实现的一些事实。
* `tail->next == nullptr`
* `tail->data == nullptr`
* `head == tail` 意味着列表为空
* `head->next == tail` 意味着只有一个元素
* 列表中的每个 `x`，如果 `x != tail`，`x->data` 指向 `T` 的一个实例，`x->next` 指向下一个节点。如果 `x->next == tail`，那么 `x` 是最后一个节点。
* 从 `head` 开始寻找 `next`，最终会走到 `tail`

`push()` 的实现比较直接：`tail_mutex` 保护数据结构的修改。

`try_pop()` 复杂一些，不仅要对 `tail_mutex` 加锁，还需要保护 `head`。`try_pop()` 和 `push()` 可能被不同的线程同时调用，如果不对 `tail_mutex` 加锁，那么两个线程可能访问同一个数据，却没有指定顺序，那么就有发生竞争的可能性。`get_tail()` 对 `tail_mutex` 加锁，那么 `try_pop()` 中访问的 `push()` 之前的旧 `tail` 或者 `push()` 之后的新 `tail`，但不会有未定义的行为。

`get_tail()` 调用是在对 `head_mutex` 加锁之后。

暂停下这本书，因为工作中暂时用不到~
