[Problem 52](https://projecteuler.net/problem=52 "Problem 52 - Project Euler")

找到一个最小整数 $x$，使得 $2x,3x,4x,5x,6x$ 和 $x$ 具有相同的数字，只是顺序不同。

下面是我的解题思路：
1. $3x$ 能被三整除，那么 $x$ 也能被三整除。
2. $5x$ 能被五整除，那么 $x$ 肯定要含有 5 或 0。
3. $x$ 首位是 1，且小于 1*，* 是若干个 6，因为 $x$ 和 $6x$ 的位数一样多。
4. $x$ 首位是 1，$2x$ 的首位就是 2 或者 3，所以 $x$ 肯定要包含 2 或 3。
5. $x$ 首位是 1，$4x$ 的首位是 4 或 5 或 6，所有 $x$ 肯定要包含 4 或5，或 6。
6. $x$ 首位是 1，$6x$ 的首位大于等于 6，所有 $x$ 肯定要有一个数字大于等于 6。
7. 根据上面的 2,4,6 三条，$x$ 至少三位。虽然我的直觉告诉我 $x$ 是 6 位的数字。

使用上述分析实现代码：
``` csharp
for (int i = 3; i < 10; i++)
{
    for (int j = (int)Math.Pow(10, i) + 2; j < (int)Math.Pow(10, i + 1) / 6; j += 3)
    {
        if (HasTwoOrThree(j) && HasZeroOrFive(j) && HasMoreThanSix(j) && HasFourOrFiveOrSix(j))
        {
            if (Utils.IsPermutation(j, j * 6)
                && Utils.IsPermutation(j, j * 5)
                && Utils.IsPermutation(j, j * 4)
                && Utils.IsPermutation(j, j * 3)
                && Utils.IsPermutation(j, j * 2))
            {
                return j.ToString();
            }
        }
    }
}

static bool HasSpecifiedDigit(int number, int spec)
{
    while (number != 0)
    {
        if (number % 10 == spec)
        {
            return true;
        }

        number /= 10;
    }

    return false;
}

static bool HasZeroOrFive(int number)
{
    return HasSpecifiedDigit(number, 0) || HasSpecifiedDigit(number, 5);
}

static bool HasTwoOrThree(int number)
{
    return HasSpecifiedDigit(number, 2) || HasSpecifiedDigit(number, 3);
}

static bool HasFourOrFiveOrSix(int number)
{
    return HasSpecifiedDigit(number, 4) || HasSpecifiedDigit(number, 5) || HasSpecifiedDigit(number, 6);
}

static bool HasMoreThanSix(int number)
{
    return HasSpecifiedDigit(number, 9)
        || HasSpecifiedDigit(number, 8)
        || HasSpecifiedDigit(number, 7)
        || HasSpecifiedDigit(number, 6);
}
```

里面有很多判断都重复了，但是主要是为了对应之前的分析。`j += 3` 对应了 $x$ 能被三整除。

答案是 142857。在这一瞬间，我想到了 1-6 除以 7 的小数位。
$$\begin{aligned}
1/7 = 0.14285714\cdots\\
2/7 = 0.28571428\cdots\\
3/7 = 0.42857142\cdots\\
4/7 = 0.57142857\cdots\\
5/7 = 0.71428571\cdots\\
6/7 = 0.85714285\cdots\\
\end{aligned}$$
每个小数的循环节都是六位，是 142857 按照顺序的轮换。
