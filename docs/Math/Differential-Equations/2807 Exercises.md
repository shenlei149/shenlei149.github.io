**Example 1.** Suppose that $F(s)$ is the Laplace transform of $f(t)$, and let $a > 0$. Find a formula for the Laplace transform of $g(t) = f(at)$ in terms of $F(s)$, by using the integral definition and making a change of variable. Verify your formula by using formulas and rules to compute both $\mathcal{L}(f(t))$ and $\mathcal{L}(f(at))$ with $f(t) = t^n$.  
**Solution.**
$$G(s)=\int_0^\infty f(at)e^{-st}dt$$
make the substitution $u = at$. Then $du = adt$,
$$\begin{aligned}
G(s)&=\int_0^\infty f(u)e^{-su/a}\frac{1}{a}du\\
&=\frac{1}{a}\int_0^\infty f(u)e^{-\frac{s}{a}u}du\\
&=\frac{1}{a}F(\frac{s}{a})
\end{aligned}$$
For example, take $f(t) = t^n$, so $F(s)=\frac{n!}{s^{n+1}}$, and $g(t)=a^nt^n$, so $G(s)=\frac{a^nn!}{s^{n+1}}$.
$$\begin{aligned}
\frac{1}{a}F(\frac{s}{a})&=\frac{1}{a}\frac{n!}{(\frac{s}{a})^{n+1}}\\
&=\frac{a^nn!}{s^{n+1}}\\
&=G(s)
\end{aligned}$$

**Example 2.** Use your calculus skills: Show that if $h(t) = f(t) ∗ g(t)$ then $H(s) = F(s)G(s)$. Do this by writing $F(s) = \int_0^\infty f(x)e^{-sx}dx$ and $G(s) = \int_0^\infty g(y)e^{-sy}dy$; expressing the product as a double integral; and changing coordinates using $x = t - \tau, y = \tau$.  
**Solution.**
$$F(s)G(s)=\int_0^\infty\int_0^\infty f(x)e^{-sx} g(y)e^{-sy} dxdy=\iint_R f(x)g(y)e^{-(x+y)s} dxdy$$
where $R$ is the first quadrant.  
We can use the substitution is $x = t - \tau, y = \tau$  
To convert to these coordinates, note that the Jacobian is
$$\det\begin{bmatrix}
\frac{\partial x}{\partial t}&\frac{\partial x}{\partial \tau}\\
\frac{\partial y}{\partial t}&\frac{\partial y}{\partial \tau}
\end{bmatrix}=\det\begin{bmatrix}
1&-1\\
0&1
\end{bmatrix}=1$$
For fixed $t, \tau$ ranges over numbers between $0$ and $t$, and $t$ ranges over positive numbers.  
Since $x+y=t$,
$$\begin{aligned}
F(s)G(s)&=\int_0^\infty\int_0^t f(t-\tau)g(\tau)e^{-st}d\tau dt\\
&=\int_0^\infty(\int_0^t f(t-\tau)g(\tau)d\tau)e^{-st} dt\\
&=\int_0^\infty (f(t)*g(t))e^{-st} dt\\
&=\int_0^\infty h(t)e^{-st} dt\\
&=H(s)
\end{aligned}$$
