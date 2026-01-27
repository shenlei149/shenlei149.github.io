这一节和下一节讨论必要的矩阵知识。

一般使用粗体大写字母 $\boldsymbol{A,B,C,\cdots}$ 或粗体大写的希腊字母 $\boldsymbol{\Phi,\Psi,\cdots}$ 表示矩阵。一个有 $m$ 行 $n$ 列的矩阵如下
$$\boldsymbol{A}=\begin{pmatrix}
a_{11}&a_{12}&\cdots&a_{1n}\\
a_{11}&a_{12}&\cdots&a_{1n}\\
\vdots&\vdots&\vdots&\vdots\\
a_{m1}&a_{m2}&\cdots&a_{mn}\\
\end{pmatrix}\tag{1}$$
$\boldsymbol{A}$ 是 $m\times n$ 的矩阵（`matrix`）。本章后续会假设矩阵的元素是实数，不过这里讨论的矩阵元素可以是复数。$i$ 行 $j$ 列的元素用 $a_{ij}$ 表示。使用记号 $(a_{ij})$ 也可以表示一个矩阵。

$\boldsymbol{A}$ 的转置（`transpose`）用 $\boldsymbol{A}^T$ 表示，交换了行和列。因此如果 $\boldsymbol{A}=(a_{ij})$，那么 $\boldsymbol{A}^T=(a_{ji})$。用记号 $\overline{a_{ij}}$ 表示 $a_{ij}$ 的共轭，那么 $\overline{\boldsymbol{A}}$ 是 $\boldsymbol{A}$ 的共轭（`conjugate`）矩阵。$\boldsymbol{A}$ 的共轭矩阵的转置，称为 $\boldsymbol{A}$ 的伴随（`adjoint`）矩阵，用 $\boldsymbol{A}^*$ 表示。这里的伴随矩阵不是代数余子式的矩阵的转置，后者也称为伴随（`adjugate`）矩阵。
$$\boldsymbol{A}=\begin{pmatrix}
3&2-i\\
4+3i&-5+2i
\end{pmatrix}$$
$$\boldsymbol{A}^T=\begin{pmatrix}
3&4+3i\\
2-i&-5+2i
\end{pmatrix}$$
$$\overline{\boldsymbol{A}}=\begin{pmatrix}
3&2+i\\
4-3i&-5-2i
\end{pmatrix}$$
$$\boldsymbol{A}^*=\begin{pmatrix}
3&4-3i\\
2+i&-5-2i
\end{pmatrix}$$
后续主要研究两类特殊的矩阵。一个是方阵（方块矩阵，`square matrix`），行数和列数相同，即 $m=n$，一个是向量或列向量（`column vector`），只有一列，即 $n\times 1$ 的矩阵。向量一般用小写字母表示，比如 $\boldsymbol{x},\boldsymbol{y},\boldsymbol{\xi},\boldsymbol{\eta},\cdots$。$\boldsymbol{x}^T$ 是行向量，只有一行。

### 矩阵的属性
如果两个 $m\times n$ 的矩阵 $\boldsymbol{A},\boldsymbol{B}$ 相等，那么对应元素相等，即 $a_{ij}=b_{ij}$。

矩阵所有元素都是零称为零矩阵，用 $\boldsymbol{0}$ 表示。

两个 $m\times n$ 的矩阵 $\boldsymbol{A},\boldsymbol{B}$ 的加法定义如下
$$\boldsymbol{A}+\boldsymbol{B}=(a_{ij})+(b_{ij})=(a_{ij}+b_{ij})\tag{2}$$
矩阵的加法满足交换律和结合律
$$\boldsymbol{A}+\boldsymbol{B}=\boldsymbol{B}+\boldsymbol{A},\boldsymbol{A}+(\boldsymbol{B}+\boldsymbol{C})=(\boldsymbol{A}+\boldsymbol{B})+\boldsymbol{C}\tag{3}$$

