[Problem 109](https://projecteuler.net/problem=109 "Problem 109 - Project Euler")

在飞镖游戏中，玩家向靶盘投掷三支飞镖。靶盘被分成 20 个大小相等的区域，编号为 1 到 20。

飞镖的得分由其落点的区域数字决定。此外，靶盘上还有两个同心圆环：

- 外环 (Double ring)：落在此处得分为该区域数字的两倍。
- 内环 (Treble ring)：落在此处得分为该区域数字的三倍。
- 
靶盘中心还有两个特殊的区域：

- 外红心 (Outer Bull)：得分为 25 分。
- 内红心 (Inner Bull)：得分为 50 分，且被视为双倍区。

通常飞镖比赛的规则是“501分减分制”，但为了赢得一局比赛（即“结镖”），玩家必须通过投掷飞镖使剩余分数正好归零，且最后一支飞镖必须落在双倍区（包括内红心）。

例如，如果剩余分数为 6 分，可以通过以下几种方式结镖：

| 方式 | 飞镖 1 | 飞镖 2 | 飞镖 3 | 总分 |
| --- | --- | --- | --- | --- |
| 1 |  |  | D3 | 6 |
| 2 |  | D1 | D2 | 6 |
| 3 |  | S2 | D2 | 6 |
| 4 |  | D2 | D1 | 6 |
| 5 |  | S4 | D1 | 6 |
| 6 |  | S1, S1 | D2 | 6 |
| 7 |  | S1, T1 | D1 | 6 |
| 8 |  | S1, S3 | D1 | 6 |
| 9 |  | D1, D1 | D1 | 6 |
| 10 |  | D1, S2 | D1 | 6 |
| 11 |  | S2, S2 | D1 | 6 |

注：S 代表单倍区，D 代表双倍区，T 代表三倍区。例如 D3 表示 3 号区域的双倍区。

请注意，`D1, D2` 和 `D2, D1` 被视为不同的结镖方式，因为它们结束在不同的双倍区上。然而，`S1, T1, D1` 和 `T1, S1, D1` 被视为相同的结镖方式，因为在最后一镖之前，飞镖的投掷顺序不影响区分。

此外，不包含那些脱靶的情况。因此，对于总分为 6 的情况，共有 11 种不同的结镖方式。

经统计，总共有 42336 种不同的结镖方式。

问题是对于总分严格小于 100 的情况，共有多少种不同的结镖方式？

这个题目数据集非常低，总共有 62 种得分方式，结镖有 21 种，如果第三标劫镖，前面两标顺序不重要，那么最多只有 `62 * 61 * 21 = 79422` 种方式，暴力法也没有问题。

稍微做一点优化，62 种得分方式，按照得分排序，如果剩余分数小于某一个值，后续的可能的值可以跳过。

下面是计算给定分数有多少种结镖方式。
```csharp
private static void Search(List<Throw> throws, int target, int startIndex, int maxThrows, ref int count)
{
    if (maxThrows == 0) { return; }

    // try to finish
    if (target % 2 == 0 && (target / 2 == 25 || (target / 2 <= 20)))
    {
        count++;
    }

    // try to continue
    for (int i = startIndex; i < throws.Count; i++)
    {
        var t = throws[i];
        int score = t.Score * t.Multiplier;

        if (score < target)
        {
            Search(throws, target - score, i, maxThrows - 1, ref count);
        }
        else
        {
            break;
        }
    }
}
```

`Throw` 的定义和初始化 `throws` 代码如下。
```csharp
struct Throw
{
    public int Score;
    public int Multiplier;

    public Throw(int score, int multiplier)
    {
        Score = score;
        Multiplier = multiplier;
    }
}

// initialize possible throws
for (int i = 1; i <= 20; i++)
{
    Throws.Add(new Throw(i, 1));
    Throws.Add(new Throw(i, 2));
    Throws.Add(new Throw(i, 3));
}
Throws.Add(new Throw(25, 1));
Throws.Add(new Throw(25, 2));

Throws.Sort((a, b) => a.Score * a.Multiplier - b.Score * b.Multiplier);
```

最后对 100 分以内所有场景求和就好。
```csharp
for (int target = 2; target < MAX; target++)
{ Search(Throws, target, 0, 3, ref count); }
```
