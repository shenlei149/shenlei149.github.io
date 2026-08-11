这一节讨论线性一阶微分方程组成的非齐次方程组
$$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}+\boldsymbol{g}(t)\tag{1}$$
其中 $\boldsymbol{P}(t)$ 是 $n\times n$ 矩阵，$\boldsymbol{g}(t)$ 是 $n\times 1$ 的向量，在 $\alpha<t<\beta$ 上都是连续的。和 3.5 小节讨论一致，方程 $(1)$ 的通解形式是
$$\boldsymbol{x}=c_1\boldsymbol{x}^{(1)}(t)+\cdots+c_n\boldsymbol{x}^{(n)}(t)+\boldsymbol{v}(t)\tag{2}$$
其中 $c_1\boldsymbol{x}^{(1)}(t)+\cdots+c_n\boldsymbol{x}^{(n)}(t)$ 是对应的齐次方程组 $\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}$ 的通解，$\boldsymbol{v}(t)$ 是非齐次方程组 $(1)$ 的特解。这里会简要讨论几种求解 $\boldsymbol{v}(t)$ 的方法。

### 对角化
我们从
$$\boldsymbol{x}'=\boldsymbol{A}\boldsymbol{x}+\boldsymbol{g}(t)\tag{3}$$
开始，其中 $\boldsymbol{A}$ 是可对角化的常量矩阵。按照 7.7 小节的讨论，通过将矩阵 $\boldsymbol{A}$ 对角化，就将 $(3)$ 变换成了容易求解的方程组了。

令 $\boldsymbol{A}$ 的特征向量 $\boldsymbol{\xi}^{(1)},\cdots,\boldsymbol{\xi}^{(n)}$ 组成了矩阵 $\boldsymbol{T}$，定义一个新的变量 $\boldsymbol{y}$
$$\boldsymbol{x}=\boldsymbol{T}\boldsymbol{y}\tag{4}$$
将 $\boldsymbol{x}$ 代入 $(3)$
$$\boldsymbol{T}\boldsymbol{y}'=\boldsymbol{A}\boldsymbol{T}\boldsymbol{y}+\boldsymbol{g}(t)$$
等式两边的左边都乘以矩阵 $\boldsymbol{T}^{-1}$ 得到
$$\boldsymbol{y}'=\boldsymbol{T}^{-1}\boldsymbol{A}\boldsymbol{T}\boldsymbol{y}+\boldsymbol{T}^{-1}\boldsymbol{g}(t)=\boldsymbol{D}\boldsymbol{y}+\boldsymbol{h}(t)\tag{5}$$
其中 $\boldsymbol{h}(t)=\boldsymbol{T}^{-1}\boldsymbol{g}(t)$，$\boldsymbol{D}$ 是对角矩阵，对角线上是 $\boldsymbol{A}$ 的特征值 $r_1,\cdots,r_n$。方程组 $(5)$ 是 $n$ 个关于 $y_1(t),\cdots,y_n(t)$ 解耦的线性微分方程，容易逐个求解。方程 $(5)$ 的标量形式是
$$y_j'(t)=r_jy_j(t)+h_j(t),j=1,\cdots,n\tag{6}$$
$h_j(t)$ 是 $g_1(t),\cdots,g_n(t)$ 的线性组合。上面的线性方程可以使用 2.1 小节讨论的方法求解
$$y_j(t)=e^{r_jt}\int_{t_0}^t e^{-r_js}h_j(s)ds+c_je^{r_jt},j=1,\cdots,n\tag{7}$$
其中 $c_j$ 是任意常量。最后使用 $(4)$ 来计算得到 $(3)$ 的解。当两边同时乘以转移矩阵 $\boldsymbol{T}$，$(7)$ 的右边的第二项就是齐次方程 $\boldsymbol{x}'=\boldsymbol{A}\boldsymbol{x}$ 的通解，第一项是非齐次方程 $(3)$ 的特解。

