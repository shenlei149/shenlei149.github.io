In the literature, solutions to linear systems often are expressed using square matrices rather than vectors. This is an elegant bookkeeping technique and a very compact, efficient way to express these formulas. As before, we state the definitions and results for a $2 \times 2$ system, but they generalize immediately to $n \times n$ systems.  
We return to the system
$$\boldsymbol{x}'=A(t)\boldsymbol{x}\tag{1}$$
with the general solution
$$\boldsymbol{x}=c_1\boldsymbol{x}_1(t)+c_2\boldsymbol{x}(t)_2\tag{2}$$
where $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are two independent solutions to $(1)$, and $c_1$ and $c_2$ are arbitrary constants.
We form the matrix whose columns are the solutions $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$:
$$\Phi(t)=\begin{pmatrix}
\boldsymbol{x}_1\\\boldsymbol{x}_2
\end{pmatrix}=\begin{pmatrix}
x_1&x_2\\y_1&y_2
\end{pmatrix}\tag{3}$$
Since the solutions are linearly independent, we called them a fundamental set of solutions, and therefore we call the matrix in $(3)$ a **fundamental matrix** for the system $(1)$.  
**Writing the general solution using** $\Phi(t)$. As a first application of $\Phi(t)$, we can use it to write the general solution $(2)$ efficiently. For according to $(2)$, it is
$$\boldsymbol{x}=c_1\begin{pmatrix}
x_1\\y_1
\end{pmatrix}+c_2\begin{pmatrix}
x_2\\y_2
\end{pmatrix}=\begin{pmatrix}
x_1&x_2\\y_1&y_2
\end{pmatrix}\begin{pmatrix}
c_1\\c_2
\end{pmatrix}$$
which becomes using the fundamental matrix
$$\boldsymbol{x}=\Phi\boldsymbol{c}, \text{ where }\boldsymbol{c}=\begin{pmatrix}
c_1\\c_2
\end{pmatrix}\tag{4}$$
Note that the vector $\boldsymbol{c}$ must be written on the right, even though the $c$'s
are usually written on the left when they are the coefficients of the solutions $\boldsymbol{x}_i$.  
Solving the IVP using $\Phi(t)$. We can now write down the solution to the IVP
$$\boldsymbol{x'}=A(t)\boldsymbol{x}, \boldsymbol{x}(t_0)=\boldsymbol{x}_0\tag{5}$$
Starting from the general solution $(4)$, we have to choose the $\boldsymbol{c}$ so that the initial condition in $(6)$ is satisfied. Substituting $t_0$ into $(5)$ gives us the matrix equation for $\boldsymbol{c}$:
$$\Phi(t_0)\boldsymbol{c}=\boldsymbol{x}_0$$
Since the determinant $|\Phi(t_0)|$ is the value at $t_0$ of the Wronskian of $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$, it is non-zero since the two solutions are linearly independent (Theorem 3 in the note on the Wronskian). Therefore the inverse matrix exists and the matrix equation above can be solved for $\boldsymbol{c}$:
$$\boldsymbol{c}=\Phi(t_0)^{-1}\boldsymbol{x}_0$$
Using the above value of $\boldsymbol{c}$ in $(4)$, the solution to the IVP $(1)$ can now be written
$$\boldsymbol{x}=\Phi(t)\Phi(t_0)^{-1}\boldsymbol{x}_0$$
Note that when the solution is written in this form, it's "obvious" that $\boldsymbol{x}(t_0) = \boldsymbol{x}_0$, i.e., that the initial condition in $(5)$ is satisfied.  
**An equation for fundamental matrices** We have been saying "a" rather than "the" fundamental matrix since the system $(1)$ doesn't have a unique fundamental matrix: there are many ways to pick two independent solutions of $\boldsymbol{x'} = A\boldsymbol{x}$ to form the columns of $\Phi$. It is therefore useful to have a way of recognizing a fundamental matrix when you see one. The following theorem is good for this; we'll need it shortly.  
**Theorem 1** $\Phi(t)$ is a fundamental matrix for the system $(1)$ if its determinant $|\Phi(t)|$ is non-zero and it satisfies the matrix equation
$$\Phi'=A\Phi\tag{7}$$
where $\Phi'$ means that each entry of $\Phi$ has been differentiated.  
**Proof.** Since $|\Phi| \not\equiv 0$, its columns $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are linearly independent, as we saw in the previous note. Let $\Phi=\begin{pmatrix}\boldsymbol{x}_1\\\boldsymbol{x}_2\end{pmatrix}$. According to the rules for matrix multiplication $(7)$ becomes
$$\begin{pmatrix}
\boldsymbol{x}_1'\\\boldsymbol{x}_2'
\end{pmatrix}=A\begin{pmatrix}
\boldsymbol{x}_1\\\boldsymbol{x}_2
\end{pmatrix}=\begin{pmatrix}
A\boldsymbol{x}_1\\A\boldsymbol{x}_2
\end{pmatrix}$$
which shows that
$$\boldsymbol{x}_1'=A\boldsymbol{x}_1 \text{ and } \boldsymbol{x}_2'=A\boldsymbol{x}_2$$
this last line says that $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are solutions to the system $(1)$.
