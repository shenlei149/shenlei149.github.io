[Problem 119](https://projecteuler.net/problem=119)

512 是一个很有意思的数字
$$5+1+2=8，8^3=512$$
同样地，
$$614656 = 28^4$$
$a_n$ 是满足以下条件的数列
* 至少有两位数
* 各个数字之和的若干次幂等于该数

求 $a_{30}$

对于这种题目，欧拉项目有个套路，你不太可能遍历每个数字去检查它是否满足条件的时候，就要换种思路，快速构造满足题目的集合，然后按照题目找到答案。

回到这个题目，我个人感觉 $a_{30}$ 不会超过 `long`能表示的范围，所以我只要检查 `long` 以内的数就好。

第一步，找到数字之和 `s` 的范围。最小值是 2，因为至少有两位数；最大值是 $9*19$，因为 `long.MaxValue` 有 19 位数字。其实这个范围比实际范围大一些，但是无所谓了，多的不多，不会影响程序效率。

第二步，求 `s` 的若干次幂，从 2 次幂开始，3 次幂，4 次幂等等，直到超出 `long` 的范围。这些都是可能的值 $n$。满足这一步的 $n$ 不到 1700 个。

第三步，求 $n$ 的各个数字之和 `sum`，如果 `sum` 等于 `s`，那么这个 $n$ 就是满足题意的。

第四步，把所有的 $n$ 排序，找出第三十个即可。很幸运，`long` 以内有 34 个满足题目的数。

下面是代码  
``` csharp
var candidates = new List<long>();
for (int i = 2; i < 9 * long.MaxValue.ToString().Length; i++)
{
    int power = 2;
    while (true)
    {
        var ret = BigInteger.Pow(i, power);
        if (ret > long.MaxValue)
        {
            break;
        }

        long number = (long)ret;
        long candidate = number;

        long sum = 0;
        while (number != 0)
        {
            sum += number % 10;
            number /= 10;
        }

        if (sum == i)
        {
            candidates.Add(candidate);
        }

        power++;
    }
}

candidates.Sort();

return candidates[29];
```