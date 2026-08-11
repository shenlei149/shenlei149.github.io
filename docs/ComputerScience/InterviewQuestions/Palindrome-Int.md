给定一个正整数，写一个函数判断该整数是否是回文。

一个很常见的思路，把整数转化成字符串，然后判断字符串是否是回文。
``` csharp
static bool IsPalindrome_solution1(int number)
{
    return IsPalindrome(number.ToString());
}

static bool IsPalindrome(string s)
{
    if (string.IsNullOrEmpty(s))
    {
        return true;
    }

    if (s.Length == 1)
    {
        return true;
    }

    return s[0] == s[s.Length - 1] && IsPalindrome(s.Substring(1, s.Length - 2));
}
```

这个题目限定了输入是一个整数，所以可以通过循环把这个数的回文数值计算出来，判断原先的整数和反转之后的整数是否相等。  
``` csharp
static bool IsPalindrome_solution2(int number)
{
    int copy = number;
    int reverted = 0;

    while (number != 0)
    {
        reverted = reverted * 10 + number % 10;
        number /= 10;
    }

    return copy == reverted;
}
```
