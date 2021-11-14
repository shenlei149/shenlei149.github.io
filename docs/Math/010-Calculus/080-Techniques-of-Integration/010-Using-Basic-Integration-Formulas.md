[公式大全](../../Formula/All.md)给出了很多函数的积分，下面也会再次列出几个基本函数的积分公式，结合第五章学习的替换法，能够解决很多更复杂的函数。
$$\int kdx=kx+C$$
$$\int x^ndx=\frac{x^{n+1}}{n+1}+C,n\neq -1$$
$$\int\frac{dx}{x}=\ln|x|+C$$
$$\int e^xdx=e^x+C$$
$$\int a^x=\frac{a^x}{\ln a}+C,a>0,a\neq 1$$

例1 求
$$\int_3^5\frac{2x-3}{\sqrt{x^2-3x+1}}dx$$
解：
$$\begin{aligned}
\int_3^5\frac{2x-3}{\sqrt{x^2-3x+1}}dx&=\int_1^{11}\frac{du}{\sqrt{u}}&&u=x^2+3x+1\\
&=2\sqrt{u}\bigg|_1^{11}\\
&=2(\sqrt{11}-1)
\end{aligned}$$

例2 配方法以求解
$$\int\frac{dx}{\sqrt{8x-x^2}}$$
解：对分母根号中的式子使用配方法
$$\begin{aligned}
8x-x^2&=-(x^2-8x)\\
&=-(x^2-8x+16-16)\\
&=16-(x-4)^2
\end{aligned}$$
代入式子
$$\begin{align*}
\int\frac{dx}{\sqrt{8x-x^2}}&=\int\frac{dx}{\sqrt{16-(x-4)^2}}\\
&=\int\frac{dx/4}{\sqrt{1-(\frac{x-4}{4})^2}}\\
&=\int\frac{du}{\sqrt{1-u^2}}&&u=\frac{x-4}{4}\\
&=\sin^{-1}u+C\\
&=\sin^{-1}\frac{x-4}{4}+C
\end{align*}$$

例3 求积分
$$\int(\cos x\sin 2x+\sin x\cos 2x)dx$$
解：通过三角变换简化被积分项，然后再求积分。
$$\begin{aligned}
\int(\cos x\sin 2x+\sin x\cos 2x)dx&=\int(\sin(x+2x))dx\\
&=\int\sin 3xdx\\
&=\int\frac{1}{3}\sin udu\\
&=-\frac{1}{3}\cos 3x+C
\end{aligned}$$

例4 求
$$\int_0^{\pi/4}\frac{dx}{1-\sin x}$$
解：分子分母同乘 $1+\sin x$，使得被积分项转化成了容易求解的式子。
$$\begin{aligned}\int_0^{\pi/4}\frac{dx}{1-\sin x}&=\int_0^{\pi/4}\frac{1}{1-\sin x}\cdot\frac{1+\sin x}{1+\sin x}dx\\
&=\int_0^{\pi/4}\frac{1+\sin x}{1-\sin^2 x}dx\\
&=\int_0^{\pi/4}\frac{1+\sin x}{\cos^2 x}dx\\
&=\int_0^{\pi/4}(\sec^2 x+\sec x\tan x)dx\\
&=\bigg[\tan x+\sec x\bigg]_0^{\pi/4}\\
&=1+\sqrt{2}-0-1\\
&=\sqrt{2}\end{aligned}$$

例5 求
$$\int\frac{3x^2-7x}{3x+2}dx$$
解：因为被积分项的分子的最高幂次大于分母的最高幂次，所以是假分数。我们可以先用长除法分离得到一个多项式加上余数的形式。
$$\frac{3x^2-7x}{3x+2}=x-3+\frac{6}{3x+2}$$
所以
$$\begin{aligned}
\int\frac{3x^2-7x}{3x+2}dx&=\int(x-3+\frac{6}{3x+2})dx\\
&=\frac{x^2}{2}-3x+2\ln|3x+2|+C
\end{aligned}$$

使用长除法花间假分数不见得都能起作用，8.5 节会有例子。

例6 求
$$\int\frac{3x+2}{\sqrt{1-x^2}}dx$$
解：首先分离被积分项。
$$\frac{3x+2}{\sqrt{1-x^2}}dx=3\int\frac{xdx}{\sqrt{1-x^2}}+2\int\frac{dx}{\sqrt{1-x^2}}$$
首先求第一个积分项，使用替换法
$$u=1-x^2$$
$$du=-2xdx$$
$$xdx=-\frac{1}{2}du$$
因此
$$\begin{aligned}
3\int\frac{xdx}{\sqrt{1-x^2}}&=3\int\frac{(-1/2)du}{\sqrt{u}}\\
&=-\frac{3}{2}\int u^{-1/2}du\\
&=-\frac{3}{2}\cdot 2u^{1/2}+C_1\\
&=-3\sqrt{1-x^2}+C_1
\end{aligned}$$
然后对第二项进行积分
$$2\int\frac{dx}{\sqrt{1-x^2}}=2\sin^{-1} x+C_2$$
所以
$$\int\frac{3x+2}{\sqrt{1-x^2}}dx=-3\sqrt{1-x^2}+2\sin^{-1} x+C$$

不是任何时候都很明确用什么作替换，很多时候只能尝试，如果都不奏效，可以尝试后续几节的方法。

例7 求
$$\int\frac{dx}{(1+\sqrt{x})^3}$$
解：尝试使用 $\sqrt{x}$ 替代，发现替换后的式子不容易求解。接着尝试使用 $u=1+\sqrt{x}$ 替换，得到
$$\begin{aligned}
\int\frac{dx}{(1+\sqrt{x})^3}&=\int\frac{2(u-1)du}{u^3}\\
&=\int(\frac{2}{u^2}-\frac{2}{u^3})du\\
&=-\frac{2}{u}+\frac{1}{u^2}+C\\&=\frac{1-2u}{u^2}+C\\
&=\frac{1-2(1+\sqrt{x})}{(1+\sqrt{x})^2}+C\\
&=C-\frac{1+2\sqrt{x}}{(1+\sqrt{x})^2}
\end{aligned}$$

例8 求
$$\int_{-\pi/2}^{\pi/2}x^3\sin xdx$$
解：没有合适的替代方法，或者明确的代数变换。不过仔细观察，被积分区间 $[-\pi/2,\pi/2]$ 是对称的，同时 $x^3$ 是奇函数，$\sin x$ 是偶函数，所以它们的积也是奇函数，所以
$$\int_{-\pi/2}^{\pi/2}x^3\sin xdx=0$$
