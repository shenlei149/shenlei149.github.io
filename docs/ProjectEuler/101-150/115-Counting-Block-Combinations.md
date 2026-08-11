[Problem 115](https://projecteuler.net/problem=115)

和 116, 117 两个题目的思路是一样的，一层一层（也就是砖的下标）向前统计。

不过有两点不同，115 题只限制了最小长度 $m$，而没有限制最大长度，那么最大长度就是 $n$，所有在最后求解满足条件的数量的和下一层灰色的数量的时候，需要从 $m$ 累加到 $n$。同时，由于砖的长度不固定，那么在更新每层的颜色的时候，不能几个语句完成而要使用 `for` 循环更新所有状态。

下面给出代码，然后在代码里面解释为什么要这么做。

```csharp
private long F(int m, int n)
{
    int Layer = n;
    int RedBlockLength = Layer;

    // Color: grey[0] or red[1:RedBlockLength]; Layer
    // 二维数组，第一维是颜色，第二位是层（瓷砖的第几块）
    // 最长和瓷砖长度一样，因为没有限制红色砖的长度
    // red[i] 表示到某个位置时，红色砖的长度
    // 初始化初始值，第一层的灰色和 red1 数量是 1，也就是第一块可以是灰色或者红色
    var reds = new long[RedBlockLength + 1, Layer];
    reds[0, 0] = 1;
    reds[1, 0] = 1;
    for (int i = 2; i <= RedBlockLength; i++)
    {
        reds[i, 0] = 0;
    }

    // 开始遍历每一层
    for (int layer = 1; layer < Layer; layer++)
    {
        long count = 0;
        // 红色砖最短长度是 m，所以从 m 开始先把上一层的值更新一下
        for (int red = m; red <= RedBlockLength; red++)
        {
            count += reds[red, layer - 1];
        }
        count += reds[0, layer - 1]; //add #grey

        // 上一层是灰色或者长度符合条件的红色数量就是下一层灰色可能的个数
        reds[0, layer] = count;

        // 对于本层，红色的第 i+1 块和上一层的第 i 块数量一样
        // 也就是顺着往后接着铺红色
        for (int red = 1; red <= RedBlockLength; red++)
        {
            reds[red, layer] = reds[red - 1, layer - 1];
        }
    }

    // 遍历完成之后，把满足题意得可能性加和起来
    long result = 0;
    for (int red = m; red <= RedBlockLength; red++)
    {
        result += reds[red, Layer - 1];
    }
    result += reds[0, Layer - 1]; //add #grey

    return result;
}
```
