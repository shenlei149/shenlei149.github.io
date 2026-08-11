[Problem 329](https://projecteuler.net/problem=329)

一只懂质数的青蛙站在编号为 1-500 的方块内，它等概率的向左或者向右跳，当然不能出 1-500 这些方块，如果到达了边缘就只能向另外一个方向跳。

如果方块的编号是质数，那么有 2/3 的概率呱出 `P` (PRIME)，1/3 的概率呱出 `N` (NOT PRIME)；反之如果编号不是质数，那么呱出 `P` 和 `N` 的概率分别是 1/3 和 2/3。

如果它从随机的一点出发（等概率），发出 `PPPPNNPPPNPPNPN` 的概率是多少呢？使用最简分数给出答案。

使用一个 `List` 放一系列数值对，每一对表示概率的分子和分母。从 1-500 个编号出发，将每种情况的概率存起来。
``` csharp
var probabilities = new List<(long, long)>();
for (int i = 1; i <= 500; i++)
{
    GetProbabilityAt(i, probabilities);
}
```
如果具体计算每种情况的概率呢？使用递归，假定第 $k$ 步从第 $i$ 个位置跳到第 $j$ 的位置，在位置 $i$ 的时候概率是 $p$，接下来考虑方向对应的概率和到了位置 $j$ 之后根据题目得到呱出想要的字母的概率，两个方向的概率之积再乘以 $p$ 就是到达位置 $j$ 的概率，直至呱完所有的字母停下来就是一个可能的值，反复递归就得到了一系列的概率，求和就是对应每种情况的概率。不过具体的实现没有求和，因为最后把 500 个系列的概率一起就和就可以了。
``` csharp
private void GetProbabilityAt(int i, List<(long, long)> p)
{
    GetProbabilityAt(i, 0, 1, 1, p);
}

private void GetProbabilityAt(int i, int step, long numerator, long denominator, List<(long, long)> result)
{
    var (n, d) = GetProbabilityWithCroak(i, croaks[step]);
    numerator *= n;
    denominator *= d;

    step++;

    if (step == croaks.Length)
    {
        result.Add((numerator, denominator));
        return;
    }

    if (i == 1)
    {
        GetProbabilityAt(i + 1, step, numerator, denominator, result);
        return;
    }

    if (i == 500)
    {
        GetProbabilityAt(i - 1, step, numerator, denominator, result);
        return;
    }

    GetProbabilityAt(i + 1, step, numerator, denominator * 2, result);
    GetProbabilityAt(i - 1, step, numerator, denominator * 2, result);
}
```
`GetProbabilityWithCroak` 计算达到每一个方块对应的概率，根据题意
```csharp
private static (long, long) GetProbabilityWithCroak(int i, char p)
{
    if (p == 'P')
    {
        if (primes[i] != 0)
        {
            return (2, 3);
        }
        else
        {
            return (1, 3);
        }
    }
    else
    {
        if (primes[i] != 0)
        {
            return (1, 3);
        }
        else
        {
            return (2, 3);
        }
    }
}
```
得到一系列的概率之后需要求和了。分母不一样，需要通分，怎么找到最小公分母呢？其实很简单，15 步，每步往左或者往右，分母乘了 2，呱某个字母的概率是 1/3 或者 2/3，那么分母乘了 3，所以公分母是 $6^{15}$。
``` csharp
var commonD = (long)Math.Pow(6, 15);
long numerator = 0;
foreach (var pair in probabilities)
{
    numerator += pair.Item1 * commonD / pair.Item2;
}
```
距离得到最后答案还有两步之遥。一是根据题意等概率的从某个格子出发，一开始我们也 `for` 循环了 500 次，所以分母还要乘以 500；二是分子分母同除最大公约数化简。
``` csharp
commonD *= 500;
var gcd = Utils.GetGcd(numerator, commonD);

return $"{numerator / gcd}/{commonD / gcd}";
```
