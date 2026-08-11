### Classical Cryptography
最早加密信息的例子之一是 Julius Caesar 将字母后移三位进行字母替换。加密（`encryption`）是使信息保密的过程。

Caesar 的加密方法是将字母映射到数字 0 到 25，然后加 3 模 26。加密函数可以写作
$$f(p)=(p+3)\operatorname{mod}26$$
从加密信息中获取原始信息的过程是解密（`decryption`）。Caesar 解密方法是使用 $f$ 的逆
$$f^{-1}(p)=(p-3)\operatorname{mod}26$$

下面一般化这个方法。引入密钥（`key`）$k$
$$f(p)=(p+k)\operatorname{mod}26$$
$$f^{-1}(p)=(p-k)\operatorname{mod}26$$
这就是移位密码（`shift cipher`），也称为凯撒密码。

可以通过如下公式增加安全性
$$f(p)=(ap+b)\operatorname{mod}26$$
其中 $a,b$ 是整数且他们的选择保证这个函数是双射（等价于 $\gcd(a,26)=1$）。

这称为仿射密码（`affine cipher`），这种变化方式称为仿射变换（`affine transformation`）。

下面解释如何对这种加密方法进行解密。给定一个数字 $p,0\leq p\leq 25$，那么 $c=(ap+b)\operatorname{mod}26,\gcd(a,26)=1$，解密就是要用 $c$ 来表示 $p$。同同余式 $c\equiv (ap+b)\operatorname{mod}26$ 开始，两边同时减去 $b$ 得到 $c-b\equiv ap\operatorname{mod}26$。由于 $\gcd(a,26)=1$，所以 $a$ 模 26 存在逆 $\bar{a}$。那么两边同时乘以 $\bar{a}$ 得到 $\bar{a}(c-b)\equiv \bar{a}ap\equiv p\operatorname{mod}26$，所以 $p\equiv\bar{a}(c-b)(\operatorname{mod}26)$。$p$ 是确定值，因为 $p\in Z_{26}$。

**密码破译（`CRYPTANALYSIS`）** 在不知道加密方法和密钥的情况下恢复原文的过程称为密码破译，这个过程非常困难。这里只介绍移位密码的破解。

英文中各个字母出现的频率不太一样，从高到低前几个字母是 E 13%,
T 9%, A 8%, O 8%, I 7%, N 7%, S 7%, H 6%, R 6%。为了破解密文，可以先统计各个字母出现的频率，然后从 E 开始尝试，认为最高频次的字母就是该字母，那么就可以计算得到移位的 $k$，进而破译密文，检查信息是否有意义。

移位密码和仿射密码称为单字母密码（`monoalphabetic ciphers`），特点是简单，但是容易被破解。我们可以对一组字母做加密，这就是分组密码（`block ciphers`）。

下面介绍一种简单的分组密码（`block cipher`）：转置密码（`transposition cipher`）。确定一个整数 $m$，密钥是 $\{1,2,3,\cdots,m\}$ 的某种排列 $\sigma$，转置是从 $\{1,2,3,\cdots,m\}$ 到排列 $\sigma$ 的函数。首先将消息切分成 $m$ 大小的块，每一块分别加密 $c_1c_2\cdots c_m=p_{\sigma(1)}p_{\sigma(2)}\cdots p_{\sigma(m)}$。解密 $c_1c_2\cdots c_m$ 的话使用 $\sigma^{-1}$。

**定义 1**
> 密码系统（`cryptosystem`）是一个五元组 $(\mathcal{P},\mathcal{C},\mathcal{K},\mathcal{E},\mathcal{D})$，$\mathcal{P}$ 是明文，$\mathcal{C}$ 是密文，$\mathcal{K}$ 是密钥空间（`keyspace`），即所有可能密钥的集合，$\mathcal{E}$ 是加密函数的集合，$\mathcal{D}$ 是解密函数的集合。$E_k$ 是 $\mathcal{E}$ 中对应密钥为 $k$ 的函数，$D_k$ 是 $\mathcal{D}$ 中对应的函数，因此 $D_k(E_k(p))=p$，其中 $p$ 是任意明文。

例 移位密码中我们最终处理的数字，所以 $\mathcal{P},\mathcal{C}$ 是 $Z_{26}$ 元素组成的字符串的集合，$\mathcal{K}$ 是可能的移位值，所以 $\mathcal{K}=Z_{26}$，$\mathcal{E}$ 是由形式如 $E_k(p) = (p + k) \operatorname{mod} 26$ 的函数组成，同样，$\mathcal{D}$ 是由形式如 $D_k(p) = (p - k) \operatorname{mod} 26$ 的函数组成。

