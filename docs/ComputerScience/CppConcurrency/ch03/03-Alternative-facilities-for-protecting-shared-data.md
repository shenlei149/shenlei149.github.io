互斥是保护共享数据最普遍的方式，但并非唯一的方式。针对具体的场景，可以提供更合适的保护措施。

一个极端但常见的情况是，共享数据只需要在初始化的时候被保护即可，之后不再需要显式的保护了。可能是创建完成之后就处于只读状态，或者是后续操作有隐式的保护。不管怎样，后续如果还加锁的话，那么有不必要的性能损失。C++提供了一套机制，仅在初始化的时候保护共享数据。

### Protecting shared data during initialization
假设需要某个共享数据，创建代价比较高，那么会延迟初始化（`Lazy initialization`）。对于单线程实现的方式是先看有没有这个资源，没有的话初始化再用。
```cpp
std::shared_ptr<some_resource> resource_ptr;
void foo()
{
    if (!resource_ptr)
    {
        resource_ptr.reset(new some_resource);
    }

    resource_ptr->do_something();
}
```
假定共享数据本身能被安全的并发访问，那么上面的代码只需要保护初始化过程。下面是最质朴的改写方式。不过多个线程仍旧无法并发访问，因为每个线程必须等待互斥，检查是否完成了初始化。
```cpp
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;
void foo()
{
    std::unique_lock<std::mutex> lk(resource_mutex);
    if (!resource_ptr)
    {
        resource_ptr.reset(new some_resource);
    }
    lk.unlock();

    resource_ptr->do_something();
}
```
为了达到并行，很多人进行了改进，包括双重检验锁模式（`double-checked locking`）。先判断指针是否为空，如果为空，获取锁，然后再判断是否为空。因为在两次判断的间隙可能其他线程完成了初始化，所有要获取锁。
```cpp
void undefined_behaviour_with_double_checked_locking()
{
    if (!resource_ptr)
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr)
        {
            resource_ptr.reset(new some_resource);
        }
    }

    resource_ptr->do_something();
}
```
这种模式饱受诟病，因为可能发生恶性条件竞争。因为在保护外面读取指针，和锁内部创建对象之间并没有同步。这涉及指针本身和数据对象。即使当前线程能够看到其他线程写入对象，但是可能无法看见指向的对象，那么会在调用`do_something()`的时候发生未定义行为。C++标准将此定义为数据竞争（`data race`），需要防范。第五章会讨论内存模型，分析形成数据竞争的前因后果。

C++标准委员会为此提供了`std::once_flag`和`std::call_once`来解决这个问题。所有线程都调用`std::call_once`函数，当函数返回后，指针初始化由其中一个线程完成并通过合适的同步机制使得每个线程可见。同步数据由`std::once_flag`实例存储，每个实例对应一次不同的初始化。相比显式的互斥，`std::call_once`开销更低。下面重写上面的例子。初始化需要一个函数完成，不过具备函数调用操作的类也可以轻松使用这种初始化方式。`std::call_once`可以和任意可执行对象一起工作。
```cpp
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource()
{
    resource_ptr.reset(new some_resource);
}

void foo()
{
    std::call_once(resource_flag, init_resource);
    resource_ptr->do_something();
}
```
在这个例子中，是初始化某个命名空间下的对象。下面展示了如何使用该机制延迟初始化类的成员。
```cpp
class X
{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection()
    {
        connection = connection_manager.open(connection_details);
    }

public:
    X(connection_info const &connection_details_) : connection_details(connection_details_)
    {
    }

    void send_data(data_packet const &data)
    {
        std::call_once(connection_init_flag, &X::open_connection, this);
        connection.send_data(data);
    }

    data_packet receive_data()
    {
        std::call_once(connection_init_flag, &X::open_connection, this);
        return connection.receive_data();
    }
};
```
在这个例子中，初始化会在`receive_data()`或者`send_data()`中完成。使用成员变量`open_connection()`初始化需要额外传入`this`指针。这个在STL中调用可执行对象时是一致的。

