[Problem 516](https://projecteuler.net/problem=516 "Problem 516 - Project Euler")

`5-smooth` 数是指质因数不超过5的数。换句话说，质因数只能是 2，3，5。

$S(L)$ 是指不超过的 $L$ 的 $n$ 求和，其中 $n$ 满足 [欧拉函数，Euler's totient function](https://en.wikipedia.org/wiki/Euler%27s_totient_function) $\phi(n)$ 是一个 `5-smooth` 数。

$S(100)=3728$，题目给出这个提示是进行一个简单的验证，检查自己的算法/程序是否有问题。

求 $S(10^{12}) \operatorname{mod} 2^{32}$

$10^{12}$ 次方很大，电脑是 GHz 级别，$10^9$，所以遍历小于 $L$ 的每一个数是不现实的。我们要想办法构造出所有满足条件的 $n$，然后求和。

$\phi(n)$ 有很多种写法，下面这种对于我们分析这个题目很有帮助。
$$  
\phi(n)=\prod_{i=1}^k p_i^{r_k-1}(p_i-1)
$$
$$
n=\prod_{i=1}^k p_i^{r_k}
$$  

如果质因数只有 2，3，5，减一之后是 1，2，4，那么这个 $n$ 是满足条件的。

质因数能大于5吗？可以。比如 7， $7-1$ 是 6，质因数是 2 和 3，如果这种质因数的次数 $r$ 只能是 1，$r-1$ 是 0，那么也还是满足要求的。如果多于 1 次，$\phi(n)$ 就会包含比 5 大的因数。再比如 11，41 等都是满足条件的。

这种质数是可以任意相乘的，组合之后仍然满足题意。他们和质因数只有 2 3 5 的数相乘，依旧满足题意。顺便说一下，只有这些质数作为质因数的数也是满足题意的。

如何构造这种质数呢？找到所有质因数是 2 3 5 的数，加一，再判断是不是质数。
让我们先写个程序找到所有小于 $10^{12}$ 且质因数都是 2 3 5 的数。
``` csharp
for (int i = 0; i < Math.Log(max) / Math.Log(2); i++)
{
    for (int j = 0; j < Math.Log(max) / Math.Log(3); j++)
    {
        for (int k = 0; k < Math.Log(max) / Math.Log(5); k++)
        {
            if (Math.Pow(2, i) * Math.Pow(3, j) * Math.Pow(5, k) <= max)
            {
                smooths.Add((long)(Math.Pow(2, i) * Math.Pow(3, j) * Math.Pow(5, k)));
            }
        }
    }
}

smooths.Sort();
```
最后，对 `smooths` 进行了排序，其目的后面会提到。

接下来，生成所有大于 5 且满足上述分析的质数。
``` csharp
var candidates =
    smooths
    .Select(l => l + 1)
    .Where(Utils.IsPrime)
    .Where(p => p > 5)
    .ToArray();
```
那么如何对这些质数进行组合呢？普通的组合算法是行不通的。因为`candidates` 有 500 多个数据，从中任选 5 个就达到了 $10^{12}$ 这个量级，所以需要合适的裁剪。

我的做法是分层，第一层都是 1 个数组成的组合，第二层都是 2 个数组成的组合，依此类推。同时，后一层是从前一层的组合生成的。对于 $i$ 层的某一个组合，$i+1$ 层就是从 `smooths` 里面选一个数，然后添加到尾部。`smooths` 排序的好处有两个，一是找到 $i$ 层组合的最后一个数，只需要往上追加比它大的数即可，排序可以利用二分查找；二是如果第 $j$ 个加上去之后，乘积大于 $10^{12}$，后面的数就不用看了。
``` csharp
List<List<List<long>>> tiers = new List<List<List<long>>>();
var firstTier = candidates.Select(p => new List<long>() { p }).ToList();
tiers.Add(firstTier);

while (tiers.Last().Count != 0)
{
    var lastTier = tiers.Last();
    var currentTier = new List<List<long>>();
    tiers.Add(currentTier);

    foreach (var combination in lastTier)
    {
        double product = combination.Aggregate((total, next) => total * next);

        int index = Array.BinarySearch(candidates, combination.Last());
        for (int i = index + 1; i < candidates.Length; i++)
        {
            if (product * candidates[i] <= max)
            {
                currentTier.Add(new List<long>(combination)
                {
                    candidates[i]
                });
            }
            else
            {
                break;
            }
        }
    }
}
```
所有的组合都找到了，很容易得到乘积。
``` csharp
var factors = new List<long>();
foreach (var tier in tiers)
{
    foreach (var combination in tier)
    {
        factors.Add(combination.Aggregate((total, next) => total * next));
    }
}
```
我们已经得到了下面这种情况。
> 这种质数是可以任意相乘的，组合之后仍然满足题意。

接着处理下一种情况
> 他们和质因数只有 2 3 5 的数相乘，依旧满足题意。
``` csharp
factors.Sort();

foreach (var factor in factors)
{
    foreach (var smooth in smooths)
    {
        if ((double)smooth * factor <= max)
        {
            sum += smooth * factor;
        }
        else
        {
            break;
        }
    }
}

sum += smooths.Sum();
```
排序的好处就是可以提前终止。上面代码的最后一句是把基本 case 包含上了。
> 如果质因数只有 2，3，5，减一之后是 1，2，4，那么这个 $n$ 是满足条件的。

`smooths` 里面包含1（$i,j,k$ 都是 0 的情况），所以循环也包含了前面提到的这种情况。
> 顺便说一下，只有这些质数作为质因数的数也是满足题意的。

最后，返回结果。
``` csharp
return (uint)sum;
```
`sum` 一开始定义为 `long`，`uint` 截断后就是模 $2^{32}$ 的结果。

上述是 2018 年 7 月解题时的思路和具体实现，仔细审视下代码，有很多地方可以优化。
1. `tiers` 最里面的 `List<long>` 我们只用到了最后一个数值和它的乘积，那么使用 `Tuple<long, long>` 保存要的值即可，避免反复拷贝 `List<long>`
2. `factors.Sort();` 是不需要的，因为 `break` 的前提仅是 `smooths` 有序就好
3. `factors` 数值都是前面保存的，那么可以不必最后统一 `Add`，在生成的地方添加即可
4. 不需要最后统一调用 `factors.Add`，那么保存所有的 `tiers` 也不需要，两个 tier 来回倒腾即可
5. 最后，`factors` 也是不需要的

经过这些调整，性能提高了 2.5+ 倍，主要贡献是前面两项。

