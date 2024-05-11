### Introduction
If we have a DE which models a situation involving a physical quantity $y(t)$ then solving the DE means finding the unknown function $y$. Knowing the possible solutions $y$ allows to understand the physical system. Of course someone needs to build the DE doing the modeling. In this note we will see how to do this for some real systems.

### The savings account model
Modeling a savings account gives a good way to understand the significance of many of the features of a general first order linear ordinary differential equation.  
Write $x(t)$ for the number of dollars in the account at time $t$. It accrues interest at an interest rate $r$. The interest rate has units of percent/year. The more money in the account the more interest you earn. At the end of an interest period of $\Delta t$ years (e.g. $\Delta t = 1/12$, or $\Delta t = 1/365$) the bank adds $r \cdot x(t) \cdot \Delta t$ dollars to your account. This means the change $\Delta x$ in your account is
$$\Delta x=rx(t)\Delta t$$
$r$ has units of $\text{(years)}^{-1}$. Mathematicians and some bankers like to take things to the limit. Rewrite our equation as
$$\frac{\Delta x}{\Delta t}=rx(t)$$
and suppose that the interest period is made to get smaller and smaller. In the limit as $\Delta t \rarr 0$, we get the differential equation
$$x'=rx(t)$$
One of the beautiful facts about this type of modeling is that it covers more complicated situations. In our computation, there was no assumption that the interest rate was constant in time; it could well be a function of time, $r(t)$. In fact it could have been a function of both time and the existing balance, $r(x, t)$. Banks often do make such a dependence - you get a better interest rate if you have a bigger bank account. If $x$ is involved, however, the equation is what is called *nonlinear* and we will not consider that case in this session.  
Now suppose we make contributions to this savings account. We'll record this by giving the *rate* of savings, $q$. This rate has units dollars per year, so if you contribute every month then the monthly payments will be $q \Delta t$ with $\Delta t = 1/12$. This payment also adds to your account, so, when we divide by $\Delta t$ and take the limit, we get
$$x'=rx+q$$
Once again, your rate of payment into the account may not be constant in time; we might have a function $q(t)$. Also, we can allow $q(t)$ to be negative, which corresponds to withdrawing money from the account.  
What we have, then, is the general first order linear ODE:
$$x'-r(t)x=q(t)\tag{1}$$

### Linear insulation
Here is another example of a linear ODE. The linear model here is not as precise as in the bank account example.  
A cooler insulates my lunchtime root beer against the warmth of the day, but ultimately heat penetrates. Let's see how you might come up with a mathematical model for this process. You can jump right to equation $(2)$ if you want, but we would like to spend a some time talking about how one might get there, so that you can carry out the analogous process to model other situations.  
The first thing to do is to identify relevant parameters and give them names. Let's write $t$ for the time variable, $x(t)$ for the temperature inside the cooler, and $y(t)$ for the temperature outside.  
Let's assume that the insulating properties of the cooler don't change over time. (We're not going to watch this process for so long that the aging of the cooler itself becomes important!) However, the insulating properties probably do depend on the inside and outside temperatures. Insulation affects the rate of change of the temperature: the rate of change at time $t$ of temperature inside depends upon the temperatures inside and outside at time $t$. This gives us a first order differential equation of the form
$$x'=F(x, y)$$
Now it's time for the next simplifying assumption, namely that this rate of change depends only on the *difference* $y - x$ between the temperatures, and not on the temperatures themselves. This means that
$$x'=f(y-x)$$
for some function $f$ of *one* variable. If the temperature inside the cooler equals the temperature outside, we expect no change. This means that $f(0) = 0$.  
Now, any reasonable function has a *tangent line approximation*, and since $f(0) = 0$ we have
$$f(z)\approx kz$$
That is, when $|z|$ is fairly small, $f(z)$ is fairly close to $kz$. (From calculus you know that $k = f'(0)$, but we won't use that here.) When we replace $f(y - x)$ by $k(y - x)$ in the differential equation, we are linearizing the equation. We get the ODE
$$x'=k(y-x)$$
The final assumption we are making, in justifying this last simplification, is that we will only use the equation when $z = y - x$ is reasonably small - small enough so that the tangent line approximation is reasonably good. For large temperature differences the linearized model will not generally give realistic results.  
We can write this equation as
$$x'+kx=ky\tag{2}$$
This is *Newton's law of cooling*.  
The constant $k$ is called the coupling constant. It mediates between the two temperatures. It will be large if the insulation is poor, and small if it's good. If the insulation is perfect, then $k = 0$. The factor of $k$ on the right might seem odd, but it you can see that it is forced on us by checking units: the left hand side is measured in degrees per hour, so $k$ must be measured in units of $\text{(hours)}^{-1}$.  
We can see some general features of insulating behavior from this equation. For example, the times at which the inside and outside temperatures coincide are the times at which the inside temperature is at a critical point:
$$x'(t_1)=0 \text{ exactly when } x(t_1)=y(t_1)\tag{3}$$
