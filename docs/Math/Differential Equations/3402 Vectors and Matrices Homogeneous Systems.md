### More on matrices
Associated with every square matrix $A$ is a number, written $|A|$ or $|\det(A)|$ called the determinant of $A$. For these notes, it will be enough if you can calculate the determinant of $2 \times 2$ matrices, which is as follows:
$$\det\begin{pmatrix}
a&b\\c&d
\end{pmatrix}=ad-bc$$
The trace of a square matrix $A$ is the sum of the elements on the main diagonal; it is denoted $tr(A)$:
$$tr\begin{pmatrix}
a&b\\c&d
\end{pmatrix}=a+d$$
**Remark.** Theoretically, the determinant should not be confused with the matrix itself. The determinant is a number, the matrix is the square array. But, everyone puts vertical lines on either side of the matrix to indicate its determinant, and then uses phrases like "the first row of the determinant," meaning the first row of the corresponding matrix.  
An important formula which everyone uses and no one can prove is
$$\det{(AB)}=\det{A}\cdot \det{B}\tag{1}$$

### Homogeneous $2 \times 2$ systems
Matrices and determinants were originally invented to handle, in an efficient way, the solution of a system of simultaneous linear equations. This is still one of their most important uses. We give a brief account of what you need to know for now. We will restrict ourselves to square $2 \times 2$ homogeneous systems; they have two equations and two variables (or "unknowns", as they are frequently called). Our notation will be:
$$A = (a_{ij}), \text{a square $2 \times 2$ matrix of constants}$$
$$\boldsymbol{x} = (x_1, x_2)^T, \text{a column vector of unknowns}$$
then the square system
$$a_{11}x_1+a_{12}x_2=0$$
$$a_{21}x_1+a_{22}x_2=0$$
can be abbreviated by the matrix equation
$$A\boldsymbol{x}=0\tag{2}$$
This always has the solution $\boldsymbol{x} = 0$, which we call the **trivial solution**. The question is: when does it have a nontrivial solution?  
Theorem. Let $A$ be a square matrix. The equation 
$$A\boldsymbol{x} = 0 \text{ has a nontrivial solution} \lrArr \det{A} = 0 \text{(i.e., A is singular)}\tag{3}$$

### Linear independence of vectors
Conceptually, linear independence of vectors means each one provides something new to the mix. For two vectors this just means they are not zero and are not multiples of each of other.  
**Example 1.** $(1, 2)$ and $(3, 4)$ are linearly independent.  
**Example 2.** $\boldsymbol{a} = (1, 2)$ and $\boldsymbol{b} = (2, 4)$ are linearly dependent because $\boldsymbol{b}$ is a multiple of $\boldsymbol{a}$. Notice that if we take linear combinations then $\boldsymbol{b}$ doesn't add anything to the set of vectors we can get from $\boldsymbol{a}$ alone.  
**Example 3.** $\boldsymbol{a} = (1, 2)$ and $\boldsymbol{b} = (0, 0)$ are linearly dependent because $\boldsymbol{b}$ is a multiple of $\boldsymbol{a}$, i.e., $\boldsymbol{b} = 0 \boldsymbol{a}$.  
**Determinantal criterion for linear independence**  
Let $\boldsymbol{a} = (a_1, a_2)$ and $\boldsymbol{b} = (b_1, b_2)$ be 2-vectors, and $A$ the square matrix having these vectors for its rows (or columns). Then
$$\boldsymbol{a},\boldsymbol{b} \text{ are linearly independent } \lrArr \det{A}\neq 0\tag{4}$$  
Let us re-visit our previous examples.  
**Examples.**
1. $\det \begin{pmatrix}1&2\\3&4\end{pmatrix} = 4 - 6 = -2 \neq 0$. Therefore, $(1, 2)$ and $(3, 4)$ are linearly independent.
2. $\det \begin{pmatrix}1&2\\2&4\end{pmatrix} = 4 - 4 = 0$. Therefore, $(1, 2)$ and $(2, 4)$ are linearly dependent.
3. $\det \begin{pmatrix}1&2\\0&0\end{pmatrix} = 0 - 0 = 0$. Therefore, $(1, 2)$ and $(0, 0)$ are linearly dependent.

**Remark.** The theorem on square homogeneous systems $(3)$ follows from this criterion. We will prove neither.

Two linearly independent 2-vectors $\boldsymbol{v}_1$ and $\boldsymbol{v}_2$ form a basis for the plane: every 2-vector $\boldsymbol{w}$ can be written as a linear combination of $\boldsymbol{v}_1$ and $\boldsymbol{v}_2$. That is, there are scalars $c_1$ and $c_2$ such that
$$c_1\boldsymbol{v}_1+c_2\boldsymbol{v}_2=\boldsymbol{w}$$
**Remark.** All of the notions and theorems mentioned in this section generalize to higher $n$ (and a larger collection of vectors), though we will not need them.
