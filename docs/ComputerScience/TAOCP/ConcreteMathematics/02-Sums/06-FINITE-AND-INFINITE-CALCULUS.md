从更高的层次审视这个问题。这里会借助与传统无限积分的概念类似的有限积分的概念来求和。

无限微分
$$Df(x)=\lim_{h\to 0}\frac{f(x+h)-f(x)}{h}$$
$D$是微分算子（`derivative`）。有限积分
$$\Delta f(x)=f(x+1)-f(x)\tag{2.42}$$
$\Delta$是差分（`difference`）。差分算子是微分算子的有限模拟，$h$只能取正整数，当$h\to 0$时，$h=1$是能达到的极限了，$\Delta f(x)$是$(f(x+h)-f(x))/h$取$h=1$时的值。

符号$D$、$\Delta$是算子（`operator`），给定一个函数，会产生一个新的函数。如果$f$是实数到实数的较为光滑的函数，那么$Df$也是一个实数到实数的函数。如果$f$是任意一个实数到实数的函数，$\Delta f$也是。函数$Df$、$\Delta f$在$x$处的定义如上。

如果$f=x^m$，那么微积分中$Df=mx^{m-1}$，也就是
$$D(x^m)=mx^{m-1}$$
但是算子$\Delta$不能得到同样优美的结果。比如
$$\Delta(x^3)=(x+1)^3-x^3=3x^2+3x+1$$
但是有一类$m$的次幂在$\Delta$的作用下可以得到优美的结果，这就是有限微积分的意义所在。新型的$m$次幂有规则
$$x^{\underline{m}}=x(x-1)\cdots(x-m+1),m\ge 0\tag{2.43}$$
定义。注意$m$下面有个横线，表示从$x$开始阶梯般的一直向下。类似的，因子可以一直向上
$$x^{\overline{m}}=x(x+1)\cdots(x+m-1),m\ge 0\tag{2.44}$$
当$m=0$时，$x^{\underline{0}}=x^{\overline{0}}=1$。通常没有因子相乘时默认值是1。

这些函数称为下降阶乘幂（`falling factorial power`）和上升阶乘幂（`rising factorial power`），因为和阶乘$n!=n(n-1)\cdots 1$密切相关，有$n!=n^{\underline{n}}=1^{\overline{n}}$。

下降阶乘幂$x^{\underline{m}}$与$\Delta$运算得到
$$\begin{aligned}
\Delta(x^{\underline{m}})&=(x+1)^{\underline{m}}-x^{\underline{m}}\\
&=(x+1)(x)\cdots(x-m)-x(x-1)\cdots(x-m+1)\\
&=mx(x-1)\cdots(x-m)
\end{aligned}$$
类似于$D(x^m)=mx^{m-1}$
$$\Delta(x^{\underline{m}})=mx^{\underline{m-1}}\tag{2.45}$$

微分算子$D$的逆运算是积分算子$\int$，它们之间的关系是
$$g(x)=Df(x) \Leftrightarrow \int g(x)dx=f(x)+C$$
类似的，$\Delta$的逆运算是求和$\sum$，两个之间关系是
$$g(x)=\Delta f \Leftrightarrow \sum g(x)\delta x=f(x)+C\tag{2.46}$$
$\sum g(x)\delta x$是$g(x)$的不定和式（`the indefinite sum`）。注意，$\delta,\Delta$是成对的，和$d,D$一样。不定积分中的$C$是常数，而不定和式中的$C$是任意满足$p(x+1)=p(x)$的函数$p(x)$。比如$C$可以是周期函数$a+b\sin 2\pi x$，去查分的时候，这样的函数会被消去，就和求导是常数被消除一样。在$x$为整数时，函数$C$是常数。

无限积分的定积分：如果$g(x)=Df(x)$，那么
$$\int_a^b g(x)dx=f(x)\bigg|_a^b=f(b)-f(a)$$
类似的，如果$g(x)=\Delta f(x)$，那么
$$\sum_a^b g(x)\delta x=f(x)\bigg|_a^b=f(b)-f(a)\tag{2.47}$$
这个公式让$\sum_a^b g(x)\delta x$有了定义。但是实际意义是什么呢？我们只是类似微积分定义了这些公式，好处是容易记忆，但是如果我们不理解它的意义，那这些符号就没有用处了。先从特殊情形入手。假设$g(x)=\Delta f(x)=f(x+1)-f(x)$。如果$b=a$，有
$$\sum_a^a g(x)\delta x=f(a)-f(a)=0$$
如果$b=a+1$，那么有
$$\sum_a^{a+1} g(x)\delta x=f(a+1)-f(a)=g(a)$$
更一般地，如果$b$增加1就有
$$\begin{aligned}
\sum_a^{b+1} g(x)\delta x-\sum_a^b g(x)\delta x&=(f(b+1)-f(a))-(f(b)-f(a))\\
&=f(b+1)-f(b)\\
&=g(b)
\end{aligned}$$
根据观察和数学归纳法，可以推出当$a,b$都是正整数且$b\ge a$时，$\sum_a^b g(x)\delta x$的确切含义是
$$\sum_a^b g(x)\delta x=\sum_{k=a}^{b-1}g(k)=\sum_{a\le k<b}g(k),b\ge a\tag{2.48}$$
也就是说，确定的和式和一般带上下界限的和式是相同的，差了一个上限值。

