We can extend the results above to the inhomogeneous case.
$$\boldsymbol{x'} = A(t)\boldsymbol{x} \text{ (homogeneous)}\tag{H}$$
$$\boldsymbol{x'} = A(t)\boldsymbol{x}+\boldsymbol{F}(t) \text{ (inhomogeneous)}\tag{I}$$
where $F(t)$ is the *input* to the system.

**Linearity/superposition:**
1. If $\boldsymbol{x}_1$ and $\boldsymbol{x}_2$ are solutions to $(H)$ then so is $\boldsymbol{x} = c_1\boldsymbol{x}_1 + c_2\boldsymbol{x}_2$
2. If $\boldsymbol{x}_h$ is a solution to $(H)$ and $\boldsymbol{x}_p$ is a solution to $(I)$ then $\boldsymbol{x} = \boldsymbol{x}_h + \boldsymbol{x}_p$ is also a solution to $(I)$.
3.  If $\boldsymbol{x}_1' = A\boldsymbol{x}_1 + \boldsymbol{F}_1$, and $\boldsymbol{x}_2' = A\boldsymbol{x}_2 + \boldsymbol{F}_2$ then $\boldsymbol{x} = \boldsymbol{x}_1 + \boldsymbol{x}_2$ satisfies $\boldsymbol{x}' = A\boldsymbol{x} + \boldsymbol{F}_1 + \boldsymbol{F}_2$. That is, superposition of inputs leads to superposition of outputs.

**proof:**
1. $\boldsymbol{x}' = c_1\boldsymbol{x}_1' + c_2\boldsymbol{x}_2' = c_1A\boldsymbol{x}_1 + c_2A\boldsymbol{x}_2 = A(c_1\boldsymbol{x}_1 + c_2\boldsymbol{x}_2) = A\boldsymbol{x}$.
2. $\boldsymbol{x}' = \boldsymbol{x}_h' + \boldsymbol{x}_p' = A\boldsymbol{x}_h + A\boldsymbol{x}_p + \boldsymbol{F} = A(\boldsymbol{x}_h + \boldsymbol{x}_p) + \boldsymbol{F} = A\boldsymbol{x} + \boldsymbol{F}$.
3. $\boldsymbol{x}' = \boldsymbol{x}_1' + \boldsymbol{x}_2' = A\boldsymbol{x}_1 + \boldsymbol{F}_1 + A\boldsymbol{x}_2 + \boldsymbol{F}_2 = A(\boldsymbol{x}_1 + \boldsymbol{x}_2) + \boldsymbol{F}_1 + \boldsymbol{F}_2 =A\boldsymbol{x} + \boldsymbol{F}_1 + \boldsymbol{F}_2$.

**Existence and uniqueness:** We start with an initial time $t_0$ and the initial value problem:
$$\boldsymbol{x}' = A(t)\boldsymbol{x} + \boldsymbol{F}(t), \boldsymbol{x}(t_0) = \boldsymbol{x}_0. \tag{IVP}$$
**Theorem:** If $A(t)$ and $\boldsymbol{F}(t)$ are continuous then there exists a unique solution to $(IVP)$.
