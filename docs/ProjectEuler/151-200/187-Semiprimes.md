[Problem 187](https://projecteuler.net/problem=187 "Problem 187 - Project Euler")

一个合数可以分解成至少两个质数的乘积，很多合数只能分解成两个质数的乘积，比如 $9 = 3 \times 3, 10 = 2 \times 5$，小于 $10^8$ 次方的数字中，有多少个能分解成两个质数相乘？9 和 10 就是满足题意的，而 $12 = 2 \times 2 \times 3$ 就是不满足题意的。

分解质因数是比较复杂的，但是给两个质数，得到乘积，看是否小于 `MAX` 还是很简单的。

进一步，我们没必要用两层 `for` 循环得到所有满足题意的数，只要知道有几个就可以了。

比如我们有一个质数 $p$，得到比 `MAX/p` 小的质数的个数就可以了。

先生成一个系列的质数，二分查找得到 `MAX/p` 所在的位置即可。
``` csharp
public static long GetAnswer()
{
    var primes = Utils.GenPrimes(MAX / 2);
    long count = 0;
    foreach (var p in primes)
    {
        long m = MAX / p;
        int index = Array.BinarySearch(primes, m);
        if (index >= 0)
        {
            count += index + 1;
        }
        else
        {
            count += ~index;
        }
    }

    count += ~Array.BinarySearch(primes, (long)Math.Sqrt(MAX));

    return count / 2;
}
```
对于非完全平方数，我们在 `foreach` 里面计算了 2 次，对于完全平方数，我们只计算了一次，于是乎，再加一次完全平方数的个数，最后统一的除以 2。
