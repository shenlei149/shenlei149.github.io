这一小节讨论称为恰当微分方程的一类微分方程，也有明确的求解方法。

例 1 求微分方程
$$2x+y^2+2xyy'=0\tag{1}$$
解：这个微分方程不是一阶方程，也不是可分离变量的微分方程。不过，注意到函数 $\Psi(x,y)=x^2+xy^2$ 有如下性质
$$\frac{\partial\Psi}{\partial x}=2x+y^2,\frac{\partial\Psi}{\partial y}=2xy\tag{2}$$
那么微分方程可以写作
$$\frac{\partial\Psi}{\partial x}+\frac{\partial\Psi}{\partial y}\frac{dy}{dx}=0\tag{3}$$
假定 $y$ 是 $x$ 的函数，$(3)$ 的左边是 $d\Psi/dx$ 的链式求导，那么
$$\frac{d\Psi}{dx}=\frac{d}{dx}(x^2+xy^2)=0\tag{4}$$
两边积分得到
$$\Psi(x,y)=x^2+xy^2=c\tag{5}$$
其中 $c$ 是任意常量。$\Psi(x,y)$ 的等高线是方程 $(1)$ 的解，即隐式方程 $(5)$。

求解方程 $(1)$ 的关键是找到满足 $(2)$ 的函数 $\Psi$。一般地，给定微分方程
$$M(x,y)+N(x,y)y'=0\tag{6}$$
假定存在函数 $\Psi(x,y)$ 使得
$$\frac{\partial\Psi}{\partial x}=M(x,y),\frac{\partial\Psi}{\partial y}=N(x,y)\tag{7}$$
那么 $\Psi(x,y)=c$ 定义了隐式的解 $y=\phi(x)$。

当存在函数 $\Psi(x,y)$ 使得 $\Psi_x=M,\Psi_y=N$，那么
$$M(x,y)+N(x,y)y'=\frac{\partial\Psi}{\partial x}+\frac{\partial\Psi}{\partial y}\frac{dy}{dx}=\frac{d}{dx}\Psi(x,\phi(x))$$
那么微分方程 $(6)$ 写作
$$\frac{d}{dx}\Psi(x,\phi(x))=0\tag{8}$$
形如 $(6)$ 的方程称为恰当微分方程（`exact differential equation`）因为恰好可以表示成指定函数的导数。方程 $(6)$ 或者等价形式 $(8)$ 的解是
$$\Psi(x,y)=c\tag{9}$$
其中 $c$ 是任意常量。

下面定理告诉我们如何确定一个微分方程是恰当的，如果是证明过程给出了求解 $\Psi(x,y)$ 的过程。

**定理 2.6.1**
> 令函数 $M,N,M_y,N_x$ 在矩形 $R:\alpha<x<\beta,\gamma<y<\delta$ 上是连续函数。这里矩形不是必须的，是简单连通区域即可。那么方程 $(6)$ 在 $R$ 上是恰当微分方程等价于
> $$M_y(x,y)=N_x(x,y)\tag{10}$$
> 因此存在函数 $\Psi$ 满足 $(7)$
> $$\Psi_x(x,y)=M(x,y),\Psi_y(x,y)=N(x,y)$$
> 等价于 $M,N$ 满足 $(10)$。

这个定理的证明分为两个部分。首先，如果有一个函数 $\Psi$ 使得 $(7)$ 成立，那么 $(10)$ 是成立的。下面从 $(7)$ 计算 $M_y,N_x$
$$M_y(x,y)=\Psi_{xy}(x,y),N_x(x,y)=\Psi_{yx}(x,y)\tag{11}$$
由于 $M_y,N_x$ 是连续的，那么 $\Psi_{xy},\Psi_{yx}$ 也是连续的。这就保证 $(10)$ 是成立的。

