[Problem 205](https://projecteuler.net/problem=205 "Problem 205 - Project Euler")

Peter 有九个 4 个面的骰子，类似于金字塔的样子，每个面上标有数字，分别是 1-4。

Colin 有六个 6 个面的骰子，就是正常的骰子了，每个面上标有数字，分别是1-6。

现在他俩比赛，各投掷一次，点数之和比较大的胜。点数之和一样的话算作平局。

问题是，Peter胜利的概率是多少？要求答案是 0.abcdefg 这种形式。也就是小数点后面有七位数字。

看到题目的想法是，不要用 double 类型，可能会有精度误差，以至于最后的答案不正确。

那就只考虑分子好了，因为分母就是 4 的 9 次方，乘以 6 的 6 次方，最后得到答案的前一步除一下就好。

首先计算 Peter 扔出的点数之和的概率分布。我使用一个字符串表示 Peter 掷出的结果，字符串长度为九，每一个字符只能是 1-4 这四个数字。下面是一段递归代码，得到了所有可能的结果：
``` csharp
private static List<string> PeterResults = new List<string>();
private static void GenPeterResults(string num, int nth)
{
    for (int i = 1; i <= 4; i++)
    {
        if (nth == 8)
        {
            PeterResults.Add(num + i);
        }
        else
        {
            GenPeterResults(num + i, nth + 1);
        }
    }
}
```

接着构造一个数组，用于存放每种点数之和出现的次数。该次数除以 4 的 9 次方就是概率，但是正如前面所说的，现在只考虑分子。
``` csharp
foreach (var s in PeterResults)
{
    int[] digits = s.Select(c => int.Parse(c.ToString())).ToArray();
    Peter[digits.Sum()]++;
}
```

下面计算 Colin 扔出的结果的概率分布。和上面的过程类似。

好了两个概率分布都有了，开始比赛吧。如果 Peter 的点数比 Colin 大，计算下概率，最后加起来就好了。
``` csharp
long numerator = 0;
for (int p = 1; p < Peter.Length; p++)
{
    for (int c = 1; c < Colin.Length; c++)
    {
        if (p > c)
        {
            numerator += Peter[p] * Colin[c];
        }
    }
}
```
现在，分母是 4 的 9 次方乘以 6 的 6 次方。那么用分子除以分母，保留 7 位小数就是答案了。
``` csharp
return ((double)numerator / 262144 / 46656).ToString("F7");
```