### Public Key Cryptography
所有的经典加密算法都是私钥加密（`private key cryptosystem`），一旦知道了加密的密钥，很容易得到解密的密钥。

使用私钥加密的双方共享一个密钥。为了通信安全，这个密钥不能让第三方知道。移位密码和仿射密码容易破解，不过很多私钥加密并不容易被破解，比如 AES。

1970 年代发明了公钥加密（`public key cryptosystem`），只有接受信息的人知道私有密钥，其他人使用公钥加密即可。破解这个就非常困难了。实际应用中，往往利用公钥机密交换私钥加密算法的密钥，后续通信使用私钥加密，原因是公钥加密算法往往比较耗时。

最知名的公钥加密是 RSA 加密算法。不过未来随着它被淘汰（量子计算机的应用），其他公钥加密将会在密码学中扮演重要角色。

### The RSA Cryptosystem
1976 年，MIT 的 Ronald Rivest, Adi Shamir, Leonard Adleman 三个人发明了 RSA 加密系统。Clifford Cocks 在稍早的时候发明了同样的算法，不过由于 QCHQ 的性质，没有公开。（这些信息直到 1990 年代才公开。）

RSA 加密系统中，每个人有一个密钥 $(n,e)$，其中 $n=pq$，$p,q$ 是两个非常大的质数，比如说 300 位这么长，$e$ 是与 $(p-1)(q-1)$ 互斥的指数。生成大的质数是容易的事情，但是将 $n$，一个接近 600 位长度的数，分解成两个质因数就非常困难了。当目前为止，没有很好的办法。

随着计算机速度的提高，$p,q$ 长度的选择也很关键。越长越安全，不过加解密的时间也越长。随着量子计算机的发展，未来可能会很容易进行质因数的分解，RSA 的安全性就不复存在，此时，需要使用量子计算机无法破解的公钥加密算法。

### RSA Encryption
首先，需要将字符串转化成数字，可以使用移位密码的方法，稍有不同的是对于个位数，前面需要补零。接着将这些数字分割成长度为 $2N$ 的块，这里的 $2N$ 是最大不超过 $n$ 数 $2525\cdots 25$ 的长度。

经过这些步骤，我们得到一系列块 $m_1,m_2,\cdots,m_k$，对于某一个块 $m_i$ 使用如下函数得到加密快 $c_i$。
$$c=m^e\operatorname{mod}n$$

下面的例子解释 RSA 是如何加密的，不过为了让例子有意义，使用了很小的 $p,q$。

例 使用密钥 $(2537,13)$ 加密信息 STOP。$2537=43\cdot 59$，所以 $p=43,q=59$，并且
$$\gcd(e,(p-1)(q-1))=\gcd(13,42\cdot 58)=1$$
解：2537 介于 2525 和 252525 之间，所以切分成长度为 4 的块。STOP 转成数字是
$$1819, 1415$$
每一块使用
$$c=m^{13}\operatorname{mod}2537$$
加密
$$c_1=1819^{13}\operatorname{mod}2537=2081$$
$$c_2=1415^{13}\operatorname{mod}2537=2182$$

### RSA Decryption
使用 $e$ 模 $(p-1)(q-1)$ 的逆 $d$ 很容易用密文恢复信息。因为 $de\equiv 1(\operatorname{mod}(p-1)(q-1))$，所以存在一个整数 $k$ 使得 $de=1+k(p-1)(q-1)$，那么
$$c^d \equiv (m^e)^d = (m^d)^e = m^{1+k(p-1)(q-1)} (\operatorname{mod} n)$$
根据费马小定理和（TODO Exercise 28）有 $m^{p-1}\equiv 1(\operatorname{mod}p),m^{q-1}\equiv 1(\operatorname{mod})q$，所以
$$c^d \equiv m \cdot (m^{p-1})^{k(q-1)} \equiv m \cdot 1 = m (\operatorname{mod} p)$$
$$c^d \equiv m \cdot (m^{1-1})^{k(p-1)} \equiv m \cdot 1 = m (\operatorname{mod} q)$$
又因为 $\gcd(p,q)=1$，根据中国剩余定理得到
$$c^d\equiv m(\operatorname{mod}pq)$$

例 使用上个例子的加密密钥解密 $0981, 0461$。

