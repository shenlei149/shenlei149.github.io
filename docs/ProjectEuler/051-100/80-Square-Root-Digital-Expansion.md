[题目链接](https://projecteuler.net/problem=80 "Problem 80 - Project Euler")

众所周知，如果一个自然数的平方根不是整数，那么就是无理数。这样的平方根的小数部分是无限并且没有任何重复模式的。

2 的平方根是 $1.41421356237309504880\cdots$，小数部分的前一百位和是475。

对于前一百个自然数，找出其中无理平方根的小数部分前一百位的总和。

这个题的难度在于精确的计算出小数点后99位。首先想到的是，肯定要模拟手算来开平方，具体的方法请参考 [Wiki百科](http://zh.wikipedia.org/wiki/%E5%B9%B3%E6%96%B9%E6%A0%B9) 里面的长除式算法。

代码如下：
``` csharp
private static int SqrtDigitSum(int n)
{
    int result = 0;
    int integerPart = (int)Math.Sqrt(n);
    result += integerPart;

    BigInteger a = new BigInteger(integerPart);
    BigInteger remainder = new BigInteger(n - integerPart * integerPart);
    for (int i = 0; i < 99; i++)
    {
        remainder *= 100;
        int b = 1;
        while ((a * 20 + b) * b < remainder)
        {
            b++;
        }

        b--;

        remainder -= (a * 20 + b) * b;
        a = a * 10 + b;
        result += b;
    }

    return result;
}
```

根据算法，$a$ 和余数会越来越大，$a$ 的长度会趋于 100 位，余数的长度会趋于 200 位，所以我使用了 `BigInteger`来存储。`for` 循环做了 99 次，因为题目要求 100 个数字，而个位数已经占了一位了。

有了这个函数就可以求得给定整数开平方后的前一百位数字了。现在，只要写一个简单的循环就能得到最后的结果。
``` csharp
int sum = 0;
for (int i = 2; i < 100; i++)
{
    int sqrt = (int)Math.Sqrt(i);
    if (sqrt * sqrt == i)
    {
        continue;
    }

    sum += SqrtDigitSum(i);
}
```

在循环中，我排除了完全平方数，根据题意，它们是不计算到最后的和里面的。
