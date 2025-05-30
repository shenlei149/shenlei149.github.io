这一小节回讲解如何将有理函数（多项式函数的商）写作更简单的部分之和，这些部分称为部分分式（`partial fractions`），拆分之后的函数更容易求积分。比如
$$\frac{5x-3}{x^2-2x-3}=\frac{2}{x+1}+\frac{3}{x-3}$$
这个技能在一些其他场景也很有用。如果能过左边函数写作右边简单的形式，那么求解积分就很容易了。
$$\begin{aligned}
\int\frac{5x-3}{x^2-2x-3}dx&=\int\frac{2}{x+1}dx+\int\frac{3}{x-3}dx\\
&=2\ln|x+1|+3\ln|x-1|+C
\end{aligned}$$
这种方法称为部分分式法。回到之前的例子，我们需要找到下面式子的 $A, B$
$$\frac{5x-3}{x^2-2x-3}=\frac{A}{x+1}+\frac{B}{x-3}$$
等式右边的两项是部分分式，原因是它们的分母是原始分母 $x^2-2x-3$ 的一部分。$A, B$ 称为待定系数。

为了求解 $A, B$，我们需要重新按照 $x$ 的幂次整理上面的方程
$$5x-3=A(x-3)+B(x+1)=(A+B)x-3A+B$$
上式成立等价于 $x$ 幂次相同的系数也相同，即
$$5=A+B,-3=-3A+B$$
解这个二元一次方程组得到 $A=2,B=3$。

### 部分分式法的一般描述
将有理函数 $f(x)/g(x)$ 写作部分分式之和依赖于下面两个事实。
* $f(x)$ 的最高幂次小于 $g(x)$ 的最高幂次，即 $f(x)/g(x)$ 是真分数。如果不是，用 $g(x)$ 除 $f(x)$，然后用部分分式法求解余数部分。例3 就是这种情况。
* 需要知道 $g(x)$ 的因数。理论上，任意多项式都能写作线性因子和二次项因子的乘积。不过实践中可能很难分解因数。

当 $g(x)$ 因数知道的时，下面是求真分数 $f(x)/g(x)$的方法。如果一个二项式不能写作两个线性因子之积，则其不可约（`irreducible`），也就是说多项式没有实数解。

1. 令 $x-r$ 是 $g(x)$ 的一个线性因子，其最高幂次是 $(x-r)^m$，那么对于这个因子，可以写作 $m$ 个部分分式之和
$$\frac{A_1}{(x-r)}+\frac{A_2}{(x-r)^2}+\cdots+\frac{A_m}{(x-r)^m}$$
2. 令 $x^2+px+q$ 是 $g(x)$ 不可约的二次项因子，即 $x^2+px+q$ 没有实数根。假定其最高次幂是 $(x^2+px+q)^n$，那么对于这个因子可以分解成 $n$ 个部分分式之和
$$\frac{B_1x+C_1}{(x^2+px+q)}+\frac{B_2x+C_2}{(x^2+px+q)^2}+\cdots+\frac{B_nx+C_n}{(x^2+px+q)^n}$$
3. 将原始函数 $f(x)/g(x)$ 写成这些分数之和的形式。两边同时乘以原始的分母，然后按照 $x$ 的降幂顺序重新排列。
4. 使 $x$ 幂次相同的系数相等，求解待定系数。

例1 求
$$\int\frac{x^2+4x+1}{(x-1)(x+1)(x+3)}dx$$
解：每个因子的幂次都是 1，那么分解的形式如下
$$\frac{x^2+4x+1}{(x-1)(x+1)(x+3)}=\frac{A}{x-1}+\frac{B}{x+1}+\frac{C}{x+3}$$
上面的方程可以写作
$$\begin{aligned}
x^2+4x+1&=A(x+1)(x+3)+B(x-1)(x+3)+C(x-1)(x+1)\\
&=A(x^2+4x+3)+B(x^2+2x-3)+C(x^2-1)\\
&=(A+B+C)x^2+(4A+2B)x+(3A-3B-C)
\end{aligned}$$
对应系数相等
$$\begin{aligned}
A&+B&+C&=1\\
4A&+2B&&=4\\
3A&-3B&-C&=1
\end{aligned}$$
解这个三元一次方程组得到
$$A=\frac{3}{4},B=\frac{1}{2},C=-\frac{1}{4}$$
那么
$$\begin{aligned}
\int\frac{x^2+4x+1}{(x-1)(x+1)(x+3)}dx&=\int[\frac{3}{4}\frac{1}{x-1}+\frac{1}{2}\frac{1}{x+1}-\frac{1}{4}\frac{1}{x+3}]dx\\
&=\frac{3}{4}\ln|x-1|+\frac{1}{2}\ln|x+1|-\frac{1}{4}\ln|x+3|+K
\end{aligned}$$
这里使用 $K$ 表示任意常数的原因是避免和待定系数 $C$ 混淆。

