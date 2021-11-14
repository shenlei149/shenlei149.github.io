**Ex 1.** What is the Fourier series for $\sin^2t$?

We could compute the Fourier coefficients directly from the formulas, but instead we use a trig identity. By the double angle formula, $\cos(2t) = 1 - 2 \sin^2t$, so
$$\sin^2t=\frac{1}{2}-\frac{1}{2}\cos(2t)$$
The right hand side is a Fourier series; it happens to be finite here. That is, the Fourier series for $\sin^2t$ has only two nonzero coefficients. When we regard $\sin^2t$ as having period $2\pi$, its series has Fourier coefficients $a_0 = 1$ and $a_2 = -1/2$.

This answer makes sense for two reasons. First, $\sin^2t$ is an even function, and here all the $b_n$'s are zero. Second, we expect polynomial functions of sine and cosine to have short Fourier series.

A remark from the point of view of material to be introduced later: This function has minimal period $\pi$, so it might be more natural to speak about its Fourier series for period $\pi$. This would be the same series, but the coefficients would be indexed differently. (If we thought of this Fourier series as having period $\pi$, $a_0$ and $a_1$ would be the nonzero coefficients.)

**Ex 2.** Explain why any function $F(x)$ is a sum of an even function and an odd function in just one way. What is the even part of $e^x$? What is the odd part?

This is a standard question to ask, and an important method to know.

An easy way to make an even function from an arbitrary $F(x)$ is to take the sum $F(x) + F(-x)$. (Why is this even? $F(x)+F(-x)=F(-x)+F(x)[f(x)=f(-x)]$)  
Similarly, subtracting $F(x) - F(-x)$ gives an odd function. (Check this is odd. $F(x) - F(-x)=-(F(-x)-F(x))[f(x)=-f(x)]$)

Adding the two together would give $2F(x)$, so we go back and divide by this factor of two:
$$F(x)=\frac{F(x) + F(-x)}{2}+\frac{F(x) - F(-x)}{2}$$
To show that this decomposition is unique, we suppose we have another decomposition $F_{even}(x) + F_{odd}(x) = F(x)$, where $F_{even}(x)$ is even and $F_{odd}(x)$ is odd.  
We are assuming that$F_{even}(x) + F_{odd}(x) = F(x) = \frac{F(x) + F(-x)}{2}+\frac{F(x) - F(-x)}{2}$. Rearranging terms, this means that
$$F_{even}(x)-\frac{F(x) + F(-x)}{2}=-F_{odd}(x)+\frac{F(x) - F(-x)}{2}$$
The left hand side here is the sum of two even functions, so it is also even, and, similarly, the right-hand side is the sum of two odd functions, so it is odd. But then each side is simultaneously both even and odd, and has to be zero.
Thus, $F_{even}(x)=\frac{F(x) + F(-x)}{2}$ and $F_{odd}(x)=\frac{F(x) - F(-x)}{2}$, so the even-odd decomposition of a function is unique.

This decomposition might seem familiar from hyperbolic trig function formulas: The even part of $e^x$ is $\frac{e^x+e^{-x}}{2}=\cosh x$, and the odd part of $e^x$ is $\frac{e^x-e^{-x}}{2}=\sinh x$.
