[Problem 429](https://projecteuler.net/problem=429 "Problem 429 - Project Euler")

整数 $n$ 的元因数（`unitary divisor`）$d$ 满足 $\gcd(d, n/d) = 1$。

$4! = 24$ 的元因数有 1，3，8，24，它们的平方和是 $1^2 + 3^2 + 8^2 + 24^2 = 650$。

$S(n)$ 表示 $n$ 的元因数的平方和。求 $S(100 000 000!)$ 模 1 000 000 009。

将 $n$分解质因数，可以写作
$$n = p_1^{k_1} + p_2^{k_2} + \cdots + p_m^{k_m}$$
那么元因数就是完全占有某些质数，比如 $p_1^{k_1}$ 和 $p_2^{k_2} + \cdots + p_m^{k_m}$ 就是一对元因数。

有多少元因数呢？$2^m$ 个，一个很典型的排列组合问题。

下面看看平方和有什么规律。先看几个简单的。令 $q_i=p_i^{k_i}$记作。如果有两个质因数，那么平方和是
$$
\begin{aligned}

S &= 1 + q_1^2 + q_2^2 + (q_1q_2)^2\\
  &= (1+q_1^2)(1+q_2^2)
\end{aligned}
$$
有三个质因数呢？
$$
\begin{aligned}
S &= 1 + q_1^2 + q_2^2 + q_3^2 + (q_1q_2)^2 + (q_1q_3)^2 + (q_2q_3)^2 + (q_1q_2q_3)^2\\
  &= (1+q_1^2)(1+q_2^2)(1+q_3^2)
\end{aligned}
$$
不难猜测到
$$S = (1+q_1^2)(1+q_2^2)...(1+q_m^2)$$

假定已经有了质因数的分解，那么很容易就能求出平方和了。
``` csharp
long ret = 1;
foreach (var pair in factors)
{
    var p = pair.Key;
    var k = pair.Value * 2;

    long r = PowMod(p, k, m);

    ret *= (r + 1);
    ret %= m;
}

// return p^k mod m
static long PowMod(long p, int k, long m)
{
    if (k == 0)
    { return 1; }

    if (k == 1)
    { return p % m; }

    var half = PowMod(p, k / 2, m);
    if (k % 2 == 0)
    {
        return (half * half) % m;
    }
    else
    {
        return (((half * half) % m) * p) % m;
    }
}
```
现在的问题是如何得到 $100 000 000!$ 的质因数，需要知道有多少个 2，多少个 3，多少个 5 等等。[Legendre's formula](https://en.wikipedia.org/wiki/Legendre%27s_formula) 给出了 $n!$ 分解质因数的方法，这个方法简单易懂。得到小于 100 000 000 的所有质数（筛选法），然后循环计算指数就可以了。
``` csharp
var factors = new Dictionary<long, int>();
foreach (var p in primes)
{
    int i = 1;
    int power = 0;
    while (true)
    {
        var cur = n / (long)Math.Pow(p, i);
        if (cur == 0)
        {
            break;
        }

        power += (int)cur;
        i++;
    }

    factors[p] = power;
}
```
