### Definition
We start with the definition (see equation $(1)4. In subsequent sections of this note we will learn other ways of describing the transfer function. (See equations $(2)$ and $(3)$.)  
For any linear time invariant system the transfer function is
$$W(s)=\mathcal{L}(w(t)), \text{ where $w(t)$ is the unit impulse response.}\tag{1}$$
**Example 1.** Find the transfer function for the system $x'+3x=f(t)$.  
**Solution.** The unit impulse response is the solution to
$$w'+3w=\delta(t), \text{ with rest IC}$$
The Laplace transform method finds $W(s)$ on the way to finding $w(t)$. Since we only want $W(s)$ we can stop when we get there. Taking the Laplace transform of the DE we get
$$sW(s)-w(0^-)+3W(s)=1 \rArr W(s)=\frac{1}{s+3}$$
The 'annoying' term $w(0^-) = 0$ because we have rest initial conditions. (Subsequent to this we will not bother writing the annoying terms when we have rest IC.)

### Other Standard Terminology
The unit impulse response is also called the **weight function** and the transfer function is also called the **system function**. All of these terms are widely used and we will use them all to help you become familiar with them.

### Formula for $W(s)$
**Example 2.** Find the transfer function for $mx'' + bx' + kx = f(t)$.  
**Solution.** The unit impulse response is the solution to
$$mw''+bw'+kw=\delta(t), \text{ with rest IC}$$
By definition, the transfer function $W(s) = \mathcal{L}(w)$. So, we take the Laplace transform of the DE. There are no 'annoying terms' because with rest initial conditions $\mathcal{L}(w'')=s^2W(s)$ and $\mathcal{L}(w')=sW(s)$. We get
$$(ms^2+bs+k)W(s)=1 \rArr W(s)=\frac{1}{ms^2+bs+k}$$
In example 2, the differential operator is $p(D) = mD^2 + bD + kI$. That is, the characteristic polynomial is $p(s) = ms^2 + bs + k$ and the transfer function is $W(s) = 1/p(s)$.  
Exactly the same reasoning holds for operators of higher order.  
**Formula:** For any polynomial operator $p(D)$ the transfer function for the system
$$p(D)x=f(t)$$
is given by
$$W(s)=\frac{1}{p(s)}\tag{2}$$
**Example 3.** Suppose $W(s) = 1/(s^2 + 4)$ is the transfer function for a system $p(D)x = f(t)$. What is $p(D)$?  
**Solution.** Since $W(s) = 1/p(s)$ we have $p(s) = s^2 + 4$, which implies $p(D) = D^2 + 4I$.

### Another Characterization of the Transfer Function
The best way to think of the transfer function is as a ratio of output to input. By this we mean the following.  
Suppose we have an equation
$$p(D)x = f(t), \text{ with rest IC.}$$
Taking Laplace transform of both sides gives
$$p(s)X(s) = F(s) \rArr X(s)=\frac{1}{p(s)}F(s)=W(s)F(s)$$
Solving for $W(s)$ shows
$$W(s)=\frac{X(s)}{F(s)}=\frac{\text{output}}{\text{input}}\tag{3}$$

### Conclusion
We have characterized the transfer functions in three different ways. Equations $(1)$ and $(3)$ are perfectly general and apply to any LTI system. Equation $(2)$ is specific to constant coefficient linear differential equations.
