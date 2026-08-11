[Problem 932](https://projecteuler.net/problem=932 "Problem 932 - Project Euler")

一堆牌，有 $R$ 个红色牌和 $B$ 个红色牌。等概率不放回的连续抽取两张牌。

* 如果两张都是红色的，那么都弃掉。
* 如果两张都是黑色的，放回到牌堆。
* 如果一张红色一张黑色，红色的放回牌堆，黑色的弃掉。

直到牌堆只剩一种颜色的牌。令 $P(R,B)$ 表示都是黑色牌的概率。

题目给了几个 $R,B$ 很小对应的 $P$ 值用于验证代码的正确性。$P(2,2)=0.4666666667,P(10,9)=0.4118903397,P(34,25)=0.3665688069$。

求 $P(24690,12345)$。结果保留小数点后 10 个数字。

先找到递归关系。根据题意
$$\begin{aligned}
P(R,B)&=\frac{B(B-1)}{(R+B)(R+B-1)}P(R,B)\\
&+\frac{R(R-1)}{(R+B)(R+B-1)}P(R-2,B)\\
&+\frac{2RB}{(R+B)(R+B-1)}P(R,B-1)
\end{aligned}$$

表达递归关系最简单的方式就是实现递归函数。为了不重复计算这里使用 `double?` 来保存数据，如果有值就直接返回，否则计算对应的概率。
```csharp
static readonly double?[,] p = new double?[RED + 1, BLACK + 1];

public string GetAnswer()
{
    return GetPBy(RED, BLACK).ToString("F10");
}

double GetPBy(int red, int black)
{
    if (p[red, black].HasValue)
    {
        return p[red, black].Value;
    }

    if (red == 0 && black == 0)
    {
        Console.WriteLine("should NOT be here...");
    }

    if (red == 0)
    {
        p[red, black] = 1;
        return p[red, black].Value;
    }

    if (black == 0)
    {
        p[red, black] = 0;
        return p[red, black].Value;
    }

    double p_r_b = GetPBy(red, black - 1);
    double r_b_choice_p = 2.0 * red * black / (red + black) / (red + black - 1);

    double p_r_r = 0;
    double r_r_choice_p = 0;
    if (red >= 2)
    {
        p_r_r = GetPBy(red - 2, black);
        r_r_choice_p = 1.0 * red * (red - 1) / (red + black) / (red + black - 1);
    }

    double b_b_choice_p = 0;
    if (black >= 2)
    {
        b_b_choice_p = 1.0 * black * (black - 1) / (red + black) / (red + black - 1);
    }

    double p_b_b = (p_r_b * r_b_choice_p + p_r_r * r_r_choice_p) / (1.0 - b_b_choice_p);
    p[red, black] = p_b_b;
    return p[red, black].Value;
}
```
对于题目给出的三个小规模数，上述代码能给出正确结果，但是对于计算 $P(24690,12345)$，会 OOM。

这样就不能用对齐的二维数组了，也不能一次性分配这么多空间。而是对红色 $R$ 遍历，从 0 到 $R+1$，一个数组一个数组的计算，同时，由于递归式子只依赖于 $R-2$，那么更早的数组就可以置 `null` 释放内存。
```csharp
static readonly double?[][] p = new double?[RED + 1][];

public string GetAnswer()
{
    for (int r = 0; r < RED + 1; r++)
    {
        p[r] = new double?[BLACK + 1];
        for (int b = 0; b < BLACK + 1; b++)
        {
            GetPBy(r, b);
        }

        if (r >= 3)
        {
            p[r - 3] = null;
        }
    }

    return GetPBy(RED, BLACK).ToString("F10");
}
```
这样就能计算得到 $P(24690,12345)$，在我的虚拟机上需要 11s+，稍微有点慢。

仔细思考，既然已经逐层计算了，那么需要的 $P(R,B)$ 的值一定存在，因此无需使用 `double?` 类型而直接使用 `double`，这样省去大量内存，性能还能提速近 50%。

同时，`GetPBy` 中几个 `if` 判断边界条件，完全可以在调用之前处理一下。另外，概率的分母重复计算了；`if (black >= 2)` 的判断也不需要，假定 `black` 是 1，那么 `b_b_choice_p` 是零，符合要求。这些优化使性能得到些许提高。
