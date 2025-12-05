这一节和下一节讨论必要的矩阵知识。

一般使用粗体大写字母 $\bold{A,B,C,\cdots}$ 或粗体大写的希腊字母 $\bold{\Phi,\Psi,\cdots}$ 表示矩阵。一个有 $m$ 行 $n$ 列的矩阵如下
$$\bold{A}=\begin{pmatrix}
a_{11}&a_{12}&\cdots&a_{1n}\\
a_{11}&a_{12}&\cdots&a_{1n}\\
\vdots&\vdots&\vdots&\vdots\\
a_{m1}&a_{m2}&\cdots&a_{mn}\\
\end{pmatrix}\tag{1}$$
$\bold{A}$ 是 $m\times n$ 的矩阵（`matrix`）。本章后续会假设矩阵的元素是实数，不过这里讨论的矩阵元素可以是复数。$i$ 行 $j$ 列的元素用 $a_{ij}$ 表示。使用记号 $(a_{ij})$ 也可以表示一个矩阵。

$\bold{A}$ 的转置（`transpose`）用 $\bold{A}^T$ 表示，交换了行和列。因此如果 $\bold{A}=(a_{ij})$，那么 $\bold{A}^T=(a_{ji})$。用记号 $\overline{a_{ij}}$ 表示 $a_{ij}$ 的共轭，那么 $\overline{\bold{A}}$ 是 $\bold{A}$ 的共轭（`conjugate`）矩阵。$\bold{A}$ 的共轭矩阵的转置，称为 $\bold{A}$ 的伴随（`adjoint`）矩阵，用 $\bold{A}^*$ 表示。这里的伴随矩阵不是代数余子式的矩阵的转置，后者也称为伴随（`adjugate`）矩阵。
$$\bold{A}=\begin{pmatrix}
3&2-i\\
4+3i&-5+2i
\end{pmatrix}$$
$$\bold{A}^T=\begin{pmatrix}
3&4+3i\\
2-i&-5+2i
\end{pmatrix}$$
$$\overline{\bold{A}}=\begin{pmatrix}
3&2+i\\
4-3i&-5-2i
\end{pmatrix}$$
$$\bold{A}^*=\begin{pmatrix}
3&4-3i\\
2+i&-5-2i
\end{pmatrix}$$
后续主要研究两类特殊的矩阵。一个是方阵（方块矩阵，`square matrix`），行数和列数相同，即 $m=n$，一个是向量或列向量（`column vector`），只有一列，即 $n\times 1$ 的矩阵。向量一般用小写字母表示，比如 $\bold{x},\bold{y},\bold{\xi},\bold{\eta},\cdots$。$\bold{x}^T$ 是行向量，只有一行。

### 矩阵的属性
如果两个 $m\times n$ 的矩阵 $\bold{A},\bold{B}$ 相等，那么对应元素相等，即 $a_{ij}=b_{ij}$。

矩阵所有元素都是零称为零矩阵，用 $\bold{0}$ 表示。

两个 $m\times n$ 的矩阵 $\bold{A},\bold{B}$ 的加法定义如下
$$\bold{A}+\bold{B}=(a_{ij})+(b_{ij})=(a_{ij}+b_{ij})\tag{2}$$
矩阵的加法满足交换律和结合律
$$\bold{A}+\bold{B}=\bold{B}+\bold{A},\bold{A}+(\bold{B}+\bold{C})=(\bold{A}+\bold{B})+\bold{C}\tag{3}$$

矩阵 $\bold{A}$ 乘以实数或复数 $\alpha$ 定义为
$$\alpha\bold{A}=\alpha(a_{ij})=(\alpha a_{ij})\tag{4}$$
也就是说每一个元素都乘以 $\alpha$。分配律如下
$$\alpha(\bold{A}+\bold{B})=\alpha\bold{A}+\alpha\bold{B},(\alpha+\beta)\bold{A}=\alpha\bold{A}+\beta\bold{A}\tag{5}$$
这样可以定义负的 $\bold{A}$
$$-\bold{A}=(-1)\bold{A}\tag{6}$$
有了负的定义，可以得到减法定义
$$\bold{A}-\bold{B}=\bold{A}+(-\bold{B})\tag{7}$$
即
$$\bold{A}-\bold{B}=(a_{ij})-(b_{ij})=(a_{ij}-b_{ij})\tag{8}$$

