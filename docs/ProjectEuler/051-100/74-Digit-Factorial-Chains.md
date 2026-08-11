[原题链接](https://projecteuler.net/problem=74 "Problem 74 - Project Euler")

数字145有一个著名的性质：其所有位上数字的阶乘和等于它本身。
$$1! + 4! + 5! = 1 + 24 + 120 = 145$$
169 不像 145 有这个性质，但是 169 可以产生如下最长的能够连接回它自己的数字链。事实证明一共有三条这样的链：
```
169 → 363601 → 1454 → 169
871 → 45361 → 871
872 → 45362 → 872

```
不难证明每一个数字最终都将陷入一个循环。例如：
```
69 → 363600 → 1454 → 169 → 363601 (→ 1454)
78 → 45360 → 871 → 45361 (→ 871)
540 → 145 (→ 145)
```
从 69 开始可以产生一条有 5 个不重复元素的链，但是以一百万以下的数开始，能够产生的最长的不重复链包含 60 个项。一共有多少条以一百万以下的数开始的链包含 60 个不重复项？

我想，这个题难度不大。首先每个数字和它对应的下一个值放到一个字典里面。
```csharp
var nexts = new Dictionary<int, int>();
for (int i = 1; i < MAX; i++)
{
    int key = i;
    int value = GetNext(i);
    nexts[key] = value;

    while (value > MAX)
    {
        key = value;
        value = GetNext(key);
        nexts[key] = value;
    }
}
```

然后从 3 开始遍历到 1'000'000，统计链的长度。这里可以将之前的一些结果保存起来，如果遇到之前知道长度的数，那么直接加上当前长度即可。
```
var length = new Dictionary<int, int>();
for (int i = 3; i < MAX; i++)
{
    var numbers = new HashSet<int>();
    int cur = i;
    while (!numbers.Contains(cur))
    {
        if (length.ContainsKey(cur))
        {
            length[i] = numbers.Count + length[cur];
            break;
        }

        numbers.Add(cur);
        cur = nexts[cur];
    }

    if (!length.ContainsKey(i))
    {
        length[i] = numbers.Count;
    }
}

return length.Values.Count(v => v == 60).ToString();
```

其中 `GetNext` 比较简单，对每个数字求阶乘然后相加即可。
```
private static int GetNext(int i)
{
    int sum = 0;
    while (i != 0)
    {
        int m = i % 10;
        sum += factorials[m];

        i /= 10;
    }

    return sum;
}
```
