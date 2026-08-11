[Problem 86](https://projecteuler.net/problem=86)

小蜘蛛从立方体的一点走最短距离爬到对顶点，爬过的长度可能是整数，比如立方体三维分别是 6，5，3，那么最短距离是 10，但也不总是整数。  
对于三边最长是 $M\times M\times M$ 的立方体，当 $M=100$ 时，有 2060 个边长均为整数的不一样的立方体，其蜘蛛爬过的最短距离是整数，而 $M=99$ 时，有 1975 个不同的立方体。  
求满足上述条件的立方体个数超过一百万时 $M$ 的最小值。

如果能够在不需要计算出具体满足题意的值的情况下，只通过计数得到结果，那么耗时将相当短。

不妨设三边和最大值满足关系 $M=a\geq b\geq c$，那么 $2\leq b+c\leq 2a$，最短路径的长度是 $\sqrt{a^2+(b+c)^2}$，如果最短路径是整数，那么 $b$ 和 $c$ 可以在一定范围内变化。$b$ 最小也就是 $b+c$ 的一半或者一半加一，最长的话就是等于 $a$，同时要满足 $c\geq 1$。所以很容易通过某个给定的 $a$ 来得到满足条件的立方体个数。
``` csharp
int GetCountByA(int a)
{
    int count = 0;
    for (int bc = 2; bc <= 2 * a; bc++)
    {
        int slope = (int)Math.Sqrt(a * a + bc * bc);
        if (a * a + bc * bc == slope * slope)
        {
            int bStart = bc % 2 == 0 ? bc / 2 : bc / 2 + 1;
            int bEnd = Math.Min(a, bc - 1);
            count += bEnd - bStart + 1;
        }
    }

    return count;
}
```

对 $a$，也就是 $M$ 进行遍历，当总数大于一百万时停止，得到要求的 $M$。
``` csharp
int count = 0;
int a = 2;
while (count < 1_000_000)
{
    a++;
    count += GetCountByA(a);
}
```
