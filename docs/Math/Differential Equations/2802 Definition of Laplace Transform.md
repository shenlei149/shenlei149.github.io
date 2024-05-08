### Definition of Laplace Transform
The Laplace transform of a function $f(t)$ of a real variable $t$ is another function depending on a new variable $s$, which is in general complex. We will denote the Laplace transform of $f$ by $\mathcal{L}f$. It is defined by the integral
$$(\mathcal{L}f)(s)=\int_{0^-}^\infty f(t)e^{-st}dt\tag{1}$$
for all values of $s$ for which the integral converges.

There are a few things to note.
* $\mathcal{L}f$ is only defined for those values of $s$ for which the improper integral on the right-hand side of $(1)$ converges.
* We will allow $s$ to be complex.
* As with convolution the use of $0^-$, in the definition $(1)$ is necessary to accomodate generalized functions containing $\delta(t)$. Many textbooks do not do this carefully, and hence their definition of the Laplace transform is not consistent with the properties they assert. In those cases where $0^-$ isn't needed we will use the less precise form
$$(\mathcal{L}f)(s)=\int_{0}^\infty f(t)e^{-st}dt\tag{1'}$$
* Also, as with convolution, the limits of integration mean that the Laplace transform is only concerned with functions on $(0^-, \infty)$.

### Notation, $F(s)$
We will adopt the following conventions:
* Writing $(\mathcal{L}f)(s)$ can be cumbersome so we will often use an uppercase letter to indicate the Laplace transform of the corresponding lowercase function:
$$(\mathcal{L}f)(s)=F(s),(\mathcal{L}g)(s)=G(s),\text{ etc.}$$
For example, in the formula
$$\mathcal{L}(f')=sF(s)-f(0^-)$$
it is understood that we mean $F(s) = \mathcal{L}(f)$.
* If our function doesn't have a name we will use the formula instead. For example, the Laplace transform of the function $t^2$ is written $\mathcal{L}(t^2)(s)$ or more simply $\mathcal{L}(t^2)$.
* If in some context we need to modify $f(t)$, e.g. by applying a translation by a number $a$, we can write $\mathcal{L}(f(t - a))$ for the Laplace transform of this translation of $f$.
* You've already seen several different ways to use parentheses. Sometimes we will even drop them altogether. So, if $f(t) = t^2$ then the following all mean the same thing
$$(\mathcal{L}f)(s) = F(s) = \mathcal{L}f(s) = \mathcal{L}(f(t))(s) = \mathcal{L}(t^2)(s); \mathcal{L}f = F = \mathcal{L}(t^2)$$

### Examples
For the first few examples we will explicitly use a limit for the improper integral. Soon we will do this implicitly without comment.

**Example 1.** Let $f(t) = 1$, find $F(s) = \mathcal{L}f(s)$.  
**Solution.** Using the definition $(1')$ we have
$$\mathcal{L}(1)=F(s)=\int_0^\infty e^{-st}dt=\lim_{T \to \infty} \frac{e^{-st}}{-s}\bigg|_0^T=\lim_{T \to \infty} \frac{e^{-sT}-1}{-s}$$
The limit depends on whether s is positive or negative.
$$\lim_{T \to \infty} e^{-sT}=\begin{cases}
0&\text{if }s>0\\
\infty&\text{if }s<0
\end{cases}$$
Therefore,
$$\mathcal{L}(1)=F(s)=\begin{cases}
\frac{1}{s}&\text{if }s>0\\
\text{diverges}&\text{if }s\leq 0
\end{cases}$$
(We didn't actually compute the case $s = 0$, but it is easy to see it diverges.)

**Example 2.** Compute $\mathcal{L}e^{at}$  
**Solution.** Using the definition $(1')$ we have
$$\mathcal{L}(e^{at})=F(s)=\int_0^\infty e^{at}e^{-st}dt=\lim_{T \to \infty} \frac{e^{(a-s)t}}{a-s}\bigg|_0^T=\lim_{T \to \infty} \frac{e^{(a-s)T}-1}{a-s}$$
The limit depends on whether $s > a$ or $s < a$.
$$\lim_{T \to \infty} e^{(a-s)T}=\begin{cases}
0&\text{if }s>a\\
\infty&\text{if }s<a
\end{cases}$$
Therefore,
$$\mathcal{L}(e^{at})=\begin{cases}
\frac{1}{s-a}&\text{if }s>a\\
\text{diverges}&\text{if }s\leq a
\end{cases}$$
(We didn't actually compute the case $s = a$, but it is easy to see it diverges.)

We have the first two entries in our table of Laplace transforms:
$$\begin{aligned}
&f(t)=1 &\rArr &F(s)=1/s, &s>0\\
&f(t)=e^{at} &\rArr &F(s)=1/(s-a), &s>a
\end{aligned}$$

### Linearity
You will not be surprised to learn that the Laplace transform is linear. For functions $f,g$ and constants $c_1, c_2$
$$\mathcal{L}(c_1f+c_2g)=c_1\mathcal{L}(f)+c_2\mathcal{L}(g)$$
This is clear from the definition $(1)$ of $\mathcal{L}$ and the linearity of integration.
