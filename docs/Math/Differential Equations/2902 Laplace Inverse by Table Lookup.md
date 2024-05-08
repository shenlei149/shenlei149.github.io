The first thing we need to be able to do is to use the Laplace table to find the inverse Laplace transform. We will illustrate this entirely by examples.

**Notation:** The inverse Laplace transform will be denoted $\mathcal{L}^{-1}$.

**Example 1.** Find $\mathcal{L}^{-1}(1/(s-2))$.  
**Solution.** Use the table entry $\mathcal{L}(e^{at})=1/(s-a)$:
$$\mathcal{L}^{-1}(1/(s-2))=e^{2t}$$

**Example 2.** Find $\mathcal{L}^{-1}(1/(s^2 + 9))$.  
**Solution.** Use the table entry $\mathcal{L}(\sin(\omega t))=\omega/(s^2+\omega^2)$ and linearity:
$$\mathcal{L}^{-1}(1/(s^2 + 9))=\frac{1}{3}\mathcal{L}^{-1}(3/(s^2 + 9))=\frac{1}{3}\sin(3t)$$

**Example 3.** Find $\mathcal{L}^{-1}(4/s^2)$.  
**Solution.** Use the table entry $\mathcal{L}(t) = 1/s^2$: $\mathcal{L}^{-1}(4/s^2) = 4t$.

**Example 4.** Find $\mathcal{L}^{-1}(4/(s - 2)^2)$.  
**Solution.** Use the $s$-shift formula $\mathcal{L}(e^{zt}f(t)) = F(s-2)$, where, in this case, $F(s) = 4/s^2 \rArr f(t) = 4t$ by example (3).  
Therefore, $\mathcal{L}^{-1}(4/(s - 2)^2) = \mathcal{L}^{-1}(F(s-2)) = e^{2t}f(t) = e^{2t} 4t$.

**Example 5.** Find $\mathcal{L}^{-1}(\frac{1}{s^2+4s+13})$.  
**Solution.** We first need to *complete the square*
$$s^2+4s+13=s^2+4s+4+9=(s+2)^2+9$$
We have a shifted function $F(s + 2)$, where $F(s) = 1/(s^2 + 9)$. Using example (2), we know that $f(t) = \sin(3t)/3$, so using the $s$-shift rule we get
$$\mathcal{L}^{-1}(\frac{1}{s^2+4s+13})=\mathcal{L}^{-1}(F(s+2))=e^{-2t}f(t)=e^{-2t}\frac{\sin(3t)}{3}$$

**Example 6.** Find $\mathcal{L}^{-1}(\frac{s}{(s^2+\omega^2)^2})$.  
**Solution.** We haven't seen this formula yet, but there is a table entry, which gives: $\frac{t}{2\omega}\sin(\omega t)$.

**Example 7.** Find $\mathcal{L}^{-1}(\frac{1}{(s^2+\omega^2)^2})$.  
**Solution.** This is also a table entry, answer: $\frac{1}{2\omega^3}(\sin(\omega t)-\omega t\cos(\omega t))$.
