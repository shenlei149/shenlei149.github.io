| $f(t)=\mathcal{L}^{-1}\{F(s)\}$ | $F(s)=\mathcal{L}\{f(t)\}$ |
|--|--|
| $1$ | $\frac{1}{s},s>0$ |
| $e^{at}$ | $\frac{1}{s-a},s>a$ |
| $t^n$ | $\frac{n!}{s^{n+1}},s>0$ |
| $t^p,p>-1$ | $\frac{\Gamma(p+1)}{s^{p+1}},s>0$ |
| $\sin at$ | $\frac{a}{s^2+a^2},s>0$ |
| $\cos at$ | $\frac{s}{s^2+a^2},s>0$ |
| $\sinh at$ | $\frac{a}{s^2-a^2},s>\|a\|$ |
| $\cosh at$ | $\frac{s}{s^2-a^2},s>\|a\|$ |
| $e^{at}\sin bt$ | $\frac{b}{(s-a)^2+b^2},s>a$ |
| $e^{at}\cos bt$ | $\frac{s-a}{(s-a)^2+b^2},s>a$ |
| $t^ne^{at}$ | $\frac{n!}{(s-a)^{n+1}},s>a$ |
| $u_c(t)\begin{cases}0&&t<c\\1&&t\geq c\end{cases}$ | $\frac{e^{-sc}}{s},s>0$ |
| $u_c(t)f(t-c)$ | $e^{-cs}F(s)$ |
| $e^{ct}f(t)$ | $F(s-c)$ |
| $f(ct)$ | $\frac{1}{c}F(\frac{s}{c}),c>0$ |
| $(f * g)(t)=\int_0^t f(t-\tau)g(\tau)d\tau$ | $F(s)G(s)$ |
| $\delta(t-c)$ | $e^{-cs}$ |
| $f^{(n)}(t)$ | $s^n\mathcal{L}\{f(t)\}-s^{n-1}f(0)\cdots-sf^{(n-2)}(0)-f^{(n-1)}(0)$ |
| $(-t)^nf(t)$ | $F^{(n)}(s)$ |