The *convolution product* of two functions is a peculiar looking integral which produces another function. It is found in a wide range of applications, so it has a special name and a special symbol. The convolution of $f$ and $g$ is denoted $f ∗ g$ and defined by
$$(f*g)(t)=\int_{0^-}^{t^+}f(s)g(t-s)ds$$
We will start by stating this formula without any motivation. It's main properties are relatively easy to deduce from its definition.

The motivation will come in the form of Green's formula. This important tool tells us how to solve a linear time invariant (LTI) system with any input (and rest IC) once we know its unit impulse response.

The rest of the session is concerned with the proof of the Green's formula and examples of convolution and Green's formula.

**Technical Detail:** Because we want convolution to work with delta functions we needed to be careful with the limits of integration. This explains the plus and minus on the limits. If both functions are continuous or have at most jump discontinuities then the limits can be 0 and $t$.
