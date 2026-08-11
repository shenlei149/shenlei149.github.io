算术运算的问题主要在于只能有限的表示真实世界中无限的数。

*Fallacy: Just as a left shift instruction can replace an integer multiply by a power of 2, a right shift is the same as an integer division by a power of 2.*

*谬论：和左移整数是乘以 2 一样，右移整数是除以 2。*

这个结论对于无符号数是成立的，对于有符号数而言就不正确了。比如 -5 除以 4，结果是 -1。-5 的二进制表示如下
```
11111111 11111111 11111111 11111011
```
右移两位得到
```
00111111 11111111 11111111 11111110
```
这是一个很大的数，不是 -1。一个解决办法是使用算术右移，前面补符号位。结果是
```
11111111 11111111 11111111 11111110
```
十进制是 -2，接近了，但仍旧不是 -1。

*Pitfall: Floating-point addition is not associative.*

*陷阱：浮点数加法不满足结合律。*

整数加法满足结合律，但是浮点数有精度的限制只能近似表示。当两个非常大的数与一个很小的数相加就会出现问题。比如 $c=-1.5\times 10^{38},a=1.5\times 10^{38},b=1.0$，那么
$$\begin{aligned}
c+(a+b)&=-1.5\times 10^{38}+(1.5\times 10^{38}+1.0)\\
&=-1.5\times 10^{38}+1.5\times 10^{38}\\
&=0.0\\
(c+a)+b&=(-1.5\times 10^{38}+1.5\times 10^{38})+1.0\\
&=0.0+1.0\\
&=1.0
\end{aligned}$$
出现问题的原因在于 $a$ 比 $b$ 大太多，由于精度表示有限，$a+b$ 和 $a$ 一样。因此 $c + (a + b) \neq (c + a) + b$，不满足结合律。

*Fallacy: Parallel execution strategies that work for integer data types also work for floating-point data types.*

*谬误：并行对于整数有效，对于浮点数也有效。*

程序从串行修改成并行，如果两者的答案不同，那么需要跟踪分析原因。

如果对一百万个数相加，不管是使用一个处理器或者使用一千个处理器，结果应该是一样的。二进制补码表示的整数满足结合律，那么结果是一致的，但是浮点数不满足结合律，那么结果就不一定一样了。

现代分布式系统，使用多少资源往往是动态的，那么同样的程序、同样的输入，由于资源多少不同，导致浮点数计算顺序不同，结果往往也有些许差异。

因此，对于浮点数运算，即使答案与顺序执行不同，也要确认结果是可信的。这就涉及数值计算这个领域了。

*Fallacy: Only theoretical mathematicians care about floating-point accuracy.*

*谬误：只有理论数学家才关心浮点数精度。*

1994，Intel 奔腾（`Pentium`）处理器一次性猜测两位商值，不过由于计算错误会导致有效数字的前 11 位是正确的，后面的比特位可能出错。

数学教授 Thomas Nicely 发现了这个问题并反馈给 Intel，没有的得到重视，于是乎 Thomas Nicely 公开了他的发现。Intel 生成这个小问题只会影响理论数学，对于诸如电子表格这种普通应用，27,000 年才会出现错误。IBM 很快打脸，结论是 24 天就会遇到错误。

Intel 不得不承认错误并修复，召回有问题的 CPU，损失 5 亿美元！
