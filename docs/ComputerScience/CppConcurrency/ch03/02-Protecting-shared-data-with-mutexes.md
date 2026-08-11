回到双向列表的例子，如果我们可以把访问数据结构的代码都在多线程上互斥（`mutually exclusive`），只能有一个线程进行操作。这样不变量就不会被破坏。

这不是天方夜谭！应用互斥（`mutex (**mut**ual **ex**clusion)`）的同步原语（`synchronization primitive`）就能做到。访问共享数据之前，先锁（`lock`）住互斥，结束之后再解锁（`unlock`）互斥。线程库保证了一个线程锁住了某个互斥，其他线程再试图加锁，必须等待，直到成功加锁的线程解锁。这就保证了共享数据是一致的，不会破坏不变量。

互斥是C++最通用的保护措施，但也不是银弹。务必妥善组织代码，从而正确的保护共享数据，同时避免固有的条件竞争。互斥本身也有自己的问题，会死锁，会存在过保护或欠保护。

### Using mutexes in C++
`std::mutex`构造函数能得到互斥的实例，成员函数`lock()`和`unlock()`能够加锁和解锁。但是不推荐裸用，因为需要在所有路径确保解锁。线程库提供了`std::lock_guard`可以通过RAII确保资源释放：构造的时候加锁，析构的时候解锁。下面是保护双向链表的例子。
```cpp
#include <list>
#include <mutex>
#include <algorithm>
std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}
bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}
```
`some_mutex`是保护`some_list`的。

C++17 引入了模板类型推导，可以简化成
```cpp
std::lock_guard guard(some_mutex);
```
后续会看到C++17 提供了加强版本的`std::scoped_lock`，所以这里更应该写作
```cpp
std::scoped_lock guard(some_mutex);
```
这个例子用了全局变量，更好的做法是封装成类。两者在同一类中，清晰的表示谁保护谁，同时封装函数以增强保护。成员函数访问前先加锁，完成后解锁，以此保护共享数据。

如果成员函数返回了共享数据的指针或者引用，那么类实现的再好也无济于事。外部代码可以通过指针或引用修改数据而绕过了互斥。所以需要谨慎设计接口。

### Structuring code for protecting shared data
如果我们通过成员函数向外返回了被保护的共享数据的指针或者引用，那么保护就有漏洞了。幸好我们可以检查所有函数的返回类型。不过这远远不够，我们也不应该向在加锁范围内的函数传递共享数据的指针或者引用，因为你不知道被调用的函数会不会保存这个指针或者引用，之后在使用就会绕过保护。下面是代码示例
```cpp
class some_data
{
    int a;
    std::string b;

public:
    void do_something();
};

class data_wrapper
{
private:
    some_data data;
    std::mutex m;

public:
    template <typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m);
        func(data);
    }
};

some_data *unprotected;

void malicious_function(some_data &protected_data)
{
    unprotected = &protected_data;
}

data_wrapper x;

void foo()
{
    x.process_data(malicious_function);
    unprotected->do_something();
}
```
`foo`函数就能够绕过保护做自己想做的事情，而无需互斥锁。不幸的是C++ 线程库并不能帮助我们，我们需要自己注意：不要返回需要保护的共享数据的指针或引用，也不要向其他函数传递。

这个例子不是唯一潜在的陷阱。即使有了互斥保护，但是还是可能会有条件竞争。

### Spotting race conditions inherent in interfaces
回到前面删除双向链表某个节点的例子，我们需要给三个节点加锁（要删除的和两侧的相邻节点）。如果只保护当前节点，那么和没有使用互斥一样，条件竞争仍旧可能发生。如果一个操作涉及多个数据，最简单的方式就是给整个数据结构加锁。比如前面保护整个`list<int>`的例子。

