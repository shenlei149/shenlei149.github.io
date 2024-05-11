**Ex 1.** For each spring-mass system, find whether pure resonance occurs, without actually calculating the solution.  
a) $2x''+10x=F(t); F(t) = 1 \text{ on } (0,1), F(t)$ is odd, and of period 2;  
a) $x''+4\pi^2x=F(t); F(t) = 2t \text{ on } (0,1), F(t)$ is odd, and of period 2;  
a) $x''+9x=F(t); F(t) = 1 \text{ on } (0,\pi), F(t)$ is odd, and of period 2$\pi$;

Consider
$$mx''+kx=F(t)$$
The natural frequency of this spring-mass system is
$$\omega_0=\sqrt{\frac{k}{m}}$$
The typical term of the Fourier expansion of $F(t)$ is $\cos \frac{n\pi}{L}t$, $\sin \frac{n\pi}{L}t$; thus we get pure resonance if and only if the Fourier series has a term of the form $\cos \frac{n\pi}{L}t$ or $\sin \frac{n\pi}{L}t$, where $\frac{n\pi}{L} = \omega_0$.

a) $\omega_0=\sqrt{5}$ for spring-mass system, and $L = 1$. Fourier series is $\sum b_n\sin n\pi t$; $n\pi \neq \sqrt{5}$, so *no resonance*.  
b) $\omega_0=2\pi$, $L = 1$. Fourier series is $\sum b_n\sin n\pi t$, and $n\pi=2\pi$ if $n=2$. Thus, *do get
resonance*.  
c) $\omega_0=3$, Fourier series is a sine series ($F(t)$ is odd): $F(t)=\sum b_n\sin n t$ - all odd $n$ occur ($b_n=0$ if $n$ is even), so $n = 3$ occurs and *do get resonance*.
