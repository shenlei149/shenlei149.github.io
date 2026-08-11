有这么一种需求，一个任务告诉另一个任务某个特殊的事件发生了，第二个任务需要等待这个事件发生后才能继续执行。

一个明显的方案是使用条件变量（`condition_variable`）来实现。不妨称检测条件的任务为检测任务（`detecting task`），等待条件的任务为响应任务（`reacting task`）。策略非常简单：响应任务等待条件变量，检测任务在事件发生时通知条件变量。
```cpp
std::condition_variable cv; // condvar for event
std::mutex m;				// mutex for use with cv
```
检测任务的代码比较简单：
```cpp
// detect event

cv.notify_one(); // tell reacting task
```
如果有多个响应任务等待同一个事件发生，检测任务可以使用`notify_all()`来通知所有等待的响应任务。

响应任务的代码相对复杂一点，为了调用 `wait()`，必须先获取互斥锁，这就需要一个 `std::unique_lock` 对象。
```cpp
// prepare to react
{
	// open critical section

	std::unique_lock<std::mutex> lk(m); // lock mutex
	cv.wait(lk);						// wait for notify; this isn't correct!

	// react to event
	// (m is locked)
} // close crit. section; unlock m via lk's dtor

// continue reacting (m now unlocked)
```
这个代码即使能工作也有代码异味（`code smell`），原因是使用了互斥。互斥是为了控制访问共享变量，但是检测任务和响应任务之间不需要共享变量。比如检测任务初始化一个全局变量，然后交给响应任务使用。响应任务在初始化之前不会访问，检测任务在初始化完之后也不会再访问，代码之间互补影响。没有必要使用互斥。

这段代码还有两个问题需要注意。

如果检测任务在响应任务调用 `wait()` 之前就已经通知了条件变量，那么响应任务就会错过通知而永远等待下去。

第二个问题是虚假唤醒（`spurious wakeup`）。条件变量的 `wait()` 方法可能会在没有收到通知的情况下返回，这种情况称为虚假唤醒。为了应对虚假唤醒，通常需要在 `wait()` 之后检查某个条件是否满足，如果不满足则继续等待。比如这里的代码可能会时限为
```cpp
cv.wait(lk,
        []{ return whether the event has occurred; });
```
利用 labmda 能力的前提是响应任务能够判断其等待的条件是否成立。但是这个例子中由检测任务触发时间，响应任务无法判断事件是否真的发生了，这也正是它需要等待条件变量的原因。

很多程序员的下一个方法是使用共享的布尔标识，初始的时候为 `false`，当检测任务检测到事件发生时将其设置为 `true`。响应任务轮询这个标识，直到它变为 `true`。
```cpp
std::atomic<bool> flag(false);

// detect event
flag = true; // tell reacting task

// prepare to react
while (!flag); // wait for event

// react to event
```
这种方法没有条件变量解法的问题，没有使用条件变量，检测任务在响应任务轮询之前设置标识也没有问题，也没有虚假唤醒的问题。

缺点是响应任务轮询带来的开销。等待标识变为 `true` 这段时间，这个任务处于被阻塞的状态，但是它仍在运行。因此，占用了一个原本可以执行其他任务的硬件线程，每次在时间片开始或者结束的时候，都会产生上下文切换的开销。一个真正被阻塞的任务是不会有这些开销的，而这正是基于条件变量方法的优势。

结合两者是一个好主意。标识用于检测任务和响应任务之间的通信，互斥可以用于保护标识的访问，条件变量用于阻塞响应任务直到事件发生。这样无需使用 `std::atomic`，普通 `bool` 就可以了。
```cpp
std::condition_variable cv;
std::mutex m;

bool flag(false);

// detect event
{
	std::lock_guard<std::mutex> g(m); // lock m via g's ctor
	flag = true;					  // tell reacting task (part 1)
} // unlock m via g's dtor

cv.notify_one(); // tell reacting task (part 2)

// prepare to react
{
	std::unique_lock<std::mutex> lk(m);
	cv.wait(lk, [] { return flag; }); // use lambda to avoid spurious wakeups

	// react to event (m is locked)
}

// continue reacting (m now unlocked)
```
这个方法避免了之前讨论的所有问题。不过代码异味仍旧存在，检测任务和响应任务之间的通信方式很奇怪。通知条件变量只是告诉响应任务它等待的事情可能发生了，响应任务要亲自去检查标识才能确定事件是否真的发生了。设置标识虽然告诉响应任务该事件已经发生，但检测任务仍旧必须通知条件变量，响应任务才能唤醒检查标识。检测任务和响应任务之间的通信方式不够直接。可行，但不优雅。

