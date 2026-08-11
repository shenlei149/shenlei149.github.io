**Exercice.** Find a solution of $x''+3x'+2x=te^{-t}$.  
**Solution.** We use the method of variation of parameters.  
Suppose there is a solution of the form $x(t) =u(t)e^{-t}$ for some function $u(t)$.
$$x'=(u'-u)e^{-t}$$
$$x''=(u''-2u'+u)e^{-t}$$
and the equation becomes
$$
\begin{aligned}
x''+3x'+x&=[(u''-2u'+u)+3(u'-u)+2u]e^{-t}\\
&=(u''+u')e^{-t}\\
&=te^{-t}
\end{aligned}
$$
The exponential is never zero, so can cancel $e^{-t}$ from both sides and obtain
$$u''+u'=t$$
Lowest order derivative is 1, so we should bump up all degrees by 1. So try $u=At^2+Bt+C$.  
$$u'=2At+B$$
$$u''=2A$$
$$u''+u'=2At+(2A+B)=t$$
$$A=1/2\text{ and }B=-1$$
so
$$u=\frac{1}{2}t^2-t$$
$$x=(\frac{t^2}{2}-t)e^{-t}$$

**Exercice.**  
(a) Find a solution of $x''+x=5te^{2t}$.  
(b) Suppose that $y(t)$ is a solution of the same equation, $x''+x=5te^{2t}$, such that $y(0) = 1$ and $y'(0)=2$. (This is probably *not* the solution you found in **(a)**.) Use $y(t)$ and other functions to write down a solution $x(t)$ such that $x(0) = 3$ and $x'(0) = 5$.  
**Solution.**  
(a) Variation of parameters:
$$x=ue^{2t}$$
$$x'=(u'+2u)e^{2t}$$
$$x''=2(u'+2u)e^{2t}+(u''+2u')e^{2t}=(u''+4u'+2u)e^{2t}$$
so
$$x''+x=(u''+4u'+5u)e^{2t}$$
and $u$ must satisfy
$$u''+4u'+5u=5t$$
Undetermined coefficients:
$$u_p=at+b$$
$$u_p'=a$$
$$u_p''=0$$
so
$$4a+5(at+b)=5t$$
$$a=1,b=-4/5$$
$$u_p=t-4/5$$
$$x_p=(t-\frac{4}{5})e^{2t}$$
(b) The homogeneous equation has general solution $a\cos t+b\sin t$,  so the general solution of $x''+x=5te^{2t}$ is $x = y + a\cos t+b\sin t$.
$$x(0)=3=y(0)+a=1+a\rArr a=2$$
$$x'(0)=5=y'(0)+b=2+b\rArr b=3$$
so
$$x=y+2\cos t+3\sin t$$
