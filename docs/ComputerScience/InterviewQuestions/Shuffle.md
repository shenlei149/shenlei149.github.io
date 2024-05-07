给定 54 张牌，写一个洗牌算法，要求等概率的出现 $54!$ 种不同的排列。

这个一个众所周知的面试题，也是一个众所周知的算法。

算法的思想很简单，就是遍历每一张，让后面（包括当前这张）的牌等概率的出现在这个位置上。比如 `index` 从 0 开始，那么每张牌都等概率的出现在 0 这个位置上， `index` 等于 1 时，除了 0 位置上的那一张，其余的牌等概率出现在位置 1 上，以此类推。这样，排列总数就是 $54!$，且每一种是等概率出现的。
下面是代码：
``` csharp
public static void Shuffle(int[] cards)
{
    Random rand = new Random(DateTime.Now.Ticks);
    for (int i = 0; i < cards.Length; i++)
    {
        int k = rand.Next(i, cards.Length);
        int temp = cards[k];
        cards[k] = cards[i];
        cards[i] = temp;
    }
}
```

我曾经看过有人在生成随机数的时候，总是生成 $(0, \text{length})$ 范围内的随机数，这是不对的，因为每个位置上的数字还会从前面已经排好的数字里面取，这样排列总数是 $54^{54}$，是伪洗牌算法，是错误的。
