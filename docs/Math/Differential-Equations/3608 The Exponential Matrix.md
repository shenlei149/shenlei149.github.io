The work in the preceding note with fundamental matrices was valid for any linear homogeneous square system of ODE's,
$$\boldsymbol{x'}=A\boldsymbol{x}$$
However, if the system has *constant coefficients*, i.e., the matrix $A$ is a constant matrix, the results are usually expressed by using the exponential matrix, which we now define.  
Recall that if $x$ is any real number, then
$$e^x=1+x+\frac{x^2}{2!}+\ldots+\frac{x^n}{n!}+\ldots\tag{1}$$
**Definition 3** Given an $n \times n$ constant matrix $A$, the exponential matrix $e^A$ is the $n \times n$ matrix defined by
$$e^A=I+A+\frac{A^2}{2!}+\ldots+\frac{A^n}{n!}+\ldots\tag{2}$$
Each term on the right side of $(2)$ is an $n \times n$ matrix adding up the $ij$-th entry of each of these matrices gives you an infinite series whose sum is the $ij$-th entry of $e^A$. (The series always converges.)  
In the applications, an independent variable $t$ is usually included:
$$e^{At}=I+At+A^2\frac{t^2}{2!}+\ldots+A^n\frac{t^n}{n!}+\ldots\tag{3}$$
This is not a new definition, it's just $(2)$ above applied to the matrix $At$ in which every element of $A$ has been multiplied by $t$, since for example
$$(At)^2=At\cdot At=A\cdot A \cdot t^2=A^2t^2$$
Try out $(2)$ and $(3)$ on these two examples (the second is very easy, since it is not an infinite series)

**Example 3A.** Let $A = \begin{pmatrix}a&0\\0&b\end{pmatrix}$. Show: $e^A=\begin{pmatrix}e^a&0\\0&e^b\end{pmatrix}$; and $e^{At}=\begin{pmatrix}e^{at}&0\\0&e^{bt}\end{pmatrix}$  
**Solution.**
$$A=\begin{pmatrix}a&0\\0&b\end{pmatrix}$$
$$A^2=\begin{pmatrix}a^2&0\\0&b^2\end{pmatrix}$$
$$A^n=A^{n-1}\begin{pmatrix}a&0\\0&b\end{pmatrix}=\begin{pmatrix}
a^{n-1}&0\\0&b^{n-1}
\end{pmatrix}\begin{pmatrix}a&0\\0&b\end{pmatrix}=\begin{pmatrix}a^n&0\\0&b^n\end{pmatrix}$$
$$\begin{aligned}
e^A&=I+A+\frac{A^2}{2!}+\ldots+\frac{A^n}{n!}+\ldots\\
&=\begin{pmatrix}
1&0\\0&1
\end{pmatrix}+\begin{pmatrix}a&0\\0&b\end{pmatrix}+\frac{1}{2!}\begin{pmatrix}a^2&0\\0&b^2\end{pmatrix}+\ldots+\frac{1}{n!}\begin{pmatrix}a^n&0\\0&b^n\end{pmatrix}+\ldots\\
&=\begin{pmatrix}
1+a+\frac{1}{2!}a^2+\ldots+\frac{1}{n1}a^n+\ldots&0\\
0&1+b+\frac{1}{2!}b^2+\ldots+\frac{1}{n!}b^n+\ldots
\end{pmatrix}\\
&=\begin{pmatrix}
e^a&0\\0&e^b
\end{pmatrix}
\end{aligned}$$
$e^{At}$ is the same.

**Example 3B.** Let $A=\begin{pmatrix}0&1\\0&0\end{pmatrix}$. Show: $e^A=\begin{pmatrix}1&1\\0&1\end{pmatrix}$ and $e^{At}=\begin{pmatrix}1&t\\0&1\end{pmatrix}$.  
**Solution.**
$$A=\begin{pmatrix}0&1\\0&0\end{pmatrix}$$
$$A^2=\begin{pmatrix}0&0\\0&0\end{pmatrix}$$
$$\begin{aligned}
e^A&=I+A+\frac{A^2}{2!}+\ldots+\frac{A^n}{n!}+\ldots\\
&=\begin{pmatrix}
1&0\\0&1
\end{pmatrix}+\begin{pmatrix}0&1\\0&0\end{pmatrix}\\
&=\begin{pmatrix}1&1\\0&1\end{pmatrix}
\end{aligned}$$
$$\begin{aligned}
e^{At}&=I+At+A^2\frac{t^2}{2!}+\ldots+A^n\frac{t^n}{n!}+\ldots\\
&=\begin{pmatrix}
1&0\\0&1
\end{pmatrix}+\begin{pmatrix}
0&t\\0&0
\end{pmatrix}\\
&=\begin{pmatrix}1&t\\0&1\end{pmatrix}
\end{aligned}$$

What's the point of the exponential matrix? The answer is given by the theorem below, which says that the exponential matrix provides a royal road to the solution of a square system with constant coefficients: no eigenvectors, no eigenvalues, you just write down the answer!  
**Theorem 3** Let $A$ be a square constant matrix. Then  
(a) $e^{At}=\widetilde{\Phi}_0(t)$ the normalized fundamental matrix at 0;  
(b) the unique solution to the IVP $\boldsymbol{x'}=A\boldsymbol{x},\boldsymbol{x}(0)=\boldsymbol{x}_0$ is $\boldsymbol{x}=e^{At}\boldsymbol{x}_0$.  
**Proof.** Recall that in the previous note we saw that if $\widetilde{\Phi}'_0(t)$ is the normalized fundamental matrix then  
The solution to the IVP:
$$\boldsymbol{x'}=A(t)\boldsymbol{x},\boldsymbol{x}(0)=\boldsymbol{x}_0$$
is
$$\boldsymbol{x}(t)=\widetilde{\Phi}_0(t)\boldsymbol{x}_0\tag{4}$$
Statement (b) follows immediately from $(1)$, in view of $(4)$.  
We prove (a) is true by using the fact that if $t_0 = 0$ then the normalized fundamental matrix has $\Phi(0) = I$. Letting $\Phi = e^{At}$, we must show $\Phi' = A\Phi$ and $\Phi(0) = I$.  
The second of these follows from substituting $t = 0$ into the infinite series definition $(3)$ for $e^{At}$.  
To show $\Phi' = A\Phi$, we assume that we can differentiate the series $(3)$ term-by-term; then we have for the individual terms
$$\frac{d}{dt}A^n\frac{t^n}{n!}=A^n\frac{t^{n-1}}{(n-1)!}$$
since $A^n$ is a constant matrix. Differentiating $(3)$ term-by-term then gives
$$\frac{d\Phi}{dt}=\frac{d}{dt}e^{At}=A+A^2t+\ldots+A^n\frac{t^{n-1}}{(n-1)!}+\ldots=Ae^{At}=A\Phi\tag{5}$$

### Calculation of $e^{At}$
The main use of the exponential matrix is in Theorem 3 - writing down explicitly the solution to an IVP. If $e^{At}$ has to be calculated for a specific system, several techniques are available.  
a) In simple cases, it can be calculated directly as an infinite series of matrices.  
b) It can always be calculated, according to Theorem 3, as the normalized fundamental matrix $\widetilde{\Phi}_0(t)$, using: $\widetilde{\Phi}_0(t) = \Phi(t)\Phi(0)^{-1}$.  
c) A third technique uses the exponential law
$$e^{(B+C)t}=e^{Bt}+e^{Ct}, \text{ valid if } BC=CB\tag{6}$$
To use it, one looks for constant matrices $B$ and $C$ such that
$$A = B + C, BC = CB, e^{Bt} \text{ and } e^{Ct} \text{ are computable}\tag{7}$$
then
$$e^{At}=e^{(B+C)t}=e^{Bt}+e^{Ct}\tag{8}$$
**Example 3C.** Let $A=\begin{pmatrix}2&1\\0&2\end{pmatrix}$. Solve $\boldsymbol{x'}=A\boldsymbol{x}, \boldsymbol{x}(0)=\begin{pmatrix}1\\2\end{pmatrix}$, using $e^{At}$.  
**Solution.** We set $B=\begin{pmatrix}2&0\\0&2\end{pmatrix}$ and $C=\begin{pmatrix}0&1\\0&0\end{pmatrix}$ then $(7)$ is satisfied, and
$$e^{At}=\begin{pmatrix}
e^{2t}&0\\0&e^{2t}
\end{pmatrix}\begin{pmatrix}
1&t\\0&1
\end{pmatrix}=e^{2t}\begin{pmatrix}
1&t\\0&1
\end{pmatrix}$$
by $(8)$ and Examples 3A and 3B.  
Therefore, by Theorem 3, we get
$$\boldsymbol{x}=e^{At}\boldsymbol{x}_0=e^{2t}\begin{pmatrix}
1&t\\0&1
\end{pmatrix}\begin{pmatrix}
1\\2
\end{pmatrix}=e^{2t}\begin{pmatrix}
1+2t\\2
\end{pmatrix}$$
