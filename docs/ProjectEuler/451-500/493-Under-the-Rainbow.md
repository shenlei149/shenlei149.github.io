[Problem 493](https://projecteuler.net/problem=493 "Problem 493 - Project Euler")  

在一个坛子中有赤橙黄绿青蓝紫七种颜色的球，每种 10 个，共 70 个球。从中随机挑选出 20 个球，求有几种颜色的期望值。

这个题思路很明确，期望等于颜色的种数乘以对应的概率。

直接算不太好算，我们换个等价的形式。

用 `i j k l m n q`七个数字表示每个颜色的球的数量，遍历 `i j k l m n q` 每种可能的情况，对于特定的一组值，计算出可能出现的拿法，然后乘以颜色种数，除以 $C(20,70)$，就是对应这组值的期望值。比如 3 3 3 3 3 5 0，可能出现的拿法是 $C(3,10)^5C(5,10)$，颜色种类是 5。

这里，$C(20,70)$ 是定值，所以可以提取出来，最后除一次就可以了。

首先，给出求排列组合的函数：
``` csharp
public static long GetCombinationsCount(long total,long pickedCount)
{
	long count = 1;

	for (int i = 0; i < pickedCount; i++)
	{
		count *= (total - i);
	}

	for (int i = 0; i < pickedCount; i++)
	{
		count /= (i + 1);
	}

	return count;
}
```

计算颜色种类的函数：
``` csharp
private static int GetColors(int[] counts)
{
	return counts.Count(i => i != 0);
}
```

准备工作做完了，我们来遍历 `i j k l m n q`，由于最多20个球，循环的时候可以做一下简单的修剪以提高效率
``` csharp
BigInteger count = 0;
for (int i = 0; i < 11; i++)
{
	for (int j = 0; i < 11 && i + j <= 20; j++)
	{
		for (int k = 0; k < 11 && i + j + k <= 20; k++)
		{
			for (int l = 0; l < 11 && i + j + k + l <= 20; l++)
			{
				for (int m = 0; m < 11 && i + j + k + l + m <= 20; m++)
				{
					for (int n = 0; n < 11 && i + j + k + l + m + n <= 20; n++)
					{
						int q = 20 - i - j - k - l - m - n;
						if (q >= 0 && q <= 10)
						{
							int[] counts = new int[] { i, j, k, l, m, n, q };
							long tmp = 1;
							foreach (var c in counts)
							{
								tmp *= Utils.GetCombinationsCount(10, c);
							}

							count += GetColors(counts) * tmp;
						}
					}
				}
			}
		}
	}
}
```

最后，我们除以 $C(20,70)$
``` csharp
for (int i = 1; i <= 20; i++)
{
	count *= i;
}

double dc = (double)count;
for (int i = 51; i <= 70; i++)
{
	dc /= i;
}

return dc.ToString().Substring(0, 11);
```

注意，这里的 `c` 就只有 11 中情况，我们可以用一个数组提前把值缓存起来，这样能快不少。
```csharp
tmp *= Utils.GetCombinationsCount(10, c);

// faster
var combinationsCount = new long[11];
for (int i = 0; i < 11; i++)
{
    combinationsCount[i] = Utils.GetCombinationsCount(10, i);
}

tmp *= combinationsCount[c];
```

反复构造销毁一个 `int[]` 数组是非常耗时的事情，稍微把代码写的复杂点，这个开销就可以避免，代码快好几倍。
```csharp
int[] counts = new int[] { i, j, k, l, m, n, q };
```
