An operator takes a function as input and outputs another function. A *transform* does the same thing with the added twist that the output function has a *different* independent variable. The Laplace transform takes a function $f(t)$ and produces a function $F(s)$.  
We will allow the variable s to be complex. We will see that it can be thought of as *complex circular frequency*.  
It is best to think of $f(t)$ and $F(s)$ as two views of the same underlying phenomenon. If we have a signal then $f(t)$ is the familiar view of that signal in time and $F(s)$ is the less familiar view in frequency. Everything about the signal is present in both views, but some things are easier to see in one view or the other. Using them together gives us a powerful tool for understanding systems and signals.  
In practice the Laplace transform has the following benefits:
* It makes explicit the long-term behavior of $f(t)$.
* It converts differential equations into algebraic equations.
* It allows you to find the operator $P(D)$ if you know its weight function $w(t)$.
* It converts Green's formula, which is a complicated convolution integral in the time view, into as simple algebraic statement in the frequency view.
* Most importantly, the frequency view can be summarized in something called the pole diagram. This diagram can show at a glance the stability and frequency response of a system. It is an important engineering design tool.
