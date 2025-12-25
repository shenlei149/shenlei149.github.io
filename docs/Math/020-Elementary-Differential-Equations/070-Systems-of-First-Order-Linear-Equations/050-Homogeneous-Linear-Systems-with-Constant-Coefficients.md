本节主要讨论常系数齐次线性方程组，形式是
$$\bold{x}'=\bold{A}\bold{x}\tag{1}$$
其中 $\bold{A}$ 是 $n\times n$ 的矩阵。除非额外声明，$\bold{A}$ 的元素都是实数。

如果 $n=1$，那么方程组简化为单个一阶方程
$$\frac{dx}{dt}=ax\tag{2}$$
解是 $x(t)=ce^{at}$，当 $a\neq 0$ 时，$x=0$ 是唯一的临界点。如果 $a<0$，当 $t$ 增大时非平凡解（$x(t)=0$ 之外的解）都趋于 $x(t)=0$，此时 $x(t)=0$ 是渐进稳定平衡解。如果 $a>0$，当 $t$ 增长时，除了平衡解之外的每一个解都远离平衡解，因此这种情况下 $x(t)=0$ 是不稳定解。

对于 $n$ 个方程的方程组，情况类似但是更复杂。通过求解 $\bold{A}\bold{x}=\bold{0}$ 可以得到平衡解。通常假定 $\det\bold{A}\neq 0$，因此 $\bold{x}=\bold{0}$ 是唯一的平衡解。那么问题来了，当 $t$ 变大时，其他解是趋于还是远离平衡解？也就是说 $\bold{x}=\bold{0}$ 是渐进稳定的还是不稳定的？还是有其他情况？

$n=2$ 这种情况非常重要，可以在 $x_1x_2$ 平面上可视化，这个平面称为相平面（`phase plane`）。通过在大量点处计算 $\bold{A}\bold{x}$ 并且画出结果矢量，可以得到微分方程组的切向量的方向场。通过方向场可以定性理解解的行为。更多信息需要画出解的曲线，即轨迹。展示出有代表性的轨迹的图称为相图（`phase portrait`）。一个很好的相图可以给出易于理解的解的信息。这项工作往往由计算机辅助完成。

下面先看一个简单的示例。

例 1 求方程组
$$\bold{x}'=\begin{pmatrix}
2&0\\0&-3
\end{pmatrix}\bold{x}\tag{3}$$
的通解。

解：这个方程组除了对角线以外的系数都是零，因此称为对角矩阵（`diagonal matrix`）。方程组写成标量形式是
$$x_1'=2x_1,x_2'=-3x_2$$
每一个方程只涉及一个未知变量，因此可以分开求解这两个方程。因此得到解
$$x_1=c_1e^{2t},x_2=c_2e^{-3t}$$
其中 $c_1,c_2$ 是任意常数。写成向量形式是
$$\bold{x}=\begin{pmatrix}
c_1e^{2t}\\c_2e^{-3t}
\end{pmatrix}=c_1\begin{pmatrix}
e^{2t}\\0
\end{pmatrix}+c_2\begin{pmatrix}
0\\e^{-3t}
\end{pmatrix}=c_2\begin{pmatrix}
1\\0
\end{pmatrix}e^{2t}+c_2\begin{pmatrix}
0\\1
\end{pmatrix}e^{-3t}\tag{4}$$
将这两个解定义为 $\bold{x}^{(1)},\bold{x}^{(2)}$，那么
$$\bold{x}^{(1)}=\begin{pmatrix}
1\\0
\end{pmatrix}e^{2t},\bold{x}^{(2)}=\begin{pmatrix}
0\\1
\end{pmatrix}e^{-3t}\tag{5}$$
朗斯基行列式是
$$W[\bold{x}^{(1)},\bold{x}^{(2)}]=\begin{vmatrix}
e^{2t}&0\\0&e^{-3t}
\end{vmatrix}=e^{-t}\tag{6}$$
朗斯基总是不为零。因此 $\bold{x}^{(1)},\bold{x}^{(2)}$ 是基础解系，方程组 $(3)$ 的通解是 $(4)$。

