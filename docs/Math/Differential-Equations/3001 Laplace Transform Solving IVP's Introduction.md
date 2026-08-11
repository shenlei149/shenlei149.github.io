In this session we apply the Laplace transform techniques we have learned to solving intitial value problems for LTI DE's $p(D)x = f(t)$. For signals $f(t)$ which are discontinuous or impulsive, using the Laplace transform is often the most efficient solution method.

We start by deriving the simple relations between the Laplace transform of a derivative of a function and the Laplace transform of the function itself. Our goal is to use these formulas to solve IVP's of the form
$$p(D)x = f(t) \text{ (with initial conditions)}$$
We do this by Laplace transforming both sides of the DE and solving for the function $X(s) = \mathcal{L}(x(t))$. It turns out that the resulting equation for $X(s)$ is a simple algebraic equation which can be solved immediately. Then one recovers the solution $x(t)$ by computing the inverse Laplace transform $x(t) = \mathcal{L}^{-1}(X(s))$.
