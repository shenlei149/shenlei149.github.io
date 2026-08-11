$n$ 个一阶线性方程
$$\begin{aligned}
x_1'&=p_{11}(t)x_1+\cdots+p_{1n}(t)x_n+g_1(t)\\
&\vdots\\
x_n'&=p_{n1}(t)x_1+\cdots+p_{nn}(t)x_n+g_n(t)\\
\end{aligned}\tag{1}$$
组成的方程组的基本理论与单个 $n$ 阶线性方程类似。因此本节讨论的思路与 3.2 和 4.1 小节类似。为了高效的讨论，我们一般将上式写成矩阵的形式。$x_1=x_1(t),\cdots,x_n=x_n(t)$ 是向量 $\boldsymbol{x}=\boldsymbol{x}(t)$ 的各个分量，$g_1(t),\cdots,g_n(t)$ 是矢量 $\boldsymbol{g}(t)$ 的各个分量，$p_{11}(t),\cdots,p_{nn}(t)$ 是 $n\times n$ 的矩阵 $\boldsymbol{P}(t)$ 的元素。方程 $(1)$ 可以写成矩阵的形式
$$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}+\boldsymbol{g}(t)\tag{2}$$
使用矩阵和向量表示不仅节省空间和方便计算，还能强调微分方程组和单个（标量）微分方程的相似性。

如果 $\boldsymbol{x}=\boldsymbol{x}(t)$ 的各个分量满足方程 $(1)$，那么 $\boldsymbol{x}$ 是方程 $(2)$ 的解。本节中假定 $\boldsymbol{P},\boldsymbol{g}$ 在区间 $\alpha<t\beta$ 上是连续的，也就是说每个分量 $g_1,\cdots,g_n,p_{11},\cdots,p_{nn}$ 是连续的。根据定理 7.1.2，这些是方程 $(2)$ 在 $\alpha<t<\beta$ 上解存在的充分条件。

我们先令方程 $(2)$ 中 $\boldsymbol{g}(t)=0$，得到齐次方程
$$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}\tag{3}$$
和之前讨论单个微分方程类似，一旦求解了齐次方程，有若干种方法求解非齐次方程 $(2)$，这会在 7.9 小节讨论。

我们使用
$$\boldsymbol{x}^{(1)}(t)=\begin{pmatrix}
x_{11}(t)\\x_{21}(t)\\\vdots\\x_{n1}(t)
\end{pmatrix},\cdots,\boldsymbol{x}^{(k)}=\begin{pmatrix}
x_{1k}(t)\\x_{2k}(t)\\\vdots\\x_{nk}(t)
\end{pmatrix}\tag{4}$$
表示 $(3)$ 的解，$x_{ij}(t)=x_i^{(j)}(t)$ 是第 $j$ 个解 $\boldsymbol{x}^{(j)}(t)$ 的第 $i$ 个分量。下面讨论方程组 $(3)$ 的解的结构，这和 3.2、4.1 小节的讨论类似。

**定理 7.4.1 叠加原理（`Principle of Superposition`）**
> 如果 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)}$ 是方程组 $(3)$ 的解，那么线性组合 $c_1\boldsymbol{x}^{(1)}+c_2\boldsymbol{x}^{(2)}$ 也是解，其中 $c_1,c_2$ 是任意常量。

叠加原理的证明很简单，只需要对 $c_1\boldsymbol{x}^{(1)}+c_2\boldsymbol{x}^{(2)}$ 微分即可。

很容易验证
$$\boldsymbol{x}^{(1)}(t)=\begin{pmatrix}
e^{3t}\\2e^{3t}
\end{pmatrix}=\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t},\boldsymbol{x}^{(2)}=\begin{pmatrix}
e^{-t}\\-2e^{-t}
\end{pmatrix}=\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}\tag{5}$$
是方程
$$\boldsymbol{x}'=\begin{pmatrix}
1&1\\4&1
\end{pmatrix}\boldsymbol{x}\tag{6}$$
根据定理 7.4.1，
$$\begin{aligned}
\boldsymbol{x}&=c_1\boldsymbol{x}^{(1)}(t)+c_2\boldsymbol{x}^{(2)}(t)\\
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

