三角函数的积分涉及六个基本三角函数的代数运算。一般地，我们通过恒等变形，将被积分的式子转化成更容易积分的式子。

### 正弦和余弦的幂的积分
我们从形如
$$\int\sin^m x\cos^n xdx$$
的积分开始，其中 $m,n$ 是非负整数。根据 $m,n$ 的奇偶性可以分成三种情况。

第一种情况，$m$ 是奇数，可以将 $m$ 写作 $2k+1$，结合恒等式 $\sin^2 x=1-\cos^2 x$ 可以得到
$$\sin^m x=\sin^{2k+1} x=(\sin^2 x)^k\sin x=(1-\cos^2 x)^k\sin x$$
将 $\sin x$ 和 $dx$ 结合得到 $-d(\cos x)$。

第二种情况，$n$ 是奇数，和上面类似的推导可以得到
$$\cos^n x=\cos^{2k+1} x=(\cos^2 x)^k\cos x=(1-\sin^2 x)^k\cos x$$
将 $\cos x$ 和 $dx$ 结合得到 $d(\sin x)$。

第三种情况，$m,n$ 都是偶数，使用
$$\sin^2 x=\frac{1-\cos 2x}{2},\cos^2 x=\frac{1+\cos 2x}{2}$$
替换被积分的式子，得到用 $\cos 2x$ 更低次幂的式子。

例1 求
$$\int\sin^3 x\cos^2 xdx$$
解：这个式子是第一种情况。
$$\begin{aligned}
\int\sin^3 x\cos^2 xdx&=\int\sin^2 x\cos^2 x\sin xdx\\
&=\int(1-\cos^2 x)(\cos^2 x)(-d(\cos x))\\
&=\int(1-u^2)(u^2)(-du)\\
&=\int(u^4-u^2)du\\
&=\frac{u^5}{5}-\frac{u^3}{3}+C\\
&=\frac{\cos^5 x}{5}-\frac{\cos^3 x}{3}+C
\end{aligned}$$

例2 求
$$\int\cos^5 xdx$$
解：这个例子是第二种情况。
$$\begin{aligned}
\int\cos^5 xdx&=\int\cos^4 x\cos xdx\\
&=\int(1-\sin^2 x)^2d(\sin x)\\
&=\int(1-u^2)^2du\\
&=\int(1-2u^2+u^4)du\\
&=u-\frac{2u^3}{3}+\frac{u^5}{5}+C\\
&=\sin x-\frac{2\sin^3 x}{3}+\frac{\sin^5 x}{5}+C
\end{aligned}$$

例3 求
$$\int\sin^2 x\cos^4 xdx$$
解：这个第三种情况。
$$\begin{aligned}
\int\sin^2 x\cos^4 xdx&=\int(\frac{1-\cos 2x}{2})(\frac{1+\cos 2x}{2})^2dx\\
&=\frac{1}{8}\int(1-\cos 2x)(1+2\cos 2x+\cos^2 2x)dx\\
&=\frac{1}{8}\int(1+\cos 2x-\cos^2 2x-\cos^3 2x)\\
&=\frac{1}{8}[x+\frac{1}{2}\sin 2x-\int(cos^2 2x+\cos^3 2x)dx]
\end{aligned}$$
这里涉及 $\cos^2 2x$，
$$\int\cos^2 2xdx=\frac{1}{2}\int(1+\cos 4x)dx=\frac{1}{2}(x+\frac{1}{4}\sin 4x)$$
对于 $\cos^3 2x$，这是第二种情况，所以
$$\begin{aligned}
\int\cos^3 2xdx&=\int(1-\sin^2 2x)\cos 2xdx\\
&=\frac{1}{2}\int(1-u^2)du\\
&=\frac{1}{2}(\sin 2x-\frac{\sin^3 2x}{3})
\end{aligned}$$
代入原始的式子可以得到
$$\int\sin^2 x\cos^4 xdx=\frac{1}{16}(x-\frac{1}{4}\sin 4x+\frac{1}{3}\sin^3 2x)+C$$

### 消除平方根
下面这个例子使用恒等式 $\cos^2\theta=(1+\cos 2\theta)/2$ 消除平方运算。