假设给定一个未知的和式，希望得到封闭形式，假设可以写作$\sum_{a\le k<b}g(k)=\sum_a^b g(x)\delta x$。如果我们能够求得一个不定和式的函数$f$，使得$g(x)=f(x+1)-f(x)$，那么就可以直接得到答案$f(b)-f(a)$。将$\sum_{a\le k<b}g(k)$展开得到
$$\begin{aligned}
\sum_{a\le k<b}g(k)&=\sum_{a\le k<b}f(x+1)-f(x)\\
&=(f(a+1)-f(a))+(f(a+2)-f(a+1))+\cdots+(f(b-1)-f(b-2))+(f(b)-f(b-1))\\
&=f(b)-f(a)
\end{aligned}$$
$(2.48)$要求$b\ge a$，如果$a<b$呢？通过$(2.47)$可以得到
$$\begin{aligned}
\sum_a^b g(x)\delta x&=f(b)-f(a)\\
&=-(f(a)-f(b))\\
&=-\sum_b^a g(x)\delta x
\end{aligned}$$
这和定积分是一致的。类似的论证方法可以得到$\sum_a^b+\sum_b^c=\sum_a^c$，这也和定积分性质一致。完全展开的形式是
$$\sum_a^b g(x)\delta x+\sum_b^c g(x)\delta x=\sum_a^c g(x)\delta x\tag{2.49}$$
这些铺垫工作使得我们计算下降幂的和式非常简单。根据$(2.45),(2.47),(2.48)$可以得到
$$\sum_{0\leq k<n}k^{\underline{m}}=\frac{k^{\underline{m+1}}}{m+1}\bigg|_0^n=\frac{n^{\underline{m+1}}}{m+1},m,n\geq 0\tag{2.50}$$
当$m=1$时，有$k^{\underline{1}}=k$，根据有限积分和可以得到
$$\sum_{0\leq k<n}k=\frac{n^{\underline{2}}}{2}=\frac{1}{2}n(n-1)$$
这个公式还暗含着对范围$0\leq k<n$求和比$1\leq k\leq n$容易，因为前者是$f(n)-f(0)$，而后者是$f(n+1)-f(1)$。

普通的幂求和也可以用这种方法来计算。首先表达成下降幂的组合。比如
$$k^2=k^{\underline{2}}+k^{\underline{1}}$$
那么
$$\sum_{0\leq k<n}k^2=\frac{k^{\underline{3}}}{3}+\frac{k^{\underline{2}}}{2}=\frac{1}{3}n(n-1)(n-2+\frac{3}{2})=\frac{1}{3}n(n-\frac{1}{2})(n-1)$$
使用$n+1$替换$n$就可以得到之前的和式$\sum_{0\leq k\leq n}k^2$。

这比之前的方法都简单。我们再上一个层次到立方和。经过简单的计算得到
$$k^3=k^{\underline{3}}+3k^{\underline{2}}+k^{\underline{1}}$$
第六章会分析斯特林数，下降幂和普通的幂之间总是能做转换的。那么
$$\sum_{a\leq k<b}k^3=\frac{k^{\underline{4}}}{4}+k^{\underline{3}}+\frac{k^{\underline{2}}}{2}\bigg|_a^b$$
下降幂还有很多性质，使得我们不必在普通的幂和下降幂之间来回转换。比如类似$(x+y)^2=x^2+2xy+y^2$，下降幂也有$(x+y)^{\underline{2}}=x^{\underline{2}}+2x^{\underline{1}}y^{\underline{1}}+y^{\underline{2}}$。同样，$(x+y)^m$与$(x+y)^{\underline{m}}$也类似的结果成立。第五章37题给出证明（这个题目应该是目标难度）。