例2 求
$$\int\frac{6x+7}{(x+2)^2}dx$$
解：首先改写为包含待定系数的部分分式之和的形式
$$\frac{6x+7}{(x+2)^2}=\frac{A}{(x+2)}+\frac{B}{(x+2)^2}$$
两边同乘 $(x+2)^2$
$$6x+7=A(x+2)+B=Ax+2A+B$$
所以
$$A=6,B=-5$$
那么
$$\begin{aligned}
\int\frac{6x+7}{(x+2)^2}dx&=\int(\frac{6}{x+2}-\frac{5}{(x+2)^2})dx\\
&=6\int\frac{1}{x+2}-5\int(x+2)^{-2}dx\\
&=6\ln|x+2|+5\frac{1}{x+2}+C
\end{aligned}$$

下面的例子说明如何处理 $f(x)/g(x)$ 是假分数的情况，即 $f$ 的最高次幂比 $g$ 大。

例3 求
$$\int\frac{2x^3-4x^2-x-3}{x^2-2x-3}dx$$
解：首先分子除以分母得到一个多项式和一个真分数
$$2x^3-4x^2-x-3=2x(x^2-2x-3)+(5x-3)$$
所以
$$\frac{2x^3-4x^2-x-3}{x^2-2x-3}=2x+\frac{5x-3}{x^2-2x-3}$$
对于最后一项真分数使用部分分式法即可，因此得到
$$\begin{aligned}
\int\frac{2x^3-4x^2-x-3}{x^2-2x-3}dx&=\int 2xdx+\int\frac{5x-3}{x^2-2x-3}dx\\
&=\int 2xdx+\int\frac{2}{x+1}dx+\int\frac{3}{x-3}dx\\
&=x^2+2\ln|x+1|+3\ln|x-3|+C
\end{aligned}$$

例4 求
$$\int\frac{-2x+4}{(x^2+1)(x-1)^2}dx$$
解：分母有一个不可约的二次多项式 $x^2+1$ 和重复的线性因子 $(x-1)^2$，所以部分分式是
$$\frac{-2x+4}{(x^2+1)(x-1)^2}=\frac{Ax+B}{x^2+1}+\frac{C}{x-1}+\frac{D}{(x-1)^2}$$
消除分母得到
$$\begin{aligned}
-2x+4&=(Ax+B)(x-1)^2+C(x^2+1)(x-1)+D(x^2+1)\\
&=(A+C)x^3+(-2A+B-C+D)x^2+(A-2B+C)x+(B-C+D)\\
\end{aligned}$$
对应系数相等
$$\begin{aligned}
0&=A+C\\
0&=-2A+B-C+D\\
-2&=A-2B+C\\
4&=B-C+D
\end{aligned}$$
这个四元一次方程组比较容易解。

第四个方程减第二个方程得到
$$4=2A,A=2$$
代入第一个方程得到
$$C=-2$$
第一个方程减第三个方程得到
$$2=2B,B=1$$
用第四个方程得到
$$D=1$$
所以
$$\frac{-2x+4}{(x^2+1)(x-1)^2}=\frac{2x+1}{x^2+1}-\frac{2}{x-1}+\frac{1}{(x-1)^2}$$
最后求积分
$$\begin{aligned}
\int\frac{-2x+4}{(x^2+1)(x-1)^2}dx&=\int(\frac{2x+1}{x^2+1}-\frac{2}{x-1}+\frac{1}{(x-1)^2})dx\\
&=\int(\frac{2x}{x^2+1}+\frac{1}{x^2+1}-\frac{2}{x-1}+\frac{1}{(x-1)^2})dx\\
&=\ln(x^2+1)+\tan^{-1}x-2\ln|x-1|-\frac{1}{x-1}+K
\end{aligned}$$

