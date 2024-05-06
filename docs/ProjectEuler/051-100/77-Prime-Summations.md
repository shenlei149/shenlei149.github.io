[Problem 77](https://projecteuler.net/problem=77 "Problem 77 - Project Euler")

一个数，能够写成一些质数的和。比如，10 可以写成下面五种不同的形式。
```
7 + 3
5 + 5
5 + 3 + 2
3 + 3 + 2 + 2
2 + 2 + 2 + 2 + 2
```
第一个能被写成大于 5000 种形式的数是都少呢？

退一步，给定一个数 $n$，如何计算有多少种分解形式呢？这类似于经典的硬币问题，给定几种面值的硬币和钱数，求有多少种方法能够凑成需要的钱数。

经典题目自有经典的解法。硬币从大到小逆序排列，先尝试使用最大的硬币，然后递归尝试使用更小的硬币。参考这种想法，可以先得到一堆质数，然后从大往小排。找到小于等于 $n$ 的质数，利用找硬币的算法得到次数。因为最小的质数是 2 不是 1，所以递归的往下减少可能会是负数，此时表示没有合适的解。
``` csharp
private static int Count(int sum)
{
    int index = Array.BinarySearch(primes, sum);
    if (index < 0)
    {
        index = ~index;
    }

    return Count(sum, index);
}

private static int Count(int sum, int index)
{
    if (sum == 0)
    {
        return 1;
    }

    if (sum < 0 || index >= primes.Length)
    {
        return 0;
    }

    return Count(sum - primes[index], index) + Count(sum, index + 1);
}
```
这里利用了 `Array.BinarySearch` 的一个性质，如果没找到，返回的是应该在的位置取反，那么，如果 $n$ 是质数，那么直接得到了 `index`，如果不是质数，能够得到小于 $n$ 的质数的位置。

有了 `Count` 函数，只需要从小往大遍历，找到第一个超过 5000 种分解形式的数。
``` csharp
private static int[] primes;
private static readonly int Max = 5000;

public static int GetAnswer()
{
    primes = Utils.GenPrimes(Max).Reverse().Select(l => (int)l).ToArray();
    for (int i = 2; i < Max; i++)
    {
        int count = Count(i);
        if (count >= Max)
        {
            return i;
        }
    }

    return 0;
}
```
