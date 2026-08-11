给定两个字符串，写一个方法，判断一个字符串是否可以由另一个字符串变换字符顺序得到？

和大多数面试题一样，往往可以和面试官交流，把一些有可能出现不同理解的地方都弄清楚。比如大小写敏感啊，空白字符算一个字符吗等等。

我们假定大小写敏感，空白字符也算为前提，进行分析。

首先，如果两个字符串的长度不一样，可以直接返回 `false` 了。

解法一，将两个字符串排序，然后对比一下。如果一样，说明一个字符串可以由另外一个字符串变换顺序得到。
``` csharp
public static bool IsPermutation(string s1, string s2)
{
    if (s1.Length != s2.Length)
    {
        return false;
    }

    return Sort(s1).Equals(Sort(s2));
}

private static string Sort(string s)
{
    char[] chars = s.ToCharArray();
    Array.Sort(chars);
    return new string(chars);
}
```
然而，这个方法不高效，但是简单易懂。下面给出一个更高效的解法：计算每个字符的个数。

实现上可能有一点点区别，遍历第一个字符串，统计每个字符有多少个，遍历第二个字符串，对统计好的数据做减法。最后，如果两个字符串的每个字符的个数相等，那么数组每项为零。反之，至少有不为零的项。这里假定只出现 ASCII 码 0-127 这些字符。
``` csharp
public static bool IsPermutation(string s1, string s2)
{
    if (s1.Length != s2.Length)
    {
        return false;
    }

    int[] letters = new int[128];
    for (int i = 0; i < s1.Length; i++)
    {
        letters[s1[i]]++;
    }

    for (int i = 0; i < s2.Length; i++)
    {
        letters[s2[i]]--;
    }

    for (int i = 0; i < letters.Length; i++)
    {
        if (letters[i] != 0)
        {
            return false;
        }
    }

    return true;
}
```
