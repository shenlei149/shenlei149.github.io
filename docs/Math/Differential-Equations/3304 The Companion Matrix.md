### Introduction
We started the session by using elimination to convert a first order linear system to an ordinary differential equation in one of the variables. In this note we'll see how to go the other way. That is, to convert a second order ODE to a $2 \times 2$ system of first order equations. Looking at a second order system this way gives us an important way to visualize the second order equation.  
Since elimination takes us in the other direction we might call this process **anti-elimination**. This term is not entirely standard, but it will serve us nicely.  
We won't cover it in this course, but every method we saw for numerical solutions to first order ODE's goes through without change to systems of first order equations. Thus, anti-elimination allows us to use numerical techniques on second order ODE's. Indeed it can be used to convert any order ODE to a system of first order equations.

### Anti-elimination and the Companion Matrix
Suppose we have a second order homogeneous linear equation, say
$$x''+bx'+kx=0\tag{1}$$
We can derive a first order linear system from this, by using the following trick. Introduce a second variable defined by
$$y=x'$$
Substituting $y = x'$ and $y' = x''$ into equation $(1)$ we get
$$y'+by+kx=0 \rArr y'=-by-kx$$
We now have a first order system
$$\begin{aligned}
x'&=&&&y\\
y'&=&-kx&&-by
\end{aligned}$$
The corresponding coefficient matrix is
$$A=\begin{pmatrix}
0&1\\
-k&-b
\end{pmatrix}$$
This is called the **companion matrix** of the equation $(1)$. In this case, the $x(t)$ solution vector $u(t)=\begin{pmatrix}x(t)\\x'(t)\end{pmatrix}$. It records both the solution to $(1)$ and its derivative.  
**Example 1.** Consider the equation $x''-x'+\frac{5}{4}x=0$.  The companion matrix $\begin{pmatrix}0&1\\-5/4&1\end{pmatrix}$.  
What do solutions of this system look like? The characteristic polynomial of the second order equation is
$$p(s)=s^2-s+5/4=(s-(1/2))^2+1$$
So, the roots are $r = (1/2) \plusmn i$. From unit 2, the general solution in amplitudephase form is given by
$$x(t)=Ce^{t/2}\cos (t-\phi)$$
where $C$ and $\phi$ are constants. These oscillate under an exponentially growing envelope. The derivative does the same, but is off phase. This means that the trajectory traced out by $(x, y) = (x, x')$ is an expanding spiral.  
**Example 2.** (Elimination followed by anti-elimination)  
Earlier in this session, we learned how to solve systems by elimination. What happens when we do elimination followed by anti-elimination? Let us re-visit the example about Romeo and Juliet. The second order system describing their mutual feelings was
$$R'=\frac{1}{4}R+J\tag{2}$$
$$J'=-\frac{17}{16}R+\frac{3}{4}J\tag{3}$$
Let us first eliminate $J$, to get a second order equation in $R$. From $(2)$, $J=R'-(1/4)R$. Substituting this into $(3)$ gives
$$R''-\frac{1}{4}R'=-\frac{17}{16}R+\frac{3}{4}R'-\frac{3}{16}R \rArr R''-R'+\frac{5}{4}R=0\tag{4}$$
(We saw this equation in example 1.)  
So elimination applied to the system $(2), (3)$ gives the ODE in equation $(4)$. We continue by applying anti-elimination to $(4)$. Letting $Y = R'$ this gives the system
$$\begin{aligned}
R'&=&&&Y\\
Y'&=&-\frac{5}{4}R&&+Y
\end{aligned}$$
The point of this example is that we end with a different system than the one we started with. That is, the companion matrix of the ODE $(4)$ is different from the original matrix associated to the system $(2), (3)$.
