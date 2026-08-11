并行的挑战并不是硬件。很少有应用程序为了多核处理器重写。使用多核让处理速度更快很难，甚至在核数增加的情况下变得更差。

为什么开发并发任务比顺序程序更难呢？

首先，必须要有性能收益或者能耗收益，否则不如继续使用顺序程序，设计更简单，跑在单个处理器上。单处理器的设计技术，比如超标量、乱序执行、指令集并行等不涉及程序员，无需重写，程序员什么都不做就能在新机器上跑的更快。

为什么写并发程序困难，特别是核数变多的场景呢？这就涉及到了调度，如何划分任务，尽可能的平衡负载，同步，任务间的通信。

下面使用 Amdahl's 法则分析并行困难之处和收益。

#### Speed-up Challenge
假定在 100 核上要提速 90 倍，那么能顺序执行的部分有多大？

Amdahl's 法则是说
$$\text{Execution time after improvement}=\frac{\text{Execution time affected by improvement}}{\text{Amount of improvement}}+\text{Execution time unaffected}$$
加速比公式是
$$\text{Speed-up}=\frac{\text{Execution time before}}{(\text{Execution time before}-\text{Execution time unaffected})+\frac{\text{Execution time affected}}{\text{Amount of improvement}}}$$
令原始执行时间是单位 1，那么被影响的执行时间就是被影响的比例，那么公式可以改写为
$$\text{Speed-up}=\frac{1}{(1-\text{Fraction time affected}+\frac{\text{Fraction time affected}}{\text{Amount of improvement}})}$$
加速 90 倍，提升数量是 100，代入得到
$$90=\frac{1}{(1-\text{Fraction time affected})+\frac{Fraction time affected}{100}}$$
那么
$$\text{Fraction time affected}=0.999$$
也就是说，为了在 100 核上提升 90 倍速度，那么只能有 0.1% 的部分是顺序执行的。

#### Speed-up Challenge: BIgger Problem
假定做两个加法：第一个是加 10 个标量值，第二是是求一个 10 乘 10 的矩阵的和。现在假定只有矩阵求和能够并行化。如果有 10 和 40 核，加速比分别是多少？假定矩阵是 20 乘 20 的矩阵呢？

假定加法需要的时间是 $t$，那么做 10 次加法和 100 次加法在单个处理器上耗时 $110t$，在 10 个核上耗时是
$$\text{Execution time after improvement}=\frac{100t}{10}+10t=20t$$
加速比是 $110t/20t=5.5$ 倍。40 核时
$$\text{Execution time after improvement}=\frac{100t}{40}+10t=12.5t$$
加速比是 $110t/12.5t=8.8$。

平摊到每个核，10 核的情况下有 55% 的提升，40 核的情况下仅有 22% 提升。

如果问题规模增加到 20 乘 20，那么单处理器情况下耗时 $10t+400t=410t$，10 核的情况下耗时是
$$\text{Execution time after improvement}=\frac{400t}{10}+10t=50t$$
加速比是 $410t/50t=8.2$。40 核时
$$\text{Execution time after improvement}=\frac{400t}{40}+10t=20t$$
加速比是 $410/20t=20.5$。因此，10 核的情况下有 82% 的提升，40 核的情况下有 51% 提升。

这个例子说明保持问题规模不变的情况下载多处理器上获取更好的加速比比增加问题规模来获得更好的加速比更困难。这就引出了零个术语。

强缩放（`strong scaling`）测量的是固定问题规模的加速比。弱缩放（`weak scaling`）指的是问题规模随着核数增长而成比例增长。假定固定问题规模，$M$ 是内存数，$P$ 是处理器数量，对于强缩放的每处理器内存量是 $M/P$，而弱缩放依旧是 $M$。

相比强缩放，内存层次更容易影响弱缩放。比如数据量无法放到多处理器的最后一级缓存中，那么性能会比强缩放差很多。

最后这个例子说明负载平衡的重要性。

#### Speed-up Challenge: Balancing Load
之前的例子中，40 核加速比 20.5 倍，这里假定负载是完美平衡的，每个核 2.5%。现在有一个核的任务量比其他核的负载更大。计算最大负载是两倍（5%）和最大负载是五倍（12.5%）的情况下的加速比，并计算此时其他核的利用率。

如果一个核有 5% 的负载，那么计算 20 次加法，其余 39 个核计算 380 次加法。这些计算是并行的，那么耗时两个部分中较大的值。
$$\text{Execution time after improvement}=\max{\frac{380t}{39},\frac{20t}{1}}+10t=30t$$
加速比从 20.5 降到了 $410t/30t=14$。其余的核的利用率小于一半，因为最长耗时是 $20t$，而自身耗时仅有 $380t/39=9.7t$。

如果一个核有 12.5% 的负载，执行 50 次加法。那么
$$\text{Execution time after improvement}=\max{\frac{350t}{39},\frac{50t}{1}}+10t=60t$$
加速比进一步降低至 $410t/60t=7$。其余核利用率小于 20%，自身耗时 $9t$，最长的任务耗时 $50t$。这个例子说明了负载平衡的重要性，最大负载是两倍的情况下，加速比就下降了三分之一，最大负载是五倍的情况下，加速比只剩下三分之一。
