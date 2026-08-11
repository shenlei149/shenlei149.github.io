之前讨论的处理单个一阶微分方程的初值问题的数值方法，也适用于一些微分方程组。由于高阶微分方程总是可以写成一阶微分方程组，因此这里讨论一阶微分方程组就足够了。简单起见，我们讨论两个微分方程组成的方程组
$$x'=f(t,x,y),y'=g(t,x,y)\tag{1}$$
初始条件是
$$x(t_0)=x_0,y(t_0)=y_0\tag{2}$$
函数 $f,g$ 满足定理 7.1.1 的条件，初值问题 $(1)$ 在 $t$ 的某个区间上有唯一解。这里要求 $t_n=t_0+nh,n=1,2,\cdots$ 时 解 $x=\phi(t),y=\psi(t)$ 的数值近似 $x_1,x_2,\cdots,x_n$ 和 $y_1,y_2,\cdots,y_n$。

使用向量表示，初值问题 $(1)$ 可以写成
$$\boldsymbol{x}'=\boldsymbol{f}(t,\boldsymbol{x}),\boldsymbol{x}(t_0)=\boldsymbol{x}_0\tag{3}$$
其中 $\boldsymbol{x}=(x,y),\boldsymbol{f}=(f,g),\boldsymbol{x}_0=(x_0,y_0)$。向量表示的一个好处是可以忽略方程的个数，初值问题总是相同的形式。所有的变化仅仅是 $\boldsymbol{x},\boldsymbol{f}$ 和 $\boldsymbol{x}_0$ 的分量个数。

前面小节讨论的问题可以快速泛化应用于两个或多个方程组成的方程组。唯一需要的事情是用向量 $\boldsymbol{x}$ 替换 $y$，用向量 $\boldsymbol{f}$ 替换 $f$。比如欧拉方法的公式变成
$$\boldsymbol{x}_{n+1}=\boldsymbol{x}_n+h\boldsymbol{f}_n\tag{4}$$
其中 $\boldsymbol{f}_n=\boldsymbol{f}(t_n,\boldsymbol{x}_n)$。其分量形式是
$$\begin{pmatrix}
x_{n+1}\\
y_{n+1}
\end{pmatrix}=\begin{pmatrix}
x_n\\
y_n
\end{pmatrix}+h\begin{pmatrix}f(t_n,x_n,y_n)\\
g(t_n,x_n,y_n)
\end{pmatrix}\tag{5}$$
初始条件用于求解 $\boldsymbol{f}_0=\boldsymbol{f}(t_0,\boldsymbol{x}_0)$。观察到 $\boldsymbol{f}_0=\boldsymbol{\phi}(t_0)$ 意味着 $\boldsymbol{f_0}$ 是解 $\boldsymbol{x}=\boldsymbol{\phi}(t)$ 在 $t=t_0$ 处的切向量。近似解就是沿着这个切向量前进 $h$ 的距离 得到 $\boldsymbol{x}_1$ 的近似值。接着计算 $\boldsymbol{x}_1$ 处的切向量 $\boldsymbol{f}_1$，再沿着这个切向量前进 $h$ 的距离得到 $\boldsymbol{x}_2$ 的近似值。以此类推。

类似的龙格-库塔法也可以扩展到方程组。比如四阶龙格-库塔法的公式从 $t_n$ 到 $t_{n+1}$ 是
$$\boldsymbol{x}_{n+1}=\boldsymbol{x}_n+\frac{h}{6}\left(\boldsymbol{k}_{n1}+2\boldsymbol{k}_{n2}+2\boldsymbol{k}_{n3}+\boldsymbol{k}_{n4}\right)\tag{6}$$
其中
$$\begin{aligned}
\boldsymbol{k}_{n1}&=\boldsymbol{f}(t_n,\boldsymbol{x}_n)\\
\boldsymbol{k}_{n2}&=\boldsymbol{f}(t_n+\frac{h}{2},\boldsymbol{x}_n+\frac{h}{2}\boldsymbol{k}_{n1})\\
\boldsymbol{k}_{n3}&=\boldsymbol{f}(t_n+\frac{h}{2},\boldsymbol{x}_n+\frac{h}{2}\boldsymbol{k}_{n2})\\
\boldsymbol{k}_{n4}&=\boldsymbol{f}(t_n+h,\boldsymbol{x}_n+h\boldsymbol{k}_{n3})
\end{aligned}\tag{7}$$
四阶亚当斯-莫尔顿预估修正法的公式是
$$\boldsymbol{x}_{n+1}=\boldsymbol{x}_n+\frac{h}{24}\left(55\boldsymbol{f}_n-59\boldsymbol{f}_{n-1}+37\boldsymbol{f}_{n-2}-9\boldsymbol{f}_{n-3}\right)$$
和
$$\boldsymbol{x}_{n+1}=\boldsymbol{x}_n+\frac{h}{24}\left(9\boldsymbol{f}_{n+1}+19\boldsymbol{f}_n-5\boldsymbol{f}_{n-1}+\boldsymbol{f}_{n-2}\right)$$
任意一阶微分方程组均可写成式 $(3)$ 给出的形式，方程 $(4)$ 即为欧拉公式，而方程 $(6)$ 和 $(7)$ 则是适用于任意一阶微分方程组的四阶龙格-库塔公式。唯一的区别仅在于向量中分量的数量。

