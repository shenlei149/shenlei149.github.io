C++ STL 提供了`std::thread::hardware_concurrency()`接口，返回一个标示（`indication`），表示能够真正并发多少线程运行。多核系统可能会返回 CPU 核数。如果这个信息不可用，也可能返回零。不管怎样，可以用这个标示来指导分割任务。

下面是一个并行`std::accumulate`的简单实现。现实中应该使用第十章介绍的`std::reduce`而不是自己实现一个，这里是为了解释基本概念。每个线程会至少分配一定的元素，避免创建过多的线程。这里假设不会有异常，实际上`std::thread`的构造可能会失败，第八章详细介绍异常的处理。
```cpp
template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);

    for (auto &entry : threads)
        entry.join();

    return std::accumulate(results.begin(), results.end(), init);
}
```
代码虽然有点长，但很直观。如果传入长度为0，那么返回`init`；否则，分割任务。这里设置最小的任务数来计算最大线程数是为了防止创建太多的线程。比如一共传入5个元素，不要在一个32核机器上创建32个线程。

计算的最大线程数和硬件线程数两者取其小。我们不应该创建超过硬件线程数的线程，否则上下文切换太多会导致性能下降。如果`std::thread::
hardware_concurrency()`返回零，我们也要提供一个替代选项，比如这里的2。选择的值太大会在单核机器上性能不佳，选择的值太小不能充分利用并发能力。

用总数除以线程数得到每个线程要处理的元素数量，无法整除的问题后面会处理。

创建`num_threads - 1`个线程放到`std::vector<std::thread>`里面，相应地，`std::vector<T>`用于存放中间结果。减一的原因是自身也是一个线程。

一个简单的循环分割任务，启动线程。最后剩余的（可能很多，也可能一个也没有）交给当前线程处理。

最后等待其他线程结束，调用`std::accumulate`得到最终结果。

关于这个例子，还有几个事情值得注意。如果`T`的累加运算不满足结合律，那么结果是不确定的。第二，`std::accumulate`要求迭代器是输入迭代器（`input iterators`），而我们要求至少是前向迭代器（`forward iterators`）。第三，我们用`vector`保存结果，需要`T`类型有默认构造函数。为了使之能并行，新的算法必然有一些新的要求。第八章会讲更多并行算法问题。第十章会介绍`std::reduce`。这里用`vector`保存中间结果的原因是我们目前没有其他方式从线程中返回值。第四章会介绍。
