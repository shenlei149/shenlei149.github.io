题目要求将零所在的行和列的所有数都设置成零。

粗一看，扫面矩阵，遇到 0 之后把所在行所在列的都设置成 0。但是，很快会发现，整个矩阵都是 0 了。这显然不符合题目的要求。

正确的做法应该是先扫描一次矩阵，看那些位置上出现了 0，我们不用精确的记录位置，只要记录要标记为 0 的行号和列号就好了，这里使用 `bool` 数组完成这件事。接着第二次扫描数组，根据第一次扫描记录的结果，把对应元素设置成 0 即可。

``` csharp
public static void SetZeros(int[,] matrix)
{
    int m = matrix.GetLength(0);
    int n = matrix.GetLength(1);
    bool[] row = new bool[m];
    bool[] col = new bool[n];

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i, j] == 0)
            {
                row[i] = true;
                col[j] = true;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (row[i] || col[j])
            {
                matrix[i, j] = 0;
            }
        }
    }
}
```