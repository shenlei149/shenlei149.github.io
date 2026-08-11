## Enumerating The Elements of Intersecting Sets
**Example 7.1.** 有14个同学踢足球17个同学打篮球，其中4个同学参与了两者。问至少玩其中一种运动的同学有多少个？  
**Solution.** 略。

**Example 7.2.** 有14个同学踢足球17个同学打篮球18个同学玩曲棍球，其中4个同学同时玩足球和篮球，3个同学同时玩足球和曲棍球，5个同学同时玩篮球和曲棍球，1个同学三者都参与了。问至少玩其中一种运动的同学有多少个？  
**Solution.** 略。

**Theorem 7.3** [(`Sieve Formula`)或容斥原理(`Principle of Inclusion-Exclusion`)]. $A_1,A_2,\cdots,A_n$是有限集合，有
$$|A_1\cup A_2\cup\cdots\cup A_n|=\sum_{j=1}^n(-1)^{j-1}\sum_{i_1,i_2,\cdots,i_j}|A_{i_1}\cap A_{i_2}\cap\cdots\cap A_{i_j}|$$
其中$\{{i_1,i_2,\cdots,i_j}\}$是所有$[n]$的$j$个元素子集。  
**Proof.** 考虑任意元素$x\in A_1\cup A_2\cup\cdots\cup A_n$，显然，左边只计算了一次$x$。令$S\subseteq [n]$是索引的集合，使得$x\in A_i$当且仅当$i\in S$，$s=|S|$。由于仅当$i\in S$时有$x\in A_i$，那么$A_{i_1}\cap A_{i_2}\cap\cdots\cap A_{i_t}$不包含$x$，除非$(i_1,i_2,\cdots,i_t)\subseteq S$。所以计算$x$在右边出现的次数时，只需要考虑$A_i$被$S$索引的时候；也就是说，这些交集都包含$x$。比如$j=2$时，从$S$任取两个值$p,q$，对应的$A_p,A_q$都是包含$x$的。那么右边计算$x$的次数是
$$s-\begin{pmatrix}
s\\2
\end{pmatrix}+\begin{pmatrix}
s\\3
\end{pmatrix}-\cdots +(-1)^{s-1}\begin{pmatrix}
s\\s
\end{pmatrix}$$
根据二项式定理
$$(1-1)^s=0=1-s+\begin{pmatrix}
s\\2
\end{pmatrix}-\begin{pmatrix}
s\\3
\end{pmatrix}+\cdots (-1)^{s-1}\begin{pmatrix}
s\\s
\end{pmatrix}$$
所以右边也只计算了$x$一次。

## Applications of the Sieve Formula
**Example 7.4.** 一个派对有$n$个客人，他们的帽子放在衣帽间。然后停电了，客人随机拿了一个帽子，他们发现所有人都拿到别人的帽子。求有多少种这种情况发生呢？  
这就是错位排列(`derangement`)。$[n]$的错位排列就是其没有不动点(`fixed point`)，没有元素$i$在第$i$个位置上。  
**Solution.** 全排列减去至少有一个不动点就是要求的数。  
用$A_i$表示元素$i$在位置$i$上。Theorem 7.3告诉了计数的方法。  
$A_1$是多少呢？元素1在位置1上，其余$n-1$个元素可以自由排列，所以$|A_1|=(n-1)!$，类似的，$|A_2|=(n-1)!$，$|A_i|=(n-1)!$，那么公式右边的第一项是$(n-1)!\cdot n=n!$。  
接下来考察$|A_i\cap A_j|$。两个元素是不动点，那么有$(n-2)!$中排列方式，从$n$个元素里面选择两个，所以第二项是
$$-\begin{pmatrix}
n\\2
\end{pmatrix}(n-2)!=-\frac{n!}{2}$$
依此类推，第$i$项是
$$(-1)^{i-1}\begin{pmatrix}
n\\i
\end{pmatrix}(n-i)!=(-1)^{i-1}\frac{n!}{i!}$$
根据Theorem 7.3
$$\begin{aligned}
|A_1\cup A_2\cup\cdots\cup A_n|&=\sum_{j=1}^n(-1)^{j-1}\sum_{i_1,i_2,\cdots,i_j}|A_{i_1}\cap A_{i_2}\cap\cdots\cap A_{i_j}|\\
&=n!-\frac{n!}{2}+\frac{n!}{3!}-\cdots+(-1)^{n-1}\frac{n!}{n!}\\
&=\sum_{i=1}^n(-1)^{i-1}\frac{n!}{i!}
\end{aligned}$$
使用全排列$n!$减去上式就得到没有不动点的个数$D(n)$
$$D(n)=\sum_{i=0}^n(-1)^i\frac{n!}{i!}$$
和$n!$有什么关系呢？
$$\frac{D(n)}{n!}=\sum_{i=0}^n\frac{(-1)^i}{i!}$$
当$n$趋于无穷大的时候，由泰勒级数
$$e^x=\sum_{n=0}^\infty \frac{x^n}{n!}$$
得到右边收敛于$1/e$。

