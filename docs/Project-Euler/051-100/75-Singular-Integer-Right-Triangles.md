[Problem 75](https://projecteuler.net/problem=75 "Problem 75 - Project Euler")

给定一个整数 $L$，可能可以弯成一个各边都是整数的直角三角形，也可能不可以。如果可以的话，可能只有一种方法，也可能有多种。

比如 12，可以组成一个三边都是整数的直角三角形，且只能组出来一个，勾三股四弦五。
比如 10，无法弯出一个各边都是整数的直角三角形。

再比如 120，有多种组合 (30,40,50), (20,48,52), (24,45,51)。

$L \leq 1,500,000$，有多少个 $L$ 恰好有唯一一种方法得到直角三角形。

和 Project Euler 的很多题目一样，1500000 这个值很大，暴力的遍历每个 $L$，然后切分成三段，三段又恰好是直角三角形，这种方法时间复杂度极大，显然不可行。

另辟蹊径，我们先想办法找到所有的周长小于 $L$ 的直角三角形，然后遍历这些三角形，用一个 map 记住周长出现的次数，最后统计一次的个数即可。

现在的问题就是如果找到直角三角形呢？三边其实就是方程 $x^2 + y^2 = z^2$ 的整数解，解的通项公式是
$$\begin{cases}
x=kab\\
y=\frac{1}{2}k(a^2-b^2)\\
z=\frac{1}{2}k(a^2+b^2)
\end{cases}$$
其中，$a>b$ 且 $a,b$ 是互质的正奇数，$k\in N$。

下面实现了这个算法，不过 `a, b` 和上面公式恰好相反。
``` csharp
var countMapping = new Dictionary<long, int>();
for (long a = 1; a < L / 4; a += 2)
{
    for (long b = a + 2; a * b < L / 2; b += 2)
    {
        if (Utils.GetGcd(a, b) == 1)
        {
            for (long k = 1; k * a * b < L / 2; k++)
            {
                long x = k * a * b;
                long y = k * (b * b - a * a) / 2;
                long z = k * (b * b + a * a) / 2;
                long c = x + y + z;
                if (c <= L)
                {
                    if (countMapping.ContainsKey(c))
                    {
                        countMapping[c]++;
                    }
                    else
                    {
                        countMapping[c] = 1;
                    }
                }
            }
        }
    }
}

return countMapping.Count(p => p.Value == 1).ToString();
```