下面证明如果 $M,N$ 满足 $(10)$，那么 $(6)$ 是恰当的。证明过程涉及构造满足 $(7)$ 的函数 $\Psi$
$$\Phi_x(x,y)=M(x,y),\Psi_y(x,y)=N(x,y)$$
首先对 $(7)$ 中的第一式子 $x$ 做积分，那么得到
$$\Psi_(x,y)=Q(x,y)+h(y)\tag{12}$$
其中 $Q(x,y)$ 任意使得 $Q_x=M$ 的可微函数，比如可以选择
$$Q(x,y)=\int_{x_0}^xM(s,y)ds$$
其中 $x_0$ 是 $\alpha<x_0<\beta$ 范围内的常量。$h$ 是仅包含 $y$ 的可微函数。现在要证明总是可以得到一个 $h$ 使得 $(7)$ 的第二个式子成立。将 $(12)$ 对 $y$ 求导得到
$$\Psi_y(x,y)=\frac{\partial Q}{\partial y}(x,y)+h'(y)=N(x,y)$$
那么
$$h'(y)=N(x,y)-\frac{\partial Q}{\partial y}(x,y)\tag{14}$$
如果上式右边仅仅是 $y$ 的函数，那么可以确定 $h(y)$。一种方法就是证明其对 $x$ 的导数为零。下面是求导结果
$$\frac{\partial N}{\partial x}(x,y)-\frac{\partial}{\partial x}\frac{\partial Q}{\partial y}(x,y)\tag{15}$$
交换上式第二项微分顺序得到
$$\frac{\partial N}{\partial x}(x,y)-\frac{\partial}{\partial y}\frac{\partial Q}{\partial x}(x,y)$$
由于 $Q_x=M$
因此
$$\frac{\partial N}{\partial x}(x,y)-\frac{\partial M}{\partial y}(x,y)$$
根据 $(10)$ 上面的结果是零。因此不管形式如何，$(14)$ 的右边不依赖于 $x$。那么通过对 $(14)$ 积分可以得到 $h(y)$，这就完成了对定理 2.6.1 的证明。

所以，为了得到 $\Psi(x,y)$，第一步是验证其存在，然后根据上述证明步骤通过积分求解。

例 2 求微分方程
$$(y\cos x+2xe^y)+(\sin x+x^2e^y-1)y'=0\tag{16}$$
解：通过计算 $M_y,N_x$ 有
$$M_y(x,y)=\cos x+2xe^y=N_x(x,y)$$
因此这是恰当微分方程，存在 $\Psi(x,y)$ 使得
$$\Psi_x(x,y)=y\cos x+2xe^y$$
$$\Psi_y(x,y)=\sin x+x^2e^y-1$$
首先第一个方程对 $x$ 积分得到
$$\Psi(x,y)=y\sin x+x^2e^y+h(y)\tag{17}$$
通过 $(17)$ 计算 $\Psi_y$ 且令其等于 $N$
$$\Psi_y(x,y)=\sin x+x^2e^y+h'(y)=\sin x+x^2e^y-1$$
那么
$$h'(y)=-1,h(y)=-y$$
由于找到任意满足条件的微分方程就足够了，无需求通解。将 $h(y)$ 代入 $(17)$ 得到
$$\Psi(x,y)=y\sin x+x^2e^y-y$$
那么微分方程 $(16)$ 的解是
$$y\sin x+x^2e^y-y=c\tag{18}$$

例 3 求微分方程
$$(3xy+y^2)+(x^2+xy)y'=0\tag{19}$$
解：首先计算
$$M_y(x,y)=3x+2y,N_x(x,y)=2x+y$$
由于 $M_y\neq N_x$，因此不是恰当微分方程。这样就无法找到满足条件的 $\Psi(x,y)$。下面试着求解 $\Psi(x,y)$
$$\Psi_x(x,y)=3xy+y^2,\Psi_y(x,y)=x^2+xy\tag{20}$$
首先对 $(20)$ 的第一个式子积分得到
$$\Psi(x,y)=\frac{3}{2}x^2y+xy^2+h(y)\tag{21}$$
上式对 $y$ 求导
$$\frac{3}{2}x^2+2xy+h'(y)=x^2+xy$$
因此
$$h'(y)=-\frac{1}{2}x^2-xy\tag{22}$$
由于 $(22)$ 的右边即依赖于 $x$ 也依赖于 $y$，因此无法求解 $h(y)$，也就没有满足 $(20)$ 的 $\Psi(x,y)$。

