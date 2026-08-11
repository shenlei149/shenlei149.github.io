每个 `std::thread` 对象都处于可连接（`joinable`）和不可连接（`unjoinable`）两种状态之一。一个处于可以连接状态的 `std::thread` 对象对应着一个正在运行或者可以运行的底层异步线程。比如对应一个阻塞或者等待调度的底层线程的 `std::thread` 对象是可以连接的。此外，对应着已经运行完毕的底层线程的 `std::thread` 对象也是可以连接的。

如果一个 `std::thread` 对象是不可连接的，那么它不对应任何底层线程。一个 `std::thread` 对象在以下情况下是不可连接的：

- 它是默认构造的 `std::thread` 对象。`std::thread` 没有函数要去执行，因此也不对应底层的线程。
- `std::thread` 对象已经被移动走了。移动的结果是被移动的 `std::thread` 对应的底层线程现在对应到了新的 `std::thread` 对象。
- `std::thread` 对象被 `join` 了。`join` 之后，`std::thread` 对象不再对应已经执行完毕的底层线程。
- `std::thread` 对象被 `detach` 了。执行 `detach` 会切断 `std::thread` 对象和底层线程的联系。

`std::thread` 的可连接性（`joinability`）如此重要的原因是如果一个处于可连接状态的线程对象被销毁，那么程序就会终止。假如我们有一个函数 `doWork`，接受一个过滤函数 `filter` 和一个最大值 `maxVal` 作为参数，首先保证前置条件都满足，然后对 0 到 `maxVal` 之间的每一个整数调用 `filter` 进行筛选。假定两个步骤都非常耗时，那么异步执行是好的选择。

我们倾向于使用基于任务的方法，但是假定这里我们要去调整线程的优先级，不得不采用基于线程的方案。
```cpp
constexpr auto tenMillion = 10'000'000;

bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion)
{
	std::vector<int> goodVals; // values that satisfy filter

	// populate goodVals
	std::thread t(
		[&filter, maxVal, &goodVals]
		{
			for (auto i = 0; i <= maxVal; ++i)
			{
				if (filter(i))
				{
					goodVals.push_back(i);
				}
			}
		});

	// use t's native handle to set t's priority
	auto nh = t.native_handle();
	// set priority

	if (conditionsAreSatisfied())
	{
		t.join();					  // let t finish
		performComputation(goodVals); // computation was performed
		return true;
	}

	return false; // computation was not performed
}
```
如果 `conditionAreSatisfied()` 返回 `true` 那么一切正常，不过如果返回 `false` 或者抛出异常，那么 `std::thread` 对象 `t` 就会在函数返回时被销毁，而此时它仍然处于可连接状态，程序就会终止。

为什么 `std::thread` 析构函数行为如此呢？下面讨论一下另外两种可能性。

- 隐式 `join`。如果 `std::thread` 析构函数在对象销毁时隐式调用 `join` 等待异步线程执行完。听起来很好，但是有难以追踪的性能异常。比如 `conditionsAreSatisfied()` 返回 `false`，那么 `doWork` 函数等待过滤计算完成才返回。
- 隐式 `detach`。这种情况 `std::thread` 析构函数在对象销毁时隐式调用 `detach`，切断 `std::thread` 对象和底层线程的联系。底层的线程会继续执行。这会导致更难调试的问题。在上面的例子中，`goodVals` 是一个局部变量，引用捕获，在 lambda 内部会被修改。假定 lambda 继续异步执行。此时 `doWork` 函数返回，`goodVals` 被销毁。帧栈弹出，在 `doWork` 之后继续使用，未来用于其他函数的调用。最终会有一个调用使用 `doWork` 所占用的部分或者全部栈内存。那么 `goodVals` 的修改会影响栈上的内容，影响了其他函数的调用，导致难以追踪的 bug。

标准委员会正是因为销毁一个处于可连接状态（`joinable`）的 `std::thread` 的结果非常严重，所以决定禁止这种i行为，在 `std::thread` 析构函数中终止程序。

这就将责任推给了写代码的人。如果使用了 `std::thread`，那么必须保证在 `std::thread` 对象销毁之前调用 `join` 或者 `detach`。这里要覆盖每一个出口上，不仅仅包含正常执行到作用域的结尾，还包括通过 `return`、`continue`、`break`、`goto` 或者抛出异常退出的情况。

