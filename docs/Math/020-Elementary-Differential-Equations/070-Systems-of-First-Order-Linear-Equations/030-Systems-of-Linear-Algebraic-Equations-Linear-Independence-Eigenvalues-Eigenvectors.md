这一节会复习一些对求解线性微分方程组很重要的线性代数的知识。

### 线性代数方程组
$n$ 元 $n$ 个方程组成的线性方程组如下
$$\begin{aligned}
a_{11}x_1+a_{12}x_2+\cdots+a_{1n}x_n&=b_1\\
&\vdots\\
a_{n1}x_1+a_{n2}x_2+\cdots+a_{nn}x_n&=b_n\\
\end{aligned}\tag{1}$$
矩阵形式是
$$\boldsymbol{A}\boldsymbol{x}=\boldsymbol{b}\tag{2}$$
其中 $\boldsymbol{A}$ 是 $n\times n$ 的矩阵，$b$ 是给定的 $n$ 维向量，$\boldsymbol{x}$ 是要求解的 $n$ 维向量。如果 $\boldsymbol{b}=\boldsymbol{0}$，那么是齐次的，否则是非齐次的。

如果矩阵 $\boldsymbol{A}$ 是非奇异的，即行列式不为零，不管 $\boldsymbol{b}$ 是任意向量，方程组 $(2)$ 都有唯一解。由于 $\boldsymbol{A}$ 是非奇异的，那么 $\boldsymbol{A}^{-1}$ 存在，方程 $(2)$ 两边同时乘以 $\boldsymbol{A}^{-1}$ 可以得到解，即
$$\boldsymbol{x}=\boldsymbol{A}^{-1}\boldsymbol{b}\tag{3}$$
对于齐次问题 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{0}$，即 $\boldsymbol{b}=\boldsymbol{0}$，那么 $(2)$ 只有一个解 $\boldsymbol{x}=0$。

如果 $\boldsymbol{A}$ 是奇异的，行列式为零，那么就依赖于右边的 $\boldsymbol{b}$，$(2)$ 或者无解或者存在解但是不是唯一的。由于 $\boldsymbol{A}$ 是奇异的，那么 $\boldsymbol{A}^{-1}$ 不存在，因此 $(3)$ 不再成立。

当 $\boldsymbol{A}$ 是奇异的，齐次方程
$$\boldsymbol{A}\boldsymbol{x}=\boldsymbol{0}\tag{4}$$
有无穷多个非零解。非齐次方程 $(2)$ 的解就更复杂一些。如果 $\boldsymbol{b}$ 满足条件
$$(\boldsymbol{b},\boldsymbol{y})=\boldsymbol{0\tag{5}}$$
其中对于所有向量 $\boldsymbol{y}$ 都满足 $\boldsymbol{A}^*\boldsymbol{y}=\boldsymbol{0}$，这里 $\boldsymbol{A}^*$ 是伴随矩阵，那么 $(2)$ 有无穷多个解。解的形式是
$$\boldsymbol{x}=\boldsymbol{x}^{(0)}+\boldsymbol{\xi}\tag{6}$$
其中 $\boldsymbol{x}^{(0)}$ 是方程 $(2)$ 的特解，$\boldsymbol{\xi}$ 是齐次方程 $(4)$ 的通解。这里 $(6)$ 和之前讨论的非齐次线性微分方程有点类似。

下面给出上述结论的证明。首先证明如果给定矩阵 $\boldsymbol{A}$，存在一个非零向量 $\boldsymbol{x}$ 使得 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{0}$，那么存在一个非零向量 $\boldsymbol{y}$ 使得 $\boldsymbol{A}^*\boldsymbol{y}=\boldsymbol{0}$。

因为存在非零向量 $\boldsymbol{x}$ 使得 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{0}$，那么 $\det\boldsymbol{A}=0$，因此 $\det\boldsymbol{A}^*=\det\overline{\boldsymbol{A}}^T=\det\overline{\boldsymbol{A}}=\overline{\det\boldsymbol{A}}=0$，因此存在一个非零向量 $\boldsymbol{y}$ 使得 $\boldsymbol{A}^*\boldsymbol{y}=\boldsymbol{0}$。