单一操作是安全的，不能保证不会遇到条件竞争。比如下面的例子，是C++ `std::stack`的适配器代码。即使我们遵循上一小节说的不要返回引用，修改`top()`的实现，并且每个函数都使用互斥保护内部数据，使用这些接口还是会遇到条件竞争。这是接口本身的问题。有互斥的版本会遇到，无锁编程的实现也会遇到。
```cpp
template <typename T, typename Container = std::deque<T>>
class stack
{
public:
    explicit stack(const Container &);
    explicit stack(Container && = Container());
    template <class Alloc>
    explicit stack(const Alloc &);
    template <class Alloc>
    stack(const Container &, const Alloc &);
    template <class Alloc>
    stack(Container &&, const Alloc &);
    template <class Alloc>
    stack(stack &&, const Alloc &);
    bool empty() const;
    size_t size() const;
    T &top();
    T const &top() const;
    void push(T const &);
    void push(T &&);
    void pop();
    void swap(stack &&);
    template <class... Args>
    void emplace(Args &&...args);
};
```
问题在于`empty()`和`size()`结果不可行。尽管对于某个线程调用时返回值是正确的，但是函数一旦返回，其他线程就不会限制可以增删元素了。这样第一个线程的结果就不正确了。

如果`stack`实例不共享，那么检查`empty()`之后调用`top()`是安全的。类似下面
```cpp
stack<int> s;
if (!s.empty())
{
    int const value = s.top();
    s.pop();
    do_something(value);
}
```
在空的栈上调用`top()`是未定义行为，因此上面的代码做了检查。单线程的时候上述代码是可以正常工作的。但是多线程就不行了。因为`empty()`和`top()`之间的时候，另一个线程调用了`pop()`使之为空。这就是条件竞争。原因就是接口本身，内部使用了互斥保护栈容器也无法避免。

如何解决问题？这个是接口设计的问题，那么必须要更新接口。最简单的方式是如果为空那么`top()`就抛出异常。但是这样在非空时也需要能接住异常——在栈已经空了的时候，调用`empty()`是一个优化手段，避免抛出异常；如果不空，由于`empty()`和`top()`之间可能状态改变，导致仍旧抛出异常。

仔细观察代码，在`top()`和`pop()`之间，也可能有条件竞争。比如初始栈有两个元素，现在有两个线程执行上述代码。但是如果按照下面的顺序执行：
| Thread A | THread B |
|--|--|
| `if(!s.empty())` | |
| | `if(!s.empty())` |
| `int const value=s.top();` | |
| | `int const value=s.top();` |
| `s.pop();` | |
| `do_something(value);` | |
| | `s.pop();` |
| | `do_something(value);` |

那么两个线程得到的值是同样的，第二个值没有被用到就被丢弃了。这相比`empty()`和`top()`竞争导致的未定义行为。这个问题更隐蔽，原因不明显，与后果联系也不易被察觉，后果与`do_something()`的实现有关。

需要从根本上更改接口设计，避免这些问题，一种改法是把`top(),pop()`组成一个函数，再采取互斥保护起来。Tom Cargill 指出，如果栈容器的拷贝构造函数抛出异常，那么这个新函数会出现问题。Herb Sutter 相当全面的解决了这个问题。虽然条件竞争的隐患依旧存在，且给新的函数引入了新问题。

考虑栈`stack<vector<int>>`，当用户`pop()`的时候，返回对象需要复制`vector`给调用者，如果系统负载很大，内存不足，那么会抛出异常`std::bad_alloc`，但是这个对象已经存栈顶移除了。这就导致数据丢失。这也是为什么`std::stack`的设计者分成了两个操作，获取对象`top()`，移除栈顶`pop()`，如果拷贝失败了那么数据还在栈上。

现在进入了两难的境地。不过还是有一些方式消除条件竞争，不过不完美。

#### OPTION 1: PASS IN A REFERENCE
外部传递一个引用来接受数据。
```cpp
std::vector<int> result;
some_stack.pop(result);
```
大部分情况行之有效，但是也有一些问题。调用`pop()`之前需要构造一个实例，传入函数。对于某些类型，构建实例的时间和代价比较大。其次是对象的构造函数需要参数，这就不总是可行了。最后，这种方法需要对象能够赋值。许多自定义类型不支持赋值，但是可以移动构造甚至是拷贝构造（`pop()`可以按值返回）。

