[Problem 961](https://projecteuler.net/problem=961 "Problem 961 - Project Euler")

这个题目是说给定一正整数，两个玩家轮流从这个数中移除一个数字，直到没有数字。每次移完数字后，要把开头的零去掉。比如从数字 105 开始，移除一个数字后可能得到 5、15、10。赢家是最后一个移除非零数字的玩家。假设两个玩家都采取最优策略，$W(N)$ 表示小于 $N$ 的数中，先手玩家获胜的数的个数。题目给了几个 $W(N)$ 的值用于验证代码的正确性。$W(100)=18,W(10^4)=1656$。求 $W(10^{18})$。

这个问题中，零和其他数字的区别很大，而其他几个数字可以视为一个数字，因此下面的分析中使用 $X$ 来表示非零数字。

我们先从简单的例子开始分析以此获得一个感性、直观的认识。当 $N=10^1$ 时，一个数字，1-9 都是先手赢，因此 $W(10)=9$。当 $N=10^2$ 时，一个数字已经讨论过了，我们看两个数字的情况。对于 $XX$，先手移除一个数字后，后手就赢了；对于 $X0$，先手移除 $X$ 后就赢了，因此 $W(10^2)=9+9=18$。

如何从 $10^1$ 的结果得到 $10^2$ 的结果呢？或者泛化这个问题，如何从 $10^k$ 的结果得到 $10^{k+1}$ 的结果呢？假定有一个模式 $XXXX000XXX$，其中 $X$ 是非零数字，$0$ 是零。这个数字长度是 10，假定长度 9 的模式已经分析过了，分成了两个集合，$S$ 是先手赢的模式集合，$L$ 是后手赢的模式集合。对于长度 10 的模式，我们逐个尝试移除一个数字，看看移除后是 $S$ 还是 $L$。只要有一种可能性，移除一个数字之后的模式属于 $L$，那么这个模式就属于 $S$。否则，这个模式就属于 $L$，即移除每一个数字得到的模式都属于 $S$。

编码中，使用 `1` 表示这里的 $X$，使用 `0` 表示这里的 $0$。对于长度 $k$ 的模式，我们用一个长度为 $k$ 的二进制数来表示这个模式。因此，遍历所有长度小于 $18$ 的数字，就能得到需要的模式，按照长度分组。
```cpp
const int NUM_DIGITS = 18;
const int NUMBERS = 1 << NUM_DIGITS;

std::string ToBinaryWithoutLeadingZeros(int num)
{
	std::string result = std::bitset<NUM_DIGITS>(num).to_string();
	result.erase(0, result.find_first_not_of('0'));
	return result;
}

std::vector<std::vector<std::string>> patterns(NUM_DIGITS + 1);
for (int i = 0; i < NUMBERS; ++i)
{
	std::string binary = ToBinaryWithoutLeadingZeros(i);
	patterns[binary.size()].push_back(std::move(binary));
}
```
按照上述的分析，逐步增加模式的长度，判断每个模式是先手赢还是后手赢。对于每个模式，尝试移除一个数字，看看移除后得到的模式是先手赢还是后手赢。
```cpp
bool Win(const std::string &pattern,
		 const std::unordered_set<std::string> &winPatterns,
		 const std::unordered_set<std::string> &losePatterns)
{
	for (size_t i = 0; i < pattern.size(); ++i)
	{
		std::string newPattern = pattern;
		newPattern.erase(i, 1);
		newPattern.erase(0, newPattern.find_first_not_of('0'));

		if (newPattern.empty())
		{
			return true;
		}

		if (losePatterns.contains(newPattern))
		{
			assert(!winPatterns.contains(newPattern));
			return true;
		}
	}

	return false;
}

std::unordered_set<std::string> winPatterns;
std::unordered_set<std::string> losePatterns;

for (auto &&pattern : patterns)
{
	for (auto &&binary : pattern)
	{
		if (binary.length() == 0)
		{
			continue;
		}

		if (binary.length() == 1)
		{
			winPatterns.insert(binary);
			continue;
		}

		bool alwaysWin = Win(binary, winPatterns, losePatterns);
		if (alwaysWin)
		{
			winPatterns.insert(binary);
		}
		else
		{
			losePatterns.insert(binary);
		}
	}
}
```
有了先手必赢的模式集合 `winPatterns`，就可以计算 $W(10^{18})$ 了。
```cpp
long sum = 0;
for (auto &&pattern : winPatterns)
{
	long count = std::count(pattern.begin(), pattern.end(), '1');
	long localSum = 1;
	for (size_t i = 0; i < count; ++i)
	{
		localSum = localSum * 9;
	}

	sum += localSum;
}
```
