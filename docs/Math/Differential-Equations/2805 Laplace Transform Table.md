### Properties and Rules
___
*learn in this session*
$$\begin{aligned}
&f(t) &&F(s)=\int_{0^-}^\infty f(t)e^{-st}dt &&\text{(Definition)}\\
&af(t)+bg(t) &&aF(s)+bG(s) &&\text{(Linearity)}\\
&e^{zt}f(t) &&F(s-z) &&\text{($s$-shift)}
\end{aligned}$$
___
*Will learn in a future session*
$$\begin{aligned}
&f'(t) &&sF(s)-f(0^-)\\
&f''(t) &&s^2F(s)-sf(0^-)-f'(0^-)\\
&f^{(n)}(t) &&s^nF(s)-s^{n-1}f(0^-)-\cdots-f^{(n-1)}(0^-)\\
&tf(t) &&-F'(s)\\
&t^nf(t) &&(-1)^nF^{(n)}(s)\\
&u(t-a)f(t-a) &&e^{-as}F(s) &&\text{($t$-translation)}\\
&u(t-a)f(t) &&e^{-as}\mathcal{L}(f(t+a)) &&\text{($t$-translation)}\\
&(f*g)(t)=\int_{0^-}^{t^+}f(t-\tau)g(\tau)d\tau &&F(s)G(s)\\
&\int_{0^-}^{t^+}f(\tau)d\tau &&\frac{F(s)}{s} &&\text{(integration rule)}\\
\end{aligned}$$
___
*Interesting, but not included in this course*
$$\begin{aligned}
&\frac{f(t)}{t} &&\int_s^\infty F(\sigma)d\sigma
\end{aligned}$$
___

### Function Table
___
*learn in this session*
$$\begin{aligned}
&1 &&1/s &&\text{Re}(s)>0\\
&e^{at} &&1/(s-a) &&\text{Re}(s)>a\\
&t &&1/s^2 &&\text{Re}(s)>0\\
&t^n &&n!/s^{n+1} &&\text{Re}(s)>0\\
&\cos(\omega t) &&s/(s^2+\omega^2) &&\text{Re}(s)>0\\
&\sin(\omega t) &&\omega/(s^2+\omega^2) &&\text{Re}(s)>0\\
&e^{zt}\cos(\omega t) &&(s-z)/((s-z)^2+\omega^2) &&\text{Re}(s)>\text{Re}(z)\\
&e^{zt}\sin(\omega t) &&\omega/((s-z)^2+\omega^2) &&\text{Re}(s)>\text{Re}(z)\\
&\delta(t) &&1 &&\text{all }s\\
&\delta(t-a) &&e^{-as} &&\text{all }s\\
&\cosh(kt)=\frac{e^{kt}+e^{-kt}}{2} &&s/(s^2-k^2) &&\text{Re}(s)>k\\
&\sinh(kt)=\frac{e^{kt}-e^{-kt}}{2} &&k/(s^2-k^2) &&\text{Re}(s)>k
\end{aligned}$$
___
*Will learn in a future session*
$$\begin{aligned}
&\frac{1}{2\omega^3}(\sin(\omega t)-\omega t\cos(\omega t)) &&\frac{1}{(s^2+\omega^2)^2} &&\text{Re}(s)>0\\
&\frac{t}{2\omega}\sin(\omega t) &&\frac{s}{(s^2+\omega^2)^2} &&\text{Re}(s)>0\\
&\frac{1}{2\omega}(\sin(\omega t)+\omega t\cos(\omega t)) &&\frac{s^2}{(s^2+\omega^2)^2} &&\text{Re}(s)>0\\
&u(t-a) &&e^{-as}/s &&\text{Re}(s)>0\\
&t^ne^{at} &&n!/(s-a)^{n+1} &&\text{Re}(s)>a
\end{aligned}$$
___
*Interesting, but not included in this course*
$$\begin{aligned}
&\frac{1}{\sqrt{\pi t}} &&\frac{1}{\sqrt{s}} &&\text{Re}(s)>0\\
&t^a &&\frac{\Gamma(a+1)}{s^{a+1}} &&\text{Re}(s)>0
\end{aligned}$$
___