假定 $\boldsymbol{A}$ 是实数矩阵。那么
$$\boldsymbol{A}\boldsymbol{x}=\begin{pmatrix}
\sum_{i=1}^na_{1i}x_i\\
\vdots\\
\sum_{i=1}^na_{ni}x_i\\
\end{pmatrix}$$
因此
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=y_1\sum_{i=1}^na_{1i}x_i+\cdots+y_n\sum_{i=1}^na_{ni}x_i=\sum_{i=1}^n\sum_{j=1}^na_{ij}x_iy_j$$
类似的，
$$\boldsymbol{A}^T\boldsymbol{y}=\begin{pmatrix}
\sum_{i=1}^na_{i1}y_i\\
\vdots\\
\sum_{i=1}^na_{in}y_i\\
\end{pmatrix}$$
那么
$$(\boldsymbol{x},\boldsymbol{A}^T\boldsymbol{y})=\sum_{i=1}^n\sum_{j=1}^na_{ij}y_ix_j=(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})$$
如果 $\boldsymbol{A}$ 不一定是实数矩阵，可以换一种方式证明，也适用于上面的结论，但是没有这么直观，因为一些之前没有讨论的矩阵的性质（TODO add link for 050？）。先计算左边
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=\boldsymbol{y}^*(\boldsymbol{A}\boldsymbol{x})=\boldsymbol{y}^*\boldsymbol{A}\boldsymbol{x}$$
然后计算右边
$$(\boldsymbol{x},\boldsymbol{A}^*\boldsymbol{y})=(\boldsymbol{A}^*\boldsymbol{y})^*\boldsymbol{x}=\boldsymbol{y}^*(\boldsymbol{A}^*)^*\boldsymbol{x}=\boldsymbol{y}^*\boldsymbol{A}\boldsymbol{x}$$
因此
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=(\boldsymbol{x},\boldsymbol{A}^*\boldsymbol{y})$$
如果 $\det\boldsymbol{A}=0$，那么存在 $\boldsymbol{y}$ 使得 $\boldsymbol{A}^*\boldsymbol{y}=\boldsymbol{0}$，根据之前的推导，那么
$$(\boldsymbol{x},\boldsymbol{A}^*\boldsymbol{y})=0=(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})$$
如果 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{b}$ 有解，代入上式得到
$$(\boldsymbol{b},\boldsymbol{y})=0$$
反过来，如果已知 $(\boldsymbol{b},\boldsymbol{y})=0$，根据
$$(\boldsymbol{x},\boldsymbol{A}^*\boldsymbol{y})=0=(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})$$
可以得到 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{b}$ 有解。

如果 $\boldsymbol{x}^{(0)}$ 是方程 $(2)$ 的特解，$\boldsymbol{\xi}$ 是齐次方程 $(4)$ 的通解，那么
$$\begin{aligned}
\boldsymbol{A}(\boldsymbol{x}^{(0)}+\alpha\boldsymbol{\xi})&=\boldsymbol{A}\boldsymbol{x}^{(0)}+\alpha\boldsymbol{A}\boldsymbol{\xi}\\
&=\boldsymbol{b}+\alpha\boldsymbol{0}\\
&=\boldsymbol{b}
\end{aligned}$$
因此 $\boldsymbol{x}=\boldsymbol{x}^{(0)}+\alpha\boldsymbol{\xi}$ 是 $\boldsymbol{A}\boldsymbol{x}=\boldsymbol{b}$ 的解，其中 $\alpha$ 是任意常量。

上一段的结论对于线性方程组解的分类很重要。不过在求解方程组时，通常最好的方法是利用行简化将方程组变换为更简单的形式，如果存在解的话，可以轻松得到解。为了高效做到这一点，这里使用增广矩阵
$$(\boldsymbol{A}|\boldsymbol{b})=\begin{pmatrix}
a_{11}&\cdots&a_{1n}&|&b_1\\
\vdots&&\vdots&|&\vdots\\
a_{n1}&\cdots&a_{nn}&|&b_n
\end{pmatrix}\tag{7}$$
执行初等行变换将矩阵 $\boldsymbol{A}$ 变成上三角矩阵（`upper triangular matrix`），即主对角线以下的元素都是零，完成之后很容易确定方程组是否有解，然后得到解。这里对增广矩阵 $(7)$ 的操作对应与求解方程组 $(1)$ 的有效操作。

例 1 求方程组
$$\begin{aligned}
x_1-2x_2+3x_3&=7\\
-x_1+x_2-2x_3&=-5\\
2x_1-x_2-x_3&=4
\end{aligned}\tag{8}$$
解：增广矩阵是
$$\begin{pmatrix}
1&-2&3&7\\
-1&1&-2&-5\\
2&-1&-1&4
\end{pmatrix}\tag{9}$$
现在执行初等行变换将 $(9)$ 的主对角线下面的元素都变为零。