例 1 求方程组
$$\boldsymbol{x}'=\begin{pmatrix}
-2&1\\1&-2
\end{pmatrix}\boldsymbol{x}+\begin{pmatrix}
2e^{-t}\\3t
\end{pmatrix}=\boldsymbol{A}\boldsymbol{x}+\boldsymbol{g}(t)\tag{8}$$
解：这个例子和 7.5 节的例子一样，因此系数矩阵的特征值是 $r_1=-3,r_2=-1$，相应的特征向量是
$$\boldsymbol{\xi}^{(1)}=\begin{pmatrix}
1\\-1
\end{pmatrix},\boldsymbol{\xi}^{(2)}=\begin{pmatrix}
1\\1
\end{pmatrix}\tag{9}$$
因此齐次方程组的通解是
$$\boldsymbol{x}=c_1\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-3t}+c_2\begin{pmatrix}
1\\1
\end{pmatrix}e^{-t}\tag{10}$$
由于系数矩阵 $\boldsymbol{A}$ 是实数对称矩阵，那么 $\boldsymbol{T}^{-1}$ 是 $\boldsymbol{T}$ 的伴随矩阵，也是转置矩阵，前提是把 $\boldsymbol{A}$ 的特征向量归一化为长度 1 的向量，即 $(\boldsymbol{\xi},\boldsymbol{\xi})=1$。由于 $\boldsymbol{\xi}^{(1)},\boldsymbol{\xi}^{(2)}$ 的长度是 $\sqrt{2}$，那么
$$\boldsymbol{T}=\frac{1}{\sqrt{2}}\begin{pmatrix}
1&1\\-1&1
\end{pmatrix},\boldsymbol{T}^{-1}=\frac{1}{\sqrt{2}}\begin{pmatrix}
1&-1\\1&1
\end{pmatrix}\tag{11}$$
令 $\boldsymbol{x}=\boldsymbol{T}\boldsymbol{y}$ 并代入 $(8)$ 得到
$$\boldsymbol{y}'=\boldsymbol{D}\boldsymbol{y}+\boldsymbol{T}^{-1}\boldsymbol{g}(t)=\begin{pmatrix}
-3&0\\0&-1
\end{pmatrix}\boldsymbol{y}+\frac{1}{\sqrt{2}}\begin{pmatrix}
2e^{-t}-3t\\2e^{-t}+3t
\end{pmatrix}\tag{12}$$
因此
$$\begin{aligned}
y_1'+3y_1&=\sqrt{2}e^{-t}-\frac{3}{\sqrt{2}}t\\
y_2'+y_2&=\sqrt{2}e^{-t}+\frac{3}{\sqrt{2}}t
\end{aligned}\tag{13}$$
$(13)$ 的每一个方程是独立的一阶线性微分方程，使用 2.1 小节的方法求解得到
$$\begin{aligned}
y_1&=\frac{\sqrt{2}}{2}e^{-t}-\frac{3}{\sqrt{2}}(\frac{t}{3}-\frac{1}{9})+c_1e^{-3t}\\
y_2&=\sqrt{2}te^{-t}+\frac{3}{\sqrt{2}}(t-1)+c_2e^{-t}
\end{aligned}\tag{14}$$
那么
$$\begin{aligned}
\boldsymbol{x}&=\boldsymbol{T}\boldsymbol{y}\\
&=\frac{1}{\sqrt{2}}\begin{pmatrix}
y_1+y_2\\-y_1+y_2
\end{pmatrix}\\
&=\begin{pmatrix}
\frac{c_1}{\sqrt{2}}e^{-3t}+(\frac{c_2}{\sqrt{2}}+\frac{1}{2})e^{-t}+t-\frac{4}{3}+te^{-t}\\
-\frac{c_1}{\sqrt{2}}e^{-3t}+(\frac{c_2}{\sqrt{2}}-\frac{1}{2})e^{-t}+2t-\frac{5}{3}+te^{-t}
\end{pmatrix}\\
&=k_1\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-3t}+k_2\begin{pmatrix}
1\\1
\end{pmatrix}e^{-t}+\frac{1}{2}\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-t}+\begin{pmatrix}
1\\1
\end{pmatrix}te^{-t}+\begin{pmatrix}
1\\2
\end{pmatrix}t-\frac{1}{3}\begin{pmatrix}
4\\5
\end{pmatrix}
\end{aligned}\tag{15}$$
其中 $k_1=c_1/\sqrt{2},k_2=c_2/\sqrt{2}$。前面两项是 $(8)$ 对应的齐次方程组的通解，其余各项是非齐次方程组的特解。

