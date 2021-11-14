### 1
这个题目没有统一的答案。我心中的解是参考答案的第一个，这个式子本身的指标集合是空集，和式为零。第二种是说对 $k$ 递减求和，即 $q_4+q_3+q_2+q_1+q_0$，但是这与 $n=0$ 时 $\sum_{k=1}^n q_k=0$ 的约定矛盾。第三种含义是 $\sum_{k=m}^nq_k=\sum_{k\leq n}q_k-\sum_{k<m}q_k$，那么题目的和式就等于 $-q_1-q_2-q_3$。

### 2
当 $x>0$ 时，$x\times ([x>0]-[x<0])=x\times (1-0)=x$，当 $x<0$ 时，$x\times ([x>0]-[x<0])=x\times (0-1)=-x$，所以 $x\times ([x>0]-[x<0])=|x|$。

### 3
第一个和式 $k\in \{0,1,2,3,4,5\}$，所以
$$\sum_{0\leq k\leq 5}a_k=a_0+a_1+a_2+a_3+a_4+a_5$$
第二个和式 $k\in \{-2,-1,0,1,2\}$，所以
$$\sum_{0\leq k^2\leq 5}a_{k^2}=a_4+a_1+a_0+a_1+a_4$$

### 4
满足集合条件的 $ijk$ 总共只有四种情况，分别是 $123,124,134,234$。  
先对 $k$ 求和，再对 $j,i$ 求和。就是
$$((a_{123}+a_{124})+a_{134})+a_{234}$$
第二个是
$$a_{123}+(a_{124}+(a_{134}+a_{234}))$$

### 5
第二个等式左右两边不同的指标 $j,k$ 变成了一个指标 $k$。这一点只有当对任意 $0\leq j,k\leq n$ 时，有 $a_j=a_k$ 才成立。

### 6
当 $1\leq j\leq n$ 时，和式的含义是 $[j,n]$ 的个数，所以和式值是 $[1\leq j\leq n](n-j+1)$。

### 7
$$\begin{aligned}
\nabla(x^{\overline{m}})&=x^{\overline{m}}-(x-1)^{\overline{m}}\\
&=x(x+1)(x+2)\cdots(x+m-2)(x+m-1)-(x-1)x(x+1)\cdots(x+m-2)\\
&=x(x+1)\cdots(x+m-2)(x+m-1-x+1)\\
&=mx(x+1)\cdots(x+m-2)\\
&=mx^{\overline{m-1}}
\end{aligned}$$

### 8
当 $m\geq 1$ 时，包含因子 0，所以整个式子的值是 0。  
当 $m\leq 0$ 时，根据负指数的定义
$$0^{\underline{m}}=\frac{1}{1\cdot 2\cdot 3\cdots (|m|)}=\frac{1}{|m|!}$$

### 9
等式 $(2.52)$ 需要改写为
$$x^{\overline{m+n}}=x^{\overline{m}}(x+m)^{\overline{n}}$$
令 $m=-n$，那么上式可以写作
$$x^{\overline{0}}=1=x^{\overline{-n}}(x-n)^{\overline{n}}$$
那么
$$\begin{aligned}
x^{\overline{-n}}&=\frac{1}{(x-n)^{\overline{n}}}\\
&=\frac{1}{(x-n)(x-n+1)\cdots(x-n+(n-1))}\\
&=\frac{1}{(x-1)(x-2)\cdots(x-n)}\\
&=\frac{1}{(x-1)^{\underline{n}}}
\end{aligned}$$

### 10
推导公式 $(2.54)$ 稍微变换一下中间项的展开和组合方式可以得到
$$\begin{aligned}
\Delta(u(x)v(x))&=u(x+1)v(x+1)-u(x)v(x)\\
&=u(x+1)v(x+1)-u(x+1)v(x)+u(x+1)v(x)-u(x)v(x)\\
&=u(x+1)\Delta v(x)+v(x)\Delta u(x)\\
&=Eu\Delta v+v\Delta u
\end{aligned}$$
得到了另外一个和 $(2.54)$ 对称的结果。

