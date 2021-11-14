Fourier series can be used to solve an inhomogeneous linear time invariant (LTI) DE
$$p(D)x = f(t)\tag{1}$$
in the case where $f(t)$ is a periodic function. The main idea is to use Fourier series and the superposition principle to reduce the problem to solving $(1)$ when the input $f(t)$ is a sinusoid.

Very briefly: if $f(t)$ is periodic then its Fourier series gives $f$ as a sum of sines and cosines. The superposition principle allows us to find the response of $(1)$ to each of the sine or cosine terms separately and then superposition these individual responses to find the response of the system to the periodic signal $f$.

The particular solution found in this way will also be a sum of sines and cosines with the same period as the input. We will call it the *steady-periodic solution*, $x_{sp}(t)$. One way in which the steady-periodic solution is useful is that it reveals when resonances will occur, and at what frequencies.

In this session we will also show how Fourier series can be used to analyze sound waves.