反复应用定理 7.4.1，可以得到如果 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(k)}$ 是 $(3)$ 的解，那么
$$\boldsymbol{x}=c_1\boldsymbol{x}^{(1)}(t)+\cdots+c_k\boldsymbol{x}^{(k)}(t)\tag{8}$$
也是 $(3)$ 的解，其中 $c_1,\cdots,c_k$ 是任意常量。因此 $(3)$ 的解的任意有限的线性组合也是解。随着而来的问题是方程 $(3)$ 的解是否都能用这种方法找到。根据之前单个微分方程的情况类推，我们有理由预期：对于由 $n$ 个一阶微分方程组成的方程组 $(3)$，只需选取 $n$ 个适当的解，然后线性组合即可。令 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是 $(3)$ 的解，矩阵 $\boldsymbol{X}(t)$ 的列是这些向量
$$\boldsymbol{X}(t)=\begin{pmatrix}
x_{11}(t)&\cdots&x_{1n}(t)\\
\vdots&&\vdots\\
x_{n1}(t)&\cdots&x_{nn}(t)\\
\end{pmatrix}\tag{9}$$
对于给定值 $t$，$\boldsymbol{X}(t)$ 的列是线性无关的等价于 $\det\boldsymbol{X}\neq 0$。这个行列式是 $n$ 个解 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 的朗斯基行列式，表示为 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$，那么
$$W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}](t)=\det\boldsymbol{X}(t)\tag{10}$$
$\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 在某点处线性无关当且仅当 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$ 在该点处不为零。

**定理 7.4.2**
> 如果矢量函数 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 在区间 $\alpha<t<\beta$ 上的每一点处都是 $(3)$ 线性无关的解，那么 $(3)$ 的每一个解 $\boldsymbol{x}=\boldsymbol{x}(t)$ 都能且只有一种方式表示为 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 的线性组合
> $$\boldsymbol{x}(t)=c_1\boldsymbol{x}^{(1)}+\cdots+c_n\boldsymbol{x}^{(n)}\tag{11}$$

定理 7.4.1 是说形如 $(11)$ 的所有解都是 $(3)$ 的解，7.4.2 是说 $(3)$ 的所有解都能表示成 $(11)$ 的解。如果 $c_1,\cdots,c_n$ 是任意常量，$(11)$ 包含了 $(3)$ 的所有解，那么称之为通解。$(3)$ 的任意一个在区间 $\alpha<t<\beta$ 上任一点都线性无关的解集 $\{\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}\}$ 称之为基础解系。

为了证明 7.4.2，需要证明 $(3)$ 的任意解都能写成 $\boldsymbol{x}(t)=c_1\boldsymbol{x}^{(1)}+\cdots+c_n\boldsymbol{x}^{(n)}$ 的形式。令 $t=t_0$ 是区间 $\alpha<t<\beta$ 上的一点，并且令 $\boldsymbol{y}=\boldsymbol{x}(t_0)$。现在需要确定是否存在一个形如 $\boldsymbol{x}(t)=c_1\boldsymbol{x}^{(1)}+\cdots,+c_n\boldsymbol{x}^{(n)}$ 的解还满足初始条件 $\boldsymbol{x}(t_0)=\boldsymbol{y}$。也就是要确定 $c_1,\cdots,c_n$ 使得
$$c_1\boldsymbol{x}^{(1)}(t_0)+\cdots+c_n\boldsymbol{x}^{(n)}(t_0)=\boldsymbol{y}\tag{12}$$
标量形式是
$$\begin{aligned}
c_1x_{11}(t_0)+\cdots+c_nx_{1n}(t_0)&=y_1\\
&\vdots\\
c_1x_{n1}(t_0)+\cdots+c_nx_{nn}(t_0)&=y_n\\
\end{aligned}\tag{13}$$
方程组 $(13)$ 有唯一解 $c_1,\cdots,c_n$ 的充分必要条件是系数行列式不为零，也就是 $t=t_0$ 时朗斯基 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$ 不为零。假定 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 在 $\alpha<t<\beta$ 上是线性无关的，那么 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$ 在 $t=t_0$ 时不为零，因此 $(3)$ 有唯一解，形式如 $\boldsymbol{x}(t)=c_1\boldsymbol{x}^{(1)}+\cdots+c_n\boldsymbol{x}^{(n)}$ 且满足条件 $(12)$。根据定理 7.1.2，这个解就是 $\boldsymbol{x}$。

