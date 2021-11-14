The starting point for understanding the mathematics of pure resonance is the generalized Exponential Response formula. First recall the simple case of the Exponential Response formula:  
Asolution to
$$
p(D)x=Be^{at}\tag{1}
$$
is given by
$$
x_p=\frac{Be^{at}}{p(a)}\ p(a)\neq0\tag{2}
$$
In the sessionon Exponential Response we also saw the generalization of this formula when $p(a)=0$. Here we will need to use the special case when $p(a)\neq0$:  
A solution to equation $(1)$ is given by
$$
x_p=\frac{Bte^{at}}{p'(a)}\text{ if } p(a)=0 \text{ and } p'(a)\neq0\tag{2}
$$
We will call this the **Resonant Response Formula**.

**Example**. Find a particular solution to the DE $x''+4x=2\cos 2t$.
As usual, we try complex replacement and the ERF:  
if $z_p$ is a solution to the complex DE $z''+4z=2e^{2it}$, then $x_p$ will be a solution to $x''+4x=2\cos 2t$.  
The characteristic polynomial is $p(s)=s^2+4$ and $a=2i$, so that we have $p(a)=0$.  
But since $p'(s)=2s$, and we have $p'(a)=p'(2i)=4i\neq0$.  
The resonant case of the ERF thus gives
$$
z_p=\frac{2te^{2it}}{4i}
$$
Then taking the real part of $z_p$ gives us our particular solution
$$
\begin{aligned}
x_p&=Re(z_p)\\
&=Re(\frac{2t}{4i}(\cos 2t + i\sin 2t))\\
&=\frac{t}{2}\sin 2t
\end{aligned}
$$
