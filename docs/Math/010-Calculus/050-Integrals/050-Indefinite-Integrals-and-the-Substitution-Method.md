微积分基本定理是说如果能找到一个函数的反导数，那么可以计算出一个连续函数的定积分。4.8 节我们给出了不定积分（`indefinite integral`）的定义，是函数 $f$ 所有的反导数，记作 $\int f(x)dx$。由于任意两个反导数之间差一个常数，那么不定积分是任意的反导数
$$\int f(x)dx=F(x)+C$$
微积分基本定理中表明的反导数和定积分的关系可以使用这个符号来解释：
$$\begin{aligned}
\int_a^bf(x)dx&=F(b)-F(a)\\
&=[F(b)+C]-[F(a)+C]\\
&=[F(x)+C]_a^b\\
&=\bigg[\int f(x)dx\bigg]_a^b
\end{aligned}$$
当我们找到 $f$ 的不定积分时，不要忘了最后加一个常量 $C$。  
我们必须严格区分定积分和不定积分。定积分 $\int_a^bf(x)dx$ 是一个数，而不定积分 $\int f(x)dx$ 是一个函数加上一个常数。  
目前，求反导数只能靠直接识别哪个函数的微分是该函数。下面介绍换元法，是一种更通用的方法。

### 换元法：反向链式法则
如果 $u$ 是关于 $x$ 的可导函数，$n$ 除了 -1 以外的整数，那么
$$\frac{d}{dx}(\frac{u^{n+1}}{n+1})=u^n\frac{du}{dx}$$
从另一个角度看，$\frac{u^{n+1}}{n+1}$ 是 $u^n(du/dx)$ 的反导数。因此
$$\int u^n\frac{du}{dx}dx=\frac{u^{n+1}}{n+1}+C$$
简写为
$$\int u^ndu=\frac{u^{n+1}}{n+1}+C$$
可以简单的看作是 $du$ 替换了 $(du/dx)dx$。莱布尼茨就洞察了这一点，引导出换元法来计算积分。与微分一样，计算积分时也有
$$du=\frac{du}{dx}dx$$

例1 求积分 $\int(x^3+x)^5(3x^2+1)dx$。  
解：令 $u=x^3+x$，那么
$$du=\frac{du}{dx}dx=(3x^2+1)dx$$
所以通过替换得到
$$\begin{aligned}
\int(x^3+x)^5(3x^2+1)dx&=\int u^5du\\
&=\frac{u^6}{6}+C\\
&=\frac{(x^3+x)^6}{6}+C
\end{aligned}$$

例2 求积分 $\int\sqrt{2x+1}dx$。  
解：这个积分不适用公式
$$\int u^ndu$$
因为令 $u=2x+1,n=1/2$，那么
$$du=\frac{du}{dx}dx=2dx$$
这不是 $dx$。原始积分中并没有 2。不过我们可以再积分号之外乘以 $1/2$ 以补偿这个因数。所以可以得到
$$\begin{aligned}
\int\sqrt{2x+1}dx&=\frac{1}{2}\int\sqrt{2x+1}\cdot 2dx\\
&=\frac{1}{2}\int u^{1/2} du\\
&=\frac{1}{2}\frac{u^{3/2}}{3/2}+C\\
&=\frac{1}{3}(2x+1)^{3/2}+C
\end{aligned}$$

**定理6 换元法**  
如果 $u=g(x)$ 在值域 $I$ 上可导，$f$ 在 $I$ 上连续，那么
$$\int f(g(x))g'(x)dx=\int f(u)du$$
证明：令 $F$ 是 $f$ 的反导数，由链式法则得到
$$\frac{d}{dx}F(g(x))=F'(g(x))g'(x)=f(g(x))g'(x)$$
所以 $F(g(x))$ 是 $f(g(x))g'(x)$ 的反导数。令 $u=g(x)$，那么
$$\begin{aligned}
\int f(g(x))g'(x)dx&=\int\frac{d}{dx}F(g(x))dx\\
&=F(g(x))+C\\
&=F(u)+C\\
&=\int F'(u)du\\
&=\int f(u)du
\end{aligned}$$
惯例使用 $u$ 作为换元法的变量，也可以使用其他变量名，比如 $v,t,\theta$ 等。如果满足定理6 的条件，那么形如 $\int f(g(x))g'(x)dx$ 的积分可以用换元法求解。这里主要挑战是确定哪个表达式被替换。

