### Separable Equations
We will now learn our first technique for solving differential equation. An equation is called **separable** when you can use algebra to separate the two variables, so that each is completely on one side of the equation. We illustrate with some examples.  
**Example 1.** Solve $y'=x(y-1)$  
**Solution.** We rewrite the equation as $\frac{dy}{dx}=x(y-1)$. Then separate the variables
$$\frac{dy}{y-1}=xdx$$
Next we integrate both sides.
$$\int \frac{dy}{y-1}=\int xdx \lrArr ln|y-1|+c_2=\frac{x^2}{2}+c_1$$
We can amalgamate the two constants of integration into one constant:
$$ln|y-1|=\frac{x^2}{2}+c_3$$
The absolute value signs can be removed, but then the right hand side might be positive or negative. We write this as
$$y-1=\plusmn e^{c_3}e^{x^2/2} \lrArr y=1+ \plusmn e^{c_3}e^{x^2/2}$$
Finally we replace the constant $\plusmn e^{c_3}$ by $C$ to get the solution
$$y=1+Ce^{x^2/2}$$
**Note.** For the more rigorously minded an expression like $\frac{dy}{y-1}=xdx$ might give pause. However, this formal method is justified by the chain rule, in the same way change of variable ($u$-substitution) is justified for integration.

### Lost Solutions
**Example 2.** Find all the solutions to the DE
$$y'=2x(1-y)^2\tag{1}$$
**Solution.** First, note there is a constant solution: $y(x) = 1$. It is easy to see this is a solution by substituting it into $(1)$ - both sides of the equation become 0. We need to note this because, as we will see, the separation of variables method will not find this particular solution.  
Now let's solve the DE by separation of variables.  
1. Separate variables:
$$\frac{dy}{(1-y)^2}=2xdx$$
2. Integrate:
$$\frac{1}{1-y}=x^2+C$$
3. Solve for $y$:
$$y=1-\frac{1}{x^2+C}$$
Notice that the constant solution $y(x) = 1$ is not in the parametrized family found by separation variables. We call this a **lost solution** because it is lost by separation of variables.  
How did it get lost? The answer is in step $(1)$ above, where the term $\frac{dy}{(1-y)^2}$ is only valid if $y\neq 1$.
In general, for the separable DE $y' = f(x)g(y)$, all the roots of $g(y)$ give lost (constant) solutions.  
**Example 3.** Find all the lost solutions of $y' = (x + 1)e^x(y^2 - 8y + 7)$.  
**Solution.** The factor $y^2 - 8y + 7$ has roots $y = 1$ and $y = 7$. Therefore the lost solutions are the constant functions $y(x) = 1$ and $y(x) = 7$.

### The Most Important DE
Even though we already know the solution, we should test our new technique on the DE for exponential growth/decay.  
**Example 4.** Solve $y' = ky$.  
**Solution.** Separate variables: $\frac{dy}{y}=k$  
Integrate: $ln|y|=kt+c_1$  
Exponentiate: $|y|=e^{kt+c_1}=e^{c_1}e^{kt}$  
Remove absolute value: $y=\plusmn e^{c_1}e^{kt}$  
Let the constant $\plusmn e^{c_1}=C$: $y=Ce^{kt}$  
All solutions to the DE are
$$y(t)=Ce^{kt}$$
If you look carefully you'll see we did one rather sneaky thing. The solution $y(t) = 0$ is a lost solution, yet it appears to have been found by the separation of variables (set $C = 0$). What happened is that when we renamed $\plusmn e^{c_1}$ as $C$ we should have noticed that the exponential is never 0, so $C = 0$. Essentially, we included the lost solution by being a little sloppy and then getting lucky. We do not recommend this technique as a way to do mathematics!
