**Ex 1.**  
(a) Verify that $\frac{1}{\omega}\sin(\omega_n t)u(t)$ is the unit impulse response of $D^2 + \omega_n^2 I$.  
(b) Find the solution to $x'' + x = \sin t$ with initial conditions $x(0) = x'(0) = 0$, using the ERF/resonance.  
(c) By the Assertion, $\sin t * \sin t$ should match the solution found in (b) for $t > 0$. Verify this by computing $\sin t * \sin t$ directly. (Hint: $sin(t-\tau) = \sin t \cos \tau - \cos t \sin \tau.$)  
**Solution.**  
(a) Check that $x(t)$ is a homogeneous solution which has $x(t) = 0$ for $t < 0$ and satisfies the required initial conditions to be the unit impulse response of this operator.  
Differentiate to obtain $Dx = \cos(\omega_nt)$ and $D^2x = -\omega_n \sin(\omega_n t)$ (for $t > 0$). So $(D^2 + \omega_n^2 I)x = 0$ when $t > 0$.  
Moreover, we can check that $x$ satisfies the required initial conditions in this case: $x(0^+) = 0$ and $x'(0^+) = 1 = 1/1$, where 1 (denominator) is the coefficient of the leading term $D^2$ in this differential operator.

(b) We will need to find the general solution to this equation and use the initial conditions to solve for the right constants (i.e. to find the particular solution that satisfies these initial conditions).  
As usual, begin by finding a particular solution. The complex replacement of this equation is $z''+ z = e^{it}$ (and $x = Im (z)$). By the Resonant ERF, the complex equation has the particular solution $z_p = \frac{te^{it}}{2i}$. So the original equation has a particular solution $x_p = Im (z_p) = -\frac{t}{2}\cos t$.  
Then read off the homogeneous solutions from the system. This system has characteristic polynomial $r^2 + 1$ with roots $\pm i$, so the homogeneous solutions to this system are of the form $c_1 \cos t + c_2 \sin t$.  
So by linearity, the general solution to this equation is
$$x=-\frac{t}{2}\cos t+c_1 \cos t + c_2 \sin t$$
Now we want $x(0) = c_1 = 0$ and $x'(0)=-\frac{1}{2}+c_2=0$. So we must have $c_1 = 0$ and $c_2 = 1/2$ , and the particular solution we are looking for is
$$x=-\frac{t}{2}\cos t+\frac{1}{2}\sin t$$

(c) From the definitions, $\sin t * \sin t$ is
$$\begin{aligned}
\int_0^t \sin \tau \sin(t-\tau)d\tau&=\int_0^t \sin \tau \sin t \cos \tau - \sin \tau \cos t \sin \tau d\tau\\
&=\int_0^t \sin t \frac{\sin(2\tau)}{2}-\cos t(\sin \tau)^2 d\tau\\
&=\int_0^t \sin t \frac{\sin(2\tau)}{2}+\cos t\frac{\cos(2\tau)-1}{2} d\tau\\
&=(\sin t \frac{-\cos(2\tau)}{4})\bigg|_0^t+(\cos t\frac{\sin(2\tau)-2\tau}{4})\bigg|_0^t\\
&=(\sin t \frac{1-\cos(2t)}{4})+(\cos t\frac{\sin(2t)-2t}{4})\\
&=\frac{1}{4}(\sin t-\sin t\cos(2t)+\cos t\sin(2t)-2t\cos t)\\
&=\frac{1}{4}(\sin t+\sin(2t-t)-2t\cos t)\\
&=-\frac{t}{2}\cos t+\frac{1}{2}\sin t\\
\end{aligned}$$
for $t > 0$, which, over $t > 0$, matches the solution $x(t)$ we found in (b).

**Ex 2.**  
Let $q(t) = \cos(\omega t)$. Compute $w(t)*q(t)$ (where $w(t)$ is the unit impulse response for $D + kI$ and verify that it is the solution to $x + kx = q(t)$ with rest initial conditions.  
**Solution.**  
$$w(t) = e^{-kt}$$
$$\begin{aligned}
x(t)&=(w*q)(t)\\
&=(q*w)(t)\\
&=\int_0^t \cos(\omega \tau) e^{-k(t-\tau)}d\tau\\
&=e^{-kt}\int_0^t e^{k\tau}Re(e^{i\omega \tau})d\tau\\
&=e^{-kt}\int_0^t Re(e^{(i\omega+k) \tau})d\tau\\
&=e^{-kt} Re(\frac{e^{(i\omega+k) \tau}}{i\omega+k}\bigg|_0^t)\\
&=e^{-kt} Re(\frac{e^{(i\omega+k)t}-1}{i\omega+k})\\
&=e^{-kt}\frac{1}{k^2+\omega^2} Re(e^{kt}(k-i\omega)(\cos \omega t+i\sin \omega t)-k+i\omega)\\
&=e^{-kt}\frac{1}{k^2+\omega^2} (e^{kt}(k\cos \omega t+\omega \sin \omega t)-k)\\
&=\frac{k\cos \omega t+\omega \sin \omega t-ke^{-kt}}{k^2+\omega^2}\\
\end{aligned}$$
$$x'(t)=\frac{-k\omega\sin \omega t+\omega^2 \cos \omega t+k^2e^{-kt}}{k^2+\omega^2}$$
$$\begin{aligned}
x'+kx&=\frac{-k\omega\sin \omega t+\omega^2 \cos \omega t+k^2e^{-kt}}{k^2+\omega^2}+\frac{k^2\cos \omega t+k\omega \sin \omega t-k^2e^{-kt}}{k^2+\omega^2}\\
&=\frac{\omega^2 \cos \omega t}{k^2+\omega^2}+\frac{k^2\cos \omega t}{k^2+\omega^2}\\
&=\cos \omega t
\end{aligned}$$
and $x(0)=0$