首先第二行加上第一行，第三行减去第一行的两倍
$$\begin{pmatrix}
1&-2&3&7\\
0&-1&1&2\\
0&3&-7&-10
\end{pmatrix}$$
第二行乘以 -1
$$\begin{pmatrix}
1&-2&3&7\\
0&1&-1&-2\\
0&3&-7&-10
\end{pmatrix}$$
第二行乘以 -3 加到第三行上
$$\begin{pmatrix}
1&-2&3&7\\
0&1&-1&-2\\
0&0&-4&-4
\end{pmatrix}$$
第三行乘以 -1/4
$$\begin{pmatrix}
1&-2&3&7\\
0&1&-1&-2\\
0&0&1&1
\end{pmatrix}$$
相应的方程组是
$$\begin{aligned}
x_1&-&2x_2&+&3x_3&=7\\
&&x_2&-&x_3&=-2\\
&&&&x_3&=1
\end{aligned}\tag{10}$$
这个方程组等价于方程组 $(8)$。从第三个方程可以得到 $x_3=1$，代入第二个方程得到 $x_2=-2+x_3=-1$，在代入第一个方程得到 $x_1=7-3x_3+2x_2=2$，因此
$$\boldsymbol{x}=\begin{pmatrix}
2\\-1\\1
\end{pmatrix}$$
是方程 $(8)$ 的解。因为解释唯一的，可以得到系数矩阵是非奇异的。

例 2 讨论不同的 $b_1,b_2,b_3$ 时方程组
$$\begin{aligned}
&x_1&-&2x_2&+&3x_3&=b_1\\
-&x_1&+&x_2&-&2x_3&=b_2\\
&2x_1&-&x_2&+&3x_3&=b_3\\
\end{aligned}\tag{11}$$
的解。

解：除了第三个方程的 $x_3$ 的系数不同之外，其余系数与 $(8)$ 一样。方程组 $(11)$ 的增广矩阵是
$$\begin{pmatrix}
1&-2&3&b_1\\
-1&1&-2&b_2\\
2&-1&3&b_3
\end{pmatrix}\tag{12}$$
根据例 1 的步骤，上面的增广矩阵转换为
$$\begin{pmatrix}
1&-2&3&b_1\\
0&1&-1&-b_1-b_2\\
0&0&0&b_1+3b_2+b_3
\end{pmatrix}\tag{13}$$
第三行对应的方程是
$$b_1+3b_2+b_3=0\tag{14}$$
除非 $b_1,b_2,b_3$ 满足上述方程，否则方程组 $(11)$ 没有解。可以证明上面的方程是条件 $(5)$。

令 $b_1=2,b_2=1,b_3=-5$，这样可以满足 $(14)$，那么矩阵 $(13)$ 的前两行是
$$\begin{aligned}
&x_1&-&2x_2&+&3x_3&=&2\\
&&&x_2&-&x_3&=&-3
\end{aligned}\tag{15}$$
为了求解 $(15)$，令 $x_3=\alpha$，这是一个任意选择的量，来表示其他两个未知数
$$x_2=x_3-3=\alpha-3$$
$$x_1=2x_2-3x_3+2=-\alpha-4$$
写成向量形式是
$$\boldsymbol{x}=\begin{pmatrix}
-\alpha-4\\\alpha-3\\\alpha
\end{pmatrix}=\alpha\begin{pmatrix}
-1\\1\\1
\end{pmatrix}+\begin{pmatrix}
-4\\-3\\0
\end{pmatrix}\tag{16}$$
很容易验证右边第二项是非齐次方程 $(11)$ 的解，第一项是 $(11)$ 对应的齐次方程的解。

行简化，即高斯消元法，在求解齐次方程和方程个数与未知数个数不同的方程组时也很有用。

### 线性相关和线性无关
如果存在一组至少一个非零的实数或复数 $c_1,\cdots,c_k$ 使得
$$c_1\boldsymbol{x}^{(1)}+\cdots+c_k\boldsymbol{x}^{(k)}=\boldsymbol{0}\tag{17}$$
那么这 $k$ 个矢量 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(k)}$ 是线性相关的。也就说它们之间存在某种线性关系。如果唯一使得 $(17)$ 成立的系数 $c_1,\cdots,c_k$ 是 $c_1=c_2=\cdots=c_k=0$，那么 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(k)}$ 是线性无关的。

