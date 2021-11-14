An important kind of second-order non-linear autonomous equation has the form
$$x''+u(x)x'+v(x)=0\text{ (Liénard equation)}\tag{1}$$
One might think of this as a model for a spring-mass system where the damping force $u(x)$ depends on position (for example, the mass might be moving through a viscous medium of varying density), and the spring constant $v(x)$ depends on how much the spring is stretched - this last is true of all springs, to some extent. We also allow for the possibility that $u(x) < 0$ (i.e., that there is "negative damping").  
The system equivalent to $(1)$ is
$$\begin{aligned}
x'&=y\\
y'&=-v(x)-u(x)y
\end{aligned}\tag{2}$$
Under certain conditions, the system $(2)$ has a unique stable limit cycle, or what is the same thing, the equation $(1)$ has a unique periodic solution; and all nearby solutions tend towards this periodic solution as $t \to \infty$. The conditions which guarantee this were given by Liénard, and generalized in the following theorem.  
**Levinson-Smith Theorem** Suppose the following conditions are satisfied.  
(a) $u(x)$ is even and continuous,  
(b) $v(x)$ is odd, $v(x) > 0$ if $x > 0$, and $v(x)$ is continuous for all $x$,  
(c) $V(x) \to \infty$ as $x \to \infty$, where $V(x) = \int_0^x v(t) dt$,  
(d) for some $k > 0$, we have
$$\begin{aligned}
&U(x) < 0, &&\text{for } 0 < x < k,\\
&U(x) > 0 \text{ and increasing}, &&\text{for } x > k,\\
&U(x) \to \infty, &&\text{as } x \to \infty,
\end{aligned}\bigg\} \text{ where } U(x)=\int_0^x u(t)dt$$
Then, the system $(2)$ has  
i) a unique critical point at the origin;  
ii) a unique non-zero closed trajectory $C$, which is a stable limit cycle around the origin;  
iii) all other non-zero trajectories spiralling towards $C$ as $t \to \infty$.
We omit the proof, as too difficult. A classic application is to the equation
$$x'' - a(1 - x^2) x' + x = 0 \text{ (van der Pol equation)} \tag{3}$$
which describes the current $x(t)$ in a certain type of vacuum tube. (The constant $a$ is a positive parameter depending on the tube constants.) The equation has a unique non-zero periodic solution. Intuitively, think of it as modeling a non-linear spring-mass system. When $|x|$ is large, the restoring and damping forces are large, so that $|x|$ should decrease with time. But when $|x|$ gets small, the damping becomes negative, which should make $|x|$ tend to increase with time. Thus it is plausible that the solutions should oscillate; that it has exactly one periodic solution is a more subtle fact.  
There is a lot of interest in limit cycles, because of their appearance in systems which model processes exhibiting periodicity. But not a great deal is known about them - this is still an area of active research.
