给定一个数组，包含 1 到 $N$ 的整数，$N$ 最大为 32000，数组可能包含重复的值，且 $N$ 的取值不定。若只有 4KB 的内存，如何打印出数组中所有的重复元素呢？

4KB，$4*8*1024$ 比特，比 32000 要大一些。我们可以创建一个 32000 比特的位向量，其中每个比特代表一个整数。

利用这个位向量，遍历整个数组，遇到元素 `v`，如果 `v` 对应的比特位已经是 1，就打印该数字，否则，把对应比特 1。

``` csharp
public static void CheckDuplicates(int[] array)
{
    BitSet bs = new BitSet(32000);
    foreach (var num in array)
    {
        if (bs.Get(num))
        {
            Console.WriteLine(num);
        }
        else
        {
            bs.Set(num);
        }
    }
}

class BitSet
{
    private int[] bitSet;

    public BitSet(int size)
    {
        bitSet = new int[size >> 5]; // size / 32
    }

    public bool Get(int pos)
    {
        int wordNumber = pos >> 5; // pos / 32
        int bitNumber = pos & 0x1F; // pos % 32
        return (bitSet[wordNumber] & (1 << bitNumber)) != 0;
    }

    public void Set(int pos)
    {
        int wordNumber = pos >> 5; // pos / 32
        int bitNumber = pos & 0x1F; // pos % 32
        bitSet[wordNumber] |= 1 << bitNumber;
    }
}
```
