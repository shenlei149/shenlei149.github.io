处于可连接的状态的 `std::thread` 对象对应着一个底层的线程。对于非推迟启动的任务对应的 `future` 对象与底层系统线程也有类似的关系。因为 `std::thread` 和 `future` 对象都可以看作是底层系统线程的句柄（`handle`）。

不过 `std::thread` 和 `std::future` 对象的析构函数行为不同。一个可连接的 `std::thread` 的对象在析构时会终止程序。`future` 对象在析构的时候，在某些情况下表现的像执行了 `join`，在另一些情况下表现的像执行了 `detach`，或者两者都没做，但是绝对不会让程序终止。这种线程句柄行为的大杂烩值得研究。

`future` 像是通信管道的一端，另一端是被调用者。被调用者把结果写进 `std::promise` 对象中，调用者通过 `future` 对象读取结果。

不过被调用者把结果写到哪里呢？在调用者调用 `future` 的 `get()` 时被调用者可能已经结束了，因此不能把结果放到被调用者的 `std::promise` 对象中。被调用者的局部对象在结束的时候会被销毁。

结果也不能放到调用者的 `future` 对象中，因为一个 `std::future` 可以被用来创建一个 `std::shared_future`，原始的 `std::future` 对象销毁之后 `std::shared_future` 可能会被拷贝多次，还可以继续读结果。同时要考虑到并非所有的结果类型都支持拷贝，并且该结果的生命周期必须至少与指向它的最后一个 `future` 对象的生命周期一样长。在众多的 `future` 中，哪一个来存放结果呢？

既然调用者和被调用者都不合适，那么结果就应该放在一个独立的对象中，这个对象称为共享状态（`shared state`）。共享状态是一个堆上的对象，标准并没有规定其类型、接口和实现，因此标准库作者可以自由发挥。

被调用者、调用者和共享状态的关系如下。
```
Caller <-----------------Callee's Result <----------------- Callee
   future                   shared state                 std::promise
```
共享状态的存在非常重要，因为 `future` 的析构行为由 `future` 关联的共享状态决定。

- 对于指向通过 `std::async` 启动的非推迟任务共享状态的最后一个 `future`，它的析构函数会被阻塞，直到这个任务完成。也就是说 `future` 的析构函数对运行该异步任务的线程执行了一次隐式 `join()`。
- 所有其他的 `future` 对象的析构函数销毁 `future` 对象本身。对于正在运行的异步任务而言，类似于对底层线程执行了 `detach()`。而与最后一个 `future` 的推迟任务而言，这意味这个延迟任务永远不会运行了。

规则看起来复杂，其实只有两点，一种是简单的正常行为和一种例外。

简单的正常行为是说 `future` 对象的析构函数销毁 `future` 对象本身。既不与任何线程 `join` 也不 `detach`，更不会去运行任何任务。额外做的事情就是递减共享状态的引用计数，该共享状态是由指向它的各个 `future` 以及被调用者的 `std::promise` 对象所共享的，共同修改引用计数。这个引用计数的目的是让标准库知道什么时候能够销毁共享状态对象。

例外仅在以下条件都满足的时候才成立：

- 共享状态是调用了 `std::async` 而创建的。
- 启动策略是 `std::launch::async`。可以是系统自动选择的，也可以是调用 `std::async` 时显式指定的。
- 这个 `future` 对象是指向共享状态的最后一个 `future` 对象。对于 `std::future` 对象来说，这个条件总是成立的。而与 `std::shared_future` 对象而言，如果有其他 `std::shared_future` 对象指向同一个共享状态，那么这个 `std::shared_future` 对象就不是最后一个 `future` 对象，就是正常的销毁。

只有以上条件都满足的时候，`future` 析构函数表现出来的特殊行为就是阻塞，直到异步运行的任务完成。从实际效果看，相当于对运行由 `std::async` 启动的任务执行了一次隐式 `join()`。如果不是最后一个 `future` 对象，就相当于 `detach()`；如果是推迟任务，相当于什么也不做。

为什么对通过 `std::async` 启动的非推迟任务的共享状态要有这么一个特殊规则呢？标准委员会希望避免隐式 `detach()` 导致的麻烦，又不想采用强制终止程序这样激进的策略，折中地选择了隐式 `join()`。这个决定也并非没有争议，甚至在 C++14 期间，有关于放弃这一行为的严肃讨论，但是最终还是保留了下来。

`future` 的 API 没有提供确定它引用的共享状态是否是通过 `std::async` 启动的非推迟任务创建的，因此无法知道在析构的时候是否会阻塞等待一个异步任务完成。
```cpp
// this container might block in its dtor, because one or more
// contained futures could refer to a shared state for a non
// deferred task launched via std::async
std::vector<std::future<void>> futs;

// Widget objects might block in their dtors
class Widget
{
public:

private:
	std::shared_future<double> fut;
};
```
不过如果知道 `future` 不满足触发特殊析构行为的条件，那么就可以确定它的析构函数不会阻塞。比如说是通过 `std::packaged_task` 创建的共享状态。一个 `std::packaged_task` 通过对一个函数（其他可以调用的对象）进行包装，使其执行结果放到一个共享状态中，从而为该函数异步执行做好了准备。随后，可以通过 `std::packaged_task` 的 `get_future()` 方法获取一个指向该共享状态的 `future` 对象。此时我们知道 `fut` 没有引用一个通过 `std::async` 创建的共享状态，因此它的析构函数不会阻塞。
```cpp
int calcValue();						 // func to run
std::packaged_task<int()> pt(calcValue); // wrap calcValue so it
auto fut = pt.get_future();				 // can run asynchronously get future for pt
```
一旦创建好了 `std::packaged_task` 对象，就可以在一个线程中执行它。也可以调用 `std::async` 来执行它，但是为什么不直接调用 `std::async` 来执行呢？因此下面我们传递给一个线程，由于 `std::packaged_task` 不能拷贝，所以要移动进去。
```cpp
{ // begin block
	std::packaged_task<int()> pt(calcValue);
	auto fut = pt.get_future();
	std::thread t(std::move(pt));

	// do something else
} // end block
```
在 `// do something else` 这段时间，`t` 有三种不同的可能：

- 没有对 `t` 做任何事情，`t` 在作用域结束的时候处于可连接状态，导致程序终止。
- 对 `t` 调用了 `join()`，`fut` 的析构函数不需要阻塞，因为代码里面已经调用了 `join()`。
- 对 `t` 调用了 `detach()`，`fut` 的析构函数也不需要隐式 `detach()`，因为代码里面已经做过了。

也就是说，当你持有一个由于 `std::packaged_task` 而产生的、指向某个共享状态的 `future` 对象时，通常不需要采用特殊的析构策略。因为究竟是 `join()` 还是 `detach()`，甚至是终止程序，都由 `std::thread` 的代码决定了。

## Things to Remember
- Future destructors normally just destroy the future's data members.
- The final future referring to a shared state for a non-deferred task launched via `std::async` blocks until the task completes.
