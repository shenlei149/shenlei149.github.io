Consider the system we studied in several examples in the opening session of unit 4:
$$\begin{pmatrix}
x'\\y'
\end{pmatrix}=\begin{pmatrix}
1&3\\1&-1
\end{pmatrix}\begin{pmatrix}
x\\y
\end{pmatrix}$$
We are now going to show a new method of solving this system, which makes use of the matrix form for writing it. Recall that two modal solutions to the system are
$$e^{2t}\begin{pmatrix}
3\\1
\end{pmatrix} \text{ and } e^{-2t}\begin{pmatrix}
3\\-1
\end{pmatrix}$$
Based on this, our new method is to look for solutions of the form
$$\begin{pmatrix}
x\\y
\end{pmatrix}=e^{\lambda t}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}\tag{1}$$
where $a_1, a_2$ and $\lambda$ are *unknown* constants. We substitute this into the system to determine what these unknown constants should be. This gives
$$\lambda e^{\lambda t}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=e^{\lambda t}\begin{pmatrix}
1&3\\1&-1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}\tag{2}$$
We can cancel the factor $e^{\lambda t}$ from both sides, getting
$$\lambda \begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
1&3\\1&-1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}\tag{3}$$
This is a matrix equation for the three unknowns. It is not very clear how to solve it. When faced with equations in unfamiliar notation, a reasonable strategy is to rewrite them in more familiar notation. If we try this, we get the pair of equations
$$\begin{aligned}
\lambda a_1&=a_1&+&&3a_2\\
\lambda a_2&=a_1&-&&a_2
\end{aligned}$$
Technically speaking, these are a pair of nonlinear equations in three variables. The trick in solving them is to look at them as a pair of linear equations in the unknowns $a_i$, with $\lambda$ viewed as a parameter. If we think of them this way, it immediately suggests writing them in standard form
$$\begin{aligned}
&&(1-\lambda)a_1&&+&&3a_2&&=0\\
&&a_1&&+&&(-1-\lambda)a_2&&=0\tag{4}
\end{aligned}$$
In this form, we recognize them as forming a square system of homogeneous linear equations. According to our theorem on square homogeneous systems they have a non-zero solution for the $a$'s if and only if the determinant of coefficients is zero:
$$\begin{vmatrix}
1-\lambda&3\\1&-1-\lambda
\end{vmatrix}=0$$
After calculation of the determinant this becomes the equation
$$\lambda^2-4=0$$
The roots of this equation are 2 and -2. What the argument shows is that the equations $(4)$ (and therefore also $(2)$) have non-trivial solutions for the $a$'s exactly when $\lambda = 2$ or $\lambda = 2$. To complete the work, we see that for these values of the parameter $\lambda$, the system $(4)$ becomes respectively
$$\begin{aligned}
-a_1+3a_2&&=0&&&&3a_1+3a_2&&=0\\
a_1-3a_2&&=0&&&&a_1+a_2&&=0\\
\text{for } \lambda=2&&&&&&\text{for } \lambda=-2\tag{5}
\end{aligned}$$
**Remark.** It is of course no accident that in each case the two equations of the system become dependent, i.e., one is a constant multiple of the other. If this were not so, the two equations would have only the trivial solution (0, 0). All of our effort has been to locate the two values of $\lambda$ for which this will not be so. The dependency of the two equations is thus a check on the correctness of the value of $\lambda$.  
To conclude, we solve the two systems in $(5)$. This is best done by assigning the value 1 to one of the unknowns, and solving for the other. First try $a_1 = 1$; if that does not work (in which case, the solution to $(5)$ will have $a_1 = 0$), try $a_2 = 1$. We get
$$\begin{aligned}
\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
3\\1
\end{pmatrix} \text{ for } \lambda=2&&&&\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
1\\-1
\end{pmatrix} \text{ for } \lambda=-2\end{aligned}$$
which gives us, in view of $(1)$, the two solutions:
$$\begin{aligned}
e^{2t}
\end{aligned}\begin{pmatrix}
3\\1
\end{pmatrix} \text{ and } e^{-2t}\begin{pmatrix}
1\\-1
\end{pmatrix}$$
which are essentially the two solutions we had found previously by the method of elimination.

**Remarks.**
1. With the elimination method, the basic normal solutions could be multiplied by an arbitrary non-zero constant without changing the validity of the general solution. Here, this corresponds to the fact that we get to select an arbitrary value of one of the $a$'s (the other value then being determined).
2. Is there some way of passing from $(3)$ (the point at which we were temporarily stuck) to $(4)$ by using matrices, without writing out the equations separately? The temptation in $(3)$ is to try to combine the two column vectors $\boldsymbol{a}$ by subtraction, but this is impossible as the matrix equation stands. If we rewrite it however as
$$\begin{pmatrix}
\lambda&0\\0&\lambda
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
1&3\\1&-1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}$$
it now makes sense to subtract the left side from the right. Using the distributive law for matrix multiplication, this becomes
$$\begin{pmatrix}
1-\lambda&3\\1&-1-\lambda
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}$$
which is just the matrix form for $(4)$. The trick therefore was in $(3)$ to replace the scalar $\lambda$ by the diagonal matrix $\lambda I$.