#### OPTION 2: REQUIRE A NO-THROW COPY CONSTRUCTOR OR MOVE CONSTRUCTOR
限制容器用途，只存储提供了无异常的拷贝构造或者移动构造的类型。虽然使用`std::is_nothrow_copy_constructible` `std::is_nothrow_move_constructible`可以方便的在编译器做判断，但是总归是一种限制，线程安全的栈容器无法存储一些类型，是在可惜。

#### OPTION 3: RETURN A POINTER TO THE POPPED ITEM
返回指向栈顶的指针，不返回值。这可以解决问题，不过缺点也很明显。返回指针需要管理内存分配问题，且存储简单类型，比如`int`，开销远远超过直接返回值。`std::shared_ptr`是一个比较好的选择，不会内存泄露，且分配策略由C++控制，不必使用`new`和`delete`。

#### OPTION 4: PROVIDE BOTH OPTION 1 AND EITHER OPTION 2 OR 3
代码应该有灵活性，泛型代码更应如此。可以选择2或者3，再提供方法1。给用户更多选择，也可以挑选更适合的方法，只承担很小的额外开销。

#### EXAMPLE DEFINITION OF A THREAD-SAFE STACK
下面是栈容器类的定义，没有条件竞争的接口设计，`pop()`有两个重载，分别实现了1和3。
```cpp
#include <exception>
#include <memory>
struct empty_stack : std::exception
{
    const char *what() const noexcept;
};
template <typename T>
class threadsafe_stack
{
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack &);
    threadsafe_stack &operator=(const threadsafe_stack &) = delete;
    void push(T new_value);
    std::shared_ptr<T> pop();
    void pop(T &value);
    bool empty() const;
};
```
接口简化能够确保安全，还能限制容器的整体操作。赋值运算符被删除了，那么容器本身不能被赋值。如果容器元素能够复制，那么容器能被复制。空的栈上调用`pop()`会抛出`empty_stack`异常，即使调用`empty()`之后容器发生了变化也能正常工作。接口简化确保安全指的是能够确保互斥在整个操作上都是加锁的。下面是实现代码，包装了`std::stack<>`。
```cpp
#include <exception>
#include <memory>
#include <mutex>
#include <stack>
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
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
```
复制构造函数首先锁住源对象，然后复制其内部的数据。没有使用类成员初始化列表而是在构造函数内部操作，就是为了能够确保互斥锁能够覆盖整个复制过程。

正如前面讨论到的`top()`和`pop()`的锁的粒度太小，不能保护数据。反之，如果太大，也会有问题，极端情况就是一个锁保护所有的共享数据。如果一个系统有大量共享数据，这会消除并发带来的性能提升，因为最多只能有一个线程访问共享数据，即使是访问不同的共享数据。Linux 第一版支持多处理器就是一个全局内核锁，性能非常差。

选择合适的粒度可能会导致在一个操作中需要多个互斥，这是你可以加大锁的粒度，一个锁来处理所有共享数据。但是需要保护的一个类的不同实例，那么不期望这么做。结果是提高保护层次让用户去做或者是一个锁保护所有实例，但都不是特别好。

一个操作需要多个互斥，可能还有一个问题：死锁（`deadlock`）。

### Deadlock: the problem and a solution
考虑两个线程都需要两个互斥，每一个线程都锁住了其中一个等待另外一个，这就会导致死锁。

一个通用的做法是以相同的顺序加锁。但是有时做不到这一点。比如交换两个对象，需要锁住这两个对象，代码实现是先锁左参再锁有参。如果两个线程，调用该方法传入同样两个对象，但是传参顺序恰好相反，那么还是有可能死锁。

