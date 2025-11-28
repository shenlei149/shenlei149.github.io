## 数学归纳法
数学归纳法证明一个命题 $P$ 分成两个步骤。

* 基础情形（`base case`）：证明命题对起始值成立，通常证明 $P(0)$ 或 $P(1)$ 成立。
* 归纳步骤（`inductive step`）：假定对任意自然数 $k$ $P(k)$ 成立，需要证明 $P(k+1)$ 也成立。

一旦完成了这两个步骤，那么对于任意自然数 $n$ 命题 $P(n)$ 都成立。

## 离散概率
### 样本空间
样本空间（`sample spaces`）是随机过程中所有可能结果的集合，一般用希腊字母 $\Omega$ 表示。比如掷骰子问题中 $\Omega=\{1,2,3,4,5,6\}$。

样本空间中每一个元素 $i$ 都有一个非负概率 $p(i)$ 表示其发生的概率。所有结果的概率之和为零
$$\sum_{i\in\Omega}p(i)=1$$

如果所有可能的结果发生的概率相等，那么是均匀分布（`uniform distribution`），每个元素对应的概率是
$$p(i)=\frac{1}{|\Omega|}$$

### 事件
事件（`event`）是样本空间的子集，$S\in\Omega$。事件的概率是
$$P(S)=\sum_{i\in S}p(i)$$

### 随机变量
随机变量（`random variable`）是随机过程的结果到某个数的映射到一个数值上。样本空间 $\Omega$ 上的函数 $X:\Omega\to\R$，输入是 $i\in\Omega$，对应的输出 $X(i)$ 是一个值。

### 期望
期望（`expectation`）是随机变量 $X$ 的加权平均值，权重是不同结果的概率，写作 $E[X]$。如果随机过程重复多次，$E[X]$ 是 $X$ 的平均值，比如掷骰子问题中 $E[X]=3.5$。
$$E[X]=\sum_{i\in\Omega}p(i)X(i)$$

### 期望的线性性质
期望的线性性质（`linearity of expectation`）是很重要的很有用的性质，核心思想就是和的期望等于期望的和。下面是正式的数学表示。比如扔两个骰子，需要求点数的期望值，一种方法是计算 36 种样本的概率乘以点数，另一种简单的方式是将其看做两个单独的随机变量，即每个骰子点数的期望值，即 3.5，然后相加得到 7。

令 $X_1,X_2,\cdots,X_n$ 是定义在 $\Omega$ 上的随机变量，$a_1,a_2,\cdots,a_n$ 是实数，那么
$$E\bigg[\sum_{j=1}^n a_jX_j\bigg]=\sum_{j=1}^n a_jE[X_j]$$
下面是证明过程。
$$\begin{aligned}
\sum_{j=1}^n a_jE[X_j]&=\sum_{j=1}^n a_j\bigg(\sum_{i\in\Omega}p(i)X_j(i)\bigg)\\
&=\sum_{j=1}^n \bigg(\sum_{i\in\Omega}a_jp(i)X_j(i)\bigg)\\
&=\sum_{i\in\Omega} \bigg(\sum_{j=1}^na_jp(i)X_j(i)\bigg)\\
&=\sum_{i\in\Omega} p(i)\bigg(\sum_{j=1}^na_jX_j(i)\bigg)\\
&=E\bigg[\sum_{j=1}^na_jX_j\bigg]\\
\end{aligned}$$