### 11
$$\begin{aligned}
\sum_{0\leq k<n}(a_{k+1}-a_k)b_k&=\sum_{0\leq k<n}a_{k+1}b_k-\sum_{0\leq k<n}a_kb_k\\
&=\sum_{0\leq k<n}a_{k+1}b_k-(\sum_{-1\leq k<n-1}a_{k+1}b_{k+1})\\
&=\sum_{0\leq k<n}a_{k+1}b_k-(\sum_{0\leq k<n-1}a_{k+1}b_{k+1}+a_0b_0)\\
&=\sum_{0\leq k<n}a_{k+1}b_k-(\sum_{0\leq k<n}a_{k+1}b_{k+1}+a_0b_0-a_nb_n)\\
&=a_nb_n-a_0b_0+\sum_{0\leq k<n}a_{k+1}(b_k-b_{k+1})\\
&=a_nb_n-a_0b_0-\sum_{0\leq k<n}a_{k+1}(b_{k+1}-b_k)
\end{aligned}$$

### 12
令 $p(k)=n=k+(-1)^kc$，两边同时加上$c$得到
$$n+c=k+((-1)^k+1)c$$
那么
$$(-1)^{n+c}=(-1)^{k+((-1)^k+1)c}=(-1)^k\cdot(-1)^{((-1)^k+1)c}$$
由于 $((-1)^k+1)$ 是个偶数，即零或者二，所以 $(-1)^{((-1)^k+1)c}=1$，那么
$$(-1)^{n+c}=(-1)^k$$
代入到 $p(k)$ 的定义中得到
$$n=k+(-1)^{n+c}c$$
所以
$$k=n-(-1)^{n+c}c$$
那么对于任意 $n$，可以反推出来原始的 $k$。

### 13
令
$$\begin{aligned}
R_0&=\alpha\\
R_n&=R_{n-1}+(-1)^n(\beta+\gamma n+\delta n^2)
\end{aligned}$$
那么
$$R_n=A(n)\alpha+B(n)\beta+C(n)\gamma+D(n)\delta$$
要求的和式是
$$\sum_{k=0}^n(-1)^kk^2$$
对应递归式得到对应的 $\alpha=\beta=\gamma=0,\delta=1$，也就是 $D(n)$ 就是要求的和式。

令 $R_n=(-1)^nn$，那么 $R_0=0=\alpha$。
$$\begin{aligned}
(-1)^nn&=(-1)^{n-1}(n-1)+(-1)^n(\beta+\gamma n+\delta n^2)\\
(-1)n&=(n-1)+(-1)(\beta+\gamma n+\delta n^2)\\
-n&=n-1-\beta-\gamma n-\delta n^2\\
-2n+1&=-\beta-\gamma n-\delta n^2
\end{aligned}$$
所以
$$\beta=-1,\gamma=2,\delta=0$$
那么
$$(-1)^nn=-B(n)+2C(n)$$

令 $R_n=(-1)^nn^2$，那么 $R_0=0=\alpha$。
$$\begin{aligned}
(-1)^nn^2&=(-1)^{n-1}(n-1)^2+(-1)^n(\beta+\gamma n+\delta n^2)\\
(-1)n^2&=(n-1)^2+(-1)(\beta+\gamma n+\delta n^2)\\
-n^2&=n^2-2n+1-\beta-\gamma n-\delta n^2\\
-2n^2+2n-1&=-\beta-\gamma n-\delta n^2
\end{aligned}$$
所以
$$\beta=1,\gamma=-2,\delta=2$$
那么
$$(-1)^nn^2=B(n)-2C(n)+2D(n)$$
可以得到 $D(n)$
$$2D(n)=(-1)^nn^2+(-B(n)+2C(n))=(-1)^nn^2+(-1)^nn=(-1)^n(n^2+n)$$

### 14
由于 $k=\sum_{1\leq j\leq k}1$，所以 $\sum_{1\leq k\leq n}k2^k$ 可以改写成 $\sum_{1\leq j\leq k\leq n}2^k$。先对 $k$ 求和。
$$\begin{aligned}
\sum_{1\leq j\leq k\leq n}2^k&=\sum_{1\leq j\leq n}\sum_{j\leq k\leq n}2^k\\
&=\sum_{1\leq j\leq n}(2^j(2^{n-j+1-1}))\\
&=\sum_{1\leq j\leq n}(2^{n+1}-2^j)\\
&=\sum_{1\leq j\leq n}2^{n+1}-\sum_{1\leq j\leq n}2^j\\
&=n2^{n+1}-(2(2^n-1))\\
&=n2^{n+1}-(2^{n+1}-2)\\
\end{aligned}$$

