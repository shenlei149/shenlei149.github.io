We can integrate a Fourier series term-by-term:  
**Example 1**. Let
$$
f(t)=1+\cos t+\frac{\cos 2t}{2}+\frac{\cos 3t}{3}+\ldots
$$
then,
$$
h(t)=\int_0^tf(u)du=t+\sin t+\frac{\sin 2t}{2^2}+\frac{\sin 3t}{3^2}+\ldots
$$
**Note:** The integrated function $h(t)$ is not periodic (because of the $t$ term), so the result is a series, but not a Fourier series.

We can also differentiate a Fourier series term-by-term to get the Fourier series of the derivative function.  
**Example 2.** Let $f(t)$ be the period $2\pi$ triangle wave (continuous sawtooth) given on the interval $[-\pi, \pi)$ by $f(t) = |t|$. Its Fourier series is
$$
f(t)=\frac{\pi}{2}-\frac{4}{\pi}(\cos t+\frac{\cos 3t}{3^2}+\frac{\cos 5t}{5^2}+\ldots)
$$
In the previous session we computed the Fourier series of a period 2 triangle wave. This series can then be obtained from that one by scaling by $\pi$ in both time and the vertical dimension, using the methods we learned in the previous note.  
The derivative of $f(t)$ is the square wave. Differentiating the Fourier series of $f(t)$ term-by-term gives
$$
f'(t)=\frac{4}{\pi}(\sin t+\frac{\sin 3t}{3}+\frac{\sin 5t}{5}+\ldots)
$$

**Example 3.** What happens if you try to differentiate the square wave
$$
sq(t)=\frac{4}{\pi}(\sin t+\frac{\sin 3t}{3}+\frac{\sin 5t}{5}+\ldots)
$$
**Solution.** Differentiation term-by-term gives
$$
sq'(t)=\frac{4}{\pi}(\cos t+\cos 3t+cos 5t+\ldots)
$$
But, what is meant by $sq'(t)$? Since $sq(t)$ consists of horizontal segments its derivative at most places is 0. However we can't ignore the 'vertical' segments where the function has a *jump discontinuity*. For now, the best we can say is that the slope is infinite at these jumps and $sq'(t)$ doesn't exist. Later in this unit we will learn about *delta functions* and *generalized derivatives*, which will allow us to make better sense of $sq'(t)$.
