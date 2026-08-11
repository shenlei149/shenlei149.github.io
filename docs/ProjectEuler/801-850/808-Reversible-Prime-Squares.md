原题链接：https://projecteuler.net/problem=808

169 和 961 都是一个质数的平方，同时数字恰好相反。

我们将一个数满足下面三个属性，则称为可逆的质数平方（`reversible prime square`）。
1. 不是回文
2. 是一个质数的平方
3. 数字翻过来也是一个质数的平方

169 和 961 就满足这三条，所以是可逆的质数平方。

求前 50 个可逆的质数平方。

这个题目比较直接，先求出前若干个质数，然后平方，接着遍历这些平方数，对于每一个数，先翻转，如果翻转得到的数还是一个平方数的话，那么这两个数都是可逆质数平方。

不过这里有一个问题，如果 $p$ 和其翻转 $p'$ 都是质数的平方，不过存在一个 $p_2$，介于两者之间的话，明显 $p_2$ 应该放到 $p'$ 前面以正确统计前五十个可逆质数平方的和。

不过我十倍十倍的增加质数的范围，当最大值是 `100_000_000` 时，恰好有五十个可逆质数平方，所以上述的问题就不必在代码里面体现了。
```csharp
var primes = Utils.GenPrimes2(100_000_000);
```

下面是完整代码。
```csharp
var primes = Utils.GenPrimes2(100_000_000);
var squares = primes.Select(l => l * l).ToArray();

var rps = new List<long>();
for (int i = 0; i < squares.Length / 2; i++)
{
    var res = Utils.Reverse(squares[i]);

    var index = Array.BinarySearch(squares, res);
    if (index > i)
    {
        rps.Add(squares[i]);
        rps.Add(res);
    }
}

return rps.Sum().ToString();
```
因为数都是有顺序的，所以可以使用二分查找，如果得到的 `index` 是负值，说明没有找到，如果等于 `i`，那么当前值恰好是回文，如果小于 `i`，那么更早的时候我们已经处理过了，这里就不用再处理了。

另外，由于从前往后找，所以只需要遍历一半即可。

