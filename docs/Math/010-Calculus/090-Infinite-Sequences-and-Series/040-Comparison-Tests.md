**定理10 直接比较测试**
> 如果级数 $\sum a_n,\sum b_n$ 对所有 $n$ 都有 $0\leq a_n\leq b_n$，那么
> 1. 如果 $\sum b_n$ 收敛，那么 $\sum a_n$ 也收敛。
> 2. 如果 $\sum a_n$ 发散，那么 $\sum b_n$ 也发散。

证明：级数 $\sum a_n,\sum b_n$ 的每一项都是非负的。根据 9.3 节定理 6 的推论可知 $\sum a_n,\sum b_n$ 收敛等价于部分和有上界。

先证明第一个部分。假定 $\sum b_n$ 收敛于 $M$。因为部分和 $\sum_{n=1}^N a_n$ 满足
$$s_N=a_1+a_2+\cdots+a_N\leq b_1+b_2+\cdots+b_N\leq\sum_{n=1}^\infty b_n=M$$
所以其有上界，那么 $\sum a_n$ 收敛。

现在证明第二个部分。$\sum a_n$ 发散，所以 $\sum_{n=1}^N a_n$ 没有上界，那么由于
$$a_1+a_2+\cdots+a_N\leq b_1+b_2+\cdots+b_N$$
那么 $\sum_{n=1}^N b_n$ 也没有上界，所以发散。

例1 定理 10 的应用。

（1）级数
$$\sum_{n=1}^\infty\frac{5}{5n-1}$$
发散，原因是第 $n$ 项
$$\frac{5}{5n-1}=\frac{1}{n-\frac{1}{5}}>\frac{1}{n}$$
而后者是发散的调和级数的第 $n$ 项。

（b）级数
$$\sum_{n=0}^\infty\frac{1}{n!}=1+\frac{1}{1!}+\frac{1}{2!}+\frac{1}{3!}+\cdots$$
收敛，因为每一项都是正值，且小于等于下面级数的对应项
$$1+\sum_{n=0}^\infty\frac{1}{2^n}=1+1+\frac{1}{2}+\frac{1}{2^2}+\cdots$$
后者是等比级数，可以求得收敛值是
$$1+\sum_{n=0}^\infty\frac{1}{2^n}=1+\frac{1}{1-1/2}=3$$
这里的推导只能说明 3 是 $\sum_{n=0}^\infty\frac{1}{n!}$ 的上界，但没有说级数收敛于 3。9.9 节会给出这个级数收敛于 e。

（c）级数
$$5+\frac{2}{3}+\frac{1}{7}+1+\frac{1}{2+\sqrt{1}}+\frac{1}{4+\sqrt{2}}+\frac{1}{8+\sqrt{3}}+\cdots+\frac{1}{2^n+\sqrt{n}}+\cdots$$
收敛。首先，我们可以截去前面三项，它们不影响级数的收敛性。剩余项和 $\sum_{n=0}^\infty(1/2^n)$ 对应项相比，$\frac{1}{2^n+\sqrt{n}}$ 小于 $\frac{1}{2^n}$，而后者是等比级数，$r=1/2<$，收敛，所以截去三项的级数也收敛，那么原始级数也收敛。

### 极限比较测试
**定理11 极限比较测试**
> 假定对 $n>N$ 都有 $a_n>0,b_n>0$，那么
> 1. 如果 $\lim_{n\to\infty}\frac{a_n}{b_n}=c,c>0$，那么 $\sum a_n,\sum b_n$ 收敛性相同。
> 2. 如果 $\lim_{n\to\infty}\frac{a_n}{b_n}=0$ 并且 $\sum b_n$ 收敛，那么 $\sum a_n$ 也收敛。
> 3. 如果 $\lim_{n\to\infty}\frac{a_n}{b_n}=\infty$ 并且 $\sum b_n$ 发散，那么 $\sum a_n$ 也发散。

证明：由于 $c/2>0$，那么存在 $N$ 使得
$$\bigg|\frac{a_n}{b_n}-c\bigg|<\frac{c}{2},n>N$$
对于 $n>N$
$$\begin{aligned}
-\frac{c}{2}&<\frac{a_n}{b_n}-c<\frac{c}{2}\\
\frac{c}{2}&<\frac{a_n}{b_n}<\frac{3c}{2}\\
(\frac{c}{2})b_n&<a_n<(\frac{3c}{2})
\end{aligned}$$
如果 $\sum b_n$ 收敛，那么 $\sum(3c/2)b_n$ 也收敛，根据直接比较法，$\sum a_n$ 收敛。如果 $\sum b_n$ 发散，那么 $\sum(c/2)b_n$ 也发散，所有 $\sum a_n$ 发散。

