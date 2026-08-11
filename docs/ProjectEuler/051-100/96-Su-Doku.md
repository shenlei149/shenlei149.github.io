[96题链接](https://projecteuler.net/problem=96 "Problem 96 - Project Euler")

数独，一个很经典的填数字游戏。规则也很简单，每一行、每一列、九个九宫格的九个数字不重复。  
设计很好的数独游戏只有一个解，题目中也假设了这一点。  
题目最后要求五十个数独的解，然后计算左上角三位数之和。  
解题思路就是把人肉做数独游戏的方法写成程序。

每一个数独使用二维数组保存。对于每个二维数组，就是一个数独题目，求解，得到左上角的三位数。求和。
``` csharp
private static readonly int Length = 9;

public static int GetAnswer()
{
    int sum = 0;
    for (int i = 0; i < 50; i++)
    {
        // soDoku = init so doku

        sum += GetTopLeft(SolveSuDoku(soDoku));
    }

    return sum;
}
```

具体解析文本变成二维数组的代码就略去了，不是解题思路的重点。

`GetTopLeft(int[,] soDoku)`函数相对也比较简单，拿到二维数组的左上角三个数字，拼成`string`然后转成`int`即可。或者是第一位乘以100加第二位乘以10加第三位。
``` csharp
private static int GetTopLeft(int[,] soDoku)
{
    return int.Parse($"{soDoku[0, 0]}{soDoku[0, 1]}{soDoku[0, 2]}");
}
```

整个题目的核心在于解数独。

解数独就是在不停的猜测和测试，整个过程显然是递归的。

解数独的方法就是找个一个空，可能有几个情况，选一个填进去，依旧是未完成的数组，再来一轮，一轮，直到完成。整个过程是一个树，分了很多叉。题目告诉我们只有一个解，那么只有一个叶子节点是完整的解，其他叶子节点都不合法，有矛盾的地方，比如某个点所在的行和列出现了重复数字。

递归的结束就是整个二维数组都填了非零的数。下面是判定是否结束的代码。
``` csharp
private static bool IsCompleted(int[,] soDoku)
{
    foreach (var cell in soDoku)
    {
        if (cell == 0)
        {
            return false;
        }
    }

    return true;
}
```

对于递归函数而言，我们判断数独是否完成，如果是，则返回该解。

如果没有，我们需要找一个点，该点没有被填写数字，且可能填的数字尽可能的少，这种情况下该节点的子树才最少，效率能尽可能的高。`FindMinimalPoint`返回参数包含三项，前两者描述的该节点的位置，第三项是可能的值。如果`Item3`是空的话，那么说明当前的数独有矛盾，返回`null`。试图尝试所有的可能性，如果有解，则返回，如果所有的可能性都无解的话，走到最后一行返回`null`。
``` csharp
private static int[,] SolveSuDoku(int[,] soDoku)
{
    if (IsCompleted(soDoku))
    {
        return soDoku;
    }

    var point = FindMinimalPoint(soDoku);
    foreach (var v in point.Item3)
    {
        int[,] newSoDoku = soDoku.Clone() as int[,];
        newSoDoku[point.Item1, point.Item2] = v;
        var ret = SolveSuDoku(newSoDoku);
        if (ret != null)
        {
            return ret;
        }
    }

    return null;
}
```

如何实现`FindMinimalPoint`呢？遍历非 0 点，找到可能的值（`FindVaulesAt`），记录下可能的值最少的点坐标和对应的值并返回。对于`FindVaulesAt`函数，很可能返回 0 个解，就是说该数独有矛盾。`minList`在初始化的时候写了十个值，目的是比九大，其实这里写九个也没问题，因为个数的最小值肯定比九要小。
``` csharp
private static (int, int, List<int>) FindMinimalPoint(int[,] soDoku)
{
    int minRow = -1;
    int minCol = -1;
    var minList = new List<int>() { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    for (int i = 0; i < Length; i++)
    {
        for (int j = 0; j < Length; j++)
        {
            if (soDoku[i, j] == 0)
            {
                var values = FindVaulesAt(i, j, soDoku);
                if (values.Count < minList.Count)
                {
                    minList = values;
                    minRow = i;
                    minCol = j;
                }
            }
        }
    }

    return (minRow, minCol, minList);
}
```

`FindVaulesAt`想法很直接。每个空可能填入一到九九个数字，排除所在行、所在列和所在的九宫格已经出现的数字即可。
``` csharp
private static List<int> FindVaulesAt(int i, int j, int[,] soDoku)
{
    List<int> values = new List<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int k = 0; k < Length; k++)
    {
        values.Remove(soDoku[i, k]);
    }

    for (int k = 0; k < Length; k++)
    {
        values.Remove(soDoku[k, j]);
    }

    for (int row = i - i % 3; row < i - i % 3 + 3; row++)
    {
        for (int col = j - j % 3; col < j - j % 3 + 3; col++)
        {
            values.Remove(soDoku[row, col]);
        }
    }

    return values;
}
```