矩阵的积 $\bold{A}\bold{B}$ 要求第一个因子的列数与第二个因子的行数相同。如果 $\bold{A},\bold{B}$ 分别是 $m\times n,n\times r$ 的矩阵，那么 $\bold{C}=\bold{A}\bold{B}$ 是 $m\times r$ 的矩阵。$\bold{C}$ 的第 $i$ 行第 $j$ 列是 $\bold{A}$ 的第 $i$ 行与 $\bold{B}$ 的第 $j$ 列的元素对应相乘然后求和，即
$$c_{ij}=\sum_{k=1}^n a_{ik}b_{kj}\tag{9}$$
根据定义，矩阵乘法满足结合律和分配律
$$(\bold{A}\bold{B})\bold{C}=\bold{A}(\bold{B}\bold{C})\tag{10}$$
$$\bold{A}(\bold{B}+\bold{C})=\bold{A}\bold{B}+\bold{A}\bold{C}\tag{11}$$
一般情况下，乘法不满足交换律。积 $\bold{A}\bold{B}$ 和 $\bold{B}\bold{A}$ 存在的前提是这两个矩阵都是方阵。即便如此一般情况下
$$\bold{A}\bold{B}\neq\bold{B}\bold{A}\tag{12}$$

例 1 下面计算两个矩阵的积，并验证不满足交换律。
$$\bold{A}=\begin{pmatrix}
1&-2&1\\
0&2&-1\\
2&1&1
\end{pmatrix},\bold{B}=\begin{pmatrix}
2&1&-1\\
1&-1&0\\
 2&-1&1
\end{pmatrix}$$
解：
$$\bold{A}\bold{B}=\begin{pmatrix}
2-2+2&1+2-1&-1+0+1\\
0+2-2&0-2+1&0+0-1\\
4+1+2&2-1-1&-2+0+1\\
\end{pmatrix}=\begin{pmatrix}
2&2&0\\
0&-1&-1\\
7&0&-1
\end{pmatrix}$$
类似的
$$\bold{B}\bold{A}=\begin{pmatrix}
0&-3&0\\1&-4&2\\4&-5&4
\end{pmatrix}$$
很明显
$$\bold{A}\bold{B}\neq\bold{B}\bold{A}$$

两个向量 $\bold{x},\bold{y}$ 的积有几种形式。物理和微积分中常用点积（`dot product`）
$$\bold{x}^T\bold{y}=\sum_{i=1}^n x_iy_i\tag{13}$$
$(13)$ 的结果可以是实数或复数。从上面的定义出发，可以得到
$$\bold{x}^T\bold{y}=\bold{y}^T\bold{x},\bold{x}^T(\bold{y}+\bold{z})=\bold{x}^T\bold{y}+\bold{x}^T\bold{z},(\alpha\bold{x})^T\bold{y}=\alpha(\bold{x}^T\bold{y})=\bold{x}^T(\alpha\bold{y})\tag{14}$$
还有一种两个维度相同的矢量的积是标量积（`scalar product`）或内积（`inner product`），用 $(\bold{x},\bold{y})$。
$$(\bold{x},\bold{y})=\sum_{i=1}^n x_i\overline{y_i}\tag{15}$$
标量积也可以是实数或者复数。对比 $(13),(15)$，如果 $\bold{y}$ 所有元素都是实数，$(13),(15)$ 是一样的，两者的关系是
$$(\bold{x},\bold{y})=\bold{x}^T\bold{y}\tag{16}$$
从 $(15)$ 这个定义出发，可以得到
$$\begin{aligned}
(\bold{x},\bold{y})&=\overline{(\bold{y},\bold{x})}\\
(\bold{x},\bold{y}+\bold{z})&=(\bold{x},\bold{y})+(\bold{x},\bold{z})\\
(\alpha\bold{x},\bold{y})&=\alpha(\bold{x},\bold{y})\\
(\bold{x},\alpha\bold{y})&=\bar{\alpha}(\bold{x},\bold{y})
\end{aligned}\tag{17}$$
即使 $x$ 有非零的虚部的元素，$\bold{x}$ 的标量积结果也是一个非负数
$$(\bold{x},\bold{x})=\sum_{i=1}^nx_i\overline{x_i}=\sum_{i=1}^n|x_i|^2\tag{18}$$
非负数 $(\bold{x},\bold{x})^{1/2}$ 通常记作 $||\bold{x}||$，是 $\bold{x}$ 的长度（`length`）或模（`magnitude`）。只有零向量 $\bold{x}=\bold{0}$ 的长度是零，即 $(\bold{x},\bold{x})=0$，其他向量的长度都是整数。如果 $(\bold{x},\bold{y})=0$，那么两个矢量 $\bold{x},\bold{y}$ 是正交的（`orthogonal`）。比如三维空间单位矢量 $\bold{i},\bold{j},\bold{k}$ 就是正交的。如果 $\bold{x}$ 的一些元素不是实数，那么点积
$$\bold{x}^T\bold{x}=\sum_{i=1}^nx_i^2\tag{19}$$
或许不是实数。对于某些非零矢量，$\bold{x}^T\bold{x}$ 也可能为零。
比如令
$$\bold{x}=\begin{pmatrix}
i\\-2\\1+i
\end{pmatrix},\bold{y}=\begin{pmatrix}
2-i\\i\\3
\end{pmatrix},\bold{z}=\begin{pmatrix}
1\\0\\i
\end{pmatrix}$$
那么
$$\begin{aligned}
\bold{x}^T\bold{y}&=4+3i\\
(\bold{x},\bold{y})&=2+7i\\
\bold{x}^T\bold{x}&=3+2i\\
(\bold{x},\bold{x})&=7\\
\bold{z}^T\bold{z}&=0\\
(\bold{z},\bold{z})&=2
\end{aligned}$$
$n\times n$ 的单位矩阵（`identity`）$\bold{I}$ 的定义如下
$$\bold{I}=\begin{pmatrix}
1&0&\cdots&0\\
0&1&\cdots&0\\
\vdots&\vdots&\ddots&\vdots\\
0&0&\cdots&1
\end{pmatrix}\tag{20}$$
那么对任意矩阵 $\bold{A}$ 都有
$$\bold{A}\bold{I}=\bold{I}\bold{A}=\bold{A}\tag{21}$$
因此矩阵乘法的交换律成立的前提是有一个矩阵是单位矩阵。

