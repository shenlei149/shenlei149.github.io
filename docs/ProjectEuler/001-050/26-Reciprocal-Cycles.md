对于有理数，分成有限小数和无限循环小数。比如 $1/3 = 0.33333\cdots$  
对于无限循环小数，循环节表示后续循环的部分。比如 $1/6 = 0.166666\cdots$，循环节是 6，长度是 1。$1/7 = 0.1428571428571428\cdots$，循环节是 142857，长度是 6。

[欧拉项目的第二十六题](https://projecteuler.net/problem=26 "Problem 26 - Project Euler")很有意思。基于以上知识，对于任意 $d < 1000$ ，找到分数 $1/d$ 循环节最长的那个 $d$ 。

如何计算循环节长度呢？  
想想我们手算，如果余数比除数小，我们会在后面补 0 然后再除。也就是说补零之后的数是下一次的被除数。如果被除数重复出现，除数确定的，那么商和余数也就是一样的。这时，循环节就出现了。

下面的函数分为两部分
1. 补零操作
2. 查找有没有同样的被除数存在，如果有，就找到了循环节，计算循环节长度并返回。在没有找到的前提下，把当前被除数记录下来，并得到余数作为下一次的被除数。

这里需要注意，如果某一次出现了除尽的情况，说明该分数是有限小数，循环节长度是 0。

下面是函数代码：
```csharp
// n/m
static int GetRecurringCycle(int n, int m)
{
    List<int> dividends = new List<int>();
    while (true)
    {
        while (n < m)
        {
            n *= 10;
        }

        // search pattern.
        int index = dividends.IndexOf(n);
        if (index >= 0)
        {
            return dividends.Count - index;
        }

        dividends.Add(n);
        n %= m;

        // n is divisible by m.
        if (n == 0)
        {
            return 0;
        }
    }
}
```

能够计算循环节长度，得到循环节最长的那个$d$就易如反掌了。
