当调用 `std::async` 时，期望函数异步执行，但是这并不必须的。我们需要指定启动策略（`launch policy`）给 `std::async` 来控制其行为。有两个标准的启动策略：`std::launch::async` 和 `std::launch::deferred`。

- `std::launch::async` 表示函数必须在不同的线程上执行。
- `std::launch::deferred` 表示函数在调用 `fut.get()` 或 `fut.wait()` 时才会执行，并且在调用这些函数的线程上同步执行，此时会阻塞调用线程直到函数执行完毕。如果没有调用 `get()` 或 `wait()`，函数就不会执行。

默认的启动策略是 `std::launch::async | std::launch::deferred`，因此传入的函数 `f` 既可以异步执行也可以同步执行。这种灵活性使得 `std::async` 和标准库的线程管理组件担负起创建和销毁线程、避免超额、负载均衡等问题的责任。这正是使用 `std::async` 进行并发编程的便利之处。

假定在线程 `t` 上使用如下的默认策略异步执行 `f`，隐含着以下几种可能性：
```cpp
auto fut = std::async(f); // run f using default launch policy
```

- 无法预测 `f` 是不是与 `t` 并发执行的。`f` 可能被推迟执行。
- 无法预测 `f` 是不是运行在与调用 `fut.get()` 或 `fut.wait()` 的线程不同的线程上。
- 无法预测 `f` 是不是运行了，因为或许不是所有的路径下都调用了 `fut.get()` 或 `fut.wait()`。

默认启动策略的灵活性导致很难与 `thread_local` 变量交互。因为如果 `f` 读写了线程局部存储（`thread_local storage`, `TLS`），无法预测最终被访问的是哪一个线程的变量。

这还会影响基于超时的 `wait()` 循环。对一个被推迟执行的任务调用 `wait_for()` 或 `wait_until()` 会返回 `std::future_status::deferred`，这意味着下面的循环看起来会终止，但是实际上可能陷入死循环。如果 `f` 运行在另一个线程上，比如启动策略是 `std::launch::async`，那么就没有问题，反之是 `f` 被推迟执行，那么循环就会永远阻塞。
```cpp
using namespace std::literals;

// f sleeps for 1 second, then returns
void f() { std::this_thread::sleep_for(1s); }

auto fut = std::async(f); // run f asynchronously (conceptually)

// loop until f has finished running... which may never happen!
while (fut.wait_for(100ms) != std::future_status::ready)
{
	// do something else
}
```
这类 bug 很容易在开发和单元测试阶段被忽视，因为它们通常只有在系统高负载的情况下才会暴露。高负载运行的情况下，机器才会被推向超额或线程耗尽的边缘，此时任务最有可能被推迟执行。如果没有超额或线程耗尽，运行时没有任何理由不把任务调度为异步执行。

修复这个问题的方式很简单，就是看调用 `std::async` 的任务是否被推迟执行。不过没有直接的方式通过 `fut` 来判断。不得不通过等待函数 `wait_for()` 来获取，这里我们并不需要等什么，而是要看返回值是不是 `std::future_status::deferred`。
```cpp
auto fut = std::async(f);

// if task is deferred...
if (fut.wait_for(0s) == std::future_status::deferred)
{
	// ...use wait or get on fut to call f synchronously
}
else
{
	// task isn't deferred
	while (fut.wait_for(100ms) != std::future_status::ready)
	{
		// task is neither deferred nor ready,
		// so do concurrent work until it's ready
	}

	// fut is ready
}
```

综上所述，只有在下面所有条件的都满足的情况下对任务使用默认策略才是安全可行的。

- 任务不需要与调用 `get()` 或 `wait()` 的线程并发执行。
- 任务具体读写哪一个线程的 `thread_local` 变量并不重要。
- 要么保证会在 `future` 上调用 `get()` 或 `wait()`，要么接受任务可能永远不会执行。
- 使用 `wait_for()` 或 `wait_until()`，将被推迟执行的情况考虑在内。

如果上述条件有一个不满足，那么我们应该指定 `std::launch::async` 来确保任务异步执行。
```cpp
auto fut = std::async(std::launch::async, f); // launch f asynchronously
```

有一个工具行为和 `std::launch::async` 一样但是自动使用 `std::launch::async` 来确保任务异步执行会非常方便。
```cpp
template<typename F, typename... Ts>
inline auto // std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F &&f, Ts &&...params)
{
	return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}
```
使用就和 `std::async` 一样，只是没有默认策略的灵活性。
```cpp
auto fut = reallyAsync(f); // run f asynchronously; throw if std::async would throw
```

## Things to Remember
- The default launch policy for `std::async` permits both asynchronous and synchronous task execution.
- This flexibility leads to uncertainty when accessing `thread_local`s, implies that the task may never execute, and affects program logic for timeout-based `wait` calls.
- Specify `std::launch::async` if asynchronous task execution is essential.
