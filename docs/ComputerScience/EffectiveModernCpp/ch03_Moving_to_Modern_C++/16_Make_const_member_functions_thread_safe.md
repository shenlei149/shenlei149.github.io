在数学领域中，使用一个类表示多项式是很直接、方便的事情。这个类中，可能有一个方法求根。这个方法不需要修改多项式，那么声明成 `const` 是很自然的事。
```cpp
class Polynomial
{
public:
    // data structure holding values where polynomial evals to zero
    using RootsType = std::vector<double>;

    RootsType roots() const;
};
```
计算根是耗时的事情，我们需要尽可能避免这个事情，那么最多只计算一次后续访问缓存就是一个很直观的方案。
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
`roots` 函数不修改 `Polynomial` 对象，但是作为缓存的一部分，需要修改 `rootsAreValid, rootVals` 这两个变量。这是使用 `mutable` 的经典场景。

现在假定有两个线程同时调用 `Polynomial` 对象的 `roots` 函数。
```cpp
Polynomial p;

/*----- Thread 1 ----- */
auto rootsOfP = p.roots();

/*------- Thread 2 ------- */
auto valsGivingZero = p.roots();
```
客户端这么写是非常合理的。`roots` 是 `const` 函数，意味着是只读操作，多个线程在不同步的情况下执行只读操作是安全的。至少应该做到这一点。但是 `roots` 并是不线程安全的，因为两个线程都可能修改成员变量 `rootsAreValid, rootVals`，这意味不同线程在不同步的情况下读写相同的内存，这就是数据竞争。行为是未定义的。

问题在于 `roots` 是 `const` 函数但是不是线程安全的，因此需要使这个函数实现成线程安全的。

常见的方案是使用 `mutex`
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
由于加锁和解锁不是 `const` 成员函数，所以需要将 `std::mutex m` 是声明为 `mutabe`，否则，在 `const` 函数 `roots` 中会被视为 `const` 对象。

由于 `std::mutex` 只能移动不能拷贝，所以现在 `Polynomial` 也只能移动不能拷贝了。

在很多场景 `mutex` 过重了。比如需要统计一个函数被调用的次数，使用 `std::atomic` 作为计数器类型是开销更小的方法。下面展示了如何使用 `std::atomic` 变量统计次数。
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

和 `std::mutex` 类似，`std::atomic` 也只能移动，所以 `Point` 对象也只能移动不能拷贝。

由于 `std::atomic` 变量的操作更轻，导致我们可能会过度依赖 `std::atomic`。下面的例子需要缓存一个计算耗时的 `int` 变量，这里使用一对 `std::atomic` 变量不是使用 `mutex`。
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
这个代码可以正常工作，但是可能会出现比我们预期多的计算。
1. 一个线程调用 `Widget::magicValue`，此时 `cacheValid` 是 `false`，开始调用两个耗时的计算过程，然后写入 `cachedValue`。
2. 第二个线程也调用 `Widget::magicValue`，也发现 `cacheValid` 是 `false`，开始调用第一个线程已经做过的两个耗时的计算过程。这里第二个线程可以是多个。

为了解决这个问题，尝试通过交换给 `cachedValue, cacheValid` 赋值的顺序。但是会很快意识到 1）在 `cacheValid` 设置成 `true` 之前，还是可能会重复计算，这背离了修改的初衷；2）使得问题更严重。
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
考虑 `cacheValid` 是 `false` 的情况，
1. 一个线程调用 `Widget::magicValue`，然后将 `cacheValid` 设置成 `true`。
2. 第二个线程调用 `Widget::magicValue` 发现 `cacheValid` 是 `true`，返回 `cachedValue`，但此时第一个线程可能还没有设置这个值，那么返回的结果是错的。

如果只有一个变量或内存地址需要同步，那么可以使用 `std::atomic`，否则还是需要使用 `metux`。上面的例子的正确实现方式如下。
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

这个条款的建议是基于多个线程是可以同时调用同一个对象的 `const` 成员函数这个假设。如果不是这种情况，可以确保永远不会有多线程调用同一个对象的 `const` 成员函数，那么这个函数是否线程安全就不重要了。这样可以避免引入 `metux` 或 `std::atomic` 带来的性能影响以及使得对象不能复制只能移动。线程无关的场景越来越少，未来会变得更少。`const` 成员函数应该支持并发，因此应该确保 `const` 成员函数是线程安全的。

## Things to Remember
* Make `const` member functions thread safe unless you're *certain* they'll never be used in a concurrent context.
* Use of `std::atomic` variables may offer better performance than a mutex, but they're suited for manipulation of only a single variable or memory location.
