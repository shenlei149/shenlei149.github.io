原题链接：https://projecteuler.net/problem=788

如果一个正数，有超过一半的数字相同，那么这个数称为 dominating 数。

比如 2022 是一个 dominating 数，而 2021 就不是 dominating 数。

令 $D(N)$ 表示小于 $10^N$ 的 dominating 数的个数，即长度小于等于 $N$ 的十进制数。题目给出两个测试用例：$D(4)=603, D(10)=21893256$。

求 $D(2022)\operatorname{mod}1000000007$。

如果我们有一个函数，给定一个 $n$，能够返回长度等于 $n$ 的十进制数中的 dominating 数的个数，那么整个解决方案是
```cpp
long sum = 0;
for (int i = 1; i <= MaxD; i++)
{
    sum += Count(i);
    sum %= Mod;
}
```

如何计算长度为 $n$ 的数有多少个 dominating 数呢？首先需要遍历超过半数的数字的个数。
```cpp
int start = D % 2 == 0 ? D / 2 + 1 : (D + 1) / 2;
for (int i = start; i <= D; i++)
```

现在问题变成了，一个数长度是 $n$（代码使用了 $D$），有 $i$ 个相同的数字，可能性有多少种。这是一个排列组合的问题，我们分三种情况讨论。

第一种，超过半数的数字是零。由于零不能在第一位，那么需要在 $n-1$ 个位置选择 $i$ 个位置放零，其余位置能随便选，每个位置的可能性是 9，计算公式如下。
$$\begin{pmatrix}
n-1\\i
\end{pmatrix}\cdot 9^{n-i}$$
代码实现如下
```cpp
// dominating is zero
{
    var thisCount = CombinationsCount(D - 1, i);
    int remainNDigits = D - i;
    thisCount *= Power9[remainNDigits];
    thisCount %= Mod;

    count += thisCount;
}
```
第二大类是个数超过一半的数字不是零。这里又分成了两个小类。第一个小类是第一个数字是这个个数超过了一半的数字，那么需要在剩余的 $n-1$ 个位置选择 $i-1$ 个放该数字。其他位置也有 9 中选法。这个数字本身可以从 1-9 中选择，也有 9 中选法。
$$9\cdot\begin{pmatrix}
n-1\\i-1
\end{pmatrix}\cdot 9^{n-i}$$

第二大类的第二小类是第一个数字不是这个个数超过了一半的数字，那么需要在 $n-1$ 个位置选择 $i$ 个位置放这个数。对于其余 $n-i$ 个位置，第一个位置不能是零，所以只有 8 中选择方式，其余位置有 9 中选法。同时，这个数字本身也可以从 1-9 中选择，也有 9 中选法。
$$9\cdot\begin{pmatrix}
n-1\\i
\end{pmatrix}\cdot 9^{n-i-1}\cdot 8$$

现在问题是如何计算
$$\begin{pmatrix}n\\i\end{pmatrix}$$
呢？

传统的方式（或者说代码）肯定不适合。第一个原因是 $n=2022$ 中选择 $m$ 个，数值比较大，那么计算过程中需要考虑取模，第二个原因是性能，每次都调用一个独立函数计算这个值，由于计算次数相当大，即使这个函数挺快的，累计起来性能也很差。

仔细观察这个题目的特征，每次调用 `Count` 函数时数都自增了 1，也就是说 $n$ 从 1 到 2022 进行了遍历。$i$ 也是从某个数遍历到 $n$。那么我们可以利用如下公式初始化一个矩阵 `count[2022][2022]`，里面存放着组合数待查。而初始化这么小的一个矩阵几乎不消耗什么时间。
$$\begin{aligned}
\begin{pmatrix}
n\\0
\end{pmatrix}&=1\\
\begin{pmatrix}
n\\1
\end{pmatrix}&=1\\
\begin{pmatrix}
n\\m
\end{pmatrix}&=\begin{pmatrix}
n-1\\m
\end{pmatrix}+\begin{pmatrix}
n-1\\m-1
\end{pmatrix}
\end{aligned}$$
