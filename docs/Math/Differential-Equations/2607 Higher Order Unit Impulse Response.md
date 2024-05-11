We can extend our reasoning in the first and second order cases to any order. Consider an $n^{th}$ order system with DE
$$a_nx^{(n)}+a_{n-1}x^{(n-1)}+\ldots+a_1x'+a_0x=f(t)\tag{1}$$
where we take $f(t)$ to be the input. The equation for the unit impulse response of this system is
$$a_nx^{(n)}+a_{n-1}x^{(n-1)}+\ldots+a_1x'+a_0x=\delta(t), \text{ with rest IC.}\tag{2}$$
The effect of the $\delta$ function input is to cause a jump in the $(n - 1)^{st}$ derivative at time $t = 0$, while the lower order derivatives do not jump. That is, the system is put in the state
$$x(0^+)=0,x'(0^+)=0,\ldots,x^{(n-2)}(0^+)=0,x^{(n-1)}(0^+)=1/a_n$$
To show this we use the same reasoning as in the second order case. Suppose there was a jump in a lower derivative. For example, suppose
$$x^{(n-3)}(0^+)=b\neq 0$$
Then the expression for $x^{(n-2)}(t)$ contains $b\delta(t)$, which implies that $x^{(n-1)}(t)$ contains $b\delta'(t)$, and $x^{(n)}(t)$ contains $b\delta''(t)$. This is impossible because the right-hand side of $(2)$ does not have any derivatives of the delta function.

Since $x^{(n-1)}(t)$ has a jump of $x^{(n-1)}(0^+)=1/a_n$ at $t=0$, its derivative $a_nx^{(n)}(t)$ has a unit impulse, $\delta(t)$, at $t=0$.

We conclude that the solution to $(2)$ is 0 for $t < 0$ and for $t > 0$ it is exactly the same as the solution to
$$a_nx^{(n)}+a_{n-1}x^{(n-1)}+\ldots+a_1x'+a_0x=0$$
with initial conditions
$$x(0)=0,x'(0)=0,\ldots,x^{(n-2)}(0)=0,x^{(n-1)}(0)=1/a_n$$
