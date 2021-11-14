[Problem 98](https://projecteuler.net/problem=98 "Problem 98 - Project Euler")

将单词 `CARE` 的每个字母替换成 1，2，9，6，得到一个平方数 $1296=36^2$，更重要的是，`RACE` 对应字母也换成这几个数字，又能得到一个平方数 $9216=96^2$。那么这两单词被称为平方变位单词对（`a square anagram word pair`）。平方数不能是 0 开始的，不同的字母不能有相同的数字。

题目给出了一系列单词，要求所有的单词对都从这些单词里面取得。求单词对的最大平方数。

首先，首先可以找到所有的变位单词对。这很简单，两层 `for` 循环，对比每两个单词即可。判断两个单词是是否是变位词也很容易，统计字母出现的次数，看是否一致。这里使用的方式是使用一个数组保存次数，对第一个单词遍历各个字母，数组对应值加一，对第二个单词遍历各个字母，数组对应值减一。如果是变位词，那么数组所有数字都应该是 0。
```csharp
private static List<List<string>> FindAnagrams(string[] words)
{
    var anagrams = new List<List<string>>();
    for (int i = 0; i < words.Length; i++)
    {
        var word = words[i];
        var anagram = new List<string>() { word };
        for (int j = i + 1; j < words.Length; j++)
        {
            if (Match(word, words[j]))
            {
                anagram.Add(words[j]);
            }
        }

        if (anagram.Count > 1)
        {
            anagrams.Add(anagram);
        }
    }

    return anagrams;
}
private static bool Match(string word, string candidate)
{
    if (word.Length != candidate.Length)
    {
        return false;
    }

    int[] letters = new int[26];
    foreach (var letter in word)
    {
        letters[letter - 'A']++;
    }

    foreach (var letter in candidate)
    {
        letters[letter - 'A']--;
    }

    return letters.All(i => i == 0);
}
```
接着，找到平方数。其实不用找太多，因为变位词最长是九位的，那么只需要生成小于 1,000,000,000 的平方数即可。对于每个单词对，只有长度一样的平方数才可能和其对应上，所以保存了长度以方便后续处理。
```csharp
// length -> squares
var squares = new Dictionary<int, List<int>>();
// anagrams.Select(a=>a[0].Length).Max() = 9
for (int i = 0; i < (int)Math.Sqrt(1_000_000_000); i++)
{
    var s = i * i;
    var length = s.ToString().Length;
    if (!squares.ContainsKey(length))
    {
        squares[length] = new List<int>();
    }

    squares[length].Add(s);
}
```

自顶向下思考问题。如果有一个函数，能够计算得到其对应的最大平方数（当然也可能不存在），那么一层 `for` 循环就可以得到最后的最大值了。
```csharp
int max = int.MinValue;
foreach (var a in anagrams)
{
    var square = GetMaxSquare(a, squares);
    if (max < square)
    {
        max = square;
    }
}
```

针对某个单词对，如何计算最大的平方数呢？长度固定的（单词这么长的）平方数很多。如果有一个函数能告诉我们某一个平方数是否满足题意，那么一层 `for` 循环就能得到针对这个单词的最大值了。
```csharp
private static int GetMaxSquare(List<string> a, Dictionary<int, List<int>> squares)
{
    var candidates = squares[a[0].Length];
    int max = int.MinValue;
    foreach (var c in candidates)
    {
        var square = GetSquare(a, c.ToString(), candidates);
        if (max < square)
        {
            max = square;
        }
    }

    return max;
}
```

最后的问题就是要解决给定的某个平方数是不是满足题意了。首先，单词长度和平方数长度一致，按序给字母分配数字。如果同一个字母，后面的字母对应的数字和前面不一致，那么这个平方数不满足题意；同时，如果有两个字母对应到了同一个数字上也不行，这里是通过比较字母的个数和对应的数字的个数是否一样多来判定这个条件的。

当有了一组映射关系之后，把它应用到第二个单词上，得到了一个数字，如果这个数字也是平方数，那么就给这个变位单词对加上了平方这个限定，是题目要求的平方变位单词对。返回这两个平方数中较大的即可。
```csharp
private static int GetSquare(List<string> a, string candidate, List<int> squares)
{
    var map = new Dictionary<char, char>();
    for (int i = 0; i < a[0].Length; i++)
    {
        if (map.ContainsKey(a[0][i]))
        {
            if (map[a[0][i]] != candidate[i])
            {
                return -1;
            }
        }

        map[a[0][i]] = candidate[i];
    }

    if (map.Values.Distinct().Count() != map.Count)
    {
        return -1;
    }

    var other = a[1];
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < other.Length; i++)
    {
        sb.Append(map[other[i]]);
    }

    int first = int.Parse(candidate);
    int second = int.Parse(sb.ToString());
    if (squares.BinarySearch(second) >= 0)
    {
        return Math.Max(first, second);
    }

    return -1;
}
```
