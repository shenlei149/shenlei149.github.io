一个级数中的项有正有负，它可能收敛也可能发散。比如下面的等比级数
$$5-\frac{5}{4}+\frac{5}{16}-\frac{5}{64}+\cdots=\sum_{n=0}^\infty 5(\frac{-1}{4})^n$$
收敛，原因是 $|r|=\frac{1}{4}<1$。不过下面的等比级数
$$1-\frac{5}{4}+\frac{25}{16}-\frac{125}{64}+\cdots=\sum_{n=0}^\infty(\frac{-5}{4})^n$$
就是发散的，因为公比 $|r|=\frac{5}{4}>1$。对于第一个级数，计算部分和的时候，一些项可以相互抵消，帮助其收敛。让级数的所有项都是正数，我们得到新的级数
$$5+\frac{5}{4}+\frac{5}{16}+\frac{5}{64}+\cdots=\sum_{n=0}^\infty |5(\frac{-1}{4})^n|=\sum_{n=0}^\infty5(\frac{1}{4})^n$$
仍旧收敛。

**定义**
> 如果一个级数 $\sum a_n$ 各项绝对值组成的级数 $\sum |a_n|$ 收敛，那么称级数 $\sum a_n$ 绝对收敛。

**定理 12 绝对收敛测试**
> 如果 $\sum_{n=1}^\infty |a_n|$ 收敛，那么 $\sum_{n=1}^\infty a_n$ 收敛。

证明：对于任意 $n$，都有
$$-|a_n|\leq a_n\leq |a_n|$$
那么
$$0\leq a_n+|a_n|\leq 2|a_n|$$
如果 $\sum_{n=1}^\infty |a_n|$ 收敛，根据直接比较测试，$\sum_{n=1}^\infty 2|a_n|$ 收敛，所以非负级数 $\sum_{n=1}^\infty (a_n+|a_n|)$ 也收敛。那么通过 $a_n=(a_n+|a_n|)-|a_n|$ 可以将级数 $\sum_{n=1}^\infty a_n$ 表达成两个收敛级数的差：
$$\sum_{n=1}^\infty a_n=\sum_{n=1}^\infty(a_n+|a_n|-|a_n|)=\sum_{n=1}^\infty(a_n+|a_n|)-\sum_{n=1}^\infty|a_n|$$
因此，$\sum_{n=1}^\infty a_n$ 收敛。

例1 下面是两个绝对收敛的例子。

（a）级数 $\sum_{n=1}^\infty(-1)^{n+1}\frac{1}{n^2}=1-\frac{1}{4}+\frac{1}{9}-\frac{1}{16}+\cdots$ 对应的绝对值级数
$$\sum_{n=1}^\infty\frac{1}{n^2}=1+\frac{1}{4}+\frac{1}{9}+\frac{1}{16}+\cdots$$
收敛，所以原级数绝对收敛。

（b）级数 $\sum_{n=1}^\infty\frac{\sin n}{n^2}=\frac{\sin 1}{1}+\frac{\sin 2}{4}+\frac{\sin 3}{9}+\cdots$ 包含正数项和负数项，对应的绝对值级数
$$\sum_{n=1}^\infty|\frac{\sin n}{n^2}|=\frac{|\sin 1|}{1}+\frac{|\sin 2|}{4}+\frac{|\sin 3|}{9}+\cdots$$
是收敛的，因为对于所有 $n$ 都有 $|\sin n|\leq 1$，和收敛级数 $\sum_{n=1}^\infty(1/n^2)$ 相比即可知其也收敛。所以原级数绝对收敛。

### 比值测试
通过检查 $a_{n+1}/a_n$ 的比值，可以判定级数增长或者衰减的速率。对于等比级数 $\sum ar^n$，比值 $(ar^{n+1})/(ar^n)=r$ 是常量，级数收敛等价于比值的绝对值是否小于 1。

**定理13 比值测试**
> 令 $\sum a_n$ 是任意级数且
> $$\lim_{n\to\infty}\bigg|\frac{a_{n+1}}{a_n}\bigg|=\rho$$
> （a）如果 $\rho<1$，那么级数绝对收敛
> （b）如果 $\rho>1$ 或者是无穷大，那么级数发散
> （c）如果 $\rho=1$，收敛性不确定

证明：

