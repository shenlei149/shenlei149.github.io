### Linear Congruences
形式如下的式子，
$$ax\equiv b(\operatorname{mod}m)$$
其中 $a,b$ 是整数，$m$ 是正整数，$x$ 是变量，被称为线性同余。

如何找到满足这个同余式子的 $x$ 的值呢？一个办法是使用称为 $a$ 模 $m$ 的逆（`inverse`）$\bar{a}$，其满足 $a\bar{a}\equiv 1(\operatorname{mod}m)$。

**定理 1**
如果 $a,m$ 是互斥的整数且 $m>1$，那么 $a$ 模 $m$ 的逆存在，且逆模 $m$ 是唯一的。也就是说，小于 $m$ 的正整数 $\bar{a}$ 是唯一的，其他的逆与该值模 $m$ 同余。

证明：根据 4.3 节定理 6，因为 $\gcd(a,m)=1$，那么存在 $s,t$ 使得
$$sa+tm=1$$
这蕴涵着
$$sa+tm\equiv 1(\operatorname{mod}m)$$
因为
$$tm\equiv 0(\operatorname{mod}m)$$
所以
$$sa\equiv 1(\operatorname{mod}m)$$
这里的 $s$ 即 $a$ 模 $m$ 的逆。

下面证明唯一性。假定还有一个 $b<m$ 的正整数也满足 $ab\equiv 1(\operatorname{mod}m)$，且 $b\neq s$，那么
$$sa\equiv ba(\operatorname{mod}m)$$
根据 4.3 节定理 7，有
$$s\equiv b(\operatorname{mod}m)$$
与假设矛盾。

如果 $m$ 很小，测试小于 $m$ 的正整数是否满足逆的属性即可。比如找 3 模 7 的逆，尝试 $j\cdot 3, j=1,2,3,4,5,6$ 即可。一个便捷方法是当发现 $2\cdot 3\equiv -1(\operatorname{mod}7)$ 时，可知 $-2\cdot 3\equiv 1(\operatorname{mod}7)$，那么 $-2$ 是逆，因此 5 也是 3 模 7 的逆。

通用方法是根据 $sa+tm=1$，利用 4.3 节的两种不同方法求 $s$ 即可。

有了 $\bar{a}$ 之后，求解 $ax\equiv b(\operatorname{mod}m)$ 就很简单了：两边同乘 $\bar{a}$ 即可。

### The Chinese Remainder Theorem
孙子曾提出一个问题：一个数，除以 3 余 2，除以 5 余 3，除以 7 余 2，问这个数是多少？

用现代数学语言描述是求满足下面同余方程的未知数 $x$：
$$\begin{aligned}
x\equiv 2(\operatorname{mod}3)\\
x\equiv 3(\operatorname{mod}5)\\
x\equiv 2(\operatorname{mod}7)
\end{aligned}$$

下面就讲解如何解决这个问题。

**定理 2 中国剩余定理（`THE CHINESE REMAINDER THEOREM`）**
令 $m_1,m_2,\cdots,m_n$ 是两两互斥的正整数，$a_1,a_2,\cdots,a_n$ 是任意整数，那么系统
$$\begin{aligned}
x\equiv a_1(\operatorname{mod}m_1)\\
x\equiv a_2(\operatorname{mod}m_2)\\
x\equiv a_n(\operatorname{mod}m_n)
\end{aligned}$$
模 $m=m_1m_2\cdots m_n$ 有唯一解。也就是说，只有一个解 $x$ 满足 $0\leq x<m$，其他解模 $m$ 同余。

证明：TODO unique modulo m is Exercise 30。

下面构造出一个解。令
$$M_k=m/m_k$$
其中 $k = 1, 2, \cdots , n$。$i\neq k$ 时，$m_i,m_k$ 互斥，所以 $\gcd(M_k,m_k)=1$。根据定理 1，存在一个整数 $y_k$，是模 $m_k$ 的逆，即
$$M_ky_k\equiv 1(\operatorname{mod}m_k)$$
那么
$$x=a_1M_1y_1+a_2M_2y_2+\cdots+a_nM_ny_n$$
是系统的解。$j\neq k$ 时，$M_j\equiv 0(\operatorname{mod}m_k)$。所以
$$x\equiv a_kM_ky_k\equiv a_k(\operatorname{mod}m_k)$$