C++ 提供了`std::lock`，可以同时锁住多个互斥而没有死锁风险。下面是交换操作的例子。
```cpp
class some_big_object;
void swap(some_big_object &lhs, some_big_object &rhs);
class X
{
private:
    some_big_object some_detail;
    std::mutex m;

public:
    X(some_big_object const &sd) : some_detail(sd) {}

    friend void swap(X &lhs, X &rhs)
    {
        if (&lhs == &rhs)
            return;

        std::lock(lhs.m, rhs.m);
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        swap(lhs.some_detail, rhs.some_detail);
    }
};
```
首先需要判断两个对象是否是同一个，这个是必须的，因为对`std::mutex`加锁之后再加锁的行为是不确定的（3.3.3 会介绍`std::recursive_mutex`，允许多次加锁）。接着对两个对象的互斥加锁，然后构造两个`std::lock_guard`，为了确保锁一定能够释放。`std::adopt_lock`的意思是互斥已加锁，不用在构造函数里面继续加锁了，过继（`adopt`）加锁的互斥所有权即可。

`std::lock`内部如果抛出异常，那么异常会传导出来。如果`std::lock`先给第一个互斥成功加锁，但是给第二个加锁的时候抛出异常，会释放第一个锁。保证了`all-or-nothing`的语义。

C++17 提供了`std::scoped_lock<>`，等价于`std::lock_guard<>`，不过是变参模板，这样它自身就可以支持多个参数，即给多个互斥加锁。配合C++17 的模板参数推导，那么代码可以简化为
```cpp
void swap(X &lhs, X &rhs)
{
    if (&lhs == &rhs)
        return;

    std::scoped_lock guard(lhs.m, rhs.m);
    swap(lhs.some_detail, rhs.some_detail);
}
```
这样更不容易出错。

`std::lock`和`std::scoped_lock`对于同时上锁的场景可以避免死锁，但是如果必须在不同的地方加锁就无能为力了。这需要程序员来确保不会死锁。

### Further guidelines for avoiding deadlock
即使不用锁，也可以发生死锁。比如两个线程，启动之后都`join()`了对方，等待对方结果，那么就死锁了。所以本质原则就是不能形成环。下面分成几个具体原则。

#### AVOID NESTED LOCKS
如果已经持有锁了，那么就不要再获取锁了。保持这个原则，锁相关的情况不会再死锁了，不过线程相互等待还是可能发生。如果需要多个锁，尽可能使用`std::lock`一次性获取。

#### AVOID CALLING USER-SUPPLIED CODE WHILE HOLDING A LOCK
用户定义的代码，你不能确定是什么行为，那么在调用前加了锁，用户代码继续加锁，那么就违反了上一个原则。但是对于写泛型代码，比如之前的栈容器，不可避免这一点，那么需要下面这个原则。

#### ACQUIRE LOCKS IN A FIXED ORDER
在不同的线程之间，定义加锁顺序。

回到操作双向链表的例子。不同操作需要访问多余一个节点，比如删除操作需要访问三个节点。或者是遍历操作，锁当前节点，访问，然后试图获取下一个节点的锁，一旦获取了，那么可以释放当前节点的锁。

如果两个线程分别从不同方向遍历，那么在相遇的时候就会发生死锁。所以规定顺序是很重要的。类似的，删除操作先对$A,C$之间的$B$加锁，然后尝试获取剩余节点的锁，同时有一个遍历操作，已经对$A$或者$C$加锁了（依赖于遍历方向），尝试获取$B$的锁，死锁了。

定义一个顺序，比如必须按照$A,B,C$的顺序进行，不允许反向遍历，那么就不会有死锁了。

#### USE A LOCK HIERARCHY
按层次划分的锁就是特定顺序加锁的特例，可以在运行时检查加锁操作是否按照预设的规则。我们把应用划分成若干层，明确每个互斥位于哪个层次。某个线程对低层次互斥加锁了，那么就不能对高层次互斥加锁。为每个互斥分配一个编号，并且记录一个线程加了哪些层次的互斥锁。这种模式很常见，但是C++ STL 没有提供支持，我们需要自己实现`hierarchical_mutex`。

下面是使用`hierarchical_mutex`的示例。
```cpp
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);
int do_low_level_stuff();
int low_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_stuff(int some_param);
void high_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}

void thread_a()
{
    high_level_func();
}

void do_other_stuff();
void other_stuff()
{
    high_level_func();
    do_other_stuff();
}

void thread_b()
{
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}
```
有三个`hierarchical_mutex`实例，传入了相应的编号（层次越低编号越小）。如果我们已经在某个`hierarchical_mutex`上加了锁，那么只能继续获取更低层级的互斥锁。

