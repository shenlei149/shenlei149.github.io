通过引入基解矩阵的概念，可以进一步阐明线性微分方程组解的结构。假定 $\bold{x}^{(1)}(t),\cdots,\bold{x}^{(n)}(t)$ 是方程
$$\bold{x}'=\bold{P}(t)\bold{x}\tag{1}$$
在某个区间 $\alpha<t<\beta$ 的基础解系。那么矩阵
$$\bold{\Psi}(t)=\begin{pmatrix}
\bold{x}^{(1)}(t)|\cdots|\bold{x}^{(n)(t)}
\end{pmatrix}=\begin{pmatrix}
x_1^{(1)}(t)&\cdots&x_1^{(n)}(t)\\
\vdots&&\vdots\\
x_n^{(1)}(t)&\cdots&x_n^{(n)}(t)
\end{pmatrix}\tag{2}$$
是方程 $(1)$ 的基解矩阵（`fundamental matrix`）。由于列是线性无关的，那么基解矩阵是非奇异的。

例 1 求方程组
$$\bold{x}'=\begin{pmatrix}
1&1\\1&4
\end{pmatrix}\bold{x}\tag{3}$$
的基解矩阵。

解：7.5 小节的例 2 已经求得了基础解系
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
e^{3t}\\2e^{3t}
\end{pmatrix},\bold{x}^{(2)}(t)=\begin{pmatrix}
e^{-t}\\-2e^{-t}
\end{pmatrix}$$
因此 $(3)$ 的基解矩阵是
$$\bold{\Psi}(t)=\begin{pmatrix}
e^{3t}&e^{-t}\\
2e^{3t}&-2e^{-t}
\end{pmatrix}\tag{4}$$

使用基解矩阵可以将初值问题的解写成非常紧凑的形式。$(1)$ 的通解是
$$\bold{x}=c_1\bold{x}^{(1)}(t)+\cdots+c_n\bold{x}^{(n)}(t)\tag{5}$$
使用 $\bold{\Psi}$ 可以写作
$$\bold{x}=\bold{\Psi}(t)\bold{c}\tag{6}$$
其中 $\bold{c}$ 是任意分量 $c_1,\cdots,c_n$ 的向量。对于一个包含 $(1)$ 和如下初始条件的初值问题，
$$\bold{x}(t_0)=\bold{x}^0\tag{7}$$
其中 $t_0$ 是区间 $\alpha<t<\beta$ 的给定点，$\bold{x}^0$ 是给定的初始向量，要求解一个 $\bold{c}$ 满足初始条件 $(7)$。因此 $\bold{c}$ 必须满足
$$\bold{\Psi}(t_0)\bold{c}=\bold{x}^0\tag{8}$$
由于 $\bold{\Psi}(t_0)$ 是非奇异的，那么
$$\bold{c}=\bold{\Psi}^{-1}(t_0)\bold{x}^0\tag{9}$$
因此
$$\bold{x}=\bold{\Psi}(t)\bold{\Psi}^{-1}(t_0)\bold{x}^0\tag{10}$$
是初值问题 $(1),(7)$ 的解。为了求解初值问题，我们一般用高斯消元法求解 $(8)$ 中的 $\bold{c}$ 然后代入 $(6)$ 得到答案而不是计算 $(10)$ 里面的 $\bold{\Psi}^{-1}(t_0)$。

基解矩阵 $\bold{\Psi}$ 的每一列都是 $(1)$ 的解，那么 $\bold{\Psi}$ 满足矩阵微分方程
$$\bold{\Psi}'=\bold{P}(t)\bold{\Psi}\tag{11}$$
通过逐列比较 $(11)$ 的两边，可以很容易验证正确性。

