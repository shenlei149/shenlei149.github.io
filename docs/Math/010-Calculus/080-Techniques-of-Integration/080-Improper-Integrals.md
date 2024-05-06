截至目前，我们所求的积分满足两个条件：一是积分的定义域 $[a,b]$ 是有限的；二是被积函数在定义域上的值域是有限的。但是实际有的积分不满足这个条件。比如函数 $y=\frac{\ln x}{x^2}, 1\leq x<\infty$，定义域是无限的；而函数 $y=\frac{1}{\sqrt{x}},0\leq x\leq 1$ 的值域是无限的。如下图所示。这些积分被称为广义积分或反常积分（`improper integrals`）。在概率论和研究一些无穷级数的收敛性质时，广义积分很有用。

![](080.010.png)

### 无穷极限积分
首先考虑定义域无穷的情况。如下图（a）所示，函数 $y=e^{-x/2}$ 的定义域是无限的。但是可以首先对某个点，比如 $b$，如下图（b）所示，求定积分，然后令其趋于无穷，得到区域的面积。

![](080.020.png)

$$A(b)=\int_0^b e^{-x/2}dx=-2{-x/2}\bigg|_0^b=-2e^{-b/2}+2$$
当 $b\to\infty$ 时，求面积 $A(b)$ 的极限。
$$\lim_{b\to\infty}A(b)=\lim_{b\to\infty}(-2e^{-b/2}+2)=2$$
所以
$$\int_0^\infty e^{-x/2}dx=\lim_{b\to\infty}\int_0^\infty e^{-x/2}dx=2$$

定义域无穷的积分称为 I 型广义积分。

如果 $f(x)$ 在 $[a,\infty)$ 上连续，那么
$$\int_a^\infty f(x)dx=\lim_{b\to\infty}\int_a^b f(x)dx$$

如果 $f(x)$ 在 $(-\infty,b]$ 上连续，那么
$$\int_{-\infty}^b f(x)dx=\lim_{a\to\infty}\int_a^b f(x)dx$$

如果 $f(x)$ 在 $(-\infty,\infty)$ 上连续，那么
$$\int_{-\infty}^\infty f(x)dx=\int_{-\infty}^c f(x)dx+\int_c^\infty f(x)dx$$

对于每一种情况，如果极限存在且有限，我们称广义积分收敛，且极限值是广义积分的值，反之，广义积分发散。

对于最后一种情况，$c$ 的选取不重要，方便计算即可。

如果在定义域上有 $f\geq 0$，那么广义积分可以看作是函数 $f$ 曲线下的面积。如上图所示，广义积分收敛，那么面积是有限的，就是 2。如果广义积分是发散的，曲线下的面积无限大。

例1 曲线 $y=(\ln x)/x^2$ 从 $x=1$ 到 $x=\infty$ 下的面积是有限的吗？如果是，值是多少？

解：如下图所示，我们先计算从 1 到 $b$ 的面积。

![](080.030.png)

$$\begin{aligned}
\int_1^b\frac{\ln x}{x^2}dx&=\bigg[(\ln x)(-\frac{1}{x})\bigg]_1^b-\int_1^b -\frac{1}{x}\frac{1}{x}dx&&u=\ln x,v'=1/x^2\\
&=-\frac{\ln b}{b}-\frac{1}{x}\bigg|_1^b\\
&=-\frac{\ln b}{b}-\frac{1}{b}+1
\end{aligned}$$

求 $b\to\infty$ 时面积的极限
$$\begin{aligned}
\int_1^\infty\frac{\ln x}{x^2}dx&=\lim_{b\to\infty}\int_1^b\frac{\ln x}{x^2}dx\\
&=\lim_{b\to\infty}(-\frac{\ln b}{b}-\frac{1}{b}+1)\\
&=-(\lim_{b\to\infty}\frac{\ln b}{b})-0+1\\
&=-(\lim_{b\to\infty}\frac{1/b}{1})+1\\
&=1
\end{aligned}$$
广义积分收敛，面积是有限的，1。

例2 求
$$\int_{-\infty}^\infty\frac{dx}{1+x^2}$$

解：我们选择 $c=0$，那么
$$\int_{-\infty}^\infty\frac{dx}{1+x^2}=\int_{-\infty}^0\frac{dx}{1+x^2}+\int_0^\infty\frac{dx}{1+x^2}$$

