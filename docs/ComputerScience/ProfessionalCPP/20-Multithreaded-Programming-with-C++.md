## 简介
执行多个任务有三种主要模式：

* 顺序执行（`sequential execution`）：任务一个接一个的执行。
* 并发执行（`concurrent execution`）：多个任务好像是同时执行的，操作系统将 CPU 分成了很小的时间片，每个任务分配一个时间片执行，不停地在多个任务之间切换。
* 并行执行（`parallel execution`）：真正的同时执行，多个任务在多个处理器单元上执行。

并行执行非常困难，最大的难点是问题本身可能不容易拆分成独立的部分，其次还有竞争条件、死锁、撕裂（`tearing`）、假共享等难点。

线程安全包括以下几点：

* 不可变的数据（`immutable data`）：对多线程访问而言天然安全。
* 原子操作（`atomic operation`）：底层类型提供原子操作实现线程安全。
* 互斥和其他同步机制（`mutual exclusion and other synchronization mechanism`）：用于协调多线程访问共享数据。
* 线程局部存储（`thread-local storage`）：标记为 `thread_local` 的变量是线程级别的变量，默认情况下其他线程无法访问。

竞争条件（`race condition`）指多个线程访问共享资源。数据竞争（`data race`）指共享的资源是内存时的竞争条件。当多个线程访问同一个变量，至少有一个是写的时候，就能发生数据竞争。

撕裂（`tearing`）是一种特殊的数据竞争。一个线程写了部分内存，同时一个线程读这块内存，数据是不一致的，这就是撕裂读（`torn read`）。两个线程同时写一块内存，最后结果部分是一个线程写的，另一部分是另一个线程写的，导致数据不一致，这就是撕裂写（`torn write`）。

死锁（`deadlock`）指的是多个线程在等待其他线程所持有的对象，导致无限等待。获取资源的方式是按照一样的顺序获取，就可以避免死锁。第二个可能得方式是试图获取资源，但是一段时候后放弃，睡眠一会继续尝试。这种方式是否可行完全依赖于死锁的类型。`std::lock` 就是基于后一种方式来实现的。

假共享（`false sharing`）是说两个线程需要向同一个 cache line 写数据，那么这里需要等待，还可能涉及缓存回写，性能比较差。因此需要小心设计数据，避免这种情况。

## 线程
`<thread>` 提供了大量功能，可以方便使用线程。

最简单使用 `std::thread` 方法是传入一个函数指针，后面可以跟上传给函数的参数来构造 `std::thread` 对象。在 `std::thread` 对象被销毁之前，比如 `join()` 或 `detach()`。

除了函数指针，还可以传入 lambda 表达式构造 `std::thread` 对象。函数对象，重载了 `operator()` 的类的对象，也可以作为参数构造 `std::thread` 对象，默认情况下复制一次对象，可以使用 `std::ref` 传引用。类的成员变量也可以当做函数指针来构造 `std::thread` 对象，额外需要传入对象指针，即 `this`。当传入对象的指针或引用，可能会引入数据竞争。

`thread_local` 修饰变量，使之成为一个线程级别的局部变量。

线程无法取消。一个可行的方式是当前线程和新线程有一个约定，通过共享变量的方式告诉新线程需要终止了。这里要注意，因为存在共享变量，可能存在数据竞争。

`<thread>` 提供了 `std::jthread`，和 `std::thread` 近乎是一样的，额外提供了析构时 `join()` 和可取消两个功能。取消机制涉及 `<stop_token>` 提供的两个对象，`std::stop_token` 和 `std::stop_source`。前者的 `stop_requested()` 成员函数返回是否需要取消，后者的 ` request_stop()` 成员函数请求取消。`std::jthread` 成员函数 `get_stop_token()` `get_stop_source()` 返回相应的对象，彼此已经关联了。此外，如果需要使用取消机制，传入的可调用对象的第一个参数需要是 `std::stop_token`。

`std::thread` 无法直接返回值。可以通过传入变量的指针或引用，或者传入引用对象（函数对象），曲线得到结果。通过 `std::future` 能够简化获取返回值。

异常不能跨线程捕获，如果线程中有异常没有捕获，会导致整个程序挂掉。如果线程无法处理，可能需要创建线程的线程处理，这就涉及跨线程处理异常。标准库提供了一些函数可以做到这一点。`std::exception_ptr std::current_exception() noexcept;` 返回的是当前捕获的异常，这里返回的指针和 `std::shared_ptr` 类似，引用计数，因此异常对象在线程销毁之后也还可以存在。`[[noreturn]] void std::rethrow_exception(exception_ptr p);` 重抛异常。为线程的可执行对象添加一个出参，`std::exception_ptr& err`，线程捕获异常会后赋值给出参。外面拿到这个引用对象，如果不是空，可以自己处理，还可以继续抛异常。

