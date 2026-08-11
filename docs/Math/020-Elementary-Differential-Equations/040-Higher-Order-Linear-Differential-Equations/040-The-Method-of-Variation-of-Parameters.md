确定非齐次 $n$ 阶线性微分方程
$$L[y]=a_0y^{(n)}+p_1(t)y^{(n-1)}+\cdots+p_{n-1}(t)y'+p_n(t)y=g(t)\tag{1}$$
的参数变分法是二阶微分方程的参数变分法的扩展。和之前一样，首先要求解对应的齐次方程的解。一般来说，除非系数是常数，否则比较困难。不过，参数变分法仍旧比系数待定法要更通用，因为无需假设 $g$ 必须是某种形式，只要其是连续函数即可。

假定我们已经知道了对应的齐次方程的基础解系是 $y_1,y_2,\cdots,y_n$，那么齐次方程的通解是
$$y_c(t)=c_1y_1(t)+c_2y_2(t)+\cdots+c_ny_n(t)\tag{2}$$
为了确定方程 $(1)$ 的特解，参数变分法要确定 $n$ 个函数 $u_1,u_2,\cdots,u_n$ 使得特解 $Y(t)$ 的形式是
$$Y(t)=u_1(t)y_1(t)+u_2(t)y_2(t)+\cdots+u_n(t)y_n(t)\tag{3}$$
由于需要确定 $n$ 个函数，那么需要 $n$ 个条件。$Y$ 要满足 $(1)$ 是一个条件。其余 $n-1$ 个条件的选择要使得计算简单。如果要求解 $u_1,u_2,\cdots,u_n$ 的高阶微分方程的话，肯定不简单。因此其余条件就是要避免出现 $u_1,u_2,\cdots,u_n$ 的高阶微分项。从 $(3)$ 有
$$Y'=(u_1y_1'+u_2y_2'+\cdots+u_ny_n')+(u_1'y_1+u_2'y_2+\cdots+u_n'y_n)\tag{4}$$
那么第一个额外条件是
$$u_1'y_1+u_2'y_2+\cdots+u_n'y_n=0\tag{5}$$
那么 $Y'$ 的公式 $(4)$ 就简化为
$$Y'=u_1y_1'+u_2y_2'+\cdots+u_ny_n'\tag{6}$$
继续这个过程得到 $Y'',\cdots,Y^{(n-1)}$。每一次都令包含 $u_1,u_2,\cdots,u_n$ 的项之和为零，那么得到 $n-2$ 个与 $(5)$ 类似的微分方程
$$u_1'y_1^{(m)}+u_2'y_2^{(m)}+\cdots+u_n'y_n^{(m)}=0,m=1,2,\cdots,n-2\tag{7}$$
那么 $Y'',\cdots,Y^{(n-1)}$ 可以化简为
$$Y^{(m)}=u_1y_1^{(m)}+u_2y_2^{(m)}+\cdots+u_ny_n^{(m)},m=2,3,\cdots,n-1\tag{8}$$
最后一个条件是 $Y$ 必须是 $(1)$ 的解。从 $(8)$ 可以得到
$$Y^{(n)}=(u_1y_1^{(n)}+u_2y_2^{(n)}+\cdots+u_ny_n^{(n)})+(u_1'y_1^{(n-1)}+u_2'y_2^{(n-1)}+\cdots+u_n'y_n^{(n-1)})$$
将 $Y$ 的各阶微分 $(3),(6),(8),(9)$ 代入 $(1)$。因为 $y_1,y_2,\cdots,y_n$ 是齐次方程的解，因此大部分项可以抵消（$n$ 项之和为零）。余下
$$u_1'y_1^{(n-1)}+u_2'y_2^{(n-1)}+\cdots+u_n'y_n^{(n-1)}=g\tag{10}$$
方程 $(5),(10)$ 和 $n-2$ 个方程 $(7)$ 组成了 $n$ 个 $u_1',u_2',\cdots,u_n'$ 的线性方程
$$\begin{aligned}
y_1u_1'+y_2u_2'+\cdots+y_nu_n'&=0\\
y_1'u_1'+y_2'u_2'+\cdots+y_n'u_n'&=0\\
y_1''u_1'+y_2''u_2'+\cdots+y_n''u_n'&=0\\
&\vdots\\
y_1^{(n-1)}u_1'+y_2^{(n-1)}u_2'+\cdots+y_n^{(n-1)}u_n'&=0\\
\end{aligned}\tag{11}$$
求解这个方程组得到 $u_1',u_2',\cdots,u_n'$，积分得到 $u_1,u_2,\cdots,u_n$。方程 $(11)$ 解存在的充分条件是系数行列式对任意 $t$ 都不为零。恰好，行列式是朗斯基 $W[y_1,y_2,\cdots,y_n]$，由于 $y_1,y_2,\cdots,y_n$ 是齐次方程的基础解系，因此系数行列式不为零。这样就能唯一确定 $u_1',u_2',\cdots,u_n'$。根据 Cramer 法则，方程 $(11)$ 的解是
$$u_m'(t)=\frac{g(t)W_m(t)}{W(t)},m=1,2,\cdots,n\tag{12}$$
其中，$W_m$ 是将 $W$ 的第 $m$ 列换成 $(0,0,\cdots,0,1)^T$ 得到的行列式。那么得到了 $(1)$ 的特解
$$Y(t)=\sum_{m=1}^ny_m(t)\int_{t_0}^t\frac{g(s)W_m(s)}{W(s)}ds\tag{13}$$
其中 $t_0$ 是任意值。

例 1，已知 $y_1(t)=e^t,y_2(t)=te^t,y_3(t)=e^{-t}$ 是
$$y'''-y''-y'+y=g(t)\tag{14}$$
对应的齐次微分方程的解。用积分形式表达 $(14)$ 的特解。

解：为了使用公式 $(13)$，需要计算一系列行列式的值。首先
$$\begin{aligned}
W(t)&=W[e^t,te^t,t^2e^t](t)\\
&=\begin{vmatrix}
e^t&&te^t&&e^{-t}\\
e^t&&(t+1)e^t&&-e^{-t}\\
e^t&&(t+2)e^t&&e^{-t}\\
\end{vmatrix}\\
&=e^t\begin{vmatrix}
1&&t&&1\\
1&&t+1&&-1\\
1&&t+2&&1\\
\end{vmatrix}\\
&=e^t\begin{vmatrix}
1&&t&&1\\
0&&1&&-2\\
0&&2&&0\\
\end{vmatrix}\\
&=4e^t
\end{aligned}$$
接下来
$$\begin{aligned}
W_1(t)&=\begin{vmatrix}
0&&te^t&&e^{-t}\\
0&&(t+1)e^t&&-e^{-t}\\
1&&(t+2)e^t&&e^{-t}\\
\end{vmatrix}\\
&=\begin{vmatrix}
te^t&&e^{-t}\\
(t+1)e^t&&-e^{-t}
\end{vmatrix}\\
&=-2t-1
\end{aligned}$$
类似的
$$\begin{aligned}
W_2(t)&=\begin{vmatrix}
e^t&&0&&e^{-t}\\
e^t&&0&&-e^{-t}\\
e^t&&1&&e^{-t}\\
\end{vmatrix}\\
&=-\begin{vmatrix}
e^t&&e^{-t}\\
e^t&&-e^{-t}\\
\end{vmatrix}\\
&=2
\end{aligned}$$
$$\begin{aligned}
W_3(t)&=\begin{vmatrix}
e^t&&te^t&&0\\
e^t&&(t+1)e^t&&0\\
e^t&&(t+2)e^t&&1\\
\end{vmatrix}\\
&=\begin{vmatrix}
e^t&&te^t\\
e^t&&(t+1)e^t
\end{vmatrix}\\
&=e^{2t}
\end{aligned}$$
讲这些代入 $(13)$ 得到
$$\begin{aligned}
Y(t)&=e^t\int_{t_0}^t\frac{g(s)(-1-2s)}{4e^s}ds+te^t\int_{t_0}^t\frac{g(s)(2)}{4e^s}ds+e^{-t}\int_{t_0}^t\frac{g(s)(e^{2s})}{4e^s}ds\\
&=\frac{1}{4}\int_{t_0}^t(e^{t-s}(-1+2(t-s))+e^{-(t-s)})g(s)ds
\end{aligned}\tag{15}$$
依赖于 $g(t)$，$(15)$ 可能可以也可能不可以用初等函数表示。

上述过程非常直接，但是随着 $n$ 的增加，计算过程会越来越复杂。某种程度上，使用 Abel 公式
$$W(t)=W[y_1,y_2,\cdots,y_n](t)=c\exp\bigg(-\int p_1(t)dt\bigg)$$
使用区间上任意一个点来确定 $c$。