假定`do_low_level_stuff()`没有锁任何互斥。`low_level_func()`位于最底层，且需要锁住`low_level_mutex`，`high_level_func()`先对`high_level_mutex`加锁，然后调用`low_level_func()`，这是符合加锁规则的。

`thread_a()`做同样的事情，也能顺利运行。

`thread_b()`就违反了相应的规则。首先对`other_mutex`加锁，编号是6000，位于中间层级。然后调用`high_level_func()`，这是不对的，因为其编号是10000，高于6000。`hierarchical_mutex`会抛出异常，也可能终止程序。层次之间不可能发生死锁。位于同一层级也不能同时加锁。对于前面链表的例子，每个节点的层次比其前驱节点的要低。

这个例子还演示了`std::lock_guard<>`和自定义互斥类型`hierarchical_mutex`联合使用。只需要实现`lock(),unlock(),try_lock()`三个函数就可以作为`std::lock_guard<>`模板类型使用。`try_lock()`函数和`lock()`类似，不过如果互斥已经加过锁了，那么直接返回`false`，不等待。`std::lock()`内部也是利用这个函数防范死锁。

`hierarchical_mutex`使用了线程专属（`thread_local`）的局部变量来存储当前层次的编号。所有的互斥实例都能访问这个值，不过每个线程的这个值可能不同。这样，每个线程各自检查其自身行为。
```cpp
class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation()
    {
        if (this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit hierarchical_mutex(unsigned long value) : hierarchy_value(value),
                                                       previous_hierarchy_value(0)
    {
    }

    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated");
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    bool try_lock()
    {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};

thread_local unsigned long
    hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);
```
这里的关键是使用`thread_local`修饰的线程专属变量`this_thread_hierarchy_value`来表示当前线程的层次编号。初始值是最大值，那么任意互斥都能够加锁。每一个线程有自己的`this_thread_hierarchy_value`，各个线程之间这个值都不相关。

第一上锁能够顺利通过检查，`lock()`内部加锁，然后更新层次编号。

后续获取锁的必要条件就是层次编号比`this_thread_hierarchy_value`小才可以。

保存上一个层次值，以便在`unlock()`中恢复。否则我们就无法在获取较高层次的锁了。我们这里使用`internal_mutex`来保护内部变量，所以要在解锁前恢复上一个层次的值。解锁的时候要按照逆序完成，如果发现当前要解的锁层次编号不是当前线程的层次编号，也抛出异常。

`try_lock()`类似的，如果获取锁失败，返回`false`，不更新相关数据结构。

#### EXTENDING THESE GUIDELINES BEYOND LOCKS
前面说过，没有锁也可能会有死锁问题，只要有循环等待就有可能发生死锁。这些原则也适用于其他情况。比如线程等待也可以有层级，只有高层级的等待地层级的线程完成。比如一个函数创建所有工作线程并等待结束。

`std::lock()`和`std::lock_guard`可以处理大部分情况。不过有时需要更多的灵活性，那么`std::unique_lock`就派上用场了。

### Flexible locking with std::unique_lock
`std::unique_lock`相比`std::lock_guard`提供了更多的灵活性，不一定占用了与之关联的互斥。构造函数的第二个参数可以传入`std::adopt_lock`，让其管理锁，也可以传入`std::defer_lock`，保持未加锁的状态。我们可以延迟调用`std::unique_lock`的`lock()`方法，或者将`std::unique_lock`实例传入`std::lock()`加锁。我们可以改写之前的例子：
```cpp
class some_big_object;
void swap(some_big_object &lhs, some_big_object &rhs);
class X
{
private:
    some_big_object some_detail;
    std::mutex m;

public:
    X(some_big_object const &sd) : some_detail(sd) {}
    friend void swap(X &lhs, X &rhs)
    {
        if (&lhs == &rhs)
            return;

        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
        std::lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
};
```
`std::unique_lock`相比`std::lock_guard`空间更大，也略慢一点。这就是灵活的代价：需要记录是否拥有互斥且运行时需要更新。

