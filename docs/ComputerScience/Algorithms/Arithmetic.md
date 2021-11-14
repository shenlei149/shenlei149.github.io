## Karatsuba 乘法
假定 $x,y$ 是 $n$ 位数，不妨假定 $n$ 是偶数，那么 $x,y$ 可以拆成两个部分。
$$x=10^{n/2}a+b$$
$$y=10^{n/2}c+d$$
那么
$$\begin{aligned}
xy&=(10^{n/2}a+b)(10^{n/2}c+d)\\
&=10^nac+10^{n/2}(ad+bc)+db
\end{aligned}$$
其中
$$ad+bc=(a+b)(c+d)-ac-bd$$
那么可以再省去一次乘法。

Karatsuba 就是根据上述的性质来实现乘法的。整个算法是递归的。

如果 `n == 1`，那么直接计算 `x * y` 并返回。

否则按照之前的描述分别拆成 `a, b` 和 `c, d` 两个部分。分别计算 `ac = a * c` `bd = b * d`，为了方便描述中间部分，引入一些变量，`p = a + b, q = c + d`，那么 `adbc = p * q - ac - bd`。那么最终结果是 `pow(10, n) * ac + pow(10, n / 2) * adbc + bd`。

具体实现可以参考 [BigInteger.cc](https://github.com/shenlei149/algorithms-data-structures/blob/1f66d6cc237214d89a025a5714497ee0370f630f/src/numeric/BigInteger.cc#L200)。

## Strassen 矩阵乘法
假定 $X,Y$ 是两个 $n\times n$ 的矩阵，乘积 $Z=XY$，第 $i$ 行第 $j$ 列的值是
$$z_{ij}=\sum_{k=1}^n x_{ik}y_{kj}$$
最朴素的算法就是对 $i,j,k$ 遍历，三层 `for` 循环，时间复杂度是 $O(n^3)$。

下面考虑分治法，将 $X,Y$ 拆成四个 $\frac{n}{2}\times\frac{n}{2}$ 的小矩阵
$$X=\begin{pmatrix}
A&B\\C&D
\end{pmatrix},Y=\begin{pmatrix}
E&F\\G&H
\end{pmatrix}$$
那么
$$XY=\begin{pmatrix}
AE+BG&AF+BH\\
CE+DG&CF+DH
\end{pmatrix}$$
需要递归的调用八次矩阵乘法，算法的复杂度仍旧是 $O(n^3)$。

如果我们能够省去一次递归调用，那么性能的提升不仅仅是 12.5%。这就是 Strassen 矩阵乘法的巧妙之处。首先执行七次递归调用
$$\begin{aligned}
P_1&=A(F-H)\\
P_2&=(A+B)H\\
P_3&=(C+D)E\\
P_4&=D(G-E)\\
P_5&=(A+D)(E+H)\\
P_6&=(B-D)(G+H)\\
P_7&=(A-C)(E+D)
\end{aligned}$$
那么
$$\begin{aligned}
XY&=\begin{pmatrix}
AE+BG&AF+BH\\
CE+DG&CF+DH
\end{pmatrix}\\
&=\begin{pmatrix}
P_5+P_4-P_2+P_6&P_1+P_2\\
P_3+P_4&P_1+P_5-P_3-P_7
\end{pmatrix}
\end{aligned}$$
下面验证各个递归调用
$$\begin{aligned}
P_5+P_4-P_2+P_6=&AE+AH+DE+DH\\
&+DG-DE\\
&-AH-BH\\
&+BG+BH-DG-DH\\
=&AE+AH+DH-AH-BH+BG+BH-DH\\
=&AE+BG
\end{aligned}$$
$$\begin{aligned}
P_1+P_2&=AF-AH+AH+BH\\
&=AF+BH
\end{aligned}$$
$$\begin{aligned}
P_3+P_4&=CE+DE+DG-DE\\
&=CE+DG
\end{aligned}$$
$$\begin{aligned}
P_1+P_5-P_3-P_7=&AF-AH\\
&+AE+AH+DE+DH\\
&-CE-DE\\
&-AE-AF+CE+CF\\
=&AF+DH-CE-AF+CE+CF\\
=&CF+DH
\end{aligned}$$
