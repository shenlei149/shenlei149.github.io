现在看下顶函数和底函数的应用。先从简单的问题开始：$\lceil\lg 35\rceil$ 是多少？由于 $2^5<35< 2^6$，那么 $5<\lg 35<6$，所以 $\lceil\lg 35\rceil=6$。

35 写成二进制有 6 位：$(100011)_2=35$。$\lceil\lg n\rceil$ 是 $n$ 写成二进制的长度吗？并非如此。比如 $32=(100000)_2$ 需要 6 位，所以 $\lceil\lg n\rceil$ 是个错误答案，数是 2 的幂次的时候就会失效。其实 $2^{m-1}\leq n<2^m$ 需要 $m$ 位，有 $(3.5)(a)$ 可知 $m-1=\lfloor\lg n\rfloor$，所以这个问题的答案是 $\lfloor\lg n\rfloor+1$，即需要这多位的二进制才能表示 $n$。类似的，$\lceil\lg(n+1)\rceil$ 也是正确答案。如果我们认为零位二进制就能表示零这个数字，这个公式对 $n=0$ 也成立。

接下来研究多个底函数和顶函数嵌套的问题。比如 $\lceil\lfloor x\rfloor\rceil$ 是多少呢？很简单，就是 $\lfloor x\rfloor$，因为它是一个整数，外层嵌套多少层底函数或者顶函数都是一样的。

下面是一个更难的问题。证明或推翻
$$\lfloor\sqrt{\lfloor x\rfloor}\rfloor=\lfloor\sqrt{x}\rfloor,x\geq 0\tag{3.9}$$
当 $x$ 是整数时，显然成立。当 $x=\pi,e,(1+\sqrt{5})/2$ 时，也是成立的，因为 $1=1$。下面尝试证明这个命题。

当面对证明或推翻某个命题这类问题时，先尝试推翻比较好。一是只需要找一个反例就能推翻命题，二是寻找反例的过程可以激发我们的创造精神，同时有可能引向证明。

如果借助微积分来证明这个命题，想到的可能是将 $x$ 分解成整数部分和小数部分 $x=\lfloor x\rfloor+\{x\}=n+\theta$，然后将其平方根展开 $(n+\theta)^{1/2}=n^{1/2}+n^{-1/2}\theta/2-n^{-3/2}\theta^2/8\cdots$，很繁琐。

下面尝试使用上一节中的一些性质。用某种方法去掉 $\lfloor\sqrt{\lfloor x\rfloor}\rfloor$ 外层的底函数，再去掉平方根运算或内层的底函数，然后再把平方根和顶函数加回去，就得到 $\lfloor\sqrt{x}\rfloor$。我们令 $m=\lfloor\sqrt{\lfloor x\rfloor}\rfloor$，那么根据 $(3.5)(a)$ 就有 $m\leq\sqrt{\lfloor x\rfloor}<m+1$，这些数都是整数，所以平方得到 $m^2\leq\lfloor x\rfloor<(m+1)^2$，根据 $(3.7)(d)$，左边可以直接去掉底函数，根据 $(3.7)(a)$，右边也可以去掉底函数，所以有 $m^2\leq x<(m+1)^2$，现在求平方得到 $m\leq\sqrt{x}<m+1$，利用 $(3.5)(a)$，得到 $m=\lfloor\sqrt{x}\rfloor$。类似的，我们可以证明
$$\lceil\sqrt{\lceil x\rceil}\rceil=\lceil\sqrt{x}\rceil,x\geq 0$$
上面的推导并没有严重依赖平方根的性质，我们试着推广这种想法：设 $f(x)$ 是满足如下特性且在一个实数区间连续的函数
$$f(x)=整数 \Rightarrow x 是整数$$
那么只要 $f(x), f(\lfloor x\rfloor), f(\lceil x\rceil)$ 有定义，那么
$$\lfloor f(x)\rfloor=\lfloor f(\lfloor x\rfloor)\rfloor,\lceil f(x)\rceil=\lceil f(\lceil x\rceil)\rceil\tag{3.10}$$
由于之前我们对底函数做了证明，并且对底函数的证明和顶函数没有区别，我们这里对顶函数证明这个命题。如果 $x=\lceil x\rceil$，显然成立，否则 $x$ 不是整数，那么 $x<\lceil x\rceil$，因为 $f$ 递增，那么 $f(x)<f(\lceil x\rceil)$。又因为 $\lceil\rceil$ 非递减，所以 $\lceil f(x)\rceil\leq\lceil f(\lceil x\rceil)\rceil$。如果 $\lceil f(x)\rceil<\lceil f(\lceil x\rceil)\rceil$，由于 $f$ 连续，那么存在一个数 $y$，使得 $x\leq y<\lceil x\rceil,f(y)=\lceil f(x)\rceil$。（我这里花了一个图，更容易理解一点。）由于 $f$ 的性质，这里 $y$ 是一个整数，但是在 $\lfloor x\rfloor,\lceil x\rceil$ 之间不可能存在一个整数。这就说明假设不满足，那么就有 $\lceil f(x)\rceil=\lceil f(\lceil x\rceil)\rceil$。

如果 $m,n$ 是整数，那么
$$\lfloor\frac{x+m}{n}\rfloor=\lfloor\frac{\lfloor x\rfloor+m}{n}\rfloor,\lceil\frac{x+m}{n}\rceil=\lceil\frac{\lceil x\rceil+m}{n}\rceil\tag{3.11}$$
比如令 $m=0$，
