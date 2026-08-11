### Division
**定义 1**
如果 $a, b$ 是整数且 $a\neq 0$，且存在一个整数 $c$ 使得 $b=ac$，那么我们称 $a$ 能整除 $b$，记作 $a|b$。此时，我们称 $a$ 是 $b$ 的因子、约数、除数，$b$ 是 $a$ 的倍数。如果 $a$ 不能整除 $b$，那么记作 $a\not |b$。

**定理 1**
令 $a, b, c$ 是整数且 $a\neq 0$，那么
1. 如果 $a|b, a|c$，那么 $a|(b+c)$
2. 如果 $a|b$，那么对于任意整数 $c$ 都有 $a|bc$
3. 如果 $a|b, b|c$，那么 $a|c$

证明：
1. 根据定义，存在 $s,t$ 使得 $b=sa,c=ta$，因此
$$b+c=sa+ta=(s+t)a$$
所以 $a$ 能整除 $b+c$。

2. 根据定义，存在 $s$ 使得 $b=sa$，因此
$$bc=sac=(sc)a$$

3. 根据定义，存在 $s,t$ 使得 $b=sa,c=tb$，因此
$$c=tb=tsa=(ts)a$$

**推论 1**
如果 $a, b, c$ 是整数且 $a\neq 0$，如果 $a|b,a|c$，那么对于任意整数 $m,n$ 都有 $a|mb+nc$。

证明：根据定理 1 的第二条，有 $a|mb,a|nc$。再应用定理 1 的第一条得到 $a|mb+nc$。

### The Division Algorithm
**定理 2**
令 $a$ 是整数且 $d$ 是正整数，那么存在唯一的整数 $q,r$，其中 $0\leq r<d$，使得 $a=qd+r$。

TODO Section 5.2 证明。

**定义 2**
根据定理 2，$d$ 被称为除数（`divisor`），$a$ 是被除数（`dividend`），$q$ 是商（`quotient`），$r$ 是余数（`remainder`）。商和余数表示为
$$q=a\operatorname{div}d$$
$$r=a\operatorname{mod}d$$

###  Modular Arithmetic
**定义 3**
如果 $a,b$ 是整数，$m$ 是正整数，且 $m$ 能整除 $a-b$，那么我们称 $a,b$ 模 $m$ 同余。

**定理 3**
令 $a,b$ 是整数且 $d$ 是正整数。$a\equiv b(\operatorname{mod}m)$ 等价于 $a\operatorname{mod}m=b\operatorname{mod}m$。

证明：  
（1）令 $a\operatorname{mod}m=b\operatorname{mod}m=r$，所以存在两个整数 $s,t$ 有
$$a=sm+r$$
$$b=tm+r$$
两式相减有
$$a-b=(s-t)m$$
所以 $m$ 能整除 $a-b$。根据定义有 $a\equiv b(\operatorname{mod}m)$。  
（2）如果 $a\equiv b(\operatorname{mod}m)$，根据定义有 $m|a-b$，那么
$$a-b=qm$$
不妨设 $a\operatorname{mod}m=r$，所以
$$a=sm+r$$
代入上式得到
$$sm+r-b=qm$$
整理得到
$$b=(s-q)m+r$$
所以 $b\operatorname{mod}m=r$。

**定理 4**
令 $m$ 是正整数。整数 $a,b$ 模 $m$ 同余等价于存在一个整数 $k$ 使得 $a=b+km$。

证明：如果 $a\equiv b(\operatorname{mod}m)$，根据定义有 $m|(a-b)$，这意味着存在一个整数 $k$ 使得 a-b=km，所以 $a=b+km$。上述证明的每一步反过来即证明定理的另一面。

所有整数中与 $a$ 模 $m$ 同余的数构成了同余类（`congruence class`）。第九章将证明模 $m$ 可以将所有整数划分为 $m$ 个不想交同余类，且他们的并集是全体整数。

