### Even and odd functions
**Definition.** A function $f(t)$ is **called** even if $f(-t) = f(t)$ for all $t$.  
The graph of an even function is symmetric about the $y$-axis.

We will need the following fact about the integral of an even function over a 'balanced' interval $[-L, L]$.  
If $f(t)$ is even then
$$\int_{-L}^{L}f(t)dt=2\int_{0}^{L}f(t)dt$$

**Definition.** A function $f(t)$ is called **odd** if $f(-t) = -f(t)$ for all $t$.  
The graph of an odd function is symmetric about the the origin.

We will need the following fact about the integral of an odd function over a 'balanced' interval $[-L, L]$.  
If $f(t)$ is odd then
$$\int_{-L}^{L}f(t)dt=0$$

### Multiplying Even and Odd Functions
When multiplying even and odd functions it is helpful to think in terms of multiply even and odd powers of $t$. This gives the following rules.
1. even × even = even
2. odd × odd = even
3. odd × even = odd

All this leads to the **even and odd Fourier coefficient rules**:  
Assume $f(t)$ is periodic then:
1. If $f(t)$ is even then we have $b_n = 0$, and $a_n=\frac{2}{L}\int_{0}^{L}f(t)\cos(n\frac{\pi}{L}t)dt$
2. If $f(t)$ is odd then we have $a_n = 0$, and $b_n=\frac{2}{L}\int_{0}^{L}f(t)\sin(n\frac{\pi}{L}t)dt$