矩阵 $\boldsymbol{A}$ 乘以实数或复数 $\alpha$ 定义为
$$\alpha\boldsymbol{A}=\alpha(a_{ij})=(\alpha a_{ij})\tag{4}$$
也就是说每一个元素都乘以 $\alpha$。分配律如下
$$\alpha(\boldsymbol{A}+\boldsymbol{B})=\alpha\boldsymbol{A}+\alpha\boldsymbol{B},(\alpha+\beta)\boldsymbol{A}=\alpha\boldsymbol{A}+\beta\boldsymbol{A}\tag{5}$$
这样可以定义负的 $\boldsymbol{A}$
$$-\boldsymbol{A}=(-1)\boldsymbol{A}\tag{6}$$
有了负的定义，可以得到减法定义
$$\boldsymbol{A}-\boldsymbol{B}=\boldsymbol{A}+(-\boldsymbol{B})\tag{7}$$
即
$$\boldsymbol{A}-\boldsymbol{B}=(a_{ij})-(b_{ij})=(a_{ij}-b_{ij})\tag{8}$$

矩阵的积 $\boldsymbol{A}\boldsymbol{B}$ 要求第一个因子的列数与第二个因子的行数相同。如果 $\boldsymbol{A},\boldsymbol{B}$ 分别是 $m\times n,n\times r$ 的矩阵，那么 $\boldsymbol{C}=\boldsymbol{A}\boldsymbol{B}$ 是 $m\times r$ 的矩阵。$\boldsymbol{C}$ 的第 $i$ 行第 $j$ 列是 $\boldsymbol{A}$ 的第 $i$ 行与 $\boldsymbol{B}$ 的第 $j$ 列的元素对应相乘然后求和，即
$$c_{ij}=\sum_{k=1}^n a_{ik}b_{kj}\tag{9}$$
根据定义，矩阵乘法满足结合律和分配律
$$(\boldsymbol{A}\boldsymbol{B})\boldsymbol{C}=\boldsymbol{A}(\boldsymbol{B}\boldsymbol{C})\tag{10}$$
$$\boldsymbol{A}(\boldsymbol{B}+\boldsymbol{C})=\boldsymbol{A}\boldsymbol{B}+\boldsymbol{A}\boldsymbol{C}\tag{11}$$
一般情况下，乘法不满足交换律。积 $\boldsymbol{A}\boldsymbol{B}$ 和 $\boldsymbol{B}\boldsymbol{A}$ 存在的前提是这两个矩阵都是方阵。即便如此一般情况下
$$\boldsymbol{A}\boldsymbol{B}\neq\boldsymbol{B}\boldsymbol{A}\tag{12}$$

例 1 下面计算两个矩阵的积，并验证不满足交换律。
$$\boldsymbol{A}=\begin{pmatrix}
1&-2&1\\
0&2&-1\\
2&1&1
\end{pmatrix},\boldsymbol{B}=\begin{pmatrix}
2&1&-1\\
1&-1&0\\
 2&-1&1
\end{pmatrix}$$
解：
$$\boldsymbol{A}\boldsymbol{B}=\begin{pmatrix}
2-2+2&1+2-1&-1+0+1\\
0+2-2&0-2+1&0+0-1\\
4+1+2&2-1-1&-2+0+1\\
\end{pmatrix}=\begin{pmatrix}
2&2&0\\
0&-1&-1\\
7&0&-1
\end{pmatrix}$$
类似的
$$\boldsymbol{B}\boldsymbol{A}=\begin{pmatrix}
0&-3&0\\1&-4&2\\4&-5&4
\end{pmatrix}$$
很明显
$$\boldsymbol{A}\boldsymbol{B}\neq\boldsymbol{B}\boldsymbol{A}$$

