下面讨论常系数线性齐次微分方程组
$$\bold{x}'=\bold{A}\bold{x}\tag{1}$$
中 $\bold{A}$ 有重复特征值的情况。7.3 小节讨论过代数重数 $m\geq 2$ 的重复特征值或许有小于 $m$ 的几何重数。也就是说，这个特征值仅对应着小于 $m$ 个线性无关的特征向量。

例 1 求矩阵
$$\bold{A}=\begin{pmatrix}
1&-1\\1&3
\end{pmatrix}\tag{2}$$
的特征值和特征向量。

解：特征值 $r$ 和特征向量 $\bold{\xi}$ 满足 $(\bold{A}-r\bold{I})\bold{\xi}=\bold{0}$，即
$$\begin{pmatrix}
1-r&-1\\1&3-r
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{3}$$
特征值是
$$\det(\bold{A}-r\bold{I})=\begin{vmatrix}
1-r&-1\\1&3-r
\end{vmatrix}=r^2-4r+4=(r-2)^2=0\tag{4}$$
的根。两个特征值数 $r_1=r_2=2$，因此特征值 2 的代数重数是 2。

为了求特征向量，代入 $r=2$ 到 $(3)$ 中，得到
$$\begin{pmatrix}
-1&-1\\1&1
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{5}$$
那么 $\xi_1+\xi_2=0$，因此 $r=2$ 对应的特征向量是
$$\bold{\xi}^{(1)}=\begin{pmatrix}
1\\1
\end{pmatrix}\tag{6}$$
因此特征值 2 的几何重数是 1。

回到方程组 $(1)$，假设 $r=\rho$ 是特征方程
$$\det(\bold{A}-r\bold{I})=0\tag{7}$$
的 $m$ 重根，那么 $\rho$ 是矩阵 $\bold{A}$ 的代数重数为 $m$ 的特征值。有两种可能性：对于特征值 $\rho$，对应着 $m$ 个线性无关的特征向量，或者对应着少于 $m$ 个线性无关的特征向量。

第一种情况，代数重数 $m$ 的特征值 $\rho$ 有 $m$ 个线性无关的特征向量 $\bold{\xi}^{(1)},\cdots,\bold{\xi}^{(m)}$，那么方程 $(1)$ 有 $m$ 个线性无关的解 $\bold{x}^{(1)}(t)=\bold{\xi}^{(1)}e^{\rho t},\cdots,\bold{x}^{(m)}(t)=\bold{\xi}^{(m)}e^{\rho t}$，这和之前的讨论没有区别，形如 $\bold{\xi}e^{rt}$ 的解依旧构成了基础解系。这种情况发生在 $\bold{A}$ 是埃尔米特矩阵，或者是实数对称矩阵（埃尔米特矩阵的特例）。

不过如果系数矩阵不是埃尔米特矩阵，那么代数重数 $m$ 的特征值 $\rho$ 只有小于 $m$ 个线性无关的特征向量，此时，方程 $(1)$ 有少于 $m$ 个形如 $\bold{\xi}e^{rt}$ 的解。因此为了构造 $(1)$ 的通解，必须要找到其他形式的解。3.4 解求解二阶线性微分方程 $ay''+by'+cy=0$ 时遇到过特征方程有两个重复根 $r$，第一个解的形式是 $y_1(t)=e^{rt}$，第二个解的形式是 $y_2(t)=te^{rt}$。下面的例子使用了类似的想法。

例 2 求
$$\bold{x}'=\bold{A}\bold{x}=\begin{pmatrix}
1&-1\\1&3
\end{pmatrix}\bold{x}\tag{8}$$
的基础解系，并画出相图。

解：方程组 $(8)$ 的方向场如下图所示，所有的非零解都远离原点。

![](080.010.png)

例 1 中我们得到了系数矩阵 $\bold{A}$ 的特征值和特征向量。$r=2$ 是代数重数为 2 的特征值，对应的特征向量是 $\bold{\xi}=(1,-1)^T$。因此 $(8)$ 的第一个解是
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
1\\-1
\end{pmatrix}e^{2t}\tag{9}$$
但是没有第二个形式为 $\bold{x}=\bold{\xi}e^{rt}$ 的解。

基于之前 3.4 小节的思想，假定第二个解的形式是
$$\bold{x}=\bold{\xi}te^{2t}\tag{10}$$
其中 $\bold{\xi}$ 是待定向量。将上式代入 $(8)$ 得到
$$2\bold{\xi}te^{2t}+\bold{\xi}e^{2t}=\bold{A}\bold{\xi}te^{2t}\tag{11}$$
上式对于所有 $t$ 都要成立，那么 $te^{2t},e^{2t}$ 的系数要相等。为了让 $e^{2t}$ 的系数相等，得到
$$\bold{\xi}=\bold{0}\tag{12}$$
因此 $(8)$ 没有形式为 $(10)$ 的非零解。

