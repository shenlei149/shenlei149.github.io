具体问题是说，使用一种极为简单的算法来压缩字符串。该算法就是字符后面跟上该字符连续出现的次数。比如 "aabcccccaaa" 压缩完之后就是 "a2b1c5a3"。如果压缩过的字符串比原始的字符串还长，那么就返回原始的字符串。

解法也很直接。从第一个字符开始遍历字符串，并记录连续的次数。遇到下一个不一样的字符时，就把上一个字符和字数都添加到一个 `StringBuilder` 里面。然后看该字符连续的次数。遍历完成之后，判断新字符串和旧字符串的长度，如果新的长度短，则返回新字符串，否则，返回旧字符串。
``` csharp
public static string Compress(string s)
{
    StringBuilder sb = new StringBuilder();
    char last = s[0];
    int count = 1;
    for (int i = 1; i < s.Length; i++)
    {
        if (last == s[i])
        {
            count++;
        }
        else
        {
            sb.Append(last).Append(count);
            last = s[i];
            count = 1;
        }
    }
    sb.Append(last).Append(count);

    if (sb.ToString().Length >= s.Length)
    {
        return s;
    }

    return sb.ToString();
}
```
