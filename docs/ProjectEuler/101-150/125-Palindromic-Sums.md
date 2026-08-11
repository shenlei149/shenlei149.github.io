[Problem 125](https://projecteuler.net/problem=125)

回文数 595 能够被写成连续几个数字的平方的和：
$$595=6^2+7^2+8^2+9^2+10^2+11^2+12^2$$

连续数字必须是正数，所以 $1=0+1$ 被排除在外。

题目告诉我们小于 1000 的数字中，有 11 个满足题目所述条件，其之和是 4164，这可以用来检测我们的程序是否正确。

求小于 $10^8$ 的数中，所以能够写成连续数字平方和的回文数之和。

欧拉项目里面这种题目的套路就是正向构造符合题意的数字。因为想要判定一个数能否写成连续数字之后比较困难，但是构造一个连续数字平方和的数就容易多了。

我们先计算出一系列的平方数，多少个呢？由于连续数字之和，那么最少也要两个数字，所以我们构造的最大平方数是 `MAX` 的一半就足够了。
``` csharp
int MAX = 100000000;
var squars = Enumerable.Range(1, (int)Math.Sqrt(MAX / 2)).Select(i => i * i).ToArray();
```
两层 `for` 循环来构造连续数字之和。
``` csharp
var candidates = new List<long>();
for (int i = 0; i < squars.Length - 1; i++)
{
    long sum = squars[i];
    for (int j = i + 1; j < squars.Length; j++)
    {
        if (sum + squars[j] < MAX)
        {
            sum += squars[j];
            candidates.Add(sum);
        }
        else
        {
            break;
        }
    }
}
```
这里面有一小撮重复的数字，去重，然后判断是否是回文数字，求和。
``` csharp
return candidates.Distinct().Where(l => Utils.IsPalindrome(l.ToString())).Sum();
```