例3 求 $\int\sec^2(5x+1)\cdot 5dx$。  
解：令 $u=5x+1$，那么 $du=5dx$，所以
$$\begin{aligned}
\int\sec^2(5x+1)\cdot 5dx&=\int\sec^2udu\\
&=\tan u+C\\
&=\tan(5x+1)+C
\end{aligned}$$

例4 求 $\int\cos(7\theta+3)d\theta$。  
解：令 $u=7\theta+3$，那么 $du=7d\theta$，而原积分中没有因数 7，所以需要整个积分要额外除以 7。
$$\begin{aligned}
\int\cos(7\theta+3)d\theta&=\frac{1}{7}\int\cos(7\theta+3) 7d\theta\\
&=\frac{1}{7}\int\cos udu\\
&=\frac{1}{7}\sin u+C\\
&=\frac{1}{7}\sin(7\theta+3)+C
\end{aligned}$$
另外一种方式是通过 $du=7d\theta$ 得到 $d\theta=(1/7)du$，代入
$$\begin{aligned}
\int\cos(7\theta+3)d\theta&=\int\cos u\cdot\frac{1}{7}du\\
&=\frac{1}{7}\sin u+C\\
&=\frac{1}{7}\sin(7\theta+3)+C
\end{aligned}$$

例5 有时，积分式中出现了 $x$ 的幂次比另外一个函数的参数中 $x$ 幂次少一的情况。这就提示我们试图替换敲高阶的 $x$ 的幂次。下面的例子 $x^3$ 是一个因式的指数，另一个因式是 $x^2$，那么可以尝试令 $u=x^3$。
$$\begin{aligned}
\int x^2e^{x^3}dx&=\int e^{x^3}\cdot x^2dx\\
&=\int e^u\cdot\frac{1}{3}du\\
&=\frac{1}{3}\int e^udu\\
&=\frac{1}{3}e^u+C\\
&=\frac{1}{3}e^{x^3}+C
\end{aligned}$$

例6 有时当我们使用 $u=g(x)$ 做替换的时候，额外有一个 $x$ 因式，那么需要解方程 $u=g(x)$，使用 $u$ 表示 $x$，进而进行积分。比如
$$\int x\sqrt{2x+1}dx$$
解：根据例2 的经验，令 $u=2x+1$，那么 $du=2dx$，所以
$$\sqrt{2x+1}dx=\frac{1}{2}\sqrt{u}du$$
但是被积分的式子多了一个 $x$，通过 $u=2x+1$ 可以得到 $x=(u-1)/2$，所以
$$x\sqrt{2x+1}dx=\frac{1}{2}(u-1)\frac{1}{2}\sqrt{u}du$$
所以积分就是
$$\begin{aligned}
\int x\sqrt{2x+1}dx&=\frac{1}{4}\int (u-1)\sqrt{u}du\\
&=\frac{1}{4}\int(u^{3/2}-u^{1/2})du\\
&=\frac{1}{4}(\frac{2}{5}u^{5/2}-\frac{2}{3}u^{3/2})du+C\\
&=\frac{1}{10}(2x+1)^{5/2}-\frac{1}{6}(2x+1)^{3/2}+C
\end{aligned}$$

例7 有时，我们需要使用三角函数恒定变化变换积分式，使得可以使用换元法。

