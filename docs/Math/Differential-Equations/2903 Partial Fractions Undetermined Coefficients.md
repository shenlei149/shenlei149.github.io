### Introduction
Not every $F(s)$ we encounter is in the Laplace table. Partial fractions is a method for re-writing $F(s)$ in a form suitable for the use of the table.  
In this note we will run through the various cases encountered when we apply the method of partial fractions decomposition to a rational function. In the next note we will learn the Heaviside cover-up method, which simplifies some of the algebra.  
**Note:** We use the term undetermined coefficients in the same way it was use when solving an ODE with polynomial input. It involves setting a polynomial with unknown coefficients equal to a known polynomial and solving for the unknown coefficients by equating them with the known ones.

### Rational Functions
A **rational function** is one that is the ratio of two polynomials. For example
$$\frac{s+1}{s^2+7s+9} \text{ and } \frac{s^2+7s+9}{s+1}$$
are both rational functions.  
A rational function is called **proper** if the degree of the numerator is strictly smaller than the degree of the denominator; in the examples above, the first is proper while the second is not.  
**Long-division:** Using long-division we can always write an improper rational function as a polynomial plus a proper rational function. The partial fraction decomposition only applies to proper functions.  
**Example 1.** Use long-division to write $\frac{s^3+2s+1}{s^2+s-2}$ as a the sum of a polynomial and a proper rational function.  
**Solution.**
$$\begin{aligned}
&&&+s&-1\\
s^2+s-2|&s^3&&+2s&+1\\
&s^3&+s^2&-2s\\
&&-s^2&+4s&+1\\
&&-s^2&-s&+2\\
&&&+5s&-1
\end{aligned}$$
Therefore,
$$\frac{s^3+2s+1}{s^2+s-2}=s-1+\frac{5s-1}{s^2+s-2}$$

### Linear Factors
Here we assume the denominator factors in distinct linear factors. We start with a simple example. We will explain the general principle immediately afterwords.  
**Example 2.** Decompose $R(s)=\frac{s-3}{(s-2)(s-1)}$ using partial fractions. Use this to find $\mathcal{L}^{-1}(R(s))$.  
**Solution.**
$$\frac{s-3}{(s-2)(s-1)}=\frac{A}{s-2}+\frac{B}{s-1}$$
Multiplying both sides by the denominator on the left gives
$$s-3 = A(s-1) + B(s-2)\tag{1}$$
The sure algebraic way is to expand out the right hand side and equate the coefficients with those of the polynomial on the left.
$$s-3 = (A+B)s + (-A-2B) \rArr \begin{cases}
1&=&A+B\\
-3&=&-A-2B
\end{cases}$$
We solve this system of equations to find the undetermined coefficients $A$ and $B$: $A = -1, B = 2$.  
Answer: $R(s)=-1/(s-2)+2/(s-1)$  
Table lookup then gives $\mathcal{L}^{-1}(R(s))=-e^{2t}+2e^t$.  
**Note:** In this example it would have been easier to plug the roots of each factor into equation $(1)$. When you do this every term except one becomes 0.  
Plug in $s=1 \rArr -2=B(1-2) \rArr B=2$.  
Plug in $s=2 \rArr -1=A(2-1) \rArr A=-1$.  
In general, if $P(s)/Q(s)$ is a proper rational function and $Q(s)$ factors into distinct linear factors $Q(s) = (s-a_1)(s-a_2)\cdots(s-a_n)$ then
$$\frac{P(s)}{Q(s)}=\frac{A_1}{s-a_1}+\frac{A_2}{s-a_2}+\cdots+\frac{A_n}{s-a_n}$$
The proof of this is not hard, but we will not give it. Remember you must have a *proper* rational function and each of the factors must be distinct. Repeated factors are discussed below.  
**Example 3.** Use partial fractions to find $\mathcal{L}^{-1}(\frac{3}{s^3-3s^2-s+3})$  
**Solution.** The hardest part of this problem is to factor the denominator. For higher order polynomials this might be impossible. In this case you can check
$$s^3-3s^2-s+3=(s-1)(s+1)(s-3)$$
The partial fractions decomposition is
$$\frac{3}{(s-1)(s+1)(s-3)}=\frac{A}{s-1}+\frac{B}{s+1}+\frac{C}{s-3}$$
Multiplying through by the denominator gives
$$3=A(s+2)(s-3)+B(s-1)(s-3)+C(s-1)(s+2)$$
Plugging in $s = 1$ gives $A = -3/4$, likewise $s = -1$ gives $B = 3/8$ and $s = 3$ gives $C = 3/8$.  
Our answer is
$$\mathcal{L}^{-1}(\frac{3}{s^3-3s^2-s+3})=Ae^t+Be^{-t}+Ce^{3t}=-\frac{3}{4}e^t+\frac{3}{8}e^{-t}+\frac{3}{8}e^{3t}$$

