### Repeated Quadratic Factors
We will add three entries to our Laplace Table.
$$\mathcal{L}(\frac{1}{2\omega^3}(\sin(\omega t)-\omega t\cos(\omega t)))=\frac{1}{(s^2+\omega^2)^2}\tag{1}$$
$$\mathcal{L}(\frac{t}{2\omega}\sin(\omega t))=\frac{s}{(s^2+\omega^2)^2}\tag{2}$$
$$\mathcal{L}(\frac{1}{2\omega}(\sin(\omega t)+\omega t\cos(\omega t)))=\frac{s^2}{(s^2+\omega^2)^2}\tag{3}$$
There are several ways to prove these formulas. We will give one using partial fractions by factoring the denominators on the frequency side into complex linear factors.  
**Proof of 1.** First some algebra:
$$\frac{1}{(s-a)^2(s+a)^2}=\frac{A}{(s-a)^2}+\frac{B}{s-a}+\frac{C}{(s+a)^2}+\frac{D}{s+a}$$
Cover-up gives us $A$ and $C$.
$$A=C=\frac{1}{4a^2}$$
Undetermined coefficients:
$$s=0 \rArr -a^3B+a^3D=\frac{1}{2}\\
s=2a \rArr 3a^3B+a^3D=-\frac{1}{2}$$
then gives $B$ and $D$
$$D=\frac{1}{4a^3}=-B$$
This gives the inverse Laplace transform
$$\mathcal{L}^{-1}(\frac{1}{(s-a)^2(s+a)^2})=\frac{1}{4a^2}(te^{at}+te^{-at})-\frac{1}{4a^3}(e^{at}-e^{-at})\tag{4}$$
We will use this on the right hand side of $(1)$, but first recall
$$e^{i\omega t}+e^{-i\omega t}=2\cos(\omega t) \text{ and } e^{i\omega t}-e^{-i\omega t}=2i\sin(\omega t)\tag{5}$$
Let $a = i\omega$, then $(4)$ and $(5)$ combine to prove formula $(1)$.
$$\begin{aligned}
\mathcal{L}^{-1}(\frac{1}{(s^2+\omega^2)^2})&=\mathcal{L}^{-1}(\frac{1}{(s-i\omega)^2(s+i\omega)^2})\\
&=-\frac{1}{4\omega^2}(te^{i\omega t}+te^{-i\omega t})+\frac{1}{4\omega^3i}(e^{i\omega t}-e^{-i\omega t})\\
&=-\frac{1}{2\omega^2}t\cos(\omega t)+\frac{1}{2\omega^3}\sin(\omega t)
\end{aligned}$$
The proofs of (2) and (3) are similar, and we will omit them.

### Note on the Relation to Resonance:
Each of the formulas $(1)$, $(2)$, and $(3)$ has a term with a factor of $t$. This is exactly what we saw with the response $x$ in the resonance equation
$$x''+\omega^2x=\cos(\omega t)$$
which has solution $x(t) = t\sin(\omega t)/(2\omega)$.  
Notice that $\mathcal{L}^{-1}(1/s^2) = t$ and the $s$-shift rule shows $\mathcal{L}^{-1}(1/(s - a)^2) =te^{at}$. So repeated factors on the frequency side always lead to multiplication by $t$ on the time side. If the repeated factor has a higher power then we get multiplication by a higher power of $t$.
