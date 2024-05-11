It is actually just as easy to write out the formula for the Fourier series expansion of the steady-periodic solution $x_{sp}(t)$ to the general secondorder LTI DE $p(D)x = f(t)$ with $f(t)$ periodic as it was to work out the previous example - the only difference is that now we use letters instead of numbers. We will choose the letters used for the spring-mass-dashpot system, but clearly the derivation and formulas will work with any three parameters.
For simplicity we will take the case of $f(t)$ even (i.e. cosine series).

**Problem:** Solve $mx''+bx'+kx=f(t)$, for the steady-periodic response $x_{sp}(t)$, where $f(t)=\frac{a_0}{2}+\sum_{n=1}^\infty a_n\cos(n\frac{\pi}{L}t)dt$

**Solution**  
Characteristic polynomial: $p(s) = ms^2 + bs + k$.  
Solving for the component pieces:
$$mx_n''+bx_n'+kx_n=\cos(n\frac{\pi}{L}t)$$
For $n=0$ we get $x_{0,p}=\frac{1}{k}$.  
For $n\geq1$:  
Complex replacement:
$$mz_n''+bz_n'+kz_n=e^{in\frac{\pi}{L}t}, x_n=Re(z_n)$$
Exponential Response formula:
$$z_{n,p}(t)=\frac{e^{in\frac{\pi}{L}t}}{p(in\frac{\pi}{L})}$$
Polar coords:
$$p(in\frac{\pi}{L})=(k-m(n\frac{\pi}{L})^2)+ibn\frac{\pi}{L}=|p(in\frac{\pi}{L})|e^{i\phi_n}$$
where 
$$|p(in\frac{\pi}{L})|=\sqrt{(k-m(n\frac{\pi}{L})^2)^2+b^2(n\frac{\pi}{L})^2}$$
and
$$\phi_n=Arg(p(in\frac{\pi}{L}))=\tan^{-1}(\frac{bn\frac{\pi}{L}}{k-m(n\frac{\pi}{L})^2})\tag{phase lag}$$
Thus,
$$z_{n,p}(t)=g_ne^{i(n\frac{\pi}{L}t-\phi_n)}$$
with
$$g_n=\frac{1}{|p(in\frac{\pi}{L})|}\tag{gain}$$
Taking the real part of $x_{n,p}$ we get
$$x_{n,p}(t)=g_n\cos(n\frac{\pi}{L}t-\phi_n)$$
Now using superposition and putting back in the coefficients $a_n$ we get:
$$x_{sp}(t)=\frac{a_0}{2}x_{0,p}+\sum_{n=1}^\infty a_n x_{n,p}(t)=\frac{a_0}{2k}+\sum_{n=1}^\infty a_n g_n \cos(n\frac{\pi}{L}t-\phi_n)$$
This is the general formula for the steady periodic response of a secondorder LTI DE to an even periodic driver $f(t)$.
