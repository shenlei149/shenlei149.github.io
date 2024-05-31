In real life, we often do not know the parameters of a system (e.g. the spring constant, the mass, and the damping constant, in a spring-mass-dashpot system). We may not even know the order of the system. For example, there may be many interconnected springs or diodes. Instead, we often learn about a system by watching how it responds to various input signals.

In this session we will study the response of a linear time invariant (LTI) system from rest initial conditions to two standard and very simple signals: the unit impulse $\delta(t)$ and the unit step function $u(t)$. Reasonably enough we will call these responses the *unit impulse response* and the *unit step response*.

The theory of the convolution integral studied in the next session will give us a method of dertemining the response of a system to *any* input once we know its unit impulse response.

Because both $\delta(t)$ and $u(t)$ are discontinuous at $t = 0$ we will have to be careful with our definition of initial conditions. The most sensible mathematical and physical way to do this is to define our initial conditions at $0^-$. As input an impulse causes a jump when it is applied. This means that the conditions at $0^+$ will be different than those at $0^-$. To distinguish these two cases we will use the terms *pre-initial conditions* (at $0^-$) and *post-initial conditions* (at $0^+$). We will be able to state precisely the effect of a unit impulse on these conditions.