上述证明过程给出了一个构造解的方法，不过实际计算会比较繁琐，下面通过例子讲解回代法（`back substitution`）。

例 使用回代法求满足
$$\begin{aligned}
x\equiv 1(\operatorname{mod}5)\\
x\equiv 2(\operatorname{mod}6)\\
x\equiv 3(\operatorname{mod}7)
\end{aligned}$$
的所有 $x$。

解：根据 4.1 解的定理 4，从第一个同余式子可以得到 $x=5t+1$，其中 $t$ 是整数。将其代入第二个同余式子中得到
$$5t+1\equiv 2(\operatorname{mod}6)$$
模数很小，从 $j=1,2,3,4,5$ 开始尝试，进而得到 $t\equiv 5(\operatorname{mod}6)$。再次使用 4.1 解的定理 4，$t=6u+5$，其中 $u$ 是整数。所以 $x=5(6u+5)+1=30u+6$。代入第三个同余式子中
$$30u+26\equiv 3(\operatorname{mod}7)$$
同样使用尝试法，得到 $u\equiv 6(\operatorname{mod}7)$，可以令 $u=7v+6$，其中 $v$ 是整数，代入 $x=30(7v+6)+26=210v+206$，因此
$$x\equiv 206(\operatorname{mod}210)$$

### Computer Arithmetic with Large Integers
假定 $m_1,m_2,\cdots,m_n$ 是两两互斥的正整数，$m$ 是它们的乘积。中国剩余定理告诉我们，任意一个整数 $a$，也就是剩余定理的 $x$，能够唯一的分解成 $n$ 元组
$$(a\operatorname{mod}m_1,a\operatorname{mod}m_2,\cdots,a\operatorname{mod}m_n)$$
这个元组即剩余定理的
$$(a_1,a_2,\cdots,a_n)$$

如果对非常大的整数做计算，可以先选取一些大于 2 的两两互斥的整数 $m_1,m_2,\cdots,m_n$，同时要求 $m=m_1m_2\cdots m_n$ 比最终要的结果大。

一旦选取了这些质数，要计算的非常大的整数模 $m_i$，就得到一个 $n$ 元组，每一个称为一个分量，分别计算结果，然后从最终的 $n$ 元组恢复得到想要的结果。这样做的好处是 1）可以处理一些计算机本来无法处理的大的整数；2）各个分量可以并行计算，提高速度。

例 假定一个计算机计算 100 以内的数比较快。我们选择一组两两互斥的模数 $(99,98,97,95)$，那么最大可以处理 $m=89403930$ 的数。

现在使用上述方法求 123684 和 413456 的和，这两个数分别可以表示成四元组 $(33,8,9,89),(32,92,42,16)$，所以
$$\begin{aligned}
(33,8,9,89)&+(32,92,42,16)\\
&=(65\operatorname{mod}99,100\operatorname{mod}98,51\operatorname{mod}97,105\operatorname{mod}95)\\
&=(65,2,51,10)
\end{aligned}$$
从 $(65,2,51,10)$ 恢复对应的整数即可，相当于求解
$$\begin{aligned}
x&\equiv &65(\operatorname{mod}99)\\
x&\equiv &2(\operatorname{mod}98)\\
x&\equiv &51(\operatorname{mod}97)\\
x&\equiv &10(\operatorname{mod}95)
\end{aligned}$$