两个向量 $\boldsymbol{x},\boldsymbol{y}$ 的积有几种形式。物理和微积分中常用点积（`dot product`）
$$\boldsymbol{x}^T\boldsymbol{y}=\sum_{i=1}^n x_iy_i\tag{13}$$
$(13)$ 的结果可以是实数或复数。从上面的定义出发，可以得到
$$\boldsymbol{x}^T\boldsymbol{y}=\boldsymbol{y}^T\boldsymbol{x},\boldsymbol{x}^T(\boldsymbol{y}+\boldsymbol{z})=\boldsymbol{x}^T\boldsymbol{y}+\boldsymbol{x}^T\boldsymbol{z},(\alpha\boldsymbol{x})^T\boldsymbol{y}=\alpha(\boldsymbol{x}^T\boldsymbol{y})=\boldsymbol{x}^T(\alpha\boldsymbol{y})\tag{14}$$
还有一种两个维度相同的矢量的积是标量积（`scalar product`）或内积（`inner product`），用 $(\boldsymbol{x},\boldsymbol{y})$。
$$(\boldsymbol{x},\boldsymbol{y})=\sum_{i=1}^n x_i\overline{y_i}\tag{15}$$
标量积也可以是实数或者复数。对比 $(13),(15)$，如果 $\boldsymbol{y}$ 所有元素都是实数，$(13),(15)$ 是一样的，两者的关系是
$$(\boldsymbol{x},\boldsymbol{y})=\boldsymbol{x}^T\boldsymbol{y}\tag{16}$$
从 $(15)$ 这个定义出发，可以得到
$$\begin{aligned}
(\boldsymbol{x},\boldsymbol{y})&=\overline{(\boldsymbol{y},\boldsymbol{x})}\\
(\boldsymbol{x},\boldsymbol{y}+\boldsymbol{z})&=(\boldsymbol{x},\boldsymbol{y})+(\boldsymbol{x},\boldsymbol{z})\\
(\alpha\boldsymbol{x},\boldsymbol{y})&=\alpha(\boldsymbol{x},\boldsymbol{y})\\
(\boldsymbol{x},\alpha\boldsymbol{y})&=\bar{\alpha}(\boldsymbol{x},\boldsymbol{y})
\end{aligned}\tag{17}$$
即使 $x$ 有非零的虚部的元素，$\boldsymbol{x}$ 的标量积结果也是一个非负数
$$(\boldsymbol{x},\boldsymbol{x})=\sum_{i=1}^nx_i\overline{x_i}=\sum_{i=1}^n|x_i|^2\tag{18}$$
非负数 $(\boldsymbol{x},\boldsymbol{x})^{1/2}$ 通常记作 $||\boldsymbol{x}||$，是 $\boldsymbol{x}$ 的长度（`length`）或模（`magnitude`）。只有零向量 $\boldsymbol{x}=\boldsymbol{0}$ 的长度是零，即 $(\boldsymbol{x},\boldsymbol{x})=0$，其他向量的长度都是整数。如果 $(\boldsymbol{x},\boldsymbol{y})=0$，那么两个矢量 $\boldsymbol{x},\boldsymbol{y}$ 是正交的（`orthogonal`）。比如三维空间单位矢量 $\boldsymbol{i},\boldsymbol{j},\boldsymbol{k}$ 就是正交的。如果 $\boldsymbol{x}$ 的一些元素不是实数，那么点积
$$\boldsymbol{x}^T\boldsymbol{x}=\sum_{i=1}^nx_i^2\tag{19}$$
或许不是实数。对于某些非零矢量，$\boldsymbol{x}^T\boldsymbol{x}$ 也可能为零。
比如令
$$\boldsymbol{x}=\begin{pmatrix}
i\\-2\\1+i
\end{pmatrix},\boldsymbol{y}=\begin{pmatrix}
2-i\\i\\3
\end{pmatrix},\boldsymbol{z}=\begin{pmatrix}
1\\0\\i
\end{pmatrix}$$
那么
$$\begin{aligned}
\boldsymbol{x}^T\boldsymbol{y}&=4+3i\\
(\boldsymbol{x},\boldsymbol{y})&=2+7i\\
\boldsymbol{x}^T\boldsymbol{x}&=3+2i\\
(\boldsymbol{x},\boldsymbol{x})&=7\\
\boldsymbol{z}^T\boldsymbol{z}&=0\\
(\boldsymbol{z},\boldsymbol{z})&=2
\end{aligned}$$
$n\times n$ 的单位矩阵（`identity`）$\boldsymbol{I}$ 的定义如下
$$\boldsymbol{I}=\begin{pmatrix}
1&0&\cdots&0\\
0&1&\cdots&0\\
\vdots&\vdots&\ddots&\vdots\\
0&0&\cdots&1
\end{pmatrix}\tag{20}$$
那么对任意矩阵 $\boldsymbol{A}$ 都有
$$\boldsymbol{A}\boldsymbol{I}=\boldsymbol{I}\boldsymbol{A}=\boldsymbol{A}\tag{21}$$
因此矩阵乘法的交换律成立的前提是有一个矩阵是单位矩阵。

