[Problem 204](https://projecteuler.net/problem=204)

一个汉明数（`Hamming number`）是指其所有质因数都不大于某个给定值的正整数 $n$。这里 $n$ 称为该汉明数的类型。

比如类型 5 的汉明数指的是所有质因数都不大于 5 的正整数，比如 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15 等。不超过 $10^8$ 的类型 5 的汉明数共有 1105 个。

问不超过 $10^9$ 的类型 100 的汉明数共有多少个？

这个题目不难，我们通过构造法生成这些汉明数。具体地说，我们需要将质数按从小到大的顺序排列，然后依次尝试每个质数的不同幂次，直到生成的数超过给定的上限。如何有序地尝试每个质数的幂次呢？我们可以从最小的质数开始，逐步增加其幂次。当超过上限时，将该质数的幂次重置为 0，并增加下一个质数的幂次，如此循环，直到所有可能的组合都被尝试过。代码如下所示。
```cpp
auto primes = Utils::GenPrimes(Type + 1);

std::vector<int32_t> powers(primes.size(), 0);
int32_t count = 1;
int64_t value = 1;
while (true)
{
	// Advance the least significant power that can be increased
	powers[0]++;
	value *= primes[0];

	if (value > Max)
	{
		// Reset the value and increment the next power
		for (size_t i = 0; i < powers.size(); ++i)
		{
			powers[i] = 0;
			if (i + 1 < powers.size())
			{
				powers[i + 1]++;
			}
			else
			{
				return std::to_string(count);
			}
			value = GetValue(powers, primes);
			if (value <= Max)
			{
				break;
			}
		}
	}

	count++;
}

return std::string {};
```
其中 `GetValue` 函数用于根据质数的幂次计算对应的值。
```cpp
int64_t GetValue(const std::vector<int32_t> &powers, const std::vector<int64_t> &primes)
{
	int64_t value = 1;
	for (size_t i = 0; i < powers.size(); ++i)
	{
		for (int32_t j = 0; j < powers[i]; ++j)
		{
			value *= primes[i];
		}
	}
	return value;
}
```
