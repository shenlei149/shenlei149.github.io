[Problem 684](https://projecteuler.net/problem=684 "Problem 684 - Project Euler")

$s(n)$ 的定义是数字之和是 $n$ 的最小值，比如 $s(10)=19$。

$S(k)=\sum_{n=1}^ks(n)$，题中给出 $S(20)=1074$，可以用于检测程序是否基本正确。

$f_i$是斐波那契数列。问题是求 $\sum_{i=2}^{90}S(f_i)$ 模 1000000007。

斐波那契数列以指数速度增长，所以暴力法肯定是不行的（$f_{90}$快接近 `long` 的表示上限了），那么需要在常数或者对数时间复杂度的算出 $S(k)$ 的算法。

$s(n)$ 其实很容易想，数字最小，那么就是高位的值越小越好，那么低位的值都是9即可。

有了 $s(n)$，我们就可以推导一下 $S(k)$ 了。

$s(k)$ 可以写作是 $r999999\ldots$，其中 $r$ 是 $k \% 9$，后面 9 的个数是 $k/9$，不妨记作 $L=k/9$。

那么 $S(k)$ 可以分成两个部分。第一部分（记作 $F$）是所有长度和 $s(k)$ 一样的数之和，第二部分（记作 $R$）是数字长度为 $1,2,3\ldots L$ 的数之和。
$$\begin{aligned}
F&=10^L\sum_{i=1}^r i+(10^L-1)r\\
&=10^L\frac{r^2+3r}{2}-r
\end{aligned}$$
长度为 $L$ 的数之和和上式类似，其中 $r=9$，$L$ 减一。
$$R(L)=54\times 10^{L-1}-9$$
所以
$$R=\sum_{i=1}^L R(l)=54\times\frac{10^L-1}{10-1}-9L=6\times10^L-6-9L$$
由于 $f_{90}$ 很大，那么 $L$ 也很大，$10^L$ 非常大，所以需要利用分治法以对数时间复杂度快速算出其模 1000000007 的值。
``` csharp
private static long GetRemainder(long l)
{
    if (l < 10)
    {
        return (long)Math.Pow(10, l);
    }

    var r = l % 2;
    var half = GetRemainder(l / 2);
    half = (half * half) % Mod;
    if (r == 0)
    {
        return half;
    }
    else
    {
        return (half * 10) % Mod;
    }
}
```
有了公式，那么就可以快速计算出 $S(k)$ 的值了。
``` csharp
private long S(long f)
{
    long L = f / 9;
    long r = f % 9;

    long remainder = GetRemainder(L);

    long first = (r * r + 3 * r) / 2 * remainder - r;
    long second = 6 * remainder - 6 - 9 * L % Mod;

    return (first + second) % Mod;
}
```
进而，很容易得出要求的数。
``` csharp
var fibonacci = new List<long>();
long f0 = 0;
long f1 = 1;
for (int i = 2; i <= 90; i++)
{
    long f2 = f0 + f1;
    fibonacci.Add(f2);

    f0 = f1;
    f1 = f2;
}

long result = 0;
foreach (var f in fibonacci)
{
    result += S(f);
}

return result %= Mod;
```
