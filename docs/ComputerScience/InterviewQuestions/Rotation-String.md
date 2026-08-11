具体问题是说，如果给两个字符串 `s1`，`s2`，写一个方法，判断 `s1` 能否通过旋转变成 `s2`。比如 watset 按照某个点旋转变成 setwat，还能旋转成 tsetwa。限制是只能用一次 `isSubstring` 方法。

《编程珠玑》这本书有提到类似的问题。

思路挺简单：假定 `x` 和 `y` 都是字符串，如果能旋转的话
```
s1=xy
s2=yx
```
那么 `s2` 是 `s1s1` 的子字符串，即 `yx` 一定是 `xyxy` 的子字符串。

``` csharp
public static bool IsRotation(string s1, string s2)
{
    if (s1.Length != s1.Length)
    {
        return false;
    }

    return IsSubstring(s1 + s1, s2);
}
```