### Definition of Differential Equations
A differential equation is an equation expressing a relation between a function and its derivatives. For example, we might know that $x$ is a function of $t$ and
$$x''+8x'+7x=0\tag{1}$$
or perhaps the relation is more complicated, like
$$\sqrt{xx^{(5)}}+\cos(t)e^{tx}+(x''x'x)^6=\sin(5t)\tag{2}$$
When the function in the differential equation has a single independent variable we call it an **ordinary differential equation**. That is, the derivatives are ordinary derivatives, not partial derivatives. This course is almost exclusively concerned with ordinary differential equations.
#### The Order of a Differential Equation
The **order** of a differential equation is the order of the largest derivative appearing in it. Equation $(1)$ is a second order differential equation. Equation $(2)$ is a fifth order equation since the highest derivative is $x^{(5)}$ (in the first term).

### Solving a Differential Equation
Solving a differential equation means finding a function that satisfies the equation. For many equations it can be hard or impossible to find a solution. One thing that is easy however is to check a proposed solution. We demonstrate with a few examples.

**Example 1.** Checking a Solution By Substitution  
Verify the $y(t) = e^{3t}$ is a solution to the differential equation
$$y'=3y\tag{3}$$
**Solution.** To do this we simply substitute $y = e^{3t}$ into $(3)$, and check that the equation holds. On the left hand side of $(3)$ we have $y = 3e^{3t}$. On the right hand side we have $3y = 3e^{3t}$. Since both sides are equal, $y = e^{3t}$ is a solution.

**Example 2.** Rejecting a Solution by Substitution  
Show that $y(t) = t^3$ is not a solution to the differential equation
$$y'=y/t\tag{4}$$
**Solution.** Again, we substitute the expression for $y$ into $(4)$.  
Left hand side: $y' = 3t^2$.  
Right hand side: $y/t = t^2$. Since the two sides are not equal, $y = t^3$ is not a solution.

### Parametrizing the Set of Solutions of a Differential Equation
Differential equations usually have more than one solution. We can describe them all at once using a parameter.

**Example.** Find all the solutions to
$$x''=2t\tag{5}$$
This is a standard calculus problem. Integrating twice and remembering to include the constants of integration gives
$$x(t)=\frac{t^3}{3}+c_1t+c_2$$
where $c_1$ and $c_2$ are arbitrary constants. This expression gives a parametrization of the set of solutions to equation $(5)$. The constants $c_1$ and $c_2$ are parameters. Every choice of $c_1$ and $c_2$ gives a different solution to $(5)$. For example, $x = t^3/3 + 2t + 1$ and $t^3/3 + \pi t + 2.718$ are both solutions.

### Initial Value Problems
Sometimes we have a differential equation and **initial conditions**. Together they make up an **initial value problem**. The meaning of the term initial conditions is best illustrated by example.

**Example.** Solve the initial value problem $x'' = 2t$ with the initial conditions $x(1) = 1, x'(1) = 2$.  
**Solution.** In the previous example we found the general solution of this differential equation
$$x(t)=\frac{t^3}{3}+c_1t+c_2$$
We use the initial conditions to find the values of $c_1$ and $c_2$.
$$x'(t)=t^2+c_1 \rArr x'(1)=1+c_1=2$$
$$x(t)=t^3/3+c_1t+c_2 \rArr x(1)=1/3+c_1+c_2=1$$
Solving for $c_1$ and $c_2$ we get $c_1 = 1$, $c_2 = -1/3$. Thus, the solution to the initial value problem is
$$x(t)=t^3/3+t-1/3$$

### Acronyms
It will be convenient at times to allow ourselves to use acronyms. Some of the most common are
1. Differential equation (DE).
2. Ordinary differential equation (ODE).
3. Initial value problem (IVP).
4. Initial conditions (IC).
