### Description of the Equation
A general $2 \times 2$ linear system is given by:
$$x'=ax+by$$
$$y'=cx+dy$$
The terms have been arranged in a suggestive manner. We can express this system using matrices and vectors:
$$\begin{pmatrix}
x'\\
y'
\end{pmatrix}=\begin{pmatrix}
a&b\\
c&d\\
\end{pmatrix}\begin{pmatrix}
x\\
y
\end{pmatrix}$$
We can present this in the following even more compact form.  
Let $A=\begin{pmatrix}a&b\\c&d\end{pmatrix}$ and write $u$ for the column vector $\begin{pmatrix}x\\y\end{pmatrix}$.  
We have $u'(t)=\begin{pmatrix}x'(t)\\y'(t)\end{pmatrix}$ and the system is simply $u'=Au$.
**Example 1.** Our favorite system, governing the rabbit populations in farmers Jones' and McGregor's fields, was
$$x'=0.3x+0.1y$$
$$y'=0.2x+0.4y$$
which has matrix form
$$\begin{pmatrix}
x'\\
y'
\end{pmatrix}=\begin{pmatrix}
0.3&0.1\\
0.2&0.4
\end{pmatrix}\begin{pmatrix}
x\\
y
\end{pmatrix}$$
or
$$u'=Au, \text{ where } A=\begin{pmatrix}
0.3&0.1\\
0.2&0.4
\end{pmatrix}$$

### Description of the Solution
To describe the solution, we will use the column vector $u(t)=\begin{pmatrix}x(t)\\y(t)\end{pmatrix}$.  
**Example 2.** Earlier we used the method of elimination to solve the system in example 1. We found $x(t)=c_1e^{0.5t}+c_2e^{0.2t}$ and $y(t)=2c_1e^{0.5t}-c_2e^{0.2t}$. Rewriting this in vector form we have
$$u(t)=\begin{pmatrix}
c_1e^{0.5t}+c_2e^{0.2t}\\
2c_1e^{0.5t}-c_2e^{0.2t}
\end{pmatrix}$$
We can rewrite this as
$$u(t)=c_1e^{0.5t}\begin{pmatrix}
1\\2
\end{pmatrix}+c_2e^{0.2t}\begin{pmatrix}
1\\-1
\end{pmatrix}$$
which is a clearer way of presenting it. Let
$$u_1(t)=e^{0.5t}\begin{pmatrix}
1\\2
\end{pmatrix}$$
$$u_2(t)=e^{0.2t}\begin{pmatrix}
1\\-1
\end{pmatrix}$$
The column vectors $u_1(t)$ and $u_2(t)$ are both solutions. Since they both involve only one form of exponential, they are sometimes known as basic **independent solutions**, or **normal modes**. The general solution is a linear combination of them. We will learn much more about normal modes in the sessions on matrix methods and the phase portrait.  
**Remark.** As with linear second order ODE's in unit 2, the general solution to to a 2 linear system should always consist of linear combinations of two truly different solutions. It is not necessary, but usually our techniques will make these two solutions the normal modes.

### Geometry of the Solutions
Suppose you want to plot a solution $u(t)$. As time increases, it traces a curve in the $xy$-plane.  
**Example.** The solution $u_1(t)$ traces a ray that passes through $(1, 2)$ at $t = 0$ and move off towards infinity in a straight line, with exponential speed. There is another ray through $(1, -1)$, corresponding to the solution $u_2(t)$. (This is tricky: the exponential in the formula for $u_1$ might make you think the trajectory is curved. However, if you look carefully at the formula you will see $u_1(t)$ is always a multiple of the vector $(1, 2)^T$.)
