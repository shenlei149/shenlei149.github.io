**Problem.** Find the general solution to
$$\boldsymbol{u'}=A\boldsymbol{u}, \text{ where } A=\begin{pmatrix}
-2&1\\-4&3
\end{pmatrix}$$
Find the solution with initial conditions $\boldsymbol{u}(0) = (1, 0)^T$. *Throughout, comments are given in italics*.  
**Solution.**  
Step 0. Write down $A-\lambda I$  
*Even if you find the characteristic equation of $A$ using its trace and determinant, you will need this later, for finding eigenvectors. Most students find it useful to write it down clearly at the start of the question.*  
$$A-\lambda I=\begin{pmatrix}
-2-\lambda&1\\-4&3-\lambda
\end{pmatrix}$$
Step 1. Find the characteristic equation of $A$.
*We use the method involving the trace and determinant of $A$*.  
$$\text{tr}A=-2+3=1$$
$$\det{A}=-2 \times 3-1\times (-4)=-2$$
Thus $p_A(\lambda)=\det(A-\lambda I)=\lambda^2-\lambda-2$  
**Step 2.** Find the eigenvalues of $A$.  
*These are the roots of the characteristic equation. We find them by completing the square. We could also have used the quadratic formula or, in this case, simply factored the equation.*  
$$p_A(\lambda)=(\lambda-1/2)^2-9/4$$
The roots are $1/2\plusmn 3/2$, so $\lambda_1=-1$ and $\lambda_2=2$.  
Step 3. Find associated eigenvectors.  
3a. Eigenvector for $\lambda_1$. This is vector $a = (a_1, a_2)$ that must satisfy
$$\begin{aligned}
&(A+I)\boldsymbol{a}=0\\
\lrArr &\begin{pmatrix}
-2+1&1\\-4&3+1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\\
\lrArr &\begin{pmatrix}
-1&1\\-4&4
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\\
\lrArr &\begin{cases}
&&-a_1&+&&a_2&=0\\
&&-4a_1&+&&4a_2&=0
\end{cases}
\end{aligned}$$
Check: one equation is a multiple of the other, as should be the case. This is a good sign. Setting $a_1 = 1$ gives $a_2 = 1$; thus one eigenvector for $\lambda_1$ is $(1, 1)^T$.  
3b. Eigenvector for $\lambda_2$. This is a vector $(a_1, a_2)^T$ that must satisfy:
$$\begin{aligned}
&(A-2I)\boldsymbol{a}=0\\
\lrArr &\begin{pmatrix}
-2-2&1\\-4&3-2
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\\
\lrArr &\begin{pmatrix}
-4&1\\-4&1
\end{pmatrix}\begin{pmatrix}
a_1\\a_2
\end{pmatrix}=\begin{pmatrix}
0\\0
\end{pmatrix}\\
\lrArr &\begin{cases}
&&-4a_1&+&&a_2&=0\\
&&-4a_1&+&&a_2&=0
\end{cases}
\end{aligned}$$
Check: one equation is a (trivial) multiple of the other.  
Setting $a_1 = 1$ gives $a_2 = 4$. Thus, one eigenvector for $\lambda_2$ is $(1, 4)^T$.  
Step 4. Normal modes and general solution  
The normal modes are $e^{-t}\begin{pmatrix}1\\1\end{pmatrix}$ and $e^{2t}\begin{pmatrix}1\\4\end{pmatrix}$.  
and the general solution is:
$$\boldsymbol{u}(t)=c_1e^{-t}\begin{pmatrix}1\\1\end{pmatrix}+c_2e^{2t}\begin{pmatrix}1\\4\end{pmatrix}$$
Step 5. Solution matching IC.  
We solve for $c_1$ and $c_2$ using our initial condition. From our expression for the general solution, $\boldsymbol{u}(0)=c_1(1,1)^T+c_2(1,4)^T=(c_1+c_2,c_1+4c_2)^T$ Thus the initial condition $\boldsymbol{u}(0) = (1, 0)^T$ gives:
$$\begin{cases}
c_1&+&&c_2&=1\\
c_1&+&&4c_2&=0
\end{cases} \lrArr c_2=-1/3, c_1=4/3$$
The solution we were asked for is:
$$\boldsymbol{u}(t)=\frac{4}{3}e^{-t}\begin{pmatrix}1\\1\end{pmatrix}-\frac{1}{3}e^{2t}\begin{pmatrix}1\\4\end{pmatrix}$$
