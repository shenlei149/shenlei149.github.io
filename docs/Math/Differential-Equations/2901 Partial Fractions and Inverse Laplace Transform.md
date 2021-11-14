In order to use the Laplace transform we need to be able to invert it and find $f(t)$ when we're given $F(s)$. Often this can be done by using the Laplace transform table. So for example, if $F(s) = 1/(s-5)$ then $f(t) = e^{5t}$.

More often we have to do some algebra to get $F(s)$ into a form suitable for the direct use of the table. Our main technique for doing this is the partial fractions decomposition. You probably saw this before in calculus as a method for computing integrals.

First we will learn how to do partial fractions in a straightforward algebraic way using the *method of undetermined coefficients*. Next we will learn the Heaviside coverup method which makes some of the algebra easier.