**定理 7.4.3 阿贝尔定理（`Abel's Theorem`）**
> 如果 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是区间 $\alpha<t<\beta$ 上 $(3)$ 的解，那么在这个区间上朗斯基 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$ 要么始终为零，要么总不是零。

定理 7.4.3 让我们无需检查 $W[\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}]$ 在所有点来判断 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是否是基础解系，只要找任意一点即可。

这里不详细展开，但可以证明 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 的朗斯基满足
$$\frac{dW}{dt}=(p_{11}(t)+p_{22}(t)+\cdots+p_{nn}(t))W\tag{14}$$
其中
$$W(t)=c\exp\bigg(\int[p_{11}(t)+p_{22}(t)+\cdots+p_{nn}(t)]dt\bigg)\tag{15}$$
其中 $c$ 是任意常数。根据上式，朗斯基要么总是零要么不会为零。$(15)$ 称为阿贝尔公式（`Abel's formula`），和定理 3.2.7 类似。

另一个证明方法是方程 $(3)$ $n$ 个解 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 在 $t=t_0$ 处线性相关，那么在区间 $\alpha<t<\beta$ 上都是线性相关的。那么 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 在一点处线性无关，那么处处都线性无关。

下一个定理是说 $(3)$ 至少有一个基础解系。

**定理 7.4.4**
> 令
> $$\boldsymbol{e}^{(1)}=\begin{pmatrix}
1\\0\\0\\\vdots\\0
\end{pmatrix},\boldsymbol{e}^{(2)}=\begin{pmatrix}
0\\1\\0\\\vdots\\0
\end{pmatrix},\cdots,\boldsymbol{e}^{(n)}=\begin{pmatrix}
0\\0\\\vdots\\0\\1
\end{pmatrix}$$
> 并且 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是 $(3)$ 的解并且满足初始条件
> $$\boldsymbol{x}^{(1)}(t_0)=\boldsymbol{e}^{(1)},\cdots,\boldsymbol{x}^{(n)}(t_0)=\boldsymbol{e}^{(n)}\tag{16}$$
> 其中 $t_0$ 是 $\alpha<t<\beta$ 内任一点。那么 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是 $(3)$ 的基础解系。

$(3)$ 的解 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 的存在性和唯一性由定理 7.1.2 保证。容易看出 $t=t_0$ 时这些解的朗斯基等于 1，因此 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是基础解系。

一旦基础解系确定了，可以通过线性组合来生成其他基础解析。出于理论目的，定理 7.4.4 给出的基础解系通常是最简单的。

与二阶线性方程类似，对于系数都是实数的方程组，可能会得到复数解。这种情况下，可以使用下面的定理（类似于定理 3.2.6）来找到实数解。

**定理 7.4.5**
> 方程 $(3)$
> $$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}$$
> 其中 $\boldsymbol{P}$ 是每个元素都是实函数。如果 $(3)$ 的解是复数 $\boldsymbol{x}=\boldsymbol{u}(t)+i\boldsymbol{v}(t)$，那么实部 $\boldsymbol{u}(t)$ 和虚部 $\boldsymbol{v}(t)$ 也是这个方程的解。

将 $\boldsymbol{u}(t)+i\boldsymbol{v}(t)$ 代入 $(3)$ 那么
$$\boldsymbol{x}'-\boldsymbol{P}(t)\boldsymbol{x}=\boldsymbol{u}'(t)-\boldsymbol{P}(t)\boldsymbol{u}(t)+i(\boldsymbol{v}'(t)-\boldsymbol{P}(t)\boldsymbol{v}(t))=\boldsymbol{0}\tag{17}$$
使用 $\boldsymbol{P}$ 是实数这一条件，将 $(17)$ 分成实部和虚部两个部分。由于复数是零等价于实部和虚部都为零，那么 $\boldsymbol{u}'(t)-\boldsymbol{P}(t)\boldsymbol{u}(t)=\boldsymbol{0},\boldsymbol{v}'(t)-\boldsymbol{P}(t)\boldsymbol{v}(t)=\boldsymbol{0}$，因此 $\boldsymbol{u}(t),\boldsymbol{v}(t)$ 也是 $(3)$ 的解。

总结一下这个小节：

1. 方程组 $\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}$ 的任意 $n$ 个线性无关的解组成了基础解系。
2. 在本节给定的条件下，基础解系总是存在。
3. 方程组 $\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}$ 的每个解可以表示成任意基础解系的线性组合。
