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
