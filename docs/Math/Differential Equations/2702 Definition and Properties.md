### Definition
The **convolution** of two functions $f$ and $g$ is a third function which we denote $f*g$. It is defined as the following integral
$$(f*g)(t)=\int_{0^-}^{t^+}f(\tau)g(t-\tau)d\tau\tag{1}$$
We will leave this unmotivated until the next note, and for now just learn how to work with it.  
There are a few things to point out about the formula.
* The variable of integration is $\tau$. We can't use $t$ because that is already used in the limits and in the integrand. We can choose any symbol we want for the variable of integration – it is just a *dummy* variable.
* The limits of integration are $0^-$ and $t^+$. This is important, particularly when we work with delta functions. If $f$ and $g$ are continuous or have at worst jump discontinuities then we can use 0 and $t$ for the limits. You will often see convolution written like this:
$$(f*g)(t)=\int_{0}^{t}f(\tau)g(t-\tau)d\tau$$
* We are considering **one-sided convolution**. There is also a two-sided convolution where the limits of integration are $\pm \infty$.
* (**Important**) One-sided convolution is only concerned with functions on the interval $(0^-, \infty)$. When using convolution we never look at t < 0.
### Examples
Example 1 below calculates two useful convolutions from the definition $(1)$. As you can see, the form of $f*g$ is not very predictable from the form of $f$ and $g$.  
**Example 1.** Show that
$$e^{at}*e^{bt}=\frac{e^{at}-e^{bt}}{a-b}, a\neq b$$
$$e^{at}*e^{at}=te^{at}$$
**Solution.** We show the first; the second calculation is similar.  
If $a = b$,
$$\begin{aligned}
e^{at}*e^{bt}&=\int_0^t e^{a\tau}e^{b(t-\tau)}d\tau\\
&=e^{bt}\int_0^t e^{(a-b)\tau}d\tau\\
&=e^{bt}\frac{e^{(a-b)\tau}}{a-b}\bigg|_0^t\\
&=e^{bt}\frac{e^{(a-b)t}-1}{a-b}\\
&=\frac{e^{at}-e^{bt}}{a-b}
\end{aligned}$$
If $a=b$,
$$\begin{aligned}
e^{at}*e^{at}&=\int_0^t e^{a\tau}e^{a(t-\tau)}d\tau\\
&=\int_0^t e^{at}d\tau\\
&=e^{at}\int_0^t 1d\tau\\
&=te^{at}
\end{aligned}$$
Note that because the functions are continuous we could safely integrate just from 0 to $t$ instead of having to specify precisely $0^-$ to $t^+$.

The convolution gives us a formula for a particular solution $y_p$ to an inhomogeneous linear ODE. The next example illustrates this for a first order equation.  
**Example 2.** Express as a convolution the solution to the first order constantcoefficient linear IVP.
$$y'+ky=q(t);y(0)=0\tag{2}$$
**Solution.** The integrating factor is $e^{kt}$; multiplying both sides by it gives
$$(ye^{kt})'=q(t)e^{kt}$$
Integrate both sides from 0 to $t$, and apply the Fundamental Theorem of Calculus to the left side; since we have $y(0) = 0$, the solution we seek satisfies
$$ye^{kt}=\int_0^t q(\tau)e^{k\tau}d\tau$$
Moving the $e^{kt}$ to the right side and placing it under the integral sign gives
$$y_p=\int_0^t q(\tau)e^{-k(t-\tau)}d\tau$$
$$y_p=q(t)*e^{-kt}$$
Now we observe that the solution is the convolution of the input $q(t)$ with $e^{-kt}$, which is the solution to the corresponding homogeneous DE $y'+ky=0$, but with IC $y(0) = 1$. This is the simplest case of Green's formula, which is the analogous result for higher order linear ODE's, as we will see shortly.
### Properties
1. **Linearity:** Convolution is *linear*. That is, for functions $f_1$, $f_2$, $g$ and constants $c_1$, $c_2$ we have
$$(c_1f_1+c_2f_2)*g=c_1(f_1*g)+c_2(f_2*g)$$
This follows from the exact same property for integration. This might also be called the **distributive law**.

2. **Commutivity:** $f*g = g*f$.  
**Proof:** This follows from the change of variable $v=t-\tau$.  
Limits: $\tau=0^- \rArr t-\tau=t^+$, and $\tau=t^+ \rArr t-\tau=0^-$  
$dv = -d\tau$  
Integral:
$$\begin{aligned}
(f*g)(t)&=\int_{0^-}^{t^+} f(\tau)g(t-\tau)d\tau\\
&=\int_{t^+}^{0^-} f(t-v)g(v) (-dv)\\
&=\int_{0^-}^{t^+} g(v)f(t-v)dv\\
&=(g*f)(t)
\end{aligned}$$
3. **Associativity:** $f * (g * h) = (f * g) * h$. The proof just amounts to changing the order of integration in a double integral.
$$\begin{aligned}
(f*(g*h))(t)&=\int_0^t f(u)(g*h)(t-u)du\\
&=\int_{u=0}^tf(u)(\int_{s=0}^{t-u}g(s)h(t-u-s)ds)du\\
&=\int_{u=0}^t\int_{s=0}^{t-u}f(u)g(s)h(t-s-u)dsdu\\
&=\int_{u=0}^t\int_{s=u}^{t}f(u)g(s-u)h(t-s)dsdu\\
&=\int_{s=0}^t\int_{u=0}^sf(u)g(s-u)h(t-s)duds\\
&=\int_{s=0}^t(\int_{u=0}^sf(u)g(s-u)du)h(t-s)ds\\
&=\int_0^t(f*g)(s)h(t-s)ds\\
&=((f*g)*h)(t)
\end{aligned}$$
### Delta Functions
We have
$$(\delta*f)(t)=f(t)\text{ and }(\delta(t-a)*f)(t)=f(t-a)\tag{3}$$
The notation for the second equation is ugly, but its meaning is clear.

We prove these formulas by direct computation. First, remember the rules of integration with delta functions: for $b>0$
$$\int_{0^-}^b \delta(\tau)f(\tau)d\tau=f(0)$$
The formulas follow easily for $t\geq 0$
$$\begin{aligned}
(\delta*f)(t)&=\int_{0^-}^{t^+}\delta(\tau)f(t-\tau)d\tau\\
&=\int_{0^-}^{t^+}\delta(\tau)f(t-0)d\tau\\
&=f(t-0)\int_{0^-}^{t^+}\delta(\tau)d\tau\\
&=f(t)
\end{aligned}$$
$$\begin{aligned}
(\delta(t-a)*f)(t)&=\int_{0^-}^{t^+} \delta(\tau-a)f(t-\tau)d\tau\\
&=\int_{0^-}^{t^+} \delta(\tau-a)f(t-a)d\tau\\
&=f(t-a)\int_{0^-}^{t^+} \delta(\tau-a)d\tau\\
&=f(t-a)
\end{aligned}$$

### Convolution is a Type of Multiplication
You should think of convolution as a type of multiplication of functions. In fact, it is often referred to as the *convolution product*. In fact, it has the properties we associate with multiplication:
* It is commutative.
* It is associative.
* It is distributive over addition.
* It has a multiplicative identity. For ordinary multiplication, 1 is the multiplicative identity. Formula $(3)$ shows that $\delta(t)$ is the multiplicative identity for the convolution product.
