###  Hashing Functions
我们经常使用
$$h(k)=k\operatorname{mod}m$$
作为哈希函数。简答，计算快，每一个可能性都会覆盖到。

如果多个 $k$ 计算的 $h(k)$ 相同，称为碰撞（`collision`）。解决冲突有很多方式，最简单的线性扫描法。$i$ 从 0 开始到 $m-1$，按照 $h(k,i) = h(k) + i \operatorname{mod} m$ 找下一个空位。

### Pseudorandom Numbers
系统方法产生的随机数不是真随机，所以成为伪随机数（`pseudorandom numbers`）。

最常用的伪随机数生成方法是线性同余法（`linear congruential method`）。我们选择四个数：模 $m$，倍数 $a$，增量 $c$ 和种子 $x_0$，生成随机数公式是
$$x_{n+1}=(ax_n+c)\operatorname{mod}m$$

有的时候，我们使用 $c=0$ 的线性同余法，此时，我们称之为纯乘法生成器（`pure multiplicative generator`）。

线性同余法被用于很多任务，不过它生成的随机数不满足某些随机数统计属性。因此，在一些对随机数要求比较高的任务中，需要使用其他随机数生成方法。

### Check Digits
例 奇偶校验比特（`Parity Check Bit`） 任意一串数据有 $n$ 个比特，我们会增加第 $n+1$ 位使得
$$x_{n+1}=(x_1+x_2+\cdots+x_n)\operatorname{mod}2$$
如果检验位错了，那么数据不正确。不过即使检验位是对的，也不代表数据是正确的，比如有偶数个比特发生了反转。

例 通用产品代码（`Universal Product Code`, `UPC`） 一般有 12 位，第一位是产品类别，接下来五位是生产商，再接下来五位指定产品，最后一位是检验位。这些数字满足
$$3x_1 + x_2 + 3x_3 + x_4 + 3x_5 + x_6 + 3x_7 + x_8 + 3x_9 + x_{10} + 3x_{11} + x_{12} ≡ 0 (\operatorname{mod} 10)$$

例 国际标准书号（`International Standard Book Number`, `ISBN`） 一般有 ISBN-10 和 ISBN-13 两种。下面分析 ISBN-10。十个数字，最后一个可能是 10，用 X 表示，这些数字满足
$$\sum_{i=1}^{10}ix_i\equiv 0(\operatorname{mod}11)$$

常见的错误有单个数字错误（`single error`）和置换错误（`transposition error`），两个不同数字交换了位置。任意一种校验码都希望能校验出这些错误，甚至是其他类型的错误。下面考察 ISBN 和 UPC 的能力。

假定新的 ISBN 是 $y_1y_2\cdots y_{10}$，假定第 $j$ 位出粗了，那么 $y_i=x_i,i\neq j$ 且 $y_j=x_j+a$，其中 $-10\leq a\leq 10,a\neq 0$，那么
$$\sum_{i=0}^{10}iy_i=\sum_{i=0}^{10}ix_i+ja\equiv ja\not\equiv 0(\operatorname{mod}11)$$
因为 $11\not |j,11\not |a$，所以 $11\not |ja$。因此 $y_1y_2\cdots y_{10}$ 不是合法的 ISBN。

现在来看置换错误，假定第 $j,k$ 位置交换，那么 $y_j=x_k,y_k=x_j$，其余有 $y_i=x_i,i\neq j,i\neq k$，那么
$$\sum_{i=0}^{10}iy_i=\sum_{i=0}^{10}ix_i+j(x_k-x_j)+k(x_j-x_k)\equiv (j-k)(x_k-x_j)\not\equiv 0(\operatorname{mod}11)$$
原因类似，$11\not |(j-k),11\not |(x_k-x_j)$，所以 $y_1y_2\cdots y_{10}$ 不是合法的 ISBN。

对于 UPC 而言，单个数字的错误与原来 UPC 的差值是 $a$ 或者 $3a$，因为 $a\not\equiv 0(\operatorname{mod}10),3a\not\equiv 0(\operatorname{mod}10)$，所以不是合法的 UPC。

对于交换错误，如果交换都在奇数位置或者都在偶数位置的两个值，前面的系数都是一样的，和不变，所以同余性质不变，无法检测。如果交换奇数和偶数位置的值，比如 $x$ 变成了 $y$，那么和的差值是 $2(x-y)$，如果差值是 5，那么能够整除 10，无法检测错误。