有时使用特殊的基解矩阵会很方便，特殊的基解矩阵用 $\bold{\Phi}(t)$ 来表示，它的每一列是由定理 7.4.4 定义的向量 $\bold{x}^{(1)}(t),\cdots,\bold{x}^{(n)}(t)$。除了微分方程 $(1)$ 之外，这些向量还满足
$$\bold{x}^{(j)}(t_0)=\bold{e}^{(j)}\tag{12}$$
其中 $\bold{e}^{(j)}$ 定理 7.4.4 定义的单位矢量，第 $j$ 个位置上 1 其余位置都是 0.因此
$$\bold{\Phi}(t_0)=\begin{pmatrix}
1&0&\cdots&0\\
0&1&\cdots&0\\
\vdots&\vdots&&\vdots\\
0&0&\cdots&1
\end{pmatrix}=\bold{I}\tag{13}$$
我们使用 $\bold{\Phi}$ 表示满足初始条件 $(13)$ 的基解矩阵，使用 $\bold{\Psi}$ 来 表示任意基解矩阵。使用 $\bold{\Phi}$ 来表示初值问题 $(1),(7)$ 的话会非常紧凑，由于 $\bold{\Phi}^{-1}(t_0)=\bold{I}$，那么
$$\bold{x}=\bold{\Phi}\bold{x}^0\tag{14}$$
尽管基解矩阵 $\bold{\Phi}(t)$ 要比 $\bold{\Psi}(t)$ 复杂，但是如果需要在很多不同初始条件下反复求解微分方程组的话就非常有用。这对应着一个物理物体有很多不同的初始状态。如果基解矩阵 $\bold{\Phi}(t)$ 已经有了，那么用每一组初始条件乘以矩阵就可以得到解，如 $(14)$。矩阵 $\bold{\Phi}(t)$ 表示从初始条件 $\bold{x}^0$ 到任意时刻 $t$ 的解 $\bold{x}(t)$ 的转换。对比 $(10),(14)$，可以得到 $\bold{\Phi}(t)=\bold{\Psi}(t)\bold{\Psi}^{-1}(t_0)$。

例 2 例 1 中的方程组是
$$\bold{x}'=\begin{pmatrix}
1&1\\1&4
\end{pmatrix}\bold{x}$$
求满足 $\bold{\Phi}(0)=\bold{I}$ 的基解矩阵。

解：满足初始条件
$$\bold{x}^{(1)}(0)=\begin{pmatrix}
1\\0
\end{pmatrix},\bold{x}^{(2)}(0)=\begin{pmatrix}
0\\1
\end{pmatrix}$$
的 $(3)$ 的解组成了 $\bold{\Phi}$ 的列。由于通解是
$$\bold{x}=c_1\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t}+c_2\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}$$
满足第一个初始条件的话，$c_1=c_2=\frac{1}{2}$，满足第二个初始条件的话，$c_1=\frac{1}{4},c_2=-\frac{1}{4}$，因此
$$\bold{\Phi}(t)=\begin{pmatrix}
\frac{1}{2}e^{3t}+\frac{1}{2}e^{-t}&\frac{1}{4}e^{3t}-\frac{1}{4}e^{-t}\\
e^{3t}-e^{-t}&\frac{1}{2}e^{3t}+\frac{1}{2}e^{-t}
\end{pmatrix}\tag{16}$$
这里基解矩阵 $\bold{\Phi}(t)$ 比 $(4)$ 给出的基解矩阵 $\bold{\Psi}(t)$ 要复杂，不过对于任意初始条件，很容易求出解。

### 矩阵 $\exp(\bold{A}t)$
标量初值问题
$$x'=ax,x(0)=x_0\tag{17}$$
的解是
$$x=x_0\exp(at)\tag{18}$$
其中 $a$ 是常量。

下面是 $n\times n$ 的方程组
$$\bold{x}'=\bold{A}\bold{x},\bold{x}(0)=\bold{x}^0\tag{19}$$
其中 $\bold{A}$ 是常量矩阵。根据之前的讨论，解是
$$\bold{x}=\bold{\Phi}(t)\bold{x}^0\tag{20}$$
其中 $\bold{\Phi}(0)=\bold{I}$。对于初值问题 $(17),(19)$ 和解 $(18),(20)$，矩阵 $\bold{\Phi}(t)$ 或许有指数特征。

