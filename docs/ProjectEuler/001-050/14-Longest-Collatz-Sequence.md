角谷猜想：该猜想由日本数学家角谷静夫发现，是指对于每一个正整数，如果它是奇数，则对它乘 3 再加 1，如果它是偶数，则对它除以 2，如此循环，最终都能够得到 1。

这是一个很有名的问题，貌似在 06 年有一个定论。

[Problem 14](https://projecteuler.net/problem=14 "Problem 14 - Project Euler") 就是源于这个猜想。

在小于一百万的数字里面，哪一个数字算到 1 所需要的序列最长。

比如，13-40-20-10-5-16-8-4-2-1，那么 13 对应的序列长度就是 10。需要注意的是，虽然数字是小于 100 万的，但是计算过程中数字可能比 100 万大。

我们把已经算好的值保存在一个数组里面。不要使用最朴素的想法，每个值都是直接计算序列长度，那么时间复杂度将是 100 万乘以序列平均长度（计算得到该值是 132）。计算量数以亿计，对现代计算机来说，算完也是分分钟的事情，但是如果稍稍改进下算法，或许，几十毫秒就出来结果了。

给定一个整数，算出它对应的序列长度，注意，要利用之前计算出来的结果。
``` csharp
static int[] termsLength;

static int GetChainLength(this long number)
{
    int length = 1;
    while (number != 1)
    {
        if (number % 2 == 0)
        {
            number /= 2;
        }
        else
        {
            number = 3 * number + 1;
        }

        if (number < 1000000 && termsLength[number] != 0)
        {
            return length + termsLength[number];
        }

        length++;
    }
    return length;
}
```

下面是主函数，从小往大，依次计算每个数对应序列的长度。找出最大的一个。
``` csharp
termsLength = new int[1000000];

int maxLength = 0;
long maxNumber = 0;
for (long i = 1; i < 1000000; i++)
{
    int tempLength = i.GetChainLength();
    termsLength[i] = tempLength;
    if (tempLength > maxLength)
    {
        maxLength = tempLength;
        maxNumber = i;
    }
}

return maxNumber;
```
