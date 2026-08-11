## Ordinary Generating Functions
### Recurrence Relations and Generating Functions
一个池塘，每年年底从里面打捞100个青蛙，青蛙自身每年繁殖四倍，若初始时青蛙有50只，第$n$年有多少青蛙呢。  
$a_0=50,a_1=4a_0-100=100,\cdots$，很容易写出递归式$a_{n+1}=4a_n-100$，但是如果只想知道第87年的青蛙数量，就必须从1开始逐个计算，浪费了很多中间结果。  
为了避免这种情况，需要知道$a_n$的显式公式(`explicit formula`)，只依赖于$n$而不包含$a_{n-1}$或其他项，是可以直接计算得到值的。  
先从这个递归式开始
$$a_{n+1}=4a_n-100\tag{8.1}$$
对$n\ge 0$都成立且初始值$a_0=50$。事实上上面的公式包含无穷个式子，从无穷个式子归纳出一个显式公式，就需要用到生成函数(`generating functions`)的概念。

**Definition 8.1.** $\{f_n\}_{n\geq 0}$是实数序列。形式幂级数(`formal power series`)$F(x)=\sum_{n\geq 0}f_nx^n$称之为序列$\{f_n\}_{n\geq 0}$的普通生成函数(`ordinary generating function`)。

下面计算序列$\{a_n\}$的生成函数。对$(8.1)$两边同乘$x^{n+1}$然后对$n\geq 0$的$a_n$求和。得到
$$\sum_{n\geq 0}a_{n+1}x^{n+1}=\sum_{n\geq 0}4a_nx^{n+1}-\sum_{n\geq 0}100x^{n+1}\tag{8.2}$$
由于$\sum_{n\geq 0}x^n=1/(1-x)$，所以生成函数$G(x)$为
$$G(x)-a_0=4xG(x)-\frac{100x}{1-x}\tag{8.3}$$
$$G(x)=\frac{a_0}{1-4x}-\frac{100x}{(1-x)(1-4x)}\tag{8.4}$$
生成函数$G(x)$是幂级数，将右侧也写作幂级数的形式，那么$x_n$对应的系数既为$a_n$。
$$\frac{a_0}{1-x}=50\sum_{n\geq 0}(4x)^n=50\sum_{n\geq 0}4^nx^n$$
$x_n$对应的系数是$50\cdot 4^n$。  
第二项稍微复杂一些，要用到微分方程课学到的部分函数(`partial function`)方法。
$$\frac{A}{1-x}+\frac{B}{1-4x}=\frac{100x}{(1-x)(1-4x)}$$
$$A(1-4x)+B(1-x)=100x$$
$$(-4A-B)x+(A+B)=100x$$
对应系数相等，所以$A+B=0,-4A-B=100$，那么$A=-100/3,B=100/3$，因此第二项可以写作
$$\begin{aligned}
\frac{100x}{(1-x)(1-4x)}&=\frac{100}{3}\cdot \frac{1}{1-4x}-\frac{100}{3}\cdot \frac{1}{1-x}\\
&=\frac{100}{3}(\sum_{n\geq 0}4^nx^n-\sum_{n\geq 0}x^n)\\
&=\sum_{n\geq 0}(4^n-1)x^n\frac{100}{3}
\end{aligned}$$
结合两项得到
$$a_n=50\cdot 4^n-100\cdot \frac{4^n-1}{3}\tag{8.5}$$
现在验证下$(8.5)$是正确的。$n=0$时，很容易计算得到$a_0=50$。
$$\begin{aligned}
4a_n-100&=4(50\cdot 4^n-100\cdot \frac{4^n-1}{3})-100\\
&=50\cdot x^{n+1}-100\cdot \frac{4^{n+1}-4}{3}-100\\
&=50\cdot x^{n+1}-100\cdot \frac{4^{n+1}-1}{3}
\end{aligned}$$
现在总结一下已知递归式求显式的步骤：
1. 定义$\{a_n\}$序列的生成函数$G(x)$
2. 将递归式写作包含$G(x)$的方程。一般是递归式两边同乘$x^n$或$x^{n+1}$，有时需要同乘$x^{n+k}$，然后对所有非负数$n$求和
3. 求解$G(x)$
4. 找到$G(x)$的$x^n$的系数

**Example 8.2.** 一个账户由1000块钱，年利率百分之五。然后每年存500。求第$n$年账户有多少钱。  
**Solution** 很容易写出递归式$a_0=1000,a_{n+1}=1.05\cdot a_n+500$，下面利用上面总结的步骤来求解。
1. 定义$\{a_n\}$序列的生成函数$G(x)=\sum_{n\geq 0}a_nx^n$
2. 递归式两边同乘$x^{n+1}$然后求和得到
$$\sum_{n\geq 0}a_{n+1}x^{n+1}=\sum_{n\geq 0}1.05a_nx^{n+1}+\sum_{n\geq 0}500x^{n+1}\tag{8.6}$$
$$G(x)-a_0=1.05xG(x)+\frac{500x}{1-x}$$
3. 因此
$$G(x)=\frac{1000}{1-1.05x}+\frac{500x}{(1-x)(1-1.05x)}\tag{8.7}$$
4. 现在找$x^n$的系数
$$\frac{1000}{1-1.05x}=1000\cdot\sum_{n\geq 0}1.05^nx^n$$
$$\begin{aligned}
\frac{500x}{(1-x)(1-1.05x)}&=\frac{10000}{1-1.05x}-\frac{10000}{1-x}\\
&=10000\sum_{n\geq 0}1.05^nx^n-10000\sum_{n\geq 0}x^n\\
&=10000\sum_{n\geq 0}(1.05^n-1)x^n
\end{aligned}\tag{8.8}$$
结合以上两式，得到
$$a_n=1000\cdot 1.05^n+10000\cdot (1.05^n-1)=1.05^n\cdot 11000-10000$$