标量指数函数 $\exp(at)$ 可以写成幂级数形式
$$\exp(at)=1+\sum_{n=1}^\infty\frac{a^nt^n}{n!}=1+at+\frac{a^2t^2}{2!}+\cdots+\frac{a^nt^n}{n!}+\cdots\tag{21}$$
对于所有 $t$ 都收敛。现在将 $a$ 换成 $n\times n$ 的矩阵 $\bold{A}$，1 换成 $n\times n$ 的单位矩阵 $\bold{I}$，那么级数是
$$\bold{I}+\sum_{n=1}^\infty\frac{\bold{A}^nt^n}{n!}=\bold{I}+\bold{A}t+\frac{\bold{A}^2t^2}{2!}+\cdots+\frac{\bold{A}^nt^n}{n!}+\cdots\tag{22}$$
$(22)$ 的每一项都是一个 $n\times n$ 的矩阵。可以证明对于所有的 $t$，当 $n\to\infty$ 时，矩阵的每一项都收敛。因此级数 $(22)$ 定义为了一个新的矩阵，用 $\exp(\bold{A}t)$ 表示
$$\exp(\bold{A}t)=\bold{I}+\sum_{n=1}^\infty\frac{\bold{A}^nt^n}{n!}\tag{23}$$
对级数 $(23)$ 微分
$$\frac{d}{dt}\exp(\bold{A}t)=\sum_{n=1}^\infty\frac{\bold{A}^nt^{n-1}}{(n-1)!}=\bold{A}(\bold{I}+\sum_{n=1}^\infty\frac{\bold{A}^nt^n}{n!})=\bold{A}\exp(\bold{A}t)\tag{24}$$
那么
$$\frac{d}{dt}\exp(\bold{A}t)=\bold{A}\exp(\bold{A}t)\tag{25}$$
将 $t=0$ 代入 $(23)$，那么 $\exp(\bold{A}t)$ 满足初值条件
$$\exp(\bold{A}t)\bigg|_{t=0}=\bold{I}+\sum_{n=1}^\infty\frac{\bold{A}^n0^n}{n!}=\bold{I}\tag{26}$$
基解矩阵 $\bold{\Phi}$ 也满足这是初值问题，即
$$\bold{\Phi}'=\bold{A}\bold{\Phi},\bold{\Phi}(0)=\bold{I}\tag{27}$$
根据定理 7.1.2 的解的唯一性，$\exp(\bold{A}t)$ 和 $\bold{\Phi}(t)$ 是相等的，因此初值问题 $(29)$ 的解也可以写成
$$\bold{x}=\exp(\bold{A}t)\bold{x}^0\tag{28}$$
这和 $(18)$ 是初值问题 $(17)$ 类似。

为了更确凿地论证用 $\exp(At)$ 表示级数 $(22)$ 的和是合理的，我们应当证明该矩阵函数具有指数函数的性质。根据定义，不难证明一下性质
$$\bold{\Phi}(t)\bold{\Phi}(s)=\bold{\Phi}(t+s),\exp(\bold{A}t)\exp(\bold{A}s)=\exp(\bold{A}(t+s))$$
$$\bold{\Phi}(t)\bold{\Phi}(-t)=\bold{I},\exp(\bold{A}t)\exp(\bold{A}(-t))=\bold{I}$$
$$\bold{\Phi}(t-s)=\bold{\Phi}(t)\bold{\Phi}^{-1}(s),\exp(\bold{A}(t-s))=\exp(\bold{A}(t))\exp(\bold{A}(-s))$$

### 可对角化矩阵
线性方程组困难的原因在于这些方程通常是耦合在一起的，其中或者所有的方程都涉及多个或全部的未知变量。因此，必须同时求解这些方程。如果每一个方程只涉及一个变量，那么可以独立的求解这些方程，就容易多了。这说明我们或许可以把一个方程组转换成一个等价的解耦的方程组，每一个方程仅涉及一个未知变量。这个变换将矩阵 $\bold{A}$ 转换为对角矩阵（`diagonal`）。

