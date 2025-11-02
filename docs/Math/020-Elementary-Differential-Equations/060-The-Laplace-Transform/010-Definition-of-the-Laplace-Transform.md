### 广义积分
由于拉普拉斯变换涉及从 0 到无穷的积分，因此掌握一些广义积分的知识是很有必要的。

在无限区间上的广义积分定义为在有限区间上的积分的极限。
$$\int_a^\infty f(t)dt=\lim_{A\to\infty}\int_a^Af(t)dt\tag{1}$$
其中 $A$ 是正的实数。如果对于每一个 $A>a$，从 $a$ 到 $A$ 的定积分都存在，并且当 $A\to\infty$ 时这些值有一个极限，那么广义积分是收敛，值是这个极限值，否则是发散的。下面是一些简单的例子。

例 1 广义积分
$$\int_1^\infty\frac{dt}{t}$$
发散还是收敛？

解：从公式 $(1)$ 有
$$\int_1^\infty\frac{dt}{t}=\lim_{A\to\infty}\int_1^A\frac{dt}{t}=\lim_{A\to\infty}\ln A$$
由于 $\lim_{A\to\infty}\ln A=\infty$，因此广义积分发散。

例 2 求广义积分
$$\int_0^\infty e^{ct}dt$$
$c$ 为何值时其收敛？

解：假定 $c$ 是非零实数，那么
$$\begin{aligned}
\int_0^\infty e^{ct}dt&=\lim_{A\to\infty}\int_0^A e^{ct}dt\\
&=\lim_{A\to\infty}\frac{e^{ct}}{c}\bigg|_0^A\\
&=\lim_{A\to\infty}\frac{1}{c}(e^{cA}-1)
\end{aligned}$$
如果 $c<0$ 那么广义积分收敛到 $-1/c$，如果 $c>0$ 发散。如果 $c=0$，那么积分项 $e^{ct}=1$，此时
$$\lim_{A\to\infty}\int_0^A1dt=\lim_{A\to\infty}(A-0)=\infty$$
因此也是发散的。

例 3 求使得广义积分 $\int_1^\infty t^{-p}dt$ 收敛的实数 $p$。$p$ 为何值时发散？

解：假定实数 $p\neq 1$，那么
$$\int_1^\infty t^{-p}dt=\lim_{A\to\infty}\int_1^At^{-p}dt=\lim_{A\to\infty}\frac{1}{1-p}(A^{1-p}-1)$$
当 $p>1$ 时，随着 $A\to\infty$ 那么 $A^{1-p}\to 0$，此时广义积分收敛到 $1/(1-p)$。当 $p<1$ 时，$A^{1-p}\to\infty$，结合例 1，$p\leq 1$ 时发散。这个结论与无穷级数 $\sum_{n=1}^\infty n^{-p}$。

如果区间 $\alpha\leq t\leq\beta$ 能够分成有限个分区 $\alpha\leq t_0<t_1<t_2<\cdots<t_n=\beta$，函数 $f$ 满足

* $f$ 在每个子区间 $t_{i-1}<t<t_i$ 上连续
* 当趋于子区间右边端点时 $f$ 趋于某个有限极限

那么函数 $f$ 是分段连续（`piecewise continuous`）。也就是说 $f$ 在区间上有有限个跳跃间断点。如果对所有 $\beta>\alpha$ 的区间 $\alpha\leq t\leq\beta$ 上 $f$ 都是分段连续的，那么 $f$ 在 $t\geq\alpha$ 上是分段连续的。下图是一个分段连续的例子。

![](010.010.png)

分段连续函数的积分是在各个子区间上积分然后求和。比如上图中函数 $f(t)$ 的积分是
$$\int_\alpha^\beta f(t)dt=\int_a^{t_1} f(t)dt+\int_{t_1}^{t_2} f(t)dt+\int_{t_2}^\beta f(t)dt\tag{2}$$
上图中，端点 $\alpha,\beta$ 和分割点 $t_1,t_2$ 处都有值。不过，$(2)$ 中的积分并不依赖于 $f(t)$ 在这些点是否有值，积分的结果也不会变化。

因此 $f$ 在区间 $\alpha\leq t\leq A$ 上分段连续，那么 $\int_a^A f(t)dt$ 存在。因此，如果 $f$ 在 $t\geq a$ 上分段连续，那么对 $A>a$ 时 $\int_a^A f(t)dt$ 都存在。不过分段连续并不能保证广义积分 $\int_a^\infty f(t)dt$ 收敛。

