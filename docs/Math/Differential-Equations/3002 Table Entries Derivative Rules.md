### $t$-derivative rule
This is a course on differential equations. We should try to compute $\mathcal{L}(f')$.  
As usual, let $\mathcal{L}(f)(s) = F(s)$. Let $f'$ be the *generalized* derivative of $f$. (Recall, this means jumps in $f$ produce delta functions in $f'$.) The $t$-derivative rule is
$$\mathcal{L}(f')=sF(s)-f(0^-)\tag{1}$$
$$\mathcal{L}(f'')=s^2F(s)-sf(0^-)-f'(0^-)\tag{2}$$
$$\mathcal{L}(f^{(n)})=s^nF(s)-s^{(n-1)}f(0^-)-s^{(n-2)}f'(0^-)+\cdots-f^{(n-1)}(0^-)\tag{3}$$
**Proof:** Rule (1) is a simple consequence of the definition of Laplace transform and integration by parts.
$$\begin{aligned}
\mathcal{L}(f')&=\int_{0^-}^\infty f'(t)e^{-st}dt\\
&=f(t)e^{-st}\bigg|_{0^-}^\infty+s\int_{0^-}^\infty f(t)e^{-st}dt\\
&=sF(s)-f(0^-)
\end{aligned}$$
The last equality follows from:
1. We assume $f(t)$ has exponential order, so if Re$(s)$ is large enough $f(t)e^{-st}$ is 0 at $t = \infty$.
2. The integral in the second term is none other than the Laplace transform of $f(t)$.

Rule $(2)$ follows by applying rule $(1)$ twice.
$$\begin{aligned}
\mathcal{L}(f'')&=s\mathcal{L}(f')-f'(0^-)\\
&=s(sF(s)-f(0^-))-f'(0^-)\\
&=s^2F(s)-sf(0^-)-f'(0^-)
\end{aligned}$$
Rule $(3)$ Follows by applying rule $(1)$ $n$ times.  
**Notes:** 1. We will call the terms $f(0^-), f'(0^-)$ the 'annoying terms'. We will be happiest when our signal $f(t)$ has rest initial conditions, so all of the annoying terms are 0.  
2. A good way to think of the $t$-derivative rules is
$$\mathcal{L}(f)=F(s)$$
$$\mathcal{L}(f')=sF(s)+ \text{ annoying terms at } 0^-$$
$$\mathcal{L}(f'')=s^2F(s)+ \text{ annoying terms at } 0^-$$
Roughly speaking, Laplace transforms differentiation in $t$ to multiplication by $s$.  
3. The proof of rule $(1)$ uses integration by parts. This is clearly valid if $f'(t)$ is continuous at $t = 0$. It is also true (although we won't show this) if $f'(t)$ is a generalized function. - See example 2 below.

**Example 1.** Let $f(t) = e^{at}$. We can compute $\mathcal{L}(f')$ directly and by using rule $(1)$.  
Directly: $f'(t)=ae^{at} \rArr \mathcal{L}(f')=a/(s-a)$  
Rule $(1)$: $\mathcal{L}(f)=1/(s-a) \rArr \mathcal{L}(f')=sF(s)-f(0^-)=s/(s-a)-1=a/(s-a)$

**Example 2.** Let $u(t)$ be the unit step function, so $u'(t)=\delta(t)$.  
Directly: $\mathcal{L}(u')=\mathcal{L}(\delta)=1$  
Rule $(1)$: $\mathcal{L}(u')=s\mathcal{L}(u)-u(0^-)=s(1/s)-0=1$

Example 3. Let $f(t) = t^2 + 2t + 1$. Compute $\mathcal{L}(f'')$ two ways.  
Directly: $f''=2 \rArr \mathcal{L}(f'')=2/s$  
Rule $(1)$: $\mathcal{L}(f'')=s^2F(s)-sf(0^-)-f'(0^-)=s^2(2/s^3+2/s^2+1/s)-s\cdot 1-2=2/s$

### $s$-derivative rule
There is a certain symmetry in our formulas. If derivatives in time lead to multiplication by $s$ then multiplication by $t$ should lead to derivatives in $s$. This is true, but, as usual, there are small differences in the details of the formulas.  
The $s$-derivative rule is
$$\mathcal{L}(tf)(s)=-F'(s)\tag{4}$$
$$\mathcal{L}(t^nf)(s)=(-1)^nF^{(n)}(s)\tag{5}$$
**Proof:** Rule $(4)$ is a simple consequence of the definition of Laplace transform
$$\begin{aligned}
&&F(s)&=\mathcal{L}(f)=\int_{0^-}^\infty f(t)e^{-st}dt\\
&\rArr &F'(s)&=\frac{d}{ds}\int_{0^-}^\infty f(t)e^{-st}dt\\
&&&=\int_{0^-}^\infty -tf(t)e^{-st}dt\\
&&&=-\mathcal{L}(tf(t))
\end{aligned}$$
Rule $(5)$ is just rule $(4)$ applied $n$ times.

**Example 4.** Use the $s$-derivative rule to find $\mathcal{L}(t)$.  
**Solution.** Start with $f(t) = 1$, then $F(s) = 1/s$. The $s$-derivative rule now says $\mathcal{L}(t) = -F'(s) = 1/s^2$ - which we know to be the answer.

**Example 5.** Use the $s$-derivative rule to find $\mathcal{L}(te^{at})$ and $\mathcal{L}(t^ne^{at})$.  
**Solution.** Start with $f(t) = e^{at}$, then $F(s) = 1/(s-a)$. The $s$-derivative rule now says $\mathcal{L}(te^{at})=-F'(s)=1/(s-a)^2$  
Continuing: $\mathcal{L}(t^2e^{at})=F''(s)=2/(s-a)^3$, $\mathcal{L}(t^3e^{at})=-F'''(s)=3\cdot 2/(s-a)^4$, $\mathcal{L}(t^4e^{at})=F^{(4)}(s)=4\cdot 3\cdot 2/(s-a)^5$, $\mathcal{L}(t^ne^{at})=(-1)^nF^{(n)}(s)=n!/(s-a)^{n+1}$  
With Laplace, there is often more than one way to compute. We know $\mathcal{L}(t^n)=n!/s^{n+1}$.  Therefore the $s$-shift rule also gives the above formula for $\mathcal{L}(t^ne^{at})$.

### Repeated Quadratic Factors
Recall the table entries for repeated quadratic factors
$$\mathcal{L}(\frac{1}{2\omega^3}(\sin(\omega t)-\omega t\cos(\omega t)))=\frac{1}{(s^2+\omega^2)^2}\tag{7}$$
$$\mathcal{L}(\frac{t}{2\omega}\sin(\omega t))=\frac{s}{(s^2+\omega^2)^2}\tag{8}$$
$$\mathcal{L}(\frac{1}{2\omega}(\sin(\omega t)+\omega t\cos(\omega t)))=\frac{s^2}{(s^2+\omega^2)^2}\tag{9}$$
Previously we proved these formulas using partial fractions and factoring the denominators on the frequency side into complex linear factors. Let's prove them again using the $s$-derivative rule.  
**Proof of $(8)$ using the $s$-derivative rule.**  
Let $f(t)=\sin(\omega t)$. We know $F(s)=\frac{\omega}{s^2+\omega^2}$.  The $s$-derivative rule implies
$$\mathcal{L}(t\sin(\omega t))=-F'(s)=\frac{2\omega s}{(s^2+\omega^2)^2}$$
This formula is $(8)$ with the factor of $2\omega$ moved from one side to the other.
