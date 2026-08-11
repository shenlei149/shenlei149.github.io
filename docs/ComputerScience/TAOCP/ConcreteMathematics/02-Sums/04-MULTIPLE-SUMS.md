一个和式的项可以用两个或多个指标指定。比如，下面是一个有九项的二重和式，由指标$j,k$控制：
$$\sum_{1\leq j,k\leq 3}a_jb_k=a_1b_1+a_1b_2+a_1b_3+a_2b_1+a_2b_2+a_2b_3+a_3b_1+a_3b_2+a_3b_3$$
对于这样的和式，可以使用与单个指标一样的方式表示。如果$P(j,k)$是关于$j,k$的属性，所有使得$P(j,k)$为真的项$a_{j,k}$之和可以用下面两种方式表示。一种是用艾弗森约定对所有的整数$j,k$进行求和
$$\sum_{P(j,k)}a_{j,k}=\sum_{j,k}a_{j,k}[P(j,k)]$$
尽管有多于一个求和指标，但只需要一个$\sum$符号；$\sum$表示对所有适用的指标组合求和。  
当谈论一个和式的和式时，我们有时也会用两个$\sum$符号。比如
$$\sum_j\sum_ka_{j,k}[P(j,k)]$$
就是
$$\sum_j(\sum_ka_{j,k}[P(j,k)])$$
的缩写。这个式子是内层$\sum_ka_{j,k}[P(j,k)]$对所有的$j$求和，而$\sum_ka_{j,k}[P(j,k)]$本身是$a_{j,k}$对所有$[P(j,k)]$为真的所有整数$k$求和。这样的情景，称此二重和式“首先对$k$求和”。多于一个指标的和式可以首先对任意一个指标求和。

因此，我们有称为交换求和次序（`interchanging the order of summation`）的基本法则，推广了早先提到的结合律$(2.16)$。
$$\sum_j\sum_ka_{j,k}[P(j,k)]=\sum_{P(j,k)}a_{j,k}=\sum_k\sum_ja_{j,k}[P(j,k)]\tag{2.27}$$
法则中间是对两个指标求和的和式。左边$\sum_j\sum_k$先对$k$求和再对$j$求和，右边恰恰相反。实践中，想计算一个二重和式，通常对其中一个先求和比对另一个先求和简单，所以要选择方便的顺序。  
这些和式可能会使人困扰，下面多举几个例子。回到开头九个项的和式。
$$\begin{aligned}
\sum_{1\leq j,k\leq 3}a_jb_k&=\sum_{j,k}a_jb_k[1\leq j,k\leq 3]
=\sum_{j,k}a_jb_k[1\leq j\leq 3][1\leq k\leq 3]\\
&=\sum_j\sum_k a_jb_k[1\leq j\leq 3][1\leq k\leq 3]\\
&=\sum_j a_j[1\leq j\leq 3]\sum_k b_k[1\leq k\leq 3]\\
&=\sum_j a_j[1\leq j\leq 3](\sum_k b_k[1\leq k\leq 3])\\
&=(\sum_j a_j[1\leq j\leq 3])(\sum_k b_k[1\leq k\leq 3])\\
&=(\sum_{j=1}^3 a_j)(\sum_{k=1}^3 b_k)
\end{aligned}$$
第一行没有限制顺序。第二行分成了三个组
$$(a_1b_1+a_1b_2+a_1b_3)+(a_2b_1+a_2b_2+a_2b_3)+(a_3b_1+a_3b_2+a_3b_3)$$
第三行相当于是提取各个因子$a$，因为$a_j,[1\leq j\leq 3]$和$k$无关，所以
$$a_1(b_1+b_2+b_3)+a_2(b_1+b_2+b_3)+a_3(b_1+b_2+b_3)$$
第四行相比第三行多了一个括号，使得第五行不那么神秘。第五行则是对每一个$j$提取公因子$(b_1+b_2+b_3)$，那么就得到了$(a_1+a_2+a_3)(b_1+b_2+b_3)$。最后一行就是这个式子的另外一种表达形式。推导过程可以用于证明一般分配律（`general distributive law`）
$$\sum_{j\in J,k\in K}a_jb_k=(\sum_{j\in J}a_j)(\sum_{k\in K}b_k)\tag{2.28}$$
它对所有指标集$J,K$都成立。

