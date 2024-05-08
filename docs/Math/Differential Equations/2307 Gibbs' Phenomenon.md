In practice it may be impossible to use all the terms of a Fourier series. For example, suppose we have a device that manipulates a periodic signal by first finding the Fourier series of the signal, then manipulating the sinusoidal components, and, finally, reconstructing the signal by adding up the modified Fourier series. Such a device will only be able to use a finite number of terms of the series.

Gibbs' phenomenon occurs near a jump discontinuity in the signal. It says that no matter how many terms you include in your Fourier series there will always be an error in the form of an overshoot near the discontinuity. The overshoot always be about 9% of the size of the jump.

We illustrate with the example. of the square wave $sq(t)$. The Fourier series of $sq(t)$ fits it well at points of continuity. But there is *always* an overshoot of about .18 (9% of the jump of 2) near the points of discontinuity.

![](pic230701.png)

In these figures, for example, 'max n=9' means we included the terms for $n = 1, 3, 5, 7$ and 9 in the Fourier sum
$$\frac{4}{\pi}(\sin t+\frac{\sin 3t}{3}+\frac{\sin 5t}{5}+\frac{\sin 7t}{7}+\frac{\sin 9t}{9})$$
