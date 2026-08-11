Periodic functions are functions which repeat: $f(t + P) = f(t)$ for all $t$.

We state this explicitly as the following defintion: a function $f(t)$ is **periodic** with **period** $P > 0$ if
$$f(t + P) = f(t) \text{ for all $t$.}$$
**Example.** $f(t) = \sin(2t$) is periodic with period $P = \pi$.  
This is true because, for all $t$, 
$$f(t + \pi) = \sin(2(t + \pi) = \sin(2t + 2\pi) = \sin(2t) = f(t)$$
Notice, though, that in the example above $f(t) = \sin(2t)$ also has period $P = 2\pi$ and period $P = 3\pi$. In fact, it has period $P = n\pi$ for any integer$n = 1,2,3...$.  
Graphically, a function with period $P$ is one whose graph stays the same if it is shifted $P$ to the left or right.

### Base Period
Most periodic functions have a *minimal* period, which is often called either *the* period or **the base period**. For example, $\sin t$ has minimal period is $2\pi$. It follows from this that the minimal period for $sin(2t)$ is $\pi$.  
The only exception is the constant function. Every value of $P > 0$ is a period and so it has no minimal period. (We don't allow $P = 0$ to be a period because then every function would be periodic with period $P = 0$.)

### Windows
To fully describe a periodic function you only need to specify the period and the value of the function over one full period. We call an interval containing one full period a **window**. Typical choices for windows are $[-P/2, P/2)$ and $[0, P)$, but any interval of length $P$ will work.

### Frequency Terminology
**Angular frequency**, also called **circular frequency** has units of radians/unit time.

**Frequency** has units of cycles/unit time.

Since one cycle is $2\pi$ radians the relationship is
$$angular frequency = 2\pi * frequency$$
The above is the official terminology, but in actual practice many people say frequency when they mean angular frequency. In fact, that has been the general usage earlier in this course where we have called $\omega$ the frequency of $cos(\omega t)$. You will have to use the context to decide exactly which frequency is being used.  
For a function with period $P$ the **base angular frequency** $\omega$ (also called the **fundamental** angular frequency) means the angular frequency corresponding to the base (or minimal) period $P$ that is
$$\omega=\frac{2\pi}{P}$$

### Fourier Series
We will see that a periodic function with base frequency $\omega$ can be written as a sum of sines and cosines whose frequencies are *integer multiples* of $\omega$. This is called the **Fourier series** for the function. That is, sines and cosines, the simplest periodic unctions, are the "building blocks" for more general periodic functions.