依次计算右边的每一项。
$$\begin{aligned}
\int_{-\infty}^0\frac{dx}{1+x^2}&=\lim_{a\to -\infty}\int_a^0\frac{dx}{1+x^2}\\
&=\lim_{a\to -\infty} \tan^{-1}x\bigg|_a^0\\
&=\lim_{a\to -\infty}(\tan^{-1}0-\tan^{-1}a)\\
&=0-(-\frac{\pi}{2})\\
&=\frac{\pi}{2}
\end{aligned}$$
$$\begin{aligned}
\int_0^\infty\frac{dx}{1+x^2}&=\lim_{b\to\infty}\int_0^b\frac{dx}{1+x^2}\\
&=\lim_{b\to\infty}\tan^{-1}x\bigg|_0^b\\
&=\lim_{b\to\infty}(\tan^{-1}b-\tan^{-1}0)\\
&=\frac{\pi}{2}
\end{aligned}$$
因此
$$\int_{-\infty}^\infty\frac{dx}{1+x^2}=\pi$$
广义积分就是曲线下方的面积，如下图所示。

![](080.040.png)

### $\int_1^\infty\frac{dx}{x^p}$ 的积分
$y=1/x$ 是形如$y=1/x^p$ 的广义积分收敛和分散的分界线。

例3 当 $p$ 为何值时 $\int_1^\infty\frac{dx}{x^p}$ 收敛？如果收敛，值是多少？

解：如果 $p\neq 1$
$$\int_1^b\frac{dx}{x^p}=\frac{x^{-p+1}}{-p+1}\bigg|_1^b=\frac{1}{1-p}(b^{-p+1}-1)=\frac{1}{1-p}(\frac{1}{b^{p-1}-1})$$
当 $b\to\infty$ 时
$$\lim_{b\to\infty}\frac{1}{b^{p-1}}=\begin{cases}
0,&&p>1\\\infty,&&p<1
\end{cases}$$
所以
$$\begin{aligned}
\int_1^\infty\frac{dx}{x^p}&=\lim_{b\to\infty}\int_1^b\frac{dx}{x^p}\\
&=\lim_{b\to\infty}\frac{1}{1-p}(\frac{1}{b^{p-1}-1})\\
&=\begin{cases}
\frac{1}{p-1},&&p>1\\
\infty,&&p<1
\end{cases}
\end{aligned}$$
因此，当 $p>1$ 时，积分收敛于 $\frac{1}{p-1}$，当 $p<1$ 时，积分发散。

如果 $p=1$，那么
$$\begin{aligned}
\int_1^\infty\frac{dx}{x^p}&=\int_1^\infty\frac{dx}{x}\\
&=\lim_{b\to\infty}\int_1^b\frac{dx}{x}\\
&=\lim_{b\to\infty}\ln x\bigg|_1^b\\
&=\lim_{b\to\infty}(\ln b-\ln 1)\\
&=\infty
\end{aligned}$$
积分也是发散的。

### 有垂直渐近线的积分
另外一种广义积分是在被积函数在积分区间的端点处或者中间某点处有垂直渐近线，即无穷间断点。如果被积函数 $f$ 在积分区间上为正，可以将广义积分看作是曲线下的面积。

计算 $y=1/\sqrt{x}$ 从 $x=0$ 到 $x=1$ 的积分。如下图所示，首先计算 $a$ 到 $1$ 积分。

![](080.050.png)

$$\int_a^1\frac{dx}{\sqrt{x}}=2\sqrt{x}\bigg|_a^1=2-2\sqrt{a}$$
求 $a\to 0^+$ 时的极限
$$\lim_{a\to 0^+}\int_a^1\frac{dx}{\sqrt{x}}=\lim_{a\to 0^+}(2-2\sqrt{a})=2$$
因此从 0 到 1 曲线下的面积是有限的
$$\int_0^1\frac{dx}{\sqrt{x}}=\lim_{a\to 0^+}\int_a^1\frac{dx}{\sqrt{x}}=2$$

在积分区间存在无穷间断点的积分称为 II 型广义积分。

如果 $f(x)$ 在 $(a,b]$ 上连续在 $a$ 处不连续，那么
$$\int_a^bf(x)dx=\lim_{c\to a^+}\int_c^bf(x)dx$$

