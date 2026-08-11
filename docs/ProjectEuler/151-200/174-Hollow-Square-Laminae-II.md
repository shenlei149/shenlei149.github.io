[Problem 174](https://projecteuler.net/problem=174 "Problem 174 - Project Euler")

这个题目是 [173](./173-Hollow-Square-Laminae-I.md) 的续，描述是一样的。

当只有 8 个瓷砖时，只有一种拼法。因此 $t=8$ 是 $L(1)$ 型。当有 32 个瓷砖时，只有两种不同的拼法。因此当 $t=32$ 是 $L(2)$ 型。

令 $N(n)$ 是 $t\leq 1,000,000$ 时，$t$ 是 $L(n)$ 型的个数。已知 $N(15)=832$。

求 $\sum_{n=1}^{10}N(n)$。

代码思路和之前类似，不同之处是需要统计瓷砖 $t$ 时，拼法的数量。因此需要声明一个数组统计次数。从题目给出的已知条件，`counts` 中值为 15 的元素个数是 832。
```csharp
int[] counts = new int[N + 1];
for (int i = 0; i < counts.Length; i++)
{
    counts[i] = 0;
}

for (int hole = 1; hole < N / 4; hole++)
{
    int min_length = hole + 1;

    for (int max_length = min_length; ; max_length += 2)
    {
        int a_1 = min_length;
        int a_n = max_length;
        int n = (a_n - a_1) / 2 + 1;
        int total = (a_1 + a_n) * n / 2 * 4;
        if (total <= N)
        {
            counts[total]++;
        }
        else
        {
            break;
        }
    }
}
```
求 $\sum_{n=1}^{10}N(n)$，即统计 `counts` 中值从 1 到 10 的个数。
```csharp
int sum = 0;
for (int j = 0; j < counts.Length; j++)
{
    if (counts[j] <= 10 && counts[j] >= 1)
    {
        sum++;
    }
}
```