解：TODO  Exercise 2 in Section 4.4 shows,  $13$ 模 $42\cdot 58=2436$ 的逆是 $d=937$，应用
$$m=c^{937}\operatorname{mod}2537$$
进行解密
$$0981^{937}\operatorname{mod} 2537 = 0704$$
$$0461^{937}\operatorname{mod} 2537 = 1115$$
最后翻译成英文是 HELP。

### RSA as a Public Key System
RSA 作为公钥系统核心思想是计算很大的 $p,q,e,d$ 这些值非常容易，但是对 $n$ 做质因数分解很难。至少截止 2017 年，需要数百万年才能破解。不过量子计算机的出现会使得破解 RSA 非常容易。

RSA 往往和私有密钥配合使用，前者用于交换后者的密钥。

### Cryptographic Protocols
Diffie-Hellman 密钥协商协议（`key agreement protocol`）由 Whitfield Diffie 和 Martin Hellman 在 1976 年描述。实际上，Malcolm Williamson 在 1974 年为 GCHQ 工作时就发明了这个协议。

下面是协商一个共同密钥的过程。
1. 甲和乙协商使用某个某个 $p$ 和 $p$ 的原根 $a$
2. 甲选择一个秘密整数 $k_1$，然后将 $a^{k_1}\operatorname{mod}p$ 发送给乙
3. 乙选择一个秘密整数 $k_2$，然后将 $a^{k_2}\operatorname{mod}p$ 发送给甲
4. 甲计算 $(a^{k_2})^{k_1}\operatorname{mod}p$
5. 乙计算 $(a^{k_1})^{k_2}\operatorname{mod}p$

协议结束之后，公共密钥就是
$$(a^{k_1})^{k_2}\operatorname{mod}p=(a^{k_2})^{k_1}\operatorname{mod}p$$

协商密钥，假定前面三步是公开的，即所有人都知道 $p,a,a^{k_1}\operatorname{mod}p,a^{k_2}\operatorname{mod}p$，但是从这些公开信息无法计算得到 $k_1,k_2$，进而密钥 $(a^{k_1})^{k_2}\operatorname{mod}p,(a^{k_2})^{k_1}\operatorname{mod}p$ 是安全的。以现在的算力，$p$ 超过 300 个数字，$k_1,k_2$ 超过 100 个数字就是安全的了。

RSA 的一个重要应用就是数字签名（`digital signature`）。原理很简单，使用 RSA 的私有密钥加密信息，任何人可以用公开密钥解密，如果解密成功，那么说明发送者就是 RSA 公开密钥的发布者。

###  Homomorphic Encryption
随着时代发展，现在很多人将加密信息保存到云端。如果在云端运行一个程序，解密数据，处理，再加密存储，可能会被其他人窃听导致数据被破解。人们提出完全同态加密（`fully homomorphic cryptosystem`）目的是能够在加密数据上做任意运算，使得无需解密即可处理数据。

下面分析 RSA 不是完全同态加密，只能进行部分运算。

例 令 $(n,e)$ 是 RSA 公钥，$M_1,M_2$ 是两个明文数据，满足 $0\leq M_1< n,0\leq M_2<n$ 那么
$$\begin{aligned}
E_{(n,e)}(M_1)E_{(n,e)}(M_2) \operatorname{mod} m &= (M^e_1 \operatorname{mod} m \cdot M^e_2 \operatorname{mod} m) \operatorname{mod} m\\
&= (M_1M_2)^e \operatorname{mod} m\\
&= E_{(n,e)}(M_1M_2)
\end{aligned}$$
所以 RSA 是乘法同态加密（`multiplicatively homomorphic`）。如果使用 RSA 加密，且需要对数据做乘法，那么无需解密可以直接对解密数据进行计算。

但是 $E_{(n,e)}(M_1)+E_{(n,e)}(M_2)= E_{(n,e)}(M_1+M_2)$ 无法对所有 $M_1,M_2$ 成立，比如 $M_1=1$。事实上，我们从 $E_{(n,e)}(M_1),E_{(n,e)}(M_2)$ 得到 $E_{(n,e)}(M_1+M_2)$ 的值，所以 RSA 不是加法同态加密（`additively homomorphic`）。所以我们称 RSA 是部分同态加密（`partially
homomorphic`）。

2009 年，Craig Gentry 发明了完全同态加密，其基于格密码（`lattice-based cryptography`）。不过现在还没有使用的完全同态加密，因为现有的都需要消耗大量的 CPU 和内存。