### 15
根据题目的提示，并利用公式 $(2.33)$
$$\begin{aligned}
S_n+s_n&=2\sum_{1\leq j\leq k\leq n}jk\\
&=(\sum_{k=1}^n k)^2+\sum_{k=1}^n k^2\\
&=(\sum_{k=1}^n k)^2+s_n
\end{aligned}$$
所以
$$S_n=(\sum_{k=1}^n k)^2=\bigg(\frac{(1+n)n}{2}\bigg)^2$$

### 16
根据 $(2.52)$
$$x^{\underline{m+n}}=x^{\underline{m}}(x-m)^{\underline{n}}=x^{\underline{n}}(x-n)^{\underline{m}}$$
稍微调整下就得到要证明的式子。

### 17
先证明第一个式子的第一个等号。从右边开始推导
$$\begin{aligned}
(-1)^m(-x)^{\underline{m}}&=(-1)^m(-x)(-x-1)(-x-2)\cdots(-x-m+1)\\
&=(-1)^m(-1)^m(x)(x+1)(x+2)\cdots(x+m-1)\\
&=(-1)^{2m}(x)(x+1)(x+2)\cdots(x+m-1)\\
&=x^{\overline{m}}
\end{aligned}$$
证明第二个等号。把上面推导的第三行反过来看，从 $x+m-1$ 开始递减了 $m$ 次。

最后证明第三个等号。首先改下为
$$(x+m-1)^{\underline{m}}(x-1)^{\underline{-m}}=1$$
用 $x$ 替代 $x-1$ 得到
$$(x+m)^{\underline{m}}(x)^{\underline{-m}}=1$$
令 $n=-m$ 代入上式且利用 $(2.52)$
$$(x)^{\underline{n}}(x-n)^{\underline{m}}=x^{\underline{n+m}}=x^{\underline{0}}=1$$

第二个式子的证明基本类似。

### 18
首先只考虑实部，分成正负两个部分。$\sum a_k$ 收敛意味着 $\sum \mathfrak{R}a_k$ 收敛，那么 $\sum \mathfrak{R}a_k^+$ 和 $\sum \mathfrak{R}a_k^-$ 收敛，那么可以记作
$$\sum \mathfrak{R}a_k^+\leq A_1$$
$$\sum \mathfrak{R}a_k^-\leq A_2$$
其中 $A_1$ 是正数，$A_2$ 是负数，对于求和数加上绝对值符号，得到
$$\sum |\mathfrak{R}a_k^+|\leq |A_1|=A_1$$
$$\sum |\mathfrak{R}a_k^-|\leq |A_2|=-A_2$$
类似的虚部也同样处理，得到
$$\sum |\mathfrak{I}a_k^+|\leq |B_1|=B_1$$
$$\sum |\mathfrak{I}a_k^-|\leq |B_2|=-B_2$$
利用复数的性质
$$|a_k|\leq |\mathfrak{R}a_k|+|\mathfrak{I}a_k|$$
分正数和负数分别考虑，求和之后小于等于 $A_1-A_2+B_1-B_2$，其和是收敛的。

### 19
求和因子
$$s_n=\frac{a_{n-1}a_{n-2}\cdots a_1}{b_nb_{n-1}\cdots b_2}$$
这个题目中 $a_n=2,b_n=n$，所以
$$s_n=\frac{2^{n-1}}{n!}$$
利用 $(2.10)$
$$T_n=\frac{1}{s_na_n}(s_1b_1T_0+\sum_{k=1}^ns_kc_k)$$
来计算结果。其中
$$s_na_n=\frac{2^n}{n!}$$
$$s_1b_1T_0=1\cdot 1\cdot 5=5$$
$$s_kc_k=\frac{2^{k-1}}{k!}\cdot 3\cdot k!=3\cdot 2^{k-1}$$
代入 $T_n$ 得到
$$\begin{aligned}
T_n&=\frac{n!}{2^n}(5+3\cdot\sum_{k=1}^n 2^{k-1})\\
&=\frac{n!}{2^n}(5+3\cdot(2^n-1))\\
&=\frac{n!}{2^n}(3\cdot 2^n+2)\\
&=3\cdot n!+\frac{n!}{2^{n-1}}
\end{aligned}$$