下面这个例子来展示如何用这个方法将包含多项的递推式写作显式公式。  
**Example 8.3.** $n\geq 0$时，令$a_{n+2}=3a_{n+1}-2a_n, a_0=0, a_1=1$，求$a_n$。  
**Solution** 令$G(x)=\sum_{n\geq 0}a_nx^n$。递归式的两边同乘$x^{n+2}$，
$$\sum_{n\geq 0}a_{n+2}x^{n+2}=3\sum_{n\geq 0}a_{n+1}x^{n+1}-2\sum_{n\geq 0}a_nx^n$$
$$G(x)-a_1x-a_0=G(x)-x=3x(G(x)-a_0)-2x^2G(x)=3xG(x)-2x^xG(x)$$
$$G(x)=\frac{x}{1-3x-2x^2}=\frac{A}{x-1}+\frac{B}{2x-1}$$
$$x=A(2x-1)+B(x-1)=(2A+B)x-(A+B)$$
$$2A+B=1,A+B=0\rArr A=1,B=-1$$
$$\begin{aligned}
G(x)&=\frac{-1}{1-x}+\frac{1}{1-2x}\\
&=-\sum_{n\geq 0}x^n+\sum_{n\geq 0}2^nx^n\\
&=\sum_{n\geq 0}(2^n-1)x^n
\end{aligned}$$
所以
$$a_n=2^n-1$$

### Products of Generating Functions
前面的例子只有一个生成函数，下面讨论若干个生成函数的积。  
**Lemma 8.4.** $\{a_n\}_{n\geq 0}$和$\{b_n\}_{n\geq 0}$是两个序列，$A(x)=\sum_{n\geq 0}a_nx^n$和$B(x)=\sum_{n\geq 0}b_nx^n$是对应的生成函数，令$c_n=\sum_{i=0}^na_ib_{n-i}$，$C(x)=\sum_{n\geq 0}c_nx^n$，那么
$$A(x)B(x)=C(x)$$
也就是说，$A(x)B(x)$中$x^n$的系数是$c_n=\sum_{i=0}^na_ib_{n-i}$。  
**Proof** 两个无限和$A(x)=a_0+a_1x+a_2x^2+\cdots$和$B(x)=b_0+b_1x+b_2x^2+\cdots$相乘，就是第一个和的每一项乘以第二个和的每一项，然后求和。形如$a_ix^i\cdot b_jx^j$，其指数若是$n$，那么$j=n-i$，证毕。

**Theorem 8.5.** (`The Product formula`) 在$n$个元素集合上构建某种结构有$a_n$种方式，在另一个$n$个元素集合上构建另外某种结构有$b_n$种方式。将集合$[n]$分成两个间隔$S=\{1,2,\cdots,i\}$和$T=\{i+1,i+2,\cdots,n\}$（$S$和$T$可空），$c_n$表示在$S$上构建第一种结构并且在$T$上构建第二种结构的方式数。$A(x),B(x),C(x)$表示对应的生成函数，那么
$$A(x)B(x)=C(x)$$
**Proof.** 在$S$上构建第一种结构有$a_i$种方式，在$T$上构建第二种结构有$b_{n-i}$种方式，$i$的取值范围是$[0,n]$，那么$c_n=\sum_{i=0}^na_ib_{n-i}$，这就回到了**Lemma 8.4.**。

**Example 8.6.** 某技术大学某学期有$n$天。教授分成两个部分，前$k(1\leq k\leq n-2)$天学习理论，后$n-k$天实践。教授从前一个部分选择一天作为假期，从后一个部分选择两天作为假期。问有多少种方式来安排这个学期。  
**Solution.** 令有$f_n$种方式安排这学期，那么最直接的方式是$f_n=\sum_{k=1}^{n-2}k\begin{pmatrix}n-k\\2\end{pmatrix}$，但是这个很难写成闭型（没有求和号）。  
换个思路，有$k$种方式从第一部分选择一天，有$\begin{pmatrix}m\\2\end{pmatrix}$种方式从第二部分选择两天，其中$m=n-k$。对应的生成函数是$A(x)=\sum_{k\geq 1}kx^k$和$B(x)=\sum_{m\geq 2}\begin{pmatrix}m\\2\end{pmatrix}x^m$。从$\sum_{i\geq 0}x^i=\frac{1}{1-x}$出发求导，可以得到
$$A(x)=\frac{x}{(1-x)^2}$$
$$B(x)=\frac{x^2}{(1-x)^3}$$
序列$\{f_n\}$的生成函数$F(x)=A(x)B(x)$，因此
$$\begin{aligned}
F(x)&=A(x)B(x)\\
&=\frac{x^3}{(1-x)^5}\\
&=x^3\sum_{n\geq 0}\begin{pmatrix}
n+4\\4
\end{pmatrix}x^n\\
&=\sum_{n\geq 3}\begin{pmatrix}
n+1\\4
\end{pmatrix}x^n
\end{aligned}$$
所以$f_n=\begin{pmatrix}n+1\\4\end{pmatrix}$。
> 上述$(1-x)^5$那里需要多次求导。

> 这个问题从之前学习的计数角度来看，可以看作往$n$天里面加一个隔板隔开，然后从$n+1$个里面选择4个，分别是第一个假期、隔板、两天的假期。

**Example 8.7.** 假设不设置假期，而是从两个部分选择一些天来自习。问有多少种方式来安排这个学期。  
**Solution.** 令$C(x)$是从前一部分选择一些天来自习的方式数的生成函数，$k$个元素的集合有$2^k$个子集，那么$C(x)=\sum_{k\geq 0}2^kx^k=\frac{1}{1-2x}$，注意$k$从0开始，因为前一个部分可以没有任何自习。后一个部分的生成函数也是$C(x)$。那么
$$F(x)=C(x)C(x)=\frac{1}{(1-2x)^2}$$
显然$F(x)=\frac{1}{2}C'(x)$，因此
$$F(x)=\frac{1}{2}\cdot \sum_{n\geq 1}n\cdot 2^nx^{n-1}=\sum_{n\geq 0}(n+1)2^nx^n$$
所以$f_n=(n+1)\cdot 2^n$。