使用回代法计算 $x$。令 $x=99u+65$，代入第二个同余式子
$$99u+65\equiv 2(\operatorname{mod}98)$$
$$99u\equiv 35(\operatorname{mod}98)$$
因为 $99\equiv 1(\operatorname{98})$，所以
$$u\equiv 35(\operatorname{mod}98)$$
令 $u-98t+35$，代入 $x$ 得到 $x=9702t+3530$，代入第三个同余式子
$$9702t+3530\equiv 51(\operatorname{mod}97)$$
$$9702t+38\equiv 51(\operatorname{mod}97)$$
$$9702t\equiv 13(\operatorname{mod}97)$$
因为 $9702\equiv 2(\operatorname{mod}97)$，所以
$$t\equiv 55(\operatorname{mod}97)$$
令 $t=97y+55$，代入 $x$ 得到 $x=941094y+537140$，代入第四个同余式子
$$941094y+537140\equiv 10(\operatorname{mod}95)$$
$$941094y+10\equiv 10(\operatorname{mod}95)$$
$$941094y\equiv 0(\operatorname{mod}95)$$
所以 $y=0$，那么 $x=537140$。

整个计算过程中，只有最后恢复整数的时候，涉及了大整数运算。

实践上我们会使用形如 $2^k-1$ 的整数集合，因为 1）计算机计算模更容易；2）更容易寻找互质集合。TODO he second reason is a consequence of the fact that gcd(2a - 1, 2b - 1) = 2gcd(a, b) - 1, as Exercise 37 in Section 4.3 shows.

比如计算机可以计算 $2^{35}$ 内下的数，根据 TODO Exercise 38 in Section 4.3 shows，$2^{35}-1,2^{34}-1,2^{33}-1,2^{31}-1,2^{29}-1,2^{23}-1$ 是两两互斥的数，那么我们可以容易地计算 $2^{182}$ 这么大的数。

### Fermat's Little Theorem
**定理 3 费马小定理（`FERMAT'S LITTLE THEOREM`）**
如果 $p$ 是质数，且 $a$ 不能被 $p$ 整除，那么
$$a^{p-1}\equiv 1(\operatorname{mod}p)$$
进而对于任意整数都有
$$a^p\equiv a(\operatorname{mod}p)$$

证明：TODO outlined in Exercise 19

例 求 $7^{222}\operatorname{mod}11$。

解：根据费马小定理，$7^{10}\equiv 1(\operatorname{mod}p)$，而 $222=22\cdot 10+2$，所以
$$7^{222}=(7^{10})^{22}\cdot 7^2\equiv (1)^{22}\cdot 49\equiv 5(\operatorname{mod}11)$$

上面的例子展示了如何利用费马小定理求 $a^n\operatorname{mod}p$，其中 $p$ 是质数且 $p\not |a$。分解 $n$ 得到 $n=q(p-1)+r,0\leq r<p-1$，那么 $a^n= (a^{p-1})^qa^r\equiv 1^q a^r\equiv a^r(\operatorname{mod}p)$。

### Pseudoprimes
中国古代的数学家认为 $n$ 是质数等价于
$$2^{n-1}\equiv 1(\operatorname{mod}n)$$
根据费马小定理，如果 $n$ 是大于 2 的质数，上式是成立的。不过，存在一些合数，也满足上述式子。这些合数称为基为 2 的伪素数（`pseudoprime`）。当研究伪素数的时候，会使用非 2 的整数作为基数。

例 $341=33\cdot 11$ 是合数，但是其满足
$$2^{340}\equiv 1(\operatorname{mod}341)$$

证明：由于
$$2^{340}=(2^{10})^{34}$$
而根据费马小定理
$$2^{10}\equiv 1(\operatorname{mod}11)$$
所以
$$2^{340}\equiv 1(\operatorname{mod}11)$$
由
$$2^{340}=(2^{5})^{68}=32^{68}$$
得到
$$2^{340}\equiv 1(\operatorname{mod}31)$$
所以我们可以将 $2^{340}$ 写作
$$2^{340}=11t+1=31s+1$$
其中 $s,t$ 均为整数。进而有
$$11t=31s$$
$s$ 是整数，所以
$$31|11t$$
但是由于 11 不能整数 31，所以
$$31|t$$
那么 $t=31x$，代入前面的式子
$$2^{340}=11(31x)+1=341x+1$$
这完全是习题 29 的一个特化形式（$n=2$）。