方程 $(11)$ 出现了 $te^{2t},e^{2t}$，那么除了 $te^{2t}$ 之外，第二个解还需要包含形式为 $\bold{\eta}e^{2t}的项$，因此假设解的形式是
$$\bold{x}=\bold{\xi}te^{2t}+\bold{\eta}e^{2t}\tag{13}$$
其中 $\bold{\xi},\bold{\eta}$ 是待定向量。将上式代入 $(8)$ 得到
$$2\bold{\xi}te^{2t}+(\bold{\xi}+2\bold{\eta})e^{2t}=\bold{A}(\bold{\xi}te^{2t}+\bold{\eta}e^{2t})\tag{14}$$
为了让 $te^{2t},e^{2t}$ 的系数相等，那么
$$2\bold{\xi}=\bold{A}\bold{\xi}$$
$$\bold{\xi}+2\bold{\eta}=\bold{A}\bold{\eta}$$
为了确定 $\bold{\xi},\bold{\eta}$，上述方程写作
$$(\bold{A}-2\bold{I})\bold{\xi}=\bold{0}\tag{15}$$
$$(\bold{A}-2\bold{I})\bold{\eta}=\bold{\xi}\tag{16}$$
方程 $(15)$ 的解就是 $r=2,\bold{\xi}=(1,-1)^T$。由于 $\det(\bold{A}-2\bold{I})=0$，$(16)$ 有非零解要求右边的 $\bold{\xi}$ 满足某种条件。而 $\bold{\xi}$ 及其整数倍恰好是能够使得方程组有解的向量。$(16)$ 的增广矩阵是
$$\begin{pmatrix}
-1&-1&1\\1&1&-1
\end{pmatrix}$$
第二行是第一行的 -1 倍，那么方程组有解
$$-\eta_1-\eta_2=1$$
令 $\eta_1=k$，其中 $k$ 是任意常量，那么 $\eta_2=-k-1$，因此
$$\bold{\eta}=\begin{pmatrix}
k\\-k-1
\end{pmatrix}=\begin{pmatrix}
0\\-1
\end{pmatrix}+k\begin{pmatrix}
1\\-1
\end{pmatrix}\tag{17}$$
将 $\bold{\xi},\bold{\eta}$ 代入 $(13)$ 得到
$$\bold{x}=\bold{\xi}te^{2t}+\bold{\eta}e^{2t}=\begin{pmatrix}
1\\-1
\end{pmatrix}te^{2t}+\begin{pmatrix}
0\\-1
\end{pmatrix}e^{2t}+k\begin{pmatrix}
1\\-1
\end{pmatrix}e^{2t}\tag{18}$$
$(18)$ 的最后一项是第一个解 $\bold{x}^{(1)}(t)$ 的若干倍，因此可以忽略，那么第二个解是
$$\bold{x}^{(2)}(t)=\begin{pmatrix}
1\\-1
\end{pmatrix}te^{2t}+\begin{pmatrix}
0\\-1
\end{pmatrix}e^{2t}\tag{19}$$
朗斯基行列式 $W[\bold{x}^{(1)},\bold{x}^{(2)}](t)=-e^{4t}\neq 0$，因此 $\bold{x}^{(1)},\bold{x}^{(2)}$ 组成了方程组 $(8)$ 的基础解系，因此通解是
$$\begin{aligned}
\bold{x}&=c_1\bold{x}^{(1)}+c_2\bold{x}^{(2)}\\
&=c_1\begin{pmatrix}
1\\-1
\end{pmatrix}e^{2t}+c_2\bigg(\begin{pmatrix}
1\\-1
\end{pmatrix}te^{2t}+\begin{pmatrix}
0\\-1
\end{pmatrix}e^{2t}\bigg)
\end{aligned}\tag{20}$$
解 $(20)$ 的相图的主要特征来自于每一项都包含 $e^{2t}$。随着 $t\to -\infty$，$\bold{x}\to\bold{0}$，除非 $c_1,c_2$ 都是零，否则随着 $t\to\infty$ 时 $\bold{x}$ 趋于无界。如果 $c_1,c_2$ 都不是零，沿着任意轨迹都有
$$\lim_{t\to -\infty}\frac{x_2(t)}{x_1(t)}=\lim_{t\to -\infty}\frac{-c_1e^{2t}+c_2(-te^{2t}-e^{2t})}{c_1e^{2t}+c_2te^{2t}}=\lim_{t\to -\infty}\frac{-c_1-c_2t-c_2}{c_1+c_2t}=-1$$
因此，随着 $t\to -\infty$，每一个轨迹都沿着 $x_2=-x_1$ 趋于原点。如下图所示。随着 $t\to\infty$，轨迹的斜率也趋于 -1。

![](080.021.png)

