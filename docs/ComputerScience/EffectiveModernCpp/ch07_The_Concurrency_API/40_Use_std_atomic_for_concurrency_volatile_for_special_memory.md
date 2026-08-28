程序员可能会把 `volatile` 与 C++ 特性 `std::atomic` 混淆。后者的实例化（比如 `std::atomic<int>` `std::atomic<bool>`）是一个原子类型，提供了原子操作的能力，对它的表现就像是处于受互斥保护的临界区内一样，这些操作往往是通过特殊的机器指令来实现的，比互斥锁的方式更高效。

在下面这段代码执行期间，如果只有这一个线程修改 `ai`，那么其他线程只能看到 0 10 和 11 三种可能的值。
```cpp
std::atomic<int> ai(0); // initialize ai to 0
ai = 10;				// atomically set ai to 10
std::cout << ai;		// atomically read ai's value
++ai;					// atomically increment ai to 11
--ai;					// atomically decrement ai to 10
```

`std::cout << ai;` 这个语句中只有读取 `ai` 的值是原子操作，不保证整个语句都是原子操作。在读取 `ai` 的值和写到标准输出 `operator<<` 之间，其他线程可能会修改 `ai` 的值，不过这不会影响 `std::cout` 的行为，因为 `operator<<` 使用的是按值传递的 `int` 值。

第二个需要注意的点是 `ai` 的自增和自减，都是读-改-写（`read-modify-write`，`RMW`）操作，不过都是原子的，这是 `std::atomic` 的一个重要特性。一旦一个 `std::atomic` 对象构造完成，所有成员函数，包括这些 `RMW` 操作，都能被其他线程视为原子的。

在多线程环境中，使用 `volatile` 的代码没有任何保证，这样代码会引发未定义的行为，同时读写没有互斥锁保护的内存，可能会导致数据竞争。
```cpp
volatile int vi(0); // initialize vi to 0
vi = 10;			// set vi to 10
std::cout << vi;	// read vi's value
++vi;				// increment vi to 11
--vi;				// decrement vi to 10
```
下面看一个简单的多线程的例子。假定两个变量，一个是 `std::atomic<int>` 类型，另一个是 `volatile int` 类型。两个线程分别对这两个变量进行自增操作。
```cpp
std::atomic<int> ac(0);
volatile int vc(0);

// thread 1
++ac;
++vc;

// thread 2
++ac;
++vc;
```
当两个线程结束的时候，`ac` 的值一定是 2，而 `vc` 的值可能是 1，也可能是 2，甚至可能是其他任意值。因为 `vc` 没有任何保护，读写操作都不是原子的。因此有下面这种可能性，thread 1 读取了 `vc` 的值为 0，thread 2 也读取了 `vc` 的值为 0，然后 thread 1 将 `vc` 的值加 1 写回，thread 2 也将 `vc` 的值加 1 写回，这样最终的结果就是 `vc` 的值为 1，而不是预期的 2。现在 `vc` 会导致数据竞争，编译器可以做任何事情，当然实际上并不会这样。一般编译器的优化，在没有数据竞争的程序中是安全合理的，但是这些优化会在有数据竞争的情况下引发无法预测的行为。

除了上面的案例以外，`std::atomic` 还可以胜任指令顺序的问题。回到 [Item 39](./39_Consider_void_futures_for_one_shot_event_communication.md) 中传递信息的例子，假定检测任务需要计算一个值，然后通知响应任务这个值可用了。
```cpp
std::atomic<bool> valAvailable(false);
auto imptValue = computeImportantValue(); // compute value
valAvailable = true;					  // tell other task it's available
```
我们在读代码的时候，知道给 `imptValue` 赋值的语句在给 `valAvailable` 赋值的语句之前执行，且这一点至关重要。不过在编译器看来，这是两个彼此独立的语句，编译器可能会将它们重新排序，先执行 `valAvailable = true;`，然后再执行 `imptValue = computeImportantValue();`。即便编译器不这么做，硬件在执行的时候也可能会这么做。如果发生了这种情况，响应任务就会读取到一个未定义的值。

不过使用了 `std::atomic` 就强加了一个限制，代码重排不能改变程序的语义，也就是说 `std::atomic` 的写操作必须在后面。不仅仅对编译器有这样的限制，还要求编译器生成的代码保证硬件也确保这一点。

`volatile` 并不能保证强加代码重排的限制。
```cpp
volatile bool valAvailable(false);
auto imptValue = computeImportantValue();
valAvailable = true; // other threads might see this assignment before the one to imptValue!
```
编译器可能会交换两个赋值语句，即便它不这么做，编译器也不会生成代码阻止硬件这么做，这就导致其他线程在 `imptValue` 被初始化之前就看到了 `valAvailable` 的值的变化。

`volatile` 即无法保证操作的原子性，也不会限制代码的重排，这就是为什么它在并发编程中毫无作用。不过它有它的作用，它告诉编译器它正在处理的内存是行为不寻常的内存。

