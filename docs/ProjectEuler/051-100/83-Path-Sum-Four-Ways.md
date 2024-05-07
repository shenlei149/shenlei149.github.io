[Problem 83](https://projecteuler.net/problem=83)

一个整数矩阵，从左上角到右下角，存在一条最短路径。走的方式没有要求，在某个点，可以往四个方向走；之前有两个题目，一个是限制只能往右或者往下，稍微复杂点的另一个题目的限制是不能往左。

我的想法很直接，构造一个矩阵，是从左上角到该点的最短路径，每个点的初始值都是 `int.MaxValue`。用一个 `queue` 来保存被更新的点，然后看看其周围的点是否可以被更新。

从左上角的点开始，赋值成给定矩阵左上角的值。该值被更新了，把 `(0, 0)` 点放入 `queue` 里，因为周围的点可能会被更新。然后从 `queue` 拿出一个点，看看周围的值是否能被更新，如果是，也放到 `queue` 里。循环往复，直到 `queue` 为空，没有需要被更新的点了。

此时，每个点上的值都是从左上角开始到该点的最短路径。

代码直接反应了上述的过程。
``` csharp
private const int N = 80;

int[,] sum = new int[N, N];
for (int i = 0; i < N; i++)
{
    for (int j = 0; j < N; j++)
    {
        sum[i, j] = int.MaxValue;
    }
}

sum[0, 0] = data[0, 0];
Queue<Tuple<int, int>> pointsToBeUpdated = new Queue<Tuple<int, int>>();
pointsToBeUpdated.Enqueue(new Tuple<int, int>(0, 0));
while (pointsToBeUpdated.Count != 0)
{
    var point = pointsToBeUpdated.Dequeue();
    if (CanMoveToLeft(point))
    {
        int newValue = sum[point.Item1, point.Item2] + data[point.Item1 - 1, point.Item2];
        if (newValue < sum[point.Item1 - 1, point.Item2])
        {
            sum[point.Item1 - 1, point.Item2] = newValue;
            pointsToBeUpdated.Enqueue(new Tuple<int, int>(point.Item1 - 1, point.Item2));
        }
    }

    if (CanMoveToRight(point))
    {
        int newValue = sum[point.Item1, point.Item2] + data[point.Item1 + 1, point.Item2];
        if (newValue < sum[point.Item1 + 1, point.Item2])
        {
            sum[point.Item1 + 1, point.Item2] = newValue;
            pointsToBeUpdated.Enqueue(new Tuple<int, int>(point.Item1 + 1, point.Item2));
        }
    }

    if (CanUp(point))
    {
        int newValue = sum[point.Item1, point.Item2] + data[point.Item1, point.Item2 - 1];
        if (newValue < sum[point.Item1, point.Item2 - 1])
        {
            sum[point.Item1, point.Item2 - 1] = newValue;
            pointsToBeUpdated.Enqueue(new Tuple<int, int>(point.Item1, point.Item2 - 1));
        }
    }

    if (CanDown(point))
    {
        int newValue = sum[point.Item1, point.Item2] + data[point.Item1, point.Item2 + 1];
        if (newValue < sum[point.Item1, point.Item2 + 1])
        {
            sum[point.Item1, point.Item2 + 1] = newValue;
            pointsToBeUpdated.Enqueue(new Tuple<int, int>(point.Item1, point.Item2 + 1));
        }
    }
}

return sum[N - 1, N - 1];
```