可以证明，当 $t\to\infty$ 时，轨线并不趋近于任何单一的渐近线。上图给出了一些轨迹，包括 $\bold{x}^{(1)}$ 是黑色实线，$\bold{x}^{(2)}$ 是黑色虚线。$x_1,x_2$ 相对于 $t$ 的变化如下图所示。

![](080.022.png)

上图（a）是典型的有相同特征值的但只对应一个独立特征向量的 $2\times 2$ 方程组 $\bold{x}'=\bold{A}\bold{x}$ 的轨迹的模式。原点称为非正常节点（`improper node`）。如果特征值是负数，那么轨迹类似不过方向向内。非正常节点是渐进稳定的或者是稳定的，这取决于特征值的符号。

从上面的例子可以看出两个一阶方程组成的方程组和一个二阶方程有一个明显不同：对于特征方程有重复根 $r_1$ 的二阶线性方程，$ce^{r_1t}$ 不会出现在第二个解中，因为它是第一个解的若干倍。但是对于一阶线性方程组，$\bold{\eta}e^{r_1t}$ 通常不是 $\bold{\xi}e^{r_1t}$ 的若干倍，因此 $\bold{\eta}e^{r_2t}$ 需要保留。

回到方程组 $(1)$，如果 $r=\rho$ 是 $\bold{A}$ 的二重特征值，并且只有一个特征向量 $\bold{\xi}$ 与之对应，那么第一个解是
$$\bold{x}^{(1)}(t)=\bold{\xi}e^{\rho t}\tag{21}$$
其中 $\bold{\xi}$ 满足
$$(\bold{A}-\rho\bold{I})\bold{\xi}=\bold{0}\tag{22}$$
和例 2 类似，第二个解是
$$\bold{x}^{(2)}=\bold{\xi}te^{\rho t}+\bold{\eta}e^{\rho t}\tag{23}$$
其中 $\bold{\xi}$ 满足 $(22)$，$\bold{\eta}$ 满足
$$(\bold{A}-\rho\bold{I})\bold{\eta}=\bold{\xi}\tag{24}$$
尽管 $\det(\bold{A}-\rho\bold{I})=0$，但总是可以用 $(24)$ 求解出 $\bold{\eta}$。这里不展示证明细节了，一个重要步骤是对 $(24)$ 两边同乘 $\bold{A}-\rho\bold{I}$，那么
$$(\bold{A}-\rho\bold{I})^2\bold{\eta}=\bold{0}$$
向量 $\bold{\eta}$ 被称为矩阵 $\bold{A}$ 对应特征值 $\rho$ 的广义特征向量（`generalized eigenvector`）。

### 基解矩阵
7.7 小节讨论过，基解矩阵由线性无关的解组成。因此，在上个例子中，基解矩阵由 $(8)$ 的解 $\bold{x}^{(1)},\bold{x}^{(2)}$ 组成，那么
$$\bold{\Psi}(t)=\begin{pmatrix}
e^{2t}&te^{2t}\\-e^{2t}&-te^{2t}-e^{2t}
\end{pmatrix}=e^{2t}\begin{pmatrix}
1&t\\-1&-1-t
\end{pmatrix}\tag{25}$$
特殊的基解矩阵 $\bold{\Phi}$ 满足 $\bold{\Phi}(0)=\bold{I}$，通过 $\bold{\Phi}(t)=\bold{\Psi}(t)\bold{\Psi}^{-1}(0)$ 可以计算得到。由 $(25)$ 得到
$$\bold{\Psi}(0)=\begin{pmatrix}
1&0\\-1&-1
\end{pmatrix}$$
那么
$$\bold{\Psi}^{-1}(0)=\begin{pmatrix}
1&0\\-1&-1
\end{pmatrix}\tag{26}$$
那么
$$\begin{aligned}
\bold{\Phi}(t)&=\bold{\Psi}(t)\bold{\Psi}^{-1}(0)\\
&=e^{2t}\begin{pmatrix}
1&t\\-1&-1-t
\end{pmatrix}\begin{pmatrix}
1&0\\-1&-1
\end{pmatrix}\\
&=e^{2t}\begin{pmatrix}
1-t&-t\\t&1+t
\end{pmatrix}
\end{aligned}\tag{27}$$
之前讨论过，满足 $\bold{\Phi}(0)=\bold{I}$ 的基解矩阵也可以写作 $\exp(\bold{A}t)$。对于例 2 中的矩阵 $\bold{A}$，初始条件 $\bold{x}(0)=\bold{x}^0$ 的微分方程组 $\bold{x}'=\bold{A}\bold{x}$ 的解是 $\bold{x}(t)=\exp(\bold{A}t)\bold{x}^0$，即 $\bold{x}(t)=\bold{\Phi}(t)\bold{x}^0$。

### Jordan 标准型