（a）
$$\begin{aligned}
\int\sin^2x dx&=\int\frac{1-\cos 2x}{2}dx\\
&=\frac{1}{2}\int(1-\cos 2x)dx\\
&=\frac{1}{2}x-\frac{1}{2}\frac{\sin 2x}{2}+C\\
&=\frac{1}{2}x-\frac{\sin 2x}{4}+C
\end{aligned}$$
（b）
$$\begin{aligned}
\int\cos^xdx&=\int\frac{1+\cos 2x}{2}dx\\
&=\frac{x}{2}+\frac{\sin 2x}{4}+C
\end{aligned}$$
（c）
$$\begin{aligned}
\int\tan xdx&=\int\frac{\sin x}{\cos x}dx\\
&=\int\frac{-du}{u}\\
&=-\ln |u|+C\\
&=-\ln |\cos x|+C\\
&=\ln\frac{1}{|\cos x|}+C\\
&=\ln|\sec x|+C
\end{aligned}$$
（d）
$$\begin{aligned}
\int\cot xdx&=\int\frac{\cos x}{\sin x}dx\\
&=\int\frac{du}{u}\\
&=\ln|u|+C\\
&=\ln|\sin x|+C
\end{aligned}$$

例8 有时被积函数需要进行一些代数运算使之可以使用换元法。下面两个例子分子分母同乘某个因式后再使用换元法。  
（a）
$$\begin{aligned}
\int\frac{dx}{e^x+e^{-x}}&=\int\frac{e^xdx}{e^{2x}+1}\\
&=\int\frac{du}{u^2+1}\\
&=\tan^{-1}u+C\\
&=\tan^{-1}(e^x)+C
\end{aligned}$$
（b）
$$\begin{aligned}
\int\sec xdx&=\int\sec x\frac{\sec x+\tan x}{\sec x+\tan x}dx\\
&=\int\frac{\sec^2x+\sec x\tan x}{\sec x+\tan x}dx\\
&=\int\frac{du}{u}\\
&=\ln|u|+C\\
&=\ln|\sec x+\tan x|+C
\end{aligned}$$
（c）
$$\begin{aligned}
\int\csc xdx&=\int\csc x\frac{\csc x+\cot x}{\csc x+\cot x}dx\\
&=\int\frac{\csc^2 x+\csc x\cot x}{\csc x+\cot x}dx\\
&=\int\frac{-du}{u}\\
&=-\ln|u|+C\\
&=-\ln|\csc x+\cot x|+C
\end{aligned}$$

现在总结如下：
$$\int\tan xdx=\ln|\sec x|+C$$
$$\int\cot xdx=\ln|\sin x|+C$$
$$\int\sec xdx=\ln|\sec x+\tan x|+C$$
$$\int\csc xdx=-\ln|\csc x+\cot x|+C$$

### 尝试不同换元法
根据经验，找到正确的替换函数是解决积分的关键步骤。如果第一次尝试失败了，没有关系，尝试其他的替换函数。有时需要配合代数和三角函数变换操作。

例9 求 $\int\frac{2zdz}{\sqrt[3]{z^2+1}}$。  
解：我们通常替换整个被积函数中最复杂的部分，然后尝试看是否能够解决。我们尝试令 $u=z^2+1$ 和 $u=\sqrt[3]{z^2+1}$ 两种替换。这个例子中都是可以的。对于其他题目，如果一种方式行不通，另一种方法可能就能奏效。  
（1）令 $u=z^2+1$
$$\begin{aligned}
\int\frac{2zdz}{\sqrt[3]{z^2+1}}&=\int\frac{du}{u^{1/3}}\\
&=\frac{3}{2}u^{2/3}+C\\
&=\frac{3}{2}(z^2+1)^{2/3}+C
\end{aligned}$$
（2）令 $u=\sqrt[3]{z^2+1}$
$$\begin{aligned}
\int\frac{2zdz}{\sqrt[3]{z^2+1}}&=\int\frac{3u^2}{u}du\\
&=3\int udu\\
&=\frac{3}{2}u^2+C\\
&=\frac{3}{2}(z^2+1)^{2/3}+C
\end{aligned}$$