（a）$\rho<1$。令 $r$ 是 $\rho$ 和 1 之间的数，那么 $\varepsilon=r-\rho$ 是正数。由于
$$\bigg|\frac{a_{n+1}}{a_n}\bigg|\to\rho$$
那么当 $n$ 充分大时，$n\geq N$，$|a_{n+1}/a_n|$ 在 $\rho$ 附近 $\varepsilon$ 以内处。
$$\bigg|\frac{a_{n+1}}{a_n}\bigg|<\rho+\varepsilon=r,n\geq N$$
那么
$$\begin{aligned}
|a_{N+1}|&<r|a_N|\\
|a_{N+2}|&<r|a_{N+1}|<r^2|a_N|\\
|a_{N+3}|&<r|a_{N+2}|<r^3|a_N|\\
\vdots\\
|a_{N+m}|&<r|a_{N+m-1}|<r^m|a_N|\\
\end{aligned}$$
因此
$$\sum_{m=N}^\infty|a_m|=\sum_{m=0}^\infty|a_{N+m}|\leq\sum_{m=0}^\infty|a_N|r^m=|a_N|\sum_{m=0}^\infty r^m$$
因为 $0<r<1$，所以最右边是收敛的等比级数，所以 $\sum_{m=N}^\infty|a_m|$ 也收敛。由于删除一个级数的有限项或者向级数中添加有限项，不影响其收敛性，所以 $\sum_{n=1}^\infty|a_n|$ 收敛。所以级数 $\sum a_n$ 绝对收敛。

（b）$1<\rho\leq\infty$。对于某些下标 $M$ 开始，有
$$\bigg|\frac{a_{n+1}}{a_n}\bigg|>1,|a_M|<|a_{M+1}|<|a_{M+2}<\cdots$$
那么级数的项不会趋于零，所以级数发散。

（c）$\rho=1$。两个级数
$$\sum_{n=1}^\infty\frac{1}{n},\sum_{n=1}^\infty\frac{1}{n^2}$$
可以证明 $\rho=1$ 时并无法确定级数是否收敛。
$$\frac{1/(n+1)}{1/n}=\frac{n}{n+1}\to 1$$
$$\frac{1/(n+1)^2}{1/n^2}=(\frac{n}{n+1})^2\to 1^2=1$$
两者的 $\rho=1$，但是第一个级数发散而第二个级数收敛。

例2 判断下面级数的收敛性。

（a）
$$\sum_{n=0}^\infty\frac{2^n+5}{3^n}$$
（b）
$$\sum_{n=1}^\infty\frac{(2n)!}{n!n!}$$
（c）
$$\sum_{n=1}^\infty\frac{4^nn!n!}{(2n)!}$$
解：

（a）
$$\bigg|\frac{a_{n+1}}{a_n}\bigg|=\frac{(2^{n+1}+5)/3^{n+1}}{(2^n+5)/3^n}=\frac{1}{3}\frac{2^{n+1}+5}{2^n+5}\to\frac{1}{3}\cdot 2=\frac{2}{3}$$
$\rho=2/3$ 小于 1，所以级数绝对收敛。但是 2/3 和级数的和无关。事实上
$$\sum_{n=0}^\infty\frac{2^n+5}{3^n}=\sum_{n=0}^\infty(\frac{2}{3})^n+\sum_{n=0}^\infty\frac{5}{3^n}=\frac{1}{1-2/3}+\frac{5}{1-1/3}=\frac{21}{2}$$
（b）
$$\begin{aligned}
\bigg|\frac{a_{n+1}}{a_n}\bigg|&=\frac{n!n!(2n+2)!}{(n+1)!(n+1)!(2n)!}\\
&=\frac{(2n+2)(2n+1)}{(n+1)(n+1)}\\
&=\frac{4n+1}{n+1}\\
&\to 4
\end{aligned}$$
$\rho=4$ 大于 1，所以级数发散。

（c）
$$\begin{aligned}
\bigg|\frac{a_{n+1}}{a_n}\bigg|&=\frac{4^{n+1}(n+1)!(n+1)!}{(2n+2)!}\frac{(2n)!}{4^nn!n!}\\
&=\frac{4(n+1)(n+1)}{(2n+2)(2n+1)}\\
&=\frac{2n+2}{2n+1}\\
&\to 1
\end{aligned}$$
$\rho=1$，无法确定级数的收敛性，需要使用其他方法。由于 $(2n+2)/(2n+1)$ 总是大于 1，所以 $a_{n+1}>a_n$，那么所有项都会大于 $a_1=2$，项不收敛到零，所以级数发散。

