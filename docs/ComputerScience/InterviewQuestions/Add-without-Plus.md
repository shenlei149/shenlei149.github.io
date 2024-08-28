看到这个题，第一直觉是使用位运算。为什么这么说呢？首先，不能使用 + 这个符号，你还能用什么呢；其次，计算机就是使用位运算来计算加法的。

我们需要真的搞明白加法的原理，然后用程序去实现它。

简单起见，我们考虑十进制。

想想我们小时候是如何计算 759 + 674 的，末位相加，大于 10 了，然后进一。然后加十位上的数字，还要加上个位的进位。以此类推。计算二进制也是类似的，对每一位相加，如果可能，还要进位。

好了，让我们分离加法操作，分成**加**和**进位**两部分。
1. 759 + 674，如果只加不进位，得到 323；
2. 只考虑进位，得到 1110；
3. 把两部分相加在一起，得到1433。

现在来看看如何进行二进制操作：
1. 只加不进位，相当于位运算里面的异或 XOR；
2. 只考虑进位，相当于和操作 AND 且左移一位；
3. 重复这个过程，直到没有进位为止。

因为十进制分析那里，第三步相当于用了加。程序上，不能使用加，只好想办法再继续递归下去。没有进位，就是第二个加数为零，那么第一个加数就是要求的和。

下面是代码：
``` csharp
public static int Add(int a, int b)
{
    if (b == 0)
    {
        return a;
    }

    int sum = a ^ b;
    int carry = (a & b) << 1;

    return Add(sum, carry);
}
```