如果 $f(x)$ 在 $[a,b)$ 上连续在 $b$ 处不连续，那么
$$\int_a^bf(x)dx=\lim_{c\to b^-}\int_a^cf(x)dx$$

如果 $f(x)$ 在 $c,a<c<b$ 处不连续，在区间 $[a,c)\cup(c,b]$ 上连续，那么
$$\int_a^bf(x)dx=\int_a^cf(x)dx+\int_c^bf(x)dx$$

对每一种情况而言，如果极限存在且有限，那么广义积分收敛且极限值是广义积分的值，否则，积分发散。

对于第三种情况，右边的两个广义积分都收敛时积分才收敛，否则积分是发散的。

例4 求
$$\int_0^1\frac{1}{1-x}dx$$
解：被积函数 $f(x)=1/(1-x)$ 在 $[0,1)$ 上连续，在 $x=1$ 处不连续。函数图像如下所示。

![](080.060.png)

$$\begin{aligned}
\lim_{b\to 1^{-1}}\int_0^b\frac{1}{1-x}dx&=\lim_{b\to 1^{-1}}\bigg[-\ln|1-x|\bigg]_0^b\\
&=\lim_{b\to 1^{-1}}[-\ln(1-b)+0]\\
&=\infty
\end{aligned}$$
极限是无穷大，所以积分是发散的。

例5 求
$$\int_0^3\frac{dx}{(x-1)^{2/3}}$$
解：被积函数在 $x=1$ 处是无穷间断点，在 $[0,1)$ 和 $(1,3]$ 上连续，如下图所示。

![](080.070.png)

根据 II 型广义积分定义的第三种情况
$$\int_0^3\frac{dx}{(x-1)^{2/3}}=\int_0^1\frac{dx}{(x-1)^{2/3}}+\int_1^3\frac{dx}{(x-1)^{2/3}}$$
$$\begin{aligned}
\int_0^1\frac{dx}{(x-1)^{2/3}}&=\lim_{b\to 1^-}\int_0^b\frac{dx}{(x-1)^{2/3}}\\
&=\lim_{b\to 1^-}3(x-1)^{1/3}\bigg|_0^b\\
&=\lim_{b\to 1^-}[3(b-1)^{1/3}+3]\\
&=3
\end{aligned}$$
$$\begin{aligned}
\int_1^3\frac{dx}{(x-1)^{2/3}}&=\lim_{c\to 1^+}\int_c^3\frac{dx}{(x-1)^{2/3}}\\
&=\lim_{c\to 1^+}3(x-1)^{1/3}\bigg|_c^3\\
&=\lim_{c\to 1^+}[3(3-1)^{1/3}-3(c-1)^{1/3}]\\
&=3\sqrt[3]{2}
\end{aligned}$$
所以
$$\int_0^3\frac{dx}{(x-1)^{2/3}}=3+3\sqrt[3]{2}$$

### 收敛性和发散性的判定
有的时候我们无法直接计算广义积分，但是又想分析其是否收敛。下面介绍两种测试方法，分别是直接比较法和极限比较法。

例6 下面的积分是收敛的吗？
$$\int_1^\infty e^{-x^2}dx$$
解：根据定义
$$\int_1^\infty e^{-x^2}dx=\lim_{b\to\infty}\int_1^b e^{-x^2}dx$$
这是一个非初等积分式，无法直接计算。不过可以证明当 $b\to\infty$ 其值是有限的。对于任意 $x\geq 1$，$e^{-x^2}\leq e^{-x}$，如下图所示。

![](080.080.png)

所以
$$\int_1^b e^{-x^2}dx\leq\int_1^b e^{-x}dx=-e^{-b}+e^{-1}<e^{-1}$$
因此
$$\int_1^\infty e^{-x^2}dx=\lim_{b\to\infty}\int_1^b e^{-x^2}dx$$
收敛到某个值，虽然不知道精确值，但是可以肯定比 $e^{-1}$ 小。

**定理2 直接比较法**
令 $f,g$ 在 $[a,\infty)$ 上连续，且 $0\leq f(x)\leq g(x),x\geq a$，那么
* 如果 $\int_a^\infty g(x)dx$ 收敛，那么 $\int_a^\infty f(x)dx$ 也收敛
* 如果 $\int_a^\infty f(x)dx$ 发散，那么 $\int_a^\infty g(x)dx$ 也发散

