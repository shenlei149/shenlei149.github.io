### 积分表
[这里](../../Formula/A-Brief-Table-of-Integrals.md)给了一个积分表。其中 $a,b,c,m,n$ 是常数，通常可以是任意实数而不必只是整数。极偶尔的对它们有所限制，比如公式 21 中 $n\neq -1$，公式 27 中 $n\neq -2$。

这些常数要使得公式有意义，分母不能为零，根号下的式子必须非负等。比如公式 24 隐含了 $a\neq 0$，公式 29a 和 29b 隐含了 $b>0$。

例1 求
$$\int x(2x+5)^{-1}dx$$
解：利用公式 24
$$\int x(ax+b)^{-1}dx=\frac{x}{a}-\frac{b}{a^2}\ln|ax+b|+C$$
代入 $a=2,b=5$ 得到
$$\int x(2x+5)^{-1}dx=\frac{x}{2}-\frac{5}{4}\ln|2x+5|+C$$

例2 求
$$\int\frac{dx}{x\sqrt{2x-4}}$$
解：利用公式 29b
$$\int\frac{dx}{x\sqrt{ax-b}}=\frac{2}{\sqrt{b}}\tan^{-1}\sqrt{\frac{ax-b}{b}}+C$$
代入 $a=2,b=4$ 得到
$$\int\frac{dx}{x\sqrt{2x-4}}=\frac{2}{\sqrt{4}}\tan^{-1}\sqrt{\frac{2x-4}{4}}+C=\tan^{-1}\sqrt{\frac{x-2}{2}}+C$$

例3 求
$$\int x\sin^{-1} xdx$$
解：首先应用公式 106
$$\int x^n\sin^{-1} axdx=\frac{x^{n+1}}{n+1}\sin^{-1} ax-\frac{a}{n+1}\int\frac{x^{n+1}dx}{\sqrt{1-a^2x^2}},n\neq -1$$
将 $a=1,n=1$ 代入
$$\int x\sin^{-1} xdx=\frac{x^2}{2}\sin^{-1} x-\frac{1}{2}\int\frac{x^2dx}{\sqrt{1-x^2}}$$
利用公式 49 计算上面式子的最后一项
$$\int\frac{x^2}{\sqrt{a^2-x^2}}dx=\frac{a^2}{2}\sin^{-1}\frac{x}{a}-\frac{1}{2}x\sqrt{a^2-x^2}+C$$
代入 $a=1$ 得到
$$\int\frac{x^2}{\sqrt{1-x^2}}dx=\frac{1}{2}\sin^{-1} x-\frac{1}{2}x\sqrt{1-x^2}+C$$
那么就得到了结果
$$\begin{aligned}
\int x\sin^{-1} xdx&=\frac{x^2}{2}\sin^{-1} x-\frac{1}{2}(\frac{1}{2}\sin^{-1} x-\frac{1}{2}x\sqrt{1-x^2}+C)\\
&=(\frac{x^2}{2}-\frac{1}{4})\sin^{-1}x+\frac{1}{4}x\sqrt{1-x^2}+C
\end{aligned}$$

### 规约公式
我们将规约公式 93、117 和 75 的简化版本复写在这里
$$\int\tan^n xdx=\frac{\tan^{n-1}x}{n-1}-\int\tan^{n-2} xdx,n\neq 1$$
$$\int (\ln x)^mdx=x(\ln x)^m-m\int x^n(\ln x)^{m-1}dx$$
$$\int\sin^n x\cos^m xdx=-\frac{\sin^{n-1} x\cos^{m+1} x}{m+n}+\frac{n-1}{m+n}\int\sin^{n-2} x\cos^m xdx,n\neq -m$$
这些公式的共同点是公式有重复的部分，不过相比原始被积分的项，幂次在减少，那么反复迭代总能得到结果。

例4 求
$$\int\tan^5 xdx$$
解：这里反复应用上面的第一个公式。首先，代入 $n=5$
$$\int\tan^5 xdx=\frac{\tan^4 x}{4}-\int\tan^3 xdx$$
然后代入 $n=3$
$$\int\tan^3 xdx=\frac{\tan^2 x}{2}-\int\tan xdx=\frac{1}{2}\tan^2 x+\ln|\cos x|+C$$
结合两个式子得到
$$\int\tan^5 xdx=\frac{1}{4}\tan^4 x-\frac{1}{2}\tan^2 x-\ln|\cos x|+C$$

### 使用计算机代数系统求积分
书中推荐 Maple、Mathematica。个人经验 WolframAlpha 非常好用。

### 非初等积分
很多函数的反函数不能表达成我们目前讨论过的函数，比如多项式、三角函数、指数函数等。这些函数的积分是非初等积分。这些积分可以表示成第九章讲解的无穷级数或者用下一小节讲解的数值法计算。非初等积分包含误差函数
$$\text{erf}(x)=\frac{2}{\sqrt{x}}\int_0^x e^{-t^2}dt$$
还有诸如下面这些常在工程和物理学出现的积分
$$\int\sin x^2dx,\int\sqrt{1+x^4}dx$$
还有
$$\int\frac{e^x}{x}dx,\int e^{e^x}dx,\int\frac{1}{\ln x}dx,\int\ln(\ln x)dx$$
$$\int\frac{\sin x}{x}dx,\int\sqrt{1-k^2\sin^2 x}dx,0<k<1$$
这些函数看起来很简单，但是求解起来就发现非常困难，已经证明这些函数的积分无法用初等函数表示。根据微积分基本定理，这些连续函数必然有反函数。
