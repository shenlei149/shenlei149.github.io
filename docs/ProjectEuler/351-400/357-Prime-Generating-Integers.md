[Problem 357](https://projecteuler.net/problem=357 "Problem 357 - Project Euler")

30 能够分解得到的因数分别是 1,2,3,5,6,10,15,30

对于 30 的每一个因数 $d,d+30/d$ 都是一个质数。

求不超过 100 000 000，且满足性质每一个因数 $d$，$d+n/d$ 都是质数的 $n$ 的和。

分析一下什么样的 $n$ 满足上述性质。

$n$ 能够分解成一系列的因数，记做 $d_1, d_2, d_3, \cdots, c_3, c_2, c_1$，其中 $c_1 \times d_1 = n \Rightarrow d_1 = n / c_1$，也就是说只要分析前面一半，小于 $\sqrt{n}$ 的因数即可。

第一个因数肯定是 1，那么 $1 + n$ 是质数。所以先使用筛选法得到小于 100 000 000 的质数，然后都减去 1，得到的数才可能满足题意。

假如 $n$ 能整除 4，也就是说 $n = 4k$，第二个因数是 2，且 $n / 2 = 2k$，$2 + 2k$ 是偶数，肯定不是质数，所以我们把能整除 4 的数排除。

同理，$n$ 也不能被 9 整除。但是剩余数能被 9 整除的也就只有几十万了，对于二百八十多万来说，差距不大，就没有用这条性质再过滤数据了。

``` csharp
long[] primes = Utils.GenPrimes(100000000);
var candidates = primes.Where(p => p != 0 && (p - 1) % 4 != 0)
    .Select(p => p - 1).ToArray();
```

现在，从剩余的 $n$ 中看有多少是满足题意的。正如上面所分析的，只要分析前面一半，小于 $\sqrt{n}$ 的因数即可。

``` csharp
long result = 0;
foreach (int n in candidates)
{
    bool meet = true;
    int h = (int)Math.Sqrt(n);
    for (int i = 2; i <= h; i++)
    {
        if (n % i != 0)
        {
            continue;
        }

        int d = n / i;
        if (primes[d + i] == 0)
        {
            meet = false;
            break;
        }
    }

    if (meet)
    {
        result += n;
    }
}
```
