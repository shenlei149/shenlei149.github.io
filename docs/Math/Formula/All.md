## 三角函数与反三角函数

### 恒等式
参考 [1.3 小节](../010-Calculus/010-Functions/030-Trigonometric-Functions.md)
$$\sin mx\sin nx=\frac{1}{2}[\cos(m-n)x-\cos(m+n)x]$$
$$\sin mx\cos nx=\frac{1}{2}[\sin(m-n)x+\sin(m+n)x]$$
$$\cos mx\cos nx=\frac{1}{2}[\cos(m-n)x+\cos(m+n)x]$$
$$\tan^2 x=\sec^2 x-1$$

### 三角函数导数
参考 [3.5 小节](../010-Calculus/030-Derivatives/050-Derivatives-of-Trigonometric-Functions.md)
$$\frac{d}{dx}\sin x=\cos x$$
$$\frac{d}{dx}\cos x=-\sin x$$
$$\frac{d}{dx}\tan x=\sec^2 x$$
$$\frac{d}{dx}\cot x=-\csc^2 x$$
$$\frac{d}{dx}\sec x=\sec x\tan x$$
$$\frac{d}{dx}\csc x=-\csc x\cot x$$

### 反三角函数导数
参考 [3.9 小节](../010-Calculus/030-Derivatives/090-Inverse-Trigonometric-Functions.md)
$$\frac{d}{dx}\sin^{-1}x=\frac{1}{\sqrt{1-x^2}},|x|<1$$
$$\frac{d}{dx}\cos^{-1}x=-\frac{1}{\sqrt{1-x^2}},|x|<1$$
$$\frac{d}{dx}\tan^{-1}x=\frac{1}{1+x^2}$$
$$\frac{d}{dx}\cot^{-1}x=-\frac{1}{1+x^2}$$
$$\frac{d}{dx}\sec^{-1}x=\frac{1}{|x|\sqrt{x^2-1}},|x|>1$$
$$\frac{d}{dx}\csc^{-1}x=-\frac{1}{|x|\sqrt{x^2-1}},|x|>1$$

### 三角函数积分
参考 [5.5 小节](../010-Calculus/050-Integrals/050-Indefinite-Integrals-and-the-Substitution-Method.md)
$$\int\tan xdx=\ln|\sec x|+C$$
$$\int\cot xdx=\ln|\sin x|+C$$
$$\int\sec xdx=\ln|\sec x+\tan x|+C$$
$$\int\csc xdx=-\ln|\csc x+\cot x|+C$$
$$\int\sin^2x dx=\frac{x}{2}-\frac{\sin 2x}{4}+C$$
$$\int\cos^2x dx=\frac{x}{2}+\frac{\sin 2x}{4}+C$$

## 双曲函数及其反函数

### 定义
参考 [7.3 小节](../010-Calculus/070-Integrals-and-Transcendental-Functions/030-Hyperbolic-Functions.md)
$$\sinh x=\frac{e^x-e^{-x}}{2}$$
$$\cosh x=\frac{e^x+e^{-x}}{2}$$
$$\tanh x=\frac{\sinh x}{\cosh x}=\frac{e^x-e^{-x}}{e^x+e^{-x}}$$
$$\coth x=\frac{\cosh x}{\sinh x}=\frac{e^x+e^{-x}}{e^x-e^{-x}}$$
$$\operatorname{sech} x=\frac{1}{\cosh x}=\frac{2}{e^x+e^{-x}}$$
$$\operatorname{csch} x=\frac{1}{\sinh x}=\frac{2}{e^x-e^{-x}}$$
$$\sinh^{-1}x=\ln(x+\sqrt{x^2+1}),-\infty<x<\infty$$
$$\cosh^{-1}x=\ln(x+\sqrt{x^2-1}),x\geq 1$$
$$\tanh^{-1}x=\frac{1}{2}\ln\frac{1+x}{1-x},|x|<1$$
$$\coth^{-1}x=\frac{1}{2}\ln\frac{x+1}{x-1},|x|>1$$
$$\operatorname{sech}^{-1}x=\ln\bigg(\frac{1+\sqrt{1-x^2}}{x}\bigg),0<x\leq 1$$
$$\operatorname{csch}^{-1}x=\ln\bigg(\frac{1}{x}+\frac{\sqrt{1+x^2}}{|x|}\bigg),x\neq 0$$

