The most important differential equation we will study is
$$y' = ay\tag{1}$$
In words the equation says  
*the rate of change of $y$ is proportional to $y$*

It is hardly an exaggeration to say that much of 18.03 is an elaboration on this fundamental equation. In any case, we need to understand this DE and its solutions thoroughly.  
Because of its importance we will write down some other ways you might see it
$$\begin{aligned}
\dot{y}=ay;&&\frac{dy}{dt}=ay(t);&&y'=ay;&&\dot{y}-ay=0
\end{aligned}\tag{2}$$
You should recognize all these as the same equation.

The solution to this equation is
$$y(t)=Ce^{at}$$
where $C$ is any constant. This is easily checked by substitution. Again, because this equation is so important we show the details.
$$\begin{aligned}
&\text{Left side of $(1)$: } y'= aCe^{at}\\
&\text{Right side of $(1)$: } ay = aCe^{at}
\end{aligned}
$$
Since, after substitution the left side equals the right side, $y(t) = Ce^{at}$ is indeed a solution of $(1)$.

Because of the exponential in the solution equation $(1)$ is said to model **expontial growth** (when $a > 0$) or **decay** (when $a < 0$). The constant $a$ is known as the **growth** or **decay** constant.  
In this course we will learn many techniques for solving differential equations. We will test almost all of them on equation $(1)$. You should, of course, understand how to use these techniques to solve $(1)$. However: *whenever you see this equation you should remind yourself it models exponential growth or decay and know the solution without computation*.