特征向量在这种变换中非常有用。假定矩阵 $\bold{A}$ 有 $n\times $ 个线性无关的特征向量 $\bold{\xi}^{(1)},\cdots,\bold{\xi}^{(n)}$，特征值是 $\lambda_1,\cdots,\lambda_n$，矩阵 $\bold{T}$ 是
$$\bold{T}=\begin{pmatrix}
\xi^{(1)}_1&\cdots&\xi^{(n)}_1\\
\vdots&&\vdots\\
\xi^{(1)}_n&\cdots&\xi^{(n)}_n
\end{pmatrix}\tag{29}$$
由于 $\bold{T}$ 的列是线性无关的，$\det\bold{T}\neq 0$，因此 $\bold{T}$ 是非奇异的，那么 $\bold{T}^{-1}$ 存在。矩阵 $\bold{A}\bold{T}$ 的列是 $\bold{A}\bold{\xi}^{(1)},\cdots,\bold{A}\bold{\xi}^{(n)}$。由于 $\bold{A}\bold{\xi}^{(k)}=\lambda_k\bold{\xi}^{(k)}$，那么
$$\bold{A}\bold{T}=\begin{pmatrix}
\lambda_1\xi^{(1)}_1&\cdots&\lambda_n\xi^{(n)}_1\\
\vdots&&\vdots\\
\lambda_1\xi^{(1)}_n&\cdots&\lambda_n\xi^{(n)}_n
\end{pmatrix}=\bold{T}\bold{D}\tag{30}$$
其中
$$\bold{D}=\begin{pmatrix}
\lambda_1&0&\cdots&0\\
0&\lambda_2&\cdots&0\\
\vdots&\vdots&\ddots&\vdots\\
0&0&\cdots&\lambda_n
\end{pmatrix}\tag{31}$$
是对角矩阵，对角线上 $\bold{A}$ 的特征值。从 $(30)$ 得到
$$\bold{T}^{-1}\bold{A}\bold{T}=\bold{D}\tag{32}$$
如果已知矩阵 $\bold{A}$ 的特征值和特征向量，就可以通过 $(32)$ 所示的过程将 $\bold{A}$ 变换为对角矩阵。这个过程被称为相似变换（`similarity transformation`），方程 $(32)$ 可以概括为：$\bold{A}$ 与对角矩阵 $\bold{D}$ 相似，或者称 $\bold{A}$ 为可对角化（`diagonalizable`）。相似变换保持 $\bold{A}$ 的特征值不变，并将 $\bold{A}$ 的特征向量变换为 $\bold{e}^{(1)},\cdots,\bold{e}^{(n)}$。

如果 $\bold{A}$ 是埃尔米特矩阵，$\bold{T}^{-1}$ 的求解会非常简单。$\bold{A}$ 的特征向量 $\bold{\xi}^{(1)},\cdots,\bold{\xi}^{(n)}$ 是正交的，对每一个 $i$ 进行归一化 $(\bold{\xi}^{(i)},\bold{\xi}^{(i)})=1$，那么 $\bold{T}^{-1}=\bold{T}*$，也就是说 $\bold{T}$ 的逆是其伴随矩阵（共轭的转置）。

如果 $\bold{A}$ 只有少于 $n$ 个线性无关的特征向量，那么不存在矩阵 $\bold{T}$ 使得 $\bold{T}^{-1}\bold{A}\bold{T}=\bold{D}$。这种情况下 $\bold{A}$ 与对角矩阵不相似，也无法对角化。这种情况会在 7.8 小节讨论。

例 3 给定矩阵
$$\bold{A}=\begin{pmatrix}
1&1\\4&1
\end{pmatrix}\tag{33}$$
求相似变换矩阵 $\bold{T}$ 并且证明 $\bold{A}$ 可对角化。

解：7.5 的例 2 给出了 $\bold{A}$ 的特征值和特征向量
$$r_1=3,\bold{\xi}^{(1)}=\begin{pmatrix}
1\\2
\end{pmatrix},r_2=-1,\bold{\xi}^{(2)}=\begin{pmatrix}
1\\-2
\end{pmatrix}\tag{34}$$
因此变换矩阵 $\bold{T}$ 和它的逆矩阵 $\bold{T}^{-1}$ 是
$$\bold{T}=\begin{pmatrix}
1&1\\2&-2
\end{pmatrix},\bold{T}^{-1}=\begin{pmatrix}
\frac{1}{2}&\frac{1}{4}\\\frac{1}{2}&-\frac{1}{4}
\end{pmatrix}\tag{35}$$
那么
$$\bold{T}^{-1}\bold{A}\bold{T}=\begin{pmatrix}
3&0\\0&01
\end{pmatrix}=\bold{D}\tag{36}$$

