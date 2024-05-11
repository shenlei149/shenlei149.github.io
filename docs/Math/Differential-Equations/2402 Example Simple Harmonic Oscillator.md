**Example.** Let $f(t) =$ the odd square wave of period $2\pi$ with $f(t) = 1$ for $0 < t < 1$. Use Fourier series to solve the DE
$$x''+9.1x=f(t)\tag{1}$$
**Solution.** From previous examples we know the Fourier series for $f(t)$,
$$f(t)=\frac{4}{\pi}(\sin t+\frac{\sin 3t}{3}+\frac{\sin 5t}{5}+\ldots)=\frac{4}{\pi}\sum_{n\text{ odd}}\frac{\sin nt}{n}$$
So the DE $(1)$ becomes
$$x''+9.1x=\frac{4}{\pi}(\sin t+\frac{\sin 3t}{3}+\frac{\sin 5t}{5}+\ldots)\tag{2}$$
Step 1: Solve the DE with a single sine function as input. That is, solve
$$x_n''+9.1x_n=\frac{\sin nt}{n}\tag{3}$$
Notice, we use the index $n$ so we can tell our solutions apart. Also notice that equation $(3)$ does not include the factor $\frac{4}{\pi}$; we will bring that back in the superposition step.  
We have a lot of experience solving equation $(3)$. using complex replacement and the Exponential Response formula. We get particular solutions
$$x_{n,p}(t)=\frac{\sin nt}{n(9.1-n^2)}$$
Step 2: Use superposition to get a particular solution $x_p$ to $(2)$. Here we line up the DE and the solution so you can see superposition in action:
$$
\begin{aligned}
x''+9.1x&=\frac{4}{\pi}(\sin t&+\frac{\sin 3t}{3}&+\frac{\sin 5t}{5}&+\ldots)&=\frac{4}{\pi}\sum_{n\text{ odd}}\frac{\sin nt}{n}\\
x_{sp}(t)&=\frac{4}{\pi}(x_{1,p}(t)&+x_{3,p}(t)&+x_{5,p}(t)&+\ldots)&=\frac{4}{\pi}\sum_{n\text{ odd}}x_{n,p}(t)\\
&=\frac{4}{\pi}(\frac{\sin t}{9.1-1}&+\frac{\sin 3t}{3(9.1-9)}&+\frac{\sin 5t}{5(9.1-25)}&+\ldots)&=\frac{4}{\pi}\sum_{n\text{ odd}}\frac{\sin nt}{n(9.1-n^2)}\tag{4}
\end{aligned}
$$
This is called the *steady periodic solution*.

**Near resonance:** The amplitudes of each of the terms in $(4)$ are:
$$\frac{4}{\pi}\frac{1}{9.1-1}\approx0.157$$
$$\frac{4}{\pi}\frac{1}{3(9.1-9)}\approx4.244$$
$$\frac{4}{\pi}\frac{1}{5(9.1-25)}\approx-0.016$$
for $n = 1, 3, 5$ respectively. Then for $n > 5$ the amplitudes are much smaller. We see the $n = 3$ term in the steady periodic response $x_{sp}(t)$ has by far the biggest amplitude.

We can explain this by noticing that the natural frequency of this system is $\sqrt{9.1}\approx 3$ and so, the system has a resonant-type response to the "embedded third harmonic" $\frac{\sin 3t}{3}$ in the input signal.  
Notice that the input signal has base (fundamental) frequency 1, so the presence of this third harmonic is not apparent to the eye, and yet the driven oscillator picked it out in its response, which has a dominant frequency *three times* the fundamental frequency of the input.  
There is a simple way to visualize this type of phenomenon: you can push a pendulum swing into resonance even if you give it a push only every third time it comes momentarily to rest at its maximum height, instead of pushing it every time.