下面给出第二类斯特林数的一个求和公式。  
**Theorem 7.5.** 对于所有正整数$n$和$k$，有
$$S(n,k)=\frac{1}{k!}\sum_{i=0}^k(-1)^i\begin{pmatrix}
k\\i
\end{pmatrix}(k-i)^n=\sum_{i=0}^k(-1)^i\frac{1}{i!(k-i)!}(k-i)^n$$
**Proof.** 通过证明$k!\cdot S(n,k)$来证明$S(n,k)$。从Corollary 5.9得到前者等于$[n]$到$[k]$满射函数的个数。  
从$[n]$到$[k]$的函数总数是$k^n$，但是这里有很多非满射的情况，需要减去。有点像之前的证明。  
$A_i$表示从$[n]$到$[k]$的不包含$i$的所有函数，那么$|A_i|=(k-1)^n$。类似的
$$|A_{i_1} \cap A_{i_2} \cap \cdots \cap A_{i_j}|=(k-j)^n$$
那么
$$|A_1 \cup A_2 \cup \cdots \cup A_n|=\sum_{i=1}^k(-1)^{i-1}\begin{pmatrix}
k\\i
\end{pmatrix}(k-i)^n$$
用总数减去上式就是要证明的结论。

**Theorem 7.6.** $f,g$是定义在$[n]$的子集上的函数，其范围是实数集。假设
$$g(S)=\sum_{T\subseteq S}f(T)$$
那么
$$f(S)=\sum_{T\subseteq S}g(T)(-1)^{|S-T|}$$
**Proof.** 将右侧的$g(T)$展开，对于所有$U\subseteq S$，$f(U)$会出现一次如果$U\subseteq T\subseteq S$。同时注意到$f(U)$会乘以$(-1)^{|S-T|}$，满足$|S-T|=i$的$T$的子集个数是$\begin{pmatrix}|S-U|\\i\end{pmatrix}$，其意义是从更大的差集里面选择$i$个元素组成小的差集。  
那么右侧$f(U)$出现的次数是
$$\sum_{i=0}^{|S-U|}(-1)^i\begin{pmatrix}
|S-U|\\i
\end{pmatrix}=(1-1)^{|S-U|}$$
除了$S=U$外，其他值均为0，所以右侧就是$f(S)$。

## Exercises
(36)(37) $F(n)$表示集合$[n]$分区中没有单元素块的数量。使用贝尔数$B(n)$来表示$F(n)$，进而证明
$$\lim_{n\to\infty}\frac{F(n)}{B(n)}$$
**Solution**
> 第一步在提问前找到了答案，后一部分提问才明白怎么证明，其实不需要知道公式也能证明，包括第一步的解题思路也是要定义二者的差值。https://math.stackexchange.com/questions/3804855/relation-between-bell-number-and-fn-the-number-of-partitions-of-n-withou
