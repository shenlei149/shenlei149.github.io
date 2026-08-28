如果想要异步运行 `doAsyncWork`，有两种方法。第一种是基于线程的（`thread-based`），创建一个线程来运行这个函数，第二种是基于任务的（`task-based`）。
```cpp
// thread-based
int doAsyncWork();
std::thread t(doAsyncWork);

// task-based
auto fut = std::async(doAsyncWork);
```
基于任务的方法比基于线程的方法更好。这里 `doAsyncWork` 是一个返回 `int` 的函数，基于线程的方法没有直接的方式来获取返回值，而基于任务的方法可以通过 `fut.get()` 来获取返回值。如果 `doAsyncWork` 抛出异常，基于线程的方法无法捕获异常，程序会终止（调用了 `std::terminate`），而基于任务的方法可以在调用 `fut.get()` 时捕获异常。

更大的差别是基于任务的方式是更高层次的抽象，将我们从管理线程的细节中解放出来。首先总结一下线程（`thread`）的三种不同的含义。

- 硬件线程（`hardware thread`）是真正执行计算的线程，现代计算机的每个 core 提供了一个或多个硬件线程。
- 软件线程（`software thread`）是操作系统提供、管理的线程，调度它们在硬件线程上运行。软件线程的数量通常比硬件线程多，因为软件线程可能会被阻塞（比如等待 I/O、条件变量），此时执行其他非阻塞的线程能够提高吞吐。
- `std::thread` 是 C++ 提供的一个类，处理底层软件线程的对象。有时 `std::thread` 可以代表空对象，比如使用默认构造函数创建的 `std::thread` 对象；或者使用 `std::move` 将一个 `std::thread` 对象移动到另一个对象后，原来的对象就变成了空对象；已经被 `join` 的线程（函数已经执行完了）；已经被 `detach` 的线程（与底层软件线程之间的连接已经断开了）。

软件线程是有限的资源。如果尝试创建比系统能够提供的更多的线程，会抛出 `std::system_error` 异常。即使 `doAsyncWork` 不会抛出异常，调用 `std::thread t(doAsyncWork);` 也可能抛出异常。如何处理这个问题呢？运行在当前线程？这不符合本意，而且如果是 GUI 线程，还会阻塞响应。等待其他线程完成任务再创建 `std::thread`？增加复杂性，并且有可能其他线程在等待 `doAsyncWork` 执行（比如等待它产生结果或者条件变量）。

即使没有遇到资源瓶颈，也有可能遇到超额（`oversubscription`）的问题。如果准备就绪的软件线程多于硬件线程，OS 线程调度会通过时间切片来让它们都运行起来，但是这会增加上下文的切换次数。这些上下文切换会增加系统的开销，特别是软件线程在不同的时间片在不同的硬件线程上运行的情况。此时，CPU 缓存会失效，同时新运行的线程会把之前线程的缓存数据覆盖掉，再次运行之前的线程时又需要重新加载数据到缓存中。

避免超额是比较困难的，因为最佳的软件线程与硬件线程的比依赖于软件线程运行的模式，而这是动态变化的，比如程序从 I/O 密集型变为 CPU 密集型。最佳比例还依赖于上下文切换的开销和硬件线程使用缓存的情况，而这些取决于机器的架构，即便程序在一个平台避免了超额的问题，换一个架构的机器可能还会出现超额的问题。

使用 `std::async` 会将这些问题交给 C++ 标准库来处理。线程耗尽这种问题就极低概率遇到。上面的调用方式 `std::async(doAsyncWork)` 使用了默认的启动策略（`launch policy`），并不保证一定会创建一个软件线程。允许调度器在请求函数结果的线程上运行 `doAsyncWork`，也就是在调用 `fut.get()` 或 `fut.wait()` 的线程上运行。如果系统处于超额状态，调度器就可以这么选择。

如果我们自己选择在需要结果的线程上运行任务，可能会导致负载均衡的问题。不过是自己写还是使用 `std::async` 都会遇到。当涉及负载均衡问题时，调度器有全局视角，其管理所有进程的线程，而不仅仅是我们写的线程。

使用 `std::async` 时，GUI 的响应问题依旧存在，因为调度器并不知道线程和响应需求是强绑定的。这时，要指定启动策略 `std::launch::async` 给 `std::async`，强制函数一定执行在不同的线程。

最先进的线程调度器会使用线程池（`thread pool`）来避免超额，并且利用工作窃取算法（`work-stealing algorithm`）来避免负载不均衡。不过 C++ 标准并不强制要求使用这些技术。老实说 C++11 的并发规范的某些技术细节使得应用这些技术难度非常大。不过有些实现方式还是使用了这些技术，也有理由期待会持续更新。如果使用了基于任务的方法来写并发，未来可能会自动获取这些好处。如果直接使用 `std::thread`，就需要自己考虑这些问题了，还有可能这些方案要和同一台机器的其他程序配合或协同。

与基于线程的方法相比，基于任务的方法无需手动管理线程，很自然的能够获取异步执行的结果（返回值或异常）。不过下面几种情况可能直接使用线程更合适。

- 需要访问底层线程实现的 API。C++ 并发 API 使用底层平台指定的 API 实现，后者往往提供了更丰富的能力，比如 C++ 标准库没有优先级（`priority`）和亲和性（`affinity`）的概念，而底层平台提供了这些能力。为了使用这些能力，`std::thread` 提供了 `native_handle()`。
- 确实需要也有能力针对程序优化线程的使用。假定只在固定的硬件上跑，并且知道程序的运行模式，同时同一个机器上没有其他程序，那么可以直接使用线程。
- 需要实现超出 C++ 并发 API 范畴的线程的技术。比如当前 C++ 实现（编译器、标准库）没有提供线程池，需要自行实现。

这些都不是常规情况，大部分时候还是应该选择基于任务的方法。

## Things to Remember
- The `std::thread` API offers no direct way to get return values from asynchronously run functions, and if those functions throw, the program is terminated.
- Thread-based programming calls for manual management of thread exhaustion, oversubscription, load balancing, and adaptation to new platforms.
- Task-based programming via `std::async` with the default launch policy handles most of these issues for you.