**Theorem 8.5**可以扩展到任意固定数量的生成函数。下面这个例子展示了其一般性。

**Example 8.8.** 将$n$天的学期分成三部分，从第一部分任意选择一些天作为假期，从第二部分选择奇数天作为假期，从第三部分选择偶数天作为假期。  
**Solution.** 令$A(x),B(x),C(x)$为对应序列的生成函数。从$n$天中选择任意天数的方式有$2^n$种，那么$A(x)=\sum_{n\geq 0}2^nx^n=\frac{1}{1-2x}$；集合$[n]$的子集中有奇数个元素的集合数是$2^{n-1},n\geq 1$，$n=0$时是0，所以$B(x)=\sum_{n\geq 1}2^{n-1}x^n=\frac{x}{1-2x}$；类似地，若子集元素是偶数，$n\geq 1$时集合数是$2^{n-1}$，$n=0$时是1，因为0本身是偶数，所以$C(x)=1+\frac{x}{1-2x}=\frac{1-x}{1-2x}$。  
$$\begin{aligned}
F(x)&=A(x)B(x)C(x)\\
&=\frac{1}{1-2x}\cdot\frac{x}{1-2x}\cdot\frac{1-x}{1-2x}\\
&=\frac{x(1-x)}{(1-2x)^3}\\
&=-\frac{1}{4}\cdot\frac{1}{1-2x}+\frac{1}{4}\cdot\frac{1}{(1-2x)^3}
\end{aligned}$$
利用二项式定理，
$$(1-2x)^{-3}=\sum_{n\geq 0}\begin{pmatrix}
-3\\n
\end{pmatrix}(-2x)^n=\sum_{n\geq 0}\begin{pmatrix}
n+2\\n
\end{pmatrix}2^nx^n$$
因此
$$F(x)=-\frac{1}{4}\sum_{n\geq 0}2^nx^n+\frac{1}{4}\sum_{n\geq 0}\begin{pmatrix}
n+2\\2
\end{pmatrix}2^nx^n$$
所以
$$f(n)=\frac{\begin{pmatrix}
n+2\\2
\end{pmatrix}2^n-2^n}{4}=2^{n-3}n(n+3),n\geq 0$$

**Example 8.9.** $p_{\leq k}(n)$表示将$n$分成大小不超过$k$的若干部分的分法，那么
$$\begin{aligned}
\sum_{n\geq 0}^\infty p_{\leq k}(n)x^n&=&&\prod_{i=1}^k\frac{1}{1-x^i}\\
&=&&(1+x+x^2+x^3+\cdots)\\
&&&(1+x^2+x^4+x^6+\cdots)\\
&&&\cdots\\
&&&(1+x^k+x^{2k}+x^{3k}+\cdots)
\end{aligned}\tag{8.11}$$
**Solution.** 考察右边$x^n$的系数。右边是$k$部分相乘，令第$i$部分形式为$x^{ij_i}$，其指数之和是$n$，也就是说$1j_1+2j_2+\cdots+kj_k=n$。将$1j_1$写成$j_1$个$1$相加$1+1+\cdots+1$，一般地，将$ij_i$写成$j_i$个$i$相加$i+i+\cdots+i$。那么这就是$n$的分割方式，其最大块不超过$k$。  
右边每个$x^n$都对应一个$n$的分割方式且满足题意；相反地，每一个满足题意的分割都对应于右边的一个$x^n$。

在第五章，已经证得$p_{\leq k}(n)$也是将$n$分成最多$k$部分的分割数，那么$\prod_{i=1}^k\frac{1}{1-x^i}$也是这些分割的生成函数。  
这个生成函数不是闭形式，那有什么用呢？一是很多数学软件可以提供扩展形式，进而可以获得很多值。二是通过它可以推得更多信息（见下一例题），甚至比精确得公式更多。

**Example 8.10.** $p(n)$表示将$n$所有分割的数量，那么
$$\begin{aligned}
\sum_{n\geq 0}^\infty p(n)x^n&=&&\prod_{k=1}^\infty\frac{1}{1-x^k}\\
&=&&(1+x+x^2+x^3+\cdots)\\
&&&(1+x^2+x^4+x^6+\cdots)\\
&&&(1+x^3+x^6+x^9+\cdots)\\
&&&\cdots
\end{aligned}\tag{8.12}$$
**Solution.** 和上一个例子类似，只是右边是无限项罢了。

**Example 8.11.** $p_{odd}(n)$表示$n$分成奇数块的分割数，$p_d(n)$表示$n$分成若干个各不相同的块的分割数，求证两者相等。  
**Solution.** 关键的核心思想是：证明两者的生成函数相等。
$$F(x)=\sum_{n\geq 0}p_{odd}(n)x^n=\prod_{\begin{aligned}
i\geq 1\\i\ odd
\end{aligned}}\frac{1}{1-x^i}$$
$$G(x)=\sum_{n\geq 0}p_d(n)x^n=\prod_{i\geq 1}(1+x^i)=\prod_{i\geq 1}\frac{1-x^{2i}}{1-x^i}$$
$G(x)$的分母是奇数才会留下来，否则和分子可以进行约分。