## 原子操作类库
标准库提供了原子类型 `std::atomic<T>`，多线程读写安全。`atomic<T>::is_always_lock_free` 是说实现是否总是无锁（`lock free`）的，成员函数 `is_lock_free()` 返回当前类型是否是无锁的，这依赖于平台是否提供了相应的指令。模板参数 `T` 可以是自定义类型，只要是能够支持最朴素的拷贝，即 `std::is_trivially_copyable_v` 返回 `true`，但是是否有锁还要依赖于类型自身，比如大小。`atomic_flag` 总是无锁实现的，不支持赋值，但是支持 `clear()` `test()` `test_and_set()`。

原子类型支持的操作很多，首先看一个比较奇怪的 `bool std::atomic<T>::compare_exchange_strong(T &expected, T desired);`，它的行为类似于
```cpp
if (*this == expected)
{
	*this = desired;
	return true;
}
else
{
	expected = *this;
	return false;
}
```

这看起来有点奇怪，但是可以方便的实现原子化的函数，比如
```cpp
void
AtomicallyMultiply(std::atomic<int> &a, int n)
{
	int expected { a.load() };
	int desired { n * expected };
	while (!a.compare_exchange_strong(expected, desired))
	{
		desired = n * expected;
	}
}
```

`atomic<T>::fetch_add()` 是返回当前值，然后加上传入的参数。整数类型的原子类型支持 `fetch_add()` `fetch_sub()` `fetch_and()` `fetch_or()` `fetch_xor()` `++` `--` `+=` `-=` `&=` `^=` `|=`，指针类型的原子类型支持 `fetch_add()` `fetch_sub()` `++` `--` `+=` `-=`，浮点类型的原子类型仅支持 `fetch_add()` `fetch_sub()`。

大部分原子操作支持指定内存序，比如
```cpp
T std::atomic<T>::fetch_add(T value, std::memory_order = memory_order_seq_cst);
```
C++ 提供了六种内存序，在称为这方面的专家之前，默认的足够用了。

`std::atomic<std::shared_ptr<T>>` 也是支持的。如果多个线程使用同一个智能指针实例，当同时调用非 `const` 函数时就可能出现数据竞争。此时应该用 `std::atomic<std::shared_ptr<T>>` 来保证正确性。

`std::atomic_ref` 和 `std::atomic` 基本一样，这个作用于引用罢了。

原子操作提供了等待机制。`wait(oldValue)` 会阻塞直到其他线程调用 `notify_one()` 或 `notify_all()`，并且值发生了变化，不再等于 `oldValue`。如果值已经不是旧值了，不会阻塞。

## 互斥
一个线程要访问（读或者写）共享数据，首先锁住一个互斥对象，如果其他线程已经锁住了这个互斥对象，那么当前线程等待其他线程释放锁。一旦获得的锁，那么访问数据就是安全的了。当访问完之后，释放锁。

有三种基本的互斥类型，`std::mutex` `recursive_mutex` `shared_mutex`，成员函数有 `lock()` `try_lock()` `unlock()`，分别是阻塞式上锁、非阻塞式上锁和释放锁。

`std::mutex` 是互斥所有权语义。获取锁的线程再次 `lock()`，可能导致死锁。`recursive_mutex` 允许再次上锁，要释放上锁的次数。`shared_mutex` 是共享锁，即读写锁。写锁是互斥的，但是读锁可以共享，`lock_shared()` `try_lock_shared()` `unlock_shared()` 对应是读锁成员函数，和 `std::mutex` 一样，不允许获取锁之后再上锁。

还有三个基于时间的互斥类型，`std::timed_mutex` `recursive_timed_mutex` `shared_timed_mutex`，除了上述成员函数之外，还有 `try_lock_for(rel_time)` 和 `try_lock_until(abs_time)`，前者在传入的 `duration` 内尝试获取锁，时间到了返回 `false`，后者在传入的 `time_point` 绝对时间之前获取锁，时间到了返回 `false`。其余性质和之前介绍的互斥类型也一致。

这些都是资源，不推荐使用显式调用 `lock()` `unlock()` 这些成员函数，而是使用下面介绍的锁类型，通过 RAII 管理资源。

`lock_guard` 有两个构造函数，第一个是传入一个互斥实例，第二个传入一个互斥实例之外，还传入一个类型是 `std::adopt_lock_t` 的对象，`std::adopt_lock`，这种情况下，假定线程已经获取了锁。`lock_guard` 在构造时阻塞式获取锁，析构的时候释放锁。

