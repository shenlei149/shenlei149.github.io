[Problem 51](http://projecteuler.net/problem=51 "Problem 51 - Project Euler")

通过置换两位数 $*3$ 的第一位得到的 9 个数中，有六个是质数：13,23,43,53,73 和 83。

同样的方法，置换五位数 $56**3$ 的第三位和第四位，这个五位数是第一个能够得到七个质数的数字，得到的质数是：56003, 56113, 56333, 56443, 56663, 56773 和 56993。因此其中最小的 56003 就是具有这个性质的最小的质数。

找出最小的质数，通过用同样的数字置换其中的一部分（不一定是相邻的部分），能够得到八个质数。

首先说明一下，我直觉觉得这道题的解答是六位数，仅仅是直觉，因为做了很多欧拉项目的题目。其实，如果答案不是六位数，比如是五位数，分析方法是不变的。

变化几个数字呢？

假设是 1 个，考虑下被三整除的性质。假设除去变化的数字之外的五位数之和模三等于一：那么变化的这一位数不能是 2, 5, 8。最多只能得到七个质数。假设除去变化的数字之外的五位数之和模三等于二：那么变化的这一位数不能是 1, 4, 7。同上。类似的，其余五个数字之和模三等于零。所以不可能只变化一个数字。

假设是 2 个呢？按照上面的思路，可以得出结论，不可能只变化两个数字。

假设是 3 个呢？同一数字乘以3，模三等于 0，不影响其余三位数字之和模三的结果，不会推出矛盾。

4个，5个？！也是不可能的。

变化的哪几位数呢？

肯定不能变化最后一位数。因为，质数的最后一位只可能是 1, 3, 7, 9，离八个质数还很远呢。

至此，我们得到一个结论：六位数字，最后一位不能是变化的，同时，每次要变化三个数字。

那么，题目的结果一定要满足下面的模式：  
``` csharp
string[] patterns = new string[] { "110001", "101001", "100101", "100011", "011001", "010101", "010011", "001011", "000111" };
```
其中，0 表示变化的数字，也就是说，对于某个质数，这三位要一样；1 表示不变化的数字。

现在，我们来生成可能的候选质数。首先，生成所有的六位数质数。
``` csharp
var primes = Utils.GenPrimes(1000000).Where(l => l > 100000).Select(l => (int)l).ToArray();
```

把符合模式的数字筛选出来，然后放到对应的组中，这个组就是这个质数满足哪一个模式。
``` csharp
foreach (var p in primes)
{
    var pStr = p.ToString();
    for (int i = 0; i < patterns.Length; i++)
    {
        string pattern = patterns[i];
        char[] chars = new char[3];
        int index = 0;
        for (int j = 0; j < pattern.Length; j++)
        {
            if (pattern[j] == '0')
            {
                chars[index] = pStr[j];
                index++;
            }
        }

        if (chars[0] == chars[1] && chars[1] == chars[2])
        {
            groups[i].Add(p);
        }
    }
}
```

对于模式一样的质数，需要把其他三位不变的数字提取出来，一样的在一组，这样的组内就是一系列变换数字能够得到的质数了。如果这些质数的个数有八个，满足题意。
```csharp
for (int i = 0; i < patterns.Length; i++)
{
    var group = groups[i];
    var eight = HandleGroup(group, patterns[i]);
    if (eight.Count > 0)
    {
        return eight.First().First().ToString();
    }
}

private static List<List<int>> HandleGroup(List<int> primes, string pattern)
{
    Dictionary<int, List<int>> subGroups = [];
    for (int i = 0; i < primes.Count; i++)
    {
        int p = primes[i];
        int key = 0;
        for (int j = pattern.Length - 1; j >= 0; j--)
        {
            if (pattern[j] == '1')
            {
                key *= 10;
                key += p % 10;
            }

            p /= 10;
        }

        if (subGroups.ContainsKey(key))
        {
            subGroups[key].Add(primes[i]);
        }
        else
        {
            subGroups[key] = [primes[i]];
        }
    }

    return subGroups.Values.Where(ints => ints.Count == 8).ToList();
}
```
