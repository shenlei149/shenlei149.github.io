[Problem 64](https://projecteuler.net/problem=64)

一个数的平方根可以写成连分数，形式如下：
$$\sqrt{N}=a_0+\frac{1}{a_1+\frac{1}{a_2+\frac{1}{a_3+\dots}}}$$
$a_0, a_1, a_2, a_3,\cdots$ 可以看作是一个数列，这个数列很快就有周期性了。比如题目中的例子，$\sqrt{23}$ 写成连分数，取数列 $a$ 就是 $4,1,3,1,8,1,3,\cdots$ 可以看出来，循环节是 1318，周期是 4。

问题是要求，小于 10000 的所有整数，有多少个周期是奇数的呢？

求解这个题目，关键是：给定一个数 $N$，如何求得 $a_0, a_1, a_2, a_3,\cdots$

祭出伟大的维基百科：[Continued_fraction_expansion](https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Continued_fraction_expansion)

可以略过推导部分，直接看 Algorithm 实现就好了。

我们现在能求得了这个数列，如何判定周期呢？

从算法上我们可以看到，$m, d, a$ 三个数是迭代计算的，也就是说，第 $n+1$ 次的 $m, d, a$ 三个数字是有第 $n$ 次的 $m, d, a$ 决定的，因为 $\sqrt{S}$ 不变。如果有两次的 $m, d, a$ 一样，那么它们后续都一样。

我的做法是拿一个 `List`，把 $m, d, a$ 作为一个元组放进去，每次计算得到一组新的 $m, d, a$，就去 `List` 里面查找一下，如果找不到，说明这个是新的组合，放到 `List` 里面，如果找到了，那么这次的序数减去上次的序数，就是周期了。
``` csharp
private static int GetPeriod(int S)
{
    int a0 = (int)Math.Sqrt(S);
    int m = 0;
    int d = 1;
    int a = a0;
    var periods = new List<Tuple<int, int, int>>();
    while (true)
    {
        m = d * a - m;
        d = (S - m * m) / d;
        a = (a0 + m) / d;
        var cur = Tuple.Create(m, d, a);
        int index = periods.IndexOf(cur);
        if (index >= 0)
        {
            return periods.Count - index;
        }
        else
        {
            periods.Add(cur);
        }
    }
}
```

有了计算周期的函数，下面的事情就好办了，遍历 $[1,10000)$ 这一万个数字即可。我们可以可以先排除完全平方数，因为它们只有 $a_0$，没有后续的$a_1, a_2, \cdots$。
``` csharp
int count = 0;

var l1 = Enumerable.Range(1, 10000);
var l2 = Enumerable.Range(1, 100).Select(x => x * x);
var candidates = l1.Except(l2);

foreach (var c in candidates)
{
    if (GetPeriod(c) % 2 != 0)
    {
        count++;
    }
}

return count;
```