### 积分因子
有时，可以通过乘以一个合适的积分因子将一个非恰当微分方程转化成一个恰当微分方程。假定积分因子是 $\mu$，乘以微分方程
$$M(x,y)+N(x,y)y'=0\tag{23}$$
得到恰当微分方程
$$\mu(x,y)M(x,y)+\mu(x,y)N(x,y)y'=0\tag{24}$$
根据定理 2.6.1 方程 $(24)$ 是恰当的等价与
$$(\mu M)_y=(\mu N)x\tag{25}$$
由于 $M,N$ 是给定的，那么 $(25)$ 要求 $\mu$ 满足
$$M\mu_y-N\mu_x+(M_y-N_x)\mu=0\tag{26}$$
如果满足 $(26)$ 的 $\mu$ 能够找到，那么 $(24)$ 是恰当微分方程，可以用之前的解法求解。这个解也是 $(23)$ 的解。

偏微分方程 $(26)$ 可能有多个解，任意解都可以作为 $(23)$ 的积分因子。

不过，$(26)$ 不容易求解，大部分时候至少和 $(23)$ 一样不同容易求解。尽管积分因子是很有用的工具，但是只能适用于特殊情况。最重要的场景是 $\mu$ 仅依赖于 $x$ 或者 $y$。

如果 $\mu$ 仅仅依赖于 $x$，那么偏微分 $\mu_x$ 就是普通的微分 $d\mu/dt$，并且 $\mu_y=0$。这样方程 $(26)$ 可以写作
$$\frac{d\mu}{dt}=\frac{M_y-N_x}{N}\mu\tag{27}$$
如果 $P=\frac{M_y-N_x}{N}$ 仅仅依赖于 $x$，积分因子 $\mu$ 也是，$(27)$ 是线性方程并且可以分离遍历，解是
$$\mu(x)=\exp\int P(x)dx$$

类似的，如果 $\mu$ 仅仅依赖于 $y$，那么如果
$$Q=\frac{N_x-M_y}{M}$$
也仅仅是 $y$ 的函数，那么
$$\mu(y)=\exp\int Q(y)dy$$

稍微更进一步，如果 $\mu$ 是 $(xy)$ 的函数，并且
$$R=\frac{N_x-M_y}{xM-yN}$$
也是 $(xy)$ 的函数，那么
$$\mu(t)=\exp\int R(t)dt,t=xy$$

例 4 找到
$$(3xy+y^2)+(x^2+xy)y'=0\tag{19}$$
的积分因子并求解方程。

解：例 3 已经告诉我们这个方程是非恰当微分方程。下面确定积分因子是否仅仅依赖于 $x$。
$$\frac{M_y-N_x}{N}=\frac{3x+2y-(2x+y)}{x^2+xy}=\frac{1}{x}\tag{28}$$
因此积分因子 $\mu$ 仅仅是 $x$ 的函数，那么满足微分方程
$$\frac{d\mu}{dt}=\frac{\mu}{x}\tag{29}$$
那么
$$\mu(x)=x\tag{30}$$
$(19)$ 两边同乘 $\mu(x)$ 得到
$$(3x^2y+xy^2)+(x^3+x^2y)y'=0\tag{31}$$
由于
$$\frac{\partial}{\partial y}(3x^2y+xy^2)=3x^2+2xy=\frac{\partial}{\partial x}(x^3+x^2y)$$
所以 $(31)$ 是恰当微分方程。因此存在函数 $\Psi$ 使得
$$\Psi_x(x,y)=3x^2y+xy^2,\Psi_y(x,y)=x^3+x^2y\tag{32}$$
第一个式子对 $x$ 积分得到
$$\Psi(x,y)=x^3y+\frac{1}{2}x^2y^2+h(y)$$
将上式代入 $(32)$ 的第二个式子得到
$$x^3+x^2y+h'(y)=x^3+x^2y$$
因此 $h'(y)=0$，那么 $h(y)$ 是常量。那么 $(31)$，也是 $(19)$ 的隐式解是
$$x^3y+\frac{1}{2}x^2y^2=c$$
不难验证
$$\mu(x,y)=\frac{1}{xy(2x+y)}$$
也是 $(19)$ 的积分因子，也能得到相同的结果，但是要困难许多。