例 1 中，我们找到了两个线性无关的解，形式是一个向量乘以一个指数函数。这是意料之中的事情，因为未知数 $\bold{x}$ 是一个矢量，并且我们之前讨论过常系数线性方程有指数解。现在泛化这个想法，那么解的形式是
$$\bold{x}=\bold{\xi}e^{rt}\tag{7}$$
其中需要确定的是指数 $r$ 和矢量 $\bold{\xi}$。将 $(7)$ 代入 $(1)$ 得到
$$r\bold{\xi}e^{rt}=\bold{A}\bold{\xi}e^{rt}$$
两边同时除以非零因子 $e^{rt}$ 得到 $\bold{A}\bold{\xi}=r\bold{\xi}$ 或者是
$$(\bold{A}-r\bold{I})\bold{\xi}=\bold{0}\tag{8}$$
其中 $\bold{I}$ 是 $n\times n$ 的单位矩阵。因此，为了求解微分方程组 $(1)$，必须求解代数方程 $(8)$。后者就是求解 $\bold{A}$ 的特征值和特征向量。因此如果 $r$ 是 $\bold{A}$ 的特征值 $\bold{\xi}$ 是对应的特征向量，那么 $(7)$ 是 $(1)$ 的解。

下面是两个典型的 $2\times 2$ 的方程组的例子，特征值是实数且不相等。在每个例子中，会求解方程组、构造相应的相图。由于特征值的符号相同或者不同，解的几何模式相当不同。稍后会回到 $n\times n$ 的方程组。

例 2 有如下方程组
$$\bold{x}'=\begin{pmatrix}
1&1\\4&1
\end{pmatrix}\bold{x}\tag{9}$$
画出方向场，给出解的定性行为。然后求通解，画出相位图和一些轨迹。

解：下图是 21 乘 21 共计 441 个箭头组成的方向场，其中 $-2.5\leq x_1\leq 2.5,-2.5\leq x_2\leq 2.5$，步长均为 0.25。每一点 $(x_1,x_2)$ 处的箭头方向是
$$\begin{pmatrix}
1&1\\4&1
\end{pmatrix}\begin{pmatrix}
x_1\\x_2
\end{pmatrix}$$
比如 $(1,0)$ 处的方向矢量是 $(1,4)^T$，$(-1,-1)$ 处的方向矢量是 $(-2,-5)^T$。这两个矢量使用绿色标出。所有的箭头同样的长度，并且足够短避免相近的箭头交叉。

TODO ADD figure

解的轨迹是沿着方向场的箭头方向的。第二象限的解最终会移动到第一或者第三象限，这和第四象限的解类似。第一象限和第三象限的解仍旧在当前象限内。原点附近的解会远离原点，最终的斜率近似是 2。

假定解是 $\bold{x}=\bold{\xi}e^{rt}$，代入 $(9)$ 得到代数方程组
$$\begin{pmatrix}
1-r&1\\4&1-r
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{10}$$
$(10)$ 有非平凡解等价于系数行列式为零。因此 $r$ 是方程
$$\begin{vmatrix}
1-r&1\\4&1-r
\end{vmatrix}=(1-r)^2-4=(r-3)(r+1)=0\tag{11}$$
的根。那么 $r_1=3,r_2=-1$，这是 $(9)$ 的系数矩阵的特征值。

