`Operators` are  to functions as functions are  to numbers. An operator takes a function, does something to it, and returns this modified function.

There are lots of examples of operators around:
- The `shift-by-aoperator` (where ais a number) takes as input a function $f(t)$ and gives as output the function $f(t-a)$. This operator shifts graphs to the right by `a` units.
- The `multiply-by-h(t) operator` (where $h(t)$ is a function) multiplies by $h(t)$: it takes as input the function $f(t)$ and gives as output the function $h(t)f(t)$.

You can go on to invent many other operators. In this course `the most important` operator is:
- The `differentiation operator`, which carries a function $f(t)$ to its derivative $f'(t)$. The differentiation operator is usually denoted by the letter `D`; so $Df(t)$is the function $f'(t)$. D carries $f$ to $f'$. For example, $Dt^3 =3t^2$. This is usually read as "D applied to $t^3$.”

The `identity operator` takes an input function $f(t)$ and returns the same function, $f(t)$; it does nothing, but it still gets a symbol, `I`: $If=f$.

Operators can be added and multiplied by numbers or more generally by functions. Thus $tD+4I$ is the operator sending $f(t)$ to $tf'(t) + 4f(t)$.

The single most important concept associated with operators is that they can be `composed` with each other. Composition of two operators in a given order means that the two operators are applied to a function one after the other. For example, $D^2$, the second-derivative operator, means dif­ferentiation twice, sending $f(t)$ to $f''(t)$. It is in fact the composition of D with itself: $D^2=D•D$, so that $D^2f=D(Df)=D(f')=f''$.
