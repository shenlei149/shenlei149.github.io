[Problem 504](https://projecteuler.net/problem=504 "Problem 504 - Project Euler")

四边形 ABCD，各点位于四个坐标轴上，坐标分别是 $A(a, 0), B(0, b), C(-c, 0), D(0, -d)$，其中 $1 \leq a, b, c, d \leq m$ 并且 $a, b, c, d, m$ 都是整数。

比如 $m = 4$, 四边形 ABCD 共有 256 种情况。256 个四边形当中，有 42 四边形精确地包含平方个数目的网格点（坐标为整数）。

如果 $m = 100$, 有多少个四边形包含平方个数目的点呢？

对于现代计算机而言，遍历 $100^4$ 其实是很快的一件事情，如果对于每一个四边形而言，如果能在常数量级得到包含点的个数，然后常数量级知道它是不是平方数，那么就基本能在 $100^4$ 这个量级搞定这个问题。

那么如何知道四边形里面点的个数呢？

预先存下来。显然不是预先存下来每个四边形的点的个数，这不就是我们要的结果嘛。我们存储直角三角形的内部点的个数。
``` csharp
static private int[,] NumbersOfInsideVector = new int[M + 1, M + 1];

for (int i = 1; i <= M; i++)
{
	for (int j = 1; j <= i; j++)
	{
		int result = GetInsideVector(i, j);
		NumbersOfInsideVector[i, j] = result;
		NumbersOfInsideVector[j, i] = result;
	}
}

private static int GetInsideVector(int a, int b)
{
	int count = 0;

	for (int i = 1; i <= a - 1; i++)
	{
		int yi = b - b * i / a;
		count += (yi - 1);
	}

	return count;
}
```
使用一个二维数组来存放边长为 $a, b$ 的直角三角形内部的点数。对于边长 $a, b$ 的直角三角形，$x$ 轴从 1 到 $a - 1$，计算高度 $y_i$，然后得到对于每个 $x$ 有的点数，相加起来就是整个直角三角形内部的点数了。

有了直角三角形内部点数，计算 ABCD 四边形内部点数就非常容易了。
``` csharp
private static int GetNumberOfVector(int a, int b, int c, int d)
{
	int number = a + b + c + d + -3;
	number += NumbersOfInsideVector[a, b];
	number += NumbersOfInsideVector[b, c];
	number += NumbersOfInsideVector[c, d];
	number += NumbersOfInsideVector[d, a];

	return number;
}
```
除了计算几个直角三角形内部的点数，还要加上坐标轴上的点，$a b c d$ 相加再减去 3 个，这是重复计算了原点。

最后就是遍历每一个可能的情况。
``` csharp
long count = 0;

for (int d = 1; d <= M; d++)
{
	for (int c = 1; c <= M; c++)
	{
		for (int b = 1; b <= M; b++)
		{
			for (int a = 1; a <= M; a++)
			{
				int numberOfVector = GetNumberOfVector(a, b, c, d);
				int sqrt = (int)Math.Sqrt(numberOfVector);
				if (sqrt * sqrt == numberOfVector)
				{
					count++;
				}
			}
		}
	}
}

return count;
```

以上是 2016 年 2 月做题时的思考，其实当时也应该能想到，$a,b,c,d$ 交换值不影响最后结果，不过要考虑各种情况的话，太复杂。

简单的考虑两种情况，$a<b,a=b$。当 $a<b$ 时，计算出结果，将其乘以 2，然后加上 $a=b$ 时的结果即可。

代码虽然略微冗长，但是提速了一倍。
```csharp
for (int d = 1; d <= M; d++)
{
    for (int c = 1; c <= M; c++)
    {
        for (int b = 1; b <= M; b++)
        {
            for (int a = 1; a < b; a++)
            {
                int numberOfVector = GetNumberOfVector(a, b, c, d);
                int sqrt = (int)Math.Sqrt(numberOfVector);
                if (sqrt * sqrt == numberOfVector)
                {
                    count++;
                }
            }
        }
    }
}

count *= 2;

for (int d = 1; d <= M; d++)
{
    for (int c = 1; c <= M; c++)
    {
        for (int b = 1; b <= M; b++)
        {
            int numberOfVector = GetNumberOfVector(b, b, c, d);
            int sqrt = (int)Math.Sqrt(numberOfVector);
            if (sqrt * sqrt == numberOfVector)
            {
                count++;
            }
        }
    }
}
```
