一个 $X \times Y$ 的矩阵，机器人只能往右走或者往下走，从点 $(0,0)$ 到点 $(X,Y)$，一共有多少种通路呢？

增加一点难度，假设有些点是不能走的，那么应该如何设计算法来找到一条可能的通路呢？

第一个问题其实很简单，就是从 $X+Y$ 这么多步数中选择 $X$ 步往右走，即
$$\begin{pmatrix}
X+Y\\X
\end{pmatrix}= \frac{(X+Y)!}{X!Y!}$$

如果找到这条通路呢？其实有没有不可走的点，差距不是很大，就是在判断往哪个方向走的时候增加一个条件判断就好了。还有一个大一点的区别，就是某些点不能走，导致从 $(0,0)$ 点无法到达一部分点。

现在我们分析一下思路，怎么走到点 $(X,Y)$ 呢？如果到了 $(X, Y-1)$ 或者是 $(X-1,Y)$，那么再走一步就能到 $(X,Y)$ 了。问题变成了，找到一条到达 $(X, Y-1)$或 $(X-1,Y)$ 的路径。

如何到 $(X, Y-1)$ 呢？再往前想一步，找一条到 $(X-1,Y-1)$ 或者是 $(X,Y-2)$ 的路径即可。

如何到 $(X-1, Y)$ 呢？那么先找到一条到 $(X-1,Y-1)$ 或者是(X-2,Y)的路径即可。

总体来说，就是一个递归算法。

仔细想一下，上述的递归算法，都需要计算到 $(X-1,Y-1)$ 点的路径，那么，我们可以考虑把这个结果缓存起来，第二次使用的时候，就不用再次递归计算了。

具体到这个题目，只需要缓存从 $(0,0)$ 能否到点 $(M,N)$ 即可。

``` csharp
struct Point
{
    public Point(int x, int y)
    {
        X = x;
        Y = y;
    }

    public int X { get; set; }
    public int Y { get; set; }
}

static bool GetPath(int x, int y, List<Point> path, Dictionary<Point, bool> cache)
{
    Point point = new Point(x, y);
    if (cache.ContainsKey(point))
    {
        // Already visited this cell
        return cache[point];
    }

    if (x == 0 && y == 0)
    {
        // found a path
        return true;
    }

    bool success = false;

    // Try left
    if (x >= 1 && IsFree(x - 1, y))
    {
        success = GetPath(x - 1, y, path, cache);
    }

    // Try up
    if (!success && y >= 1 && IsFree(x, y - 1))
    {
        success = GetPath(x, y - 1, path, cache);
    }

    if (success)
    {
        // Right way! Add to path
        path.Add(point);
    }

    // Cache result
    cache.Add(point, success);

    return success;
}

private static bool IsFree(int x, int y)
{
    return !(x == 0 && y == 1
           || x == 0 && y == 2
           || x == 1 && y == 2
           || x == 2 && y == 2);
}
```