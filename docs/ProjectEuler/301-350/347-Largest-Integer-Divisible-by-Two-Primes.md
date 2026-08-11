[Problem 347](https://projecteuler.net/problem=347 "Problem 347 - Project Euler")

在所有小于 100 的整数中，能被两个质数整除的最大整数是 96，$96 = 2^5 \times 3$。我们定义函数 $M(p,q,N)$：对于不同的两个质数 $p, q$，$M(p,q,N)$ 是小于等于 $N$ 中能被 $p,q$ 整除的最大整数，如果不存在，$M(p,q,N)=0$。比如
$$M(2,3,100)=96$$
$$M(3,5,100)=75$$
后者而不是 90，因为 90 能被 2, 3, 5 整除。

$M(2,73,100)=0$ 是因为 $2\times 73=146<100$，小于 100 的数里面没有数能同时被 2 和 73 整除。

$S(N)$ 是对所有不同的 $M(p,q,N)$ 求和。

题目最后是求 $S(10'000'000)$。

解题思路很清晰，只是要尽可能早的排除不可能的质数组合。

首先获取 0 到 5 000 000 之间的质数，因为大于 5 000 000 的质数乘以最小的质数 2 也会大于 10 000 000，$M=0$，不用考虑了。

然后就是组合每一个可能的指数对 $p,q$，获得 $M(p,q,10'000'000)$。

两层循环遍历 $p,q$。不妨设p比较小，用遍历所有的质数吗？显然不用，当 $p$ 大于 10 000 000 的平方时，$p,q$ 的乘积就会大于 10 000 000，$M=0$。

$q$ 也不用遍历所有大于 $p$ 的质数，一旦 $p,q$ 的乘积大于 10 000 000，就可以停止了。

给定 $p,q$，如何计算 $M$ 呢？这里使用了暴力法，就是先通过 $\log_p, 10 000 000$ 和 $\log_q 10 000 000$ 计算出可能的最大指数，然后两层循环，遍历所有可能的情况，得到小于等于 $M$ 且能被 2 个质数整除的最大值。

大约 1s多一点就能得到结果了。

最后，贴一下我的代码：
``` csharp
const int MAX = 10000000;
public static long GetAnswer()
{
	long sum = 0;
	var primes = Utils.GenPrimes(MAX / 2);
	for (int i = 0; primes[i] < Math.Sqrt(MAX); i++)
	{
		for (int j = i + 1; j < primes.Length; j++)
		{
			long p = primes[i];
			long q = primes[j];
			if (p * q < MAX)
			{
				sum += GetM(p, q);
			}
			else
			{
				break;
			}
		}
	}

	return sum;
}

private static long GetM(long p, long q)
{
	var max = 0L;
	var e1Max = (int)(Math.Log(MAX) / Math.Log(p));
	var e2Max = (int)(Math.Log(MAX) / Math.Log(q));
	for (int e1 = 0; e1 < e1Max; e1++)
	{
		for (int e2 = 0; e2 < e2Max; e2++)
		{
			var product = Math.Pow(p, e1 + 1) * Math.Pow(q, e2 + 1);
			if (product <= MAX && product > max)
			{
				max = (long)product;
			}
		}
	}

	return max;
}
```