如果方程组 $(3)$ 的系数矩阵 $\boldsymbol{A}$ 不能对角化，不过可以通过特征向量和广义特征向量组成的变换矩阵 $\boldsymbol{T}$ 得到 Jordan 标准型 $\boldsymbol{J}$。由于 $\boldsymbol{J}$ 的一些行会有两个非零元素（主对角线上是特征值，右侧可能有非零元素），因此 $y_1,\cdots,y_n$ 的微分方程不是完全解耦的。不过从 $y_n$ 开始，也能逐步求解 $y_1,\cdots,y_n$，接着通过 $\boldsymbol{x}=\boldsymbol{T}\boldsymbol{y}$ 求解得到 $(3)$ 的解。

### 待定系数法
第二个求非齐次方程组 $(1)$ 的解的方法是待定系数法，和 3.5 小节的讨论类似。我们首先假定解的形式，其中包含一些待确定的系数，这些解满足微分方程组，进而求解这些系数。这种方法仅适用于 $\boldsymbol{P}$ 是常量矩阵，并且 $\boldsymbol{g}$ 的各个分量是由多项式、指数、正弦余弦或者它们的和组成的函数。此时，可以确定解的形式。选择解的形式和求解过程和 3.5 小节讨论类似。主要差别是当非齐次项包含 $\boldsymbol{u}e^{\lambda t}$ 时，之前是假定解的形式是 $\boldsymbol{a}te^{\lambda t}$，现在需要假定解的形式是 $\boldsymbol{a}te^{\lambda t}+\boldsymbol{b}e^{\lambda t}$，其中 $\lambda$ 是特征方程的根。

例 2 使用待定系数法求解
$$\boldsymbol{x}'=\begin{pmatrix}
-2&1\\1&-2
\end{pmatrix}\boldsymbol{x}+\begin{pmatrix}
2e^{-t}\\3t
\end{pmatrix}=\boldsymbol{A}\boldsymbol{x}+\boldsymbol{g}(t)\tag{16}$$
的特解。

解：为了使用待定系数法，将 $\boldsymbol{g}(t)$ 写作
$$\boldsymbol{g}(t)=\begin{pmatrix}
2\\0
\end{pmatrix}e^{-t}+\begin{pmatrix}
0\\3
\end{pmatrix}t\tag{17}$$
$r=-1$ 是系数矩阵的特征值，因此必须包含 $\boldsymbol{a}te^{-t},\boldsymbol{b}e^{-t}$ 两项，因此可以假定解是
$$\boldsymbol{x}=\boldsymbol{v}(t)=\boldsymbol{a}te^{-t}+\boldsymbol{b}e^{-t}+\boldsymbol{c}t+\boldsymbol{d}\tag{18}$$
其中 $\boldsymbol{a,b,c,d}$ 是待定的向量。将 $(18)$ 代入 $(16)$ 并令对应项的系数相等，那么
$$\begin{aligned}
\boldsymbol{A}\boldsymbol{a}&=-\boldsymbol{a}\\
\boldsymbol{A}\boldsymbol{b}&=\boldsymbol{a}-\boldsymbol{b}-\begin{pmatrix}
2\\0
\end{pmatrix}\\
\boldsymbol{A}\boldsymbol{c}&=-\begin{pmatrix}
0\\3
\end{pmatrix}\\
\boldsymbol{A}\boldsymbol{d}&=\boldsymbol{c}
\end{aligned}\tag{19}$$
第一个说明 $\boldsymbol{a}$ 是 $\boldsymbol{A}$ 的特征值 $r=-1$ 对应的特征向量，那么 $\boldsymbol{a}=(\alpha,\alpha)^T$，其中 $\alpha$ 是非零常量。那么令 $\alpha=1$ 可以用第二个方程求解
$$\boldsymbol{b}=k\begin{pmatrix}
1\\1
\end{pmatrix}-\begin{pmatrix}
0\\1
\end{pmatrix}\tag{20}$$
其中 $k$ 是任意常量。如果选择 $k=0$，那么 $\boldsymbol{b}=(0,-1)^T$。