如果 $f$ 的积分不能很容易的表示为基础函数，那么很难应用 $\int_a^\infty f(t)dt$ 的收敛定义。那么判断广义积分收敛还是发散最方便的方式是使用如下定理。

**定理 6.1.1**
> 如果 $f$ 在 $t\geq a$ 上分段连续，并且当 $t\geq M$ 时有 $|f(t)|\leq g(t)$，其中 $M$ 是正的常亮，如果 $\int_M^\infty g(t)dt$ 收敛，那么 $\int_a^\infty f(t)dt$ 也收敛。
>
> 相反，如果 $f(t)>g(t)>0,t\geq M$，那么如果 $\int_M^\infty g(t)dt$，那么 $\int_a^\infty f(t)dt$ 也发散。

这里忽略证明，其过程可以参考微积分教科书，比如[广义积分](../../010-Calculus/080-Techniques-of-Integration/080-Improper-Integrals.md)。

常用的比较函数是 $e^{ct},t^{-p}$，其收敛性见上面的例 1、2、3。

### 拉普拉斯变换
求解线性微分方程很有用的工具之一是积分变换（`integral transform`）。积分变换形式如下
$$F(s)=\int_\alpha^\beta K(s,t)f(t)dt\tag{1}$$
其中 $K(s,t)$ 是给定函数，是变换的内核（`kernel`），积分的上下界 $\alpha,\beta$ 也是给定值。积分上下界是无穷，即 $\alpha=-\infty,\beta=\infty$ 也是有可能的。变换关系 $(3)$ 将函数 $f$ 变换成另一个函数 $F$， $F$ 称为 $f$ 的变换（`transform`）。

在应用数学中有很多有用的积分变换，这里讨论拉普拉斯变换。令给定的 $f(t),t\geq 0$ 满足后面定理中声明的条件，那么 $f$ 的拉普拉斯变换是
$$\mathcal{L}\{f(t)\}=F(s)=\int_0^\infty e^{-st}f(t)dt\tag{4}$$
要求广义积分收敛。拉普拉斯变换的内核函数是 $K(s,t)=e^{-st}$。由于常系数线性微分方程的解是基于指数函数的，所以拉普拉斯变换对这些方程非常有用。使用拉普拉斯变换解微分方程的步骤如下：

* 使用方程 $(4)$ 将关于定义域 $t$ 上的函数 $f$ 的初值问题转换为 $s$ 定义域上 $F$ 的问题，通常更容易求解
* 求解 $F$
* 从变换 $F$ 中恢复要求的 $f$。

通常，$s$ 是复数，对应的 $F(s)$ 是复变函数。不过这里只考虑 $s$ 是实数的情况。

**定理 6.1.2**
> 假定
>
> * $f$ 是区间 $0\leq t\leq A$ 上的分段连续函数，其中 $A$ 是任意正数
> * 存在实数常量 $K,a,M$ 使得
> $$|f(t)|\leq Ke^{at},t\geq M$$
> 那么拉普拉斯变换 $\mathcal{L}\{f(t)\}=F(s)$ 在 $s>a$ 时存在。

这里需要证明 $s>a$ 时 $(4)$ 中的积分收敛。首先将广义积分分成两个部分
$$\int_0^\infty e^{-st}f(t)dt=\int_0^M e^{-st}f(t)dt+\int_M^\infty e^{-st}f(t)dt\tag{5}$$
根据假定定理的第一个假定，上式右边的第一项是有限的（存在），因此 $F(s)$ 的存在性只依赖于第二项积分的收敛性。根据第二个假设，$t\geq M$ 时
$$|e^{-st}f(t)|\leq Ke^{-st}e^{at}=Ke^{(a-s)t}$$
根据定理 6.1.1，$F(s)$ 存在的前提是 $\int_M^\infty e^{(a-s)t}$ 收敛，根据例 1，$a-s<0$ 时收敛，因此定理 6.1.2 成立。

这一章中（除了 6.5 小节之外）遇到的函数都满足上述假设。这样的函数是分段连续并且当 $t\to\infty$ 时是指数阶（`exponential order`）的。有一些函数在 $t\to\infty$ 时不是指数阶的，比如 $f(t)=e^{t^2}$，当 $t\to\infty$ 时，不管常量 $K,a$ 为何值，这个函数增长都比 $Ke^{at}$ 快的多。

