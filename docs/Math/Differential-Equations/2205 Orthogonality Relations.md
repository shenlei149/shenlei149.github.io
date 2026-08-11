We now explain the basic reason why the remarkable Fourier coefficent formulas work. We begin by repeating them from the last note:
$$
a_0=\frac{1}{L}\int_{-L}^{L}f(t)dt\\
a_n=\frac{1}{L}\int_{-L}^{L}f(t)\cos(n\frac{\pi}{L}t)dt\\
b_n=\frac{1}{L}\int_{-L}^{L}f(t)\sin(n\frac{\pi}{L}t)dt\tag{1}
$$
The key fact is the following collection of integral formulas for sines and cosines, which go by the name of **orthogonality relations**:
$$
\frac{1}{L}\int_{-L}^L\cos(n\frac{\pi}{L}t)\cos(m\frac{\pi}{L}t)dt=\begin{cases}
1 &n=m\neq 0\\
0 &n\neq m\\
2 &n=m=0
\end{cases}
\tag{2}
$$
$$
\frac{1}{L}\int_{-L}^L\cos(n\frac{\pi}{L}t)\sin(m\frac{\pi}{L}t)dt=0\tag{3}
$$
$$
\frac{1}{L}\int_{-L}^L\sin(n\frac{\pi}{L}t)\sin(m\frac{\pi}{L}t)dt=\begin{cases}
1 &n=m\neq 0\\
0 &n\neq m
\end{cases}\tag{4}
$$
**Proof of the orthogonality relations:**  
For $(2)$
$$
\frac{1}{L}\int_{-L}^L\cos(n\frac{\pi}{L}t)\cos(m\frac{\pi}{L}t)dt=\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)+\cos((n-m)\frac{\pi}{L}t)dt
$$
For $n=m\neq 0$,
$$
\begin{aligned}
\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)+\cos((n-m)\frac{\pi}{L}t)dt&=\frac{1}{2L}\int_{-L}^L[\cos((n+m)\frac{\pi}{L}t)+1]dt\\
&=\frac{1}{2L}(\frac{L}{(n+m)\pi}\sin (n+m)\frac{\pi}{L}t)\bigg|_{-L}^L+\frac{1}{2L}\cdot 2L\\
&=\frac{1}{2L}\frac{L}{(n+m)\pi}(\sin (n+m)\pi +\sin (n+m)\pi) + 1\\
&=0+1=1
\end{aligned}
$$
For $n\neq m$,
$$
\sin (n-m)\pi=0 \text{ and }\sin (n+m)\pi=0 \Rightarrow\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)+\cos((n-m)\frac{\pi}{L}t)dt=0
$$
For $n=m=0$,
$$
\begin{aligned}
\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)+\cos((n-m)\frac{\pi}{L}t)dt&=\frac{1}{2L}\int_{-L}^L(1+1)dt\\
&=\frac{1}{2L}\cdot 4L=2
\end{aligned}
$$
For $(3)$  
sine is odd function and cosine is even function, so $\cos(n\frac{\pi}{L}t)\sin(m\frac{\pi}{L}t)$ is odd function, so
$$
\frac{1}{L}\int_{-L}^L\cos(n\frac{\pi}{L}t)\sin(m\frac{\pi}{L}t)dt=0
$$
For $(4)$
$$
\frac{1}{L}\int_{-L}^L\sin(n\frac{\pi}{L}t)\sin(m\frac{\pi}{L}t)dt=-\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)-\cos((n-m)\frac{\pi}{L}t)dt
$$
For $n\neq m$, similar to $(2)$, so the result is 0.

For $n=m\neq 0$,
$$
\begin{aligned}
-\frac{1}{2L}\int_{-L}^L\cos((n+m)\frac{\pi}{L}t)-\cos((n-m)\frac{\pi}{L}t)dt&=-\frac{1}{2L}\int_{-L}^L[\cos((n+m)\frac{\pi}{L}t)-1]dt\\
&=-\frac{1}{2L}(\frac{L}{(n+m)\pi}\sin (n+m)\frac{\pi}{L}t)\bigg|_{-L}^L+-\frac{1}{2L}\cdot -2L\\
&=\frac{1}{2L}\frac{L}{(n+m)\pi}(\sin (n+m)\pi +\sin (n+m)\pi) + 1\\
&=0+1=1
\end{aligned}
$$

### Using the orthogonality relations to prove the Fourier coefficient formula
Suppose we know that a periodic function $f(t)$ has a Fourier series expansion
$$f(t)=\frac{a_0}{2}+\sum_{n=1}^{\infty}a_n\cos (n\frac{\pi}{L}t)+b_n\sin (n\frac{\pi}{L}t)\tag{5}$$
How can we find the values of the coefficients? Let's choose one coefficient, say $a_2$, and compute it; you will easily how to generalize this to any other coefficient. The claim is that the right-hand side of the Fourier coefficient formula $(1)$, namely the integral
$$
\frac{1}{L}\int_{-L}^{L}f(t)\cos(2\frac{\pi}{L}t)dt
$$
is in fact the coefficent $a_2$ in the series $(5)$. We can replace $f(t)$ in this integral by the series in $(5)$ and multiply through by $\cos(2\frac{\pi}{L}t)$, to get
$$
\frac{1}{L}\int_{-L}^{L}\frac{a_0}{2}\cos(2\frac{\pi}{L}t)+\sum_{n=1}^{\infty}a_n\cos (n\frac{\pi}{L}t)\cos(2\frac{\pi}{L}t)+b_n\sin (n\frac{\pi}{L}t)\cos(2\frac{\pi}{L}t)dt
$$
Now the orthogonality relations tell us that almost every term in this sum will integrate to 0. In fact, the only non-zero term is the $n = 2$ cosine term
$$
\frac{1}{L}\int_{-L}^{L}a_2\cos(2\frac{\pi}{L}t)\cos(2\frac{\pi}{L}t)dt
$$
and the orthogonality relations for the case $n = m = 2$ show this integral is equal to $a_2$ as claimed.

### Why the denominator of 2 in $\frac{a_0}{2}$?
Answer: it is in fact just a convention, but the one which allows us to have the same Fourier coefficent formula for an when $n = 0$ and $n \geq 1$. (Notice that in the $n = m$ case for cosine, there is a factor of 2 only for $n = m = 0$.)
### Interpretation of the constant term $\frac{a_0}{2}$
We can also interpret the constant term $\frac{a_0}{2}$ in the Fourier series of $f(t)$ as the average of the function $f(t)$ over one full period: $\frac{a_0}{2}=\frac{1}{2L}\int_{-L}^{L}f(t)dt$.
