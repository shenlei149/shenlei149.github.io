[Problem 719](https://projecteuler.net/problem=719 "Problem 719 - Project Euler")

首先来定义什么是 $S$-number。若一个自然数 $n$ 是完全平方数，且将 $n$ 的十进制写法分割成 2 个或多个数字，这些数字之和恰好是其平方根，那么这个数字就是 $S$-number。

以下都是$S$-number：
* $\sqrt{81}=8+1$
* $\sqrt{6724}=6+72+4$
* $\sqrt{8281}=8+2+81=82+8+1$
* $\sqrt{9801}=98+0+1$

$T(N)$ 表示所有 $n\leq N$ 的 $S$-number 之和。已知 $T(10^4)=41333$。求 $T(10^{12})$。

遍历 $i\leq 10^6$，可以得到所有 $n\leq 10^{12}$ 的候选值，这些数可能的 $S$-number，需要考虑的就是如何判断一个根 $\sqrt{n}$ 和 $n$ 满足 $S$-number 的性质。

首先，对于某个 $n$，其长度是 $l$，那么分割的方式总是固定的。比如$123$ 和 $124$ 的分割模式是一致的，前者分别是 $\{1,2,3\},\{12,3\},\{1,23\}$，后者分别是 $\{1,2,4\},\{12,4\},\{1,24\}$。那么遍历出所有模式，长度最多是 12。$10^{12}$ 长度是 13，但是它是 $S$-number 先不考虑，最后求和加上就行。

``` csharp
private static List<Pattern> GetAllPatternsByLength(int length)
{
    var result = new List<Pattern>() { new Pattern().AddPair(Tuple.Create(0, length)) };
    for (int i = 1; i < length; i++)
    {
        var subPatterns = GetAllPatternsByLength(length - i);
        result.AddRange(subPatterns.Select(pattern => pattern.AddOffset(i).AddPair(Tuple.Create(0, i))));
    }

    return result;
}

class Pattern
{
    // (start, length)
    public List<Tuple<int, int>> Pairs { get; private set; }
    private int max;

    public Pattern()
    {
        Pairs = [];
    }

    public Pattern AddPair(Tuple<int, int> pair)
    {
        Pairs.Add(pair);
        if (pair.Item2 > max)
        {
            max = pair.Item2;
        }

        return this;
    }

    public Pattern AddOffset(int offset)
    {
        var result = new Pattern();
        foreach (var pair in Pairs)
        {
            result.AddPair(Tuple.Create(pair.Item1 + offset, pair.Item2));
        }

        return result;
    }

    public int GetMaxLength()
    {
        return max;
    }
}
```
这里会多计算一种模式，就是 $n$ 本身，但是 $n$ 本身比 $\sqrt{n}$ 大，不符合条件，后续会过滤掉，所以没有关系。

$\sqrt{n}$ 和 $n$ 之间是有一些关系的，比如 $\sqrt{n}$ 长度是 6，而 $n$ 可能是 11 也可能是 12。不过若长度是 12，那么 $\sqrt{n}$ 首字母会大于等于 3，如果分割的数字最长的长度小于 6 的话，那么两个五位数再加一个两位数，是不会得到一个以 3 开头的数字的。对于 $n$ 的长度是 11 的情况，最长的被分割数字也不能太短，最短也要五位数字。所有可以通过这个过滤掉很多不必要的选项。由于这个性质之和 $\sqrt{n}$ 和 $n$ 的长度有关，可以把结果缓存起来。
``` csharp
private List<Pattern> GetPatterns(int rootLength, int length)
{
    string key = rootLength + "_" + length;
    if (!mappings.ContainsKey(key))
    {
        if (rootLength * 2 == length)
        {
            mappings[key] = patterns[length].Where(p => p.GetMaxLength() == rootLength).ToList();
        }
        else
        {
            mappings[key] = patterns[length].Where(p => p.GetMaxLength() == rootLength || p.GetMaxLength() == rootLength - 1).ToList();
        }
    }

    return mappings[key];
}
```

有了这些可能的值，就可以遍历这些可能的值来判断一个数字 $n$ 是不是 $S$-number。
``` csharp
private bool CanBeSplit(long root, long S)
{
    int rootLength = (int)Math.Floor(Math.Log10(root) + 1);
    int sLength = (int)Math.Floor(Math.Log10(S) + 1);
    var patterns = GetPatterns(rootLength, sLength);
    string strS = S.ToString();
    foreach (var pattern in patterns)
    {
        int sum = 0;
        foreach (var pair in pattern.Pairs)
        {
            sum += int.Parse(strS.Substring(pair.Item1, pair.Item2));
        }

        if (sum == root)
        {
            return true;
        }
    }

    return false;
}
```

最外层遍历 $i\leq 10^6$ 即可。
``` csharp
for (int i = 2; i <= 12; i++)
{
    patterns[i] = GetAllPatternsByLength(i);
}

long END = 1_000_000;
long sum = END * END;

// 1, 4, 9 cannot be split into 2 or more numbers
for (long i = 4; i < END; i++)
{
    long S = i * i;
    if (CanBeSplit(i, S))
    {
        sum += S;
    }
}

return sum.ToString();
```
这个方法相对是比较慢的，在我的机器上大概需要二十多秒。下面将几个优化点。

第一个是代码
``` csharp
sum += int.Parse(strS.Substring(pair.Item1, pair.Item2));
```
这里耗时比较多，这里很容易通过数字的除法和求余来搞定，会快不少。
``` csharp
private readonly long[] Pows = new long[] {1,10,100,1000, 10000,
    100000, 1000000, 10000000,100000000,1000000000,
    10000000000,100000000000,1000000000000};

sum += S / Pows[sLength - pair.Item1 - pair.Item2] % Pows[pair.Item2];
```

第二个是仔细考虑下
> $\sqrt{n}$ 和 $n$ 之间是有一些关系的，比如 $\sqrt{n}$ 长度是 6，而 $n$ 可能是 11 也可能是 12。不过若长度是 12，那么 $\sqrt{n}$ 首字母会大于等于 3，如果分割的数字最长的长度小于 6 的话，那么两个五位数再加一个两位数，是不会得到一个以 3 开头的数字的。对于 $n$ 的长度是 11 的情况，最长的被分割数字也不能太短，最短也要五位数字。所有可以通过这个过滤掉很多不必要的选项。

两者的判断标准可以更细致，如果 $\sqrt{n}$ 是的前两个数字大于等于 21，那么分割的数字最长的长度就必须等于 $\sqrt{n}$ 的长度，因为再小的话就无法加起达到 $\sqrt{n}$ 了。举个具体例子，$\sqrt{n}$ 长度是 3，如果 $n$ 的长度是 6，回到上一段的情况，若 $n$ 的长度是 5，最长长度是 2，那么两个两位数和一个 1 位数，其和最大不可能是 210，而 $\sqrt{n}$ 是的前两个数字大于等于 21。改写这个判断逻辑，能够再减少一些不必要的模式。
``` csharp
while (root > 100)
{
    root /= 10;
}

var t = root >= 21;
string key = rootLength + "_" + sLength + (t ? "T" : "F");
if (!mappings.ContainsKey(key))
{
    if (t)
    {
        mappings[key] = patterns[sLength].Where(p => p.GetMaxLength() == rootLength).ToList();
    }
    else
    {
        mappings[key] = patterns[sLength].Where(p => p.GetMaxLength() == rootLength || p.GetMaxLength() == rootLength - 1).ToList();
    }
}
```

第三个优化点是在求和那里，如果前面的几个数字之和已经超过了 $\sqrt{n}$，就不要继续了。比如 $212\times 212=44944$，前面三个数字超过了 212，不过后面怎么样，也无所谓了。
``` csharp
sum += S / Pows[sLength - pair.Item1 - pair.Item2] % Pows[pair.Item2];

if (sum > root)
{
    break;
}
```
经过优化，在我机器上大概 13s 能得到答案。

其实有一个方向，是第三点的扩展，各个模式恰好能组成一棵树，那么自上而下算到某个节点超过了 $\sqrt{n}$，下面整个子树都不需要算了。但是构建这个树也要遍历所有的模式的每一个起始 `pair`，不好说最后能够提高性能。
