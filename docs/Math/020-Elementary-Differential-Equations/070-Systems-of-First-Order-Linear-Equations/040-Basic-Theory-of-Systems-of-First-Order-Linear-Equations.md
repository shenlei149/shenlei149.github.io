$n$ 个一阶线性方程
$$\begin{aligned}
x_1'&=p_{11}(t)x_1+\cdots+p_{1n}(t)x_n+g_1(t)\\
&\vdots\\
x_n'&=p_{n1}(t)x_1+\cdots+p_{nn}(t)x_n+g_n(t)\\
\end{aligned}\tag{1}$$
组成的方程组的基本理论与单个 $n$ 阶线性方程类似。因此本节讨论的思路与 3.2 和 4.1 小节类似。为了高效的讨论，我们一般将上式写成矩阵的形式。$x_1=x_1(t),\cdots,x_n=x_n(t)$ 是向量 $\bold{x}=\bold{x}(t)$ 的各个分量，$g_1(t),\cdots,g_n(t)$ 是矢量 $\bold{g}(t)$ 的各个分量，$p_{11}(t),\cdots,p_{nn}(t)$ 是 $n\times n$ 的矩阵 $\bold{P}(t)$ 的元素。方程 $(1)$ 可以写成矩阵的形式
$$\bold{x}'=\bold{P}(t)\bold{x}+\bold{g}(t)\tag{2}$$
使用矩阵和向量表示不仅节省空间和方便计算，还能强调微分方程组和单个（标量）微分方程的相似性。

如果 $\bold{x}=\bold{x}(t)$ 的各个分量满足方程 $(1)$，那么 $\bold{x}$ 是方程 $(2)$ 的解。本节中假定 $\bold{P},\bold{g}$ 在区间 $\alpha<t\beta$ 上是连续的，也就是说每个分量 $g_1,\cdots,g_n,p_{11},\cdots,p_{nn}$ 是连续的。根据定理 7.1.2，这些是方程 $(2)$ 在 $\alpha<t<\beta$ 上解存在的充分条件。

我们先令方程 $(2)$ 中 $\bold{g}(t)=0$，得到齐次方程
$$\bold{x}'=\bold{P}(t)\bold{x}\tag{3}$$
和之前讨论单个微分方程类似，一旦求解了齐次方程，有若干种方法求解非齐次方程 $(2)$，这会在 7.9 小节讨论。

我们使用
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
x_{11}(t)\\x_{21}(t)\\\vdots\\x_{n1}(t)
\end{pmatrix},\cdots,\bold{x}^{(k)}=\begin{pmatrix}
x_{1k}(t)\\x_{2k}(t)\\\vdots\\x_{nk}(t)
\end{pmatrix}\tag{4}$$
表示 $(3)$ 的解，$x_{ij}(t)=x_i^{(j)}(t)$ 是第 $j$ 个解 $\bold{x}^{(j)}(t)$ 的第 $i$ 个分量。下面讨论方程组 $(3)$ 的解的结构，这和 3.2、4.1 小节的讨论类似。

**定理 7.4.1 叠加原理（`Principle of Superposition`）**
> 如果 $\bold{x}^{(1)},\bold{x}^{(2)}$ 是方程组 $(3)$ 的解，那么线性组合 $c_1\bold{x}^{(1)}+c_2\bold{x}^{(2)}$ 也是解，其中 $c_1,c_2$ 是任意常量。

叠加原理的证明很简单，只需要对 $c_1\bold{x}^{(1)}+c_2\bold{x}^{(2)}$ 微分即可。

很容易验证
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
e^{3t}\\2e^{3t}
\end{pmatrix}=\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t},\bold{x}^{(2)}=\begin{pmatrix}
e^{-t}\\-2e^{-t}
\end{pmatrix}=\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}\tag{5}$$
是方程
$$\bold{x}'=\begin{pmatrix}
1&1\\4&1
\end{pmatrix}\bold{x}\tag{6}$$
根据定理 7.4.1，
$$\begin{aligned}
\bold{x}&=c_1\bold{x}^{(1)}(t)+c_2\bold{x}^{(2)}(t)\\
&=c_1\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t}+c_2\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}\\
&=\begin{pmatrix}
c_1e^{3t}+c_2e^{-t}\\2c_1e^{3t}-2c_2e^{-t}
\end{pmatrix}
\end{aligned}\tag{7}$$
也是方程组的解。

反复应用定理 7.4.1，可以得到如果 $\bold{x}^{(1)},\cdots,\bold{x}^{(k)}$ 是 $(3)$ 的解，那么
$$\bold{x}=c_1\bold{x}^{(1)}(t)+\cdots+c_k\bold{x}^{(k)}(t)\tag{8}$$
也是 $(3)$ 的解，其中 $c_1,\cdots,c_k$ 是任意常量。因此 $(3)$ 的解的任意有限的线性组合也是解。随着而来的问题是方程 $(3)$ 的解是否都能用这种方法找到。根据之前单个微分方程的情况类推，我们有理由预期：对于由 $n$ 个一阶微分方程组成的方程组 $(3)$，只需选取 $n$ 个适当的解，然后线性组合即可。令 $\bold{x}^{(1)},\cdots,\bold{x}^{(n)}$ 是 $(3)$ 的解，矩阵 $\bold{X}(t)$ 的列是这些向量
$$\bold{X}(t)=\begin{pmatrix}
x_{11}(t)&\cdots&x_{1n}(t)\\
\vdots&&\vdots\\
x_{n1}(t)&\cdots&x_{nn}(t)\\
\end{pmatrix}\tag{9}$$
对于给定值 $t$，$\bold{X}(t)$ 的列是线性无关的等价于 $\det\bold{X}\neq 0$。这个行列式是 $n$ 个解 $\bold{x}^{(1)},\cdots,\bold{x}^{(n)}$ 的朗斯基行列式，表示为 $W[\bold{x}^{(1)},\cdots,\bold{x}^{(n)}]$，那么
$$W[\bold{x}^{(1)},\cdots,\bold{x}^{(n)}](t)=\det\bold{X}(t)\tag{10}$$
$\bold{x}^{(1)},\cdots,\bold{x}^{(n)}$ 在某点处线性无关当且仅当 $W[\bold{x}^{(1)},\cdots,\bold{x}^{(n)}]$ 在该点处不为零。

**定理 7.4.2**
> 如果矢量函数 $\bold{x}^{(1)},\cdots,\bold{x}^{(n)}$ 在区间 $\alpha<t<\beta$ 上的每一点处都是 $(3)$ 线性无关的解，那么 $(3)$ 的每一个解 $\bold{x}=\bold{x}(t)$ 都能且只有一种方式表示为 $\bold{x}^{(1)},\cdots,\bold{x}^{(n)}$ 的线性组合
> $$\bold{x}(t)=c_1\bold{x}^{(1)}+\cdots,+c_n\bold{x}^{(n)}$$

定理 7.4.1 是说形如 $(11)$ 的所有解都是 $(3)$ 的解，7.4.2 是说 $(3)$ 的所有解都能表示成 $(11)$ 的解。如果 $c_1,\cdots,c_n$ 是任意常量，$(11)$ 包含了 $(3)$ 的所有解，那么称之为通解。$(3)$ 的任意一个在区间 $\alpha<t<\beta$ 上任一点都线性无关的解集 $\{\bold{x}^{(1)},\cdots,\bold{x}^{(n)}\}$ 称之为基础解系。