另一种方案彻底避开条件变量、互斥锁、标志位，让响应任务等待一个由检测任务设置的 `future` 对象。之前说过 `future` 是被调用者与调用者之间通信的接收端，不过这里检测任务与响应任务之间没有这种关系。这是一种通用的通信方式，任何需要将信息从一个地方转到另一个地方的场景，都可以使用这种方式。在这种情况下传递信息表示所关注的事件已经发生。

检测任务有一个 `std::promise` 对象，响应任务有相应的 `future` 对象。当事件发生时，检测任务调用 `set_value()` 来设置 `promise` 对象的值，同时，响应任务调用 `wait()` 来等待 `future` 对象的值被设置。`std::promise` 和 `std::future` 或 `std::shared_future` 需要一个类型参数，表示传递信息的类型。这里我们没有信息需要传递，只是表示事情发生了。因此类型是 `void`。
```cpp
std::promise<void> p; // promise for communications channel

// detect event
p.set_value(); // tell reacting task

// prepare to react
p.get_future().wait(); // wait on future corresponding to p
// react to event
```
和使用标志位的方法类似，这个方案无需互斥，也无需关心在设置 `std::promise` 的值之前，响应任务是否已经调用了 `wait()`，也没有虚假唤醒的问题。和条件变量方法类似，调用 `wait()` 的任务被阻塞。很完美不是吗？不过这个方法也有自己的问题。

在 `std::promise` 和 `std::future` 之间有一个共享状态，需要动态分配内存，因此这个方法有堆内存的分配和释放。

更严重的是，`std::promise` 只能被设置一次，也就是说两者之间的通信时一次性的，不能重复。这和之前讨论的方法不尽相同。条件变量可以通知多次，标志位也可以清除了再设置。

还有一个问题。假设我们需要先创建挂起状态的系统线程。比如想提前消除所有与线程创建相关的额外开销，当准备在线程上执行某些操作的时候就可以避免创建线程的延迟。或者想要在运行之前设置优先级或亲和性（这些需要通过 `std::thread` 的 `native_handle()` 来设置）。假定我们只需要一次性类似的操作，因此可以使用上述的方法。
```cpp
std::promise<void> p;

// func for reacting task
void react();

// func for detecting task
void detect()
{
	std::thread t(
		[] // create thread
		{
			p.get_future().wait(); // suspend t until future is set
			react();
		});

	// here, t is suspended prior to call to react

	p.set_value(); // unsuspend t (and thus call react)

	// do additional work
	t.join(); // make t unjoinable
}
```
由于可连接性很重要，因此可以使用 `ThreadRAII`。
```cpp
void detect()
{
	// use RAII object
	ThreadRAII tr(std::thread(
					  []
					  {
						  p.get_future().wait();
						  react();
					  }),
				  ThreadRAII::DtorAction::join);

	// here, t is suspended prior to call to react

	p.set_value(); // unsuspend t (and thus call react)
}
```
如果程序在 `// here, t is suspended prior to call to react` 这段代码抛出了异常，那么 `p.set_value()` 就不会被调用，lambda 里面的 `wait()` 就会永远阻塞不会结束，RAII 对象 `tr` 要在析构函数中执行 `join()`，但是线程已经阻塞了，析构就不会完成。解决方案可以使用 `std::promise` 的 `set_exception()` 方法来设置异常，这样 `wait()` 就会抛出异常而不是阻塞。

下面从原始使用 `std::thread` 的代码开始，从挂起单个响应任务泛化到控制多个响应任务。核心技巧就是用 `std::shared_future` 来替代 `std::future`。每个响应的线程都需要一个指向共享状态的 `std::shared_future` 的独立拷贝，因此通过 `share()` 方法获取的 `std::shared_future` 要以传值的方法被 lambda 捕获。
```cpp
std::promise<void> p;

void detect()
{
	// now for multiple reacting tasks

	auto sf = p.get_future().share(); // sf's type is std::shared_future<void>

	std::vector<std::thread> vt; // container for reacting threads
	for (int i = 0; i < threadsToRun; ++i)
	{
		vt.emplace_back(
			[sf]
			{
				sf.wait(); // wait on local  copy of sf; see
				react();
			});
	}

	// detect hangs if here throws!

	// unsuspend all threads
	p.set_value();
	for (auto &t : vt)
	{
		t.join();
	}
}
```

## Things to Remember
- For simple event communication, condvar-based designs require a superfluous mutex, impose constraints on the relative progress of detecting and reacting tasks, and require reacting tasks to verify that the event has taken place.
- Designs employing a flag avoid those problems, but are based on polling, not blocking.
- A condvar and flag can be used together, but the resulting communications mechanism is somewhat stilted.
- Using `std::promise`s and futures dodges these issues, but the approach uses heap memory for shared states, and it's limited to one-shot communication.
