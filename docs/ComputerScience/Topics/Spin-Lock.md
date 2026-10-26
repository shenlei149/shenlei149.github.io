# 自旋锁的进化

> 本文整理自 David Álvarez Rosa 的 [Optimizing a Spin-Lock](https://david.alvarezrosa.com/posts/optimizing-a-spin-lock/)。原文包含完整的 benchmark 和 perf 数据；这里省略测量部分，只保留四个版本的设计思路和背后的机制。

自旋锁是一种从不睡眠的锁。它不会把线程交还给调度器，而是让线程留在 CPU 上自旋。没有系统调用，没有上下文切换。下面一步一步搭出四个版本。

## V1：朴素的交换循环

先用一个原子布尔量和一个 `exchange` 循环实现。`exchange` 原子地写入 `true` 并返回旧值。旧值为 `false`，说明锁是空闲的，现在归当前线程所有；旧值为 `true`，说明别人持有锁，继续重试。

```cpp
class SpinLockV1
{
	std::atomic_bool locked_ { false };

public:
	auto lock() noexcept -> void { while (locked_.exchange(true)); }

	auto unlock() noexcept -> void { locked_.store(false); }
};
```

一个 CPU 核要写这个缓存行，必须先拿到它的独占所有权，所以等待者会互相争夺这个缓存行。缓存缺失和分支预测失败都会增加。`exchange()` 是否成功由其他核心决定，分支预测器也学不到稳定的模式。

## V2：内存序

默认的 `seq_cst` 比锁需要的更强。锁只需要在进入时获取，在离开时释放。

```cpp
class SpinLockV2
{
	std::atomic_bool locked_ { false };

public:
	auto lock() noexcept -> void { while (locked_.exchange(true, std::memory_order_acquire)); }

	auto unlock() noexcept -> void { locked_.store(false, std::memory_order_release); }
};
```

在 x86 上，`lock()` 生成的指令没有变化。

```asm
SpinLockV2::lock():
        mov     al, 1
        xchg    byte ptr [rdi], al  // Locked exchange, both orderings
        test    al, 1
        jne     .LBB0_1
        ret
```

区别在 `unlock()`：在默认的顺序一致性下，除了 `lock()` 那次读改写，`unlock()` 还要再做一次带锁语义的读改写。

```asm
SpinLockV1::unlock():
        xor     eax, eax
        xchg    byte ptr [rdi], al  // Locked read-modify-write
        ret
```

使用 `memory_order_release` 后，`unlock()` 变成一条普通存储。

```asm
SpinLockV2::unlock():
        mov     byte ptr [rdi], 0   // Plain store
        ret
```

这样一次加锁、一次解锁，只需要一次带锁操作（原子读改写），而不是两次；缓存缺失和分支预测失败也随之减少。

`exchange` 在失败时仍然会写这个缓存行，所以等待者必须停止这种写入。

## V3：Test and test-and-set

先尝试一次 `exchange()`，失败后改为在只读的 `load()` 上等待。`_mm_pause()` 告诉 CPU 当前是在自旋等待，从而降低功耗并避免流水线清空。

```cpp
class SpinLockV3
{
	std::atomic_bool locked_ { false };

public:
	auto lock() noexcept -> void
	{
		while (locked_.exchange(true, std::memory_order_acquire))
		{
			while (locked_.load(std::memory_order_relaxed))
			{				 // Read-only spin
				_mm_pause(); // Backoff
			}
		}
	}

	auto unlock() noexcept -> void { locked_.store(false, std::memory_order_release); }
};
```

内层的 `load` 可以用 `relaxed`。真正建立临界区顺序的是成功的那次 `exchange`，而不是失败的读。

只读自旋的行为是可预测的。每个等待者暂停的时间相同，所以这些线程会一起醒来。

## V4：指数退避

Intel 的优化手册给出了建议：每一轮等得更久，逐次翻倍，直到一个上限。

```cpp
class SpinLockV4
{
	std::atomic_bool locked_ { false };

public:
	auto lock() noexcept -> void
	{
		auto backoff = 1;
		while (locked_.exchange(true, std::memory_order_acquire))
		{
			do
			{
				for (auto i = 0; i < backoff; ++i)
				{
					_mm_pause(); // Backoff
				}
				backoff = backoff < 64 ? backoff << 1 : 64; // Exp. growth
			} while (locked_.load(std::memory_order_relaxed)); // Read-only spin
		}
	}

	auto unlock() noexcept -> void { locked_.store(false, std::memory_order_release); }
};
```

等待者退避的时间不同，就不会再同时醒来。

例子见 *Intel 64 and IA-32 Architectures Optimization Reference Manual* 的 Example 2-10 “Contended Locks with Increasing Back-off”。

## 小结

| 版本 | 改动 |
| --- | --- |
| V1 | 朴素 |
| V2 | 内存序 |
| V3 | Test and test-and-set |
| V4 | 指数退避 |

在大多数代码里，`std::mutex` 仍然是正确的默认选择。只有当线程绑定在专用核心上，并且经过测量之后，才值得考虑自旋锁。
