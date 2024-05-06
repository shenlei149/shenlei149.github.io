[Problem 89](https://projecteuler.net/problem=89 "Problem 89 - Project Euler")

这道题就是给了 1000 个罗马数字，让你求解一下，如果能写成最简形式，能节约多少个字符。

题中给出了提示，没有四个以上连续一样的，也就是说，不会出现 10 个 I 让你写成 X 或者 9 个 I 写成 IX。

题目乍一看很唬人，仔细分析，能简写，无非就是应用减法，[这篇文章](https://projecteuler.net/about=roman_numerals)给出了减法的规则，再加上提示的最多 4 个连续，那么，能简写的就无非下面 6 种情况：
```
DCCCC|LXXXX|VIIII|CCCC|XXXX|IIII
```
能够简写成
```
CM|XC|IX|CD|XL|IV
```

那么，代码就很简单了，找到这六种模式，然后替换简写后计算差值。
``` csharp
public static int GetAnswer()
{
    var words = input.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

    string pattern = "DCCCC|LXXXX|VIIII|CCCC|XXXX|IIII";
    string replacement = "RR";

    return words.Select(roman => roman.Length - Regex.Replace(roman, pattern, replacement).Length).Sum();
}
```
这里，替换的时候用了个小技巧，反正都是换成 2 个字符，也没有必要非要一一对应（当然，如果要输出简写，那么需要对应的替换），于是乎，给了 `RR` 两个字符，算是占位符了。
