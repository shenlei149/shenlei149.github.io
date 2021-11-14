[Problem 518](https://projecteuler.net/problem=518 "Problem 518 - Project Euler")

$S(n) = \sum(a+b+c)$，其中 $(a,b,c)$ 满足以下三个性质的所有元组
1. $a,b,c$ 都是质数
2. $a < b < c < n$
3. $a+1, b+1, c+1$ 能组成等比数列

题目中给出了一个例子 $S(100) = 1035$ 可以用于测试。

求 $S(10^8)$

前一亿个数里面有 576 万个质数，显然，不可以接受 $O(n^2)$的算法。

为了描述方便，后面 $a,b,c$ 是题目中的 $a+1,b+1,c+1$。

$a,b,c$ 能组成等比数列，那么 $b^2=ac$。

给定一个 $a$，我们不能用遍历的方式去找 $b$，否则会是 $O(n^2)$ 的算法。我们需要一个更快的方式找到可能的 $b$。 

考虑这样一个问题，$a$ 的质因数里面有 4 个 $p$，那么 $b$ 的质因数里面至少有 2 个 $p$，那么 $a$ 和 $b$ 之间最少也相差 $p^2$；$a$ 的质因数里面有 $5$ 个 $q$，那么 $b$ 的质因数里面至少有 3 个 $q$，那么 $a$ 和 $b$ 之间最少也差着 $q^3$；同时考虑 $p$ 和 $q$ 的话，那么差值是 $p^2q^3$。有了这个差值，就能够大大减少内层循环的数目。通过计数器发现，内层循环大概是 8-9次。

问题转换到如何得到 $a$ 的质因数及其次数，这是一个比较简单的问题。
``` csharp
public static List<Tuple<long, int>> TrialDivisioFactor(long n, long[] primes)
{
    var results = new List<Tuple<long, int>>();
    int index = 0;
    while (true)
    {
        if (primes[index] != 0)
        {
            if (primes[n] != 0)
            {
                results.Add(new Tuple<long, int>(n, 1));
                break;
            }

            int count = 0;
            while (n % primes[index] == 0)
            {
                count++;
                n /= primes[index];
            }

            if (count != 0)
            {
                results.Add(new Tuple<long, int>(primes[index], count));
            }

            if (n == 1)
            {
                break;
            }
        }

        index++;
    }

    return results;
}
```
下面是完整的程序。`GenPrimes` 就是通过筛选法得到质数。
``` csharp
int end = 100000000;
long sum = 0;
Utils.GenPrimes(end, out long[] primes);
for (int i = 0; i < primes.Length; i++)
{
    if (primes[i] != 0)
    {
        var a = primes[i] + 1;
        var factors = Utils.TrialDivisioFactor(a, primes);
        var step = 1;
        foreach (var f in factors)
        {
            step *= (int)Math.Pow(f.Item1, (f.Item2 + 1) / 2);
        }
        for (long b = a + step; b < end; b += step)
        {
            var c = b * b / a;
            if (c >= end)
            {
                break;
            }

            if (primes[b - 1] != 0 && primes[c - 1] != 0)
            {
                sum += a + b + c - 3;
            }
        }
    }
}

return sum;
```