当 $r=3$ 时，$(10)$ 的两个方程减少为一个
$$-2\xi_1+\xi_2=0\tag{12}$$
因此 $\xi_2=2\xi_1$。那么对应 $r_1=3$ 的特征向量是
$$\bold{\xi}^{(1)}=\begin{pmatrix}
1\\2
\end{pmatrix}\tag{13}$$
类似的，代入 $r_2=-1$，得到 $\xi_2=-2\xi_1$，因此特征向量是
$$\bold{\xi}^{(2)}=\begin{pmatrix}
1\\-2
\end{pmatrix}\tag{14}$$
微分方程的解是
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t},\bold{x}^{(2)}(t)=\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}\tag{15}$$
这两个解的朗斯基
$$W[\bold{x}^{(1)},\bold{x}^{(2)}]=\begin{vmatrix}
e^{3t}&e^{-t}\\
2e^{3t}&-2e^{-t}\\
\end{vmatrix}=-4e^{2t}\tag{16}$$
不会为零。因此 $\bold{x}^{(1)},\bold{x}^{(2)}$ 是基础解系，那么方程 $(9)$ 的通解是
$$\begin{aligned}
\bold{x}&=c_1\bold{x}^{(1)}(t)+c_2\bold{x}^{(2)}(t)\\
&=c_1\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t}+c_2\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}
\end{aligned}\tag{17}$$
其中 $c_1,c_2$ 是任意常量。

下面对解 $(17)$ 进行可视化，考虑不同常量 $c_1,c_2$ 并在 $x_1x_2$ 平面上画出对应图像。首先从 $\bold{x}=c_1\bold{x}^{(1)}(t)$ 开始，其标量形式是
$$x_1=c_1e^{3t},x_2=c_1e^{3t}$$
消除变量 $t$ 可以得到 $x_2=2x_1$。如下图（a）所示。这条线通过原点，方向是特征向量 $\bold{\xi}^{(1)}$ 的方向。当 $c_1>0$ 时，沿着轨迹移动，解在第一象限，反之 $c_1<0$，解在第三象限。两种情况下，随着 $t$ 增加，都远离原点。

接下来讨论 $\bold{x}=c_2\bold{x}^{(2)}$ 的情况
$$x_1=c_2e^{-t},x_2=-2c_2e^{-t}$$
解是直线 $x_2=-2x_1$，方向由特征向量 $\bold{\xi}^{(2)}$ 决定。当 $c_2>0$ 时解位于第四象限，$c_2<0$ 时解在第二象限。两种情况下，随着 $t$ 的增加，都趋于原点。

TODO add （a）

通解 $(17)$ 是 $\bold{x}^{(1)},\bold{x}^{(2)}$ 的线性组合。当 $t$ 很大时，$c_1\bold{x}^{(1)}$ 占主导，$c_2\bold{x}^{(2)}$ 可以忽略不计。因此当 $c_1\neq 0$ 时，随着 $t\to\infty$，解趋于渐近线 $x_2=2x_1$。类似的，当 $c_2\neq 0$ 时，随着 $t\to -\infty$，解趋于渐近线 $x_2=-2x_1$。基础解析 $\bold{x}^{(1)},\bold{x}^{(2)}$ 分别使用了黑色虚线和实线表示。上图的轨迹模式是典型的 $2\times 2$ 具有符号相反的特征值是实数的方程组 $\bold{x}'=\bold{A}\bold{x}$ 的解的行为。原点被称为鞍点（`saddle point`）。随着 $t$ 的增加，原有的轨迹都远离鞍点，因此是不稳定的。

上图中 $x_1,x_2$ 都是 $t$ 的函数，也可以分别画出它们关于 $t$ 的行为。如下图（b）（c）所示。对于某些初始条件，使得 $c_1=0$，那么 $x_1=c_2e^{-t}$，因此 $t\to\infty$ 时 $x_1\to 0$。下图（b）画出了该图像。对应于（a）中趋于原点的轨迹。不过对于大多数初始条件，$c1\neq 0$，此时 $x_1$​ 由 $x_1=c_1e^{3t}+c_2e^{-t}x1$ 给出。正指数项的存在使得随 $t$ 的增加 $x_1$​ 的绝对值呈指数增长。（b）中展示了几个这类图像，它们对应于（a）中远离原点的轨线。理解图（a）的方向场与（b）（c）部分的分量图之间的关系至关重要，因为这样可以在在 $x_1x_2$​ 平面上对解进行可视化，也可以将其视为自变量 $t$ 的函数进行可视化。

