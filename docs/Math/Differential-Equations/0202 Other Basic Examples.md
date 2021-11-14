Here are some basic examples of DE's taken from math and science. Except for example 1 we will not give solutions. We will do that and more with these DE's as we go through the course.

**Example 1.** (From Calculus)  
Solve for $y$ satisfying $\frac{dy}{dx}=2x$  
**Solution.** This problem is just asking for the anti-derivative of $2x$:
$$y(x) = x^2 + c$$
Notice that there are many solutions, parametrized by *c*. An expression like this, which parametrizes all the solutions is called **the general solution**.

**Example 2.** (Heat Diffusion)  
A body at temperature $T$ sits in an environment of temperature $T_E$. Newton's law of cooling models the rate of change in temperature by
$$T' = -k(T - T_E)$$
where $k$ is a positive constant. Note, the minus sign guarantees that the temperature $T$ is always heading towards the temperature of the environment $T_E$.

**Example 3.** (Newton‘s Law of Motion: Constant Gravity)  
Near the earth a body falls according to the law
$$\frac{d^2y}{dt^2}=-g$$
where $y$ is the height of the body above the Earth and $g$ is the acceleration due to gravity, $9.8 m/sec^2$.

**Example 4.** (Newton's Law of Gravitation)  
Newton's law of gravity says that the acceleration due to gravity of a body at distance $r$ from the center of the Earth is
$$\frac{d^2r}{dt^2}=-GM_E/r^2$$
where $M_E$ is the mass of the Earth and $G$ is the universal gravitational constant.

**Example 5.** (Simple Harmonic Oscillator: Hooke's Law)  
Suppose a body of mass $m$ is attached to a spring. Let $x$ be the amount the spring is stretched from its unstretched equilibrium position. Hooke's law combined with Newton's law of motion says
$$mx''=-kx \lrArr mx''+kx=0$$
where $k$ is the spring constant. The minus sign indicates that the force always points back towards equilibrium, as it does in the real world.

**Example 6.** (Damped Harmonic Oscillator)  
If we add a damping force proportional to velocity to the spring-mass system in example 5, we get
$$mx''=-kx-bx' \lrArr mx''+bx+kx=0$$
here $-bx$ is the damping force and $b$ is called the **damping constant**.

**Example 7.** (Damped Harmonic Oscillator with an External Force)  
If we add a time varying external force $F(t)$ to the system in example 6, we get
$$mx''=-kx-bx'+F(t) \lrArr mx''+bx'+kx=F(t)$$
