[Problem 93](https://projecteuler.net/problem=93)

类似 24 点游戏，使用数字 $1,2,3,4$、运算符 $+,-,*,/$ 和括号能够得到不同的正整数。对于数字的限制是每个数字只能使用一次，且不能将数字连起来用，比如 12、34。比如
$$\begin{aligned}
8 &= (4 * (1 + 3)) / 2\\
14 &= 4 * (3 + 1 / 2)\\
19 &= 4 * (2 + 3) - 1\\
36 &= 3 * 4 * (2 + 1)\\
\end{aligned}$$
使用数字 $1,2,3,4$ 可以得到 31 个不同的数字，最大值是 36，1-28 是连续的，直到 29 是第一个不能通过算术表达式得到的数字。

求四个不同的数字 $a<b<c<d$，能够得到最长连续的 $1\to n$。

先估算下暴力（遍历法）的复杂度。9 个数字，任选 4 个。
$$\begin{pmatrix}9\\4\end{pmatrix}=126$$
四个数字排列的个数是 $4!=24$；四种不同的运算符插入三个位置 $4^3=64$；表达式有五种不同的组合方式，先算前两个、中间两个、最后两个，得到三个数字，还可以先算前两个或者后两个，有六种方式，其中先计算前两个、再计算最后两个，最后得出结果这种模式重复了一次，所以是五种。所以总计有 $126 * 24 * 64 * 5=967,680$ 种不同的情况，不到 $1M$，完全可以直接遍历。

首先定义四种运算，然后递归的方式得到 64 个表达式。
```csharp
enum Operators
{
    Add,
    Sub,
    Mul,
    Div,
    Invalid,
}

private static void FillExpressions(List<Operators[]> expressions)
{
    InternalFillExpressions(expressions, new Operators[3], 0);
}

private static void InternalFillExpressions(List<Operators[]> expressions, Operators[] expression, int depth)
{
    if (depth == 3)
    {
        expressions.Add(expression);
        return;
    }

    for (Operators op = Operators.Add; op < Operators.Invalid; op++)
    {
        expression[depth] = op;
        InternalFillExpressions(expressions, (Operators[])expression.Clone(), depth + 1);
    }
}
```
自顶向下设计程序，先计算出 126 种组合，遍历它们，针对每个组合取最长连续的值，得到最后结果。
```csharp
int max = -1;
List<int> digits = [];
var series = Utils.Combination(new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 4);
foreach (var array in series)
{
    var tmp = GetLastConsecutiveNumber(array as List<int>);
    if (tmp > max)
    {
        max = tmp;
        digits = array as List<int>;
    }
}

return string.Join("", digits);
```
针对每种组合，计算出 24 种排列，遍历每一种情况，计算得到最长连续的值。
```csharp
private int GetLastConsecutiveNumber(List<int> numbers)
{
    List<int> targets = [];
    var candidates = Utils.Permutations(numbers);
    foreach (var c in candidates)
    {
        targets.AddRange(GetTargets(c.ToList()));
    }

    targets = targets.Where(i => i > 0).Distinct().ToList();
    targets.Sort();
    for (int i = 0; i < targets.Count; i++)
    {
        if (targets[i] != i + 1)
        {
            return i;
        }
    }

    return -1;
}
```
针对其中一种排列，运用 64 个表达式，计算出各种可能的值。
```csharp
private List<int> GetTargets(List<int> numbers)
{
    List<int> targets = [];
    foreach (var expression in expressions)
    {
        targets.AddRange(GetTargets(numbers, expression));
    }

    return targets;
}
```
对于给定的排列和表达式，有五种结合的方式，计算出五种对应的值。
```csharp
private List<int> GetTargets(List<int> numbers, Operators[] expression)
{
    List<int> targets = [];

    var target = Compute(expression[1], Compute(expression[0], numbers[0], numbers[1]),
                                        Compute(expression[2], numbers[2], numbers[3]));
    AddNullableTarget(targets, target);

    target = Compute(expression[2],
                     Compute(expression[1],
                             Compute(expression[0], numbers[0], numbers[1]),
                             numbers[2]),
                     numbers[3]);
    AddNullableTarget(targets, target);

    target = Compute(expression[0], numbers[0],
                     Compute(expression[2],
                             Compute(expression[1], numbers[1], numbers[2]),
                             numbers[3]));
    AddNullableTarget(targets, target);

    target = Compute(expression[2],
                     Compute(expression[0],
                             numbers[0],
                             Compute(expression[1], numbers[1], numbers[2])),
                     numbers[3]);

    AddNullableTarget(targets, target);
    target = Compute(expression[0], numbers[0],
                     Compute(expression[1], numbers[1],
                             Compute(expression[2], numbers[2], numbers[3])));
    AddNullableTarget(targets, target);

    return targets;
}
```
这里的 `Compute` 需要特化处理一下除法，因为对于表达式 $4/(1+2-3)$ 来说，被除数是 0，这里使用 `nullable` 类型来表达这种特殊的情况，所以 `Compute` 会返回 `null`。
```csharp
private double? Compute(Operators op, double? left, double? right)
{
    if (!left.HasValue || !right.HasValue)
    {
        return null;
    }

    switch (op)
    {
        case Operators.Add:
            return left + right;
        case Operators.Sub:
            return left - right;
        case Operators.Mul:
            return left * right;
        case Operators.Div:
            if (right == 0)
            {
                return null;
            }

            return left / right;
    }

    return null;
}
```
五种结合方式得到五个数，去重加入结果集，由于计算过程使用的是 `double` ，不一定是整数，用一个比较小的数 `0.001` 和转化前后的值的差的绝对值对比即可。如果差的比较多，那么结果明显不是整数，出现结果不是整数的原因是除不尽，比如表达式 $(1+2+3)/4$。
```csharp
private void AddNullableTarget(List<int> targets, double? v)
{
    if (v.HasValue)
    {
        var i = Convert.ToInt32(v.Value);
        if (Math.Abs(v.Value - i) < 0.001)
        {
            targets.Add(i);
        }
    }
}
```