下面讨论 $n$ 个向量的集合，每一个向量有 $n$ 维，构成了一个 $n\times n$ 的矩阵 $\boldsymbol{X}$，$\boldsymbol{X}$ 的第 $j$ 列是 $\boldsymbol{x}_j$，因此 $\boldsymbol{X}=(x_{ij})$，其中 $x_{ij}=x_i^{(j)}$，也就是向量 $\boldsymbol{x}^{(j)}$ 的第 $i$ 个分量。令 $\boldsymbol{c}=c_{j}$，那么 $(17)$ 可以写作
$$\boldsymbol{X}\boldsymbol{c}=\boldsymbol{0}\tag{18}$$
如果 $\det\boldsymbol{X}\neq 0$，$(18)$ 的解只有 $\boldsymbol{c}=\boldsymbol{0}$，那么等价于 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(n)}$ 是线性无关的。如果 $\det\boldsymbol{X}=0$，有非零解。

比如从例 1 可以知道向量
$$\begin{pmatrix}
1\\-1\\2
\end{pmatrix},\begin{pmatrix}
-2\\1\\-1
\end{pmatrix},\begin{pmatrix}
3\\-2\\-1
\end{pmatrix}$$
是线性无关的。从例 2 可以知道向量
$$\begin{pmatrix}
1\\-1\\2
\end{pmatrix},\begin{pmatrix}
-2\\1\\-1
\end{pmatrix},\begin{pmatrix}
3\\-2\\3
\end{pmatrix}$$
是线性相关的。

例 3 确定向量
$$\boldsymbol{x}^{(1)}=\begin{pmatrix}
1\\2\\-1
\end{pmatrix},\boldsymbol{x}^{(2)}=\begin{pmatrix}
2\\1\\3
\end{pmatrix},\boldsymbol{x}^{(3)}=\begin{pmatrix}
-4\\1\\-11
\end{pmatrix}\tag{19}$$
是线性相关还是线性无关的。如果线性相关，找到它们之间的线性关系。

解：为了确定 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)},\boldsymbol{x}^{(3)}$ 是线性无关还是线性相关，要求使得
$$c_1\boldsymbol{x}^{(1)}+c_2\boldsymbol{x}^{(2)}+c_3\boldsymbol{x}^{(3)}=\boldsymbol{0}\tag{20}$$
的常量 $c_1,c_2,c_3$。

方程 $(20)$ 可以写作形式
$$\begin{pmatrix}
1&2&-4\\
2&1&1\\
-1&3&-11
\end{pmatrix}\begin{pmatrix}
c_1\\c_2\\c_3
\end{pmatrix}=\begin{pmatrix}
0&0&0
\end{pmatrix}\tag{21}$$
可以通过对下面的增广矩阵进行初等行变换求解
$$\begin{pmatrix}
1&2&-4&0\\
2&1&1&0\\
-1&3&-11&0
\end{pmatrix}\tag{22}$$
第一行乘以 -2 加到第二行，第一行加到第三行得到
$$\begin{pmatrix}
1&2&-4&0\\
0&-3&9&0\\
0&5&-15&0
\end{pmatrix}$$
第二行乘以 -1/3 得到
$$\begin{pmatrix}
1&2&-4&0\\
0&-1&3&0\\
0&5&-15&0
\end{pmatrix}$$
第二行乘以 5 加到第三行得到
$$\begin{pmatrix}
1&2&-4&0\\
0&-1&3&0\\
0&0&0&0
\end{pmatrix}$$
因此得到等价的方程组
$$\begin{aligned}
c_1+2c_2-4c_3&=0\\
c_2-3c_3&=0
\end{aligned}\tag{23}$$
因此 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)},\boldsymbol{x}^{(3)}$ 是线性相关的。

为了找到 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)},\boldsymbol{x}^{(3)}$ 之间的线性关系，从 $(23)$ 的第二个方程可以得到 $c_2=3c_3$，代入第一个方程得到 $c_1=4c_3-2c_2=-2c_3$。这里使用 $c_3$ 来表示 $c_1,c_2$，而 $c_3$ 可以任意选择。比如令 $c_3=-1$，那么 $c_1=2,c_2=-3$，那么 $(20)$ 的线性关系是
$$2\boldsymbol{x}^{(1)}-3\boldsymbol{x}^{(2)}-1\boldsymbol{x}^{(3)}=\boldsymbol{0}$$