和`std::mutex`一样，`std::once_flag`既不能复制也不能移动。那么作为成员变量，必须有成员函数来初始化它们。

如果使用`static`的局部变量，那么初始化过程和上面遇到的问题一样。C++标准规定第一次遇见静态数据的声明语句进行初始化。多个线程调用同一个函数，就有潜在的条件竞争的问题。C++11标准发布之前，很多编译器不能正确处理这个问题。可能不止一个线程认为自己是第一个访问的线程，进而进行初始化或者试图使用未完全初始化的对象。C++解决了这个问题。如果代码只需要一个全局实例，那么可以用下面的方式替换`std::call_once`。
```cpp
class my_class;
my_class &get_my_class_instance()
{
    static my_class instance;
    return instance;
}
```
不用担心初始化问题！

### Protecting rarely updated data structures
考虑类似于 DNS 缓存这种很少更新的场景，如果使用`std::mutex`使得没法并发读，影响性能。这种场景需要读写锁（`reader-writer mutex`）。

C++17提供了`std::shared_mutex`和`std::shared_timed_mutex`（C++14新增的）。不同之处在于后者支持一些附加操作（参考4.3节），所以如果没有其他操作的需要，使用前者在某些平台上会有性能提升。

第八章我们会看到使用这种互斥也不是灵丹妙药，性能取决于硬件以及读和写的负载。所以需要进行性能分析，已确定引入多线程的复杂性能带来性能提升。

类似于`std::mutex`，这里使用`std::shared_mutex`保护数据。对于更新，可以使用`std::lock_guard<std::shared_mutex>`和`std::unique_lock<std::shared_mutex>`加锁，保持排他性。不需要更新的线程使用`std::shared_lock<std::shared_mutex>`获取共享锁，这个和`std::unique_lock`类似，RAII管理资源，不同的是允许多个线程获取共享锁。如果一个线程有了共享锁，那么排它锁需要等待其他线程释放锁；如果任意线程有了排它锁，其他线程无法获得共享锁或者排它锁。下面的例子是使用这些锁来实现 DNS 缓存场景。
```cpp
#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>
class dns_entry;
class dns_cache
{
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mutex;

public:
    dns_entry find_entry(std::string const &domain) const
    {
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string, dns_entry>::const_iterator const it =
            entries.find(domain);
        return (it == entries.end()) ? dns_entry() : it->second;
    }

    void update_or_add_entry(std::string const &domain,
                             dns_entry const &dns_details)
    {
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
};
```
`find_entry()`使用`std::shared_lock<>`保护只读操作，多个线程可以同时调用`find_entry()`。`update_or_add_entry()`使用`std::lock_guard<> `获取排它锁，使得其他线程无法调用`update_or_add_entry()`进行更新，也无法通过`find_entry()`来读数据。

### Recursive locking
当线程已经获取了某个`std::mutex`，当再次获取的时候是未定义行为。但有时允许获取一个`std::mutex`多次，为此，C++提供了`std::recursive_mutex`。你必须释放所有的当前线程获得的锁，比如加锁三次，那么必须释放锁三次。`std::lock_guard<std::recursive_mutex>`和`std::unique_lock<std::recursive_mutex>`会处理这些事情。

很多时候，如果需要使用递归锁，那么往往可以改变设计以摆脱对它的依赖。一个常见的使用递归锁的场景是每个成员函数都需要加锁保护成员数据。但是出现了一个成员函数调用另外一个成员函数，那么第二个成员函数还是需要加锁。

不推荐使用递归锁的原因是这样会让我们不去深入思考问题本身。加锁往往意味着不变量已经被破坏了，第二个成员函数需要在这种情况下继续加锁修改数据。更好的方式是抽取私有函数，两个成员函数都调用它，这个私有函数就不必再加锁了。同时需要思考在这种情况下数据的状态。
