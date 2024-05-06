[Problem 345](https://projecteuler.net/problem=345 "Problem 345 - Project Euler")

题目定义了一种矩阵和：从每一行每一列各取一个数字，找到和最大的组合。有点八皇后的意思。比如下面这个矩阵：
```
  7  53 183 439 863
497 383 563  79 973
287  63 343 169 583
627 343 773 959 943
767 473 103 699 303
```
最大和是
$$863 + 383 + 343 + 959 + 767 = 3315$$
求更大的矩阵的和
```
  7  53 183 439 863 497 383 563  79 973 287  63 343 169 583
627 343 773 959 943 767 473 103 699 303 957 703 583 639 913
447 283 463  29  23 487 463 993 119 883 327 493 423 159 743
217 623   3 399 853 407 103 983  89 463 290 516 212 462 350
960 376 682 962 300 780 486 502 912 800 250 346 172 812 350
870 456 192 162 593 473 915  45 989 873 823 965 425 329 803
973 965 905 919 133 673 665 235 509 613 673 815 165 992 326
322 148 972 962 286 255 941 541 265 323 925 281 601  95 973
445 721  11 525 473  65 511 164 138 672  18 428 154 448 848
414 456 310 312 798 104 566 520 302 248 694 976 430 392 198
184 829 373 181 631 101 969 613 840 740 778 458 284 760 390
821 461 843 513  17 901 711 993 293 157 274  94 192 156 574
 34 124   4 878 450 476 712 914 838 669 875 299 823 329 699
815 559 813 459 522 788 168 586 966 232 308 833 251 631 107
813 883 451 509 615  77 281 613 459 205 380 274 302  35 805
```
首先，这个题不能使用暴力法，因为第一行有 15 种取法，第二行有 14 种，以此类推，共 $15!$ 种，这是一个很大的数字，1307674368000。

那么应该怎么思考呢？至少要剪枝，第一行可以有 15 种选法，但是第二行要排除一些不可能的答案，继续第三行的时候，再剪枝去掉不可能的选择，这样子保证最后从比 $15!$ 小的多可能性中选择一个最大值。同时，我们循环 15 次，第一次当作这个矩阵只有一行，第二次当作这个矩阵有两行，依此类推。因为我们推导前 $k+1$ 行的最大值的时候，会利用这个前 $k$ 行最大值这个信息。

我们先扫描一次矩阵，每行的最大值保存下来，以后会用的着。

我们再定义一个数组 `int[] columns`，用于保存当前选中了第 $i$ 行第 $j$ 个数。

如何找到前 $k+1$ 行的最大值呢？

假设我们得到了前 $k$ 行的最大值 `maxSum`，现在分析如何得到前 $k+1$ 行的最大值。

其实，前 $k$ 行最大值选出的 $k$ 个数对 $k+1$ 行找最大值没啥帮助。唯一的帮助是剪枝，下面会详细解释。

很容易想到，这是一个搜索前 $k+1$ 行最大值是个递归算法。

进入这个函数，我们先计算一下 `int[] columns` 保存的数字之和。

如果已经推理到了 $k+1$ 行，递归结束，如果此时得到的和比 `maxSum` 大，更新下最大值。

如果还没有到 $k+1$ 行，我们拿着前 $i$ 行（假设推理到了第 $i$ 行）的和，再利用之前保存的每行的最大值，计算下这一次和的极限。也就是说，可以计算在选择了 $i$ 行的情况下，加上第 $i+1$ 行到 $k+1$ 行每行最大的值，得到的就是最大可能值，如果这个最大可能值比已经得到最大值还小的话，那么就没有必要继续往下继续了。这就是剪枝！

如果可能的最大值比现在的最大值小，继续下一行，这个时候，就要遍历每一列，检查这一列是否可以被选中（每列只能有一个数），如果能够被选择，把选择的列放入 `int[] columns` 数组中，递归调用。
整个题目的大致思路就是这样子。下面是我代码。
``` csharp
private static int[,] matrix;
private static int MatrixLength = 15;
private static int[] MaxNumber = new int[MatrixLength]; 
private static int MaxSum = 0;

public static int GetAnswer()
{
    Init();
    InitMaxNumber();

    int[] columns = new int[MatrixLength];
    for (int curRow = 0; curRow < MatrixLength; curRow++)
    {
        SearchMaxSum(0, columns, curRow);
    }

    return MaxSum;
}

private static void SearchMaxSum(int row, int[] columns, int searchTo)
{
    int sum = GetSum(columns, row);
    if (row == searchTo + 1)
    {
        if (sum > MaxSum)
        {
            MaxSum = sum;
        }
    }
    else
    {
        int possibleMax = sum;
        for (int curRow = row; curRow <= searchTo; curRow++)
        {
            possibleMax += MaxNumber[curRow];
        }

        if (possibleMax < MaxSum)
        {
            return;
        }

        for (int col = 0; col < MatrixLength; col++)
        {
            if (CheckValid(columns, row, col))
            {
                columns[row] = col;
                SearchMaxSum(row + 1, columns, searchTo);
            }
        }
    }
}

private static bool CheckValid(int[] columns, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        if (columns[i] == col)
        {
            return false;
        }
    }

    return true;
}

private static int GetSum(int[] columns, int row)
{
    int sum = 0;
    for (int i = 0; i < row; i++)
    {
        sum += matrix[i, columns[i]];
    }

    return sum;
}

private static void InitMaxNumber()
{
    for (int i = 0; i < MatrixLength; i++)
    {
        int max = 0;
        for (int j = 0; j < MatrixLength; j++)
        {
            if (matrix[i, j] > max)
            {
                max = matrix[i, j];
            }
        }

        MaxNumber[i] = max;
    }
}
```

需要注意的是，主函数里面每次调用 `SearchMaxSum` 都是从 0 行开始，这对应开头说的那句“我们循环 15 次，第一次当作这个矩阵只有一行，第二次当作这个矩阵有两行，依此类推。”

通过剪枝之后，整个算法运行的很快，大量的可能性都被排除了，50ms 就得到了最终结果。