另一种方法是不妨称 $(21)$ 系数方程为 $\boldsymbol{X}$ 然后计算 $\det\boldsymbol{X}$。
$$\begin{aligned}
\det\boldsymbol{X}&=\begin{vmatrix}
1&2&-4\\2&1&1\\-1&3&-11
\end{vmatrix}\\
&=(1)\begin{vmatrix}
1&1\\3&-11
\end{vmatrix}-(2)\begin{vmatrix}
2&1\\-1&-11
\end{vmatrix}+(-4)\begin{vmatrix}
2&1\\-1&3
\end{vmatrix}\\
&=-14-2(-21)-4(7)\\
&=0
\end{aligned}$$
因此 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)},\boldsymbol{x}^{(3)}$ 是线性相关的。不过为了求线性关系，还是不得不求解方程 $(20)$。

将矩阵 $\boldsymbol{A}$ 的列或者行看做向量很有用。这些向量线性无关等价于 $\det\boldsymbol{A}\neq 0$。如果 $\boldsymbol{C}=\boldsymbol{A}\boldsymbol{B}$，那么 $\det\boldsymbol{C}=\det\boldsymbol{A}\det\boldsymbol{B}$，因此 $\boldsymbol{A},\boldsymbol{B}$ 的列或者行线性无关，那么 $\boldsymbol{C}$ 的列或者行也线性无关。

现在将这些定义扩展到区间 $\alpha<t<\beta$ 上的矢量函数 $\boldsymbol{x}^{(1)}(t),\cdots,\boldsymbol{x}^{(k)}(t)$。如果在存在不全是零的常量 $c_1,\cdots,c_k$ 使得在上述区间内有
$$c_1\boldsymbol{x}^{(1)}(t)+\cdots+c_k\boldsymbol{x}^{(k)}(t)=\boldsymbol{0}$$
那么 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(k)}$ 是线性先关的，否则是线性无关的。如果 $\boldsymbol{x}^{(1)}(t),\cdots,\boldsymbol{x}^{(k)}(t)$ 在区间上线性相关，那么在区间内的每一点都是线性相关的。不过，如果 $\boldsymbol{x}^{(1)}(t),\cdots,\boldsymbol{x}^{(k)}(t)$ 在区间上线性无关，或许在每一点处都是线性无关的，或许不是。还有可能是在每一点处都是线性无关的，但是不同点的常量集合不同。

### 特征值和特征向量
方程
$$\boldsymbol{A}\boldsymbol{x}=\boldsymbol{y}\tag{24}$$
可以看作是将一个矢量 $\boldsymbol{x}$ 转换称另一个矢量 $\boldsymbol{y}$。在许多应用中，一个矢量能转换成自身的若干倍是很重要的，包括求解常系数一阶线性微分方程组。

令 $\boldsymbol{y}=\lambda\boldsymbol{x}$，那么
$$\boldsymbol{A}\boldsymbol{x}=\lambda\boldsymbol{x}\tag{25}$$
$$(\boldsymbol{A}-\lambda\boldsymbol{I})\boldsymbol{x}=\boldsymbol{0}\tag{26}$$
方程有非零解等价于 $\lambda$ 使得
$$\det(\boldsymbol{A}-\lambda\boldsymbol{I})=0\tag{27}$$
方程 $(27)$ 是 $\lambda$ 的 $n$ 阶多项式，称为矩阵 $\boldsymbol{A}$ 的特征方程（`characteristic equation`）。满足 $(27)$ 的 $\lambda$ 可以是实数也可以是复数，称为 $\boldsymbol{A}$ 的特征值（`eigenvalue`）。对应某个 $\lambda$，满足 $(25)$ 或 $(26)$ 的矢量 $\boldsymbol{x}$ 称为特征值对应的特征向量（`eigenvector`）。

例 4 求矩阵
$$\boldsymbol{A}=\begin{pmatrix}
3&-1\\4&-2
\end{pmatrix}\tag{28}$$
的特征值和特征向量。