### 根测试
目前为止，级数 $\sum a_n$ 都比较简单。考虑通项公式如下的级数
$$a_n=\begin{cases}
n/2^n&&n \text{ 是奇数}\\
1/2^n&&n \text{ 是偶数}
\end{cases}$$
为了研究其收敛性，我们展开几项
$$\sum_{n=1}^\infty a_n=\frac{1}{2}+\frac{1}{4}+\frac{3}{8}+\frac{1}{16}+\frac{5}{32}+\frac{1}{64}+\cdots$$
很明显，这不是等比级数。当 $n$ 充分大时，项趋于零。积分测试看起来也不管用。使用比值测试
$$\bigg|\frac{a_{n+1}}{a_n}\bigg|=\begin{cases}
\frac{1}{2n},&&n \text{ 是奇数}\\
\frac{n+1}{2}&&n \text{ 是偶数}
\end{cases}$$
当 $n\to\infty$ 时，比值在接近零和非常大之间交错，极限不存在。下面介绍可以解决这个问题的测试方法。

**定理 14 根测试**
> 令 $\sum a_n$ 是任意级数且
> $$\lim_{n\to\infty}\sqrt[n]{|a_n|}=\rho$$
> （a）如果 $\rho<1$，那么级数绝对收敛
> （b）如果 $\rho>1$ 或者是无穷大，那么级数发散
> （c）如果 $\rho=1$，收敛性不确定

证明：

（a）$\rho<1$。选择足够小的 $\varepsilon >0$ 使得 $\rho+\varepsilon<1$。由于 $\sqrt[n]{|a_n|}\to\rho$，那么 $\sqrt[n]{|a_n|}$ 最终会落在 $\rho$ 附近距离小于 $\varepsilon$ 处。所以存在 $M$ 使得
$$\sqrt[n]{|a_n|}<\rho+\varepsilon,n\geq M$$
那么
$$|a_n|<(\rho+\varepsilon)^n,n\geq M$$
级数 $\sum_{n=M}^\infty(\rho+\varepsilon)^n$ 是公比为 $\rho+\varepsilon<1$ 的等比数列，收敛，所以 $\sum_{n=M}^\infty|a_n|$ 收敛。一个级数删除或者增加有限项不影响其收敛性，所以 $\sum_{n=1}^\infty |a_n|$ 收敛，所以 $\sum a_n$ 绝对收敛。

（b）$1<\rho\leq \infty$。存在 $M$ 使得 $\sqrt[n]{|a_n|}>1,n\geq M$，所以 $|a_n|>1,n>M$。级数的项不趋于零，所以级数发散。

（c）$\rho=1$。使用级数 $\sum_{n=1}^\infty\frac{1}{n},\sum_{n=1}^\infty\frac{1}{n^2}$ 可以证明当 $\rho=1$ 时无法确定级数的收敛性。两者都满足 $\sqrt[n]{|a_n|}\to 1$，但是第一个级数发散而第二个级数收敛。

例3 回到之前的例子，通项公式如下的级数
$$a_n=\begin{cases}
n/2^n&&n \text{ 是奇数}\\
1/2^n&&n \text{ 是偶数}
\end{cases}$$
是收敛还是发散？

解：使用根测试，有
$$\sqrt[n]{|a_n|}=\begin{cases}
\sqrt[n]{n}/2&&n \text{ 是奇数}\\
1/2&&n \text{ 是偶数}
\end{cases}$$
因此
$$\frac{1}{2}\leq\sqrt[n]{|a_n|}\leq\frac{\sqrt[n]{n}}{2}$$
根据 9.1 节定理 5 有 $\sqrt[n]{n}\to 1$，使用夹逼定理，$\lim_{n\to\infty}\sqrt[n]{|a_n|}=1/2<1$，所以级数收敛。

例4 下面哪个级数发散？哪个收敛？

（a）
$$\sum_{n=1}^\infty\frac{n^2}{2^n}$$
（b）
$$\sum_{n=1}^\infty\frac{2^n}{n^3}$$
（c）
$$\sum_{n=1}^\infty(\frac{1}{1+n})^n$$
解：

（a）收敛。因为
$$\sqrt[n]{\frac{n^2}{2^n}}=\frac{\sqrt[n]{n^2}}{\sqrt[n]{2^n}}=\frac{(\sqrt[n]{n})^2}{2}\to\frac{1^2}{2}<1$$
（b）发散。因为
$$\sqrt[n]{\frac{2^n}{n^3}}=\frac{2}{(\sqrt[n]{n})^3}\to\frac{2}{1^3}>1$$
（c）收敛。因为
$$\sqrt[n]{(\frac{1}{1+n})^n}=\frac{1}{1+n}\to 0<1$$