交换求和次序的基本法则$(2.27)$有许多变形，在我们想要限制指标集的范围而不是所有整数$j,k$的时候就会出现。简单形式是
$$\sum_{j\in J}\sum_{k\in K}a_{j,k}=\sum_{j\in J,k\in K}a_{j,k}=\sum_{k\in K}\sum_{j\in J}a_{j,k}\tag{2.29}$$
这恰好是$(2.27)$的另外一种写法，因为艾佛森中$[j\in J,k\in K]$可以分解成$[j\in J][k\in K]$。这种写法只限于$j,k$范围无关时成立。  
复杂性需要一点点技巧，适用于内外范围指标有关的情况
$$\sum_{j\in J}\sum_{k\in K(j)}a_{j,k}=\sum_{k\in K'}\sum_{j\in J'(k)}a_{j,k}\tag{2.30}$$
这里的$J,K(j),K',J'(k)$必须以下面的形式关联起来：
$$[j\in J][k\in K(j)]=[k\in K'][j\in J'(k)]$$
原则上，这样的分解总是存在的。我们可以令$J=K'$是所有整数的集合，而$K(j)=J'(k)$是控制二重和式的属性$P(j,k)$对应得到的集合。对于一些重要形式，$J,K(j),K',J'(k)$存在简单形式。比如下面就是一个很有用的分解
$$[1\leq j\leq n][j\leq k\leq n]=[1\leq j\leq k\leq n]=[1\leq k\leq n][1\leq j\leq k]\tag{2.31}$$
那么
$$\sum_{j=1}^n\sum_{k=j}^na_{j,k}=\sum_{1\leq j\leq k\leq n}a_{j,k}=\sum_{k=1}^n\sum_{j=1}^ka_{j,k}\tag{2.32}$$
两个和式中，其中一个计算能比另一个简单，我们可以利用$(2.32)$把困难的转化成简单的。

现在看一个实际的例子。考虑有$n^2$个乘积$a_ia_j$组成的矩阵
$$\begin{bmatrix}
a_1a_1&&a_1a_2&&a_1a_3&&\cdots&&a_1a_n\\
a_2a_1&&a_2a_2&&a_2a_3&&\cdots&&a_2a_n\\
a_3a_1&&a_3a_2&&a_3a_3&&\cdots&&a_3a_n\\
\vdots&&\vdots&&\vdots&&\ddots&&\vdots\\
a_na_1&&a_na_2&&a_na_3&&\cdots&&a_na_n\\
\end{bmatrix}$$
我们的目的是求
$$S_{up}=\sum_{1\leq j\leq k\leq n}a_ja_k$$
他是主对角线右上角的所有元素和主对角线上的元素之和。由于$a_ja_k=a_ka_j$，所有这个行列式是对称的，从而$S_{up}$近似是所有元素和的一半（需要修正对角线元素）。  
这使得我们得到如下启发：
$$S_{up}=\sum_{1\leq j\leq k\leq n}a_ja_k=\sum_{1\leq k\leq j\leq n}a_ka_j=\sum_{1\leq k\leq j\leq n}a_ja_k=S_{down}$$
因为我们可以把$(j,k)$更名为$(k,j)$。另外，由于
$$[1\leq j\leq k\leq n]+[1\leq k\leq j\leq n]=[1\leq j,k\leq n]+[1\leq j=k\leq n]$$
所以有
$$2S_{up}=S_{up}+S_{down}=\sum_{1\leq j,k\leq n}a_ja_k+\sum_{1\leq j=k\leq n}a_ja_k$$
根据$(2.28)$，第一个和式等于$(\sum_{j=1}^na_j)(\sum_{k=1}^na_k)=(\sum_{k=1}^na_k)^2$。第二个和式是$\sum_{k=1}^na_k^2$。所以
$$S_{up}=\sum_{1\leq j\leq k\leq n}a_ja_k=\frac{1}{2}((\sum_{k=1}^na_k)^2+\sum_{k=1}^na_k^2)\tag{2.33}$$
这是一个更简单的由单个指标给出的右上三角的和的表达式。