对 $n\times n$ 的矩阵 $\boldsymbol{A}$ 而言，如果有另一个矩阵 $\boldsymbol{B}$ 使得 $\boldsymbol{A}\boldsymbol{B}=\boldsymbol{I}=\boldsymbol{B}\boldsymbol{A}$，那么 $\boldsymbol{A}$ 是非奇异的（`nonsingular`）或可逆的（`invertible`）。如果存在这样的 $\boldsymbol{B}$，那么可以证明其是唯一的，称为 $\boldsymbol{A}$ 的逆矩阵（`inverse`），记作 $\boldsymbol{B}=\boldsymbol{A}^{-1}$，那么
$$\boldsymbol{A}\boldsymbol{A}^{-1}=\boldsymbol{A}^{-1}\boldsymbol{A}=\boldsymbol{I}\tag{22}$$
如果矩阵不存在逆矩阵，那么称为奇异的（`singular`）或不可逆（`noninvertible`）。

假定逆矩阵存在，有很多方式求 $\boldsymbol{A}$ 的逆矩阵 $\boldsymbol{A}^{-1}$。先讨论使用行列式（`determinant`）的方法。对于每个元素 $a_{ij}$，从矩阵中删除第 $i$ 行和第 $j$ 列得到的矩阵的行列式是余子式（`minor`）$M_{ij}$，$a_{ij}$ 的代数余子式（`cofactor`）定义为
$$C_{ij}=(-1)^{i+j}M_{ij}\tag{23}$$
$\boldsymbol{A}$ 的行列式记作 $\det A$，是沿着某行或者某列得到的各个代数余子式之和。比如对第一行做展开得到
$$\det\boldsymbol{A}=C_{11}+C_{12}+\cdots+C_{1n}$$
如果 $\det\boldsymbol{A}\neq 0$，那么 $\boldsymbol{A}$ 可逆，即 $\boldsymbol{A}^{-1}$ 存在。如果 $\boldsymbol{B}=\boldsymbol{A}^{-1}$，那么
$$b_{ij}=\frac{C_{ji}}{\det\boldsymbol{A}}\tag{24}$$
上面的公式并不是计算 $\boldsymbol{A}^{-1}$ 的高效的方式，但是指出了 $\boldsymbol{A}$ 可逆的条件。这个条件是冲要条件：$\det\boldsymbol{A}\neq 0$ 等价于 $\boldsymbol{A}$ 是非奇异的，$\det\boldsymbol{A}=0$ 等价于 $\boldsymbol{A}$ 是奇异的。

另一个求解 $\boldsymbol{A}^{-1}$ 的方式是对矩阵做初等行变换。

* 交换两行。
* 乘以一个非零的标量。
* 将一行的若干倍加到另外一行上。

通过一系列初等行变换对矩阵变换称之为行简化（`row reduction`）或高斯消元法（`Gaussian elimination`）。通过一系列初等行变换可以将非奇异矩阵 $\boldsymbol{A}$ 变换乘单位矩阵 $\boldsymbol{I}$。可以真名，对 $\boldsymbol{I}$ 进行同样的操作，得到了 $\boldsymbol{A}^{-1}$。对增广矩阵（`augmented matrix`） $(\boldsymbol{A}|\boldsymbol{I})$ 进行操作会很高效。

例 2 求矩阵的逆。
$$\boldsymbol{A}=\begin{pmatrix}
1&-1&-1\\
3&-1&2\\
2&2&3
\end{pmatrix}$$
解：首先写出增广矩阵
$$(\boldsymbol{A}|\boldsymbol{I})=\begin{pmatrix}
1&-1&-1&1&0&0\\
3&-1&2&0&1&0\\
2&2&3&0&0&1
\end{pmatrix}$$
首先将第一列中除了第一行之外的元素变为零，第一行乘以 -3 加到第二行上，第一行乘以 -2 加到第三行上，得到
$$\begin{pmatrix}
1&-1&-1&1&0&0\\
0&2&5&-3&1&0\\
0&4&5&-2&0&1
\end{pmatrix}$$
接着将第二列第二行的元素变成 1，第二行除以 2，得到
$$\begin{pmatrix}
1&-1&-1&1&0&0\\
0&1&5/2&-3/2&1/2&0\\
0&4&5&-2&0&1
\end{pmatrix}$$
将第二列除了第二行的元素变成 0，第二行加到第一行上，第二行乘以 -4 加到第三行上，得到
$$\begin{pmatrix}
1&0&3/2&-1/2&1/2&0\\
0&1&5/2&-3/2&1/2&0\\
0&0&-5&4&-2&1
\end{pmatrix}$$
第三列第三行变成 1，即第三行乘以 -1/5，得到
$$\begin{pmatrix}
1&0&3/2&-1/2&1/2&0\\
0&1&5/2&-3/2&1/2&0\\
0&0&1&-4/5&2/5&-1/5
\end{pmatrix}$$
最后第三列除了第三行的元素都变成 0，第三行乘以 -3/2 加到第一行，第三行乘以 -5/2 加到第二行得到
$$\begin{pmatrix}
1&0&0&7/10&-1/10&3/10\\
0&1&0&1/2&-1/2&1/2\\
0&0&1&-4/5&2/5&-1/5
\end{pmatrix}$$
那么
$$\boldsymbol{A}^{-1}=\begin{pmatrix}
7/10&-1/10&3/10\\
1/2&-1/2&1/2\\
-4/5&2/5&-1/5
\end{pmatrix}$$
$\boldsymbol{A}$ 的逆与 $\boldsymbol{A}$ 正交，很容易验证答案是否正确。

