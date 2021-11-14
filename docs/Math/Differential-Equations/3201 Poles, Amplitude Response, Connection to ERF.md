For our standard LTI system $p(D)x = f$, the transfer function is $W(s) = 1/p(s)$. In this case the poles of $W(s)$ are simply the zeros of the characteristic polynomial $p(s)$ (also known as the characteristic roots). We have had lots of experience using these roots in this course and know they give important information about the system. The reason we talk about the poles of the transfer function instead of just sticking with the characteristic roots is that all LTI systems have a transfer function, while the characteristic polynomial is defined only for systems described by constant coefficient linear ODE's.  
Much of units 1,2 and the current unit have been devoted to the study of DE's for LTI systems:
$$p(D)x=f(t)\tag{1}$$
In units 1 and 2 we saw that the stability of the system is determined by the roots of the characteristic polynomial. We saw as well that the amplitude response of the system to a sinusoidal input of frequency $\omega$ is also determined by the characteristic polynomial, namely as the gain
$$g(\omega)=\frac{1}{p(i\omega)}$$
In this unit we've learned about the Laplace transform, which gives us another view of a signal by transforming it from a function of $t$, say $f(t)$, to a function $F(s)$ of the complex frequency $s$.  
A key object from this point of view is the transfer function. For the system $(1)$, if we consider $f(t)$ to be the input and $x(t)$ to be the output, then the transfer function is $W(s) = 1/p(s)$, which is again determined by the characteristic polynomial.  
In this session we will learn about poles and the pole diagram of an LTI system. This ties together the notions of stability, amplitude response and transfer function, all in one diagram in the complex $s$-plane. The pole
diagram gives us a way to visualize systems which makes many of their important properties clear at a glance; in particular, and remarkably, the pole diagram
1. shows whether the system stable;
2. shows whether the unforced system is oscillatory;
3. shows the exponential rate at which the unforced system returns to equilibrium (for stable systems); and
4. gives a rough picture of the amplitude response and practical resonances of the system.

For these reasons the pole diagram is a standard tool used by engineers in understanding and designing systems.  
We conclude by reminding you that every LTI system has a transfer function. Everything we learn in this session will apply to such systems, including those not modeled by DE's of the form $(1)$