例4 求
$$\int_0^{\pi/4}\sqrt{1+\cos 4x}dx$$
解：利用
$$\cos^2\theta=\frac{(1+\cos 2\theta)}{2}$$
消除平方根。令 $\theta=2x$，那么
$$1+\cos 4x=2\cos^2 2x$$
所以
$$\begin{aligned}
\int_0^{\pi/4}\sqrt{1+\cos 4x}dx&=\int_0^{\pi/4}\sqrt{2\cos^2 2x}dx\\
&=\sqrt{2}\int_0^{\pi/4}|\cos 2x|dx\\
&=\sqrt{2}\int_0^{\pi/4}\cos 2xdx\\
&=\sqrt{2}\bigg[\frac{\sin 2x}{2}\bigg]_0^{\pi/4}\\
&=\frac{\sqrt{2}}{2}
\end{aligned}$$

### 正切和正割的幂的积分
涉及正切和正割的幂的积分，我们利用恒等式 $\tan^2 x=\sec^2 x-1,\sec^2 x=\tan^2 x+1$ 和部分积分来降低幂的次。

例5 求
$$\int\tan^4 xdx$$
解：
$$\begin{aligned}
\int\tan^4 xdx&=\int\tan^2 x\tan^2 xdx\\
&=\int\tan^2 x(\sec^2 x-1)dx\\
&=\int\tan^2 x\sec^2 xdx-\int\tan^2 xdx\\
&=\int\tan^2 x\sec^2 xdx-\int(\sec^2 x-1)dx\\
&=\int\tan^2 x\sec^2 xdx-\int\sec^2 x+\int dx
\end{aligned}$$
对于第一项，令
$$u=\tan x,du=\sec^2 xdx$$
有
$$\int u^2du=\frac{1}{3}u^3+C$$
所以
$$\int\tan^4 xdx=\frac{1}{3}\tan^3 x-\tan x+x+C$$

例6 求
$$\int\sec^3 xdx$$
解：分部积分法，令
$$u=\sec x,dv=\sec^2 xdx,du=\sec x\tan xdx,v=\tan x$$
所以
$$\begin{aligned}
\int\sec^3 xdx&=\sec x\tan x-\int\tan x(\sec x\tan x)dx\\
&=\sec x\tan x-\int(\sec^2 x-1)\sec xdx\\
&=\sec x\tan x+\int\sec xdx-\int\sec^3 xdx
\end{aligned}$$
简单地调整一下顺序
$$2\int\sec^3 xdx=\sec x\tan x+\int\sec xdx$$
所以
$$\int\sec^3 xdx=\frac{1}{2}\sec x\tan x+\frac{1}{2}\ln|\sec x+\tan x|+C$$

例7 求
$$\int\tan^4 x\sec^4 xdx$$
解：
$$\begin{aligned}
\int\tan^4 x\sec^2 xdx&=\int\tan^4 x(1+\tan^2 x)\sec^2 xdx\\
&=\int(\tan^6 x+\tan^4 x)\sec^2 xdx\\
&=\int(u^6+u^4)du\\
&=\frac{u^7}{7}+\frac{u^5}{5}+C\\
&=\frac{\tan^7 x}{7}+\frac{\tan^5 x}{5}+C
\end{aligned}$$

### 正弦和余弦的积分
许多涉及周期的应用会出现下面的积分式
$$\int\sin mx\sin nxdx, \int\sin mx\cos nxdx, \int\cos mx\cos nxdx$$
可以使用分部积分法，不过需要使用两次。更简单的方式是利用如下恒等式
$$\sin mx\sin nx=\frac{1}{2}[\cos(m-n)x-\cos(m+n)x]$$
$$\sin mx\cos nx=\frac{1}{2}[\sin(m-n)x+\sin(m+n)x]$$
$$\cos mx\cos nx=\frac{1}{2}[\cos(m-n)x+\cos(m+n)x]$$

例8 求
$$\int\sin 3x\cos 5xdx$$
解：
$$\begin{aligned}
\int\sin 3x\cos 5xdx&=\frac{1}{2}\int[\sin(-2x)+\sin(8x)]dx\\
&=\frac{1}{2}\int(\sin 8x-\sin 2x)dx\\
&=-\frac{\cos 8x}{16}+\frac{\cos 2x}{4}+C
\end{aligned}$$