求解第三个方程可以得到 $\boldsymbol{c}=(1,2)^T$，代入第四个方程可以得到 $\boldsymbol{d}=-\frac{1}{3}(4,5)^T$。因此特解是
$$\boldsymbol{v}(t)=\begin{pmatrix}
1\\1
\end{pmatrix}te^{-t}-\begin{pmatrix}
0\\1
\end{pmatrix}e^{-t}+\begin{pmatrix}
1\\2
\end{pmatrix}t-\frac{1}{3}\begin{pmatrix}
4\\5
\end{pmatrix}\tag{21}$$
这个特解和 $(15)$ 稍有不同，差异在 $e^{-t}$ 前的向量。不过，如果在 $(20)$ 中令 $k=1/2$，那么 $\boldsymbol{b}=\frac{1}{2}(1,-1)^T$，此时两个特解就完全一样了。

### 参数变分法
现在回到最一般的情况，系数矩阵不是常量，也无法对角化。令
$$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}+\boldsymbol{g}(t)\tag{22}$$
其中 $\boldsymbol{P}(t),\boldsymbol{g}(t)$ 在 $\alpha<t<\beta$ 上连续。假定
$$\boldsymbol{x}'=\boldsymbol{P}(t)\boldsymbol{x}\tag{23}$$
的基解矩阵是 $\boldsymbol{\Psi}(t)$。下面使用参数变分法来构造特解。

齐次方程组 $(23)$ 的通解是 $\boldsymbol{\Psi}(t)\boldsymbol{c}$，和 3.6 的过程类似，非齐次方程组 $(23)$ 的解是将常数向量 $\boldsymbol{c}$ 替换为向量函数 $\boldsymbol{u}(t)$。那么
$$\boldsymbol{x}=\boldsymbol{\Psi}(t)\boldsymbol{u}(t)\tag{24}$$
将上式代入 $(22)$ 得到
$$\boldsymbol{\Psi}'(t)\boldsymbol{u}(t)+\boldsymbol{\Psi}(t)\boldsymbol{u}'(t)=\boldsymbol{P}(t)\boldsymbol{\Psi}(t)\boldsymbol{u}(t)+\boldsymbol{g}(t)\tag{25}$$
由于 $\boldsymbol{\Psi}(t)$ 是基解矩阵，那么 $\boldsymbol{\Psi}'(t)=\boldsymbol{P}(t)\boldsymbol{\Psi}(t)$，那么
$$\boldsymbol{\Psi}(t)\boldsymbol{u}'(t)=\boldsymbol{g}(t)\tag{26}$$
当 $\boldsymbol{P}$ 是连续的那么 $\boldsymbol{\Psi}$ 是非奇异的，因此 $\boldsymbol{\Psi}^{-1}(t)$ 存在，因此
$$\boldsymbol{u}'(t)=\boldsymbol{\Psi}^{-1}(t)\boldsymbol{g}(t)\tag{27}$$
只要选择一个 $\boldsymbol{u}(t)$ 满足 $(27)$ 即可，那么
$$\boldsymbol{u}(t)=\int\boldsymbol{\Psi}^{-1}\boldsymbol{g}(t)dt+\boldsymbol{c}\tag{28}$$
其中 $\boldsymbol{c}$ 是任意常数向量。如果上式可以求得积分，将积分结果代入 $(24)$ 就得到了 $(22)$ 的特解。即便无法求解积分，也可以给出 $(22)$ 的通解形式
$$\boldsymbol{x}=\boldsymbol{\Psi}(t)\boldsymbol{c}+\boldsymbol{\Psi}(t)\int_{t_1}^t\boldsymbol{\Psi}^{-1}(s)\boldsymbol{g}(s)ds\tag{29}$$
其中 $t_1$ 是 $(\alpha,\beta)$ 区间上的任一点。上式右边第一项是齐次方程组 $(22)$ 的通解，第二项是 $(23)$ 的特解。

