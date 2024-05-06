这是欧拉项目的第五百个题目！

[Problem 500](https://projecteuler.net/problem=500 "Problem 500 - Project Euler")

题目描述很简单，120 个有 16 个因数，是最小的拥有 16 个因数的数字。求最小有 $2^{500500}$ 个因数的数字，因为这个值相当大，要求给出模 500500507 的结果。

首先，我们要明白，如何求一个数有多少个因数？

这里需要用到 [Euler's totient function](https://en.wikipedia.org/wiki/Euler%27s_totient_function)，这里不得不提一句，出题人很用心，第 500 题，一个有里程碑意义的题目，需要用到 Euler 本人的研究成果。

根据这个定理，把前 500500 个质数相乘，那个数字就有 $2^{500500}$ 次方个因数，假定这是第一个数。但是肯定不是最小的，因为当前 2 出现了一次，对因数的个数贡献是 2 的 1 次方，那么再乘以 2 个 2，2 的次数是 3 次，对因数的贡献是 2 的平方，，那么不用乘以第 500500 个质数，就能使得因数个数是 $2^{500500}$。显然 2 的平方比 500500 小，那么把之前的数除以第 500500 个质数，再除以 4，得到一个更小的满足题意的数，假定这时第二个数。要让 2 的贡献再过一次方，达到 2 的 3 次方，那么需要再多 4 个 2，使得有 7 个 2，显然 4 个 2 的乘积比第 500504 个质数小很多。就这样子继续下去。然后继续考虑 3、5、7 等等。

大致思路如此，具体的看代码。这里出现了几个魔幻数，比如第二个 `for` 循环，手算了前多少个质数的平方比后面的质数小。
``` csharp
const long mod = 500500507;
long answer = 1;
var primes = Utils.GenPrimes(8000000).ToList();

for (int i = 0; i < 500084; i++)
{
	answer *= primes[i];
	answer %= mod;
}

for (int i = 0; i < 396; i++)
{
	answer *= (long)Utils.Pow((int)primes[i], 2);
	answer %= mod;

}

for (int i = 0; i < 15; i++)
{
	answer *= (long)Utils.Pow((int)primes[i], 4);
	answer %= mod;
}

for (int i = 0; i < 4; i++)
{
	answer *= (long)Utils.Pow((int)primes[i], 8);
	answer %= mod;
}

answer *= (long)Utils.Pow((int)primes[0], 16);

return answer % mod;
```