受此启发，下面研究二重和式：
$$S=\sum_{1\leq j<k\leq n}(a_k-a_j)(b_k-b_j)$$
交换$j,k$，值不变，对称性是成立的
$$S=\sum_{1\leq k<j\leq n}(a_j-a_k)(b_j-b_k)=\sum_{1\leq k<j\leq n}(a_k-a_j)(b_k-b_j)$$
利用恒等式
$$[1\leq j<k\leq n]+[1\leq k<j\leq n]=[1\leq j,k\leq n]-[1\leq j=k\leq n]$$
得到
$$2S=\sum_{1\leq j,k\leq n}(a_j-a_k)(b_j-b_k)-\sum_{1\leq j=k\leq n}(a_j-a_k)(b_j-b_k)$$
第二个和式为零。第一个和式可以展成四个单独的和式
$$\begin{aligned}
&\sum_{1\leq j,k\leq n}a_jb_j-\sum_{1\leq j,k\leq n}a_jb_k-\sum_{1\leq j,k\leq n}a_kb_j+\sum_{1\leq j,k\leq n}a_kb_k\\
&=2\sum_{1\leq j,k\leq n}a_kb_k-2\sum_{1\leq j,k\leq n}a_jb_k\\
&=2n\sum_{1\leq k\leq n}a_kb_k-2(\sum_{k=1}^na_k)(\sum_{k=1}^nb_k)
\end{aligned}$$
上式的第一项详细步骤如下：
$$\begin{aligned}
2\sum_{1\leq j,k\leq n}a_kb_k&=2\sum_{1\leq k\leq n}\sum_{1\leq j\leq n}a_kb_k\\
&=2\sum_{1\leq k\leq n}a_kb_k\sum_{1\leq k\leq n}1\\
&=2\sum_{1\leq k\leq n}a_kb_k n\\
&=2n\sum_{1\leq k\leq n}a_kb_k
\end{aligned}$$
如果乘数（这里的$n$）和求和指标（这里的$j$）无关，那么可以提出到求和号外面。  
现在我们回到求和式，两边除以2并且调整等式顺序，得到
$$(\sum_{k=1}^na_k)(\sum_{k=1}^nb_k)=n\sum_{1\leq k\leq n}a_kb_k-\sum_{1\leq j<k\leq n}(a_k-a_j)(b_k-b_j)\tag{2.34}$$
这个恒等式是切比雪夫单调不等式的一个特例。
$$(\sum_{k=1}^na_k)(\sum_{k=1}^nb_k)\leq n\sum_{1\leq k\leq n}a_kb_k,a_1\leq\cdots\leq a_n,b_1\leq\cdots\leq b_n$$
$$(\sum_{k=1}^na_k)(\sum_{k=1}^nb_k)\geq n\sum_{1\leq k\leq n}a_kb_k,a_1\leq\cdots\leq a_n,b_1\geq\cdots\geq b_n$$
一般地，如果$a_1\leq\cdots\leq a_n$，并且$p$是$\{1,2,\cdots,n\}$的一个排列，那么$b_{p(1)}\leq\cdots\leq b_{p(n)}$时，$\sum_{k=1}^na_kb_{p(k)}$取得最大值。当$b_{p(1)}\geq\cdots\geq b_{p(n)}$，取得最小值。  
书中说，不难证明。我们考虑$n=2$的情况。不妨令$a_1\leq a_2,b_1\leq b_2$，如果有其中一个是等号，显然$a_2b_2+a_1b_1=a_2b_1+a_1b_2$，前者也是最大值。下面考虑都不等的情况。
$$\begin{aligned}
a_1&<a_2\\
\frac{a_1}{a_2}&<1\\
1-\frac{a_1}{a_2}&>0
\end{aligned}$$
那么对$b_1<b_2$两边同乘上式
$$\begin{aligned}
b_1(1-\frac{a_1}{a_2})<b_2(1-\frac{a_1}{a_2})\\
b_1a_2-b_1a_1<b_2a_2-b_2a_1\\
b_1a_2+b_2a_2<b_2a_2+b_1a_1
\end{aligned}$$
所以按序相乘得到最大值。  
考虑$n$个的情况，对于$a_i,a_{i+1}$可以独立考虑，此时假设其他值都是相等的，那么就回到了$n=2$的情况。

多重求和和单个和式中改变求值顺序有着千丝万缕的联系。如果$p(k)$是整数的任意一个排列，那么
$$\sum_{k\in K}a_k=\sum_{p(k)\in K}a_{p(k)}$$
如果$f$是任意函数
$$f:J\to K$$
将整数$j\in J$变成$k\in K$。如果用$f(j)$替换$k$会发生什么？指标替换的一般公式是
$$\sum_{j\in J}a_{f(j)}=\sum_{k\in K}a_k\#f^{\_}(k)\tag{2.35}$$
这里的$\#f^{\_}(k)$表示集合
$$f^{\_}(k)=\{j|f(j)=k\}$$
的元素个数，即使得$f(j)=k$的$j\in J$的数量。  
因为$\sum_{j\in J}[f(j)=k]=\#f^{\_}(k)$，容易通过交换求和顺序证明$(2.35)$
$$\sum_{j\in J}a_{f(j)}=\sum_{j\in J,k\in K}a_{k}[f(j)=k]=\sum_{k\in K}a_{k}\sum_{j\in J}[f(j)=k]$$
如果$f$是$J,K$上的双射（一一对应），即对所有$k$我们都有$\#f^{\_}(k)=1$，一般公式$(2.35)$就变成了
$$\sum_{j\in J}a_{f(j)}=\sum_{f(j)\in K}a_{f(j)}=\sum_{k\in K}a_k$$
这就是前面$(2.17)$的变化版本。

