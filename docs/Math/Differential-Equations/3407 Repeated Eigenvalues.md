### Repeated Eigenvalues
Again, we start with the real $2 \times 2$ system
$$\boldsymbol{x'}=A\boldsymbol{x}\tag{1}$$
We say an eigenvalue $\lambda_1$ of $A$ is repeated if it is a multiple root of the characteristic equation of $A$; in our case, as this is a quadratic equation, the only possible case is when $\lambda_1$ is a double real root.  
We need to find two linearly independent solutions to the system $(1)$. We can get one solution in the usual way. Let $\boldsymbol{v}_1$ be an eigenvector corresponding to $\boldsymbol{\lambda}_1$. This is found by solving the system
$$(A-\lambda_1 I)\boldsymbol{a}=0\tag{2}$$
This gives the solution $\boldsymbol{x}_1 = e^{\lambda_1t}\boldsymbol{v}_1$ to the system $(1)$. Our problem is to find a second solution. To do this we have to distinguish two cases, called complete and defective. The first one is easier, especially in the $2 \times 2$ case.  
**A. The complete case.**  
Still assuming $\lambda_1$ is a real double root of the characteristic equation of $A$, we say $\lambda_1$ is a **complete** eigenvalue if there are two linearly independent eigenvectors $\boldsymbol{v}_1$ and $\boldsymbol{v}_2$ corresponding to $\lambda_1$; i.e., if these two vectors are two linearly independent solutions to the system $(2)$.  
In the $2 \times 2$ case, this only occurs when $A$ is a *scalar matrix* that is, when $A = \lambda_1 I$. In this case, $A - \lambda_1 I=0$, and every vector is an eigenvector. It is easy to find two independent solutions; the usual choices are
$$e^{\lambda_1 t}\begin{pmatrix}
1\\0
\end{pmatrix} \text{ and } e^{\lambda_1 t}\begin{pmatrix}
0\\1
\end{pmatrix}$$
So the general solution is
$$c_1e^{\lambda_1 t}\begin{pmatrix}
1\\0
\end{pmatrix} + c_2e^{\lambda_1 t}\begin{pmatrix}
0\\1
\end{pmatrix}=e^{\lambda_1 t}\begin{pmatrix}
c_1\\c_2
\end{pmatrix}$$
Of course, we could choose any other pair of independent eigenvectors to generate the solutions, e.g.,
$$e^{\lambda_1 t}\begin{pmatrix}
5\\1
\end{pmatrix} \text{ and } e^{\lambda_1 t}\begin{pmatrix}
1\\-1
\end{pmatrix}$$
**Remark.** For $n = 3$ and above the situation is more complicated. We will not discuss it here. The interested reader can consult, for instance, the textbook by Edwards and Penney.  
**B. The defective case.**  
If the eigenvalue $\lambda$ is a double root of the characteristic equation, but the system $(2)$ has only one non-zero solution $\boldsymbol{v}_1$ (up to constant multiples), then the eigenvalue is said to be **incomplete** or **defective** and $\boldsymbol{x}_1 = e^{\lambda_1t}\boldsymbol{v}_1$ is the unique normal mode. However, a second order system needs two independent solutions. Our experience with repeated roots in second order ODE's suggests we try multiplying our normal solution by $t$. It turns out this doesn't quite work, but it can be fixed as follows: a second independent solution is given by
$$\boldsymbol{x}_2=e^{\lambda_1t}(t\boldsymbol{v}_1+\boldsymbol{v}_2)\tag{3}$$
where $\boldsymbol{v}_2$ is any vector satisfying
$$(A-\lambda_1 I)\boldsymbol{v}_2=\boldsymbol{v}_1$$
**Fact.** The equation for $\boldsymbol{v}_2$ is guaranteed to have a solution, provided that the eigenvalue $\lambda_1$ really is defective. When solving for $\boldsymbol{v}_2 = (b_1, b_2)^T$, try setting $b_1 = 0$, and solving for $b_2$. If that does not work, try setting $b_2 = 0$ and solving for $b_1$.  
**Remarks**
1. Some people do not bother with $(3)$. When they encounter the defective case (at least when $n = 2$), they give up on eigenvalues, and simply solve the original system $(1)$ by elimination.
2. Although we will not go into it in this course, there is a well developed theory of defective matrices which gives insight into where this formula comes from. You will learn about all this when you study linear algebra.

### Worked example: Defective Repeated Eigenvalue
**Problem.** Solve $\boldsymbol{u'}=A\boldsymbol{u}$, where $A=\begin{pmatrix}-2&1\\-1&0\end{pmatrix}$.  
*Comments are given in italics.*  
**Solution.**  
Step 0. Write down $A-\lambda I$: $A-\lambda I=\begin{pmatrix}-2-\lambda&1\\-1&-\lambda\end{pmatrix}$  
Step 1. Find the characteristic equation of $A$:
$$\text{tr}(A)=-2+0=-2,\det{(A)}=0-(-1)=1$$
Thus
$$p_A(\lambda)=\det{(A-\lambda I)}=\lambda^2-\text{tr}(A)\lambda+\det{(A)}=\lambda^2-2\lambda+1$$
Step 2. Find the eigenvalues of $A$.  
The characteristic polynomial factors: $p_A(\lambda) = (\lambda + 1)^2$. This has a repeated root, $\lambda_1 = -1$.  
As the matrix $A$ is not the identity matrix, we must be in the defective repeated root case.  
Step 3. Find an eigenvector.  
This is vector $\boldsymbol{v}_1 = (a_1, a_2)^T$ that must satisfy:
$$\begin{aligned}
&(A+I)\boldsymbol{v}_1=0\\
\lrArr &\begin{pmatrix}
-2+1&1\\
-1&1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\\
\lrArr &\begin{pmatrix}
-1&1\\-1&1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}
\end{aligned}$$
*Check: this gives two identical equations, which is good.*  
The equation is $-a_1 + a_2 = 0$. Setting $a_1 = 1$ gives $a_2 = 1$. Thus, one eigenvector for $\lambda_1$ is $\boldsymbol{v}_1 = (1, 1)^T$. All other eigenvectors for $\lambda_1$ are multiples of this.  
Step 4. Find $\boldsymbol{v}_2$: This vector $\boldsymbol{v}_2 = (b_1, b_2)^T$ must satisfy
$$\begin{aligned}
&(A-\lambda_1 I)\boldsymbol{v}_2=\boldsymbol{v}_1\\
\lrArr &\begin{pmatrix}
-1&1\\-1&1
\end{pmatrix}\begin{pmatrix}
b_1\\b_2
\end{pmatrix}=\begin{pmatrix}
1\\1
\end{pmatrix}\\
\lrArr &-b_1+b_2=1
\end{aligned}$$
Setting $b_1 = 0$ gives $b_2 = 1$; so $\boldsymbol{v}_2 = (0, 1)^T$ is suitable.  
Step 5. General solution.  
The general solution is
$$\boldsymbol{u}(t)=c_1e^{-t}\begin{pmatrix}
1\\1
\end{pmatrix}+c_2\bigg(te^{-t}\begin{pmatrix}
1\\1
\end{pmatrix}+e^{-t}\begin{pmatrix}
0\\1
\end{pmatrix}\bigg)=e^{-t}\bigg(c_1\begin{pmatrix}
1\\1
\end{pmatrix}+c_2\begin{pmatrix}
t\\t+1
\end{pmatrix}\bigg)$$
