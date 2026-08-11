[Problem 348](https://projecteuler.net/problem=348)

很多数能够写成一个平方数和一个立方数之和的形式，其中一些可能会有多种组合形式。

有一些数有 4 种组合形式，且它是一个回文数。比如 5229225 是一个回文数，能写成四种组合形式：
$$
2285^2 + 20^3\\
2223^2 + 66^3\\
1810^2 + 125^3\\
1197^2 + 156^3
$$
找到前五小的这种数，求和。

一个数是否能拆解成一个平方数和一个立方数之和是比较难的问题，但是构造一个这样的数相对比较简单。开始假设在 `int` 范围内就至少有五个满足题意的数。

下面这段代码就会找出所有是一个平方数和一个立方数之和的回文数，并且统计了它们出现的次数。
``` csharp
var counts = new Dictionary<int, int>();
for (int i = 2; i < Math.Sqrt(int.MaxValue); i++)
{
    for (int j = 2; j < 1291; j++)
    {
        long sum = i * i + j * j * j;
        if (sum > int.MaxValue)
        {
            break;
        }

        int s = (int)sum;
        if (Utils.IsPalindrome(s.ToString()))
        {
            if (counts.ContainsKey(s))
            {
                counts[s]++;
            }
            else
            {
                counts[s] = 1;
            }
        }
    }
}
```
然后找出出现次数为 4 的回文数，排序，取前五，求和。
``` csharp
var candidates = counts.Where(pair => pair.Value == 4).Select(pair => pair.Key).ToList();
candidates.Sort();

return candidates.Take(5).Sum();
```