#### The Catalan Numbers
某人往瓶子每天往一个瓶子中放入一枚硬币或者取出一枚硬币，第一天放之前瓶子是空的，$2n$天之后，瓶子刚好也是空的。问有多少种不同的方式放入和取出硬币呢？  
设$c_n$表示事件的总数，显然$c_0=1$。严格地说，$c_n$是序列$b_1,b_2,\cdots,b_{2n},b_j=\plusmn 1,\sum_j^{j=2n}b_j=0$的个数。关键信息是对于所有的$k\in [2n]$，不等式$\sum_{j=1}^kb_j\geq 0$都成立，因为瓶子不能有负数个硬币，称这样的长度为$2n$的序列为好序列(`good sequences`)。  
令$C(x)=\sum_{n\geq 0}c_nx^n$是$c_n$的生成函数。很快能看到可以用生成函数的积来解决这个问题。下面要论证的是将好序列分成两个结构的对，其中一个的生成函数是$C(x)$，另一个是$xC(x)$。  
找到除第一天外第一个瓶子为空的一天，不妨令其为$2i$天，那么从$2i$到$2n$天是长度为$2(n-i)$的好序列  
前$2i$个序列不仅是好序列，还满足当$0<k<2i$时，$\sum_{j=1}^kb_j>0$，称为非常好(`very good`)的序列。一个非常好的序列一定是1开头-1结尾的，去掉这两个，构成了一个好序列，所以得到了一个双射，长度为$2i$的非常好的序列$B=(b_1,b_2,\cdots,b_{2i})$和长度为$2(i-1)$的好序列$B'=(b_2,b_3,\cdots,b_{2i-1})$一一对应。  
换句话说，$n\geq 1$时，长度为$2i$非常好的序列的个数是$c_{i-1}$，从开头的分析可知$i=0$时个数是0，所以其生成函数是$\sum_{n\geq 1}c_{n-1}x^n=xC(x)$。  
序列被分成了两个部分，一个非常好的序列和一个好序列，利用生成函数的积得到
$$C(x)-1=xC(x)\cdot C(x)$$
上式$n$从1开始，所以左边是$C(x)-1$而不是$C(x)$。  
整理上式得到
$$xC(x)^2-C(x)+1=0\tag{8.13}$$
根据求根公式知道上式有两个根，分别是
$$\frac{1+\sqrt{1-4x}}{2x},\frac{1-\sqrt{1-4x}}{2x}$$
已知第一项是1，代入$x=0$，得到
> 关于代入$x=0$，可以参考[我提的问答](https://math.stackexchange.com/questions/3856176/why-can-we-substitute-x-0-into-frac1-sqrt1-4x2x)

$$C(x)=\frac{1-\sqrt{1-4x}}{2x}$$
**Example 4.16.** 求得$\sqrt{1-4x}=1-2x-2\sum_{n\geq 2}\frac{\begin{pmatrix}2n-2\\n-1\end{pmatrix}}{n}x^n$，所以
$$C(x)=\sum_{n\geq 0}\frac{\begin{pmatrix}
2n\\n
\end{pmatrix}}{n+1}x^n$$
所以$c_n=\frac{\begin{pmatrix}2n\\n\end{pmatrix}}{n+1}$。  
$c_n$称为卡特兰数(`Catalan numbers`)，以法国和比利时数学家`Eugène Catalan`命名。从0开始前几个卡特兰数是$1,1,2,5,14,42$。

### Compositions of Generating Functions
**Definition 8.12.** 令$F(x)=\sum_{n\geq 0}f_nx^n$是形式幂级数，$G$是包含常数项0的形式幂级数，那么
$$F(G(x))=\sum_{n\geq 0}f_n(G(x))^n=f_0+f_1G(x)+f_2(G(x))^2+\cdots$$

**Theorem 8.13.** 令$a_n$是在$n$个元素的集合上构建某种结构的不同方式的个数，且$a_0=0$。令$h_n$是将集合$[n]$分成若干个不相交非空区间，然后在各个区间构建指定结构的个数，且$h_0=1$。$A(x)=\sum_{n\geq 0}a_nx^n$，$H(x)=\sum_{n\geq 0}h_nx^n$，那么
$$H(x)=\frac{1}{1-A(x)}$$
注意，这个和**Theorem 8.5.** 不同的地方在于区间要非空，因为没有指定区间的个数，若允许非空，那么中间插入不同个数的空区间，那么结果是无穷大。  
**Proof.**  沿着**Theorem 8.5.** 的证明继续，$A(x)^k$表示将集合$[n]$分成$k$个区间，然后构建指定结构的方式的个数的生成函数。对$k$累加，得到$\sum_{k\geq 1}A(x)^k$。由于$a_0=0$，幂级数$A(x)^k$中没有一项有非零的常数项，同时，$H(x)$有常数项1。那么
$$H(x)=1+\sum_{k\geq 1}A(x)^k=\sum_{k\geq 0}A(x)^k=\frac{1}{1-A(x)}$$

**Example 8.14.** $n$个士兵站成一行，长官将其分成非空的若干个小单元，然后每个单元指定一个指挥官。求有$h_n$种方式去做这个事情。  
**Solution.** 令$a_k$表示从$k$个士兵的单元中指定一个指挥官的方式的个数，显然$a_k=k$，那么$A(x)=\sum_{k\geq 1}kx^k=\frac{x}{(1-x)^2}$（参考**Example 8.6**），所以
$$H(x)=\frac{1}{1-A(x)}=\frac{1}{1-\frac{x}{(1-x)^2}}=1+\frac{x}{1-3x+x^2}$$
$H(x)$是序列$\{h_n\}_{n\geq 0}$的生成函数。  
分母$1-3x+x^2$有两个根$\alpha=(3+\sqrt{5})/2,\beta=(3-\sqrt{5})/2$，那么
$$\frac{1}{1-3x+x^2}=\frac{1}{(x-\alpha)(x-\beta)}=\frac{A}{x-\alpha}-\frac{B}{x-\beta}$$
$$1=(A-B)x-A\beta+B\alpha$$
所以，$A=B=1/\sqrt{5}$，那么
$$\frac{1}{1-3x+x^2}=\frac{1}{\sqrt{5}}(\frac{1}{x-\alpha}-\frac{1}{x-\beta})$$
注意$\alpha\cdot\beta=1$，所以括号内的第一项分子分母同乘$\beta$，第二项分子分母同乘$\alpha$，那么
$$\frac{1}{1-3x+x^2}=\frac{1}{\sqrt{5}}(\frac{\alpha}{1-\alpha x}-\frac{\beta}{1-\beta x})=\frac{1}{\sqrt{5}}(\alpha\sum_{n=0}^\infty\alpha^nx^n-\beta\sum_{n=0}^\infty\beta^nx^n)$$
$\frac{1}{1-3x+x^2}$中$x^n$的系数是$\frac{1}{\sqrt{5}}(\alpha^{n+1}-\beta^{n+1})$，因此$n=0$时，$H(x)$中$x^n$系数是1，$n>0$时
$$h_n=\frac{1}{\sqrt{5}}(\alpha^n-\beta^n)$$
从$h_1$开始的前几项分别是$1,3,8,21,55$。

**Theorem 8.13.** 没有对这些区间组成的集合做任何事情，比如上个例子中长官没有要求若干个小单元选出一个值班，或者重新站成一列。下面对这方面进行泛化。

**Theorem 8.15.** [组合公式(`The Compositional formula`)] 令$a_n$表示在$n$个元素的集合上构建第一类结构的方式的个数，并且$a_0=0$。令$b_n$表示在$n$个元素的集合上构建第二类结构的方式的个数，并且$b_0=1$。令$g_n$表示下述操作的个数：将$n$个元素的集合分成若干个非空区间，在每个区间上构建第一类结构，然后在区间组成的集合上构建第二类结构，$g_0=1$。$A(x),B(x),G(x)$分别表示$a_n,b_n,g_n$的生成函数，那么
$$G(x)=B(A(x))$$
**Proof.** 假设将$[n]$分成$k$部分，那么有$b_k$中方式在区间上构建第二类结构，根据生成函数的积可以得到在$k$个区间上对每个区间构建第一类结构的生成函数是$A(x)^k$，那么对于分成$k$部分这种情况$G(x)=b_kA(x)^k$，求和$G(x)=\sum_{k\geq 0}b_kA(x)^k$。

**Example 8.16.** $n$个士兵，分成若干个非空小单元，选择其中一些单元（可空）值夜班。求有多少种不同方式做这个事情。  
**Solution.** 只是分成若干个非空区间，并不需要构建什么结构，那么$k\geq 1$时，$a_k=1$。从$m$个区间选择值班的小单元，有$b_m=2^m$种方式。因此$A(x)=\frac{x}{1-x},B(x)=\frac{1}{1-2x}$，因此
$$\begin{aligned}
G(x)&=B(A(x))\\
&=\frac{1}{1-\frac{2x}{1-x}}\\
&=\frac{1-x}{1-3x}\\
&=\frac{1}{1-3x}-\frac{x}{1-3x}\\
&=\sum_{n\geq 0}3^nx^n-\sum_{n\geq 1}3^{n-1}x^n\\
&=1+\sum_{n\geq 1}2\cdot 3^{n-1}x^n
\end{aligned}$$
所以$n\geq 1$时，有$2\cdot 3^{n-1}$种方式。

## Exponential Generating Functions
### Recurrence Relations and Exponential GeneratingFunctions
不是所有的递归关系都能通过普通生成函数找到一个闭形式，其中一些是因为闭形式的公式不存在，有一些可以用其他形式的生成函数来求解。

**Example 8.17.** 令$a_0=1; a_{n+1}=(n+1)(a_n-n+1), n\geq 0$。求$a_n$。  
由于这个序列增长的太快了如果用普通生成函数求解，很快会遇到麻烦而无法给出一个闭形式的公式。需要利用下面这个概念。

**Definition 8.18.** 令$\{f_n\}_{n\geq 0}$为一实数序列。幂级数$F(x)=\sum_{n\geq 0}f_n\frac{x^n}{n!}$称为序列$\{f_n\}_{n\geq 0}$的指数生成函数(`exponential generating function`)。  
当$f_n=1$式，$F(x)=e^x$，这就是指数的含义。

**Solution.** 令$A(x)=\sum_{n=0}^\infty a_n\frac{x^n}{n!}$是序列$\{a_n\}_{n\geq 0}$的指数生成函数。和用普通生成求解递归式一样，将递归式两边同乘$\frac{x^{n+1}}{(n+1)!}$，然后对$n\geq 0$求和
$$\sum_{n=0}^\infty a_{n+1}\frac{x^{n+1}}{(n+1)!}=\sum_{n=0}^\infty a_n\frac{x^{n+1}}{n!}-\sum_{n=0}^\infty (n-1)\frac{x^{n+1}}{n!}$$
左边是$A(x)-1$，右边第一项是$xA(x)$，所以
$$A(x)-1=xA(x)-x^2e^x+xe^x$$
$$A(x)=\frac{1}{1-x}+xe^x=\sum_{n\geq 0}x^n+\sum_{n\geq 0}\frac{x^{n+1}}{n!}$$
第一项对于$x^n/n!$的系数是$n!$，第二项其系数是$n$，因为其被加数等价于$x^n/(n-1)!$。因此$a_n=n!+n$。

**Example 8.19.** 令$f_0=0;f_{n+1}=2(n+1)f_n+(n+1)!,n\geq 0$。求显式的$f_n$。  
**Solution.** 令$F(x)$是序列$f_n$的指数生成函数。两边同乘$\frac{x^{n+1}}{(n+1)!}$，然后对$n\geq 0$求和
$$\sum_{n\geq 0} f_{n+1}\frac{x^{n+1}}{(n+1)!}=2x\sum_{n\geq 0}f_n\frac{x^n}{n!}+\sum_{n\geq 0}x^{n+1}$$
$$F(x)=2xF(x)+\frac{x}{1-x}$$
$$F(x)=\frac{x}{(1-x)(1-2x)}$$
$$F(x)=\sum_{n\geq 0}(2^n-1)x^n$$
所以$f_n=(2^n-1)n!$

### Products of Exponential Generating Functions
**Lemma 8.20.** 令$\{a_i\}, \{b_k\}$是两个序列，$A(x)=\sum_{i\geq 0}a_i\frac{x^i}{i!},B(x)=\sum_{k\geq 0}b_k\frac{x^k}{k!}$是其指数生成函数。那么$c_n=\sum_{i=0}^n\begin{pmatrix}n\\i\end{pmatrix}a_ib_{n-i}$对应的指数生成函数$C(x)$满足
$$A(x)B(x)=C(x)$$
也就是说$A(x)B(x)$中$x^n/n!$的系数是$c_n$。  
**Proof.** 和证明**Lemma 8.4**类似，$A(x)$与$B(x)$相乘就是其每一项互乘。一般形式是
$$a_i\frac{x^i}{i!}\cdot b_j\frac{x^j}{j!}=a_ib_j\cdot\frac{x^{i+j}}{i!j!}\cdot\frac{(i+j)!}{(i+j)!}=a_ib_j\cdot\frac{x^{i+j}}{(i+j)!}\cdot\begin{pmatrix}
i+j\\i
\end{pmatrix}$$
当且仅当$i+j=n$时，其指数是$n$。

**Theorem 8.21** (`Product formula, exponential version`). 在$n$个元素集合上构建某种结构有$a_n$种方式，在另一个$n$个元素集合上构建另外某种结构有$b_n$种方式。将集合$[n]$分成两个不相交集合$S,T(S\cup T)$，$c_n$表示在$S$上构建第一种结构并且在$T$上构建第二种结构的方式数。$A(x),B(x),C(x)$表示对应的生成函数，那么
$$A(x)B(x)=C(x)$$
和**Theorems 8.5**类似但有所不同，**Theorems 8.5**要求更高，$S,T$是两个区间，相当于将$n$个数排成一列，中间加一个隔板分成两个子集；但是**Theorem 8.21**没有这个要求，可以自由的分成任意两个子集。  
**Proof.** $S$有$i$个元素，那么有$\begin{pmatrix}n\\i\end{pmatrix}$种方式来选取集合$S$，在其上构建第一种结构有$a_i$种方式，有$b_{n-i}$种方式在$T$上构建第二种结构，对$i,0\leq i\leq n$求和，那么$c_n=\sum_{i=0}^n\begin{pmatrix}n\\i\end{pmatrix}a_ib_{n-i}$，再使用**Lemma 8.20**即可。

**Example 8.22.** 一个足球队$n$个人，分成两组，第一组可以穿橘黄色、白色或蓝色的T恤，第二组只能穿红色T恤，然后每组列队成一行。问有多少种方式？  
**Solution.** 假设第一组有$k$个人，那么穿不同T恤的方式有$3^k$，站成一行有$k!$种，所以$a_k=k!3^k$，其指数生成函数是
$$A(x)=\sum_{k\geq 0}k!3^k\frac{x^k}{k!}=\frac{1}{1-3x}$$
第二组有$m$个人，那么$b_m=m!$，其指数生成函数是
$$B(x)=\sum_{m\geq 0}m!\frac{x^m}{m!}=\frac{1}{1-x}$$
那么
$$\begin{aligned}
C(x)&=A(x)B(x)\\
&=\frac{1}{1-3x}\cdot\frac{1}{1-x}\\
&=\frac{3}{2}\cdot\frac{1}{1-3x}-\frac{1}{2}\cdot\frac{1}{1-x}\\
&=\frac{3}{2}\sum_{n\geq 0}3^nx^n-\frac{1}{2}\sum_{n\geq 0}x^n
\end{aligned}$$
对$n\geq 0$，$C(x)$中$x^n/n!$的系数$c_n=n!(3^{n+1}-1)/2$。

**Example 8.23.** $k$是固定正整数，求第二类斯特林数$S(n,k)$的指数生成函数$S_k(x)=\sum_{n\geq k}S(n,k)x^n/n!$。  
**Solution.** $[n]$分成$k$块，对于每一块，什么也不需要做，即一种方式。那么对于每一块，生成函数是
$$A(x)=\sum_{k\geq 1}\frac{x^k}{k!}=e^x-1$$
根据乘法公式，那么生成公式是$A(x)^k$，对于分成$k$部分，顺序不重要，所以
$$S_k(x)=\frac{1}{k!}(e^x-1)^k$$
$S_k(x)$中$x^n/n!$的系数就是$S(n,k)$。对上式使用二项式定理，可以得到和**Theorem 7.5**一样的结论。

指数生成函数有一个很有用的性质。由$(\frac{x^{n+1}}{(n+1)^n})'=\frac{x^n}{n!}$可知
$$(\sum_{n\geq 0}a_n\frac{x^n}{n!})'=\sum_{n\geq 0}a_{n+1}\frac{x^n}{n!}$$

**Example 8.24.** 令$B(x)$是贝尔数$B(n)$的指数生成函数。求证$B(x)=e^{e^x-1}$。  
**Solution.** 贝尔数递归性质是$B(n+1)=\sum_{i=0}^nB(i)\begin{pmatrix}n\\i\end{pmatrix}, n\geq 0$并且$B(0)=1$。递归式两边同乘$x^n/n!$并求和得到
$$\sum_{n\geq 0}B(n+1)\frac{x^n}{n!}=\sum_{n\geq 0}\sum_{i=0}^nB(i)\begin{pmatrix}n\\i\end{pmatrix}\frac{x^n}{n!}$$
左边是$B(x)'$，根据**Lemma 8.20**可知右侧是$B(x)e^x$，因此
$$B'(x)=B(x)e^x$$
$$\frac{B'(x)}{B(x)}=e^x$$
$$\ln{B(x)}=e^x+C$$
代入$x=0$，左侧是$\ln{1}=0$，因此$C=1$。那么
$$\ln{B(x)}=e^x-1$$
进而
$$B(x)=e^{e^x-1}$$

### Compositions of Exponential Generating Functions
**Theorem 8.25** (`The Exponential formula`). 令$a_n$是在$n$个元素的集合上构建某种结构的不同方式的个数，且$a_0=0$。令$h_n$是将集合$[n]$分成若干个非空子集，然后在各个区间构建指定结构的个数，且$h_0=1$。$A(x), H(x)$是对应的指数生成函数。那么
$$H(x)=e^{A(x)}$$
**Proof.** 和**Example 8.23**类似，分成$k$个子集且不在意顺序，指数生成函数是$A(x)^k/k!$。对$k$求和得到$\sum_{k\geq 1}A(x)^k/k$，由于$a_0=0$并且幂级数$A(x)^k/k$中没有常数项，但$H(x)$有常数项1。因此
$$H(x)=1+\sum_{k\geq 1}\frac{A(x)^k}{k!}=\sum_{k\geq 0}\frac{A(x)^k}{k!}=e^{A(x)}$$

微积分中有下面这个等式
$$\int\frac{1}{1-x}=\ln{\frac{1}{1-x}}$$
另一方面
$$\int\frac{1}{1-x}=\int\sum_{n\geq 0}x^ndx=\sum_{n\geq 0}\frac{x^{n+1}}{n+1}=\sum_{k\geq 1}\frac{x^k}{k}$$
所以
$$\sum_{k\geq 1}\frac{x^k}{k}=\ln{\frac{1}{1-x}}$$

**Example 8.26.** 将$n$个人分成若干组，每组围着一组桌子坐。问多少种坐法？  
**Solution.** $k$个人围着桌子坐有$(k-1)!$种坐法，$a_k=(k-1)!$，那么
$$A(x)=\sum_{k\geq 1}(k-1)!\frac{x^k}{k!}=\sum_{k\geq 1}\frac{x^k}{k}=\ln{\frac{1}{1-x}}$$
利用指数公式
$$H(x)=e^{\ln{\frac{1}{1-x}}}=\frac{1}{1-x}=\sum_{n\geq 0}x^n=\sum_{n\geq 0}n!\cdot\frac{x^n}{n!}$$
因此有$h_n=n!$种不同的坐法。

**Example 8.27.** 有序列$\{f_n\}$种方式将集合$[n]$分成大小为3、4、9的块，求其指数生成函数$F(x)$。  
**Solution.** 令$a_n,b_n,c_n$表示将集合$[n]$分成大小只能是3、4、9的块的方法的个数，$A(x),B(x),C(x)$是其对应的指数生成函数。令$t_n$表示集合$n$能组成一个大小为3的块，显然$t_3=1;t_n=0, n\neq 3$，它的指数生成函数是$T(x)=x^3/3!$。根据**Theorem 8.25**
$$A(x)=e^{T(x)}=e^{x^3/3!}$$
同理$B(x)=e^{x^4/4!},C(x)=e^{x^9/9!}$。
现在将$n$分成三个子集（可空），将第一个子集分成大小都是3的块，第二个子集分成大小都是4的块，第三个子集分成大小都是9的块。所以
$$F(x)=A(x)B(x)C(x)=e^{\frac{x^3}{3!}+\frac{x^4}{4!}+\frac{x^9}{9!}}$$

**Theorem 8.28** (`Compositional formula`, `Exponential version`). 令$a_n$表示在$n$个元素的集合上构建第一类结构的方式的个数，并且$a_0=0$。令$b_n$表示在$n$个元素的集合上构建第二类结构的方式的个数，并且$b_0=1$。令$g_n$表示下述操作的个数：将集合$[n]$分成若干个非空子集，在每个自己上构建第一类结构，然后对所有子集组成的集合构建第二类结构，且$g_0=1$。$A(x),B(x),G(x)$分别是序列$\{a_n\},\{b_n\},\{g_n\}$的指数生成函数，那么
$$G(x)=B(A(x))$$
**Proof** 不妨设将$[n]$分成$k$个子集。$k$个子集上构建第一类结构的指数生成函数是$A(x)^k/k!$，有$b_k$种方式在$k$个子集组成的集合上构建第二类结构，那么根据**Theorem 8.21**$b_kA(x)^k/k!$是$k$个子集时的指数生成函数。$a_0=0$，并且$b_kA(x)^k/k!$中没有常数项，但是$G(x)$有常数项1，所以
$$G(x)=1+\sum_{k\geq 1}b_k\frac{A(x)^k}{k!}=\sum_{k\geq 0}b_k\frac{A(x)^k}{k!}=B(A(x))$$

**Example 8.29.** 有$n$张不同的纸牌。将其分成非空的若干组，要求每组是偶数张牌，对每一组进行排序，然后将这些组排成一行。求有$g_n$种排列方式。  
**Solution** $n\geq 2$且是偶数时，$a_n=n!$；$n$是奇数时，$a_n=0$。显然$b_n=n!,n\geq 0$。因此
$$A(x)=\sum_{n\geq 0}a_n\frac{x^n}{n!}=\sum_{n\geq 2, even}x^n=\frac{x^2}{1-x^2}$$
$$B(x)=\sum_{n\geq 0}b_n\frac{x^n}{n!}=\sum_{n\geq 0}x^n=\frac{1}{1-x}$$
所以
$$\begin{aligned}
G(x)&=B(A(x))\\
&=\frac{1}{1-\frac{x^2}{1-x^2}}\\
&=\frac{1-x^2}{1-2x^2}\\
&=1+\frac{x^2}{1-2x^2}\\
&=1+x^2\sum_{m\geq 0}(2x^2)^m\\
&=1+\sum_{m\geq 0}2^mx^{2m+2}
\end{aligned}$$
$n$是奇数的时候，$G(x)$中$x^n/n!$的系数是0；$n=2m,m>0$时，系数是$2^{m-1}(2m)!$。所以对于偶数$n$，$g_n=2^{\frac{n}{2}-1}\cdot n!$。

## Exercises
(7) $h_n$表示**Example 8.14**定义的序列，$f_n$表示**Exercise 5**定义的序列（斐波那契数列）。使用双射直接证明$h_n=f(2n-1)$（不能使用递归关系或者生成函数）。  
**Solution** $f(2n-1)$表示将$2n-1$分成1或2的`composition`。任意一个`composition`$\alpha$有$2k-1$个1和$n-k$个2。对于这些1和2，从左向右读，遇到2标记为非指挥官，第一次遇到1标记为指挥官，第二次遇到1标记为小单元的分隔符，以此类推，第$3,5,7,\cdots$次遇到1标记为指挥官，第$4,6,8,\cdots$遇到1标记为小单元的分隔符。那么创建了$k$个单元，指定了$k$个指挥官。  
比如$n=8$，`composition`$\alpha=2+2+1+1+2+1+2+2+1+1=15$，对应的$\beta(\alpha)=NNC|NCNN|C$，其中$N$是非指挥官，$C$是指挥官，$|$是单元的分隔符。  
从$\beta$到$\alpha$刚好相反。将$\beta$中的$N$还原成2，将$C$和$|$还原成1得到相应的$\alpha$。

(10) $f(n)$是$[n]$的子集中任意两个元素的差至少是3的子集个数，求$f(n)$的生成函数。  
**Solution** 考虑这样一个子集。若$n$不在里面，则有$f(n-1)$种选择；若$n$在该子集，那么显然$n-1,n-2$不能在，那么有$f(n-1)$种选择。所以
$$f(n)=f(n-1)+f(n-2)$$
显然，$f(0)=1,f(1)=2,f(2)=3$，递归式乘以$x^n$且对$n\geq 3$求和
$$\sum_{n\geq 3}f(n)x^n=x\sum_{n\geq 3}f(n-1)x^{n-1}+x^3\sum_{n\geq 3}f(n-3)x^{n-3}$$
$$F(x)-3x^2-2x-1=x(F(x)-2x-1)+x^3F(x)$$
所以
$$F(x)=\frac{1+x+x^2}{1-x-x^3}$$

(16) $a_n$表示满足如下条件的单调函数$f$的个数：从$[n]$到$[n]$的函数，且对于任意$i\in[n]$，$f(i)\leq i$。求$a_n$。  
**Solution** 不妨设$i$是集合$[n]$中最大的满足$f(i)=i$的值。根据题意，$f(1)=1$总是成立的。那么在$[i]$上的函数$f$有$a_{i-1}$种可能性。对于$\{i+1,i+2,\cdots,n\}$，$f(j)=j$是不可能的。根据题意$f(i+1)\leq i+1$，根据前面假设$f(i+1)\neq i+1$，那么$f(i+1)=i$，并且
$$i+1\leq f(i+2)+1\leq f(i+3)+1\leq\cdots\leq f(n)+1\leq n$$
$f(i+1)=i$还可以写作$f(i+1)-(i-1)=1$，上式可以写作
$$1\leq f(i+2)-(i-1)\leq f(i+3)-(i-1)\leq\cdots\leq f(n)-(i-1)\leq n-i$$
令$g(j)=f(j+i)-(i-1)$，上式说明集合的后段（$\geq i+2$）有$a_{n-i}$种可能性。因此当$n\geq 1$时有
$$a_n=\sum_{i=1}^n a_{i-1}a_{n-i}$$
且$a_0=1$。两边同乘$x^n$且对$n\geq 1$求和
$$C(x)-1=xC(x)^2$$
上式与$(8.13)$一致，所以$a_n=c_n=\begin{pmatrix}2n\\n\end{pmatrix}/(n+1)$。

(18) 一条路径从原点$(0,0)$到$(n, n)$，可走的步数是$(1,0), (1,1), (0,1)$，且要求不能超过对角线$x=y$，$f_n$表示满足这些条件的路径的数量。求生成函数$F(x)=\sum_{n\geq 0}f_nx^n$。$f_n$被称为`Schr ̈oder numbers`。  
**Solution** 首先找到递归式。如果第一步是$(1,1)$，那么有$f_{n-1}$种路径，否则不妨设第一个和对角线的焦点是$(i, i)$，那么后半段有$f_{n-i}$种路径，前半段有$f_{i-1}$种路径。前者很明显，后者的是因为不能和对角线相交，那么第一步一定是$(1,0)$，最后一步一定是从$(i,i-1)$向上到$(i,i)$。  
当$n\geq 1$时，递归式是$f_n=f_{n-1}+\sum_{n\geq 1}f_{i-1}f_{n-i}$；$f_0=1$。两边同乘$x^n$且对$n\geq 1$求和
$$F(x)-1=xF(x)+xF(x)^2$$
$$F(x)=\frac{1-x-\sqrt{x^2-6x-1}}{2x}$$
另外一个根的常数是-2而不是1，所以排除了。

(20) 当$n\geq 0$时，$r(n)$满足下述递归式
$$r(n+2)=r(n+1)+(n+1)r(n)$$
且$r(0)=r(1)=1$。求指数生成函数$R(x)$。  
**Solution**
> 我的解体思路与答案略微不同。

两边同乘$x^{n+1}/(n+1)!$并求和
$$\sum_{n\geq 0}r(n+2)\frac{x^{n+1}}{(n+1)!}=\sum_{n\geq 0}r(n+1)\frac{x^{n+1}}{(n+1)!}+\sum_{n\geq 0}(n+1)r(n)\frac{x^{n+1}}{(n+1)!}$$
$$\sum_{n\geq 0}r(n+1)\frac{x^n}{n!}-1=R(x)-1+xR(x)$$
$$R(x)'-1=R(x)-1+xR(x)$$
$$R(x)'=R(x)+xR(x)$$
$$R(x)=e^{x+x^2/2}$$

(23) $b(n)$表示将$n$分成每部分都是奇数的composition的个数。求生成函数$B(x)$。  
**Solution** 分成每个部分都是奇数的composition等价于分成奇数长度的区间。区间是奇数的时候，对应序列的值是1，否则是0，所以生成函数$A(x)=x+x^3+x^5+\cdots=x/(1-x^2)$。根据**Theorem 8.13**，联合若干个区间得到的生成函数是
$$B(x)=\frac{1}{1-A(x)}=\frac{1}{1-\frac{x}{1-x^x}}=\frac{1-x^2}{1-x-x^x}$$
