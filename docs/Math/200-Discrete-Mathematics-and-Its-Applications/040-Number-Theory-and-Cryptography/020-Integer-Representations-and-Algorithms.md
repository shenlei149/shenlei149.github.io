所有整数都可以用任意大于 1 的基底表示出来。日常常用十进制，计算机领域常用二进制、八进制和十六进制。

### Representations of Integers
**定理 1**
令 $b$ 是任意大于 1 的正整数。如果 $n$ 是正整数，那么可以唯一的表示为
$$n=a_kb^k+a_{k-1}b^{k-1}+\cdots+a_1b+a_0$$
其中 $k$ 是非负整数，$a_0,a_1,\cdots,a_{k}$ 是小于 $b$ 的整数，且 $a_k\neq 0$。

证明：证明过程需要使用归纳法，会在 5.1 节证明。TODO  link

这种表示方法称为基为 $b$ 的展开式，写作 $(a_ka_{k-1}\cdots a_1a_0)_b$。一般情况下，十进制表示不写下标。

计算机中常用二进制（`binary`）八进制（`octal`）和十六进制（`hexadecimal`）。其中十六进制需要十六个字符来表示一个数，超过了十进制的十个数字，后面使用 `A,B,C,D,E,F` 表示。

下面的算法描述了如何将 $n$ 表示为基为 $b$ 的展开形式 $(a_{k-1}\cdots a_1a_0)_b$。

```
procedure - base b expansion(n, b: positive integers with b > 1)
q := n
k := 0
while q != 0
    a_k := q mod b
    q := q div b
    k := k + 1

{(a_{k-1}, ... , a1, a0)_b is the base b expansion of n}
return (a_{k-1}, ... , a1, a0)
```

八进制和十六进制的每个字符是二的整数倍，所以相互之间的转化非常容易，见下表。

|||||||||||||||||||
|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
| Decimal | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
| Hexadecimal | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
| Octal | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 |
| Binary | 0 | 1 | 10 | 11 | 100 | 101 | 110 | 111 | 1000 | 1001 | 1010 | 1011 | 1100 | 1101 | 1110 | 1111 |

###  Algorithms for Integer Operations
这一个小节我们考察
$$a=(a_{n-1}a_{n-2}\cdots a_2a_1a_0)_2$$
$$b=(b_{n-1}b_{n-2}\cdots b_2b_1b_0)_2$$
的算术运算，并使用比特个数（数字的二进制长度）来分析算法的时间复杂度。

加法运算，时间复杂度是 $O(n)$。
```
procedure - add(a, b: positive integers)
{the binary expansions of a and b are (a_{n-1}a_{n-2}\cdots a_2a_1a_0)_2 and (b_{n-1}b_{n-2}\cdots b_2b_1b_0)_2, respectively}
c := 0
for j := 0 to n - 1
    d := (a_j + b_j + c)∕2
    s_j := a_j + b_j + c - 2d
    c := d
sn := c

{the binary expansion of the sum is (s_ns_{n-1} ... s_0)_2}
return (s0, s1, ... , s_n) 
```

乘法运算，时间复杂度是 $O(n^2)$。
```
procedure - multiply(a, b: positive integers)
{the binary expansions of a and b are (a_{n-1}a_{n-2}\cdots a_2a_1a_0)_2 and (b_{n-1}b_{n-2}\cdots b_2b_1b_0)_2, respectively}
for j := 0 to n - 1
    if b_j = 1 then c_j := a shifted j places
    else c_j := 0
    {c_0, c_1, ... , c_{n-1} are the partial products}

p := 0
for j := 0 to n - 1
    p := add(p, c_j)

{p is the value of ab}
return p
```

除法和模运算，时间复杂度是 $O(q\log a), \log a=n$。
```
procedure - division algorithm(a: integer, d: positive integer)
q := 0
r := |a|
while r >= d
    r := r - d
    q := q + 1
if a < 0 and r > 0 then
    r := d - r
    q := -(q + 1)

{q = a div d is the quotient, r = a mod d is the remainder}
return (q, r)
```

### Modular Exponentiation
下面讨论高效地计算 $b^n\operatorname{mod}m$，其中 $b,n,m$ 都很大。如果直接计算，效率低，且 $b^n$ 非常大，浪费内存。

一个直观的方法是利用 4.1 节的定理 5 的推论 2 $b^{k+1}\operatorname{mod}m = b(b^k \operatorname{mod} m) \operatorname{mod} m$ 连续计算 $b^k\operatorname{mod}m,k=1,2,3,\cdots$，最终得到 $b^n\operatorname{mod}m$。这种算法需要 $n-1$ 次乘法，当 $n$ 非常大的时候计算也比较耗时。

下面是快速计算法。令 $n=(a_{k-1}a_{k-2}\cdots a_2a_1a_0)_2$，那么
$$b^n=b^{(a_{k-1}a_{k-2}\cdots a_2a_1a_0)_2}=b^{a_{k-1}2^{k-1}}b^{a_{k-2}2^{k-2}}\cdots b^{a_12^1}b^{a_0}$$
由于 $a_i$ 是 0 或者 1，那么只需要知道 $b,b^2,\cdots,b^{2^{k-1}}$ 等值即可。这样，只需要 $O(\log(n))$ 次乘法。实际算法在计算过程中需要不断地模 $m$。整个算法非常高效，只需要 $O((\log m)^2\log n)$ 次比特操作。

```
procedure - modular exponentiation(b: integer, n = (a_{k-1}a_{k-2} ... a_1a_0)_2,
m: positive integers)
x := 1
power := b mod m
for i := 0 to k - 1
    if a_i = 1 then x := (x * power) mod m
    power := (power * power) mod m

{x equals b^n mod m}
return x
```
