[Problem 700](https://projecteuler.net/problem=700 "Problem 700 - Project Euler")

先瞎扯一句，欧拉项目网站的第700题（整百）在纪念欧拉，看得出来，出题人很有情怀。500 题也类似。

欧拉（`Leonhard Euler`）出生于 1707 年 4 月 15 日。

考虑序列 $1504170715041707n \operatorname{mod} 4503599627370517$。

在这个序列的元素，如果小于前一个 Eulercoin，那么它就是Eulercoin。

显然第一个 Eulercoin 是 1504170715041707，也是序列的第一个数；该序列的第二个数 3008341430083414 比上一个 Eulercoin 大，它不是 Eulercoin，第三项 8912517754604 比第一项小，所以是新的Eulercoin。

求所有 Eulercoin 之和。

要计算所有的 Eulercoin 之和，那么该序列最后一个值是 0 就算结束，其实得到 1 的时候也可以结束了，因为下一个一定是 0，对和没有影响。

如果知道连续两个 Eulercoin，如何计算下一个呢？假设这三个 Eulercoin 分别是 $s,f,n$，用 $Mod$ 表示 4503599627370517，$E$ 来表示 1504170715041707，那么他们满足下面三个等式
$$s_1E=s_2Mod+s$$
$$f_1E=f_2Mod+f$$
$$n_1E=n_2Mod+n$$
第三个等式可以由第一个等式和第二个等式线性组合得到。

$f<s$，同时 $s_1,f_1$ 必须是正整数，那么 $s_1<f_1$，为了保证 $0<n<f$，那么可能的线性组合是
$$mf-s=n$$
所以
$$n=f-s\operatorname{mod} f$$
