[Problem 124](http://projecteuler.net/problem=124 "Problem 124 - Project Euler")

$n$ 的根函数，$\operatorname{rad}(n)$，定义为其所有不同质因子之积。例如，$504 = 2^3 \times 3^2 \times 7$, 所以 $\operatorname{rad}(504) = 2 \times 3 \times 7 = 42$

令 $E(k)$ 是求得了前 $n$ 个 $\operatorname{rad}(n)$ 之后排序，第 $k$ 个所对应的 $n$。

$n$ 从 1 到 100,000，求 $\operatorname{rad}(n)$，问题是要求 $E(10000)$。

这道题的第一步是分解质因数。

为此，我先生成了一个数组 `primes`，放了从 1 到 100,000 中的质数部分，从小到大排列，这很重要，求质因数用得着。

拿到所有的质因数之后，求积就可以得到对应的 $\operatorname{rad}$。

对 $\operatorname{rad}(n)$ 排序，为了简单起见，我把 $n$ 和 $\operatorname{rad}(n)$ 打包在一起，并且实现了 `IComparable`，便于之后的排序。排序规则是以 $\operatorname{rad}$ 为主键，$\operatorname{rad}$ 一样的情况下，按照 $n$ 排序。
```csharp
public class NAndRadN : IComparable
{
    public int n;
    public long rad;

    public int CompareTo(object that)
    {
        var t = that as NAndRadN;
        if (this.rad == t.rad)
        {
            return this.n.CompareTo(t.n);
        }
        else
        {
            return this.rad.CompareTo(t.rad);
        }
    }
}
```

准备工作做好之后，写一个 `for`循环，把 1 到 100,000 的 $\operatorname{rad}$ 算出来。排序，找到第 10,000 个数字，输出即可。
``` csharp
for (long i = 1; i <= 100000; i++)
{
    var fac = Utils.Factorize(i, primes).Keys;
    long p = 1;
    foreach (var f in fac)
    {
        p *= f;
    }

    results[i - 1] = new NAndRadN { N = i, Rad = p };
}

var rads = results.ToArray();
Array.Sort(rads);
return rads[9999].n;
```
