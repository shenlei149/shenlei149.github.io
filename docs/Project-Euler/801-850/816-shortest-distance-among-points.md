原题链接：https://projecteuler.net/problem=816

给定序列
$$s_0=290797$$
$$s_{n+1}=s_n^2 \text{ mod } 50515093$$
令二维点序列是
$$P_n=(s_{2n},s_{2n+1})$$
用 $d(k)$ 表示 $k$ 个点 $P_0,\cdots,P_{k-1}$ 任意不同的两点之间的最短距离。  
比如 $d(14)=546446.466846479$。这个可以小范围验证程序。  
求 $d(2000000)$。

求 $s_n$ 和 $P_n$ 都是线性的，没啥问题。关键是如何求 $d(n)$。

如果使用暴力法，每两个之间都计算距离，那么需要 $n^2$ 的时间开销，$2M * 2M$ 是一个很大的值，不现实。

于是乎我想了一个比较朴素的方法。  
对点按照 $x$ 排序，如果一样就按照 $y$ 排序。  
对于某个点 $p$，只需要考虑坐标轴右边的点 $p_o$ 和它的距离即可。  
如果最小距离 $d_{min}$ 已经比要考察的点 $p_o$ 和点 $p$ 的横坐标差值还小了，那么 $p_o$ 右边的点都不需要计算了。
```csharp
double distance = double.MaxValue;
for (int i = 0; i < P.Length - 1; i++)
{
    var p = P[i];
    for (int j = i + 1; j < P.Length; j++)
    {
        var other = P[j];
        if ((other.X - p.X) > distance)
        { break; }

        var d = p.Distance(other);
        if (d < distance)
        { distance = d; }
    }
}
```
跑了一下，速度很快，不到 1s 就能出结果~