对于 $(22)$ 加上初始条件
$$\boldsymbol{x}(t_0)=\boldsymbol{x}^0\tag{30}$$
的初值问题，可以选择 $t_0$ 作为 $(29)$ 的积分下限。那么通解是
$$\boldsymbol{x}=\boldsymbol{\Psi}(t)\boldsymbol{c}+\boldsymbol{\Psi}(t)\int_{t_0}^t\boldsymbol{\Psi}^{-1}(s)\boldsymbol{g}(s)ds\tag{31}$$
对于 $t=t_0$，$(31)$ 的积分是零，因此为了满足初始条件 $(39)$，必须有
$$\boldsymbol{c}=\boldsymbol{\Psi}^{-1}(t_0)\boldsymbol{x}^0\tag{32}$$
因此
$$\boldsymbol{x}=\boldsymbol{\Psi}(t)\boldsymbol{\Psi}^{-1}(t_0)\boldsymbol{x}^0+\boldsymbol{\Psi}(t)\int_{t_0}^t\boldsymbol{\Psi}^{-1}(s)\boldsymbol{g}(s)ds\tag{33}$$
是初值问题的解。使用 $\boldsymbol{\Psi}^{-1}$ 来表示 $(29),(33)$ 很有用，但是通常是通过高斯消元法来求解 $\boldsymbol{\Psi}^{-1}$ 并代入 $(29),(33)$。

如果使用满足 $\boldsymbol{\Phi}(t_0)=\boldsymbol{I}$ 的基解矩阵 $\boldsymbol{\Phi}(t)$，可以进一步简化 $(33)$
$$\boldsymbol{x}=\boldsymbol{\Phi}(t)\boldsymbol{x}^0+\boldsymbol{\Phi}(t)\int_{t_0}^t\boldsymbol{\Phi}^{-1}(s)\boldsymbol{g}(s)ds\tag{34}$$
如果 $\boldsymbol{P}(t)$ 是常数矩阵，7.7 小节在 $\exp(\boldsymbol{A}t)$ 部分我们得到了公式
$$\boldsymbol{\Phi}(t-s)=\boldsymbol{\Phi}(t)\boldsymbol{\Phi}^{-1}(s)$$
代入 $(34)$ 进一步简化为
$$\begin{aligned}
\boldsymbol{x}&=\boldsymbol{\Phi}(t)\boldsymbol{x}^0+\boldsymbol{\Phi}(t)\int_{t_0}^t\boldsymbol{\Phi}^{-1}(t)\boldsymbol{\Phi}(t-s)\boldsymbol{g}(s)ds\\
&=\boldsymbol{\Phi}(t)\boldsymbol{x}^0+\boldsymbol{\Phi}(t)\boldsymbol{\Phi}^{-1}(t)\int_{t_0}^t\boldsymbol{\Phi}(t-s)\boldsymbol{g}(s)ds\\
&=\boldsymbol{\Phi}(t)\boldsymbol{x}^0+\int_{t_0}^t\boldsymbol{\Phi}(t-s)\boldsymbol{g}(s)ds
\end{aligned}$$

