[Problem 120](http://projecteuler.net/problem=120 "Problem 120 - Project Euler")

令 $r$ 是 $(a-1)^n + (a+1)^n$ 除以 $a^2$ 的余数。随着 $n$ 的变化，$r$ 也会变化，但是众多 $r$ 一定存在最大值 $r_{\max}$。

当 $3\leq a\leq 1000$ 时，求 $\sum r_{\max}$。

给定一个 $a，n$ 取何值时能让 $(a-1)^n + (a+1)^n \operatorname{mod} a^2$ 最大呢？

首先考虑n是偶数的时候。
$$(a-1)^n + (a+1)^n  
=2(a^n + C(n,2)a^{(n-2)} + C(n,4)a^{(n-4)} + \cdots + 1)$$
由于 $n$ 是偶数，$a^n$ 能够被 $a^2$ 整除，同理，$a^{(n-2)}, a^{(n-4)},\cdots ,a^2$ 都可以被 $a^2$ 整除。
所以，最后的余数是 1，显然不大，下面我们考虑 $n$ 是奇数的时候。
$$(a-1)^n + (a+1)^n  
= 2(a^n + C(n,2)a^{(n-2)} + C(n,4)a^{(n-4)} + \cdots + C(n,n-1)a)$$
$a^n$ 模 $a^2$ 余数是 $a$，同理，$a^{(n-2)},\cdots,a$ 模 $a^2$ 都余 $a$。

那么上面的数模 $a^2$ 等于
$$2a(1+C(n,2)+C(n,4)+\cdots+C(n,n-1)) = 2af(n)$$

想要 $2af(n)$ 模 $a^2$ 余数最大呢，让 $2af(n)$ 越接近 $a^2$ 越好，也就是 $2f(n)$ 越接近 $a$ 越好，但是不能等于 $a$。

$2f(n)$ 一定是偶数。所以，当 $a$ 是奇数的时候，$2f(n)=a-1$，当 $a$ 是偶数的时候，$2f(n)=a-2$。

有了以上的分析，很容易就能写出获取 $r_{\max}$ 的函数：
``` csharp
private static int GetMaxRByA(int a)
{
	return (a - 1) / 2 * 2 * a;
}
```

离最后的答案只差一步
``` csharp
public static int GetAnswer()
{
    int sum = 0;
    for (int a = 3; a <= 1000; a++)
    {
        sum += GetMaxRByA(a);
    }

    return sum.ToString();
}
```

有了数学分析得到的结论之后，程序的时间复杂度是 $O(n)$，理论最低值，而且这里 $n$ 就 1000，0ms 就完成了计算。
