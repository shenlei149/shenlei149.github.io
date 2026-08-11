[Problem 72](https://projecteuler.net/problem=72 "Problem 72 - Project Euler")

考虑分数 $n/d$, 其中 $n$ 和 $d$ 是正整数。如果 $n < d$ 并且最大公因数 $\text{HCF}(n,d)=1$, 它被称作一个最简真分数。

如果我们将 $d \leq 8$ 的最简真分数按照大小的升序列出来，我们得到：
$$1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5 , 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8$$
可知该集合中共有 21 个元素。

$d\leq 1,000,000$ 的最简真分数集合中包含多少个元素？

如果写代码就是遍历 $d$，然后针对每个 $d$ 求 1 到 $d-1$ 中有多少个数与 $d$ 互质（互质才能是最简真分数），求和。

假设我们有一个函数能够得到互质数的个数，那么这个题目很容易解决，代码如下：
``` csharp
long count = 0;
for (int d = 2; d <= 1000000; d++)
{
    count += Utils.GetCoprimeCount(d);
}

return count;
```

现在关键问题是，给定一个数 $n$，如何求出 1 到 $n-1$ 中，有多少个数与之互质。

欧拉函数描述了这个问题的解：
$$
\varphi(x)=x(1-1/p_1)(1-1/p_2)(1-1/p_3)(1-1/p_4)\cdots(1-1/p_n)
$$
其中 $p_1, p_2, \ldots, p_n$ 为 $x$ 的所有质因数。
又
$$
x={p_1}^{k_1} \cdot {p_2}^{k_2} \cdot \cdots \cdot {p_n}^{k_n}
$$
所以
$$\varphi(x)=\prod_{i=1}^n (p_i-1)*{p_i}^{k_i-1}$$

开始写代码实现欧拉函数求互质数的个数：
``` csharp
public static int GetCoprimeCount(int n)
{
    int ret = 1;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            n /= i;
            ret *= i - 1;
            while (n % i == 0)
            {
                n /= i;
                ret *= i;
            }
        }
    }

    if (n > 1)
    {
        ret *= n - 1;
    }

    return ret;
}
```
简单解释两点。

`if (n % i == 0)` 里面的 `ret *= i - 1;` 相当于 $\prod_{i=1}^n (p_i-1)*{p_i}^{k_i-1}$ 里面的 $(p_i-1)$，`while` 里面的 `ret *= i;` 相当于 $p_i$，整个 `while` 循环就相当于 ${p_i}^{k_i-1}$。

后面的 `if` 是判断，如果 $n > 1$，那么剩余的 $n$ 肯定是质数，`ret *= n - 1;` 就相当于 $p_i-1$，与之对应的 $k_i$是 1。