### 20
扰动法，加上额外一项然后重组顺序
$$\begin{aligned}
S_n+(n+1)H_{n+1}&=\sum_{0\leq k\leq n+1}(kH_k)\\
&=0+\sum_{1\leq k\leq n+1}(kH_k)\\
&=\sum_{0\leq k\leq n}((k+1)H_{k+1})\\
&=\sum_{0\leq k\leq n}((k+1)(H_k+\frac{1}{k+1}))\\
&=\sum_{0\leq k\leq n}(kH_k+H_k+1)\\
&=\sum_{0\leq k\leq n}(kH_k)+\sum_{0\leq k\leq n}H_k+\sum_{0\leq k\leq n}1)\\
&=S_n+\sum_{0\leq k\leq n}H_k+(n+1)
\end{aligned}$$
所以
$$\sum_{0\leq k\leq n}H_k=(n+1)(H_{n+1}-1)$$

### 21
对 $S_{n+1}$ 提取第一项得到
$$\begin{aligned}
S_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}\\
&=(-1)^{n+1}+\sum_{k=1}^{n+1}(-1)^{n+1-k}\\
&=(-1)^{n+1}+\sum_{k=0}^{n}(-1)^{n+1-(k+1)}\\
&=(-1)^{n+1}+\sum_{k=0}^{n}(-1)^{n-k}\\
&=(-1)^{n+1}+S_n\\
\end{aligned}$$
对 $S_{n+1}$ 提取最后一项得到
$$\begin{aligned}
S_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}\\
&=\sum_{k=0}^{n}(-1)^{n+1-k}+1\\
&=(-1)\sum_{k=0}^{n}(-1)^{n-k}+1\\
&=1-S_n
\end{aligned}$$
所以
$$1-S_n=(-1)^{n+1}+S_n$$
$$2S_n=1-(-1)^{n+1}=1+(-1)^{n+2}=1+(-1)^n$$
即当 $n$ 是偶数时 $S_n=1$，$n$ 是奇数时 $S_n=0$。

对 $T_{n+1}$ 提取第一项得到
$$\begin{aligned}
T_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}k\\
&=0+\sum_{k=1}^{n+1}(-1)^{n+1-k}k\\
&=\sum_{k=0}^{n}(-1)^{n-k}(k+1)\\
&=T_n+S_n
\end{aligned}$$
对 $T_{n+1}$ 提取最后一项得到
$$\begin{aligned}
T_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}k\\
&=\sum_{k=0}^{n}(-1)^{n+1-k}k+(-1)^0\cdot(n+1)\\
&=(-1)\sum_{k=0}^{n}(-1)^{n-k}k+(n+1)\\
&=n+1-T_n
\end{aligned}$$
所以
$$T_n+S_n=n+1-T_n$$
$$2T_n=n+1-S_n$$
即当 $n$ 是偶数时 $T_n=n/2$，$n$ 是奇数时 $T_n=(n+1)/2$。

对 $U_{n+1}$ 提取第一项得到
$$\begin{aligned}
U_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}k^2\\
&=0+\sum_{k=1}^{n+1}(-1)^{n+1-k}k^2\\
&=\sum_{k=1}^{n+1}(-1)^{n+1-k}k^2\\
&=\sum_{k=0}^{n}(-1)^{n-k}(k+1)^2\\
&=\sum_{k=0}^{n}(-1)^{n-k}(k^2+2k+1)\\
&=U_n+2T_n+S_n\\
&=U_n+(n+1)
\end{aligned}$$
对 $U_{n+1}$ 提取最后一项得到
$$\begin{aligned}
U_{n+1}&=\sum_{k=0}^{n+1}(-1)^{n+1-k}k^2\\
&=\sum_{k=0}^{n}(-1)^{n+1-k}k^2+(-1)^0\cdot(n+1)^2\\
&=(-1)\sum_{k=0}^{n}(-1)^{n-k}k^2+(n+1)^2\\
&=(n+1)^2-U_n
\end{aligned}$$
所以
$$U_n+(n+1)=(n+1)^2-U_n$$
$$2U_n=(n+1)^2-(n+1)=n(n+1)$$
$$U_n=\frac{1}{2}n(n+1)$$

