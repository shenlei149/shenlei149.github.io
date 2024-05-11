We have studied the homogeneous system of ODE's with constant coefficients,
$$\boldsymbol{x'}=A\boldsymbol{x}\tag{1}$$
where $A$ is an $n \times n$ matrix of constants ($n = 2, 3$). We described how to calculate the eigenvalues and corresponding eigenvectors for the matrix $A$, and how to use them to find $n$ independent solutions to the system $(1)$.  
With this concrete experience in solving low-order systems with constant coefficients, what can be said when the coefficients are functions of the independent variable $t$? We can still write the linear system in the matrix form $(1)$, but now the matrix entries will be functions of $t$:
$$\begin{aligned}
x'=a(t)x+b(t)y\\
y'=c(t)x+d(t)y
\end{aligned},\begin{pmatrix}
x'\\y'
\end{pmatrix}=\begin{pmatrix}
a(t)&b(t)\\c(t)&d(t)
\end{pmatrix}\begin{pmatrix}
x\\y
\end{pmatrix}\tag{2}$$
or in more abridged notation, valid for $n \times n$ linear homogeneous systems,
$$\boldsymbol{x'}=A(t)\boldsymbol{x}\tag{3}$$
Note how the matrix becomes a function of $t$ - we call it a *matrix-valued function* of $t$, since to each value of $t$ the function rule assigns a matrix:
$$t_0 \rarr A(t_0)=\begin{pmatrix}
a(t_0)&b(t_0)\\c(t_0)&d(t_0)
\end{pmatrix}$$
In the rest of this chapter we will often not write the variable $t$ explicitly, but it is always understood that the matrix entries are functions of $t$.  
We will sometimes use $n = 2$ or $3$ in the statements and examples in order to simplify the exposition, but the definitions, results, and the arguments which prove them are essentially the same for higher values of $n$.  
**Definition 1** Solutions $\boldsymbol{x}_1(t), \ldots, \boldsymbol{x}_n(t)$ to $(3)$ are called linearly dependent if there are constants $c_i$, not all of which are 0, such that
$$c_1\boldsymbol{x}_1(t)+\ldots+c_n\boldsymbol{x}_n(t)=0, \text{ for all } t\tag{4}$$
If there is no such relation, i.e., if
$$c_1\boldsymbol{x}_1(t)+\ldots+c_n\boldsymbol{x}_n(t)=0, \text{ for all } t \rArr \text{ all } c_i=0 \tag{5}$$
the solutions are called **linearly independent**, or simply *independent*.  
The phrase for all $t$ is often in practice omitted, as being understood. This can lead to ambiguity. To avoid it, we will use the symbol $\equiv 0$ for identically 0, meaning zero for all $t$; the symbol $\not\equiv 0$ means not identically 0, i.e., there is some $t$-value for which it is not zero. For example, $(4)$ would be written 
$$c_1\boldsymbol{x}_1(t)+\ldots+c_n\boldsymbol{x}_n(t)\equiv 0$$
**Theorem 1** If $\boldsymbol{x}_1, \ldots, \boldsymbol{x}_n$ is a linearly independent set of solutions to the $n \times n$ system $\boldsymbol{x'} = A(t)\boldsymbol{x}$, then the general solution to the system is
$$\boldsymbol{x}=c_1\boldsymbol{x}_1+\ldots+c_n\boldsymbol{x}_n\tag{6}$$
Such a linearly independent set is called a **fundamental** set of solutions.  
This theorem is the reason for expending so much effort to find two independent solutions, when $n = 2$ and $A$ is a constant matrix. In this chapter, the matrix $A$ is not constant; nevertheless, $(6)$ is still true.  
**Proof.** There are two things to prove:  
(a) All vector functions of the form $(6)$ really are solutions to $\boldsymbol{x'} = A\boldsymbol{x}$.  
This is the *superposition principle* for solutions of the system; it's true because the system is *linear*. The matrix notation makes it really easy to prove. We have
$$\begin{aligned}
&(c_1\boldsymbol{x}_1+\ldots+c_n\boldsymbol{x}_n)'\\
=&c_1\boldsymbol{x}_1'+\ldots+c_n\boldsymbol{x}_n'\\
=&c_1A_1\boldsymbol{x}_1+\ldots+c_nA_n\boldsymbol{x}_n\\
=&A(c_1\boldsymbol{x}_1+\ldots+c_n\boldsymbol{x}_n)
\end{aligned}$$
(b) All solutions to the system are of the form $(6)$.  
This is harder to prove and will be the main result of the next note.
