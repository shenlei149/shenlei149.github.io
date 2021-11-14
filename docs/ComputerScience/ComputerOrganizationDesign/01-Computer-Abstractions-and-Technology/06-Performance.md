### Defining Performance
响应时间（`response time`），就是一个任务从开始到结束的时间，也称为执行时间（`execution time`）。

吞吐（`throughput`），指的是给定时间处理的任务数，也称为带宽（`bandwidth`）。

减少响应时间一定能提高吞吐，反之才不一定。

我们衡量 $X$ 的性能，使用如下公式
$$\text{Perf}_X=\frac{1}{\text{Eexe Time}}_X$$
定义 $X$ 比 $Y$ 快多少
$$\frac{\text{Perf}_X}{\text{Perf}_Y}=n$$
为了统一，一般都说性能提高多少倍，而不是混用性能提高和执行之间减少。

### Measuring Performance
时间本身有多种定义。

最直观的是挂钟时间（`wall clock time`），也称为运行时间或经过时间（`elapsed time`），就是程序执行所花的实际时间，包括 CPU 运算、访问内存、I/O 等等一切开销。

计算机上往往运行多个程序，操作系统往往是优化吞吐而不是一个程序的执行时间。CPU 执行时间（`CPU execution time`）单指消耗了多少 CPU 的时间，又可以分为用户 CPU 时间（`user CPU time`）和系统 CPU 时间（`system CPU time`）。

衡量 CPU 时间往往使用时钟周期（`clock cycles`, `clock ticks`, `clock period` 等）表示。还有一个概念是时钟频率（`clock rate`），是时钟周期的倒数。比如时钟周期是 250ps，时钟频率是 4GHz。

### CPU Performance and Its Factors
$$\text{Exec time}=\text{nCycles}\times\text{cycle time}=\frac{\text{nCycles}}{\text{clock rate}}$$
那么硬件设计师可以通过减少程序用的 CPU 时钟周期数或者提高时钟频率。不过，往往需要折衷，因为许多减少程序的时钟周期数的技术往往会也会增加时钟周期。

### Instruction Performance
$$\text{nCycles}=\text{nInstructions}\times\text{CPI}$$
每条指令的时钟周期（`clock cycles per instruction`, `CPI`）指的是一个程序或者程序段中每条指令的平均时钟周期数。

### The Classic CPU Performance Equation
$$\text{Exec time}=\text{nInstructions}\times\text{CPI}\times\text{cycle time}=\frac{\text{nInstructions}\times\text{CPI}}{\text{clock rate}}$$
一般地，我们可以容易地测量 CPU 时间，时钟周期往往硬件规格会给出。剩余两项比较难获得，不过获得其中一个，另一个也就确定了。有一些软件可以帮助我们获取指令数。

性能是由这三个因子共同确定的。只是用其中一个分析可能会误入歧途。比较两个机器，那么需要综合考虑这三个因子。

算法、编程语言、编译器、指令集架构都会影响性能。具体的看，前面三个会影响指令数和 CPI，而指令集架构会影响三者。比如一个算法除法用的多，那么 CPI 会比较大。
