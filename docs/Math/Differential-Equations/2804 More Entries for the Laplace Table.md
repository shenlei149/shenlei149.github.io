In this note we will add some new entries to the table of Laplace transforms.

**1.** $\mathcal{L}(\cos(\omega t))=\frac{s}{s^2+\omega^2}$, with region of convergence Re$(s) > 0$.  
**2.** $\mathcal{L}(\sin(\omega t))=\frac{\omega}{s^2+\omega^2}$, with region of convergence Re$(s) > 0$.  
**Proof:** We already know that $\mathcal{L}(e^{at}) = 1/(s - a)$. Using this and Euler's formula for the complex exponential, we obtain
$$\mathcal{L}(\cos(\omega t)+i\sin(\omega t))=\mathcal{L}(e^{i\omega t})=\frac{1}{s-i\omega}=\frac{1}{s-i\omega}\cdot \frac{s+i\omega}{s+i\omega}=\frac{s+i\omega}{s^2+\omega^2}$$
Taking the real and imaginary parts gives us the formulas.
$$\begin{aligned}
\mathcal{L}(\cos(\omega t))&=\text{Re}(\mathcal{L}(e^{i\omega t}))&=&s/(s^2+\omega^2)\\
\mathcal{L}(\sin(\omega t))&=\text{Im}(\mathcal{L}(e^{i\omega t}))&=&\omega/(s^2+\omega^2)
\end{aligned}$$
The region of convergence follow from the fact that $\cos(\omega t)$ and $\sin(\omega t)$ both have exponential order 0.  
Another approach would have been to use integration by parts to compute the transforms directly from the Laplace integral.  
**3.** For a positive integer $n, \mathcal{L}(t^n) = n!/s^{n+1}$. The region of convergence is Re$(s) > 0$.  
**Proof:** We start with $n = 1$.
$$\mathcal{L}(t)=\int_0^\infty te^{-st}dt$$
Using integration by parts:
$$\begin{aligned}
&u=t&&dv=e^{-st}\\
&du=1&&v=e^{-st}/(-s)
\end{aligned}\bigg\} = \mathcal{L}(t)=\frac{te^{-st}}{-s}\bigg|_0^\infty+\frac{1}{s}\int_0^\infty e^{-st}dt$$
For Re$(s) > 0$ the first term is 0 and the second term is $\frac{1}{s}\mathcal{L}(1) =1/s^2$. Thus, $\mathcal{L}(t)=1/s^2$.  
Next let's do $n = 2$:
$$\mathcal{L}(t^2)=\int_0^\infty t^2e^{-st}dt$$
Again using integration by parts:
$$\begin{aligned}
&u=t^2&&dv=e^{-st}\\
&du=2t&&v=e^{-st}/(-s)
\end{aligned}\bigg\} = \mathcal{L}(t^2)=\frac{t^2e^{-st}}{-s}\bigg|_0^\infty+\frac{1}{s}\int_0^\infty 2te^{-st}dt$$
For Re$(s) > 0$ the first term is 0 and the second term is $\frac{1}{s}\mathcal{L}(2t) =2/s^3$. Thus, $\mathcal{L}(t^2)=2/s^3$.  
We can see the pattern: there is a reduction formula for 
$$\mathcal{L}(t^n)=\int_0^\infty t^ne^{-st}dt$$
Integration by parts:
$$\begin{aligned}
&u=t^n&&dv=e^{-st}\\
&du=nt^{n-1}&&v=e^{-st}/(-s)
\end{aligned}\bigg\} = \mathcal{L}(t^n)=\frac{t^ne^{-st}}{-s}\bigg|_0^\infty+\frac{1}{s}\int_0^\infty nt^{n-1}e^{-st}dt$$
For Re$(s) > 0$ the first term is 0 and the second term is $\frac{1}{s}\mathcal{L}(nt^{n-1})$. Thus, $\mathcal{L}(t^n)=\frac{n}{s}\mathcal{L}(t^{n-1})$.  
Thus we have
$$\begin{aligned}
\mathcal{L}t^3&=\frac{3}{s}\mathcal{L}(t^2)&=&\frac{3\cdot 2}{s^4}&=\frac{3!}{s^4}\\
\mathcal{L}t^4&=\frac{4}{s}\mathcal{L}(t^3)&=&\frac{4\cdot 3!}{s^5}&=\frac{4!}{s^5}\\
\ldots\\
\mathcal{L}(t^n)&=\frac{n!}{s^{n+1}}
\end{aligned}$$
**4.** ($s$-shift formula) If $z$ is any complex number and $f(t)$ is any function then
$$\mathllap{L}(e^{zt}f(t))=F(s-z)$$
As usual we write $F(s) = \mathcal{L}(f)(s)$. If the region of convergence for $\mathcal{L}(f)$ is Re$(s) > a$ then the region of convergence for $\mathllap{L}(e^{zt}f(t))$ is Re$(s)>$ Re$(z)+a$.  
**Proof:** We simply calculate
$$\begin{aligned}
\mathllap{L}(e^{zt}f(t))&=\int_0^\infty e^{zt}f(t)e^{-st}dt\\
&=\int_0^\infty f(t)e^{-(s-z)t}dt\\
&=F(s-z)
\end{aligned}$$

**Example.** Find the Laplace transform of $e^{-t} \cos(3t)$.
**Solution.** use the $s$-shift formula with $z = -1$, which gives
$$\mathcal{L}(e^{-t}f(t))=F(s+1)$$
where here $f(t) = \cos(3t)$, so that $F(s) = s/(s^2 + 9)$. Shifting $s$ by -1 according to the $s$-shift formula gives
$$\mathcal{L}(e^{-t} \cos(3t))=F(s+1)=\frac{s+1}{(s+1)^2+9}$$
We record two important cases of the $s$-shift formula:  
**4a)** $\mathcal{L}(e^{zt}\cos(\omega t))=\frac{s-z}{(s-z)^2+\omega^2}$  
**4b)** $\mathcal{L}(e^{zt}\sin(\omega t))=\frac{\omega}{(s-z)^2+\omega^2}$

### Consistency
It is always useful to check for consistency among our various formulas:
1. We have $\mathcal{L}(1) = 1/s$, so the $s$-shift formula gives $\mathcal{L}(e^{zt}\cdot 1) = 1/(s - z)$. This matches our formula for $\mathcal{L}(e^{zt})$.
2. We have $\mathcal{L}(t^n) = n!/s^{n+1}$. If $n = 1$ we have $\mathcal{L}(t^0) = 0!/s = 1/s$. This matches our formula for $\mathcal{L}(1)$.