### 22
令
$$S_n=\sum_{1\leq j<k\leq n}(a_jb_k-a_kb_j)(A_jB_k-A_kB_j)$$
利用恒等式
$$[1\leq j<k\leq n]+[1\leq k<j\leq n]=[1\leq j,k\leq n]-[1\leq j=k\leq n]$$
得到
$$2S_n=\sum_{1\leq j,k\leq n}(a_jb_k-a_kb_j)(A_jB_k-A_kB_j)-\sum_{1\leq j=k\leq n}(a_jb_k-a_kb_j)(A_jB_k-A_kB_j)$$
由于 $j=k$，所以第二项是零。现在把第一项展成四项。
$$2S_n=\sum_{1\leq j,k\leq n}(a_jb_kA_jB_k)-\sum_{1\leq j,k\leq n}(a_jb_kA_kB_j)-\sum_{1\leq j,k\leq n}(a_kb_jA_jB_k)+\sum_{1\leq j,k\leq n}(a_kb_jA_kB_j)$$
由于 $j,k$ 完全等价，可以互换，那么第一项和第四项是相等的，第二项和第三项是相等的。那么
$$S_n=\sum_{1\leq j,k\leq n}(a_jb_kA_jB_k)-\sum_{1\leq j,k\leq n}(a_jb_kA_kB_j)$$
根据公式 $(2.28)$
$$\sum_{j\in J,k\in K}a_jb_k=(\sum_{j\in J}a_j)(\sum_{k\in K}b_k)$$
改写上面的式子得到
$$S_n=\sum_{1\leq j\leq n}(a_jA_j)\sum_{1\leq k\leq n}(b_kB_k)-\sum_{1\leq j\leq n}(a_jB_j)\sum_{1\leq k\leq n}(b_kA_k)$$
统一下标得到
$$S_n=\sum_{1\leq k\leq n}(a_kA_k)\sum_{1\leq k\leq n}(b_kB_k)-\sum_{1\leq k\leq n}(a_kB_k)\sum_{1\leq k\leq n}(b_kA_k)$$

### 23
方法一：用 $1/k-1/(k+1)$ 代替 $1/(k(k+1))$ 得到
$$\begin{aligned}
\sum_{k=1}^n\frac{2k+1}{k(k+1)}&=\sum_{k=1}^n\frac{2k+1}{k}-\sum_{k=1}^n\frac{2k+1}{k+1}\\
&=\sum_{k=1}^n(2+\frac{1}{k})-\sum_{k=1}^n(2-\frac{1}{k+1})\\
&=2n+H_n-2n+(H_n-1+\frac{1}{n+1})
\end{aligned}$$
方法二：分部求和法。令 $u=2k+1,\Delta v=\frac{1}{k(k+1)}=(k-1)^{\underline{-2}}$，那么 $\Delta u=2,v=-(k-1)^{\underline{-1}}=-\frac{1}{k}$。
$$\begin{aligned}
\sum_{k=1}^n\frac{2k+1}{k(k+1)}&=uv-\sum Ev\Delta u\\
&=-\frac{1}{x}(2x+1)\bigg|_1^{n+1}-\sum_{k=1}^n-2\frac{1}{x+1}\\
&=\frac{1}{x}(2x+1)\bigg|_{n+1}^1+2\sum_{k=1}^n\frac{1}{x+1}\\
&=1-\frac{1}{n+1}+2(H_n-1+\frac{1}{n+1})\\
&=1-\frac{1}{n+1}+2H_n-2+\frac{2}{n+1}\\
&=2H_n-1+\frac{1}{n+1}
\end{aligned}$$

### 24
这个题目前和答案不一致。但是不知道哪里错了。TODO。

分部求和。令 $\Delta v=x^{\underline{m}}, u=H_x$，那么 $v=\frac{x^{\underline{m+1}}}{m+1},\Delta u=\frac{1}{x+1}=x^{\underline{-1}}$，
$$Ev \Delta u=\frac{(x+1)^{\underline{m+1}}}{m+1}\frac{1}{x+1}=\frac{x^{\underline{m}}}{m+1}$$
那么
$$\begin{aligned}
\sum_{0\leq k<n} k^{\underline{m}}H_k&=(H_x\frac{x^{\underline{m+1}}}{m+1}-\frac{x^{\underline{m}}}{m+1})\bigg|_0^n\\
&=(H_n\frac{n^{\underline{m+1}}}{m+1}-\frac{n^{\underline{m}}}{m+1})+\frac{0^{\underline{m}}}{m+1}
\end{aligned}$$
题目中 $m=-2$，那么
$$
\begin{aligned}
\sum_{0\leq k<n} k^{\underline{m}}H_k&=(H_n\frac{n^{\underline{-1}}}{-1}-\frac{n^{\underline{-2}}}{-1})+\frac{0^{\underline{-1}}}{-1}\\
&=-H_n\frac{1}{n+1}+\frac{1}{(n+1)(n+2)}-1
\end{aligned}
$$

### 25
TODO

### 26
TODO

### 27
TODO

### 28
TODO
