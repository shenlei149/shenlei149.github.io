在数学领域，使用一个类表示多项式既直接又方便。该类可能提供一个用于求根的方法。该方法不需要修改多项式，因此将其声明为 `const` 很自然。
```cpp
class Polynomial
{
public:
    // data structure holding values where polynomial evals to zero
    using RootsType = std::vector<double>;

    RootsType roots() const;
};
```
计算根很耗时，应尽可能避免重复计算。一个直观的方案是只计算一次，并将结果缓存起来供后续访问使用。
```cpp
class Polynomial
{
public:
    using RootsType = std::vector<double>;
    RootsType roots() const
    {
        if (!rootsAreValid)
        {
            // if cache not valid compute roots,
            // store them in rootVals
            rootsAreValid = true;
        }

        return rootVals;
    }

private:
    mutable bool rootsAreValid{false};
    mutable RootsType rootVals{};
};
```
`roots` 函数不会修改 `Polynomial` 对象的逻辑状态，但为了维护缓存，需要修改 `rootsAreValid` 和 `rootVals` 这两个变量。这是使用 `mutable` 的经典场景。

现在假定有两个线程同时调用 `Polynomial` 对象的 `roots` 函数。
```cpp
Polynomial p;

/*----- Thread 1 ----- */
auto rootsOfP = p.roots();

/*------- Thread 2 ------- */
auto valsGivingZero = p.roots();
```
客户端这样写非常合理。`roots` 是 `const` 函数，意味着它在逻辑上是只读操作；多个线程在没有同步的情况下执行只读操作应当是安全的。至少应做到这一点。然而，`roots` 并不是线程安全的，因为两个线程都可能修改成员变量 `rootsAreValid` 和 `rootVals`。这意味着不同线程会在没有同步的情况下读写相同的内存，从而导致数据竞争，行为未定义。

问题在于，`roots` 虽然是 `const` 函数，却不是线程安全的，因此需要将其实现为线程安全的函数。

常见的方案是使用 `mutex`：
```cpp
class Polynomial
{
public:
    using RootsType = std::vector<double>;
    RootsType roots() const
    {
        std::lock_guard<std::mutex> g(m); // lock mutex

        if (!rootsAreValid)
        {
            // if cache not valid compute roots,
            // store them in rootVals
            rootsAreValid = true;
        }

        return rootVals;
    } // unlock mutex

private:
    mutabe std::mutex m;
    mutable bool rootsAreValid{false};
    mutable RootsType rootVals{};
};
```
由于加锁和解锁不是 `const` 成员函数，需要将 `std::mutex m` 声明为 `mutable`；否则，它在 `const` 函数 `roots` 中会被视为 `const` 对象。

`std::mutex` 既不能拷贝也不能移动，因此包含它的 `Polynomial` 默认也不能拷贝或移动。

在许多场景中，`mutex` 的开销过大。例如，若只需统计函数的调用次数，使用 `std::atomic` 作为计数器类型的开销通常更小。下面展示了如何使用 `std::atomic` 变量统计调用次数。
```cpp
class Point // 2D point
{
public:
    double distanceFromOrigin() const noexcept
    {
        ++callCount; // atomic increment
        return std::sqrt((x * x) + (y * y));
    }

private:
    mutable std::atomic<unsigned> callCount{0};
    double x, y;
};
```

和 `std::mutex` 类似，`std::atomic` 既不能拷贝也不能移动，因此 `Point` 默认也不能拷贝或移动。

`std::atomic` 变量的操作较轻量，因此可能会让人过度依赖它。下面的例子需要缓存一个计算耗时得到的 `int` 值，这里使用一对 `std::atomic` 变量，而不是 `mutex`。
```cpp
class Widget
{
public:
    int magicValue() const
    {
        if (cacheValid)
            return cachedValue;
        else
        {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    }

private:
    mutable std::atomic<bool> cacheValid{false};
    mutable std::atomic<int> cachedValue;
};
```
这段代码可以正常工作，但可能会执行超出预期次数的计算。
1. 一个线程调用 `Widget::magicValue`；此时 `cacheValid` 为 `false`，该线程开始执行两个耗时的计算，然后写入 `cachedValue`。
2. 第二个线程也调用 `Widget::magicValue`，同样发现 `cacheValid` 为 `false`，于是又执行一遍这两个耗时的计算。这样的第二个线程可能不止一个。

为了解决这个问题，可以尝试交换为 `cachedValue` 和 `cacheValid` 赋值的顺序。但很快会意识到：1）在 `cacheValid` 被设为 `true` 之前，仍可能发生重复计算，这违背了修改的初衷；2）这样会使问题更加严重。
```cpp
class Widget
{
public:
    int magicValue() const
    {
        if (cacheValid)
            return cachedValue;
        else
        {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cacheValid = true;
            return cachedValue = val1 + val2;
        }
    }

private:
    mutable std::atomic<bool> cacheValid{false};
    mutable std::atomic<int> cachedValue;
};
```
考虑 `cacheValid` 为 `false` 的情况：
1. 一个线程调用 `Widget::magicValue`，然后将 `cacheValid` 设为 `true`。
2. 第二个线程调用 `Widget::magicValue`，发现 `cacheValid` 为 `true` 后返回 `cachedValue`；但此时第一个线程可能尚未设置该值，因此返回的结果会是错误的。

通常，若只需同步一个变量或内存位置，可以使用 `std::atomic`；否则，应考虑使用 `mutex`。上例的正确实现方式如下。
```cpp
class Widget
{
public:
    int magicValue() const
    {
        std::lock_guard<std::mutex> guard(m); // lock m
        if (cacheValid)
            return cachedValue;
        else
        {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    } // unlock m

private:
    mutable std::mutex m;
    mutable int cachedValue;        // no longer atomic
    mutable bool cacheValid{false}; // no longer atomic
};
```

本条款的建议基于这样一个假设：多个线程可以同时调用同一对象的 `const` 成员函数。如果能够确保同一对象的 `const` 成员函数绝不会被多个线程同时调用，那么该函数是否线程安全就不重要了。这样可以避免引入 `mutex` 或 `std::atomic` 所带来的性能开销，以及由此导致对象默认不可复制和不可移动的问题。不涉及并发的场景越来越少，今后只会更少。`const` 成员函数应支持并发调用，因此应确保其线程安全。

## Things to Remember
* Make `const` member functions thread-safe unless you're *certain* they'll never be used in a concurrent context.
* `std::atomic` variables may offer better performance than a mutex, but they are suitable only for manipulating a single variable or memory location.
