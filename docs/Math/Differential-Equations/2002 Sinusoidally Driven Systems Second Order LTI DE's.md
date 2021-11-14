We start with the second order linear constant coefﬁcient (CC) DE, which as we've seen can be interpreted as modeling a **damped forced harmonic oscillator**. If we further specify the oscillator to be a mechanical system with mass $m$, damping coefﬁcient $b$, spring constant $k$, and with a *sinusoidal driving* force $B\cos \omega t$ (with $B$ constant), then the DE is
$$mx''+bx'+kx=B\cos \omega t\tag{1}$$
For many applications it is of interest to be able to predict the periodic response of the system to various values of $\omega$.

In the sessions on Exponential Response and Gain & Phase Lag we worked out the general case of a sinusoidally driven LTI DE. Specializing these results to the second order case we have:  
Characteristic polynomial:
$$p(s)=ms^2+bs+k$$
Complex replacement:
$$mz''+bz'+kz=Be^{i\omega t}, x=Re(z)$$
Exponential Response Formula:
$$
z_p=\frac{Be^{i\omega t}}{p(i\omega)}=\frac{Be^{i\omega t}}{k-m\omega^2+ib\omega}
$$
$$
\begin{aligned}
x_p=Re(z_p)&=Re(\frac{k-m\omega^2-ib\omega}{(k-m\omega^2)^2+(b\omega)^2}(B(\cos \omega t+i\sin \omega t)))\\
&=\frac{B((k-m\omega^2)\cos \omega t + b\omega \sin \omega t)}{(k-m\omega^2)^2+(b\omega)^2}\\
&=\frac{B}{\sqrt{(k-m\omega^2)^2+(b\omega)^2}}\cos (\omega t - \phi)
\end{aligned}
$$
where $\phi=Arg(p(i\omega))=\tan^{-1}(\frac{b\omega}{k-m\omega^2})$. (In this case $\phi$ must be between 0 and $\pi$. We say $\phi$ is in the ﬁrst or second quadrants.)  
Letting $A=\frac{B}{\sqrt{(k-m\omega^2)^2+(b\omega)^2}}$, we can write the periodic response $x_p$ as
$$x_p=A\cos (\omega t - \phi)$$

The *complex gain*, which is deﬁned as the ratio of the amplitude of the output to the amplitude of the input in the *complexiﬁed* equation, is
$$\widetilde g(\omega)=\frac{1}{p(i\omega)}=\frac{1}{k-m\omega^2+ib\omega}$$

The *gain*, which is deﬁned as the ratio of the amplitude of the output to the amplitude of the input in the *real* equation, is
$$g=g(\omega)=\frac{1}{\left\lvert p(i\omega)\right\rvert}=\frac{1}{\sqrt{(k-m\omega^2)^2+(b\omega)^2}}\tag{2}$$

The *phase lag* is
$$
\phi=\phi(\omega)=Arg(p(i\omega))=\tan^{-1}(\frac{b\omega}{k-m\omega^2})\tag{3}
$$
and we also have the *time lag* = $\phi/\omega$.

### Terminology of Frequency Response
We call the gain $g(\omega)$ the **amplitude response** of the system. The phase lag $\phi(\omega)$ is called the **phase response** of the system. We refer to them collectively as the **frequency response** of the system.
