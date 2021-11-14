[104题链接](https://projecteuler.net/problem=104 "Problem 104 - Project Euler")

这里就不赘述什么是斐波那契数列了。

$F(541)$ 包含 113 个数字，最后九个数字恰好是由数字 1-9 组成的；$F(2749)$ 包含 575 个数字，前九个数字恰好是由数字 1-9 组成的。

求 $k$，使得 $F(k)$ 是第一个前九个数字和最后九个数字都是由数字 1-9 组成的。

我一开始的思路是使用 `BigInteger` 来存放斐波那契数，逐个计算上去，判断前九个数字和最后九个数字是否是由 1-9 组成的。

不过我发现程序很慢，分析了一下程序的性能，发现 `BigInteger.Log10` 和 `BigInteger.Pow` 非常耗时。我用 `BigInteger.Log10` 计算出数字的长度 `L`，然后除以 `BigInteger.Pow(10, L - 9)` 得到最前面的九个数字。

稍微改变了一下策略，先判断最后九个数字，后判断前九个数字。速度提高很多，因为判断最后九个数字，只要模 10 的九次方就行了，比计算长度再除快非常多，而需要进行后一个判断的几率很小。于是乎，大约 20 秒能解决。

能不能再快一点呢？我发现 `BigInteger` 的模运算也挺慢的。其实我可以用一个长度为九的数组进行最后九位的加法，然后判断是否是由 1-9 九个数字组成就好。
``` csharp
private static int[] Add(int[] fn_1, int[] fn_2)
{
    int[] ret = new int[9];
    int n1 = fn_1.Length - 1;
    int n2 = fn_2.Length - 1;
    int nr = ret.Length - 1;
    int carry = 0;
    for (; nr >= 0; n1--, n2--, nr--)
    {
        int a = n1 >= 0 ? fn_1[n1] : 0;
        int b = n2 >= 0 ? fn_2[n2] : 0;
        int sum = a + b + carry;
        ret[nr] = sum % 10;
        carry = sum / 10;
    }

    return ret;
}
```
判断是否是由数字1-9组成也很简单。
``` csharp
private static bool IsPandigitalFibonacci(int[] fn)
{
    int[] digitalCounts = new int[10];
    for (int i = 0; i < 9; i++)
    {
        digitalCounts[fn[i]]++;
    }

    for (int i = 1; i < 10; i++)
    {
        if (digitalCounts[i] != 1)
        {
            return false;
        }
    }

    return true;
}
```
使用这个方式来代替原来的想法，得到了完整的算法。
``` csharp
int[] fn_2 = new int[] { 1 };
int[] fn_1 = new int[] { 1 };
BigInteger f1 = 1;
BigInteger f2 = 1;
int i = 3;
while (true)
{
    BigInteger fnBig = f1 + f2;
    int[] fn = Add(fn_1, fn_2);
    if (IsPandigitalFibonacci(fn))
    {
        int digits = (int)Math.Floor(BigInteger.Log10(fnBig) + 1);
        var dividend = BigInteger.Pow(10, digits - 9);
        var firstNineDigits = fnBig / dividend;
        if (Utils.IsPandigital(firstNineDigits.ToString(), false))
        {
            return i;
        }
    }

    fn_2 = fn_1;
    fn_1 = fn;
    f2 = f1;
    f1 = fnBig;
    i++;
}
```
这比以前又快了很多，大约 2.5 秒就能搞定。

分析下程序的瓶颈，发现最耗时的是 `BigInteger.Add`，这个没办法优化了，除非和尾部的九个数字一样，能有个算法能够直接得到前九个数字是什么，这样就不要 `BigInteger` 来保存结果，我觉得没有这种算法；次耗时的是 `BigInteger.Pow`，它的调用次数已经优化了，而且我也没有想到更好的构造被除数的方式。
