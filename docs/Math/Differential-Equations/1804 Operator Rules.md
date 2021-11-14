In stating these rules, we will always assume that the functions involved are sufficiently differentiable, so that the operators can be applied to them.

**Sum Rule.** If $p(D)$ and $q(D)$ are polynomial operations, the any (sufficiently differentiable) function u, 
$$[P(D)+q(D)]u=p(D)u+q(D)u\tag{1}$$

**Lineariry Rule.** If $f$ and $g$ are functions and $c_1$ and $c_2$ are constants,
$$p(D)(c_1f+c_2g)=c_1p(D)f+c_2P(D)g\tag{2}$$

**Proof of the linearity rule:** This rule follows from the linearity of differen­tiation. That is, 
$$D(c_1f+c_2g)=(c_1f+c_2g)'=c_1f'+c_2g'=c_1Du_1+c_2Du_2$$

Similarly taking the second or higher derivative also follows the linearity rule . That is,
$$D^{(n)}(c_1f+c_2g)=\frac{d^n}{dt^n}(c_1f+c_2g)=c_1f^{(n)}+c_2g^{(n)}=c_1D^{(n)}f+c_2D^{(n)}g$$

Next, we can scale the linear operator $D^n$ by $a$ and it stays linear. That is,
$$aD^{(n)}(c_1f+c_2g)=a\frac{d^n}{dt^n}(c_1f+c_2g)=c_1af^{(n)}+c_2ag^{(n)}=c_1aD^{(n)}f+c_2aD^{(n)}g$$

(Notice that `a` does not actually have to be a constant, it can be a function of t (or of whatever independent variable we're using). )  
Finally we can combine these operators into a polynomial operator
$$D^n+a_1D^{n-1}+...+a_{n-1}D^{n-1}+a_n$$
which clearly still obeys the linearity rule. □

**Multiplication rule.** If $p(D)=g(D)h(D)$ as polynomials in $D$, then
$$p(D)u=g(D)(h(D)u)\tag{3}$$
The property is true when $h(D)$ is the simple operator $aD^k$, essentially because
$$D^m(aD^ku)=aD^{m+k}u$$
It extends to general polynomial operators $h(D)$ by linearity. Note that here `a` must be a constant; it's `false` otherwise.

An important corollary of the multiplication property is that `polynomial operators with constant coefﬁcients commute`; i.e., for every function $u(t)$,
$$g(D)(h(D)u)=h(D)(g(D)u)\tag{4}$$
As polynomials, $g(D)h(D) = h(D)g(D) = p(D)$.

The remaining two rules are of a different type and are more concrete: they tell us how polynomial operators behave when applied to exponential functions and products involving exponential functions.

**Substitution rule.**
$$p(D)e^{at}=p(a)e^{at}\tag{5}$$
**Proof** We have, by repeated differentiation,
$$De^{at}=ae^{at},D^2e^{at}=a^2e^{at},...,D^ke^{at}=a^ke^{at}$$
therefore,
$$(D^n+c_1D^{n-1}+...+c_n)e^{at}=(a^n+c_1a^{n-1}+...+c_n)e^{at}$$
which is the substitution rule. □

**The exponential-shift rule** This handles expressions such as $t^ke^{at}$ and $t^k\sin{at}$. Let $u=u(t)$. Then
$$p(D)e^{at}u=e^{at}p(D+a)u\tag{6}$$
**Proff.** We prove it in successive stages. First, it is true when $P(D)=D$, since by the product rule for differentiation,
$$De^{at}u(t)=e^{at}Du(t)+ae^{at}u(t)=e^{at}(D+a)u(t)\tag{7}$$
To show the rule is true for $D^k$,we apply $(7)$ to $D$ repeatedly:
$$
\begin{aligned}
D^2e^{at}u=D(De^{at}u)&=D(e^{at}(D+a)u)\\
&=e^{at}(D+a)((D+a)u)\\
&=e^{at}(D+a)^2u
\end{aligned}
$$
In the same way,
$$
\begin{aligned}
D^3e^{at}u=D(D^2e^{at}u)&=D(e^{at}(D+a)^2u)\\
&=e^{at}(D+a)((D+a)^2u)\\
&=e^{at}(D+a)^3u
\end{aligned}
$$
and so on. This shows that $(6)$ is true for an operator of the form $D^k$. To show it is true for a general operator
$$P(D)=D^n+c_1D^{n-1}+...+c_n$$
we write $(6)$ for each $D^k(e^{at}u)$, multiply both sides by the coefficient $c_k$, and add up the resulting equations for the different values of $k$.

**Example.** Find $D^3e^{-t}\sin{t}$.
**Solution using the exponential-shift rule.**
$$
\begin{aligned}
D^3e^{-t}\sin{t}&=e^{-t}(D-1)^3\sin{t}\\
&=e^{-t}(D^3-3D^2+3D-1)\sin{t}\\
&=e^{-t}(-\cos{t}+3\sin{t}+3\cos{t}-\sin{t})\\
&=e^{-t}(2\cos{t}+2\sin{t})
\end{aligned}
$$
**Solution using the substitution rule.** Write $e^{-t}\sin{t}=Img(e^{(-1+i)t}$. We have
$$
\begin{aligned}
D^3e^{(-1+i)t}&=(-1+i)^3e^{(-1+i)t}\\
&=(2+2i)e^{-t}(\cos t+i\sin t)
\end{aligned}
$$
Take the imaginary part: $e^{-t}(2\cos{t}+2\sin{t})$
