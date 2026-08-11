The fundamental matrix $\Phi(t)$ also provides a very compact and efficient integral formula for a particular solution to the inhomogeneous equation $\boldsymbol{x'} = A(t)\boldsymbol{x} + F(t)$. (presupposing of course that one can solve the homogeneous equation $\boldsymbol{x'} = A(t)\boldsymbol{x}$ first to get $\Phi$.) In this short note we give the formula (with proof!) and one example.

**Variation of parameters:** (solving inhomegeneous systems)  
(H) $\boldsymbol{x'}=A(t)\boldsymbol{x}\rightsquigarrow \Phi(t) =$ fundamental matrix  
(I) $\boldsymbol{x'}=A(t)\boldsymbol{x}+F(x)$  
Variation of parameters formula for solution to (I) (just like order 1 DE's):
$$\boldsymbol{x}=\Phi\cdot(\int\Phi^{-1}\cdot Fdt+C)$$
**proof**  
General homogeneous solution: $\boldsymbol{x}=\Phi\cdot \boldsymbol{c}$ for a constant vector $\boldsymbol{c}$  
Make $c$ variable $\rightsquigarrow$ trial solution $\boldsymbol{x}=\Phi\cdot\boldsymbol{v}(t)$  
Plug this into (I): $\boldsymbol{x'}=A\boldsymbol{x}+F\rArr \Phi'\cdot\boldsymbol{v}+\Phi\cdot\boldsymbol{v'}=A\Phi\cdot\boldsymbol{v}+F$  
Now substitute for $\Phi'=A\Phi$
$$\begin{aligned}
\rArr &A\Phi\cdot\boldsymbol{v}+\Phi\cdot\boldsymbol{v'}=A\Phi\cdot\boldsymbol{v}+F\\
\rArr &\Phi\cdot\boldsymbol{v'}=F\\
\rArr &\boldsymbol{v'}=\Phi^{-1}\cdot F\\
\rArr &\boldsymbol{v}=\int\Phi^{-1}\cdot F+C\\
\rArr &\boldsymbol{x}=\Phi\cdot(\int\Phi^{-1}\cdot Fdt+C)
\end{aligned}
$$
**Definite integral version of variation of parameters**  
$$\boldsymbol{x}(t)=\Phi(t)(\int_{t_0}^{t}\Phi^{-1}(u)\cdot F(u)du+C), \text{ where } C=\Phi^{-1}(t_0)\cdot\boldsymbol{x}(t_0)$$

**Example.** Solve
$$\boldsymbol{x'}=\begin{pmatrix}
6&5\\1&2
\end{pmatrix}\boldsymbol{x}+\begin{pmatrix}
e^t\\e^{5t}
\end{pmatrix}$$
Notation: $A = \begin{pmatrix}6&5\\1&2\end{pmatrix}$, $F=\begin{pmatrix}e^t\\e^{5t}\end{pmatrix}$  
Fundamental matrix
$$\Phi=\begin{pmatrix}
e^t&5e^{7t}\\-e^t&e^{7t}
\end{pmatrix}$$
$$\Phi^{-1}=\frac{e^{-8t}}{6}\begin{pmatrix}
e^{7t}&-5e^{7t}\\e^t&e^t
\end{pmatrix}$$
Variation of parameters:
$$\begin{aligned}
\boldsymbol{x}&=\Phi\int \Phi^{-1}\cdot Fdt\\
&=\Phi \int \frac{e^{-8t}}{6}\begin{pmatrix}
e^{7t}&-5e^{7t}\\e^t&e^t
\end{pmatrix} \cdot \begin{pmatrix}
e^t\\e^{5t}
\end{pmatrix}dt\\
&=\Phi \int \frac{1}{6} \begin{pmatrix}
1-5e^{4t}\\e^{-6t}+e^{-2t}
\end{pmatrix}dt\\
&=\frac{1}{6}\Phi\begin{pmatrix}
t-\frac{5}{4}e^{4t}+c_1\\
-\frac{1}{6}e^{-6t}-\frac{1}{2}e^{-2t}+c_2
\end{pmatrix}\\
&=\frac{1}{6}\begin{pmatrix}
te^t-\frac{5}{4}e^{5t}+c_1e^t-\frac{5}{6}e^{t}-\frac{5}{2}e^{5t}+5c_2e^{7t}\\
-e^tt+\frac{5}{4}e^{5t}-c_1e^t-\frac{1}{6}e^{t}-\frac{1}{2}e^{5t}+c_2e^{7t}
\end{pmatrix}\\
&=\frac{1}{6}[te^t\begin{pmatrix}
1\\-1
\end{pmatrix}+e^{5t}\begin{pmatrix}
-15/4\\3/4
\end{pmatrix}+e^t\begin{pmatrix}
-5/6\\-1/6
\end{pmatrix}+c_1e^t\begin{pmatrix}
1\\-1
\end{pmatrix}+c_2e^{7t}\begin{pmatrix}
5\\1
\end{pmatrix}]
\end{aligned}$$
