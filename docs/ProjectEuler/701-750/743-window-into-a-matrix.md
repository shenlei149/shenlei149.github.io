原题链接：https://projecteuler.net/problem=743

一个矩阵的窗口就是一个子矩阵。

考虑一个元素要么是 0 要么是 1 的 $2\times n$ 矩阵。

令 $A(k, n)$ 是使得任意 $2\times k$ 的窗口元素和都是 $k$ 的矩阵的个数。

题目给了我们两个检测值：$A(3,9)=560,A(4,20)=1060870$

求 $A(10^8,10^{16})$

首先考虑 $k=3,n=9$ 的情况。

任意一个 $2\times 3$ 的窗口其元素和都要是 3。

每一列两个单元格，其中任意一个是单元格是 1 另一个是 0，这样满足条件，所以有 $2^9$ 方种可能。

还有一种情况，第一个窗口中有一列全 1，一列全 0，一列一个 1 一个 0，然后后续循环，即每 3 列重复一次，对于有 0 有 1 的列，可以额外多出一倍的选择的可能性。这样有
$$\begin{pmatrix}
3\\1
\end{pmatrix}\begin{pmatrix}
2\\1
\end{pmatrix}2^3$$
上式三个部分，意思分别是：第一个是三列里面选择一个全 1 的，第二个是其余两列里面选择一个全 0 的，最后有一列（会重复三次，$9/3=3$）可以任意选择一个放 1。

两者相加得到了题目给出的 560。

抽象来看。如果对于宽度为 $k$ 的窗口，有 $i$ 个全 1 的列，那么也刚好有 $i$ 个全 0 的列。这些列的选择个数是
$$\begin{pmatrix}
k\\i
\end{pmatrix}\begin{pmatrix}
k-i\\i
\end{pmatrix}$$
也可以这么想，先选择 $2i$ 列，然后再从其中选择 $i$ 个全 1 列。
$$\begin{pmatrix}
k\\2i
\end{pmatrix}\begin{pmatrix}
2i\\i
\end{pmatrix}$$
很容易验证两者是等价的。

接着考虑剩余的 $k-2i$ 列，这些列都可以任意一个单元格放置 1。一个有 $n/k$ 个循环。所以这些列的选择个数是
$$2^{\frac{n}{k}(k-2i)}$$

$i$ 的取值范围是 $[0, \frac{k}{2}]$，求 $i$ 求和得到结果
$$\sum_{i=0}^{k/2}\begin{pmatrix}
k\\i
\end{pmatrix}\begin{pmatrix}
k-i\\i
\end{pmatrix}2^{\frac{n}{k}(k-2i)}$$

将 $k=4,n=20$ 代入上面的公式，可以得到题目给出的结果。

最后求解的 $k=10^8$，所以求和号里面的数值必须在 $O(1)$ 时间内给出，最多是 $O(\log k)$，其他情况就会导致求解的时间非常大。分别独立的看求和号里面的三项。如果 $i$ 和 $i+1$ 的值有递推关系的话（不管从 $i$ 到 $i+1$ 还是反之，都是可以的，比如第三项从后往前推到比较容易，乘就可以了），就容易在几步（常量时间）时间计算。

不过这里遇到一个问题，对于
$$\begin{pmatrix}
k\\i
\end{pmatrix}$$
从 $i$ 推导到 $i+1$，要乘以一个数再除以一个数，由于计算的中间需要模 $1,000,000,007$，那么这里不能保证整除。虽然原始的值肯定是整数，但是太大了，怎么保存是一个问题，而且全部计算需要 $O(k)$ 的时间，明显不可行。