### 恒等式
参考 [7.3 小节](../010-Calculus/070-Integrals-and-Transcendental-Functions/030-Hyperbolic-Functions.md)
$$\cosh^2 x-\sinh^2 x=1$$
$$\sinh 2x=2\sinh x\cosh x$$
$$\cosh 2x=\cosh^2 x+\sinh^2 x$$
$$\cosh^2 x=\frac{\cosh 2x+1}{2}$$
$$\sinh^2 x=\frac{\cosh 2x-1}{2}$$
$$\tanh^2 x=1-\operatorname{sech}^2 x$$
$$\coth^2 x=1+\operatorname{csch}^2 x$$
$$\operatorname{sech}^{-1}x=\cosh^{-1}\frac{1}{x}$$
$$\operatorname{csch}^{-1}x=\sinh^{-1}\frac{1}{x}$$
$$\coth^{-1}x=\tanh^{-1}\frac{1}{x}$$

### 和差倍半
$$\sinh(x+y)=\sinh x\cosh y+\cosh x\sinh y$$
$$\cosh(x+y)=\cosh x\cosh y+\sinh x\sinh y$$
$$\sinh 2x=2\sinh x\cosh x$$
$$\cosh 2x=\cosh^2x+\sinh^2x$$

### 微分
参考 [7.3 小节](../010-Calculus/070-Integrals-and-Transcendental-Functions/030-Hyperbolic-Functions.md)
$$\frac{d}{dx}\sinh u=\cosh u\frac{du}{dx}$$
$$\frac{d}{dx}\cosh u=\sinh u\frac{du}{dx}$$
$$\frac{d}{dx}\tanh u=\operatorname{sech}^2 u\frac{du}{dx}$$
$$\frac{d}{dx}\coth u=-\operatorname{csch}^2 u\frac{du}{dx}$$
$$\frac{d}{dx}\operatorname{sech} u=-\operatorname{sech} u\tanh u\frac{du}{dx}$$
$$\frac{d}{dx}\operatorname{csch} u=-\operatorname{csch} u\coth u\frac{du}{dx}$$
$$\frac{d}{dx}\sinh^{-1}u=\frac{1}{\sqrt{1+u^2}}\frac{du}{dx}$$
$$\frac{d}{dx}\cosh^{-1}u=\frac{1}{\sqrt{u^2-1}}\frac{du}{dx},u>1$$
$$\frac{d}{dx}\tanh^{-1}u=\frac{1}{1-u^2}\frac{du}{dx},|u|<1$$
$$\frac{d}{dx}\coth^{-1}u=\frac{1}{1-u^2}\frac{du}{dx},|u|>1$$
$$\frac{d}{dx}\operatorname{sech}^{-1}u=-\frac{1}{u\sqrt{1-u^2}}\frac{du}{dx},0<u<1$$
$$\frac{d}{dx}\operatorname{csch}^{-1}u=-\frac{1}{|u|\sqrt{1+u^2}}\frac{du}{dx},u\neq 0$$

### 积分
参考 [7.3 小节](../010-Calculus/070-Integrals-and-Transcendental-Functions/030-Hyperbolic-Functions.md)
$$\int\sinh udu=\cosh u+C$$
$$\int\cosh udu=\sinh u+C$$
$$\int\operatorname{sech}^2 udu=\tanh u+C$$
$$\int\operatorname{csch}^2 udu=-\coth u+C$$
$$\int\operatorname{sech} u\tanh udu=-\operatorname{sech} u+C$$
$$\int\operatorname{csch} u\coth udu=-\operatorname{csch} u+C$$
$$\int\operatorname{sech}udu=\tan^{-1}(\sinh u)+C$$
$$\int\operatorname{sech}udu=\sin^{-1}(\tanh u)+C$$
$$\int\frac{du}{\sqrt{a^2+u^2}}=\sinh^{-1}\frac{u}{a}+C,a>0$$
$$\int\frac{du}{\sqrt{u^2-a^2}}=\cosh^{-1}\frac{u}{a}+C,0<a<u$$
$$\int\frac{du}{a^2-u^2}=\begin{cases}
\frac{1}{a}\tanh^{-1}\frac{u}{a}+C,u^2<a^2\\
\frac{1}{a}\coth^{-1}\frac{u}{a}+C,u^2>a^2
\end{cases}$$
$$\int\frac{du}{u\sqrt{a^2-u^2}}=-\frac{1}{a}\operatorname{sech}^{-1}\frac{u}{a}+C,0<u<a$$
$$\int\frac{du}{u\sqrt{a^2+u^2}}=-\frac{1}{a}\operatorname{csch}^{-1}\bigg|\frac{u}{a}\bigg|+C,u\neq 0,a>0$$

