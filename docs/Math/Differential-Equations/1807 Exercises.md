**Problem 1:** Find a particular solution to the DE
$$x''+x=t^2+\cos(2t-1)$$
**Solution:** $x_p=x_{p,1}+x_{p,2}$, where $p(D)x_{p,1}=t^2$, and $p(D)x_{p,2}=\cos(2t-1)$, by superposition. Here $p(D)=D^2+1$.  
try a solution of form $x_{p,1}=At^2+Bt+C$
$$x_{p,1}'=2At+B, x_{p,1}''=2A$$
$$x_{p,1}''+x_{p,1}=2A+At^2+Bt+C=t^2$$
$$A=1, B=0, C=-2$$
try a solution of form $x_{p,2}=A\cos(2t-1)+B\sin(2t-1)$
$$x_{p,2}'=-2A\sin(2t-1)+2B\cos(2t-1)$$
$$x_{p,2}''=-4A\cos(2t-1)-4B\sin(2t-1)$$
$$x_{p,2}''+x_{p,2}=-3A\cos(2t-1)-3B\sin(2t-1)=\cos(2t-1)$$
$$A=-1/3,B=0$$
so
$$x_p=x_{p,1}+x_{p,2}=t^2-2-\frac{1}{3}\cos(2t-1)$$

**Problem 2:** Find the general solution to
$$y''+y'+y=2xe^x$$
**Solution:** Characteristic equation:
$$p(s)=s^2+s+1$$
$$r=\frac{-1\pm \sqrt{-3}}{2}$$
so the solution to the homogeneous equation is
$$y_h=e^{-x/2}(c_1\cos\frac{\sqrt{3}}{2}x+c_2\sin \frac{\sqrt{3}}{2}x)$$
To find the particular solution, try
$$y_p=Axe^x+Be^x$$
$$y_p'=Ae^x+Axe^x+Be^x=Axe^x+(A+B)e^e$$
$$y_p''=Ae^x+Axe^x+(A+B)e^x=Axe^x+(2A+B)e^e$$
$$y_p''+y_p'+y_p=3Axe^x+(3A+3B)e^x=2xe^x$$
$$A=2/3, B=-2/3$$
Thus the general solution is
$$y=e^{-x/2}(c_1\cos\frac{\sqrt{3}}{2}x+c_2\sin \frac{\sqrt{3}}{2}x)+\frac{2}{3}e^x(x-1)$$

**Problem 3:** Find a particular solution to the DE
$$y^{(4)}-2y''+y=xe^x$$
**Solution:**  
$$p(s)=s^4-2s^2+1=(s^2-1)^2$$
so $p(1) = 0$ repeated root (order 2), so try
$$y_p=x^2(Ax+B)e^x$$
Use the exponential shift rule
$$p(D)e^xu=e^xp(D+1)u$$
so
$$p(D+1)(x^2(Ax+B))=x$$
$$(Ax^3+Bx^2)'=3Ax^2+2Bx\\
(Ax^3+Bx^2)''=6Ax+2B\\
(Ax^3+Bx^2)'''=6A\\
(Ax^3+Bx^2)^{(4)}=0$$
$$p(D+1)=(D+1)^4-2(D+1)^2+1=D^4+4D^3+4D^2$$
$$\begin{aligned}
&p(D+1)(x^2(Ax+B))\\
=&24A+24Ax+8B\\
=&x
\end{aligned}$$
$$A=1/24, B=-3/24$$
so
$$x_p=\frac{x^2e^x}{24}(x-3)$$
