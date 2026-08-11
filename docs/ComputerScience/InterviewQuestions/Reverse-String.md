这个题几乎不需要什么算法，很简单，但是可以花 2 分钟考察一下面试者到底会不会写代码，如果不会，可以节省面试官很多时间。

下面用 `C#` 仿照传统的 `C` 来实现反转字符串，思路就是从头开始，把前面的字符和后面的字符对调，对换到中间的时候结束。
``` csharp
public static string Reverse(string text)
{
    char[] cArray = text.ToCharArray();
    for (int i = 0; i < cArray.Length / 2; i++)
    {
        char tmp = cArray[i];
        cArray[i] = cArray[cArray.Length - i - 1];
        cArray[cArray.Length - i - 1] = tmp;
    }

    return new string(cArray);
}
```
