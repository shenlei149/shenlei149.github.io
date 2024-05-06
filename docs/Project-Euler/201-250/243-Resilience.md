[Problem 243](https://projecteuler.net/problem=243 "Problem 243 - Project Euler")

分子小于分母的分数被称为真分数。比如 $d=12$，那么有 11 个真分数
$$\frac{1}{12},\frac{2}{12},\frac{3}{12},\frac{4}{12},\frac{5}{12},\frac{6}{12},\frac{7}{12},\frac{8}{12},\frac{9}{12},\frac{10}{12},\frac{11}{12}$$
其中分子分母不能约分的分数称为最简分数，用 $R(d)$ 来表示最简分数的个数与 $d-1$ 之比，比如 $R(12)=\frac{4}{11}$，事实上，$d=12$ 是最小的整数满足 $R(d)<\frac{4}{10}$的。

求最小的 $d$，使得 $R(d)<\frac{15499}{94744}$。

一开始，我使用了暴力算法遍历 $d$，但是大约几十秒过去了，$d$ 都到了大几百万还是不满足题意。所以需要先推理简化问题再写程序。

令 $d$ 有 $k$ 个质因数，那么 $d$ 可以写作
$$d=p_1^{q_1}p_2^{q_2}\cdots p_k^{q_k}$$
和 $d$ 互斥且小于 $d$ 的数的个数是
$$(p_1-1)p_1^{q_1-1}(p_2-1)p_2^{q_2-1}\cdots (p_k-1)p_k^{q_k-1}$$
那么
$$\begin{aligned}
R(d)&=\frac{(p_1-1)p_1^{q_1-1}(p_2-1)p_2^{q_2-1}\cdots (p_k-1)p_k^{q_k-1}}{d-1}\\
&=\frac{d\cdot \frac{p_1-1}{p_1}\frac{p_2-1}{p_2}\cdots\frac{p_k-1}{p_k}}{d-1}\\
&=\frac{d}{d-1}\frac{(p_1-1)(p_2-1)\cdots (p_k-1)}{p_1p_2\cdots p_k}
\end{aligned}$$
因为前面我已经知道 $d$ 很大，那么 $\frac{d}{d-1}\to 1$。所以要先找到一些质数，使得
$$\frac{(p_1-1)(p_2-1)\cdots (p_k-1)}{p_1p_2\cdots p_k}<\frac{15499}{94744}$$
且再多一个质数就会不满足这个不等式。

要使得 $d$ 最小化，那么这些质数就是从 2 开始的一些质数，然后$d$ 就是这些质数之积。当然，这个时候的 $d$ 可能不满足题意，因为忽略了系数 $\frac{d}{d-1}$，但是 $d$ 的若干倍（倍数小于下一个质数）就会满足题意的。

下面是实现代码，其中 $c,d$ 要使用 `long` 的原因是 $p$ 很小但乘积可能超过 `int` 的表示范围。
``` csharp
int[] primes = Utils.GenPrimes(50).Where(l => l != 0).Select(Convert.ToInt32).ToArray();
long c = 1;
long d = 1;
foreach (var p in primes)
{
    c *= p - 1;
    d *= p;
    if (c * 94744L < d * 15499L)
    {
        break;
    }
}

for (int i = 2; i <= primes.Last(); i++)
{
    long n = d * i;
    if (c * n * 94744L < d * (n - 1) * 15499L)
    {
        return n.ToString();
    }
}
```
