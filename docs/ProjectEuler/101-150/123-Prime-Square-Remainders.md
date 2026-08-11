[题目链接](https://projecteuler.net/problem=123 "Problem 123 - Project Euler")

$p_n$ 表示第 $n$ 个质数，$r$ 是 $(p_n-1)^n+(p_n+1)^n$ 模 $p_n^2$ 的余数。

求满足余数 $r$ 超过 $10^{10}$ 的 $n$ 的最小值。

这问题的关键在于找到一个简单的方式来表达 $r$。我们先把 $(p_n-1)^n$ 展开看看。
$$(p_n-1)^n=\binom{n}{n}p_n^n+\binom{n}{n-1}(-1)^1p_n^{n-1}+\cdots+\binom{n}{2}(-1)^{n-2}p_n^2+\binom{n}{1}(-1)^{n-1}p_n+\binom{n}{0}(-1)^n$$
除了最后两项，其他能被 $p_n^2$ 整除。$(p_n+1)^n$ 类似。所以
$$r=n(-1)^{n-1}p_n+(-1)^n+n(1)^{n-1}p_n+(1)^n$$
$n$ 为偶数时
$$r=n(-1)p_n+1+n(1)p_n+1=2$$
显然不大于 $10^{10}$。

$n$ 为奇数时
$$r=np_n-1+np_n+1=2np_n$$
我们只需要考察 $n$ 是奇数即可，同时，给定 $n$，我们能够很快的计算出 $r$ 的值。

写代码得到答案。
``` csharp
var primes = Utils.GenPrimes(1000000).Where(p => p != 0).ToList();
int n = 7039;
for (; n < primes.Count; n += 2)
{
    var r = 2 * n * primes[n - 1];
    if (r > 10000000000)
    {
        break;
    }
}
```
这里说明两个事情：
1. 7037 是题目给出的数，其对应的 $r$ 第一次超过了 $10^9$，所以我们从 7039 开始找即可；其实多算 7000 多个数也消耗不了啥时间。
2. `primes[n - 1]` 减一的原因是数组从 0 开始，而不是 1。