下面讨论一些重要的基本函数的拉普拉斯变换。

例 4 求 $\mathcal{L}\{1\}$。

解：令 $f(t)=1,t\geq 0$，那么
$$\mathcal{L}\{1\}=\int_0^\infty e^{-st}dt=-\lim_{A\to\infty}\frac{e^{-st}}{s}\bigg|_0^A=\frac{1}{s},s>0$$

例 5 求 $\mathcal{L}\{e^{at}\}$。

解：令 $f(t)=e^{at},t\geq 0$，那么
$$\begin{aligned}
\mathcal{L}\{e^{at}\}&=\int_0^\infty e^{-st}e^{at}dt\\
&=\int_0^\infty e^{-(s-a)t}dt\\
&=\frac{1}{s-a},s>a
\end{aligned}$$

例 6 求下图中函数的拉普拉斯变换。

![](010.020.png)

解：令
$$f(t)=\begin{cases}
1,&&0<t<t\\
k,&&t=1\\
0,t>1
\end{cases}$$
其中 $k$ 是常量。在工程中 $f(t)$ 表示单位脉冲函数。这里 $f$ 是分段连续函数，因此
$$\begin{aligned}
\mathcal{L}\{f(t)\}&=\int_0^\infty e^{-st}f(t)dt\\
&=\int_0^1 e^{-st}dt\\
&=-\frac{e^{-st}}{s}\bigg|_0^1\\
&=\frac{1-e^{-s}}{s},s>0
\end{aligned}$$
注意 $\mathcal{L}\{f(t)\}$ 不依赖于 $k$，即不依赖于不连续点的函数值。即使 $f(t)$ 在该点没有定义，$f$ 的拉普拉斯变换也是一样的。因此如果许多函数只在某个点出的值不同，那么它们的拉普拉斯变换是一样的。

例 7 求 $\mathcal{L}\{\sin at\}$ 和 $a$ 为何值时变换有定义。

解：令 $f(t)=\sin at,t\geq 0$，那么
$$\mathcal{L}\{\sin at\}=F(s)=\int_0^\infty e^{-st}\sin atdt,s>0$$
那么
$$F(s)=\lim_{A\to\infty}\int_0^\infty e^{-st}\sin at dt$$
分部积分法
$$\begin{aligned}
F(s)&=\lim_{A\to\infty}\bigg[-\frac{e^{-st}\cos at}{a}\bigg|_0^A-\frac{s}{a}\int_0^A e^{-st}\cos atdt\bigg]\\
&=\frac{1}{a}-\frac{s}{a}\int_0^\infty e^{-st}\cos atdt
\end{aligned}$$
对上式第二部分再次使用分部积分法
$$\begin{aligned}
F(s)&=\frac{1}{a}-\frac{s^2}{a^2}\int_0^\infty e^{-st}\sin atdt\\
&=\frac{1}{a}-\frac{s^2}{a^2}F(s)
\end{aligned}$$
因此
$$F(s)=\frac{a}{s^2+a^2},s>0$$

类似的，$\mathcal{L}\{\cos at\}=\frac{s}{s^2+a^2},s>0$。假定 $f_1,f_2$ 两个函数在 $s>a_1,s>a_2$ 上存在拉普拉斯变换，那么对于 $s>\max(a_1,a_2)$ 有
$$\begin{aligned}
\mathcal{L}\{c_1f_1(t)+c_2f_2(t)\}&=\int_0^\infty e^{-st}(c_1f_1(t)+c_2f_2(t))dt\\
&=c_1\int_0^\infty e^{-st}f_1(t)dt+c_2\int_0^\infty e^{-st}f_2(t)dt\\
&=c_1\mathcal{L}\{f_1(t)\}+c_2\mathcal{L}\{f_2(t)\}
\end{aligned}\tag{6}$$
上式说明拉普拉斯变换是线性算子（`linear operator`）。下面是使用这个性质的例子。

例 8 求函数
$$f(t)=5e^{-2t}-3\sin 4t,t\geq 0$$
的拉普拉斯变换变换。

解：根据公式 $(6)$
$$\mathcal{L}\{f(t)\}=5\mathcal{L}\{e^{-2t}\}-3\mathcal{L}\{\sin 4t\}$$
根据之前的结论，得到
$$\mathcal{L}\{f(t)\}=\frac{5}{s+2}-\frac{12}{s^2+16},s>0$$
