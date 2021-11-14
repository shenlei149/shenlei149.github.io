原题链接：https://projecteuler.net/problem=800

一个整数形如 $p^qq^p,p\neq q$ 称为 `hybrid-integer`。  
比如 $800 = 2^55^2$ 就是 `hybrid-integer`。  
定义 $C(n)$ 是小于等于 $n$ 的 `hybrid-integer` 个数。  
比如 $C(800)=2, C(800^{800})=10790$。  
求 $C(800800^{800800})$。

这个题目可以使用 `double` 这种非精确的小数表示，因为 $800^{800}, 800800^{800800}$ 这两个边界值不是 `hybrid-integer`。前者可以写作
$$800^{800}=(2^55^2)^{800}=2^{4000}5^{1600}$$
而后者的质因数有 $2,5,7,11,13$。

不妨设 $p<q$，给定 $p$，有多少个 `hybrid-integer` 呢？也就是求满足下面条件 $q$ 的个数。
$$p^qq^p<n$$
将 $q$ 看做是变量，这是一个递增函数，也就是要找到最小的不满足条件的 $q$，然后 $p,q$ 之间的质数个数即可。

两边取对数（底数不重要）
$$\begin{aligned}
\ln p^qq^p&<\ln n=\ln m^m\\
q\ln p+p\ln q&<m\ln m
\end{aligned}$$
左边第二项是正数，可以稍微放宽条件，将其减少一点
$$\begin{aligned}
q\ln p&<m\ln m-p\ln p\\
q&<\frac{m\ln m}{\ln p}-p
\end{aligned}$$

由于需要得到 $p,q$ 之间的质数个数，需要一个质数的列表。在计算之前，我们需要知道需要多少个质数。使用 2 作为底数，极端情况下
$$q<\frac{800800\log_2 800800}{\log_2 2}=800800\log_2 800800=15704554.8<15710000$$
后面的事情就比较简单了。  
1. $p$ 从 2 开始遍历，直到 $q<=p$
2. 根据 $p$ 计算 $q$，这时 $q$ 有可能不满足条件，太大了，然后开始往下减直到满足条件
3. 计算当前 $p, q$ 之间的质数个数

```csharp
var primes = Utils.GenPrimes(15710000).Where(l => l != 0).ToArray();

long count = 0;
int pindex = 0;
while (true)
{
    int p = (int)primes[pindex];
    int q = (int)(M * Math.Log(M, p)) + 1;
    if (q <= p)
    { break; }

    int qindex = Array.BinarySearch(primes, q);
    if (qindex < 0)
        qindex = ~qindex;

    q = (int)primes[qindex];
    while (true)
    {
        if (q * Math.Log(p) + p * Math.Log(q) < M * Math.Log(M))
        { break; }

        qindex--;
        q = (int)primes[qindex];

        if (qindex <= pindex)
        { break; }
    }

    if (qindex > pindex)
        count += qindex - pindex;

    pindex++;
}
```
能够得到正确结果，不过比较慢。  
其实 `Math.Log` 不需要反复计算。
```csharp
if (q * Math.Log(p) + p * Math.Log(q) < M * Math.Log(M))
```
右边的是个定值，计算一次即可，左边的 $p, q$ 就是最开始生成的质数，那么可以提前计算好，后面用就好了。
```csharp
double[] logs = new double[primes.Length];
for (int i = 0; i < primes.Length; i++)
{
    logs[i] = Math.Log(primes[i]);
}

var limit = M * Math.Log(M);
```