上面的类似中 $\boldsymbol{A}$ 比较简单。如果 $a_{11}$ 不是 1，那么需要第一行乘以 $1/a_{11}$ 将其变成 1。如果 $a_{11}=0$，需要将第一列中非零元素换到 $a_{11}$，即第一行与某一行交换。如果无法交换，说明第一行每个元素都是 0，那么行列式 $\det\boldsymbol{A}=0$，因此 $\boldsymbol{A}$ 不可逆，是奇异的。后续每一列处理都类似，与更低的行交换使 $a_{ii}$ 不为零，如果无法做到，那么说明 $\boldsymbol{A}$ 是奇异的，没有逆矩阵。

### 矩阵函数
有时，向量或者矩阵的元素是实数 $t$ 的函数。写作
$$\boldsymbol{x}(t)\begin{pmatrix}
x_1(t)\\\vdots\\x_n(t)
\end{pmatrix},\boldsymbol{A}=\begin{pmatrix}
a_{11}(t),&\cdots&a_{1n}(t)\\
\vdots&&\vdots\\
a_{m1}(t),&\cdots&a_{mn}(t)\\
\end{pmatrix}\tag{25}$$
如果矩阵 $\boldsymbol{A}$ 的每一个元素在 $t=t_0$ 时或者在区间 $\alpha<t<\beta$ 上连续，那么矩阵 $\boldsymbol{A}$ 在给定点或指定区间上连续。类似的，如果每一个元素都可微，那么称为 $\boldsymbol{A}$ 是可微的，那么
$$\frac{d\boldsymbol{A}}{dt}=\bigg(\frac{da_{ij}}{dt}\bigg)\tag{26}$$
即 $d\boldsymbol{A}/dt$ 是 $\boldsymbol{A}$ 的每个元素求导。类似的，矩阵函数的积分定义如下
$$\int_a^b\boldsymbol{A}(t)dt=\bigg(\int_a^ba_{ij}dt\bigg)\tag{27}$$
比如
$$\boldsymbol{A}(t)=\begin{pmatrix}
\sin t&t\\1&\cos t
\end{pmatrix}$$
那么
$$\boldsymbol{A}'(t)=\begin{pmatrix}
\cos t&1\\0&-\sin t
\end{pmatrix},\int_0^\pi \boldsymbol{A}(t)dt=\begin{pmatrix}
2&\pi^2/2\\\pi&0
\end{pmatrix}$$
很多初等微积分规则也适用于矩阵函数
$$\frac{d}{dt}(\boldsymbol{C}\boldsymbol{A})=\boldsymbol{C}\frac{d\boldsymbol{A}}{dt}\tag{28}$$
其中 $\boldsymbol{C}$ 是常量矩阵。
$$\frac{d}{dt}(\boldsymbol{A}+\boldsymbol{B})=\frac{d\boldsymbol{A}}{dt}+\frac{d\boldsymbol{B}}{dt}\tag{29}$$
$$\frac{d}{dt}(\boldsymbol{A}\boldsymbol{B})=\boldsymbol{A}\frac{d\boldsymbol{B}}{dt}+\frac{d\boldsymbol{A}}{dt}\boldsymbol{B}$$
注意 $(28),(30)$ 要保持矩阵乘法的顺序。定义 $(26),(27)$ 也适用于向量。

矩阵的一些操作是应用操作与每一个元素，比如乘以一个常数、微分、积分。不过，这对许多操作而言不成立，比如矩阵的平方并不是每个元素平方。