例 3 使用参数变分法求
$$\boldsymbol{x}'=\begin{pmatrix}
-2&1\\1&-2
\end{pmatrix}\boldsymbol{x}+\begin{pmatrix}
2e^{-t}\\3t
\end{pmatrix}=\boldsymbol{A}\boldsymbol{x}+\boldsymbol{g}(t)\tag{35}$$
的通解。

解：对应的齐次方程组的通解已经由 $(10)$ 给出，那么
$$\boldsymbol{\Phi}(t)=\begin{pmatrix}
e^{-3t}&e^{-t}\\
-e^{-3t}&e^{-t}
\end{pmatrix}\tag{36}$$
是基解矩阵。那么 $(35)$ 的通解是 $\boldsymbol{x}=\boldsymbol{\Phi}(t)\boldsymbol{u}(t)$，其中 $\boldsymbol{u}(t)$ 满足 $\boldsymbol{\Phi}(t)\boldsymbol{u}'(t)=\boldsymbol{g}(t)$，即
$$\begin{pmatrix}
e^{-3t}&e^{-t}\\
-e^{-3t}&e^{-t}
\end{pmatrix}\begin{pmatrix}
u_1'\\u_2'
\end{pmatrix}=\begin{pmatrix}
2e^{-t}\\3t
\end{pmatrix}\tag{37}$$
利用高斯消元法得到
$$\begin{aligned}
u_1'&=e^{2t}-\frac{3}{2}te^{3t}\\
u_2'&=1+\frac{3}{2}te^t
\end{aligned}$$
那么
$$\begin{aligned}
u_1(t)&=\frac{1}{2}e^{2t}-\frac{1}{2}te^{3t}+\frac{1}{6}e^{3t}+c_1\\
u_2(t)&=t+\frac{3}{2}te^t-\frac{3}{2}e^t+c_2
\end{aligned}$$
那么
$$\begin{aligned}
\boldsymbol{x}&=\boldsymbol{\Phi}(t)\boldsymbol{u}(t)\\
&=c_1\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-3t}+c_2\begin{pmatrix}
1\\1
\end{pmatrix}e^{-t}+\frac{1}{2}\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-t}+\begin{pmatrix}
1\\1
\end{pmatrix}te^{-t}+\begin{pmatrix}
1\\2
\end{pmatrix}t-\frac{1}{3}\begin{pmatrix}
4\\5
\end{pmatrix}
\end{aligned}\tag{38}$$
通解与例 1 和例 2 一致。