解：特征值 $\lambda$ 和特征向量 $\boldsymbol{x}$ 满足方程 $(\boldsymbol{A}-\lambda\boldsymbol{I})\boldsymbol{x}=\boldsymbol{0}$，即
$$\begin{pmatrix}
3-\lambda&-1\\4&-2-\lambda
\end{pmatrix}\begin{pmatrix}
x_1\\x_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{29}$$
特征值是特征方程
$$\det(\boldsymbol{A}-\lambda\boldsymbol{I})=\begin{vmatrix}
3-\lambda&-1\\4&-2-\lambda
\end{vmatrix}=\lambda^2-\lambda-2=(\lambda-2)(\lambda+1)=0\tag{30}$$
的根，那么 $\lambda_1=2,\lambda_2=-1$。

为了求解特征向量，将每个特征值 $\lambda$ 代回 $(29)$。对于 $\lambda=2$ 有
$$\begin{pmatrix}
1&-1\\4&-4
\end{pmatrix}\begin{pmatrix}
x_1\\x_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{31}$$
那么 $x_1-x_2=0$。如果 $x_1=c$，那么 $x_2=c$，因此特征向量 $\boldsymbol{x}^{(1)}$ 是
$$\boldsymbol{x}^{(1)}=c\begin{pmatrix}
1\\1
\end{pmatrix},c\neq 0\tag{32}$$
对于特征值 $\lambda=2$，有无数个特征向量。一般从中选择一个作为其余的代表，这里令 $c=1$，那么
$$\boldsymbol{x}^{(1)}=\begin{pmatrix}
1\\1
\end{pmatrix}\tag{33}$$
这个矢量的非零倍数也是特征向量。一般称 $\boldsymbol{x}^{(1)}$ 是特征值 $\lambda_1=2$ 的特征向量。

令 $(29)$ 中的 $\lambda=-1$，那么得到
$$\begin{pmatrix}
4&-1\\4&-1
\end{pmatrix}\begin{pmatrix}
x_1\\x_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{34}$$
这里仍旧只能获得一个条件，即 $4x_1-x_2=0$。因此对应特征值 $\lambda_2=-1$ 的特征向量是
$$\boldsymbol{x}^{(2)}=\begin{pmatrix}
1\\4
\end{pmatrix}\tag{35}$$
或者是其非零倍。

上面的例子中，特征向量之间差一个非零的系数，如果以某种方式指定这个常数，称这个特征向量被归一化（`normalized`）。例 4 中我们的选择让每个分量是尽可能小的整数，选择其他值也是有效的，但是可能没有那么方便。有时选择让特征向量归一化 $||\boldsymbol{x}||=(\boldsymbol{x},\boldsymbol{x})^{1/2}=1$ 很方便。

$n\times n$ 的矩阵 $\boldsymbol{A}$ 的特征方程 $(27)$ 是一个关于 $\lambda$ 的 $n$ 阶多项式方程，那么有 $n$ 个特征值 $\lambda_1,\cdots,\lambda_n$，其中一些可能有重复。如果一个特征值出现 $m$ 次，称其代数重数（`algebraic multiplicity`）为 $m$。每个特征值至少有一个与之对应的特征向量，也可能有其他线性无关的特征向量。如果一个特征值有 $q$ 个线性无关特征向量，称其几何重数（`geometric multiplicity`）是 $q$。可以证明有如下关系
$$1\leq q\leq m\tag{36}$$
也就是说几何重数不会超过代数重数。如果 $\boldsymbol{A}$ 的每个特征值都是简单的（`simple`），即代数重数为 1，那么几何重数也是 1。

如果 $\lambda_1,\lambda_2$ 是 $\boldsymbol{A}$ 的两个特征值，并且 $\lambda_1\neq\lambda_2$，那么相应的特征向量 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(1)}$ 是线性无关的。要证明线性无关从下面的线性方程开始
$$c_1\boldsymbol{x}^{(1)}+c_2\boldsymbol{x}^{(2)}=\boldsymbol{0}$$
两边同时乘以矩阵 $\boldsymbol{A}$，得到
$$c_1\boldsymbol{A}\boldsymbol{x}^{(1)}+c_2\boldsymbol{A}\boldsymbol{x}^{(2)}=\boldsymbol{0}$$
因为两个特征值满足方程
$$(\boldsymbol{A}-\lambda\boldsymbol{I})\boldsymbol{x}=\boldsymbol{0}$$
那么
$$\boldsymbol{A}\boldsymbol{x}^{(1)}=\lambda_1\boldsymbol{I}\boldsymbol{x}^{(1)}$$
$$\boldsymbol{A}\boldsymbol{x}^{(2)}=\lambda_2\boldsymbol{I}\boldsymbol{x}^{(2)}$$
代入最开始的等式得到
$$c_1\lambda_1\boldsymbol{I}\boldsymbol{x}^{(1)}+c_2\lambda_2\boldsymbol{I}\boldsymbol{x}^{(2)}=\boldsymbol{0}$$
$$c_1\lambda_1\boldsymbol{x}^{(1)}+c_2\lambda_2\boldsymbol{x}^{(2)}=\boldsymbol{0}$$
由于 $\lambda_1\neq\lambda_2$，那么为了上式成立必须有 $c_1=c_2=0$，因此是线性无关的。

