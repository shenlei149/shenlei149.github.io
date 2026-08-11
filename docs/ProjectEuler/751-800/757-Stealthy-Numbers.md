原题链接：https://projecteuler.net/problem=757

如果存在四个整数 $a,b,c,d$，满足 $a+b=c+d+1$，$ab=cd=N$，那么 $N$ 被称为 `Stealthy` 的。比如 $36=4\times 9=6\times 6$ 就是 `Stealthy` 数。

题目给出不超过 $10^6$ 的数中有 2851 个数是 `Stealthy` 数。

求不超过 $10^{14}$ 的数中有多少个 `Stealthy` 数。

$10^{14}$ 非常大，平方根是 $10^{7}$，这是一个可以遍历的范围。使用构造法来找到 $a,b$，那么需要两层 `for` 循环，时间肯定不可接受。

在尝试了几种方法之后，还是从数学上分析 $a,b,c,d$ 的关系是一个靠谱的入手手段。

由于 $ab=cd$，可以假定 $a<c\leq d<b$。利用题目中的两个等式，可以得到
$$\begin{aligned}
N&=(c-a+1)ac/(c-a)\leq \text{MAX}\\
b&=(c-a+1)c/(c-a)\\
d&=(c-a+1)a/(c-a)
\end{aligned}$$
由于 $b,d$ 是整数，且 $c-a+1$ 与 $c-a$ 互斥，那么
$$a\operatorname{mod}(c-a)=c\operatorname{mod}(c-a)=0$$
令 $c-a=q$，那么 $a=nq$，其中 $n$ 是正整数。那么
$$\begin{aligned}
b&=(q+1)*(n+1)\\
c&=q*(n+1)\\
d&=(q+1)*n
\end{aligned}$$

$n$ 从 $1$ 开始，最大不能让 $ab$ 超过 $\text{MAX}$。

这里遍历 $q$，即代码中的 `delta`。
```csharp
var n = GetMaxN(delta);
if (n * (n + 1) * delta * (delta + 1) > MAX)
{
    n--;
}

return n - delta + 1;
```
函数 `GetMaxN` 是
```csharp
private static long GetMaxN(long delta)
{
    return (long)Math.Sqrt(MAX / (delta * (delta + 1)));
}
```

运行 `MAX = 1_000_000` 看看对不对，程序给出的答案比 2851 多很多。经过 debug，发现上述分析假定 $c\leq d$，但是上面把 $c>d$ 的情况也包含了进去，相当于重复计算了一次。从 $c\leq d$ 很容易得到 $n\geq q$，于是在 $n$ 之后加上一个判断。
```csharp
if (n < delta) // ensure c\leq d
{
    return;
}
```

结果比 2851 稍大一点点，很接近了。又是一顿 debug，发现这样一个问题：$10 * 18 = 9 * 20 = 180$，对应两组 $a,b,c,d$，即 $10,18,12,15$ 和 $9,20,10,18$，它俩对应的 $q$ 分别是 $2$ 和 $1$，但是都对应着 $180$ 这个 `Stealthy` 数，重复计算了。

想了半天，没有太好的办法，只能稍微暴力一点了，搞了一个 `HashSet<long>` 存放所有的数，在计算到 $n$ 之后，真实的计算出 $N$ 放进去去重。
```csharp
for (long m = delta; m <= n; m++)
{
    long a = m * delta;
    long b = (m + 1) * (delta + 1);
    StealthyNumbers.Add(a * b);
}
```
至此结果就对了。

当运行 `MAX = 100_000_000_000_000` 时，有点慢，差不多 10s 才能给出结果，主要是当 `delta` 比较小的时候，满足的数都是百万量级的，需要放到 `HashSet<long>` 中，很耗时。
