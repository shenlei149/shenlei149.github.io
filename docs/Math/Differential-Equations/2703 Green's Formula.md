In this note we state Green's formula and look at some examples. We will prove it in the next note.

### Green's formula
Suppose that we have a linear time invariant system with rest IC.
$$P(D)y=f(t), y(t)=0 \text{ for } t<0\tag{1}$$
* As in previous sessions, we will consider $f(t)$ to be the input to this system. Everything we say will also hold for systems like $T' + kT = kT_e$ with input $T_e$ and $mx'' + bx' + kx = by'$ with input $y$.
* In this context, where we don't consider functions for $t < 0$, the initial conditions mean that $y(t)$ and all its derivatives are 0 at $t = 0^-$.
* $P(D)$ is a polynomial differential operator. Although it can be of any order, recall that we developed the second order case extensively in the last unit, where it was often written as
$$P(D)y=my''+by'+ky$$

Suppose further that $w(t)$ is the unit impulse response for $(1)$. That is, $w(t)$ satisfies $P(D)w = \delta(t)$, with rest IC. Then, for any input $f(t)$ the solution to equation $(1)$ is given by **Green's formula**
$$y(t)=(f*w)(t)=\int_{0^-}^{t^+} f(\tau)w(t-\tau)d\tau\tag{2}$$
This is a wonderful formula! It tells us the response to *any* input once we know the unit impulse response. Furthermore, it gives us that response as an integral which can be computed numerically if necessary. For many physical systems the impulse response can be measured directly or deduced from measurements. So, Green's formula gives us a method for predicting the system's response to any input.

### Unit Impulse Response = Weight Function
The unit impulse response is also called the **weight function**. We will use the terms interchangeably. If we think of an integral as a 'sum' then Green's formula shows the solution $y(t)$ to $(1)$ is given as a weighted sum of the small bits of input, $f(\tau) d\tau$ from before time $t$. Each piece is weighted by $w(t - \tau)$.  
Before proceeding, let us recall the definition of the unit impulse response. The weight $w(t)$ is the unique solution to the IVP
$$P(D)y=\delta(t) \text{ with rest IC}\tag{3}$$
In the previous session we learned how to rewrite $(3)$ as a homegeneous equation. We will only restate this for second order equations. The weight function for the system
$$mx''+bx'+kx=f(t)$$
is 0 for $t < 0$ and the solution to
$$mx''+bx'+kx=0, x(0)=0, x'(0)=1/m$$
for $t>0$.

### Examples
We now try out Green's formula $(2)$ in a couple of cases where it can be checked against the solution found using another method.

**Example 1.** Find the particular solution given by $(2)$ to
$$y''+y=A,y(0)=0,y'(0)=0, \text{ where A is a constant.}$$
**Solution.** The unit impulse response is $w(t) = \sin t.$ Therefore for $t \geq 0$,
we have
$$y_p(t)=\int_0^t A\sin(t-\tau)d\tau=A\cos(t-\tau)\bigg|_0^t=A(1-\cos t)$$
We check this by another method: The exponential response formula or the method of undetermined coefficients produces the particular solution $y_p = A$. Adding in the homogeneous solution we get the general solution to the DE is
$$y=A+c_1\cos t+c_2\sin t$$
You can easily compute that the rest initial conditions are matched by $y = A - A \cos t$, as found by Green's formula.

**Example 2.** Find the particular solution for $t \geq 0$ given by $(2)$ to
$$y''+y=f(t)=\begin{cases}
1&\text{for }0\leq t\leq \pi\\
0&\text{elsewhere}
\end{cases}$$
**Solution.** Here the method of Example 1 leads to two cases: $0\leq t\leq \pi$ and $t \geq \pi$
$$\begin{aligned}
y_p&=\int_0^t f(\tau)\sin(t-\tau)d\tau\\
&=\begin{cases}
\int_0^t \sin(t-\tau)d\tau&=\cos(t-\tau)\bigg|_0^t&=1-\cos t \text{ for } 0\leq t\leq \pi\\
\int_0^\pi \sin(t-\tau)d\tau&=\cos(t-\tau)\bigg|_0^\pi&=-2\cos t \text{ for } t \geq \pi
\end{cases}
\end{aligned}$$
Let's check this by our earlier methods.  
For $0\leq t\leq \pi$, it is example 1 and $A=1$, so $y_p=1-\cos t$.  
For $t \geq \pi$,
$$y=c_1\cos t+c_2\sin t, y(\pi)=2, y'(\pi)=0$$
$$y(\pi)=-c_1=2 \rArr c_1=-2$$
$$y'(\pi)=-c_2=0 \rArr c_2=0$$