行为寻常（`normal`）的内存的特性是如果将一个值写入某个内存位置，这个值就会一直保留在那里，直到有其他操作将其覆盖。比如我们有变量 `int x` 和如下代码
```cpp
auto y = x; // read x
y = x;		// read x again
```
编译器会优化生成的代码，删除第一个赋值语句。

行为寻常的内存还有一个特点是如果写一个值到某个内存，从来不读，再次写入一个值，那么第一次写会被消除，因为这个值没有被用到，因此对于如下代码
```cpp
x = 10; // write x
x = 20; // write x again
```
编译器会消除第一个赋值。这意味着下面的代码会被编译器简化
```cpp
auto y = x; // read x
y = x;		// read x again

x = 10; // write x
x = 20; // write x again

// simplified to:
auto y = x; // read x
x = 20; // write x
```
通常人们不会写出这种冗余的代码，不过编译器拿到了看起来源代码，执行模板实例化、内联、各种重排优化之后，可能就会产生类似的冗余代码。

如果是行为寻常的内存，上述优化是合理的，但是对于行为特殊的内存，这种优化是不可接受的。通常行为特殊的内存用于内存映射的 I/O 设备，内存地址与外部设备交互。此时看似冗余的代码就很合理了。比如
```cpp
auto y = x; // read x
y = x;		// read x again
```
中 `x` 的值是温度传感器的值，第二次读取就不是冗余的，因为温度传感器的值可能已经发生了变化。再比如
```cpp
x = 10; // write x
x = 20; // write x again
```
中 `x` 的值用于控制无线电发射，每个值表示发送给无线电的命令，10 和 20 对应着不同的命令，如果优化了第一次写，那么发送给无线电的命令序列就发生了变化。

`volatile` 就是告诉编译器正在处理行为特殊的内存，不要对这块内存上的操作做优化。因此如果 `x` 对应的是行为特殊的内存，应该被声明为
```cpp
volatile int x;
```
下面的代码就不会被优化了，注意 `y` 的类型是 `int`，因为 `auto` 推导不会包含 `const` 或 `volatile` 限定符。
```cpp
auto y = x; // read x
y = x;		// read x again

x = 10; // write x
x = 20; // write x again
```

正是由于在处理行为特殊的内存时，看似冗余的加载和存储操作必须被保留，这也解释了为什么 `std::atomic` 不能胜任这类任务。编译器可以消除对 `std::atomic` 对象的冗余操作。假定可以写出如下代码
```cpp
std::atomic<int> x;
auto y = x;
y = x;

x = 10;
x = 20;
```
编译器可以优化成下面的形式
```cpp
auto y = x;
x = 20;
```
这对于行为特殊的内存而言是一个不可以接受的结果。

不过我们无法写出上述这种代码，因为 `std::atomic` 的拷贝构造函数和拷贝赋值运算符都是被删除的。因为 `x` 是 `std::atomic<int>` 类型，`auto` 推导结果 `y` 也是 `std::atomic<int>` 类型。之前说过，`std::atomic` 的操作都是原子的，那么赋值操作需要读 `x` 的值，然后写入 `y`，这也要求是原子。硬件通常无法实现这一点，因此 `std::atomic` 的拷贝构造函数和拷贝赋值运算符都是被删除的，因此上述代码无法编译。

如果想从 `x` 读取值写到 `y`，可以使用 `load()` `store()` 方法，分别是原子的读和写操作。
```cpp
std::atomic<int> y(x.load()); // read x
y.store(x.load());			  // read x again
```
读取 `x`（`x.load()`）是一个单独的函数调用，初始化 `y` 也是独立操作，因此整体操作不是原子的。

上述代码可以通过将 `x` 的值存储到寄存器中而不是读两次进行优化。
```cpp
int value = x.load();		  // read x into value
std::atomic<int> y(value);    // init y with value
y.store(value);               // store value into y
```
这里优化对于行为特殊的内存而言是要避免的。

使用两者的场景非常明确了：

- `std::atomic` 用于并发编程，不能用于访问行为特殊的内存。
- `volatile` 用于访问行为特殊的内存，不能用于并发编程。

由于二者用于不同的目的，因此可以结合使用
```cpp
// operations on vai are atomic and can't be optimized away
volatile std::atomic<int> vai;
```
如果场景是一个同时被多个线程并发访问的内存映射 I/O 地址，上述定义就很有用啦。

最后需要说明的是，即使在不需要的时候，也应该倾向于显式调用 `load()` 和 `store()` 方法，因为这能在源代码中清晰地表明这个变量不是一个普通的变量。访问一个 `std::atomic` 对象比访问普通变量慢得多，同时使用 `std::atomic` 会阻止编译器重排。特意显式使用 `std::atomic` 的 `load()` 和 `store()` 有助于识别潜在的瓶颈。从正确性上看，如果想向其他线程传递信息，变量赋值的时候没有看到对 `store` 的调用，可能意味着该变量应该声明为 `std::atomic` 类型但实际上却不是。

## Things to Remember
- `std::atomic` is for data accessed from multiple threads without using
mutexes. It's a tool for writing concurrent software.
- `volatile` is for memory where reads and writes should not be optimized
away. It's a tool for working with special memory.