## 泰勒级数
$$\frac{1}{1-x}=1+x+x^2+\cdots+x^n+\cdots=\sum_{n=0}^\infty x^n,|x|<1$$
$$\frac{1}{1+x}=1-x+x^2-\cdots+(-x)^n+\cdots=\sum_{n=0}^\infty (-1)^nx^n,|x|<1$$
$$e^x=1+x+\frac{x^2}{2!}+\cdots+\frac{x^n}{n!}+\cdots=\sum_{n=0}^\infty\frac{x^n}{n!},|x|<\infty$$
$$\sin x=x-\frac{x^3}{3!}+\frac{x^5}{5!}-\cdots+(-1)^n\frac{x^{2n+1}}{(2n+1)!}+\cdots=\sum_{n=0}^\infty\frac{(-1)^nx^{2n+1}}{(2n+1)!},|x|<\infty$$
$$\cos x=1-\frac{x^2}{2!}+\frac{x^4}{4!}-\cdots+(-1)^n\frac{x^{2n}}{(2n)!}+\cdots=\sum_{n=0}^\infty\frac{(-1)^nx^{2n}}{(2n)!},|x|<\infty$$
$$\ln(1+x)=x-\frac{x^2}{2}+\frac{x^3}{3}-\cdots+(-1)^{n-1}\frac{x^n}{n}+\cdots=\sum_{n=0}^\infty\frac{(-1)^{n-1}x^n}{n},-1<x\leq 1$$
$$\tan^{-1}x=x-\frac{x^3}{3}+\frac{x^5}{5}-\cdots+(-1)^n\frac{x^{2n+1}}{2n+1}+\cdots=\sum_{n=0}^\infty\frac{(-1)^nx^{2n+1}}{2n+1},|x|\leq 1$$

## 空间中的曲线
单位切矢量
$$\boldsymbol{T}=\frac{\boldsymbol{v}}{|\boldsymbol{v}|}$$
主单位法矢量
$$\boldsymbol{N}=\frac{d\boldsymbol{T}/dt}{|d\boldsymbol{T}/dt|}$$
次法线矢量
$$\boldsymbol{B}=\boldsymbol{T}\times\boldsymbol{N}$$
曲率
$$\kappa=\bigg|\frac{d\boldsymbol{T}}{ds}\bigg|=\frac{|\boldsymbol{v}\times\boldsymbol{a}|}{|\boldsymbol{v}|^3}$$
扭矩
$$\tau=-\frac{d\boldsymbol{B}}{ds}\cdot\boldsymbol{N}=\frac{\begin{pmatrix}
\dot{x}&\dot{y}&\dot{z}\\
\ddot{x}&\ddot{y}&\ddot{z}\\
\dddot{x}&\dddot{y}&\dddot{z}
\end{pmatrix}}{|\boldsymbol{v}\times\boldsymbol{a}|^2}$$
加速度的切向和法向分量
$$\begin{aligned}
\boldsymbol{a}&=a_T\boldsymbol{T}+a_N\boldsymbol{N}\\
a_T&=\frac{d}{dt}|\boldsymbol{v}|\\
a_N&=\kappa|\boldsymbol{v}|^2=\sqrt{|\boldsymbol{a}|^2-a_T^2}
\end{aligned}$$

## 坐标转换公式
详情参考 [14.7](../010-Calculus/140-Multiple-Integrals/070-Triple-Integrals-in-Cylindrical-and-Spherical-Coordinates.md)

柱坐标系到直角坐标系
$$\begin{aligned}
x&=r\cos\theta\\
y&=r\sin\theta\\
z&=z
\end{aligned}$$
球坐标系到直角坐标系
$$\begin{aligned}
x&=\rho\sin\phi\cos\theta\\
y&=\rho\sin\phi\sin\theta\\
z&=\rho\cos\phi
\end{aligned}$$
球坐标系到柱坐标系
$$\begin{aligned}
r&=\rho\sin\phi\\
z&=\rho\cos\phi\\
\theta&=\theta
\end{aligned}$$
三维积分
$$\begin{aligned}
dV&=dxdydz\\
&=rdzdrd\theta\\
&=\rho^2\sin\phi d\rho d\phi d\theta
\end{aligned}$$
