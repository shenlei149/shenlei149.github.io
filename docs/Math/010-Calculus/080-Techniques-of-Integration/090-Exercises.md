### Practice Exercises
135.
$$\int\frac{\sin^2 x}{1+\sin^2 x}dx$$
解：https://math.stackexchange.com/questions/4781459/evaluate-int-frac-sin2-x1-sin2-xdx

$$\frac{\sin^2 x}{1+\sin^2 x}=1-\frac{1}{1+\sin^2 x}$$
所以
$$\int\frac{\sin^2 x}{1+\sin^2 x}dx=x-\int\frac{dx}{1+\sin^2 x}$$
我接受的答案是沿着这个思路的。由于 $\sin$ 的幂次是偶数，所以可以用 $x=\tan^{-1}s$ 做替换，$\sin x=\frac{s}{\sqrt{1+s^2}}, dx=\frac{ds}{1+s^2}$
$$\int\frac{dx}{1+\sin^2 x}=\int\frac{ds}{2s^2+1}=\frac{1}{\sqrt{2}}\tan^{-1}(\sqrt{2}s)$$
令一个人的思路是将分母的 1 写成 $\sin^2 x+\cos^2 x$，然后分子分母同时除以 $\cos^2 x$，使用同样的替换就能得到相同的结果，更容易理解。


### Additional and Advanced Exercises
5.
$$\int\frac{dt}{t-\sqrt{1-t^2}}$$
解：https://math.stackexchange.com/questions/4781860/evaluate-int-fracdtt-sqrt1-t2

令 $t=\sin x,dt=\cos x$，所以
$$\int\frac{dt}{t-\sqrt{1-t^2}}=\int\frac{\cos xdx}{\sin x-\cos x}$$
我接受的答案简洁明了
令
$$C=\int\frac{\cos xdx}{\sin x-\cos x}$$
$$S=\int\frac{\sin xdx}{\sin x-\cos x}$$
$$C+S=\int\frac{\cos x+\sin x}{\sin x-\cos x}=\ln|\sin x-\cos x|+c_2$$
$$C-S=\int\frac{\cos x-\sin x}{\sin x-\cos x}=-\int dx=-x+c_2$$
所以
$$C=\frac{1}{2}(\ln(t-\sqrt{1-t^2})-\sin^{-1}t)+c$$