**定义 1**
令 $b$ 是正整数。如果 $n$ 是合数且满足 $b^{n-1}\equiv 1(\operatorname{mod}n)$，称 $n$ 是基底为 $b$ 的伪素数。

给定一个数 $n$，如果有 $2^{n-1}\equiv 1(\operatorname{mod}n)$，那么 $n$ 是质数或者基底为 2 的伪素数。如果不满足，那么一定是合数。测试更多的 $b$，那么是质数的概率会增加，不过结论不变是质数或者是伪素数。对于不超过 $x$ 的数，选定一个 $b$ 进行判定，只有很少的伪素数。比如 $10^{10}$ 以内有 $455,052,512$ 个质数，但是仅有 $14,884$ 个基底为 2 的伪素数。不幸的是，我们无法通过增加任意多的测试来确定其是质数还是伪素数。

**定义 2**
一个合数 $n$ 对任意 $\gcd(b,n)=1$ 的 $b$ 都有 $b^{n-1}\equiv 1(\operatorname{mod}n)$，那么其成为卡迈克尔数（`Carmichael number`）。

例 561 是卡迈克尔数数。首先，$561=3\cdot 11\cdot 17$ 是一个合数。如果 $\gcd(b,561)=1$，那么 $\gcd(b,3)=1,\gcd(b,11)=1,\gcd(b,17)=1$。

根据费马小定理，有
$$\begin{aligned}
b^2\equiv 1(\operatorname{mod}3)\\
b^{10}\equiv 1(\operatorname{mod}11)\\
b^{16}\equiv 1(\operatorname{mod}17)
\end{aligned}$$

那么
$$\begin{aligned}
b^{560}=(b^{2})^{280}\equiv 1(\operatorname{mod}3)\\
b^{560}=(b^{10})^{56}\equiv 1(\operatorname{mod}11)\\
b^{560}=(b^{16})^{35}\equiv 1(\operatorname{mod}17)
\end{aligned}$$

根据习题 29，有 $b^{560}\equiv 1(\operatorname{mod}561)$。

$$\tag*{$\blacksquare$}$$

尽管存在无限多的卡迈克尔数，它们可以通过一些列设计巧妙的测试。不过这种测试方式可以很快测试出合数。也就是说，给定一个合数，通过测试的概率趋于零。第七章会讨论这个问题。

### Primitive Roots and Discrete Logarithms
**定义 3**
模 $p$ 的原根 $r$ 使得 $Z_p$ 中的每一个数都是 $r$ 的幂次。

例 判定 2 和 3 是否是模 11 的原根。

解：首先计算 2 的幂次模 11。$2^1=2,2^2=4,2^3=8,2^4=5,2^5=10,2^6=9,2^7=7,2^8=3,2^9=6,2^{10}=1$，$Z_p$ 中的每一个非零数都是 2 的幂次，所以 2 是原根。

接着判定 3。$3^1=3,3^2=9,3^3=5,3^4=4,3^5=1,3^6=3$，循环起来了。$Z_p$ 中有非零数不是 3 的幂次，所以 3 不是原根。

$$\tag*{$\blacksquare$}$$

对任意一个质数 $p$ 都存在模 $p$ 的原根。TODO Ro10 中有证明。

**定义 4**
假设 $p$ 是质数，$r$ 是模 $p$ 的原根，$a$ 是属于 $[1,p-1]$ 的整数。如果 $r^e\equiv a(\operatorname{mod}p)$，其中 $0\leq e\leq p-1$，那么 $e$ 是基底为 $r$ 的模 $p$ 的 $a$ 的离散对数（`discrete logarithm`），写作 $\log_r a=e$。

例 求基底为 2 的模 11 的 3 和 5 的离散对数。

解：根据上个例子 $2^8=3,2^4=5$，所以
$$\log_2 3=8,\log_2 5=4$$

$$\tag*{$\blacksquare$}$$

求解离散对数问题无法在多项式时间复杂度完成，而这在密码学中扮演了重要角色。