### 拉普拉斯变换
第六章我们使用拉普拉斯变换求解任意阶线性方程，这种方法也可以求解方程组。由于变换是积分，因此向量的变换是对这个分量进行变换。$\boldsymbol{x}(t)$ 各个分量变换组成的向量表示为 $\mathcal{L}\{\boldsymbol{x}(t)\}$。$\mathcal{L}\{\boldsymbol{x}'(t)\}$ 类似。这里用 $\boldsymbol{X}(s)$ 来表示 $\mathcal{L}\{\boldsymbol{x}(t)\}$，那么扩展定理 6.2.1 就得到
$$\mathcal{L}\{\boldsymbol{x}'(t)\}=s\boldsymbol{X}(s)-\boldsymbol{x}(0)\tag{39}$$

例 4 使用拉普拉斯变换法求解方程组
$$\boldsymbol{x}'=\begin{pmatrix}
-2&1\\1&-2
\end{pmatrix}\boldsymbol{x}+\begin{pmatrix}
2e^{-t}\\3t
\end{pmatrix}=\boldsymbol{A}\boldsymbol{x}+\boldsymbol{g}(t)\tag{40}$$
解：$(40)$ 的拉普拉斯变换是
$$s\boldsymbol{X}(s)-\boldsymbol{x}(0)=\boldsymbol{A}\boldsymbol{X}(s)+\boldsymbol{G}(s)\tag{41}$$
其中 $\boldsymbol{G}(s)$ 是 $\boldsymbol{g}(t)$ 的拉普拉斯变换。
$$\boldsymbol{G}(s)=\begin{pmatrix}
\frac{2}{s+1}\\\frac{3}{s^2}
\end{pmatrix}\tag{42}$$
为了进一步处理和简便，这里选择 $\boldsymbol{x}(0)=\boldsymbol{0}$，那么 $(41)$ 简化为
$$(s\boldsymbol{I}-\boldsymbol{A})\boldsymbol{X}(s)=\boldsymbol{G}(s)\tag{43}$$
那么
$$\boldsymbol{X}(s)=(s\boldsymbol{I}-\boldsymbol{A})^{-1}\boldsymbol{G}(s)\tag{44}$$
矩阵 $(s\boldsymbol{I}-\boldsymbol{A})^{-1}$ 称为转移矩阵（`transfer matrix`），乘以输入向量 $\boldsymbol{g}(t)$ 后就是输出向量 $\boldsymbol{x}(t)$。在这个例子中
$$s\boldsymbol{I}-\boldsymbol{A}=\begin{pmatrix}
s+2&-1\\-1&s+2
\end{pmatrix}\tag{45}$$
逆矩阵是
$$(s\boldsymbol{I}-\boldsymbol{A})^{-1}=\frac{1}{(s+1)(s+3)}\begin{pmatrix}
s+1&1\\1&s+2
\end{pmatrix}\tag{46}$$
将 $(42),(46)$ 代入 $(44)$ 得到
$$\boldsymbol{X}(s)=\begin{pmatrix}
\frac{2(s+2)}{(s+1)^2(s+3)}+\frac{3}{s^2(s+1)(s+3)}\\\frac{2}{(s+1)^2(s+3)}+\frac{3(s+2)}{s^2(s+1)(s+3)}
\end{pmatrix}\tag{47}$$
可以计算拉普拉斯逆变换求的 $\boldsymbol{x}(t)$。这可以通过部分分式法加上查表计算，也可以通过适当的计算机辅助计算得到。结果是
$$\boldsymbol{x}(t)=-\frac{2}{3}\begin{pmatrix}
1\\-1
\end{pmatrix}e^{-3t}+\begin{pmatrix}
2\\1
\end{pmatrix}e^{-t}+\begin{pmatrix}
1\\1
\end{pmatrix}te^{-t}+\begin{pmatrix}
1\\2
\end{pmatrix}t-\frac{1}{3}\begin{pmatrix}
4\\5
\end{pmatrix}\tag{48}$$
$(48)$ 是满足初始条件 $\boldsymbol{x}(0)=\boldsymbol{0}$ 的特解。这个结果和之前的特解有些许差异。为了获得 $(40)$ 的通解，必须将齐次方程组的通解 $(10)$ 加到 $(48)$ 上。

每一种方法都有优势和劣势。待定系数法无需积分，但是它限制了代数方程的形式。对角化方法需要计算转移矩阵的逆矩阵，然后求解一组解耦的一阶线性微分方程，最后利用矩阵的乘法得到解。对于埃尔米特矩阵，这种方法优势比较大，因为逆矩阵就是转置矩阵。拉普拉斯变换法涉及通过矩阵求逆来求转移矩阵，然后是乘法运算，最后确定所得表达式中每一项的逆变换。这种方法在处理包含间断项或脉冲项的强迫函数的时很有效。参数变分法是最通用的方法，不过它涉及求解一组系数是函数的线性代数方程的求解，还需要积分和矩阵乘法，也是最复杂的方法。对于很小的常系数方程组，比如这一节的例子，这些方法都能求解，并没有什么理由选择某一个方法。
