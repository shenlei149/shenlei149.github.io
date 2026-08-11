### Complex Eigenvalues
In the previous note, we obtained the solutions to a homogeneous linear system with constant coefficients
$$\boldsymbol{x'}=A\boldsymbol{x}$$
under the assumption that the roots of its characteristic equation
$$|A-\lambda I|=0$$
i.e., the eigenvalues of $A$ - were real and distinct.  
In this section we consider what to do if there are complex eigenvalues. Since the characteristic equation has real coefficients, its complex roots must occur in conjugate pairs:
$$\lambda=a+bi, \bar{\lambda}=a-bi$$
Let's start with the eigenvalue $a + bi$. According to the solution method described in the note *Eigenvectors and Eigenvalues*, (from earlier in this session) the next step would be to find the corresponding eigenvector $\boldsymbol{v}$, by solving the equations
$$\begin{aligned}
&&(a-\lambda)a_1+&&ba_2&=0\\
&&ca_1+&&(d-\lambda)a_2&=0
\end{aligned}$$
for its components $a_1$ and $a_2$. Since $\lambda$ is complex, the $a_i$ will also be complex, and therefore the eigenvector $\boldsymbol{v}$ corresponding to $\lambda$ will have complex components. Putting together the eigenvalue and eigenvector gives us formally the complex solution
$$\boldsymbol{x}=e^{(a+bi)t}\boldsymbol{v}\tag{1}$$
Naturally, we want real solutions to the system, since it was real to start with. To get them, the following theorem tells us to just take the real and imaginary parts of $(1)$. (This theorem is exactly analogous to what we did with ordinary differential equations.)  
**Theorem.** Given a system $\boldsymbol{x'} = Ax$, where $A$ is a real matrix. If $\boldsymbol{x} = \boldsymbol{x}_1 + i\boldsymbol{x}_2$ is a complex solution, then its real and imaginary parts $\boldsymbol{x}_1, \boldsymbol{x}_2$ are also solutions to the system.  
**Proof.** Since $\boldsymbol{x}_1 + i\boldsymbol{x}_2$ is a solution, we have
$$(\boldsymbol{x}_1 + i\boldsymbol{x}_2)'=A(\boldsymbol{x}_1 + i\boldsymbol{x}_2)=A\boldsymbol{x}_1 + iA\boldsymbol{x}_2$$
Equating real and imaginary parts of this equation,
$$\boldsymbol{x}_1'=A\boldsymbol{x}_1, \boldsymbol{x}_2'=A\boldsymbol{x}_2$$
which shows exactly that the real vectors $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are solutions to $\boldsymbol{x}'=A\boldsymbol{x}$.  
**Example.** Find the corresponding two real solutions to $\boldsymbol{x}'=A\boldsymbol{x}$ if a complex eigenvalue and corresponding eigenvector are
$$\lambda=-1+2i,\boldsymbol{v}=\begin{pmatrix}
i\\2-2i
\end{pmatrix}$$
**Solution.** First write $\boldsymbol{v}$ in terms of its real and imaginary parts:
$$\boldsymbol{v}=\begin{pmatrix}
0\\2
\end{pmatrix}+i\begin{pmatrix}
1\\-2
\end{pmatrix}$$
The corresponding complex solution $\boldsymbol{x} = e^{\lambda t} \boldsymbol{v}$ to the system can then be written
$$\boldsymbol{x}=e^{-t}(\cos(2t)+i\sin(2t))\bigg(\begin{pmatrix}
0\\2
\end{pmatrix}+i\begin{pmatrix}
1\\-2
\end{pmatrix}\bigg)$$
Now, using the theorem, the real and imaginary parts of $\boldsymbol{x}$ are
$$x_1=e^{-t}\bigg(\begin{pmatrix}
0\\2
\end{pmatrix}\cos(2t)-i\begin{pmatrix}
1\\-2
\end{pmatrix}\sin(2t)\bigg)=e^{-t}\begin{pmatrix}
-\sin(2t)\\2\cos(2t)+2\sin(2t)
\end{pmatrix}$$
$$x_2=e^{-t}\bigg(\begin{pmatrix}
1\\-2
\end{pmatrix}\cos(2t)+i\begin{pmatrix}
0\\2
\end{pmatrix}\sin(2t)\bigg)=e^{-t}\begin{pmatrix}
-\cos(2t)\\-2\cos(2t)+2\sin(2t)
\end{pmatrix}$$
These are two distinct real solutions to the system.  
In general, if the complex eigenvalue is $a + bi$, to get the real solutions to the system, we write the corresponding complex eigenvector $\boldsymbol{v}$ in terms of its real and imaginary part:
$$\boldsymbol{v}=\boldsymbol{v}_1+\boldsymbol{v}_2, \text{ where } \boldsymbol{v}_1,\boldsymbol{v}_2 \text{ are real vectors;}$$
(study carefully in the example above how this is done in practice). Then we substitute into $(1)$ and calculate as in the example:
$$\boldsymbol{x}=e^{at}(\cos(bt)+i\sin(bt))(\boldsymbol{v}_1+i\boldsymbol{v}_2)$$
so that the real and imaginary parts of $\boldsymbol{x}$ give respectively the two real solutions
$$\begin{aligned}
\boldsymbol{x}_1=e^{at}(\boldsymbol{v}_1\cos(bt)-\boldsymbol{v}_2\sin(bt))\\
\boldsymbol{x}_2=e^{at}(\boldsymbol{v}_1\sin(bt)+\boldsymbol{v}_2\cos(bt))\tag{2}
\end{aligned}$$
These solutions are linearly independent: they are two truly different solutions. The general solution is given by their linear combinations
$$c_1\boldsymbol{x}_1+c_2\boldsymbol{x}_2$$
**Remarks**
1. The complex conjugate eigenvalue $a - bi$ gives up to sign the same two solutions $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$.
2. The expression $(2)$ was not written down for you to memorize, learn, or even use; the point was just for you to get some practice in seeing how a calculation like the one in our example looks when written out in general. To actually solve ODE systems having complex eigenvalues, imitate the procedure in the following example.

### Worked Example
**Problem.** Solve $\boldsymbol{u'} = A\boldsymbol{u}$, where $A=\begin{pmatrix}1&2\\-2&1\end{pmatrix}$.  
*Comments are given in italics; the steps initially follow those in section worked example: real distinct eigenvalues, then diverge.*  
**Solution.**  
Step 0. Write down $A-\lambda I$: $A-\lambda I=\begin{pmatrix}1-\lambda&2\\-2&1-\lambda\end{pmatrix}$
Step 1. Find the characteristic equation of $A$.  
We use the method involving the trace and determinant of $A$.
$$\text{tr}(A)=1+1=2,\det{A}=1+4=5$$
Thus
$$p_A(\lambda)=\det{(A-\lambda I)}=\lambda^2-\text{tr}(A)\lambda+\det{A}=\lambda^2-2\lambda+5$$
Step 2. Find the eigenvalues of $A$.  
We complete the square.
$$p_A(\lambda)=\lambda^2-2\lambda+5=(\lambda-1)^2+4$$
The roots
$$\lambda_1=1+2i$$
$$\lambda_2=1-2i$$
Step 3. Find the eigenvector associated to one eigenvalue  
*The eigenvalues are complex, so we'll only need one eigenvector.*  
We look for the eigenvector
$$\begin{pmatrix}
a_1\\a_2
\end{pmatrix} \text{ for } \lambda_1=1+2i$$
It must satisfy:
$$\begin{aligned}
&(A-(1+2i)I)\boldsymbol{a}=0\\
\lrArr &\begin{pmatrix}
-2i&2\\-2&-2i
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=0\\
\lrArr &\begin{cases}
&&-2ia_1&+&&2a_2&=0\\
&&-2a_1&-&&2ia_2&=0
\end{cases}
\end{aligned}$$
*You should check that these two equations are equivalent.*  
This gives $a_1 + ia_2 = 0$. Pick $a_1 = 1$, this implies $a_2 = i$. Thus an eigenvec­tor for $\lambda_1$ is $\boldsymbol{v}_1=\begin{pmatrix}1\\i\end{pmatrix}$  
Step 4. Find the real and imaginary parts of solution associated to $\lambda_1$  
The solution we associated to $\lambda_1$ is
$$e^{\lambda_1 t}\boldsymbol{v}_1=e^{(1+2i)t}\begin{pmatrix}
1\\i
\end{pmatrix}=e^t(\cos(2t)+i\sin(2t))\begin{pmatrix}
1\\i
\end{pmatrix}$$
This has real and imaginary parts:
$$\boldsymbol{x}_1=e^t\begin{pmatrix}
\cos(2t)\\-\sin(2t)
\end{pmatrix},\boldsymbol{x}_2=e^t\begin{pmatrix}
\sin(2t)\\\cos(2t)
\end{pmatrix}$$
If you are confused by steps 3 or 4, you should read over the note again.  
Step 5. General solution.
$$c_1e^t\begin{pmatrix}
\cos(2t)\\-\sin(2t)
\end{pmatrix}+c_2e^t\begin{pmatrix}
\sin(2t)\\\cos(2t)
\end{pmatrix}$$