例 1 求解初值问题
$$x'=x-4y,x(0)=1,y'=-x+y,y(0)=0$$
的解 $x=\phi(t),y=\psi(t)$ 在 $t=0.2$ 的近似值。使用 $h=0.1$ 的欧拉方法和 $h=0.2$ 的四阶龙格-库塔方法。使用精确解
$$\phi(t)=\frac{e^{-t}+e^{3t}}{2},\psi(t)=\frac{e^{-4}-e^{3t}}{4}\tag{8}$$
对比结果。

解：首先使用欧拉法。那么 $f_n=x_n-4y_n,g_n=-x_n+y_n$，因此
$$f_0=1-(4)(0)=1,g_0=-1+0=-1$$
使用欧拉公式 $(5)$，得到
$$x_1=1+0.1(1)=1.1,y_1=0+0.1(-1)=-0.1$$
下一步
$$f_1=1.1-(4)(-0.1)=1.5,g_1=-1.1+(-0.1)=-1.2$$
那么
$$x_2=1.1+(0.1)(1.5)=1.25,y_2=-0.1+(0.1)(-1.2)=-0.22$$
精确值是 $\phi(0.2)=1.3204248,\psi(0.2)=-0.25084701$，与欧拉法计算的近似值差值是 0.0704 和 0.0308，百分比是 5.3\% 和 12.3\%。

接下来使用四阶龙格-库塔方法。使用公式 $(7)$，得到
$$\boldsymbol{x}=\begin{pmatrix}
x\\y
\end{pmatrix},\boldsymbol{f}=\begin{pmatrix}
f(x,y)\\g(x,y)
\end{pmatrix}=\begin{pmatrix}
x-4y\\-x+y
\end{pmatrix},\boldsymbol{x}_0=\begin{pmatrix}
1\\0
\end{pmatrix}$$
使用 $h=0.2$，得到
$$\begin{aligned}\boldsymbol{k}_{01}&=f(0,\begin{pmatrix}
1\\0
\end{pmatrix})=\begin{pmatrix}
1\\-1
\end{pmatrix}\\
\boldsymbol{k}_{02}&=f(0.1,\begin{pmatrix}
1.1\\-0.1
\end{pmatrix})=\begin{pmatrix}1.5\\-1.2
\end{pmatrix}\\
\boldsymbol{k}_{03}&=f(0.1,\begin{pmatrix}
1.15\\-0.12
\end{pmatrix})=\begin{pmatrix}1.63\\-1.27
\end{pmatrix}\\
\boldsymbol{k}_{04}&=f(0.2,\begin{pmatrix}
1.326\\-0.254
\end{pmatrix})=\begin{pmatrix}2.342\\-1.580
\end{pmatrix}
\end{aligned}$$
代入公式 $(6)$，得到
$$\boldsymbol{x}_1=\begin{pmatrix}1.3200667\\-0.2506667
\end{pmatrix}$$
$x_1,y_1$ 的近似值与精确值的差值是 0.000358 和 0.000180，百分比小于 1%。

这个例子再次表明，通过使用更精确的近似方法（如龙格-库塔法）可以获得极大的精度提升。上面的计算中，龙格-库塔法所需的函数求值次数仅为欧拉法的两倍，但欧拉法的误差却是龙格-库塔法误差的 200 倍。
