[Problem 95](https://projecteuler.net/problem=95 "Problem 95 - Project Euler")

28 有 4 个不包含自身的因数 1 2 4 7，其和恰好是 28，我们称之为完美数。

220 不包含自身的的因数之和是 284，284 不包含自身的因数之和是 220，形成了一个长度为 2 的链，也称之为亲和数。

从 12496 能形成一个长度为 5 的链：`12496 -> 14288 -> 15472 -> 14536 -> 14264 (-> 12496 -> ...)`。

找到一个所有数值都不超过一百万的最长的链，问其中最小的数。

首先，我们要快速找到每个数对应的因数之和。
``` csharp
var numberToDivisorSum = new List<int>(MAX + 1);
for (int i = 0; i <= MAX; i++)
{
    // 1 is divisor
    numberToDivisorSum.Add(1);
}

// i is a factor
for (int i = 2; i <= MAX / 2; i++)
{
    // excluding the number itself, so j starts from 2
    for (int j = 2; i * j <= MAX; j++)
    {
        numberToDivisorSum[i * j] += i;
    }
}
```
接下来计算每个数开始能够形成的链的长度，找到最长链所对应的最小的数字。  
``` csharp
int longest = -1;
int index = -1;
for (int i = 1; i <= MAX; i++)
{
    if (numberToDivisorSum[i] != -1)
    {
        int length = GetChainLength(i, numberToDivisorSum);
        if (length > longest)
        {
            index = i;
            longest = length;
        }
    }
}

return index.ToString();

private static int GetChainLength(int number, List<int> numberToDivisorSum)
{
    var chain = new List<int>();
    int cur = number;
    while (true)
    {
        chain.Add(cur);
        int next = numberToDivisorSum[cur];
        if (next > MAX || next == -1)
        {
            foreach (var s in chain)
            {
                if (s > number)
                {
                    numberToDivisorSum[s] = -1;
                }
            }
            return -1;
        }

        if (chain.Contains(next) && number != next)
        {
            return -1;
        }

        if (next == number)
        {
            foreach (var s in chain)
            {
                if (s > number)
                {
                    numberToDivisorSum[s] = -1;
                }
            }

            return chain.Count;
        }

        cur = next;
    }
}
```
这里有一个有点 tricky 的地方，如果一个链形成了（已经知道最小数了），或者因数之和比 `MAX` 要大导致无法形成一个链，那么把当前 `number` 之后的数都标记一下，在外层调用函数的 `for` 循环里面跳过，这样子大概能节省 10% 的时间吧。