目前为止多重和式都是$a_k,b_k$这样的同样，本书应该是具体的（《具体数学》），所以看一个包含具体数字的例子。
$$S_n=\sum_{1\leq j<k\leq n}\frac{1}{k-j}$$
比如$S_1=0,S_2=1,S_3=\frac{1}{2-1}+\frac{1}{3-1}+\frac{1}{3-2}=\frac{5}{2}$。  
正规计算二重和式是先对$j,k$其中一个进行求和。接下来探讨两种情况。
$$\begin{aligned}
S_n&=\sum_{1\leq k\leq n}\sum_{1\leq j<k}\frac{1}{k-j}&&\text{先对$j$求和}\\
&=\sum_{1\leq k\leq n}\sum_{1\leq k-j<k}\frac{1}{j}&&\text{$k-j$替换$j$}\\
&=\sum_{1\leq k\leq n}\sum_{0< j\leq k-1}\frac{1}{j}&&\text{简化$j$的边界}\\
&=\sum_{1\leq k\leq n}H_{k-1}&&\text{$(2.13)$，这是$H_{k-1}$的定义}\\
&=\sum_{1\leq k+1\leq n}H_{k}&&\text{$k+1$替换$k$}\\
&=\sum_{0\leq k<n}H_k&&\text{简化$k$的边界}
\end{aligned}$$
我们不知道如何把调和数的和表示成封闭形式。  
尝试另外一种方法
$$\begin{aligned}
S_n&=\sum_{1\leq j\leq n}\sum_{j< k\leq n}\frac{1}{k-j}&&\text{先对$k$求和}\\
&=\sum_{1\leq j\leq n}\sum_{j< k+j\leq n}\frac{1}{k}&&\text{$k+j$替换$k$}\\
&=\sum_{1\leq j\leq n}\sum_{0< k\leq n-j}\frac{1}{k}&&\text{简化$k$的边界}\\
&=\sum_{1\leq j\leq n}H_{n-j}&&\text{$(2.13)$，这是$H_{n-j}$的定义}\\
&=\sum_{1\leq n-j\leq n}H_{j}&&\text{$n-j$替换$j$}\\
&=\sum_{0\leq j< n}H_{j}&&\text{简化$j$的边界}\\
\end{aligned}$$
到了同样的死胡同，其实这两者完全是对称的。  
还有一个方法，就是在决定怎么求和之前先用$k+j$替换$k$
$$\begin{aligned}
S_n&=\sum_{1\leq j<k\leq n}\frac{1}{k-j}&&\text{定义本身}\\
&=\sum_{1\leq j<k+j\leq n}\frac{1}{k}&&\text{$k+j$替换$k$}\\
&=\sum_{1\leq k\leq n}\sum_{1\leq j\leq n-k}\frac{1}{k}&&\text{先对$j$求和}\\
&=\sum_{1\leq k\leq n}\frac{n-k}{k}&&\text{与$j$无关}\\
&=\sum_{1\leq k\leq n}\frac{n}{k}-\sum_{1\leq k\leq n}1&&\text{结合律}\\
&=n(\sum_{1\leq k\leq n}\frac{1}{k})-n&&\text{$n$与求和下标无关，提取}\\
&=nH_n-n&&\text{$(2.13)$，这是$H_{n}$的定义}
\end{aligned}$$
结合最开始不成功的尝试，我们可以得到
$$\sum_{0\leq k<n}H_k=nH_n-n\tag{2.36}$$

我们可以从代数和几何两个角度理解这个问题。从代数角度看，如果有包含$k+f(j)$的和式，我们先用$k-f(j)$去替换$k$，且先对$j$求和；从几何角度说，看下面$S_4$的情况
$$\begin{aligned}
&k=1&&k=2&&k=3&&k=4\\
j=1&&&\frac{1}{1}&+&\frac{1}{2}&+&\frac{1}{3}\\
j=2&&&&&\frac{1}{1}&+&\frac{1}{2}\\
j=3&&&&&&&\frac{1}{1}\\
j=4
\end{aligned}$$
一开始的尝试是先对$j$求和（按列）或先对$k$求和（按行），那么得到$H_1+H_2+H_3=H_3+H_2+H_1$。成功的方法本质上是对角线求和，得到$\frac{3}{1}+\frac{2}{2}+\frac{1}{3}$。