证明：如果 $0\leq f(x)\leq g(x),x\geq a$，那么根据 5.3 节定理 2 有
$$\int_a^b f(x)dx\leq\int_a^b g(x)dx$$
结论就显而易见了。

这个定理以 I 型广义积分给出，不过其对 II 型广义积分也成立。

例7 直接比较法的应用。

（1）因为
$$0\leq\frac{\sin^2 x}{x^2}\leq\frac{1}{x^2},x\geq 1$$
且根据例3 有
$$\int_1^\infty\frac{1}{x^2}dx$$
收敛，所以
$$\int_1^\infty\frac{\sin^2 x}{x^2}dx$$
收敛。

（2）因为
$$\frac{1}{\sqrt{x^2-0.1}}\geq\frac{1}{x},x\geq 1$$
且
$$\int_1^\infty\frac{1}{x}dx$$
发散，所以
$$\int_1^\infty\frac{1}{\sqrt{x^2-0.1}}$$
也发散。

（3）因为
$$0\leq\frac{\cos x}{\sqrt{x}}\leq\frac{1}{\sqrt{x}},x\in\bigg[0,\frac{\pi}{2}\bigg]$$
且
$$\begin{aligned}
\int_0^{\pi/2}\frac{dx}{\sqrt{x}}&=\lim_{a\to 0^+}\int_a^{\pi/2}\frac{dx}{\sqrt{x}}\\
&=\lim_{a\to 0^+}2\sqrt{x}\bigg|_0^{\pi/2}\\
&=\lim_{a\to 0^+}(\sqrt{2\pi}-2\sqrt{a})\\
&=\sqrt{2\pi}
\end{aligned}$$
是收敛的，所以
$$\int_0^{\pi/2}\frac{\cos x}{\sqrt{x}}$$
也是收敛的。

**定理3 极限比较法**
如果 $f,g$ 在 $[a,\infty)$ 上连续且是正函数，如果
$$\lim_{x\to\infty}\frac{f(x)}{g(x)}=L,0<L<\infty$$
那么
$$\int_a^\infty f(x)dx,\int_a^\infty g(x)dx$$
同时收敛或者同时发散。

定理3 的证明和定理2 的证明类似。

这个定理只说明两个函数的收敛性相同，但是并没有说积分必须一样。

例8 证明
$$\int_1^\infty\frac{dx}{1+x^2}$$
收敛。提示，与 $\int_1^\infty(1/x^2)dx$ 比较。并比较二者的积分值。

解：函数 $f(x)=1/x^2,g(x)=1/(1+x^2)$ 是正函数且在 $[1,\infty)$ 上连续。
$$\begin{aligned}
\lim_{x\to\infty}\frac{f(x)}{g(x)}&=\lim_{x\to\infty}\frac{1/x^2}{1/(1+x^2)}\\
&=\lim_{x\to\infty}\frac{1+x^2}{x^2}\\
&=\lim_{x\to\infty}(\frac{1}{x^2}+1)\\
&=1
\end{aligned}$$
因此 $\int_1^\infty\frac{dx}{1+x^2}$ 与 $\int_1^\infty(1/x^2)dx$ 收敛性质相同，而后者收敛，所以前者也收敛。如下图所示。

![](080.090.png)

根据例3
$$\int_1^\infty\frac{dx}{x^2}=\frac{1}{2-1}=1$$
而
$$\begin{aligned}
\int_1^\infty\frac{dx}{1+x^2}&=\lim_{b\to\infty}\int_1^b\frac{dx}{1+x^2}\\
&=\lim_{b\to\infty}[\tan^{-1}b-\tan^{-1} 1]\\
&=\frac{\pi}{2}-\frac{\pi}{4}\\
&=\frac{\pi}{4}
\end{aligned}$$

例9 研究 $\int_1^\infty\frac{1-e^{-x}}{x}dx$ 的收敛性。

解：令 $f(x)=\frac{1-e^{-x}}{x}dx$，可以与 $g(x)=1/x$ 比较。$g(x)$ 是发散的，而 $f(x)\leq g(x)$，通过直接比较法无法判定其收敛性。下面使用极限比较法。
$$\lim_{x\to\infty}\frac{f(x)}{g(x)}=\lim_{x\to\infty}\frac{1-e^{-x}}{x}\cdot\frac{x}{1}=\lim_{x\to\infty}(1-e^{-x})=1$$
所以 $f(x)$ 也发散。