例5 求
$$\int\frac{dx}{x(x^2+1)^2}$$
解：被积分式子分解成
$$\frac{1}{x(x^2+1)^2}=\frac{A}{x}+\frac{Bx+C}{x^2+1}+\frac{Dx+E}{(x^2+1)^2}$$
两边同乘 $x(x^2+1)^2$
$$\begin{aligned}
1&=A(x^2+1)^2+(Bx+C)x(x^2+1)+(Dx+E)x\\
&=A(x^4+2x^2+1)+B(x^4+x^2)+C(x^3+x)+Dx^2+Ex\\
&=(A+B)x^4+Cx^3+(2A+B+D)x^2+(C+E)x+A
\end{aligned}$$
对应系数相等
$$A+B=0,C=0,2A+B+D=0,C+E=0,A=1$$
所以
$$A=1,B=-1,C=0,D=-1,E=0$$
因此
$$\begin{aligned}
\int\frac{dx}{x(x^2+1)^2}&=\int\bigg[\frac{1}{x}-\frac{x}{x^2+1}-\frac{x}{(x^2+1)^2}\bigg]dx\\
&=\int\frac{dx}{x}-\int\frac{xdx}{x^2+1}-\int\frac{xdx}{(x^2+1)^2}\\
&=\int\frac{dx}{x}-\frac{1}{2}\int\frac{du}{u}-\frac{1}{2}\int\frac{du}{u^2}\\
&=\ln|x|-\frac{1}{2}\ln|u|+\frac{1}{2u}+K\\
&=\ln|x|-\frac{1}{2}\ln(x^2+1)+\frac{1}{2(x^2+1)}+K\\
&=\ln\frac{|x|}{x^2+1}+\frac{1}{2(x^2+1)}+K
\end{aligned}$$

当 $f(x)$ 的最高幂次比 $g(x)$ 低且 $g(x)$ 只有幂次为 1 的线性因子
$$g(x)=(x-r_1)(x-r_2)\cdots(x-r_n)$$
有快捷的方式将 $f(x)/g(x)$ 写作部分分式。

例6 求下面方程的待定系数 $A,B,C$
$$\frac{x^2+1}{(x-1)(x-2)(x-3)}=\frac{A}{x-1}+\frac{B}{x-2}+\frac{C}{x-3}$$
解：两边同时乘以 $x-1$
$$\frac{x^2+1}{(x-2)(x-3)}=A+\frac{B(x-1)}{x-2}+\frac{C(x-1)}{x-3}$$
代入 $x=1$ 得到
$$\frac{1^2+1}{(-1)(-2)}=A$$
这就得到了 $A=1$。同样的，两边同乘 $(x-2)$ 并代入 $x=2$ 可以得到
$$\frac{2^2+1}{(1)(-1)}=B$$
所以 $B=-5$。最后两边同乘 $(x-3)$ 并代入 $x=3$ 得到
$$\frac{3^2+1}{(2)(1)}=C$$
所以 $C=5$。

### 求解待定系数的其他方法
使用微分，同时也可以使用代入特定的 $x$ 值。

例7 求下面方程的待定系数 $A,B,C$
$$\frac{x-1}{(x+1)^3}=\frac{A}{x+1}+\frac{B}{(x+1)^2}+\frac{C}{(x+1)^3}$$
解：消除分母得到
$$x-1=A(x+1)^2+B(x+1)+C$$
将 $x=-1$ 代入得到 $C=-2$。两边微分得到
$$1=2A(x+1)+B$$
再次将 $x=-1$ 代入得到 $B=1$。再次微分得到
$$0=2A$$
所以 $A=0$。最后
$$\frac{x-1}{(x+1)^3}=\frac{1}{(x+1)^2}-\frac{2}{(x+2)^3}$$

对于一些问题，可以尝试代入 $x=0,\pm 1\pm 2\pm$ 等值可以快速得到待定系数 $A,B,C$ 的值。

例8 求下面方程的待定系数 $A,B,C$
$$\frac{x^2+1}{(x-1)(x-2)(x-3)}=\frac{A}{x-1}+\frac{B}{x-2}+\frac{C}{x-3}$$
解：消除分母得到
$$x^2+1=A(x-2)(x-3)+B(x-1)(x-3)+C(x-1)(x-2)$$
将 $x=1,2,3$ 代入可以快速得到 $A,B,C$ 的值。
$$\begin{aligned}
x=1:&&1^2+1&=A(-1)(-2)\\
&&A&=1\\
x=2:&&2^2+1&=B(1)(-1)\\
&&B&=-5\\
x=3:&&3^2+1&=C(2)(1)\\
&&C&=5
\end{aligned}$$
所以
$$\frac{x^2+1}{(x-1)(x-2)(x-3)}=\frac{1}{x-1}-\frac{5}{x-2}+\frac{5}{x-3}$$
