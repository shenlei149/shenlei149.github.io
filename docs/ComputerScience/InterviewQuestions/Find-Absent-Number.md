给一个文件，里面包含 40 亿个 32 比特非负数字，但是有某个非负整数不再里面，提供一种算法，输出文件不包含的非负数字，假设我们有 1GB 内存可以用。

更进一步，假设数字不重复，但是内存只有 10MB 可以用，又该如何处理呢？

非负整数只有 $2^{31}$ 个那么多，1GB 内存，对应有 $8 * 2^{30}$ 位，比 $2^{31}$ 大多了，那么可以用这些 bit 位标识该数字是否出现了。思路很简单，创建一个足够大的 bit 向量来放所有的非负整数，把每一位都清为 0；然后开始扫描文件，把对应位置的 bit 标记为 1；遍历 bit 向量，遇到 0 说明对应的数字没有在文件中出现过，就打印出来。

下面是代码，需要注意的是，算 bit 位数的时候，int 最大值加 1 会溢出成 int 最小值，所以需要转成 long 再加 1。
``` csharp
static void PrintNotContainedNumber()
{
	long count = (long)int.MaxValue + 1;
	byte[] bits = new byte[count / 8];
	using (var reader = new StreamReader("file.txt"))
	{
		while (!reader.EndOfStream)
		{
			int number = int.Parse(reader.ReadLine());
			bits[number / 8] |= (byte)(1 << (number % 8));
		}
	}

	for (int i = 0; i < bits.Length; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((bits[i] & 1 << j) == 0)
			{
				Console.WriteLine(i * 8 + j);
			}
		}
	}
}
```

现在考虑更进一步的问题，内存有限，该怎么办？《编程珠玑》里面有介绍这种情况，两次扫描法。

第一次扫描，我们搞一个数组，比如大小为 `ArraySize`，那么共 $2^{31}$ 个数，第一个数记录 0 到 $2^{31}/\text{ArraySize} - 1$ 数字出现的次数，第二个数组记录 $2^{31}/\text{ArraySize}$ 到 $2^{31}/(\text{ArraySize} *2)-1$ 数字出现的次数，以此类推。

我们定义 $\text{RangeSize} = 2^{31}/\text{ArraySize}$，如果数字没少的话，数组的每个元素都应该是 `RangeSize`。但是少了一个数字，那么某个元素就是 RangeSize-1，那么就可以确定少了的数字的范围。

第二次扫描，我们搞一个 `RangeSize` bit 的向量，用于标识上一步确定的范围数字是否出现了。

最后，该 bit 向量有一位是 0，就是缺少的那个数字。

我们如何确定  `ArraySize` 呢？只有 10MB，那么 $\text{ArraySize} * 4 < 2^{23} < 10 * 2^{20}$，所以 $\text{ArraySize} < 2^{21}$。$\text{RangeSize} < 2^{26} < 8 * 10 * 2^{20}$，$\text{ArraySize}=2^{31}/\text{RangeSize}>2^{11}$。综上，`ArraySize` 的大小可选范围还是挺大的。

下面是实现的代码
``` csharp
static int ArraySize = 4096; // 2 ^ 12
static int RangeSize = 524288; // 2 ^ 19
static void PrintNotContainedNumber()
{
	int[] blocks = new int[ArraySize];
	using (var reader = new StreamReader("file.txt"))
	{
		int number = int.Parse(reader.ReadLine());
		blocks[number / RangeSize]++;
	}

	int starting = 0;
	for (int i = 0; i < ArraySize; i++)
	{
		if (blocks[i] < RangeSize)
		{
			starting = i * RangeSize;
			break;
		}
	}

	byte[] bits = new byte[RangeSize / 8];
	using (var reader = new StreamReader("file.txt"))
	{
		int number = int.Parse(reader.ReadLine());
		if (number >= starting && number < starting + RangeSize)
		{
			bits[number / 8] |= (byte)(1 << ((number - starting) % 8));
		}
	}

	for (int i = 0; i < bits.Length; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((bits[i] & 1 << j) == 0)
			{
				Console.WriteLine(i * 8 + j + starting);
				return;
			}
		}
	}
}
```

如果内存更小，比如 10KB，该怎么办呢？那么可能需要三次扫描，第一次 range 给到千万，看到底是第几千万少了一个数字，第二次确定这个千万里面，哪一万少了一个数字，第三次再确定最终的数字。思路是一样的。
