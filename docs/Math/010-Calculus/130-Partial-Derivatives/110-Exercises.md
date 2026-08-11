1. 函数
$$f(x,y)=\begin{cases}
xy\frac{x^2-y^2}{x^2+y^2},&&(x,y)\neq(0,0)\\
0,&&(x,y)=(0,0)
\end{cases}$$
在 $(0,0)$ 处是连续的。求 $f_{xy}(0,0),f_{yx}(0,0)$。

解：
$$\begin{aligned}
\frac{\partial f}{\partial x}&=y\frac{x^2-y^2}{x^2+y^2}+xy\frac{2x(x^2+y^2)-2x(x^2-y^2)}{(x^2+y^2)^2}\\
&=y\frac{x^2-y^2}{x^2+y^2}+xy\frac{4xy^2}{(x^2+y^2)^2}\\
&=y\frac{(x^2-y^2)(x^2-y^2)}{(x^2+y^2)^2}+xy\frac{4xy^2}{(x^2+y^2)^2}\\
&=\frac{yx^4-y^5}{(x^2+y^2)^2}+\frac{4x^2y^3}{(x^2+y^2)^2}\\
&=\frac{yx^4-y^5+4x^2y^3}{(x^2+y^2)^2}
\end{aligned}$$
$$\begin{aligned}
f_x(0,0)&=\lim_{x\to 0}\frac{f(x,0)-f(0,0)}{x}\\
&=\lim_{x\to 0}\frac{0-0}{x}\\
&=0
\end{aligned}$$
$$\begin{aligned}
f_{xy}(0,0)&=\lim_{y\to 0}\frac{f_x(0,y)-f_x(0,0)}{y}\\
&=\lim_{y\to 0}\frac{-y-0}{y}\\
&=-1
\end{aligned}$$
$$\begin{aligned}
\frac{\partial f}{\partial y}&=x\frac{x^2-y^2}{x^2+y^2}+xy\frac{-2y(x^2+y^2)-2y(x^2-y^2)}{(x^2+y^2)^2}\\
&=x\frac{x^2-y^2}{x^2+y^2}+xy\frac{-4x^2y}{(x^2+y^2)^2}\\
&=x\frac{(x^2-y^2)(x^2-y^2)}{(x^2+y^2)^2}+xy\frac{-4x^2y}{(x^2+y^2)^2}\\
&=\frac{x^5-xy^4}{(x^2+y^2)^2}+\frac{-4x^3y^2}{(x^2+y^2)^2}\\
&=\frac{x^5-xy^4-4x^3y^2}{(x^2+y^2)^2}
\end{aligned}$$
$$\begin{aligned}
f_y(0,0)&=\lim_{y\to 0}\frac{f(0,y)-f(0,0)}{y}\\
&=\lim_{y\to 0}\frac{0-0}{y}\\
&=0
\end{aligned}$$
$$\begin{aligned}
f_{yx}(0,0)&=\lim_{x\to 0}\frac{f_y(x,0)-f_y(0,0)}{x}\\
&=\lim_{x\to 0}\frac{x-0}{x}\\
&=1
\end{aligned}$$
