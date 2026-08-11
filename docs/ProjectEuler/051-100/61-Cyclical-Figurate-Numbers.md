[Problem 61](https://projecteuler.net/problem=61 "Problem 61 - Project Euler")

三角形数，四角形数，五角形数，六角形数，七角形数和八角形数都是定形数，他们分别由以下公式产生：

三角形数
$$P(3,n)=n(n+1)/2$$
$$1, 3, 6, 10, 15, \cdots$$
四角形数
$$P(4,n)=n^2$$
$$1, 4, 9, 16, 25, \cdots$$
五角形数
$$P(5,n)=n(3n-1)/2$$
$$1, 5, 12, 22, 35, \cdots$$
六角形数
$$P(6,n)=n(2n-1)$$
$$1, 6, 15, 28, 45, \cdots$$
七角形数
$$P(7,n)=n(5n-3)/2$$
$$1, 7, 18, 34, 55, \cdots$$
八角形数
$$P(8,n)=n(3n-2)$$
$$1, 8, 21, 40, 65, \cdots$$

三个四位数形成的有序集合: 8128, 2882, 8281 有三个有趣的性质：
1. 这个集合是循环的：每个数的后两位数是下一个数的前两位数，包括第三个和第一个的关系。
2. 三种定形数中的每一种都能被这三个数中的一个不同的数代表：三角形数 $P(3,127)=8128$，四角形数 $P(4,91)=8281，和五角形数 $P(5,44)=2882$。
3. 这是唯一具有以上性质的四位数的集合。

找出唯一的一个六个四位数的循环集合，使得从三角形数到八角形数中的每一种都能由该集合中的一个不同的数代表。

求这个集合中元素之和。

首先，初始化这六个数组。
``` csharp
private static void Initialize(
    out List<int> P3, out List<int> P4, out List<int> P5,
    out List<int> P6, out List<int> P7, out List<int> P8)
{
    P3 = [];
    P4 = [];
    P5 = [];
    P6 = [];
    P7 = [];
    P8 = [];

    for (int n = 0; n * (n + 1) / 2 < 10000; n++)
    {
        P3.Add(n * (n + 1) / 2);
        P4.Add(n * n);
        P5.Add(n * (3 * n - 1) / 2);
        P6.Add(n * (2 * n - 1));
        P7.Add(n * (5 * n - 3) / 2);
        P8.Add(n * (3 * n - 2));
    }

    P3 = P3.Where(Meet).ToList();
    P4 = P4.Where(Meet).ToList();
    P5 = P5.Where(Meet).ToList();
    P6 = P6.Where(Meet).ToList();
    P7 = P7.Where(Meet).ToList();
    P8 = P8.Where(Meet).ToList();
}

private static bool Meet(int i)
{
    return i >= 1000 && i < 10000;
}
```
循环集合，不管最终的数字顺序是怎么样的，都可以看作是从 `P3` 开始，最后循环到 `P3`。当然，也可以从 `P4` 开始，都一样的。

这里遍历 `P3` 数组，然后再遍历其他的数组，若 `P3` 里面数字的后两位等于其他数组中的前两位，就把他们拼接起来组成新的集合 M1，M1 中都是可能的解。

遍历 M1 接着往后拼接。如果某种定形数已经拼接过了，就不再拼接了。如此五轮。
``` csharp
List<OrderedNumber> cur = [];
foreach (var i in P3)
{
    cur.Add(new OrderedNumber(i));
}

for (int i = 0; i < 5; i++)
{
    List<OrderedNumber> next = [];

    foreach (var orderedSet in cur)
    {
        Cat(P4, next, orderedSet, EnumCandidate.Four);
        Cat(P5, next, orderedSet, EnumCandidate.Five);
        Cat(P6, next, orderedSet, EnumCandidate.Six);
        Cat(P7, next, orderedSet, EnumCandidate.Seven);
        Cat(P8, next, orderedSet, EnumCandidate.Eight);
    }

    cur = next;
}

[Flags]
enum EnumCandidate
{
    None = 0,
    Four = 1,
    Five = 2,
    Six = 4,
    Seven = 8,
    Eight = 16,
}

private class OrderedNumber
{
    public OrderedNumber(long content, EnumCandidate flags = EnumCandidate.None)
    {
        Content = content;
        Flags = flags;
    }

    public long Content { get; set; }

    public EnumCandidate Flags { get; set; }
}

private static void Cat(List<int> Pi, List<OrderedNumber> numbers, OrderedNumber number, EnumCandidate flag)
{
    if (!number.Flags.HasFlag(flag))
    {
        foreach (var i in Pi)
        {
            if (number.Content % 100 == i / 100)
            {
                numbers.Add(new OrderedNumber(number.Content * 100 + i % 100, number.Flags | flag));
            }
        }
    }
}
```

好了，距离得到结果还有一步之遥。遍历 `cur` 集合中的数字，根据题目要求，最后两位和开头两位是相等的。并且题目告诉我们结果集唯一，那么一旦有满足条件的就可以求和返回了。
``` csharp
foreach (var orderedSet in cur)
{
    if (orderedSet.Content / 1000000000000L == orderedSet.Content % 100)
    {
        long sum = 0;
        long result = orderedSet.Content;
        for (int i = 0; i < 6; i++)
        {
            sum += result % 10000;
            result /= 100;
        }

        return sum.ToString();
    }
}
```
