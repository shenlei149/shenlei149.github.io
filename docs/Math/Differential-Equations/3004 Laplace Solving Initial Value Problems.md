### Introduction
We now have everything we need to solve IVP's using Laplace transform. We will show how to do this through a series of examples.  
To be honest we should admit that some IVP's are more easily solved by other techniques.  However, we will also see some examples where the Laplace machinery we've developed is a big help.  

### Examples of Solving IVP's
**Example 1.** Solve $x'+3x=e^{-t}$ with rest initial conditions (rest IC).  
**Solution.** Rest IC mean that $x(t) = 0$ for $t < 0$, so $x(0^-), x'(0^-), \ldots$ are all 0. As usual, we let $X = \mathcal{L}(x)$.  
Using the $t$-derivative rule we can take the Laplace transform of (both sides) of the DE.
$$(sX(s)-X(0^-))+3X(s)=1/(s+1)$$
Next we substitute the known value $x(0^-) = 0$ and solve for $X(s)$
$$(s+3)X(s)=\frac{1}{s+1} \rArr X(s)=\frac{1}{(s+1)(s+3)}\tag{1}$$
Finally, we find $x(t) = \mathcal{L}^{-1}(X)$ by using cover-up to do the partial fractions decomposition.
$$\frac{1}{(s+1)(s+3)}=\frac{1/2}{s+1}-\frac{1/2}{s+3} \rArr x(t)=\frac{1}{2}e^{-t}-\frac{1}{2}e^{-3t} \text{ for } t>0$$
**Notes:**
1. The term $e^{-t}/2$ is what the exponential response formula would give us. The term $e^{-3t}/2$ is the homogenous part of the solution, needed to match the IC.
2. This technique found $x(t)$ for $t > 0$. The rest IC tell us $x(t) = 0$ for $t < 0$.
3. $x(0^+) = 0$: Since the input does not contain $\delta(t)$. There is no jump in $x(t)$ at 0.
4. The factor of $(s + 3)$ in front of $X(s)$ in $(1)$ is none other than the characteristic polynomial of this system.

**Example 2.** Solve $x'+3x=e^{-t}, x(0^-)=4$.  
**Solution.** Laplace:
$$sX(s)-X(0^-)+3X(s)=1/(s+1) \rArr (s+3)X(s)=4+1/(s+1)$$
Solve for $X(s)$:
$$X(s)=\frac{4}{s+3}+\frac{1}{(s+1)(s+3)}\tag{2}$$
We can use the partial fractions work from example $(1)$.
$$\begin{aligned}
x(t)&=4e^{-3t}+\frac{1}{2}e^{-t}-\frac{1}{2}e^{-3t} \text{ for } t>0\\
&=\frac{1}{2}e^{-t}+\frac{7}{2}e^{-3t} \text{ for } t>0
\end{aligned}$$
**Notes:** (Same remarks as in example 1.)

**Example 3.** Find the unit impulse response for the operator $D + 3I$. Give your answer in both $u$ and cases format.  
**Solution.** The unit impulse response is the solution to
$$(D+3I)w=w'+3w=\delta(t), \text{  with rest IC.}$$
Taking the Laplace transform we get
$$sW(s)-W(0^-)+3W(s)=1 \rArr (s+3)W=1 \rArr \frac{1}{s+3}$$
Laplace inverse now implies $w(t)=e^{-3t}$ for $t>0$. Thus,
$$w(t)=\begin{cases}
0&\text{ for } t<0\\
e^{-3t}&\text{ for } t>0
\end{cases}=u(t)e^{-3t}$$
**Notes:**
1. The post-initial condition is $w(0^+) = 1$. This came out of the calculation, we didn't have to think about the effect of the input $\delta(t)$ at $t = 0$.
2. The Laplace transform method did not help us find $w(t)$ for $t < 0$. For this we used the rest IC that are part of the definition of the unit impulse function.
3. Since $w(0^-) = 0$ the output jumps by 1 unit at $t = 0$.
4. Once again you saw the characteristic polynomial appearing.

**Example 4.** Find the unit impulse response for the system $p(D)x = f$, where $p(D) = D^2 + 2D + 2I$ and we consider $f$ to be the input. Give your answer in both $u$ and cases format.  
**Solution.** IVP: $w'' + 2w' + 2w = \delta(t)$, with rest IC.  
Laplace: $s^2W + 2sW + 2W = 1 \rArr W = 1/(s^2 + 2s + 2)$.  
(Here we left out all the 'annoying terms' because they are all 0 due to the rest IC.)  
Complete the square: $s^2 + 2s + 2 = (s + 1)^2 + 1$.  
Inverse Laplace: (using the $s$-shift rule)
$$W = 1/((s + 1)^2 + 1) \rArr w(t)=e^{-t}\mathcal{L}^{-1}(1/(s^2+1))=e^{-t}\sin(t) \text{ for } t>0$$
Thus
$$w(t)=\begin{cases}
0&\text{ for } t<0\\
e^{-t}\sin(t)&\text{ for } t>0
\end{cases}=u(t)e^{-t}\sin(t)$$
**Notes:**
1. The post-initial conditions emerge naturally from the solution and are $w(0^+) = 0, w'(0^+)=1$
2. Since $w'(0^-) = 0$ the first derivative jumps by 1 unit at $t = 0$.
3. Once again you saw the characteristic polynomial appearing.

**Example 5.** Solve $x' + 2x = 4t$, with initial condition $x(0) = 1$.  
**Remark.** Because the input contains no delta functions it is okay to specify the initial condition at $t = 0$ instead of $t = 0^-$. There will be no jump in the output, i.e., $x(0) = x(0^-) = x(0^+)$.  
**Solution.** Laplace: $sX-x(0^-)+2X=4/s^2$.  
Algebra and partial fractions:
$$X(s)=\frac{4}{s^2(s+2)}+\frac{1}{s+2}=\frac{A}{s}+\frac{B}{s^2}+\frac{C}{s+2}+\frac{1}{s+2}$$
Cover-up gives $B = 2, C = 1$. Undetermined coefficients gives $A = -1$.  
Inverse Laplace: $x(t)=-1+2t+2e^{-2t} \text{ for } t>0$.

**Example 6.** Solve $x'' + 4x = \cos(2t)$, with rest IC.  
**Solution.** Laplace: $(s^2+4)X(s)=s/(s^2+4) \rArr X(s)=s/(s^2+4)^2$. This is a repeated quadratic factor and it is in our table: $x(t)=t\sin(2t)/4$.  
**Notes:**
1. This is a response of pure resonance.
2. We could have turned the logic around and used our previous knowledge of the solution to this equation to give yet another proof for the table entry $\mathcal{L}(t\sin(\omega t)/2\omega)=s/(s^2+\omega^2)^2$.