这个结果可以扩展到任意个不同的特征值 $\lambda_1,\cdots,\lambda_k$，对应的特征向量 $\boldsymbol{x}^{(1)},\cdots,\boldsymbol{x}^{(k)}$ 是线性无关的。因此一个 $n\times n$ 的矩阵的每一个特征值都是简单的，那么有 $n$ 个特征值，每一个对应一个特征向量，这些特征向量是无关的。反之，如果 $\boldsymbol{A}$ 有一个或多个重复的特征值，可能只有少于 $n$ 个线性无关的向量，这是因为对于重复特征值而言可能有 $q<m$。后续章节会看到这一点会导致微分系方程组的复杂性。

例 5 求矩阵
$$\boldsymbol{A}=\begin{pmatrix}
0&1&1\\1&0&1\\1&1&0
\end{pmatrix}\tag{37}$$
的特征值和特征向量。

解：特征值 $\lambda$ 和特征向量 $\boldsymbol{x}$ 满足方程 $(\boldsymbol{A}-\lambda\boldsymbol{I})\boldsymbol{x}=\boldsymbol{0}$，那么
$$\begin{pmatrix}
-\lambda&1&1\\1&-\lambda&1\\1&1&-\lambda
\end{pmatrix}\begin{pmatrix}
x_1\\x_2\\x_3
\end{pmatrix}=\begin{pmatrix}
0\\0\\0
\end{pmatrix}\tag{38}$$
特征值是方程
$$\det(\boldsymbol{A}-\lambda\boldsymbol{I})=\begin{vmatrix}
-\lambda&1&1\\1&-\lambda&1\\1&1&-\lambda
\end{vmatrix}=-\lambda^3+3\lambda+2=0\tag{39}$$
方程有三个根 $\lambda_1=2,\lambda_2=-1,\lambda_3=-1$，因此 2 是简单特征值，-1 是代数重数为 2 的特征值。

将 $\lambda=2$ 代入 $(38)$ 得到
$$\begin{pmatrix}
-2&1&1\\1&-2&1\\1&1&-2
\end{pmatrix}\begin{pmatrix}
x_1\\x_2\\x_3
\end{pmatrix}=\begin{pmatrix}
0\\0\\0
\end{pmatrix}\tag{40}$$
通过初等行变换（高斯消元法）得到等价方程组
$$\begin{pmatrix}
-2&1&1\\0&1&-1\\0&0&0
\end{pmatrix}\begin{pmatrix}
x_1\\x_2\\x_3
\end{pmatrix}=\begin{pmatrix}
0\\0\\0
\end{pmatrix}\tag{41}$$
得到特征方程
$$\boldsymbol{x}^{(1)}=\begin{pmatrix}
1\\1\\1
\end{pmatrix}\tag{42}$$
代入 $\lambda=-1$，方程 $(38)$ 可以简化为
$$x_1+x_2+x_3=0\tag{43}$$
因此三个变量的其中两个值可以任意指定，第三个由上式确定。这里令 $x_1=c_1,x_2=c_2$，那么 $x_3=-c_1-c_2$，那么
$$\boldsymbol{x}=\begin{pmatrix}
c_1\\c_2\\-c_1-c_2
\end{pmatrix}=c_1\begin{pmatrix}
1\\0\\-1
\end{pmatrix}+c_2\begin{pmatrix}
0\\1\\-1
\end{pmatrix}\tag{44}$$
选择 $c_1=1,c_2=0$ 得到特征向量
$$\boldsymbol{x}^{(2)}=\begin{pmatrix}
1\\0\\-1
\end{pmatrix}$$
令 $c_1=0,c_2=1$ 可以得到另一个线性无关的特征向量
$$\boldsymbol{x}^{(3)}=\begin{pmatrix}
0\\1\\-1
\end{pmatrix}\tag{46}$$
这个代数重数为 2 的特征值对应两个线性无关的特征向量。