`std::unique_lock` 更精巧，允许延迟获取锁，构造函数有好几种重载。一参函数行为和 `lock_guard` 一致。可以传入 `std::defer_lock_t` 类型的 `std::defer_lock`，此时就可以延迟调用 `lock()`。传入 `std::try_to_lock`，类似于 `try_lock()` 行为。也支持 `std::adopt_lock`。也可以传入绝对时间 `std::chrono::time_point` 或相对时间 `std::chrono::duration`，行为和之前类似。

`shared_lock` 和 `unique_lock` 基本一致，不过是读写锁。

函数 `lock()` 可以一次性获取多个锁，无需担心死锁。`try_lock()` 也类似，如果失败了，会将已经获得的锁释放。
```cpp
template<class L1, class L2, class... Ln>
void lock(L1 &, L2 &, Ln &...);
```

`std::scoped_lock` 接受多个互斥对象，简化了获取多个锁，无需担心获取的顺序。比如下面的例子就是一次性获取放在 `std::array` 中的 `std::mutex` 的锁。
```cpp
template<size_t N, size_t... Is>
auto
make_scoped_lock(std::array<std::mutex, N> &mutexes, std::index_sequence<Is...>)
{
	return std::scoped_lock { mutexes[Is]... };
}

template<size_t N>
auto
make_scoped_lock(std::array<std::mutex, N> &mutexes)
{
	return make_scoped_lock(mutexes, std::make_index_sequence<N> {});
}

int
main()
{
	std::array<std::mutex, 4> mutexes;
	auto lockAll { make_scoped_lock(mutexes) };
}
```

通过 `std::call_once()` 和 `std::once_flag` 可以实现只调用某个函数或者成员函数一次的效果。如果抛出异常，异常会传到给调用者，会从其他调用者中选择一个执行。

## 条件变量
`std::condition_variable` 只能等待在 `unique_lock<mutex>` 上，`std::condition_variable_any` 可以等待在任意类型的锁。

`notify_one()` 唤醒等待在当前条件变量上的一个线程，`notify_all()` 唤醒等待在当前条件变量上的所有线程。`wait(unique_lock<mutex>& lk)`，线程等待，线程应该已经获取了 `lk`，`wait` 相当于释放锁，当唤醒时尝试再次获取锁。`wait_for(unique_lock<mutex>& lk, const chrono::duration<Rep, Period>& rel_time)` `wait_until(unique_lock<mutex>& lk, const chrono::time_point<Clock, Duration>& abs_time)` 和 `wait()` 类似，不过依赖于时间唤醒而不是 `notify`。`wait()` 支持一个额外谓词，逻辑等价于
```cpp
while (!predicate())
{
	wait(lk);
}
```

## latch
`std::latch` 可以看做一个 counter，所有线程达到每一个点，然后再继续开始执行。`arrive_and_wait()` 将 latch 计数器减一操作并等待。`wait()` 只等待不对计数器进行操作。`try_wait()` 用于检测计数器是否到零了。`count_down()`，只进行计数器减一但是不等待。

## barrier
屏障（`barrier`）用于多个线程同步执行的进度，由于实在循环中。与 `latch` 一个很大的不同是，它可以重用。当期待的线程完成工作，然后可以开始新的一轮，计数器重置了。

## 信号量
信号量是一种更轻量级的同步机制。初始化一个非负计数器，表示一组 `slot`，`acquire()` 获取一个 `slot`，可能会阻塞，`try_acquire()` 类似，但是不会阻塞，`try_acquire_for()` `try_acquire_until()` 类似，但是仅尝试获取一段时间，`release()` 释放一个 `slot`。`std::counting_semaphore` 表示非负个资源，`std::binary_semaphore` 仅有一个 `slot`。

## future
`std::future` 用于从一个线程中得到结果或者转移异常。`get()` 是阻塞式调用得到结果，`wait_for()` 等待一段时间，返回值表示是否得到了结果。

`std::promise` 用于存储结果，可以通过 `set_value()` `set_exception()` 存储结果或者异常。

`std::packaged_task` 相比显式使用 `std::promise` 更简单。传入一个可调用对象，通过 `get_future()` 得到 `std::future` 对象取结果，把自身移动给一个线程去执行即可。

`std::sync` 是更易用的异步执行的方式，不过在构造的时候有几种选择。`launch::async` 强行使用不同线程计算，`launch::deferred` 强行使用同一个线程在 `get()` 时计算，`launch::async | launch::deferred`，运行时决定其行为，这也是默认行为。

`std::future<T>` 要求 `T` 是移动构造，因此只能调用一次 `get()` 获取值。如果想要获取多次，需要通过 `std::future::share()` 函数或者通过 `future` 构造一个 `std::shared_future`，这样就可以多次 `get()` 结果了。
