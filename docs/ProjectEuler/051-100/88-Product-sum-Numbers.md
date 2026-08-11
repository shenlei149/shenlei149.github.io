[Problem 88](https://projecteuler.net/problem=88)

一个自然数 $N$ 如果能被至少两个自然数的集合 $\{a_1,a_2,\cdots,a_k\}$ 同时用乘积和和表示，即 $N=a_1+a_2+\cdots+a_k=a_1\times a_2\times\cdots\times a_k$，那么称为 `Product-sum number`。

比如 $6=1+2+3=1\times 2\times 3$。

给定一个固定大小 $k$，可以找到一个最小的 $N$ 是 `Product-sum number`。对于 $k=2,3,4,5,6$，最小 $N$ 如下
$$\begin{aligned}
k=2:&&&4=2\times 2=2+2\\
k=3:&&&6=1+2+3=1\times 2\times 3\\
k=4:&&&8=1\times 1\times 2\times 4=1+1+2+4\\
k=5:&&&8=1\times 1\times 2\times 2\times 2=1+1+2+2+2\\
k=6:&&&12=1\times 1\times 1\times 1\times 2\times 6=1+1+1+1+2+6
\end{aligned}$$
因此 $2\leq k\leq 6$，最小 `Product-sum number` $N$ 之和是 $4+6+8+12=30$，注意 8 只记录一次。

求$2\leq k\leq 12000$时，最小 `Product-sum number` 的 $N$ 之和。

先估计下数据量，上限是 12k，如果采用暴力法，如果对于给定的 $k$ 能够线性时间得出对应的最小的 $N$ 的话，复杂度也就 144M，对于 CPU 而言，是很小的量级，如果能够 $k\lg k$ 的时间复杂度得出结果，也完全是可以接受的。那么先按照暴力法思考下问题。

比如 $k=12,000$，$2^k$ 是一个很大很大的值，显然 12000 个数的和不可能有那么大的，所以不需要真的遍历 12000 个数，其实 $2^{15}=32768$ 就比 12000 大挺多的了，所以 $k$ 的低 15 个数字就能决定乘积与和能否相等了，剩余的数字都是 1，很容易得出积与和。

既然只有 15 个数字就能决定的话，完全不需要暴力求解了。

这里引入进位和加一的概念，把这 15 个数字当作 15 位，加一就是把最低为加一。进位呢？当乘积比和大的时候，就进一位，然后进位这个地方之后的数字保持和前一位一样（这样做是为了不重不丢），这样乘积就会变小，比如 $2\times 2\times 90$ 进位得到 $2\times 3\times 3$，乘积从 360 降到 18。

如何判断从哪里进位呢？上述描述是说进位后的每一位和进位的数字保持一样，所以从低位向高位扫描，两两比较，如果相同，说明是从上次进位留下的结果，后续没有经过加一的步骤，无须再进位，第一次需要不同的地方，高位加一表示进位的操作，高位之后的数字自然保持和高位一样的数字。下面是描述进位的函数
```csharp
private static bool Carry(int[] digits)
{
    for (int i = 0; i < digits.Length - 1; i++)
    {
        if (digits[i] != digits[i + 1])
        {
            digits[i + 1]++;
            for (int j = 0; j <= i; j++)
            {
                digits[j] = digits[i + 1];
            }

            return true;
        }
    }

    return false;
}
```
从最小值最低两位是 2 其余数字是 1 开始，加一，或者进位，直到进位也无法无法使得乘积再比和小（比如 $2,3,3,3,3$ 进位得到 $3,3,3,3,3$），或者无法再进位（数字都一样），则结束整个过程。

这样得到一个集合，每个元素是 15 个数字组成的集合。那么对于每个 $k$ 对应的最小值 $N$ 都能用该集合中某个元素表示，反过来想，这个集合的每个元素，都决定了某个 $k$ 值对应的 $N$（这里不一定是最小值）。通过过滤得到最小值是很容易。

现在问题是如果通过某个 15 个数字的集合反推得到对应的 $k$。计算得到这 15 个数字的乘积 `p` 与和 `s`，如果 $p\geq s$ 且 $p-s\leq 12000-15$（不要超出上限），令 $d=p-s$，差了一些，但是不多，那么 $k$ 比 15 大的话，求和时候前面有若干个一增加 $s$，使得 $p=s$，前面若干个是多少个？$d$ 个！所以这里的 $p$ 就是一个宽度为 $d+15$ 的 `Product-sum number`。$p<s$ 的话，相当于前面的 1 多了，原理是一样的。

下面的代码反应的就是上述的分析。
```csharp
// 定义 `ks`，用于保存各个宽度 k 对应的 `Product-sum number`
// 我们从 k=2 开始，所以第零个和第一个初始化成0，后面初始化成最大值。
int[] ks = new int[Max + 1];
ks[0] = 0;
ks[1] = 0;
for (int i = 2; i < ks.Length; i++)
{
    ks[i] = int.MaxValue;
}

// 0 开始是低位
int[] digits = new int[15] { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

do
{
    // 计算差值，填充 diff+15 对应的 `Product-sum number`
    // 这里有一个大小的判断以存取最小值
    int product = digits.Aggregate(1, (a, b) => a * b);
    int sum = digits.Sum();
    int diff = product - sum;
    if (diff <= Max - 15 && diff >= -13 && ks[diff + 15] > product)
    {
        ks[diff + 15] = product;
    }

    // 每次迭代要么需要进位，要么进行加一操作
    // 如果无法进位，迭代结束
    if (diff >= Max - 15)
    {
        bool ok = Carry(digits);
        if (!ok)
        {
            break;
        }
    }
    else
    {
        digits[0]++;
    }
} while (true);
```
有了一系列宽度 $k$ 对应的最小 `Product-sum number`，去重求和得到最后的结果。
