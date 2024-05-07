每章都会有这个小节。谬论（`fallacies`）是容易误解的概念。陷阱（`pitfalls`）是容易出错的点。

*Pitfall: Expecting the improvement of one aspect of a computer to increase overall performance by an amount proportional to the size of the improvement.*

*陷阱：期望一个方面的改进，整体性能的提高与改进收益成正比。*

优化最常见路径往往不能达到我们的目标，因为它受到优化的地方本身消耗的时间的影响。

一个简单的问题能够说明了这一点。假设一个程序在计算机上运行 100 秒，其中乘法运算占了 80 秒。 如果想让程序运行速度快五倍，那么需要将乘法速度提高多少倍？

改进后程序的执行时间由阿姆达尔定律（`Amdahl's Law`）的简单方程给出：
$$\text{New Execution time}=\frac{\text{Affected Execution time}}{\text{Amount of improvement}}+\text{Unaffected Execution time}$$

带入这个题目的数据
$$20=\frac{80}{n}+20$$
不管 $n$ 多大都不能满足条件。

*Fallacy: Computers at low utilization use little power.*

*谬论：负载低的时候消耗功耗很少。*

07 小节有说到，即使只有 10% 的负载，也有峰值时 30% 的功耗。Google 的 CPU 利用率多数时候也在 10%-50%。如果功耗能线性于负载，那么将节省大量的能源。

*Fallacy: Designing for performance and designing for energy efficiency are unrelated goals.*

*谬论：性能和功耗是不相关的目标。*

一个优化能提升性能，往往能够减少功耗。

*Pitfall: Using a subset of the performance equation as a performance metric.*

*陷阱：使用性能方程的子集作为性能评估指标。*

只是用时钟频率、指令数和 CPI 中的一个或者两个来评估性能都可能会得到错误结果。

常见的错误是使用 MIPS（`million instructions per second`）来做评估。
$$\begin{aligned}
\text{MIPS}&=\frac{\text{nInstructions}}{\text{Execution time}\times 10^6}\\
&=\frac{\text{nInstructions}}{\frac{\text{nInstructions}\times\text{CPI}}{\text{clock rate}}\times 10^6}\\
&=\frac{\text{clock rate}}{\text{CPI}\times 10^6}
\end{aligned}$$
本质上只用了其中两个指标。
