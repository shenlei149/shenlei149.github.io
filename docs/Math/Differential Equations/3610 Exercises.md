**Ex 1**  
In this problem, $A=\begin{pmatrix}1&1\\-4&1\end{pmatrix}$ and we are interested in the equation $\boldsymbol{u'}=A\boldsymbol{u}$.  
(a) Find a fundamental matrix $\Phi(t)$ for $A$.  
(b) Find the exponential matrix $e^{At}$.  
(c) Find the solution to $\boldsymbol{u'}=A\boldsymbol{u}$ with $\boldsymbol{u}(0)=\begin{bmatrix}1\\2\end{bmatrix}$.  
(d) Find a solution to $\boldsymbol{u'}=A\boldsymbol{u}+\begin{bmatrix}5\\10\end{bmatrix}$. What is the general solution? What is the solution with $\boldsymbol{u}(0)=\boldsymbol{0}$.  
**Solution**  
(a) The eigenvalues of $A$ are solutions to $(1-\lambda)^2+4=\lambda^2-2\lambda+5=0$, which are in this case the complex conjugates $1\plusmn 2i$.  
Choose one of them, say $\lambda_1=1+2i$, and find an eigenvector $v_1$ for it.  
The eigenvectors corresponding to $\lambda_1$ are the vectors in the killed by $A-\lambda_1I=\begin{bmatrix}-2i&1\\-4&-2i\end{bmatrix}$, so we can take, say, $v_1=\begin{bmatrix}1\\2i\end{bmatrix}$.  
This means this equation has a complex-valued solution
$$\begin{aligned}
\boldsymbol{u}(t)&=e^{\lambda_1t}v_1=e^{(1+2i)t}\begin{bmatrix}
1\\2i
\end{bmatrix}\\
&=e^t\begin{bmatrix}
\cos 2t+i\sin 2t\\
2i\cos 2t-2\sin 2t
\end{bmatrix}\\
&=\begin{bmatrix}
e^t\cos 2t\\-2e^t\sin 2t
\end{bmatrix}+i\begin{bmatrix}
e^t\sin 2t\\2e^t\cos 2t
\end{bmatrix}
\end{aligned}$$
By taking real and imaginary parts, we get two independent real solutions, and so can read off a fundamental matrix
$$\Phi(t)=\begin{bmatrix}
e^t\cos 2t&e^t\sin 2t
\\-2e^t\sin 2t&2e^t\cos 2t
\end{bmatrix}$$
(b) we can compute the exponential matrix for this system from any fundamental matrix $\Phi(t)$ found in the first part by using the formula
$$e^{At}=\Phi(t)\Phi(0)^{-1}$$
$$\Phi(0)=\begin{bmatrix}
1&0\\0&2
\end{bmatrix}$$
so
$$\Phi(0)^{-1}=\begin{bmatrix}
1&0\\0&1/2
\end{bmatrix}$$
$$\begin{aligned}
e^{At}&=\begin{bmatrix}
e^t\cos 2t&e^t\sin 2t
\\-2e^t\sin 2t&2e^t\cos 2t
\end{bmatrix}\begin{bmatrix}
1&0\\0&1/2
\end{bmatrix}\\
&=\begin{bmatrix}
e^t\cos 2t&\frac{1}{2}\sin 2t\\
-2e^t\sin 2t&e^t\cos 2t
\end{bmatrix}
\end{aligned}$$
(c) The general strategy is to find the solution with a given initial condition directly from the exponential matrix:
$$\begin{aligned}
\boldsymbol{u}(t)&=e^{At}\boldsymbol{u}(0)\\
&=\begin{bmatrix}
e^t\cos 2t&\frac{1}{2}\sin 2t\\
-2e^t\sin 2t&e^t\cos 2t
\end{bmatrix}\begin{bmatrix}
1\\2
\end{bmatrix}\\
&=\begin{bmatrix}
e^t(\cos 2t+\sin 2t)\\2e^t(-\sin 2t+\cos 2t)
\end{bmatrix}
\end{aligned}$$
(d) We guess a constant solution
$$\boldsymbol{u}=\begin{bmatrix}
k_1\\k_2
\end{bmatrix}$$
Substituting this into the DE gives
$$\begin{bmatrix}
0\\0
\end{bmatrix}=A\begin{bmatrix}
k_1\\k_2
\end{bmatrix}+\begin{bmatrix}
5\\10
\end{bmatrix}$$
This implies
$$\boldsymbol{u}=-A^{-1}\begin{bmatrix}
5\\10
\end{bmatrix}=-\frac{1}{5}\begin{bmatrix}
1&-1\\4&1
\end{bmatrix}\begin{bmatrix}
5\\10
\end{bmatrix}=\begin{bmatrix}
1\\-6
\end{bmatrix}$$
Since all homogeneous solutions are of the form $e^{At}\begin{bmatrix}a\\b\end{bmatrix}$, the general solution is then given by
$$e^{At}\begin{bmatrix}a\\b\end{bmatrix}+\begin{bmatrix}
1\\-6
\end{bmatrix}=\begin{bmatrix}
e^t(a\cos 2t+\frac{1}{2}b\sin 2t)+1\\
e^t(-2a\sin 2t+b\cos 2t)-6
\end{bmatrix}$$
for some constants $a, b$.  
To find the particular solution with $\boldsymbol{u}(0)=0$, we plug $t = 0$ into this expression, and get that
$$\begin{bmatrix}
a+1\\b-6
\end{bmatrix}=\begin{bmatrix}
0\\0
\end{bmatrix}$$
so the desired solution is given by the constants $a = -1$ and $b = 6$:
$$\begin{bmatrix}
e^t(-\cos 2t+3\sin 2t)+1\\
e^t(2\sin 2t+6\cos 2t)-6
\end{bmatrix}$$

**Ex 2**  
Suppose $\boldsymbol{u}_1(t)=\begin{bmatrix}1\\1\end{bmatrix}$ (the constant trajectory) and $\boldsymbol{u}_2(t)=\begin{bmatrix}e^t\\-e^{t}\end{bmatrix}$ are solutions to the equation $\boldsymbol{u'}=B\boldsymbol{u}$ for some matrix $B$.  
(a) What is the general solution? What is the solution $\boldsymbol{u}(t)$ with $\boldsymbol{u}(0)=\begin{bmatrix}2\\2\end{bmatrix}$? What is the solution with $\boldsymbol{u}(0)=\begin{bmatrix}1\\0\end{bmatrix}$?  
(b) Find a fundamental matrix, and compute the exponential $e^{Bt}$. What is $e^B$?  
(c) What are the eigenvalues and eigenvectors of $B$?  
(d) What is $B$?  
**Solution.**  
(a) The general solution is
$$\boldsymbol{u}(t)=a\boldsymbol{u}_1(t)+b\boldsymbol{u}_2(t)=a\begin{bmatrix}1\\1\end{bmatrix}+b\begin{bmatrix}e^t\\-e^{t}\end{bmatrix}$$
We can find the particular solution that satisfies any given initial condition $\boldsymbol{u}(0)$ explicitly by solving the system of equations given by $a\boldsymbol{u}_1(0) + b\boldsymbol{u}_2(0) = \boldsymbol{u}(0)$ for $a$ and $b$.  
Here $\boldsymbol{u}_1(0)=\begin{bmatrix}1\\1\end{bmatrix}$ and $\boldsymbol{u}_2(0)=\begin{bmatrix}1\\-1\end{bmatrix}$, so the solution with initial condition $\boldsymbol{u}(0)=\begin{bmatrix}2\\2\end{bmatrix}$ is the constant trajectory
$$2\boldsymbol{u}_1(t)=\begin{bmatrix}
2\\2
\end{bmatrix}$$
and the solution with initial condition $\boldsymbol{u}(0)=\begin{bmatrix}1\\0\end{bmatrix}$ is
$$\frac{1}{2}\boldsymbol{u}_1(t)+\frac{1}{2}\boldsymbol{u}_2(t)=\begin{bmatrix}
1/2+e^t/2\\1/2-e^t/2
\end{bmatrix}$$
(b) Since the two given solutions are linearly independent, a fundamental matrix is
$$\begin{bmatrix}
1&e^t\\1&-e^t
\end{bmatrix}$$
Evaluating this matrix at $t = 0$ gives $\begin{bmatrix}1&1\\1&-1\end{bmatrix}$ which has inverse $\frac{1}{2}\begin{bmatrix}1&1\\1&-1\end{bmatrix}$, so the exponential matrix is
$$\begin{aligned}
e^{Bt}&=\begin{bmatrix}
1&e^t\\1&-e^t
\end{bmatrix}\frac{1}{2}\begin{bmatrix}1&1\\1&-1\end{bmatrix}\\
&=\frac{1}{2}\begin{bmatrix}
1+e^t&1-e^t\\1-e^t&1+e^t
\end{bmatrix}
\end{aligned}$$
Evaluating $e^{Bt}$ at $t = 1$ gives us $e^B=\frac{1}{2}\begin{bmatrix}1+e&1-e\\1-e&1+e\end{bmatrix}$  
(c) $\boldsymbol{u}_1=\begin{bmatrix}1\\1\end{bmatrix}$, so $\lambda_1=0$ and $\boldsymbol{v}_1=\begin{bmatrix}1\\1\end{bmatrix}$  
$\boldsymbol{u}_2(t)=\begin{bmatrix}e^t\\-e^{t}\end{bmatrix}=e^t\begin{bmatrix}1\\-1\end{bmatrix}$, so $\lambda_2=1$ and $\boldsymbol{v}_2=\begin{bmatrix}1\\-1\end{bmatrix}$  
(d) $\boldsymbol{u'}=B\boldsymbol{u}$
From the equation for the first eigenvalue
$$\begin{bmatrix}
0\\0
\end{bmatrix}=B\begin{bmatrix}
1\\1
\end{bmatrix}$$
so $B$ has the form $\begin{bmatrix}a&-a\\c&-c\end{bmatrix}$  
From the equation for the second eigenvalue
$$e^t\begin{bmatrix}1\\-1\end{bmatrix}=\begin{bmatrix}a&-a\\c&-c\end{bmatrix}\begin{bmatrix}e^t\\-e^t\end{bmatrix}$$
so $a=1/2$ and $c=-1/2$. Thus,
$$B=\begin{bmatrix}
1/2&-1/2\\-1/2&1/2
\end{bmatrix}$$

**Ex 3.**  
Say that a pair of solutions $x_1(t), x_2(t)$ of the equation $mx'' + bx' + kx = 0$ is *normalized* at $t = 0$ if:
$$x_1(0)=1,x_1'=0$$
$$x_2(0)=0,x_2'=1$$
For example, find the normalized pair of solutions to $x''+2x'+2x=0$. Then find $e^{At}$ where $A$ is the companion matrix for the operator $D^2 + 2D + 2I$.  
**Solution.**    
$s^2 + 2s + 2 = (s + 1)^2 + 1$ so the roots of the characteristic polynomial are $-1 \plusmn i$.  
Basic solutions are given by 
$$y_1=e^{-t}\cos t \text{ and } y_2=e^{-t}\sin t$$
$$y_1(0)=1, y_1'(0)=-1,y_2(0)=0,y_2'(0)=1$$
So $x_1 = y_1 + y_2$ and $x_2 = y_2$ form a normalized pair of solutions:
$$x_1(t)=e^{-t}(\cos t+\sin t) \text{ and } x_2(t)=e^{-t}\sin t$$
From 3304, we know
> The corresponding coefficient matrix is
> $$A=\begin{pmatrix}0&1\\-k&-b\end{pmatrix}$$
The companion matrix is $A=\begin{bmatrix}0&1\\-2&-2\end{bmatrix}$.  
Its characteristic polynomial is the same, $\lambda^2+2\lambda+2$, so its eigenvalues are the same, $-1\plusmn i$.  
An eigenvector for value $-1+i$ is given by $\boldsymbol{v}_1$ such that
$$\begin{bmatrix}
-1+i&1\\-2&-1-i
\end{bmatrix}\boldsymbol{v}_1=\begin{bmatrix}
0\\0
\end{bmatrix}$$
We can take $\boldsymbol{v}_1=\begin{bmatrix}1\\-1+i\end{bmatrix}$. The corresponding normal mode is
$$\begin{aligned}
e^{-1+i}\begin{bmatrix}
1\\-1+i
\end{bmatrix}&=e^{-t}(\cos t+i\sin t)\begin{bmatrix}
1\\-1+i
\end{bmatrix}\\
&=e^{-t}(\cos t\begin{bmatrix}
1\\-1
\end{bmatrix}+i\sin t\begin{bmatrix}
0\\i
\end{bmatrix})+e^{-t}(\cos t\begin{bmatrix}
0\\i
\end{bmatrix}+i\sin t\begin{bmatrix}
1\\-1
\end{bmatrix})\\
&=e^{-t}(\begin{bmatrix}
\cos t\\-\cos t-\sin t
\end{bmatrix}+i\begin{bmatrix}
\sin t\\\cos t-\sin t
\end{bmatrix})
\end{aligned}$$
Thus
$$\Phi(t)=\begin{bmatrix}
\cos t&\sin t\\-\cos t-\sin t&\cos t-\sin t
\end{bmatrix}$$
$$\Phi(0)=\begin{bmatrix}
1&0\\-1&1
\end{bmatrix}$$
$$\Phi(0)^{-1}=\begin{bmatrix}
1&0\\1&1
\end{bmatrix}$$
so
$$\begin{aligned}
e^{At}&=\Phi(t)\Phi(0)^{-1}\\
&=\begin{bmatrix}
\cos t+\sin t&\sin t\\
-2\sin t&\cos t-\sin t
\end{bmatrix}
\end{aligned}$$

**Ex 4.**  
We have seen that a complex number $z = a + bi$ determines a matrix $A(z)$ in the following way: $A(a + bi) =\begin{bmatrix}a&-b\\b&a\end{bmatrix}$. This matrix represents the operation of multiplication by $z$, in the sense that if $z(x + yi) = v + wi$ then $A(z)\begin{bmatrix}x\\y\end{bmatrix}=\begin{bmatrix}v\\w\end{bmatrix}$. What is $e^{A(z)t}$? What is $A(e^{zt})$?  
**Solution.**  
With $A(z)=\begin{bmatrix}a&-b\\b&a\end{bmatrix}$,
$$p_A(\lambda)=(\lambda-a)^2+b^2$$
so the eigenvalues are $a \plusmn bi$. An eigenvector for $\lambda_1 = a + bi$ is given by $\boldsymbol{v}_1$ such that
$$\begin{bmatrix}
-bi&-b\\b&-bi
\end{bmatrix}\boldsymbol{v}_1=\begin{bmatrix}
0\\0
\end{bmatrix}$$
and we can take $\boldsymbol{v}_1 = \begin{bmatrix}1\\-i\end{bmatrix}$. The corresponding normal mode is
$$\begin{aligned}
e^{(a+bi)t}\begin{bmatrix}1\\-i\end{bmatrix}&=e^{at}(\cos bt + i\sin bt)\begin{bmatrix}1\\-i\end{bmatrix}\\
&=e^{at}(\cos bt\begin{bmatrix}
1\\0
\end{bmatrix}+i\sin bt\begin{bmatrix}
0\\-i
\end{bmatrix}+\cos bt\begin{bmatrix}
0\\-i
\end{bmatrix}+i\sin bt\begin{bmatrix}
1\\0
\end{bmatrix})\\
&=e^{at}(\begin{bmatrix}
\cos bt\\\sin bt
\end{bmatrix}+i\begin{bmatrix}
\sin bt\\-\cos bt
\end{bmatrix})
\end{aligned}$$
So a fundamental matrix is given by
$$\Phi(t)=e^{at}\begin{bmatrix}
\cos bt&\sin bt\\\sin bt&-\cos bt
\end{bmatrix}$$
$$\Phi(0)=\begin{bmatrix}
1&0\\0&-1
\end{bmatrix}$$
$$\Phi(0)^{-1}=\begin{bmatrix}
1&0\\0&-1
\end{bmatrix}$$
So
$$e^{At}=\Phi(t)\Phi(0)^{-1}=e^{at}\begin{bmatrix}
\cos bt&-\sin bt\\\sin bt&\cos bt
\end{bmatrix}$$
$$\begin{aligned}
A(e^{zt})&=A(e^{(a+bi)t})=A(e^{at}(\cos bt+i\sin bt))\\
&=\begin{bmatrix}
e^{at}\cos bt&-e^{at}\sin bt\\
e^{at}\sin bt&e^{at}\cos bt
\end{bmatrix}=e^{at}\begin{bmatrix}
\cos bt&-\sin bt\\\sin bt&\cos bt
\end{bmatrix}\\
&=e^{At}=e^{A(z)t}=e^{A(a+bi)t}
\end{aligned}$$