至此，我们看到的下降幂指数都是非负数。和普通的幂次一样，需要扩展到负数，即在$m<0$时给出合适的$x^{\underline{m}}$的定义。观察序列
$$\begin{aligned}
x^{\underline{3}}&=x(x-1)(x-2)\\
x^{\underline{2}}&=x(x-1)\\
x^{\underline{1}}&=x\\
x^{\underline{0}}&=1
\end{aligned}$$
可以发现两边除以$(x-2)$从$x^{\underline{3}}$得到$x^{\underline{2}}$，再除以$(x-1)$得到$x^{\underline{1}}$，除以$x$得到$x^{\underline{0}}$。接下来，应该除以$(x+1)$得到$x^{\underline{-1}}$，继续就会得到前几个负指数的下降幂
$$\begin{aligned}
x^{\underline{-1}}&=\frac{1}{x+1}\\
x^{\underline{-2}}&=\frac{1}{(x+1)(x+2)}\\
x^{\underline{-3}}&=\frac{1}{(x+1)(x+2)(x+3)}
\end{aligned}$$
那么一般定义是
$$x^{\underline{-m}}=\frac{1}{(x+1)(x+2)\cdots(x+m)},m>0\tag{2.51}$$
第五章会拓展到实数和复数。

有了这个定义，下降幂有了更多的性质。通常的幂法则
$$x^{m+n}=x^mx^n$$
类似的，下降幂的指数法则的形式是
$$x^{\underline{m+n}}=x^{\underline{m}}(x-m)^{\underline{n}}\tag{2.52}$$
比如$x^{\underline{2+3}}=x^{\underline{2}}(x-2)^{\underline{3}}$。对于负数$n$，我们有
$$x^{\underline{2-3}}=x^{\underline{2}}(x-2)^{\underline{-3}}=x(x-1)\frac{1}{(x-1)x(x+1)}=\frac{1}{x+1}=x^{\underline{-1}}$$
如果我们令$x^{\underline{-1}}$是$1/x$而不是$1/(x+1)$，那么在$m=-1,n=1$时$(2.52)$会不成立。取$m=-n$，通过$(2.52)$也可以得到负指数情况下的下降幂的定义。一般地，拓展原有符号的所包含的情况时，使得已有的一般法则成立的定义是最佳选择。

现在让我们确认新定义下差分性质仍旧成立。当$m<0$时，是否有$\Delta x^{\underline{m}}=mx^{\underline{m-1}}$？比如$m=-2$，差分是
$$\begin{aligned}
\Delta x^{\underline{-2}}&=\frac{1}{(x+2)(x+3)}-\frac{1}{(x+1)(x+2)}\\
&=\frac{(x+1)-(x+3)}{(x+1)(x+2)(x+3)}\\
&=-2\Delta x^{\underline{-3}}
\end{aligned}$$
结果是成立的。对所有$m<0$都是适用的。

那么$(2.50)$对负数也成立，只要不出现除零的情况。
$$\sum_{0\leq k<n}k^{\underline{m}}=\frac{k^{\underline{m+1}}}{m+1}\bigg|_0^n=\frac{n^{\underline{m+1}}}{m+1},m\neq -1$$
如果$m=-1$会出现什么情况呢？先看下微积分中的情况。
$$\int_a^b x^{-1}dx=\ln x\bigg|_a^b$$
我们需要一个函数模拟$\ln x$，也就是寻找一个函数$f(x)$满足
$$x^{\underline{-1}}=\frac{1}{1+x}=\Delta f(x)=f(x+1)-f(x)$$
不难看出当$x$是整数时
$$f(x)=\frac{1}{1}+\frac{1}{2}+\cdots+\frac{1}{x}$$
这恰好是$(2.13)$（TODO验证）中的$H_x$，其是$\ln x$的离散模拟。

下面是对下降幂和式的完整描述：
$$\sum_a^b x^{\underline{m}}\delta x=\begin{cases}
\frac{x^{\underline{m+1}}}{m+1}\bigg|_a^b&&m\neq -1\\
H_x\bigg|_a^b&&m=-1
\end{cases}\tag{2.53}$$
这就是为什么像快速排序这样的离散问题会出现调和级数。这就和连续问题的解中经常出现自然对数一样。

找到了$\ln x$对应，现在研究$e^x$的对应。也就是要找打一个函数$f(x)$满足$\Delta f(x)=f(x)$。这比较容易
$$f(x+1)-f(x)=f(x)\Leftrightarrow f(x+1)=2f(x)$$
这是一个简单的递归式，取$f(x)=2^x$即可。

$c^x$的差分也比较简单。对于任意$c$有
$$\Delta(c^x)=c^{x+1}-c^x=(c-1)c^x$$
如果$c\neq 1$，那么$c^x$的逆差分是$c^x/(c-1)$。结合$(2.47),(2.48)$可以得到几何级数的求和公式
$$\sum_{a\leq k<b}c^k=\sum_a^b c^x\delta x=\frac{c^x}{c-1}\bigg|_a^b=\frac{c^b-c^a}{c-1}$$
下表是对求和有用的差分和逆差分。

