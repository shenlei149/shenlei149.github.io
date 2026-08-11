关于字符串操作的问题，有一个普适的方法，从后往前来处理字符串。因为我们往往会开一个足够大的缓冲区去放置替换后的字符串，从后往前遍历操作，不用考虑正在被操作的字符被覆盖。

我们需要扫描两次字符串。第一次的目的是统计有多少个空格，这样我们就能确定替换之后的字符串的长度了；第二次从后往前，将空格替换成 `%20`，如果是非空格字符，我们直接拷贝原始的字符就好了。

使用 `C#` 来模拟操作这个字符串：
``` csharp
public static string ReplaceSpaces(string s)
{
    int spaceCount = 0;
    int newLength;
    for (int i = 0; i < s.Length; i++)
    {
        if (s[i] == ' ')
        {
            spaceCount++;
        }
    }

    newLength = s.Length + 2 * spaceCount;
    char[] newString = new char[newLength];
    for (int i = s.Length - 1; i >= 0; i--)
    {
        if (s[i] == ' ')
        {
            newString[newLength - 1] = '0';
            newString[newLength - 2] = '2';
            newString[newLength - 3] = '%';
            newLength -= 3;
        }
        else
        {
            newString[newLength - 1] = s[i];
            newLength--;
        }
    }

    return new string(newString);
}
```
