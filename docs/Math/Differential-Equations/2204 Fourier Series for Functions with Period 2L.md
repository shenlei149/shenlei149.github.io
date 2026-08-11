Suppose that we have a periodic function $f(t)$ with arbitrary period $P = 2L$, generalizing the special case $P = 2\pi$ which we have already seen. Then a simple re-scaling of the interval $(-\pi, \pi)$ to $(-L, L)$ allows us to write down the general Fourier series and Fourier coefficent formulas:
$$f(t)=\frac{a_0}{2}+\sum_{n=1}^{\infty}a_n\cos (n\frac{\pi}{L}t)+b_n\sin (n\frac{\pi}{L}t)\tag{1}$$
with Fourier coefficients given by the general Fourier coefficent formulas 
$$
a_0=\frac{1}{L}\int_{-L}^{L}f(t)dt\\
a_n=\frac{1}{L}\int_{-L}^{L}f(t)\cos(n\frac{\pi}{L}t)dt\\
b_n=\frac{1}{L}\int_{-L}^{L}f(t)\sin(n\frac{\pi}{L}t)dt\tag{2}
$$
Note: The number $L = \frac{P}{2}$ is called the **half-period**.

### Example
Let $f(t)$ be the period 2 function, which is defined on the window $[-1, 1)$ by $f(t) = |t|$. Compute the Fourier series of $f(t)$.

**Solution** In this case the period is $P = 2$, so the half-period $L = 1$. This means $n\frac{\pi}{L}=n\pi$ and we compute the coefficients from the formulas in $(2)$,
using integration by parts, as follows.  
For $n\neq0$
$$
\begin{aligned}
a_n&=\frac{1}{L}\int_{-L}^{L}f(t)\cos(n\frac{\pi}{L}t)dt\\
&=\frac{1}{1}\int_{-1}^{1}|t|\cos(n\pi t)dt\\
&=2\int_0^1t\cos(n\pi t)dt\\
&=2(\frac{t\sin n\pi t}{n\pi}\bigg|_0^1-\int_0^1\frac{\sin n\pi t}{n\pi}dt)\\
&=2(\frac{t\sin n\pi t}{n\pi}+\frac{\cos n\pi t}{n^2\pi^2})\bigg|_0^1\\
&=2(\frac{\cos n\pi}{n^2\pi^2}-\frac{1}{n^2\pi^2})\\
&=\frac{2}{n^2\pi^2}((-1)^n-1)\\
&=\begin{cases}
-\frac{4}{n^2\pi^2}&\text{for $n$ odd}\\
0&\text{for $n$ even}
\end{cases}
\end{aligned}
$$
and for $n=0$
$$a_0=\frac{1}{1}\int_{-1}^{1}|t|dt=2\int_0^1tdt=1$$
Since $f(t)$ is an even function and $\sin(n\pi t)$ is odd, the sine coefficients $b_n = 0$. (We will justify this carefully in the next session. For now you can compute the integrals for $b_n$ as an exercise and verify it in this case.)

Thus, the Fourier series for $f(t)$ is
$$f(t)=\frac{1}{2}-\frac{4}{\pi^2}(\cos \pi t+\frac{\cos 3\pi t}{3^2}+\frac{\cos 5\pi t}{5^2}+\ldots)=\frac{1}{2}-\frac{4}{\pi^2}\sum_{n\text{ odd}}\frac{\cos n\pi t}{n^2}$$