有一类特殊的矩阵，称为自伴随（`self-adjoint`）或埃尔米特矩阵（`Hermitian`）：$\boldsymbol{A}=\boldsymbol{A}^*$，即 $a_{ij}=\overline{a_{ji}}$。埃尔米特矩阵包含一个子类，对称的实数矩阵，也就是说矩阵的元素是实数并且有 $\boldsymbol{A}^T=\boldsymbol{A}$。埃尔米特矩阵的特征值和特征向量满足下面这些有用的属性：

1. 所有的特征值是实数。
2. 不管特征值的代数重数是多少，总是存在 $n$ 个线性无关的特征向量。
3. 如果特征向量 $\boldsymbol{x}^{(1)},\boldsymbol{x}^{(1)}$ 是不同特征值对应的特征向量，那么 $(\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})=0$。如果所有的特征值是简单的，那么对应的特征向量都是正交的。
4. 一个特征值的代数重数是 $m$，可以选择出 $m$ 个特征向量彼此正交。因此，总是可以选择 $n$ 个特征向量，彼此正交，且它们是线性无关的。

下面证明属性 1。之前已经证明
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=(\boldsymbol{x},\boldsymbol{A}^*\boldsymbol{y})$$
而埃尔米特矩阵是 $\boldsymbol{A}=\boldsymbol{A}^*$ 的矩阵，代入上式得到
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=(\boldsymbol{x},\boldsymbol{A}\boldsymbol{y})$$
这里 $\boldsymbol{y}$ 是任意向量，取值为 $\boldsymbol{x}$，那么得到
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{x})=(\boldsymbol{x},\boldsymbol{A}\boldsymbol{x})$$
$\lambda$ 是特征值，$\boldsymbol{x}$ 是对应的特征向量，那么
$$\boldsymbol{A}\boldsymbol{x}=\lambda\boldsymbol{x}$$
代入上式得到
$$(\lambda\boldsymbol{x},\boldsymbol{x})=(\boldsymbol{x},\lambda\boldsymbol{x})$$
因此
$$\lambda(\boldsymbol{x},\boldsymbol{x})=\bar{\lambda}(\boldsymbol{x},\boldsymbol{x})$$
那么
$$\lambda=\bar{\lambda}$$
所以 $\lambda$ 是实数。

下面证明属性 3。假定 $\lambda_1,\lambda_2$ 是 $\boldsymbol{A}$ 的两个特征值，且 $\lambda_1\neq\lambda_2$，$\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)}$ 是与之对应的两个特征向量。

我们从
$$(\boldsymbol{A}\boldsymbol{x},\boldsymbol{y})=(\boldsymbol{x},\boldsymbol{A}\boldsymbol{y})$$
开始，这里令 $\boldsymbol{x}=\boldsymbol{x}^{(1)},\boldsymbol{y}=\boldsymbol{x}^{(2)}$，那么
$$\begin{aligned}
(\boldsymbol{A}\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})&=(\boldsymbol{x}^{(1)},\boldsymbol{A}\boldsymbol{x}^{(2)})\\
(\lambda_1\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})&=(\boldsymbol{x}^{(1)},\lambda_2\boldsymbol{x}^{(2)})\\
\lambda_1(\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})&=\lambda_2(\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})\\
(\lambda_1-\lambda_2)(\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})&=0
\end{aligned}$$
因此
$$(\boldsymbol{x}^{(1)},\boldsymbol{x}^{(2)})=0$$
所以两个特征向量正交。

例 5 是一个实数对称矩阵，满足属性 1、2、3，但是选择的两个矢量 $\boldsymbol{x}^{(2)},\boldsymbol{x}^{(3)}$ 并不满足属性 4。不过总是可以做到这一点。比如分别令 $c_1=1,c_2=-2$ 得到两个矢量
$$\boldsymbol{x}^{(2)}=\begin{pmatrix}
1\\0\\-1
\end{pmatrix},\boldsymbol{x}^{(3)}=\begin{pmatrix}
1\\-2\\1
\end{pmatrix}$$
作为 $\lambda=-1$ 对应的两个特征向量。这两个矢量彼此正交并且与 $\lambda=2$ 对应的特征向量 $\boldsymbol{x}^{(1)}$ 正交。
