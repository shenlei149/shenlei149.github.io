[Problem 179](https://projecteuler.net/problem=179 "Problem 179 - Project Euler")

14 和 15 有同样个数的正因数，14 有四个，1,2,7,14；15 也有四个，1,3,5,15。

求在 1 到 $10^7$ 次方之间，有多少个有相同个数正因数的连续的整数？

正向求解的方式就是分解质因数，然后计算得到因数个数，这个操作看似很快，但是还是稍微有点耗时，感觉不太行。

于是乎，走向另外一条路，每找到一个因数，就加一。
``` csharp
int[] numberOfDivisors = new int[N + 1];
for (int i = 1; i <= N; i++)
{
    for (int j = i; j <= N; j += i)
    {
        numberOfDivisors[j]++;
    }
}
```
`i` 就是因数，若干倍的 `j` 就包含一个因数 `i` 了。

知道了每个数的因数个数就简单了，看连续两个数的因数个数是否相等即可。
``` csharp
int count = 0;
for (int i = 2; i < N; i++)
{
    if (numberOfDivisors[i] == numberOfDivisors[i + 1])
    {
        count++;
    }
}

return count;
```