### Quadratic Factors
If the denominator has quadratic factors then, the numerator in the partial fraction decomposition will be a linear term instead of a constant.  
**Example 4.** Find $\mathcal{L}^{-1}(\frac{s-1}{(s+1)(s^2+4)})$.  
**Solution.** This is a proper rational function so
$$\frac{s-1}{(s+1)(s^2+4)}=\frac{A}{s+1}+\frac{Bs+C}{s^2+4}\tag{2}$$
Notice the quadratic factor gets a linear term in the numerator. Notice also that the number of unknown coefficients is the same as the degree of the denominator in the original fraction.  
From $(2)$ we can write
$$\mathcal{L}^{-1}(\frac{s-1}{(s+1)(s^2+4)}=Ae^{-t}+B\cos(2t)+\frac{C}{2}\sin(2t)$$
All that's left is to do some algebra to find the coefficients Muliplying $(2)$ through by the denominator gives
$$s - 1 = A(s^2 + 4) + (Bs + C)(s + 1) = (A + B)s
2 + (B + C)s + (4A + C)$$
Equate the coefficients on both sides:
$$\begin{aligned}
s^2:&&0&=A+B\\
s:&&1&=B+C\\
s^0:&&-1&=4A+C
\end{aligned}$$
Solving, we get $A = -2/5, B = 2/5, C = 3/5$.  
**Example 5.** Don't be fooled by quadratic terms that factor into linear ones.
$$\frac{1}{(s+1)(s^2-4)}=\frac{1}{(s+1)(s+2)(s-2)}=\frac{A}{s+1}+\frac{B}{s+2}+\frac{C}{s-2}$$
**Example 6.** Don't forget that the rational function must be proper. For example, decompose $\frac{s^3+2s+1}{s^2+s-2}$ using partial fractions.  
**Solution.** First, we must use long-division to make this proper. From example $(1)$ we have
$$\begin{aligned}
\frac{s^3+2s+1}{s^2+s-2}&=s-1+\frac{5s-1}{s^2+s-2}\\
&=s-1+\frac{5s-1}{(s+2)(s-1)}\\
&=s-1+\frac{A}{s+2}+\frac{B}{s-1}
\end{aligned}$$
Solving for the undetermined coefficients gives $A = 11/3, B = 4/3$.

### Repeated Linear Factors
For repeated linear factors we need one partial fraction term for each power of the factor as illustrated by the following example.
**Example 7.** Find $\mathcal{L}^{-1}(\frac{2s}{s^3(s+1)^2(s+2)})$ using partial fractions.  
**Solution.**
$$\frac{2s}{s^3(s+1)^2(s+2)}=\frac{A}{s}+\frac{B}{s^s}+\frac{C}{s^3}+\frac{D}{s+1}+\frac{E}{(s+1)^2}+\frac{F}{s+2}$$
Here the denominator has a linear factor $s$ repeated three times (term $s^3$), and a linear factor $(s + 1)$ repeated twice (term $(s + 1)^2$); hence three partial fractions are associated with the first, while two are associated with the latter. The term $(s+2)$ which is not repeated leads to one partial fraction as previously seen. You can check that the coefficients are
$$A = -5/2, B = 1, C = 0, D = 2, E = 2, F = 1/2$$
Using the $s$-shift rule we have $\mathcal{L}^{-1}(1/(s+1)^2)=te^{-t}$. Thus,
$$\begin{aligned}
\mathcal{L}^{-1}(\frac{2s}{s^3(s+1)^2(s+2)})&=A+Bt+\frac{C}{2}t^2+De^{-t}+Ete^{-t}+Fe^{-2t}\\
&=-\frac{5}{2}+t+2e^{-t}+2te^{-t}+\frac{1}{2}e^{-2t}
\end{aligned}$$

### Repeated Quadratic Factors
Just like repeated linear factors, quadratic factors have one term for each power of the factor as illustrated in the following example.  
**Example 8.** Find $\mathcal{L}^{-1}(\frac{2s}{s(s^2+1)^2(s^2+4s+6)})$ using partial fractions.
**Solution.** The partial fractions decomposition is
$$\frac{2s}{s(s^2+1)^2(s^2+4s+6)}=\frac{A}{s}+\frac{Bs+C}{s^2+1}+\frac{Ds+E}{(s^2+1)^2}+\frac{Fs+G}{s^2+4s+6}$$
Note the repeated factor $(s^2 + 1)^2$ lead to two partial fraction terms.  
We won't compute the coefficients - you can do this by going through the algebra. Instead, we'll focus on finding the Laplace inverse. Our table contains the terms with repeated quadratic factors.
$$\begin{aligned}
&\mathcal{L}^{-1}(\frac{Ds}{(s^2+1)^2})&=&D\frac{t}{2}\sin(t)\\
&\mathcal{L}^{-1}(\frac{E}{(s^2+1)^2})&=&E\frac{1}{2}(\sin(t)-t\cos(t))
\end{aligned}$$
For the terms with denominator $s^2 + s + 2$ we need to complete the square. Notice that we make sure to also shift the $s$-term in the numerator.
$$\begin{aligned}
\mathcal{L}^{-1}(\frac{Fs+G}{s^2+4s+6})&=\mathcal{L}^{-1}(\frac{F(s+2)+G-2F}{(s+2)^2+2})\\
&=Fe^{-2t}\cos(\sqrt{2}t)+\frac{1}{\sqrt{2}}(G-2F)e^{-2t}\sin(\sqrt{2}t)
\end{aligned}$$
Putting this together, the answer is
$$\begin{aligned}
\mathcal{L}^{-1}(\frac{2s}{s(s^2+1)^2(s^2+4s+6)})=&A+B\cos(t)+C\sin(t)\\
&+D\frac{t}{2}\sin(t)+E\frac{1}{2}(\sin(t)-t\cos(t))\\
&+Fe^{-2t}\cos(\sqrt{2}t)+\frac{1}{\sqrt{2}}(G-2F)e^{-2t}\sin(\sqrt{2}t)
\end{aligned}$$

### Complex Factors
We can allow complex roots. In this case all quadratic terms factor into linear terms.
**Example 9.** Decompose $s/(s^2 + \omega^2)$ using complex partial fractions and use it to show $\mathcal{L}^{-1}(s/(s^2 + \omega^2))=\cos(\omega t)$.  
**Solution.**
$$\frac{s}{s^2 + \omega^2}=\frac{s}{(s+i\omega)(s-i\omega)}=\frac{A}{s+i\omega}+\frac{B}{s-i\omega}$$
Multiplying through by the denominator gives $s=A(s-i\omega)+B(s+i\omega)$  
Plug in $s=i\omega \rArr B=1/2$  
Plug in $s=-i\omega \rArr A=1/2$  
From the table:
$$\begin{aligned}
\mathcal{L}^{-1}(\frac{s}{s^2 + \omega^2})&=\mathcal{L}^{-1}(\frac{A}{s+i\omega}+\frac{B}{s-i\omega})\\
&=Ae^{-i\omega}+Be^{i\omega}\\
&=\frac{1}{2}(e^{i\omega}+e^{-i\omega})\\
&=\cos(\omega t)
\end{aligned}$$
