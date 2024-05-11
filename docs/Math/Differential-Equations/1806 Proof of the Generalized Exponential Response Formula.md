Using the exponential shift rule, we can now give a proof of the general case of the ERF which we stated without proof in the session on Exponential Response. This is a slightly complicated proof and you can safely skip it if you are not interested.  
**Generalized Exponential Response Formula.** Let $p(D)$ be a polynomial operator with constant coefficients and $p^{(s)}$ its $s$-th derivative. Then
$$p(D)x=e^{at} \text{ where $a$ is real or complex}\tag{1}$$
has the particular solution
$$
x_p=\begin{cases}
i) \frac{e^{at}}{p(a)}&p(a)\neq0\\
ii) \frac{te^{at}}{p'(a)}&p(a)=0&\text{and}&p'(a)\neq0\\
iii) \frac{t^2e^{at}}{p''(a)}&p(a)=p'(a)=0&\text{and}&p''(a)\neq0\\
...\\
iv) \frac{t^se^{at}}{p^{(s)}(a)}&f\text{ a is an s-fold zero}
\end{cases}
$$
**Proof.** That $(i)$ is a particular solution to $(1)$ follows immediately by using the linearity and substitution rules given earlier.
$$p(D)x_p=p(D)\frac{e^{at}}{p(a)}=\frac{1}{p(a)}p(D)e^{at}=\frac{p(a)e^{at}}{p(a)}=e^{at}$$
Since cases $(ii)$ and $(iii)$ are special cases of $(iv)$ we skip right to that. For case $(iv)$, we begin by noting that to say the polynomial $p(D)$ has the number $a$ as an $s$-fold zero is the same as saying $p(D)$ has a factorization
$$p(D)=q(D)(D-a)^s,q(a)\neq 0\tag{2}$$
We will first prove that $(2)$ implies
$$p^{(s)}(a)=q(a)s!\tag{3}$$
To prove this, let $k$ be the degree of $q(D)$ and write it in powers of $(D-a)$:
$$\begin{aligned}
q(D)&=q(a)+c_1(D-a)+\ldots+c_k(D-a)^k; \text{ then}\\
p(D)&=q(a)(D-a)^s+c_1(D-a)^{s+1}+\ldots+c_k(D-a)^{s+k};\\
p^{(s)}(D)&=q(a)s!+\text{ positive powers of } (D-a)
\end{aligned}$$
Substituting $a$ for $D$ on both sides proves $(3)$.
Using $(3)$, we can now prove $(iv)$ easily using the exponential-shift rule. We have
$$\begin{aligned}
p(D)\frac{e^{at}x^s}{p^{(s)}(a)}&=\frac{e^{at}}{p^{(s)}(a)}p(D+a)x^s&\text{by linearity and ERF case }(i)\\
&=\frac{e^{at}}{p^{(s)}(a)}q(D+a)D^sx^s&\text{by }(2)\\
&=\frac{e^{at}}{q(a)s!}q(D+a)s!&\text{by }(3)\\
&=\frac{e^{at}}{q(a)s!}q(a)s!=e^{at}
\end{aligned}$$
where the last line follows from $(4)$, since $s!$ is a constant:
$$q(D+a)s!=(q(a)+c_1D+\ldots+c_kD^k)s!=q(a)s!$$
**Note:** By linearity we could have stated the formula with a factor of $B$ in the input and a corresponding factor of $B$ to the output. That is, the DE
$$p(D)x=Be^{at}$$
has a particular solution
$$x_p=\frac{Be^{at}}{p(a)}, \text{ if } p(a)\neq 0 \text{ etc.}$$
