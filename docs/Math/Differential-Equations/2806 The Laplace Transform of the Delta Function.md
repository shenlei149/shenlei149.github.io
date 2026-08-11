Since the Laplace transform is given by an integral, it should be easy to compute it for the delta function. The answer is
1. $\mathcal{L}(\delta(t))=1$
2. $\mathcal{L}(\delta(t-a))=e^{-as} \text{ for } a>0$

As expected, proving these formulas is straightforward as long as we use the precise form of the Laplace integral. For $(1)$ we have:
$$\mathcal{L}(\delta(t))=\int_{0^-}^\infty \delta(t)e^{-st}dt=1$$
As we saw in a previous session, integrating $e^{-st}$ against $\delta(t)$ amounts to evaluating $e^{-st}$ at $t = 0$, and $e^0 = 1$. Similarly for the shifted version $(2)$, integrating $e^{-st}$ against $\delta(t - a)$ amounts to evaluating $e^{-st}$ at $t = a$:
$$\mathcal{L}(\delta(t-a))=\int_{0^-}^\infty \delta(t-a)e^{-st}dt=e^{-sa}$$
Notice that the two formulas are consistent: if we set $a = 0$ in formula $(2)$ then we recover formula $(1)$.
