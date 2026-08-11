We turn our attention now to the negative side of the problem of showing limit cycles exist. Here are two theorems which can sometimes be used to show that a limit cycle does *not* exist.

### Bendixson's Criterion
If $f_x$ and $g_y$ are continuous in a region $R$ which is simply-connected (i.e., without holes), and
$$\frac{\partial f}{\partial x}+\frac{\partial g}{\partial y}\neq 0 \text{ at any point of } R$$
then the system
$$\begin{aligned}
x'=f(x,y)\\y'=g(x,y)
\end{aligned}\tag{1}$$
has no closed trajectories inside $R$.
**Proof.** Assume there is a closed trajectory $C$ inside $R$. We shall derive a contradiction, by applying Green's theorem, in its normal (or flux) form. This theorem says
$$\oint_C (f\boldsymbol{i}+g\boldsymbol{j})\cdot \boldsymbol{n}ds=\oint_C fdy-gdx=\iint_D (\frac{\partial f}{\partial x}+\frac{\partial g}{\partial y})dxdy\tag{2}$$
where $D$ is the region inside the simple closed curve $C$.  
This however is a contradiction. Namely, by hypothesis, the integrand on the right-hand side is continuous and never 0 in $R$; thus it is either always positive or always negative, and the right-hand side of $(2)$ is therefore either positive or negative.  
On the other hand, the left-hand side must be zero. For since $C$ is a closed trajectory, $C$ is always tangent to the velocity field $(f\boldsymbol{i}+g\boldsymbol{j})$ defined by the system. This means the normal vector $n$ to $C$ is always perpendicular to the velocity field $(f\boldsymbol{i}+g\boldsymbol{j})$, so that the integrand $(f\boldsymbol{i}+g\boldsymbol{j})\cdot \boldsymbol{n}$ on the left is identically zero.  
This contradiction means that our assumption that $R$ contained a closed trajectory of $(1)$ was false, and Bendixson's Criterion is proved.

### Critical-point Criterion
A closed trajectory has a critical point in its interior.  
If we turn this statement around, we see that it is really a criterion for *non-existence*: it says that if a region $R$ is simply-connected (i.e., without holes) and has no critical points, then it cannot contain any limit cycles. For if it did, the Critical-point Criterion says there would be a critical point inside the limit cycle, and this point would also lie in $R$ since $R$ has no holes.  
Note carefully the distinction between this theorem, which says that limit cycles enclose regions which do contain critical points, and the Poincare-Bendixson theorem, which seems to imply that limit cycles tend to lie in regions which *don't* contain critical points. The difference is that these latter regions always contain a hole; the critical points are in the hole. Example 1 illustrated this.  
**Example 2.** For what $a$ and $d$ does $\begin{cases}x'=ax+by\\y'=cx+dy\end{cases}$ have closed trajectories?  
**Solution.** By Bendixson's criterion, $a + d \neq 0 \rArr$ no closed trajecto­ries.  
What if $a + d = 0?$ Bendixson's criterion says nothing. We go back to our analysis of the linear system. The characteristic equation of the system is
$$\lambda^2-(a+d)\lambda+(ad-bc)=0$$
Assume $a + d = 0$. Then the characteristic roots have opposite sign if $ad - bc < 0$ and the system is a saddle; the roots are pure imaginary if $ad - bc > 0$ and the system is a center, which has closed trajectories. Thus
$$\text{the system has closed trajectories} \lrArr a + d = 0, ad - bc > 0.$$
