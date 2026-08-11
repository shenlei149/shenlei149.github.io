### Unit Step Response
We will use the example of an undamped harmonic oscillator with input $f(t)$ modeled by
$$mx''+kx=f(t)$$
The **unit step response** is the solution to this equation with input $u(t)$ and rest initial conditions $x(t) = 0$ for $t < 0$. That is, it is the solution to the
initial value problem (IVP)
$$mx''+kx=u(t),x(0^-)=0,x'(0^-)=0$$
This could be an undamped spring-mass system with mass $m$ and spring constant $k$. The mass is at rest at equilibrium until time $t = 0$ when a steady force starts to act on it.

Force represents a change in momentum over time. A finite force $F(t)$ can only cause an ininitesimal change in momentum (i.e. $F(t) dt$) at a time. Therefore, the mass does not change position abruptly, nor does it change velocity instantaneously. Because of this we should expect a solution which is continuous with continuous derivative. Only the acceleration experiences a discontinuity.  
For $t < 0$ we already know that $x(t) = 0$. For $t > 0$ the DE is
$$mx''+kx=1$$
This has a constant particular solution $x(t) = 1/k$, and a general homogeneous solution
$$x_h(t)=c_1\cos(\omega_nt)+c_2\sin(\omega_nt), \text{ for }\omega_n=\sqrt{k/m}$$
Putting the two together gives the general solution
$$x(t)=1/k+c_1\cos(\omega_nt)+c_2\sin(\omega_nt), \text{ for }t>0$$
The continuity of $x$ and $x$ implies $x(0) = x(0^-) = 0$ and $x(0) = x(0^-) = 0$. This allows us to find $c_1$ and $c_2$.
$$
\begin{aligned}
0&=x(0)&=&1/k+c_1 &\rArr &c_1=-1/k\\
0&=x'(0)&=&c_2\omega_n &\rArr &c_2=0
\end{aligned}
$$
The **unit step response** for this system is (in both cases and $u$-format)
$$
x(t)=\begin{cases}
0&\text{for }t<0\\
\frac{1}{k}(1-\cos(\omega_nt))&\text{for }t>0
\end{cases}=\frac{1}{k}(1-\cos(\omega_nt))u(t)
$$
As in the first order case, we will sometimes denote this $v(t)$.

The claim that we get a continuous response is true, but may feel a bit unjustified. Let's redo the above example very carefully without making this assumption. It will take more work, but we will get the same answer.
In cases format the equation for the IVP is
$$
mx''+kx=\begin{cases}
0&\text{for }t<0\\
1&\text{for }t>0
\end{cases}, x(0^-)=0, x'(0^-)=0\tag{1}
$$
Solving the two pieces we get
$$
x(t)=\begin{cases}
c_1\cos(\omega_nt)+c_2\sin(\omega_nt)&\text{for }t<0\\
1/k+c_3\cos(\omega_nt)+c_4\sin(\omega_nt)&\text{for }t>0
\end{cases}
$$
The pre-initial conditions $x(0^-) = x'(0^-) = 0$ easily imply $c_1 = c_2 = 0$. So our solution looks like
$$
x(t)=\begin{cases}
0&\text{for }t<0\\
1/k+c_3\cos(\omega_nt)+c_4\sin(\omega_nt)&\text{for }t>0
\end{cases}
$$
To find $c_3$ and $c_4$ we substitute $x(t)$ into equation $(1)$.
To measure the jumps we compute $x(0^+) = 1/k + c_3$ and $x'(0^+) = c_4\omega_n$. We use this as we compute derivatives of $x$.
$$
x'(t)=(1/k + c_3)\delta(t)+\begin{cases}
0&\text{for }t<0\\
-c_3\omega_n\sin(\omega_nt)+c_4\omega_n\cos(\omega_nt)&\text{for }t>0
\end{cases}
$$
$$
x''(t)=(1/k + c_3)\delta'(t)+c_4\omega_n\delta(t)+\begin{cases}
0&\text{for }t<0\\
-c_3\omega_n^2\cos(\omega_nt)-c_4\omega_n^2\sin(\omega_nt)&\text{for }t>0
\end{cases}
$$
Since the right-hand side of equation $(1)$ does not have any delta functions or any $\delta'(t)$ the coefficients in front of these terms in the formula for $x''$ must be 0:
$$
\begin{aligned}
1/k+c_3&=0 &\rArr &c_3=-1/k\\
c_4\omega_n&=0 &\rArr &c_4=0
\end{aligned}
$$
In the end, we have exactly the same solution as above for the unit step response.

To summarize: the continuity assumptions follow because any jumps in $x(t)$ or $x'(t)$ would result in delta functions when $x$ is substituted into equation $(1)$.  
The generalized derivative $\delta'(t)$ is not something we've seen before. It is often called a doublet. There is an entire theory of these and other generalized functions, but we will only use $\delta(t)$ in this course.  
Figure 1 shows the graph of the unit step response (with $k = 1$ and $m = 0.5$.  
![](pic260501.png)  
If we added some damping the homogeneous part of the solution would go to 0 and the unit step response would go asymptotically to $1/k$.

### The Meaning of the Phrase 'Unit Step Response'
As we noted in the first order case, the unit step response is the response of the system to a unit step input. For example, if our system is
$$mx''+bx'+kx=by'$$
and we consider $y$ to be the input, then the unit step response is the solution to
$$mx''+bx'+kx=bu'(t), \text{ equivalently }mx''+bx'+kx=b\delta(t)$$
