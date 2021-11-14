[原题链接](https://projecteuler.net/problem=82)

从矩阵的左边一行的任意一个元素开始，可以往上、往下、往右前进，到达最右边的某个元素为止。由此形成了一条路径，要求使得该路径上的数字之和最小。

题中的 $5\times 5$ 的矩阵可以用于测试和 debug，最后要求给出答案的是一个 $80\times 80$ 的矩阵。

这是一道典型的动态规划题目。

初始化一个 $80\times 80$ 的二维数组用于保存结果。第一列就是矩阵的第一列。

然后从 $j - 1$ 列向 $j$ 列前进。

考虑第 $i$ 行，可以是从 $j - 1$ 列的任何一个位置 $k$ 向上或者向下到达第 $i$ 行，然后向右平移过来，选择和最小的记录在第 $j$ 列第 $i$ 行。

三层循环，将 $80\times 80$ 的二维数组填满。

最后要的最小路径和就是最后一列值最小的一个数字。

整个题目不难，代码如下：
```cpp
long[,] sums = new long[dim, dim];
for (int i = 0; i < dim; i++)
{
    sums[i, 0] = matrix[i, 0];
}

for (int j = 1; j < dim; j++)
{
    // from column j-1 to column j
    for (int i = 0; i < dim; i++)
    {
        // consider row i
        long min = long.MaxValue;
        for (int k = 0; k < dim; k++)
        {
            long temp = sums[k, j - 1] + matrix[i, j];
            int begin;
            int end;
            if (k < i)
            {
                begin = k + 1;
                end = i;
            }
            else
            {
                begin = i;
                end = k - 1;
            }
            for (int index = begin; index <= end; index++)
            {
                temp += matrix[index, j - 1];
            }

            if (temp < min)
            {
                min = temp;
            }
        }

        sums[i, j] = min;
    }
}

var lastColumn = new List<long>();
for (int i = 0; i < dim; i++)
{
    lastColumn.Add(sums[i, dim - 1]);
}

return lastColumn.Min().ToString();
```