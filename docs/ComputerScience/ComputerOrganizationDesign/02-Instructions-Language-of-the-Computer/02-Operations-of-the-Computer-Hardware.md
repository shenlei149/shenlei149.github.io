每个计算机必须能执行算术操作。RISC-V 的加法指令如下
```c
add a, b, c
```
意思是把 `b, c` 相加存储到 `a` 中。

如果要把变量 `b, c, d, e` 之和放到变量 `a` 中，需要三条指令。
```c
add a, b, c // The sum of b and c is placed in a
add a, a, d // The sum of b, c, and d is now in a
add a, a, e // The sum of b, c, d, and e is now in a
```
RISC-V 规定一行最多一个指令。这里没有给出变量的含义，下一节会解释。

加法天然的三个操作数，如果规定所有指令都需要三个操作数的话，那么硬件设计就会变得很简单。这就是三个硬件设计原则的第一个：

*Design Principle 1*: Simplicity favors regularity.

*设计原则 1*：简单有利于规律性。

下面是关于加减法的例子。
```c
a = b + c;
d = a − e;

add a, b, c
sub d, a, e
```

```c
f = (g + h) − (i + j);

add t0, g, h    // temporary variable t0 contains g + h
add t1, i, j    // temporary variable t1 contains i + j
sub f, t0, t1   // f gets t0 − t1, which is (g + h) − (i + j)
```