如果你想在跳出代码块的每条路径都执行某种操作，最常规的做法就是将该操作放入一个局部对象的解析函数中。此类对象被称为 RAII 对象，它们所属的类称为 RAII 类。RAII 是资源获取即初始化（`Resource Acquisition Is Initialization`）的缩写，不过该技术的核心要义在于析构，并非初始化。RAII 在标准库中随处可见，比如 STL 的容器（析构的时候会销毁所有元素并释放内存）、智能指针（析构函数会析构所管理的对象并释放内存或者减少引用计数）等等。不过，标准库并没有对 `std::thread` 提供 RAII 类。或许是因为标准化委员会在拒绝将 `join` 或 `detach` 的其中一个作为默认行为的提案，也不知道这样一个 RAII 类应该做什么。

不过我们自己实现一个也并不困难。比如下面的类在析构的时候会调用 `join` 或 `detach`，取决于构造函数的参数。
```cpp
class ThreadRAII
{
public:
	enum class DtorAction
	{
		join,
		detach
	};

	ThreadRAII(std::thread &&t, DtorAction a) // in dtor, take action a on t
		: action(a)
		, t(std::move(t))
	{}

	~ThreadRAII()
	{
		if (t.joinable())
		{
			if (action == DtorAction::join)
			{
				t.join();
			}
			else
			{
				t.detach();
			}
		}
	}

	std::thread &get() { return t; }

    // support moving
	ThreadRAII(ThreadRAII &&) = default;
	ThreadRAII &operator=(ThreadRAII &&) = default;

private:
	DtorAction action;
	std::thread t;
};
```
构造函数接受 `std::thread` 对象的右值，因为我们需要把对象移动到 `ThreadRAII` 对象中。`std::thread` 对象是不支持拷贝的。

参数声明的顺序和初始化的顺序不太一样，我们把初始化 `std::thread` 对象放在最后，因为 `std::thread` 可能在初始化之后立即就开始运行了，声明在最后保证在初始化完它之前其他成员变量已经初始化完毕，这样可以能够安全的被这个 `std::thread` 对应的异步运行线程访问。

`ThreadRAII` 提供了一个 `get()` 成员函数返回对 `std::thread` 对象的引用，这样避免了 `ThreadRAII` 需要重复 `std::thread` 的接口，在需要 `std::thread` 的地方也可以使用 `ThreadRAII` 对象。

我们实现了析构函数，因此编译器不会再为我们生成移动构造函数和移动赋值运算符，但是没有理由不支持这些操作，因此我们显式的声明了移动构造函数和移动赋值运算符为 `default`。

在调用 `ThreadRAII` 析构函数时，我们首先检查 `t` 是否是可连接的。因为在不可连接的线程对象上调用 `join` 或者 `detach` 是未定义行为，而构造 `std::thread` 对象和 `ThreadRAII` 对象的客户端代码可能通过 `get()` 获取了 `t` 然后调用了 `join` 或者 `detach`，亦或者是移动了对象，这些都会导致 `t` 变为不可连接的状态。

在析构函数中，如果在 `t.joinable()` 和 `t.join()` 或者 `t.detach()` 之间 `t` 变成了不可连接的状态，还是会有未定义行为。这里有一个竞争。不过不应该担心。一个 `std::thread` 对象发生变化，一定是在这个对象上调用了成员函数。在 `ThreadRAII` 析构函数中，不应该再有其他线程再去调用 `t` 的成员函数。

使用 `ThreadRAII` 版本的 `doWork` 函数如下：
```cpp
bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion)
{
	std::vector<int> goodVals;

	ThreadRAII t(std::thread( // use RAII object
					 [&filter, maxVal, &goodVals]
					 {
						 for (auto i = 0; i <= maxVal; ++i)
						 {
							 if (filter(i))
							 {
								 goodVals.push_back(i);
							 }
						 }
					 }),
				 ThreadRAII::DtorAction::join // RAII action
	);

	// use t's native handle to set t's priority
	auto nh = t.get().native_handle();
	// set priority

	if (conditionsAreSatisfied())
	{
		t.get().join();
		performComputation(goodVals);
		return true;
	}

	return false;
}
```
这里我们选择 `join` 作为析构函数的行为。`detach` 会导致诡异的 bug，放任不管可能会导致程序终止，`join` 可能会导致性能异常，相比之下，最后一种选择不算太糟糕。

## Things to Remember
- Make `std::thread`s unjoinable on all paths.
- Join-on-destruction can lead to difficult-to-debug performance anomalies.
- Detach-on-destruction can lead to difficult-to-debug undefined behavior.
- Declare `std::thread` objects last in lists of data members.