**定理 5**
令 $m$ 是正整数。如果 $a\equiv b(\operatorname{mod}m),c\equiv d(\operatorname{mod}m)$，那么
$$a+c\equiv b+d(\operatorname{mod}m),ac\equiv db(\operatorname{mod}m)$$

证明：根据定理 4 存在两个整数 $s,t$ 使得
$$b=a+sm,d=c+tm$$
所以
$$b+d=a+sm+c+tm=(a+c)+(s+t)m$$
$$bd=(a+sm)(c+tm)=ac+(at+sc+stm)m$$
进而得到结论。

在处理同余问题时必须非常小心。比如如果 $ac\equiv bc(\operatorname{mod}m)$，但是 $a\equiv b(\operatorname{mod}m)$ 不一定为真。类似的，如果 $a\equiv b(\operatorname{mod}m),c\equiv d(\operatorname{mod}m)$，$a^c\equiv b^d(\operatorname{mod}m)$ 可能为假。很容易找到反例，比如对于第一个例子，$a=2,b=10,c=3,m=6$ 就是反例；对于第二个而言，$a=c=2,b=d=5,m=3$ 是反例之一。

**推论 2**
令 $m$ 是正整数，$a,b$ 是整数，那么有
$$(a+b)(\operatorname{mod}m)=((a\operatorname{mod}m)+(b\operatorname{mod}m))(\operatorname{mod}m)$$
$$(ab)(\operatorname{mod}m)=((a\operatorname{mod}m)(b\operatorname{mod}m))(\operatorname{mod}m)$$

证明：根据模的定义和同余的定义，我们有
$$a\equiv (a\operatorname{mod}m)(\operatorname{mod}m)$$
$$b\equiv (b\operatorname{mod}m)(\operatorname{mod}m)$$
然后直接应用定理 5 就可以得到推论了。

### Arithmetic Modulo $m$
由小于 $m$ 的整数构成了集合 $Z_m$，在其上我们定义算术操作 $+_m,\cdot_m$
$$a+_mb=(a+b)\operatorname{mod}m$$
$$a\cdot_mb=(a\cdot b)\operatorname{mod}m$$
这两个操作称为模 $m$ 的加法乘法。满足如下性质：
* 封闭性：如果 $a,b$ 属于 $Z_m$，那么 $a+_mb,a\cdot_mb$ 也属于 $Z_m$；
* 结合率：如果 $a,b,c$ 属于 $Z_m$，那么 $(a+_mb)+_mc=a+_m(b+_mc),(a\cdot_mb)\cdot_mc=a\cdot_m(b\cdot_mc)$；
* 交换律：如果 $a,b$ 属于 $Z_m$，那么 $a+_mb=b+_ma,a\cdot_mb=b\cdot_ma$；
* 单位元：0 和 1 分别是加法和乘法的单位元。如果 $a$ 属于 $Z_m$，那么 $a+_m0=0+_ma=a,a\cdot_m1=1\cdot_ma=a$；
* 加法的逆运算：如果 $a\neq 0$ 属于 $Z_m$，那么 $m-a$ 是 $a$ 的模 $m$ 加法的逆元，0 的逆元是其本身，即 $a+_m(m-a)=0,0+_m0=0$
* 分配率：如果 $a,b,c$ 属于 $Z_m$，那么 $a\cdot_m(b+_mc)=a\cdot_mb+_ma\cdot_mc,(a+_mb)\cdot_mc=a\cdot_mc+_mb\cdot_mc$

TODO 证明 exercises 48-50

注意，乘法没有逆运算。比如 2 模 6，就是一个反例，如果有类似定义的话，其逆元是 3，但是 $2\cdot_m3=6\operatorname{mod}6=0$，而乘法的单位元是 1 而不是 0。

由于满足上述属性，$Z_m$ 及定义其上的加法是交换群（`commutative group`），$Z_m$ 及定义其上的乘法是交换环（`commutative ring`）。

