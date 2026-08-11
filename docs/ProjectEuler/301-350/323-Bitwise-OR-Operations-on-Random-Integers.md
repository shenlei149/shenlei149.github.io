[Problem 323](https://projecteuler.net/problem=323)

这道题乍一看是位运算，其实是一道和概率分布相关的题目。

有 $y_0,y_1,y_2$ 一系列 32 bits 的无符号数。

$x$ 初始为 0，然后和这一系列数异或操作，操作了 $N$ 次，直到 $x$ 是 $2^32-1$，即 $x$ 所有 bits 都是1。

求 $N$ 的期望值。

期望怎么计算呢？

计算对于每一个 $n$ 对应的概率 $p(n)$，然后加权求和。
我们现在逐一分析一下
1. $N$ 为 1，那么说明 $y_0$ 每一个 bits 都是 1，概率是 $1/(2^{32})$
2. $N$ 为 2，针对每个 bit 进行计算，第一个 bit，结果是 1 的概率是 1 减去不是 1 的概率，不是 1 的概率就是 $y_0$ 的第一个 bit 是 0，$y_1$ 的第一个 bit 也是 0，概率是 $1/(2^2)$，那么 $p(2)=(1-1/(2^2))^{32} - p(1)$
3. $N$ 为3，$p(3)=(1-1/(2^3))^{32} - p(1) - p(2)$

以此类推。题目要求小数点后十位，如果 $p(n)$ 很小，乘以 $n$ 对结果都没有影响了，也就是 $p(n)\times n$ 小于 10 的十次方。

我计算的时候搞了一个数据结构保存概率的分子和分母。我解答出来之后，看了下别人的分析，感觉直接使用 `double` 类型保存概率也能给出正确答案，我没有试，不确定。

下面是我的代码：
``` csharp
public class Probability
{
    public BigInteger Numerator { get; set; }
    public BigInteger Denominator { get; set; }

    public Probability(BigInteger numerator, BigInteger denominator)
    {
        Numerator = numerator;
        Denominator = denominator;
    }
}

public static string GetAnswer()
{
    List<Probability> probabilities = new List<Probability>();
    probabilities.Add(new Probability(0, 1));
    BigInteger twoPow32 = BigInteger.Pow(2, 32);
    for (int i = 1; ; i++)
    {
        BigInteger baseDenominator = BigInteger.Pow(2, i);
        BigInteger baseNumerator = baseDenominator - 1;
        BigInteger lastBaseDenominator = BigInteger.Pow(2, i - 1);
        BigInteger lastBaseNumerator = lastBaseDenominator - 1;
        BigInteger denominator = BigInteger.Pow(baseDenominator, 32);
        BigInteger numerator = BigInteger.Pow(baseNumerator, 32);
        BigInteger lastNumerator = BigInteger.Pow(lastBaseNumerator, 32);
        probabilities.Add(new Probability(
            numerator - lastNumerator * twoPow32,
            denominator));

        if (probabilities[i].Denominator / probabilities[i].Numerator / i > BigInteger.Pow(10, 12))
        {
            break;
        }
    }

    BigInteger ansDenominator = probabilities.Last().Denominator;
    BigInteger ansNumerator = 0;
    for (int i = 1; i < probabilities.Count; i++)
    {
        Probability current = probabilities[i];
        BigInteger times = ansDenominator / current.Denominator;
        ansNumerator += times * current.Numerator * i;
    }

    BigInteger times11 = ansNumerator * 100000000000 / ansDenominator;
    double answer = (double)times11 / 100000000000;
    return answer.ToString("N10");
}
```