`std::unique_lock`实现了`lock(), unlock(), try_lock()`，所以可以传递给`std::lock()`加锁。`std::unique_lock`内部维护是否拥有关联的互斥，在析构的时候根据这个信息是否调用`unlock()`。这个信息可以通过`owns_lock()`查询。一般情况使用`std::scoped_lock`或者`std::lock_guard`即可。除非是想获得延迟加锁的灵活性，或者是转移锁的所有权。

### Transferring mutex ownership between scopes
`std::unique_lock`不拥有与之关联的互斥，所有可以通过移动转移互斥的所有权。`std::unique_lock`也是一个可以移动但是不能复制的例子。

一个有用的操作是允许函数加锁对应的互斥，然后转移给调用者，那么调用者可以使用同一个互斥的保护下进行后续操作。`get_lock()`加锁，准备数据，然后把锁返回给调用者继续处理数据。
```cpp
std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    return lk;
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());
    do_something();
}
```
`get_lock()`返回值自动使用移动转移所有权。`process_data()`在构造的时候转移了所有权。

这个模式用于加锁是依赖于程序自身的状态，或者是传入返回`std::unique_lock`对象的函数的参数。通常的做法不是使用返回的锁本身而是使用把锁作为成员变量的`gateway`类实例。比如`get_lock()`返回的是`gateway`的实例，内部已加锁，然后通过`gateway`的成员函数访问数据，最后`gateway`实例销毁的时候自动释放锁。这样，就需要`gateway`是可移动的，那么作为成员变量的锁页需要可移动。

`std::unique_lock`还允许在对象销毁前调用`unlock()`释放锁。同时，实现了三个接口，所以可以传递给诸如`std::lock`这样的泛型函数使用。提前释放锁能够更好的控制锁的粒度。

### Locking at an appropriate granularity
粒度精细的锁保护少量数据，粗粒度的锁保护大量数据。选择锁的粒度很重要，既要保证数据得到充分的保护，又要只在需要的时候加锁。

多线程中如果使用同一个资源，如何线程在非必要的线程之外加锁，就会增加等待时间。如果可能，只在访问共享数据的时候加锁，其他处理的时候不用锁保护，特别是耗时的I/O操作。

这种情况就适合使用`std::unique_lock`，只在需要的地方加锁。
```cpp
void get_and_process_data()
{
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process = get_next_data_chunk();
    my_lock.unlock();
    result_type result = process(data_to_process);
    my_lock.lock();
    write_result(data_to_process, result);
}
```
假定`process()`函数不需要互斥锁的保护，那么在其前后先解锁再加锁，保证锁的粒度最小。

如果一个互斥锁保护整个数据结构，结果是可能加剧锁的竞争，同时很难减少持锁的时间。如果某个操作需要全程加锁，那么其做的事情越多，持有锁的时间越长。不利因素加倍了，我们需要改用粒度精细的锁。

一般地，我们只在所需的最短时间内加锁。除非必要，绝不加锁。

之前交换数据的例子中，必须给两个对象加锁。如果是比较呢？比较对象是原生`int`类型呢？因为复制`int`开销很低，所以可以先加一个锁，复制出来，同样地再操作第二个对象，最后比较。这样，持有锁的时间更短，而且是分别加锁，竞争更小。代码如下
```cpp
class Y
{
private:
    int some_detail;
    mutable std::mutex m;
    int get_detail() const
    {
        std::lock_guard<std::mutex> lock_a(m);
        return some_detail;
    }

public:
    Y(int sd) : some_detail(sd) {}
    friend bool operator==(Y const &lhs, Y const &rhs)
    {
        if (&lhs == &rhs)
            return true;
        int const lhs_value = lhs.get_detail();
        int const rhs_value = rhs.get_detail();
        return lhs_value == rhs_value;
    }
};
```
新场景一次持有一个锁，避免了死锁的可能性。不过这改变了比较的语义，现在的语义是`lhs.some_detail`在某个时间点的值和`rhs.some_detail`在某个时间点的值相等。在两次取值之间，它们的值可能发生了变化，比较就没有意义了。