TODO add （b）（c）

例 3 有如下方程组
$$\bold{x}'=\begin{pmatrix}
-3&\sqrt{2}\\\sqrt{2}&-2
\end{pmatrix}\bold{x}\tag{18}$$
画出方向场，给出解的定性行为。然后求通解，画出相位图和一些轨迹。

解：下图是原点附近的方向场。

TODO ADD figure

假定解是 $\bold{x}=\bold{\xi}e^{rt}$，代入 $(18)$ 得到代数方程组
$$\begin{pmatrix}
-3-r&\sqrt{2}\\\sqrt{2}&-2-r
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{19}$$
特征值 $r$ 是方程
$$(-3-r)(-2-r)-2=(r+1)(r+4)=0\tag{20}$$
的根。那么 $r_1=-1,r_2=-4$。

当 $r=-1$ 时，$(19)$ 是
$$\begin{pmatrix}
-2&\sqrt{2}\\\sqrt{2}&-1
\end{pmatrix}\begin{pmatrix}
\xi_1\\\xi_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\tag{ 21}$$
因此 $\xi_2=\sqrt{2}\xi_1$。那么对应 $r_1=-1$ 的特征向量是
$$\bold{\xi}^{(1)}=\begin{pmatrix}
1\\\sqrt{2}
\end{pmatrix}\tag{22}$$
类似的，代入 $r_2=-4$，得到 $\xi_1=-\sqrt{2}\xi_2$，因此特征向量是
$$\bold{\xi}^{(2)}=\begin{pmatrix}
-\sqrt{2}\\1
\end{pmatrix}\tag{23}$$
微分方程的解是
$$\bold{x}^{(1)}(t)=\begin{pmatrix}
1\\\sqrt{2}
\end{pmatrix}e^{-t},\bold{x}^{(2)}(t)=\begin{pmatrix}
-\sqrt{2}\\1
\end{pmatrix}e^{-4t}\tag{24}$$
那么通解是
$$\begin{aligned}
\bold{x}&=c_1\bold{x}^{(1)}(t)+c_2\bold{x}^{(2)}(t)\\
&=c_1\begin{pmatrix}
1\\2
\end{pmatrix}e^{3t}+c_2\begin{pmatrix}
1\\-2
\end{pmatrix}e^{-t}
\end{aligned}\tag{ 25}$$

方程组 $(18)$ 相图如下所示。$\bold{x}^{(1)}$ 的解是虚黑线表示，即直线 $x_2=\sqrt{2}x_1$，$\bold{x}^{(2)}$ 的解是实黑线表示，即直线 $x_1=-\sqrt{2}x_2$。这些直线的斜率由向量 $\bold{\xi}^{(1)},\bold{\xi}^{(2)}$ 确定。一般情况下是这基础解系的线性组合。当 $t\to\infty$ 时，$e^{-4t}$ 比 $e^{-t}$ 小的多，相比 $\bold{x}^{(1)}$ 而言 $\bold{x}^{(2)}$ 可以忽略。因此，除非 $c_1=0$，否则解 $(25)$ 趋于 $x_2=\sqrt{2}x_1$。

TODO add （a）

上图的轨迹模式是典型的 $2\times 2$ 具有符号相同的特征值是实数的方程组 $\bold{x}'=\bold{A}\bold{x}$ 的解的行为。原点被称为节点（`node`）。 如果特征值是正的而不是负的，轨迹类似，不过方向朝外。如果特征值是负的，渐进是稳定的，特征是正的则是不稳定的。

一些典型的 $x_1-t$ 的曲线如下图（b）所示，相应的 $x_2-t$ 的曲线如（c）所示。随着 $t$ 的增加，两个图中的曲线都趋于 $t$ 轴，在上面（a）图中这些对应的解都趋于原点。

TODO add （b）（c）
