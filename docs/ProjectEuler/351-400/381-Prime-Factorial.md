[Problem 381](https://projecteuler.net/problem=381 "Problem 381 - Project Euler")

对于一个质数 $p$ 来说，$S(p) = (\sum(p-k)!)\operatorname{mod}(p), 1\leq k\leq 5$。

举个例子
$$(7-1)! + (7-2)! + (7-3)! + (7-4)! + (7-5)! = 6! + 5! + 4! + 3! + 2! = 720+120+24+6+2 = 872$$
因为 $872\operatorname{mod}(7) = 4$， 所以 $S(7) = 4$。

对于 $5\leq p < 10^8$，求 $\sum S(p)$。

题目给了一个可以检验程序是否写正确的示例：$\sum S(p) = 480,5 ≤ p < 100$。

根据 [Wilson's theorem](https://en.wikipedia.org/wiki/Wilson%27s_theorem "Wilson's theorem") 可以得到下面等式：
$$
\begin{aligned}
(p-1)!&\equiv -1\operatorname{mod} (p)\\
(p-2)!&\equiv (p-1)!(p-1)^{-1}\equiv 1\operatorname{mod} (p)\\
(p-3)!&\equiv (p-2)!(p-2)^{-1}\equiv (p-2)^{-1}\operatorname{mod} (p)\\
(p-4)!&\equiv (p-3)!(p-3)^{-1}\equiv (p-2)^{-1} \times (p-3)^{-1}\operatorname{mod} (p)\\
(p-5)!&\equiv (p-4)!(p-4)^{-1}\equiv (p-2)^{-1} \times (p-3)^{-1} \times (p-4)^{-1}\operatorname{mod} (p)
\end{aligned}
$$

$(p-2)^{-1} (p-3)^{-1} (p-4)^{-1}$ 这玩意如何整成整数呢？

下面给出逆元的定义：

对于整数 $n,p$，如果存在整数 $b$，满足 $nb\operatorname{mod} p =1$，则说，$b$ 是 $n$ 的模 $p$ 乘法逆元。并且 $n$ 存在模 $p$ 的乘法逆元的充要条件是 $\gcd(n,p) = 1$。
显然，$p-2, p-3, p-4$ 与 $p$ 互质。

那么如何求逆元呢？考虑 [扩展欧几里得算法](../../Math/200-Discrete-Mathematics-and-Its-Applications/040-Number-Theory-and-Cryptography/030-Primes-and-Greatest-Common-Divisors.md)
$$ap + b(p-2) = 1$$
显然，$p-2$ 的逆元就是扩展欧几里得算法中的 $b$。

到此为止，基础的数学知识都已经准备妥当，开始写代码吧。

``` csharp
var primes = Utils.GenPrimes(100000000);
for (int i = 2; i < primes.Length; i++)
{
    long p = primes[i];
    long a = 0;
    long p2 = 0;
    long p3 = 0;
    long p4 = 0;
    Utils.GetGcd(p, p - 2, ref a, ref p2);
    Utils.GetGcd(p, p - 3, ref a, ref p3);
    Utils.GetGcd(p, p - 4, ref a, ref p4);

    long s3 = ((p2 % p) + p) % p;
    long s4 = ((s3 * p3) % p + p) % p;
    long s5 = ((s4 * p4) % p + p) % p;
    long s = ((s3 + s4 + s5) % p + p) % p;
    sum += s;
}

return sum;
```
简单的解释一下，`primes` 是 2 到 $10^8$ 所有的质数，利用的是埃拉托斯特尼筛选法。

由于得到的逆元可能很大，也可能是负数，所以模 $p$ 后加了 $p$ 再进行一次模运算。