下面回到方程组
$$\bold{x}'=\bold{A}\bold{x}\tag{37}$$
其中 $\bold{A}$ 是常量矩阵。7.5 和 7.6 小节讨论了假定解是 $\bold{x}=\bold{\xi}e^{rt}$ 的形式来求解方程组。下面从 $\bold{A}$ 对角化系数的角度来分析问题。

根据前面的分析，$\bold{A}$ 可对角化是 $\bold{A}$ 有 $n$ 个线性无关的特征向量。令 $\bold{A}$ 的特征值是 $r_1,\cdots,r_n$ 对应的特征向量是 $\bold{\xi}^{(1)},\cdots,\bold{\xi}^{(n)}$，这些特征向量组成了矩阵 $\bold{T}$。下面定义一个新的变量 $\bold{y}$
$$\bold{x}=\bold{T}\bold{y}\tag{38}$$
代入 $(37)$ 得到
$$\bold{T}\bold{y}'=\bold{A}\bold{T}\bold{y}\tag{39}$$
两边同乘 $\bold{T}^{-1}$
$$\bold{y}'=(\bold{T}^{-1}\bold{A}\bold{T})\bold{y}\tag{40}$$
由于 $\bold{T}^{-1}\bold{A}\bold{T}=\bold{D}$，那么 $(40)$ 可以写成
$$\bold{y}'=\bold{D}\bold{y}\tag{41}$$
$\bold{D}$ 是 $\bold{A}$ 的对角化矩阵，对角线上的值是 $\bold{A}$ 的特征值。方程组 $(41)$ 的基解矩阵是对角矩阵
$$\bold{Q}(t)=\exp(\bold{D}t)=\begin{pmatrix}
e^{r_1t}&0&\cdots&0\\
0&e^{r_2t}&\cdots&0\\
\vdots&\vdots&\ddots&\vdots\\
0&0&\cdots&e^{r_nt}
\end{pmatrix}\tag{42}$$
方程组 $(37)$ 的基解矩阵 $\bold{\Psi}$ 是
$$\bold{\Psi}=\bold{T}\bold{Q}\tag{43}$$
即
$$\bold{\Psi}(t)=\begin{pmatrix}
\xi_1^{(1)}e^{r_1t}&\cdots&\xi_1^{(n)}e^{r_nt}\\
\vdots&&\vdots\\
\xi_n^{(1)}e^{r_1t}&\cdots&\xi_n^{(n)}e^{r_nt}
\end{pmatrix}\tag{44}$$
这里 $\bold{\Psi}$ 的每一列和 7.5 小节的 $(27)$ 一致。对角化的过程和 7.5 小节的解法相比，没有计算优势，因为计算特征值和特征向量都是必须的。

例 4 给定微分方程
$$\bold{x}'=\bold{A}\bold{x}\tag{45}$$
其中 $\bold{A}$ 是 $(33)$ 给出的矩阵，使用变换 $\bold{x}=\bold{T}\bold{y}$，其中 $\bold{T}$ 由 $(35)$ 给出，那么方程组 $(45)$ 可以简化为
$$\bold{y}'=\begin{pmatrix}
3&0\\0&-1
\end{pmatrix}\bold{y}=\bold{D}\bold{y}\tag{46}$$
计算 $(46)$ 的基解矩阵，并给出原始 $(45)$ 的基解矩阵。

解：根据 $(42)$ 可以得到
$$e^{\bold{D}t}=\begin{pmatrix}
e^{3t}&0\\0&e^{-t}
\end{pmatrix}\tag{47}$$
那么
$$\bold{\Psi}(t)=\bold{T}\exp(\bold{D}t)=\begin{pmatrix}
1&1\\2&-2
\end{pmatrix}\begin{pmatrix}
e^{3t}&0\\0&e^{-t}
\end{pmatrix}=\begin{pmatrix}
e^{3t}&e^{-t}\\
2e^{3t}&-2e^{-t}
\end{pmatrix}\tag{48}$$
这和例 1 的结果一致。