对 $n\times n$ 的矩阵 $\bold{A}$ 而言，如果有另一个矩阵 $\bold{B}$ 使得 $\bold{A}\bold{B}=\bold{I}=\bold{B}\bold{A}$，那么 $\bold{A}$ 是非奇异的（`nonsingular`）或可逆的（`invertible`）。如果存在这样的 $\bold{B}$，那么可以证明其是唯一的，称为 $\bold{A}$ 的逆矩阵（`inverse`），记作 $\bold{B}=\bold{A}^{-1}$，那么
$$\bold{A}\bold{A}^{-1}=\bold{A}^{-1}\bold{A}=\bold{I}\tag{22}$$
如果矩阵不存在逆矩阵，那么称为奇异的（`singular`）或不可逆（`noninvertible`）。

假定逆矩阵存在，有很多方式求 $\bold{A}$ 的逆矩阵 $\bold{A}^{-1}$。先讨论使用行列式（`determinant`）的方法。对于每个元素 $a_{ij}$，从矩阵中删除第 $i$ 行和第 $j$ 列得到的矩阵的行列式是余子式（`minor`）$M_{ij}$，$a_{ij}$ 的代数余子式（`cofactor`）定义为
$$C_{ij}=(-1)^{i+j}M_{ij}\tag{23}$$
$\bold{A}$ 的行列式记作 $\det A$，是沿着某行或者某列得到的各个代数余子式之和。比如对第一行做展开得到
$$\det\bold{A}=C_{11}+C_{12}+\cdots+C_{1n}$$
如果 $\det\bold{A}\neq 0$，那么 $\bold{A}$ 可逆，即 $\bold{A}^{-1}$ 存在。如果 $\bold{B}=\bold{A}^{-1}$，那么
$$b_{ij}=\frac{C_{ji}}{\det\bold{A}}\tag{24}$$
上面的公式并不是计算 $\bold{A}^{-1}$ 的高效的方式，但是指出了 $\bold{A}$ 可逆的条件。这个条件是冲要条件：$\det\bold{A}\neq 0$ 等价于 $\bold{A}$ 是非奇异的，$\det\bold{A}=0$ 等价于 $\bold{A}$ 是奇异的。

另一个求解 $\bold{A}^{-1}$ 的方式是对矩阵做初等行变换。