下面证明第二个式子，比上述证明简单。
$$\bigg|\frac{a_n}{b_n}\bigg|<\varepsilon,n>N$$
由于 $a_n>0,b_n>0$，那么
$$a_n<\varepsilon b_n$$
所以 $\sum b_n$ 收敛，$\sum\varepsilon b_n$ 收敛，所以 $\sum a_n$ 也收敛。

最后一个式子最简单。$a_n>cb_n$。

$$\tag*{$\blacksquare$}$$

例2 下面两个级数哪个发散，哪个收敛？

（a）
$$\frac{3}{4}+\frac{5}{9}+\frac{7}{16}+\cdots=\sum_{n=1}^\infty\frac{2n+1}{(n+1)^2}$$
（b）
$$\frac{1}{1}+\frac{1}{3}+\frac{1}{7}+\cdots=\sum_{n=1}^\infty\frac{1}{2^n-1}$$
（c）
$$\frac{1+2\ln 2}{9}+\frac{1+3\ln 3}{14}+\frac{1+4\ln 4}{21}+\cdots=\sum_{n=2}^\infty\frac{1+n\ln n}{n^2+5}$$
解：

（a）令 $a_n=(2n+1)/(n^2+2n+1)$，当 $n$ 充分大时，$a_n$ 的行为近似 $2n/n^2=2/n$，那么可以令 $b_n=1/n$。由于
$$\sum_{n=1}^\infty b_n=\sum_{n=1}^\infty\frac{1}{n}$$
是发散的，且
$$\lim_{n\to\infty}\frac{a_n}{b_n}=\lim_{n\to\infty}\frac{2n^2+n}{n^2+2n+1}=2$$
所以 $\sum a_n$ 发散。

（b）令 $a_n=1/(2^n-1)$，当 $n$ 充分大时，$a_n$ 的行为近似于 $1/2^n$，所以令 $b_n=1/n^2$，那么
$$\sum_{n=1}^\infty b_n=\sum_{n=1}^\infty\frac{1}{2^n}$$
是收敛的，并且因为
$$\lim_{n\to\infty}\frac{a_n}{b_n}=\lim_{n\to\infty}\frac{2^n}{2^n-1}=1$$
所以 $\sum a_n$ 是收敛的。

（c）令 $a_n=(1+n\ln n)/(n^2+5)$，当 $n$ 充分大时，$a_n$ 的行为近似于 $(n\ln n)/(n^2)=\ln n/n$，当 $n\geq 3$ 时，$\ln n/n>1/n$，令 $b_n=1/n$，那么 $\sum b_n$ 发散，且由于
$$\lim_{n\to\infty}\frac{a_n}{b_n}=\lim_{n\to\infty}\frac{n+n^2\ln n}{n^2+5}=\infty$$
所以 $\sum a_n$ 发散。

例3 下面的级数收敛吗？
$$\sum_{n=1}^\infty\frac{\ln n}{n^{3/2}}$$
解：$\ln n$ 增长速度比 $n^c$ 慢得多，其中 $c$ 是任意整数，所以这里可以和 $p$ 级数相比。为了得到 $p$ 级数，有
$$\frac{\ln n}{n^{3/2}}<\frac{n^{1/4}}{n^{3/2}}=\frac{1}{n^{5/4}}$$
令 $b_n=\frac{1}{n^{5/4}}$，$\sum b_n$ 是 $p$ 级数，且 $p>1$，收敛。令 $a_n=\frac{\ln n}{n^{3/2}}$，那么
$$\begin{aligned}
\lim_{n\to\infty}\frac{a_n}{b_n}&=\lim_{n\to\infty}\frac{\ln n}{n^{1/4}}\\
&=\lim_{n\to\infty}\frac{1/n}{(1/4)n^{-3/4}}\\
&=\lim_{n\to\infty}\frac{4}{n^{1/4}}\\
&=0
\end{aligned}$$
所以 $\sum a_n$ 级数。