| $f=\sum g$ | $\Delta f=g$ |
|--|--|
| $x^{\underline{0}}=1$ | $0$ |
| $x^{\underline{1}}=x$ | $1$ |
| $x^{\underline{2}}=x(x-1)$ | $2x$ |
| $x^{\underline{m}}$ | $mx^{\underline{m-1}}$ |
| $x^{\underline{m+1}}/(m+1)$ | $x^{\underline{m}}$ |
| $H_x$ | $x^{\underline{-1}}=1/(x+1)$ |
| $2^x$ | $2^x$ |
| $c^x$ | $(c-1)c^x$ |
| $c^x/(c-1)$ | $c^x$ |
| $cu$ | $c\Delta u$ |
| $u+v$ | $\Delta u+\Delta v$ |
| $uv$ | $u\Delta v+Ev\Delta u$ |

连续数学和离散数据有一些对应的地方，但是某些连续概念并没有离散概念与之类似。比如，连续函数的链式求导法则，有限微积分就没有这个法则，因为$\Delta f(g(x))$没有很好的格式。除了像$c\pm x$替换$x$这样的情形外，离散变量的变换很难。

不过$\Delta (f(x)g(x))$有很好的格式，那么分部求和（微积分中的分部积分）可以用。微积分中的乘法法则
$$D(uv)=uD(v)+vD(u)$$
分部积分公式
$$\int uDv=uv-\int vDu$$
这里我们做类似的事情。首先分析差分算子作用于两个函数的积
$$\begin{aligned}
\Delta(u(x)v(x))&=u(x+1)v(x+1)-u(x)v(x)\\
&=u(x+1)v(x+1)-u(x)v(x+1)+u(x)v(x+1)-u(x)v(x)\\
&=u(x)\Delta v(x)+v(x+1)\Delta u(x)
\end{aligned}\tag{2.54}$$
我们引入移位算子$E$
$$Ef(x)=f(x+1)$$
可以简化上面的式子。使用$Ev(x)$替换$v(x+1)$就得到一个紧凑的格式
$$\Delta(uv)=u\Delta v+Ev\Delta u\tag{2.55}$$
这样，两边取和，重新排列就能得到分部求和公式
$$\sum u\Delta v=uv-\sum Ev\Delta u\tag{2.56}$$
和分部积分一样，加上明确的上下界就是确定和式的公式了。

当左边的和式比右边的和式更难处理的时候，这个公式就很有用了。函数$\int xe^xdx$是分部积分的典型例子，对应离散求和是$\sum x2^x\delta x$。这是前一章遇到的$\sum_{k=0}^n k2^k$。为了代入分部求和，令$u(x)=x,\Delta v(x)=2^x$，那么$\Delta u(x)=1,v(x)=2^x,Ev(x)=2^{x+1}$，代入
$$\sum x2^x\delta x=x2^x-\sum 2^{x+1}\delta x=x2^x-2^{x+1}+C$$
加上界限，可以计算之前得到的和式
$$\begin{aligned}
\sum_{k=0}^n k2^k&=\sum_0^{n+1} x2^x\delta x\\
&=(x2^x-2^{x+1})\bigg|_0^{n+1}\\
&=((n+1)2^{n+1}-2^{n+2})-(0-2^1)\\
&=(n-1)2^{n+1}+2
\end{aligned}$$
上一章我们计算了$\sum_{0\leq k<n}H_k$。现在利用公式，解决看似更难的和式$\sum_{0\leq k<n}kH_k$来解决这个问题。令$u(x)=H_x,\Delta v(x)=x=x^{\underline{1}}$，从而$\Delta u(x)=x^{\underline{-1}},v(x)=x^{\underline{2}}/2,Ev(x)=(x+1)^{\underline{2}}/2$，那么
$$\begin{aligned}
\sum xH_x\delta x&=\frac{x^{\underline{2}}}{2}H_x-\sum\frac{(x+1)^{\underline{2}}}{2}x^{\underline{-1}}\delta x\\
&=\frac{x^{\underline{2}}}{2}H_x-\frac{1}{2}\sum x^{\underline{1}}\delta x\\
&=\frac{x^{\underline{2}}}{2}H_x-\frac{x^{\underline{2}}}{4}+C
\end{aligned}$$
上面第一行到第二行应用了公式$(2.52)$，其中$m=-1,n=2$。给出上下界就能得到结果
$$\sum_{0\leq k<n}kH_k=\sum_0^nxH_x\delta x=\frac{n^{\underline{2}}}{2}(H_n-\frac{1}{2})\tag{2.57}$$