* 交换两行。
* 乘以一个非零的标量。
* 将一行的若干倍加到另外一行上。

通过一系列初等行变换对矩阵变换称之为行简化（`row reduction`）或高斯消元法（`Gaussian elimination`）。通过一系列初等行变换可以将非奇异矩阵 $\bold{A}$ 变换乘单位矩阵 $\bold{I}$。可以真名，对 $\bold{I}$ 进行同样的操作，得到了 $\bold{A}^{-1}$。对增广矩阵（`augmented matrix`） $(\bold{A}|\bold{I})$ 进行操作会很高效。

例 2 求矩阵的逆。
$$\bold{A}=\begin{pmatrix}
1&-1&-1\\
3&-1&2\\
2&2&3
\end{pmatrix}$$
解：首先写出增广矩阵
$$(\bold{A}|\bold{I})=\begin{pmatrix}
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
$$\bold{A}^{-1}=\begin{pmatrix}
7/10&-1/10&3/10\\
1/2&-1/2&1/2\\
-4/5&2/5&-1/5
\end{pmatrix}$$
$\bold{A}$ 的逆与 $\bold{A}$ 正交，很容易验证答案是否正确。

上面的类似中 $\bold{A}$ 比较简单。如果 $a_{11}$ 不是 1，那么需要第一行乘以 $1/a_{11}$ 将其变成 1。如果 $a_{11}=0$，需要将第一列中非零元素换到 $a_{11}$，即第一行与某一行交换。如果无法交换，说明第一行每个元素都是 0，那么行列式 $\det\bold{A}=0$，因此 $\bold{A}$ 不可逆，是奇异的。后续每一列处理都类似，与更低的行交换使 $a_{ii}$ 不为零，如果无法做到，那么说明 $\bold{A}$ 是奇异的，没有逆矩阵。

### 矩阵函数
有时，向量或者矩阵的元素是实数 $t$ 的函数。写作
$$\bold{x}(t)\begin{pmatrix}
x_1(t)\\\vdots\\x_n(t)
\end{pmatrix},\bold{A}=\begin{pmatrix}
a_{11}(t),&\cdots&a_{1n}(t)\\
\vdots&&\vdots\\
a_{m1}(t),&\cdots&a_{mn}(t)\\
\end{pmatrix}\tag{25}$$
如果矩阵 $\bold{A}$ 的每一个元素在 $t=t_0$ 时或者在区间 $\alpha<t<\beta$ 上连续，那么矩阵 $\bold{A}$ 在给定点或指定区间上连续。类似的，如果每一个元素都可微，那么称为 $\bold{A}$ 是可微的，那么
$$\frac{d\bold{A}}{dt}=\bigg(\frac{da_{ij}}{dt}\bigg)\tag{26}$$
即 $d\bold{A}/dt$ 是 $\bold{A}$ 的每个元素求导。类似的，矩阵函数的积分定义如下
$$\int_a^b\bold{A}(t)dt=\bigg(\int_a^ba_{ij}dt\bigg)\tag{27}$$
比如
$$\bold{A}(t)=\begin{pmatrix}
\sin t&t\\1&\cos t
\end{pmatrix}$$
那么
$$\bold{A}'(t)=\begin{pmatrix}
\cos t&1\\0&-\sin t
\end{pmatrix},\int_0^\pi \bold{A}(t)dt=\begin{pmatrix}
2&\pi^2/2\\\pi&0
\end{pmatrix}$$
很多初等微积分规则也适用于矩阵函数
$$\frac{d}{dt}(\bold{C}\bold{A})=\bold{C}\frac{d\bold{A}}{dt}\tag{28}$$
其中 $\bold{C}$ 是常量矩阵。
$$\frac{d}{dt}(\bold{A}+\bold{B})=\frac{d\bold{A}}{dt}+\frac{d\bold{B}}{dt}\tag{29}$$
$$\frac{d}{dt}(\bold{A}\bold{B})=\bold{A}\frac{d\bold{B}}{dt}+\frac{d\bold{A}}{dt}\bold{B}$$
注意 $(28),(30)$ 要保持矩阵乘法的顺序。定义 $(26),(27)$ 也适用于向量。

矩阵的一些操作是应用操作与每一个元素，比如乘以一个常数、微分、积分。不过，这对许多操作而言不成立，比如矩阵的平方并不是每个元素平方。
