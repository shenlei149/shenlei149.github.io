[Problem 43](https://projecteuler.net/problem=43 "Problem 43 - Project Euler")：

数字 1406357289 是由 0-9 组成的，说明是一个 pandigital number（全位数？）。除此之外，它还有一个很好的性质，描述如下：

设 $d_1$ 是第一个数字，$d_2$ 是第二个数字，以此类推。有以下整除性质：
$$
\begin{aligned}
d_2d_3d_4&=406 \text{ is divisible by 2}\\
d_3d_4d_5&=063 \text{ is divisible by 3}\\
d_4d_5d_6&=635 \text{ is divisible by 5}\\
d_5d_6d_7&=357 \text{ is divisible by 7}\\
d_6d_7d_8&=572 \text{ is divisible by 11}\\
d_7d_8d_9&=728 \text{ is divisible by 13}\\
d_8d_9d_{10}&=289 \text{ is divisible by 17}\\
\end{aligned}
$$
问题是，求所有满足这种性质的 pandigital number 之和。

一开始，想了一个最直接的想法，找到所有的 pandigital number，其实就是一个全排列算法，然后从里面过滤出满足性质的数字，最后求和。算法复杂度正比于 $9 \times 9!$！

本地实验了一下，100 多秒得到了正确的结果。不过，网站上有说明，算法足够好，小于 1 分钟。其实大部分问题应该在 1s 内就有结果。

后来我发现，若干位整除是独立开的，那么，可以一步一步构造数字，比如先找到 3 位数字里面满足整除 17 要求的数字，在上一步的基础上，最前面添加一位数字，再要求整除 13，以此类推，最后得到整个数字。需要 8 次迭代，但是每次的结果集都非常小，所以速度非常快，十几毫秒就能得到结果。

下面是写代码。首先定义了数字这个数组：
``` csharp
var digits = new string[] { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
```
找到能够整除 17 的。
``` csharp
var d3 = (from i in digits
          from j in digits
          from k in digits
          select i + j + k).Where(IsPandigital).Where(s => int.Parse(s) % 17 == 0).ToArray();
```
接着，往前面添加一个数字，使之成为 4 位数，同时，筛选出指定三位能整除 13 的数字：
``` csharp
var d4 = (from i in digits
          from n in d3
          select i + n).Where(IsPandigital).Where(s => int.Parse(s.Substring(0, 3)) % 13 == 0).ToArray();
```
下面是以此类推，最后拼接出满足条件的 10 位数：
``` csharp
var d5 = (from i in digits
          from n in d4
          select i + n).Where(IsPandigital).Where(s => int.Parse(s.Substring(0, 3)) % 11 == 0).ToArray();

var d6 = (from i in digits
          from n in d5
          select i + n).Where(IsPandigital).Where(s => int.Parse(s.Substring(0, 3)) % 7 == 0).ToArray();

var d7 = (from i in digits
          from n in d6
          select i + n).Where(IsPandigital).Where(s => int.Parse(s[2].ToString()) % 5 == 0).ToArray();

var d8 = (from i in digits
          from n in d7
          select i + n).Where(IsPandigital).Where(s => int.Parse(s.Substring(0, 3)) % 3 == 0).ToArray();

var d9 = (from i in digits
          from n in d8
          select i + n).Where(IsPandigital).Where(s => int.Parse(s[2].ToString()) % 2 == 0).ToArray();

var d10 = (from i in digits
           from n in d9
           select i + n).Where(IsPandigital).Where(s => s[0] != 0).ToArray();
```
最后，我们返回和：
``` csharp
return d10.Select(s => long.Parse(s)).ToArray().Sum();
```

Where中使用的方法如下，作用是判断一个数是否是 pandigital number：
``` csharp
private static bool IsPandigital(string s)
{
    return s.Length == s.ToArray().Distinct().Count();
}
```
