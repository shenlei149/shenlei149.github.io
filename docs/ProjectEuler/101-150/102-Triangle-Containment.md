[Problem 102](https://projecteuler.net/problem=102 "Problem 102 - Project Euler")

在笛卡尔平面上随机取三个不同的点，其中 $-1000\leq x, y\leq 1000$，会形成一个三角形。

考虑如下两个三角形：
$$A(-340,495), B(-153,-910), C(835,-947)$$
$$X(-175,41), Y(-421,-714), Z(574,-645)$$
可以证明原点在三角形 ABC 内部，而三角形XYZ则不包含原点。
题目中给了一个文件，triangles.txt，包含一千个随机三角形，求其中有多少个三角形的内部包含原点。

如何判断一个点是不是在某个三角形内呢？

一个不太笨的方法是，如果点 C 和点 O 在直线 AB 的同侧，点 B 和点 O 在直线 AC 的同侧，点 A 和点 O 在直线 BC 同侧，那么点 O 就在三角形 ABC 里面。

好了，问题转化为，如何判断两个点在某直线的同侧。

算法导论中有说这个问题，向量 AB 和向量 AC 叉积，向量 AB 和向量 AO 叉积，如果两个叉积结果的符号相同，那么说明点 C 和点 O 在 AB 的同侧。其实这就是叉积右手定则的体现，如果在一侧，那么四指从 AB 向 AC 或 AO 旋转，大拇指方向不变，所有符号相同。

思路如此，下面是代码。
``` csharp
private static bool IsSameSide(int xa, int ya, int xb, int yb, int xc, int yc, int xo, int yo)
{
    return (long)CrossProduct(xa, ya, xb, yb, xc, yc) * CrossProduct(xa, ya, xb, yb, xo, yo) > 0;
}

private static int CrossProduct(int xa, int ya, int xb, int yb, int xc, int yc)
{
    int xv = xb - xa;
    int yv = yb - ya;
    int xu = xc - xa;
    int yu = yc - ya;
    return xv * yu - yv * xu;
}
```
需要解释的是，坐标小于 1000，叉积结果大约是十的六次方数量级，`IsSameSide` 判断的时候如果不强转成 `long` 型，可能会溢出，造成判断错误。

给出的数据比较大，需要写一点点代码去解析它们。
``` csharp
int count = 0;
string[] triangles = Triangles.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

int xo = 0;
int yo = 0;

foreach (string triangle in triangles)
{
    int[] tmp = triangle.Split(',').Select(s => int.Parse(s)).ToArray();

    int xa = tmp[0];
    int ya = tmp[1];
    int xb = tmp[2];
    int yb = tmp[3];
    int xc = tmp[4];
    int yc = tmp[5];

    if (IsSameSide(xa, ya, xb, yb, xc, yc, xo, yo)
        && IsSameSide(xa, ya, xc, yc, xb, yb, xo, yo)
        && IsSameSide(xb, yb, xc, yc, xa, ya, xo, yo))
    {
        count++;
    }
}

return count;
```
