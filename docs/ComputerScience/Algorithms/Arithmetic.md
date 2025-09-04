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
