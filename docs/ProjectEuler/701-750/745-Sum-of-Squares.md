[Problem 745](https://projecteuler.net/problem=745 "Problem 745 - Project Euler")

对任意正数 $n$，$g(n)$ 是最大的能被 $n$ 整除的完全平方数。

比如 $g(18)=9,g(19)=1$。

定义
$$S(N)=\sum_{n=1}^ng(n)$$
例如 $S(10)=24,S(100)=767$。

求 $S(10^{14})$，结果模 $1'000'000'007$。

$10^{14}$ 非常大，遍历肯定是不现实的。

但是小于这个值的完全平方数只有 $10^7$ 个，遍历几遍也足够快。如果知道了所有的完全平方数的根，那么其完全平方数及其整倍数 $n$ 对应的 $g(n)$ 就得到了。直观地看，有很多 $n$，其 $g(n)$ 都是1。

我们只需要计算每个完全平方数的根 $r$ 是多少个 $g(n)$ 的次数即可（即满足 $g(n)=r^2$ 的 $g$ 的个数），因为计算 $S(N)$ 的时候可以用完全平方数乘以次数。

如果两个完全平方数的根 $a,b$，有关系 $a=kb,k>1$，那么计算平方数及其整倍数的时候需要注意一下。$g(a^2)=a$，但是 $a^2$ 也能整除 $b^2$。所以我们需要从大的平方根向小的平方根遍历，对于后者 $b$，对应的 $g(n)$ 个数（即满足 $g(n)=b^2$ 的 $g$ 的个数），需要减去 $a$ 对应的 $g(n)$ 的个数。

首先定义几个常量
```csharp
private static readonly long N = 100_000_000_000_000;
private static readonly int NSqrt = 10_000_000;
private static readonly int Mod = 1_000_000_007;
```
下面计算完全平方数的根对应的 $g(n)$ 的个数。从大往小计算，同时对于更小的数 $i$，需要把其整数倍对应的 $j$ 的个数减去。
```csharp
var counts = new long[NSqrt + 1];
for (long i = counts.Length - 1; i >= 2; i--)
{
    long count = N / (i * i);
    for (long j = i * 2; j < counts.Length; j += i)
    {
        count -= counts[j];
    }

    counts[i] = count;
}
```
还有一个问题没有讨论，$g(n)=1$ 的个数。这个很简单，总数 $N$ 减去上面统计的次数之和即可。
```csharp
long sum = N;
for (int i = 2; i < counts.Length; i++)
{
    sum -= counts[i];
}
```
至此，`sum` 就是 $g(n)=1$ 的个数。现在加上完全平方数和对应次数的乘积。整个运算中，为了保持数很小（不超出 `long` 的范围），需要不停的取模。
```csharp
sum %= Mod;

for (long i = 2; i < counts.Length; i++)
{
    sum += counts[i] * i * i;
    sum %= Mod;
}
```
