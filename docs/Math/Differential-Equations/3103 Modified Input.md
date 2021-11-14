Way back when we introduced the language of system, input and response we decided that the right hand side of our equations wasn't always the input. Sometimes it was a modified version of the input.

**Example 1.** Recall the heat diffusion equation
$$x'+kx=kT_e(t)$$
where $T_e(t)$ is the temperature of the environment. Consider $T_e(t)$ to be the input and find the system function.  
**Solution.** Look at the equation for the unit impulse response
$$w'+kw=k\delta(t), \text{ rest IC}$$
Notice that since $T_e(t)$ is the input, the unit impulse response comes by letting $T_e(t) = \delta(t)$. The Laplace transform now gives
$$(s+k)W(s)=k \rArr W(s)=\frac{k}{s+k}$$
Note well, that with modified input on the right hand side of the DE, the system function does not automatically have a 1 in the numerator.  
You might have noticed that in the previous example we could have written $W(s) = \frac{1}{s/k+1}$, which has our usual form. The next example shows that this is not always the case.

**Example 2.** Consider an LC circuit with input voltage $v(t)$. We'll assume $L$ and $C$ are set so the differential equation for the current $i(t)$ is
$$i''(t)+4i=v'(t)$$
We consider the input to be $v(t)$ and the output to be $i(t)$.  
Finding the unit impulse response is tricky, because if we set $v(t) = \delta(t)$ then we will have $\delta(t)$ on the right hand side of the DE. Let's avoid this by using the characterization of the transfer function as the ratio output/input. In this case, we'll have $W(s) = I(s)/V(s)$.  
Assuming rest IC, we have $\mathcal{L}(v')=sV(s)$, where, as usual, we have let the uppercase letter be the Laplace transform of the lowercase one. Applying the Laplace transform gives
$$(s^2+4)I(s)=sV(s) \rArr W(s)=\frac{I(s)}{V(s)}=\frac{s}{s^2+4}$$
The $s$ in the numerator guarantees this cannot be written in the form $1/p(s)$ for any polynomial $p(s)$.  
As a concluding note, we'll say that we were too pessimistic about our ability to handle $\delta(t)$. We might not know what it is, but we do know how to find its Laplace transform.
