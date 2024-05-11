### Independent and Dependent Variables
When we write a function such as 
$$f(x)=3x^2+2x+1$$
we say that $x$ is an **independent variable**: it can be freely set to any value (or any value within the given domain) and the value of the function is then computed.  
When we give a name to the value of the function, such as
$$y=3x^2+2x+1 \text{ or }y=f(x)$$
we say that $y$ is a **dependent variable**. That is, the value of $y$ *depends* on the value we choose for $x$.
We can have systems of equations with more than one dependent variable. For example, 
$$\begin{aligned}
x&=t^2-1\\
y&=3e^t
\end{aligned}$$
Here the dependent variables $x$ and $y$ depend on the independent variable $t$.  
We can have functions with more than one independent variable. For example,
$$x=st^2-t-s$$
Here the independent variables are $t$ and $s$ and the dependent variable is $x$.
And, of course, we can have more than one of each:
$$\begin{aligned}
x&=st^2-t-s\\
y&=3e^{t+s}
\end{aligned}$$
As a matter of notation (often referred to by mathematicians as "abuse of notation") we can use the dependent variable to also denote the function. So, for example, we can write
$$x=x(t)=t^2-1$$
Most of what we do will involve *ordinary* differential equations. These have only *one* independent and *one* dependent variable. Differential equations arise from many sources, and the independent variable can signify many different things. Nonetheless, very often it represents time, and the dependent variable is some dynamical quantity which depends upon time. For this reason, in this course we will often use $t$ for the independent variable.

### Parameters
Parameters are similar to variables - that is, letters that stand for numbers - but have a different meaning. We use parameters to describe a set of (usually) similar things. Parameters can take on different values, with each value of the parameter specifying a member of this set of similar objects.  
An example should make this clear. In calculus you learned to find the antiderivative (integral) of $t^2$. There are many functions whose derivative is $t^2$. For example,
$$t^3/3+2 \text{ or }t^3/3+\pi$$
So, to give the full answer we write
$$\int t^2dt=t^3/3+c$$
where $c$ is called the *constant of integration*. In this case, each value of $c$ specifies a single antiderivative. We call $c$ the **parameter** of the set of *all* the antiderivatives of $t^2$. Each value of the parameter $c$ specifies a single antiderivative.  
Sets are written formally using curly braces, e.g., {$t^3/3 + c$ : $c$ any number}, but we will rarely do this. For example, we will write,
$$x(t)=t^3/3+c, \text{ where } c \text{ is an arbitrary constant.}\tag{1}$$
This means a set of functions $x=x(t)$ *parametrized* by $c$.  
Sets can depend on more than one parameter. For example,
$$x(t)=c_1e^{-t}+c_2e^{-7t} \text{ where } c_1, c_2 \text{ are arbitrary constants.}\tag{2}$$
Because each of the functions in $(1)$ are similar - they all have a family resemblance - we say equation $(1)$ gives a **1-parameter family of functions**. Likewise, we say $(2)$ gives a **2-parameter family of functions**. You see the pattern!
