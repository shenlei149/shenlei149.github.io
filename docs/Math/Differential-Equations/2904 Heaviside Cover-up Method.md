### Introduction
The cover-up method was introduced by Oliver Heaviside as a fast way to do a decomposition into partial fractions. This is an essential step in using the Laplace transform to solve differential equations, and this was more or less Heaviside's original motivation.  
The cover-up method can be used to make a partial fractions decomposition of a proper rational function $\frac{p(s)}{q(s)}$ *whenever the denominator can be factored into distinct linear factors*.

### Linear Factors
We first show how the method works on a simple example, and then show why it works.  
**Example 1.** Decompose $\frac{s-7}{(s-1)(s+2)}$ into partial fractions.  
**Solution.** We know the answer will have the form
$$\frac{s-7}{(s-1)(s+2)}=\frac{A}{s-1}+\frac{B}{s+2}\tag{1}$$
To determine $A$ by the cover-up method, on the left-hand side we mentally remove (or cover up with a finger) the factor $s-1$ associated with $A$, and substitute $s = 1$ into what's left; this gives $A$:
$$\frac{s-7}{s+2}\bigg|_{s=1}=\frac{1-7}{1+2}=-2=A\tag{2}$$
Similarly, $B$ is found by covering up the factor $s + 2$ on the left, and substituting $s = -2$ into what's left. This gives
$$\frac{s-7}{s-1}\bigg|_{s=-2}=\frac{-2-7}{-2-1}=3=B$$
Thus, our answer is
$$\frac{s-7}{(s-1)(s+2)}=\frac{-2}{s-1}+\frac{3}{s+2}\tag{3}$$
Why does the method work? The reason is simple. The "right" way to determine $A$ from equation $(1)$ would be to multiply both sides by $(s - 1)$; this would give
$$\frac{s-7}{(s+2)}=A+\frac{B}{s+2}(s-1)\tag{4}$$
Now if we substitute $s = 1$, what we get is exactly equation $(2)$, since the term on the right disappears. The cover-up method therefore is just an easy and efficient way of doing the calculations.

In general, if the denominator of the proper rational function factors into the product of distinct linear factors:
$$\frac{p(s)}{(s-a_1)(s-a_2)\cdots(s-a_r)}=\frac{A_1}{s-a_1}+\cdots+\frac{A_r}{s-a_r}, a_i\neq a_j$$
then $A_i$ is found by covering up the factor $s - a_i$ on the left, and setting $s = a_i$ in the rest of the expression.  
**Example 2.** Decompose $\frac{1}{s^3-s}$ into partial fractions.  
**Solution.** Factoring, $s^3 - s = s(s^2 - 1) = s(s - 1)(s + 1)$. By the cover-up method,
$$\frac{1}{s(s - 1)(s + 1)}=\frac{-1}{s}+\frac{1/2}{s-1}+\frac{1/2}{s+1}$$
To be honest, the real difficulty in all of the partial fractions methods (the cover-up method being no exception) is in factoring the denominator. Even the programs which do symbolic integration, like Macsyma, or
Maple, can only factor polynomials whose factors have integer coefficients, or "easy coefficients" like $\sqrt{2}$. and therefore they can only integrate rational functions with "easily-factored" denominators.

### Quadratic Factors
Heaviside's cover-up method can be used even when the denominator doesn't factor into distinct linear factors. This only gives partial results, but these can often be a big help, as the following example illustrates.  
**Example 3.** Decompose $\frac{5s+6}{(s^2+4)(s-2)}$.  
**Solution.** We write
$$\frac{5s+6}{(s^2+4)(s-2)}=\frac{As+B}{s^2+4}+\frac{C}{s-2}\tag{5}$$
We first determine $C$ by the cover-up method, getting $C = 2$. Then $A$ and $B$ can be found by the method of undetermined coefficients; the work is greatly reduced since we need to solve only two simultaneous equations to find $A$ and $B$, not three.  
Following this plan, using $C = 2$, we combine terms on the right of $(5)$ so that both sides have the same denominator. The numerators must then also be equal, which gives us
$$5s + 6 = (As + B)(s - 2) + 2(s^2 + 4)\tag{6}$$
Comparing the coefficients of $s^2$ and of the constant terms on both sides of $(6)$ gives the two equations
$$0 = A + 2 \text{ and } 6 = -2B + 8$$
from which $A = -2$ and $B = 1$.  
In using $(6)$, one could have instead compared the coefficients of $s$, getting $5 = -2A + B$, leading to the same result, but providing a valuable check on the correctness of the computed values for $A$ and $B$.  
In Example 3, an alternative to undetermined coefficients would be to substitute two numerical values for $s$ into the original equation $(5)$, say $s = 0$ and $s = 1$ (any values other than $s = 2$ are usable). Again one gets two simultaneous equations for $A$ and $B$. This method requires addition of fractions, and is usually better when only one coefficient remains to be determined (as in Example 4 below).  
Still another method would be to factor the denominator completely into linear factors, using complex coefficients, and then use the cover-up method, but with complex numbers. At the end, conjugate complex terms have to be combined in pairs to produce real summands, and the calculations can sometimes be longer.

### Repeated Linear Factors
The cover-up method can also be used if a linear factor is repeated, but there too it gives just partial results. It applies only to *the highest power of the linear factor*.  
**Example 4.** Decompose $\frac{1}{(s-2)^2(s+2)}$.  
**Solution.** We write
$$\frac{1}{(s-2)^2(s+2)}=\frac{A}{(s-1)^2}+\frac{B}{s-1}+\frac{C}{s+2}\tag{7}$$
To find $A$ cover up $(s - 1)^2$ and set $s = 1$; you get $A = 1/3$. To find $C$, cover up $s + 2$, and set $s = -2$; you get $C = 1/9$.  
This leaves $B$ which cannot be found by the cover-up method. But since $A$ and $C$ are already known in $(7)$, $B$ can be found by substituting any numerical value (other than 1 or -2) for $s$ in $(7)$. For instance, if we put $s = 0$ and remember that $A = 1/3$ and $C = 1/9$, we get
$$\frac{1}{2}+\frac{1/3}{1}+\frac{B}{-1}+\frac{1/9}{2}$$
giving $B = -1/9$.  
$B$ could also be found by applying the method of undetermined coefficients to the $(7)$; note that since $A$ and $C$ are known, it is enough to get a single linear equation in order to determine $B$ - simultaneous equations are no longer needed.  
The fact that the cover-up method works for just the *highest power* of the repeated linear factor can be seen just as before. In the above example for instance, the cover-up method for finding $A$ is just a short way of multiplying $(5)$ through by $(s - 1)^2$ and then substituting $s = 1$ into the resulting equation.
