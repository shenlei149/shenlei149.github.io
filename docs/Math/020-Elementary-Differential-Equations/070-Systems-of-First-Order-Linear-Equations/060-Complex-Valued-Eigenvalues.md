这一节继续讨论 $n$ 个方程组成的常系数线性齐次方正组
$$\bold{x}'=\bold{A}\bold{x}\tg1$$
其中系数矩阵 $\bold{A}$ 是实数矩阵。如果解的形式是 $\bold{x}=\bold{\xi}e^{rt}$，那么 $r,\bold{\xi}$ 是系数方程 $\bold{A}$ 的特征值和特征向量。$\bold{A}$ 的特征值 $r_1,\cdots,r_n$ 是特征方程
$$\det(\bold{A}-r\bold{I})=\bold{0}\tag{2}$$
的根，相应的非零特征向量满足
$$(\bold{A}-r\bold{I})\bold{\xi}=\bold{0}\tag{3}$$
如果 $\bold{A}$ 的系数是实数，多项式方程 $(2)$ 的系数是实数，任意复数根会共轭出现。如果 $r_1=\lambda+i\mu$ 是 $\bold{A}$ 的特征向量，其中 $\lambda,\mu$ 是实数，那么 $r_2=\lambda-i\mu$ 也是 $\bold{A}$ 的特征向量。在正式讨论之前先看个例子。

例 1 求方程组
$$\bold{x}'=\begin{pmatrix}
-\frac{1}{2}&1\\-1&-\frac{1}{2}
\end{pmatrix}\bold{x}\tag{4}$$
的实数基础解系。给出相图和一些解的曲线。

解：下图是方程组 $(4)$ 的方向场。解的轨迹顺时针向原点旋转。

![](060.010.png)

假定解的形式是
$$\bold{x}=\bold{\xi}e^{rt}\tag{5}$$
那么得到线性方程组
$$\begin{pmatrix}
-\frac{1}{2}-r&1\\-1&-\frac{1}{2}-r
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{6}$$
特征方程是
$$\begin{vmatrix}
-\frac{1}{2}-r&1\\-1&-\frac{1}{2}-r
\end{vmatrix}=r^2+r+\frac{5}{4}=0\tag{7}$$
因此特征值是 $r_1=-\frac{1}{2}+i,r_2=0\frac{1}{2}-i$，代入 $(6)$ 得到对应的特征向量是
$$\bold{\xi}^{(1)}=\begin{pmatrix}
1\\i
\end{pmatrix},\bold{\xi}^{(2)}=\begin{pmatrix}
1\\-i
\end{pmatrix}\tag{8}$$
从上面可以看出特征向量 $\bold{\xi}^{(1)},\bold{\xi}^{(2)}$ 也互为复共轭。因此方程组 $(4)$ 的基础解析是
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
1\\i
\end{pmatrix}e^{(-1/2+i)t},\bold{x}^{(2)}(t)=\begin{pmatrix}
1\\-i
\end{pmatrix}e^{(-1/2-i)t}\tag{9}$$
为了获得实数解，可以取 $\bold{x}^{(1)}$ 或 $\bold{x}^{(2)}$ 的实部和虚部。那么
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
1\\i
\end{pmatrix}e^{-t/2}(\cos t+i\sin t)=\begin{pmatrix}
e^{-t/2}\cos t\\-e^{-t/2}\sin t
\end{pmatrix}+i\begin{pmatrix}
e^{-t/2}\sin t\\e^{-t/2}\cos t
\end{pmatrix}\tag{10}$$
因此方程组 $(4)$ 的实数解是
$$\bold{u}(t)=e^{-t/2}\begin{pmatrix}
\cos t\\-\sin t
\end{pmatrix},\bold{v}=e^{-t/2}\begin{pmatrix}
\sin t\\\cos t
\end{pmatrix}\tag{11}$$
它们的朗斯基是
$$W[\bold{u},\bold{v}]=\begin{vmatrix}
e^{-t/2}\cos t&e^{-t/2}\sin t\\
-e^{-t/2}\sin t&e^{-t/2}\cos t
\end{vmatrix}=e^{-t}$$
不会为零，因为 $\bold{u},\bold{v}$ 是线性无关的，是方程组 $(4)$ 的基础解系。

解 $\bold{u},\bold{v}$ 的图分别在下图中黑色实线和虚线表示。

![](060.021.png)

因为
$$\bold{u}(0)=\begin{pmatrix}
1\\0
\end{pmatrix},\bold{v}(0)=\begin{pmatrix}
0\\1
\end{pmatrix}$$
因此 $\bold{u},\bold{v}$ 分别会通过 $(1,0),(0,1)$。方程组 $(4)$ 的其他解是这两个解的线性组合，其中一些如上图所示。当 $t\to\infty$ 时，每个解都沿着螺旋线趋于原点，并绕着原点旋转无数次，这是因为解 $(11)$ 是一个衰减的指数和正弦、余弦函数的乘积。下图是 $x_1$ 随 $t$ 变化的图像如下（b）所示，每一条曲线代表一种随时间衰减的振荡。$x_2$ 随 $t$ 变化的图像如（c）所示。

![](060.022.png)

上面的（a）图是一个典型的特征值是共轭复数且实部为负数的 $2\times 2$ 的方程组 $\bold{x}'=\bold{A}\bold{x}$ 的解的图像。原点称为螺旋点（`spiral point`），随着 $t$ 的增加，所有轨迹都趋于原点，因此是渐进稳定的。如果复数的实部是正数，图像也和（a）类似，不过轨迹方向是远离原点的，因此轨迹会变得无穷大，此时原点是不稳定的。

如果实部为零，那么既不会趋于原点也不会远离原点，轨迹是周而复始的绕着原点的曲线。后续会有这样的例子。此时原点称为中心（`center`），也被称为是稳定的，但不是渐进稳定的。这三种情况下，运动方向可能是顺时针，也可能是逆时针，这依赖于系数矩阵 $\bold{A}$。

上面的图是计算机辅助作图，但是人工可以给出草图。假设特征值是 $\lambda\pm i\mu$，如果 $\lambda<0$，向内旋转，$\lambda>0$，向外旋转，如果 $\lambda=0$，那么是闭合曲线。为了确定是顺时针还是逆时针，只需要确定一个点的方向即可。比如 $(4)$ 中我们选择 $\bold{x}=(0,1)^T$，那么 $\bold{A}\bold{x}=(1,-\frac{1}{2})^T$。因此该点处切线方向 $\bold{x}'$ 确定了，$x_1$ 方向上是正的，因此轨迹从第二象限到第一象限，那么是顺时针运动。
