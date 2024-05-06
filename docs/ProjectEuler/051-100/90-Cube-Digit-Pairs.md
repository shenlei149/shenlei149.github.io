[Problem 90](https://projecteuler.net/problem=90)

给两个骰子，每个骰子每面可以有不同的数字（0-9）。将这两个骰子并排摆放可以组成一个两位数。仔细挑选骰子上的数字，那么这俩骰子可以摆出所有一百以内的平方数 $01, 04, 09, 16, 25, 36, 49, 64, 81$。比如一个骰子上的数字是 $\{0, 5, 6, 7, 8, 9\}$，另一个是 $\{1, 2, 3, 4, 8, 9\}$。6 和 9上下颠倒看起来是一样的，所以骰子上的数字如果是 $\{0, 5, 6, 7, 8, 9\}, \{1, 2, 3, 4, 6, 7\}$ 的话，也是满足条件的。我们不关心数字的顺序，所以 $\{1, 2, 3, 4, 5, 6\}$ 和 $\{3, 6, 4, 1, 2, 5\}$ 是同一种选择，但是 $\{1, 2, 3, 4, 5, 6\}$ 和 $\{1, 2, 3, 4, 5, 9\}$ 是不同的选择。问一共有多少种不同的选择方式。

这个题目还是比较简单且直接的，因为数量级不大。
$$\begin{pmatrix}10\\6\end{pmatrix}^2=\begin{pmatrix}10\\4\end{pmatrix}^2=210^2=44100$$
即使算上 6 和 9 可以颠倒，数量级也不会发生变化，所以直接使用暴力法解题即可。

如果有 6 或者 9，先将 6 和 9 扩充到现有的数字中去。这里我用了最暴力的方式，没有判断任何情况。
``` csharp
private void Extend(List<int> dice)
{
    if (dice.Contains(6) || dice.Contains(9))
    {
        dice.Add(6);
        dice.Add(9);
    }
}
```

接下来遍历两个骰子，组成两位数，然后检查是否覆盖所有的平方数。前面一系列 `if` 判断，可以提高 3 倍性能。
``` csharp
private static readonly int[] squares = [1, 4, 9, 16, 25, 36, 49, 64, 81];

private bool CanBeDisplayed(List<int> d1, List<int> d2)
{
    if (!d1.Contains(0) && !d2.Contains(0))
    { return false; }
    if (!d1.Contains(1) && !d2.Contains(1))
    { return false; }
    if (!d1.Contains(2) && !d2.Contains(2))
    { return false; }
    if (!d1.Contains(3) && !d2.Contains(3))
    { return false; }
    if (!d1.Contains(4) && !d2.Contains(4))
    { return false; }
    if (!d1.Contains(5) && !d2.Contains(5))
    { return false; }
    if (!d1.Contains(8) && !d2.Contains(8))
    { return false; }

    var newd1 = new List<int>(d1);
    var newd2 = new List<int>(d2);
    Extend(newd1);
    Extend(newd2);
    var numbers = new List<int>();
    foreach (var first in newd1)
    {
        foreach (var second in newd2)
        {
            numbers.Add(first * 10 + second);
            numbers.Add(second * 10 + first);
        }
    }

    return numbers.Intersect(squares).Count() == 9;
}
```

最后，从十个数字里面选择六个，遍历，计算数量。最后除以 2 的原因我们并不区分谁是第一个骰子。
``` csharp
int count = 0;
var dice = Utils.Combination(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 6);
foreach (List<int> d1 in dice)
{
    var newd1 = new List<int>(d1);
    foreach (List<int> d2 in dice)
    {
        var newd2 = new List<int>(d2);
        if (CanBeDisplayed(d1, d2))
        {
            count++;
        }
    }
}
return (count / 2).ToString();
```
