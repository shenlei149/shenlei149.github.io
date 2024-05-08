In the previous note we saw two main facts about the fundamental matrix:
$$\Phi(t)=\begin{pmatrix}
\boldsymbol{x}_1\\\boldsymbol{x}_2
\end{pmatrix}=\begin{pmatrix}
x_1&x_2\\y_1&y_2
\end{pmatrix}\tag{1}$$
and
$$\boldsymbol{x}=\Phi(t)\Phi(t_0)^{-1}\boldsymbol{x}_0\tag{2}$$
Is there a "best" choice for fundamental matrix?  
There are two common choices, each with its advantages. If the ODE system has constant coefficients, and its eigenvalues are real and distinct, then a natural choice for the fundamental matrix would be the one whose columns are the normal modes - the solutions of the form
$$\boldsymbol{x}_i=\overrightarrow{\alpha_i}e^{\lambda_i t}, i=1,2$$
There is another choice however which is suggested by $(2)$ and which is particularly useful in showing how the solution depends on the initial conditions. Suppose we pick $\Phi(t)$ so that
$$\Phi(t_0)=I=\begin{pmatrix}
1&0\\0&1
\end{pmatrix}\tag{3}$$
Referring to the definition $(1)$, this means the solutions $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are picked so
$$\boldsymbol{x}_1(t_0)=\begin{pmatrix}
1\\0
\end{pmatrix},\boldsymbol{x}_2(t_0)=\begin{pmatrix}
0\\1
\end{pmatrix}\tag{3'}$$
Since the $\boldsymbol{x}_i(t)$ are uniquely determined by these initial conditions, the fundamental matrix $\Phi(t)$ satisfying $(3)$ is also unique; we give it a name.  
**Definition 2** The unique matrix $\widetilde{\Phi}_{t_0}(t)$ satisfying
$$\widetilde{\Phi}_{t_0}'=A\widetilde{\Phi}_{t_0}, \widetilde{\Phi}_{t_0}(t_0)=I$$
is called the **normalized fundamental matrix** at $t_0$ for $A$.  
For convenience in use, the definition uses Theorem 1 to guarantee $\widetilde{\Phi}_{t_0}$ will actually be a fundamental matrix. The condition $|\widetilde{\Phi}_{t_0}(t)| \neq 0$ in Theorem 1 is satisfied, since thedefinition implies $|\widetilde{\Phi}_{t_0}(t_0)| = 1$.  
To keep the notation simple, we will assume in the rest of this section that $t_0 = 0$, as it almost always is; then $\widetilde{\Phi}_0$ is the normalized fundamental matrix. Since $\widetilde{\Phi}_0(0) = I$, we get from $(2)$ the matrix form for the solution to the IVP: $\boldsymbol{x'} = A(t) \boldsymbol{x}, \boldsymbol{x}(0) = \boldsymbol{x}_0$ is
$$\boldsymbol{x}(t)=\widetilde{\Phi}_0(t)\boldsymbol{x}_0\tag{5}$$
**Calculating $\widetilde{\Phi}_0$.** One way is to find the two solutions in $(3')$ and use them as the columns of $\widetilde{\Phi}_0$. This is fine if the two solutions can be determined by inspection.  
If not, a simpler method is this: find any fundamental matrix $\Phi(t)$; then
$$\widetilde{\Phi}_0(t)=\Phi(t)\Phi(0)^{-1}\tag{6}$$
To verify this, we have to see that the matrix on the right of $(6)$ satisfies the two conditions in Definition 2. The second is trivial. The first is easy using the rule for matrix differentiation:  
If $M = M(t)$ and $B, C$ are constant matrices, then $(BM)' = BM', (MC)' = M'C$  
from which we see that since $\Phi$ is a fundamental matrix,
$$(\Phi(t)\Phi(0)^{-1})'=\Phi(t)'\Phi(0)^{-1}=A\Phi(t)\Phi(0)^{-1}=A(\Phi(t)\Phi(0)^{-1})$$
showing that $\Phi(t)\Phi(0)^{-1}$ also satisfies the first condition in Definition 2.
**Example 2A.** Find the solution to the IVP: $\boldsymbol{x'}=\begin{pmatrix}0&1\\-1&0\end{pmatrix}\boldsymbol{x}, \boldsymbol{x}(0)=\boldsymbol{x}_0$.  
**Solution.** Since the system is $x' = y, y' = -x$, we can find by inspection the fundamental set of solutions satisfying $(3')$:
$$\begin{aligned}
x&=&\cos t\\y&=&-\sin t
\end{aligned} \text{ and } \begin{aligned}
x&=&\sin t\\y&=&\cos t
\end{aligned}$$
Thus by $(5)$ the normalized fundamental matrix at 0 and solution to the IVP is
$$\boldsymbol{x}=\widetilde{\Phi}\boldsymbol{x}_0=\begin{pmatrix}
\cos t&\sin t\\-\sin t&\cos t
\end{pmatrix}\begin{pmatrix}
x_0\\y_0
\end{pmatrix}=x_0\begin{pmatrix}
\cos t\\-\sin t
\end{pmatrix}+y_0\begin{pmatrix}
\sin t\\\cos t
\end{pmatrix}$$
**Example 2B.** Give the normalized fundamental matrix at 0 for $\boldsymbol{x'}=\begin{pmatrix}1&3\\1&1\end{pmatrix}\boldsymbol{x}$.  
**Solution.** This time the solutions $(3')$ cannot be obtained by inspection, so we use the second method. You can easily find the eigenvalues and eigenvectors for this system. Doing so produces the normal modes. Using them as the columns of a fundamental matrix gives us
$$\Phi(t)=\begin{pmatrix}
3e^{2t}&-e^{-2t}\\
e^{2t}&e^{-2t}
\end{pmatrix}$$
Using $(6)$ and the formula for calculating the inverse matrix we get
$$\Phi(0)=\begin{pmatrix}
3&-1\\1&1
\end{pmatrix}$$
$$\Phi(0)^{-1}=\frac{1}{4}\begin{pmatrix}
1&1\\-1&3
\end{pmatrix}$$
so that
$$\widetilde{\Phi}(t)=\frac{1}{4}\begin{pmatrix}
3e^{2t}&-e^{-2t}\\
e^{2t}&e^{-2t}
\end{pmatrix}\begin{pmatrix}
1&1\\-1&3
\end{pmatrix}$$
