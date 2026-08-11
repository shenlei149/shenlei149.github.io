We know that a standard way of testing whether a set of $n$ $n$-vectors are linearly independent is to see if the $n \times n$ determinant having them as its rows or columns is non-zero. This is also an important method when the nvectors are solutions to a system; the determinant is given a special name. (Again, we will assume $n = 2$, but the definitions and results generalize to any $n$.)  
**Definition 3** Let $\boldsymbol{x}_1(t)$ and $\boldsymbol{x}_2(t)$ be two 2-vector functions. We define their **Wronskian** to be the determinant
$$W(\boldsymbol{x}_1,\boldsymbol{x}_2)(t)=\begin{vmatrix}
x_1(t)&x_2(t)\\
y_1(t)&y_2(t)
\end{vmatrix}\tag{1}$$
whose columns are the two vector functions.  
The independence of the two vector functions should be connected with their Wronskian not being zero. For points, the relationship is clear. Using the result mentioned above, we can say
$$W(\boldsymbol{x}_1,\boldsymbol{x}_2)(t_0)=\begin{vmatrix}
x_1(t_0)&x_2(t_0)\\
y_1(t_0)&y_2(t_0)
\end{vmatrix} \lrArr \boldsymbol{x}_1(t_0) \text { and } \boldsymbol{x}_2(t_0) \text{ are dependent.}\tag{2}$$
However for vector functions, the relationship is clear-cut only when $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are solutions to a well-behaved ODE system $(3)$. The theorem is:  
We are still considering the system
$$\begin{aligned}
x'=a(t)x+b(t)y\\
y'=c(t)x+d(t)y
\end{aligned};\begin{pmatrix}
x\\y
\end{pmatrix}'=\begin{pmatrix}
a(t)&b(t)\\c(t)&d(t)
\end{pmatrix}\begin{pmatrix}
x\\y
\end{pmatrix}\tag{3}$$

### Theorem 3 Wronskian vanishing theorem
On an interval $I$ where the entries of $A(t)$ are continuous, let $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ be two solutions to $(3)$ and $W(t)$ their Wronskian $(1)$. Then either  
a) $W(t) \equiv 0$ on $I$, and $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are linearly dependent on $I$, or  
b) $W(t)$ is never 0 on $I$, and $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are linearly independent on $I$.  
**Proof.** Using $(2)$, there are just two possibilities.  
a) $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are linearly dependent on $I$; say $\boldsymbol{x}_2 = c_1\boldsymbol{x}_1$. In this case they are dependent at each point of $I$, and $W(t) \equiv 0$ on $I$, by $(2)$.  
b) $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are linearly independent on $I$, in which case by Theorem 2A they are linearly independent at each point of $I$, and so $W(t)$ is never zero on $I$, by (2).
