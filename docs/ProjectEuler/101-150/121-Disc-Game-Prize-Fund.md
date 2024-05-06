原题链接：https://projecteuler.net/problem=121

一个袋子里面有一个红色的碟子和一个蓝色的碟子。随机选择一个，记录颜色。一轮结束之后，放回袋子里面，然后添加一个红色的碟子。开始新的一轮，随机选择一个。

玩家付一块钱玩一局，如果蓝色的碟子的个数更多，那么玩家胜利。

如果一局包含四轮，那么蓝色碟子个数多的概率只有 $11/120$。如果庄家最高设置的奖金是十块钱，那么从概率上说玩家会亏。其实，玩家只赢了九块钱，因为其中包含了玩家付的一块钱。

如果一局包含十五轮，那么最大奖金设置成多大可以保证玩家会亏呢？

令 $P$ 表示十五轮蓝色碟子多的概率，那么最后的奖金就是 $\lfloor 1/P\rfloor$。如何求这个概率呢？首先每一轮取到蓝色碟子的概率分别是
$$\frac{1}{2},\frac{1}{3},\frac{1}{4},\cdots,\frac{1}{16}$$
那么红色碟子出现的概率就是
$$\frac{1}{2},\frac{2}{3},\frac{3}{4},\cdots,\frac{15}{16}$$
蓝色碟子数量多，即红色碟子出现次数小于八次。考虑红色碟子出现次数 $r$ 对应的概率是从上面的 $15$ 个概率值中选择 $r$ 个然后相乘。然后对 $r$ 从 $0$ 到 $7$ 求和。

为了计算过程中保持精度，概率$P$的分子分母分别保存。分母显然就是
$$\prod_{i=2}^{16}i$$
对应代码是
```csharp
var denominator = Enumerable.Range(2, count).Aggregate(1, (long i, int j) => i * j);
```
分子的计算过程按照上述描述写成代码如下
```csharp
long numerator = 0;
var Reds = Enumerable.Range(1, count);
for (int i = 1; i < (count + 1) / 2; i++)
{
    var selection = Utils.Combination(Reds, i);
    numerator += selection.Sum(list => list.Aggregate(1L, (long p, int l) => p * l));
}
```
注意，显式的指明`Aggregate`的参数类型的原因是默认情况下是`int`，因为`Range`返回的是`int`的`Enumerable`对象，但是乘积很大，会超过范围，所以显式的把`TAccumulate`类型指定成`long`。
