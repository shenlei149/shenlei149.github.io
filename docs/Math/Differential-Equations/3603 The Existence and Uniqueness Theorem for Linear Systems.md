For simplicity, we stick with $n = 2$, but the results here are true for all $n$. There are two questions about the following general linear system that we need to consider.
$$\begin{aligned}
x'=a(t)x+b(t)y\\
y'=c(t)x+d(t)y
\end{aligned}\text{ ; in matrix form, } \begin{pmatrix}
x\\y
\end{pmatrix}'=\begin{pmatrix}
a(t)&b(t)\\c(t)&d(t)
\end{pmatrix}\begin{pmatrix}
x\\y
\end{pmatrix}\tag{1}$$
The first is from the previous section: to show that all solutions are of the form
$$\boldsymbol{x}=c_1\boldsymbol{x}_1+c_2\boldsymbol{x}_2$$
where the $\boldsymbol{x}_i$ form a fundamental set, that is, no $\boldsymbol{x}_i$ is a constant multiple of the other). (The fact that we can write down *all* solutions to a linear system in this way is one of the main reasons why such systems are so important.)  
An even more basic question for the system $(1)$ is: how do we know that it *has* two linearly independent solutions? For systems with a constant coefficient matrix $A$, we showed in the previous chapters how to solve them explicitly to get two independent solutions. But the general non-constant linear system $(1)$ does not have solutions given by explicit formulas or procedures.  
The answers to these questions are based on following theorem.

### Theorem 2 Existence and uniqueness theorem for linear systems.
If the entries of the square matrix $A(t)$ are continuous on an open interval $I$ containing $t_0$, then the initial value problem
$$\boldsymbol{x'}=A(t)\boldsymbol{x}, \boldsymbol{x}(t_0)=\boldsymbol{x}_0\tag{2}$$
has *one and only one* solution $\boldsymbol{x}(t)$ on the interval $I$.  
The proof is difficult and we shall not attempt it. More important is to see how it is used. The following three theorems answer the questions posed for the $2 \times 2$ system $(1)$. They are true for $n > 2$ as well, and the proofs are analogous.  
In the following theorems, *we assume the entries of $A(t)$ are continuous on an open interval $I$*. Here the conclusions are valid on the interval $I$, for example, $I$ could be the whole $t$-axis.

### Theorem 2A Linear independence theorem
Let $\boldsymbol{x}_1(t)$ and $\boldsymbol{x}_2(t)$ be two solutions to $(1)$ on the interval $I$, such that at some point $t_0$ in $I$, the vectors $\boldsymbol{x}_1(t0)$ and $\boldsymbol{x}_2(t0)$ are linearly independent. Then  
a) the solutions $\boldsymbol{x}_1(t)$ and $\boldsymbol{x}_2(t)$ are linearly independent on $I$, and  
b) the vectors $\boldsymbol{x}_1(t_1)$ and $\boldsymbol{x}_2(t_1)$ are linearly independent at every point $t_1$ of $I$.  
**Proof.** a) By contradiction. If they were dependent on $I$, one would be a constant multiple of the other, say $\boldsymbol{x}_2(t) = c_1\boldsymbol{x}_1(t)$. Then $\boldsymbol{x}_2(t_0) = c_1\boldsymbol{x}_1(t_0)$, showing them dependent at $t_0$.  
b) By contradiction. If there were a point $t_1$ on $I$ where they were dependent, say $\boldsymbol{x}_2(t_1) = c_1\boldsymbol{x}_1(t_1)$, then $\boldsymbol{x}_2(t)$ and $c_1x_1(t)$ would be solutions to $(1)$ which agreed at $t_$1. Hence, by the uniqueness statement in Theorem 2, $\boldsymbol{x}_2(t) = c_1\boldsymbol{x}_1(t)$ on all of $I$, showing them linearly dependent on $I$.

### Theorem 2B General solution theorem
a) The system $(1)$ has two linearly independent solutions.  
b) If $\boldsymbol{x}_1(t)$ and $\boldsymbol{x}_2(t)$ are any two linearly independent solutions, then every solution $\boldsymbol{x}$ can be written in the form $(3)$, for some choice of $c_1$ and $c_2$:
$$\boldsymbol{x}=c_1\boldsymbol{x}_1+c_2\boldsymbol{x}_2\tag{3}$$
**Proof.** Choose a point $t = t_0$ in the interval $I$.  
a) According to Theorem 2, there are two solutions $\boldsymbol{x}_1, \boldsymbol{x}_2$ to $(1)$, satisfying respectively the initial conditions
$$\boldsymbol{x}_1(t_0) = i, \boldsymbol{x}_2(t_0) = j \tag{4}$$
where $i$ and $j$ are the usual unit vectors in the $xy$-plane. Since the two solutions are linearly independent when $t = t_0$, they are linearly independent on $I$, by Theorem 2A.  
b) Let $\boldsymbol{u}(t)$ be a solution to $(1)$ on $I$. Since $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are independent at $t_0$ by Theorem 2, using the parallelogram law of addition we can find constants $c_1'$ and $c_2'$ such that
$$\boldsymbol{u}(t_0) = c_1'\boldsymbol{x}_1(t_0) + c_2'\boldsymbol{x}_2(t_0)\tag{5}$$
The vector equation $(5)$ shows that the solutions $\boldsymbol{u}(t)$ and $c_1'\boldsymbol{x}_1(t) + c_2'\boldsymbol{x}_2(t)$ agree at $t_0$. Therefore by the uniqueness statement in Theorem 2, they are
equal on all of $I$; that is,
$$\boldsymbol{u}(t)=c_1'\boldsymbol{x}_1(t) + c_2'\boldsymbol{x}_2(t) \text{ on } I$$
