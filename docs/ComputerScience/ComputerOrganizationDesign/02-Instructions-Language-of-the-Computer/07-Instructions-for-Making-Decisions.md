代码要有决策能力，比如编程语言中常见的 `if` 有时配合 `goto` 和一个标签。RISC-V 中提供两个个类似配合 `goto` 的 `if` 语句
```c
beq rs1, rs2, L1
```
当寄存器 `rs1` 和 `rs2` 内容相等时，调转到标签 `L1`。第二个是
```c
bne rs1, rs2, L1
```
两个寄存器中的内存不相等的时候，跳转到 `L1`。传统上，这两个指令被称为条件分支（`conditional branch`）。

下面看一个完整的例子。
```c
// 假定 f, g, h, i, j 这些变量分别在 x19 到 x23 这些寄存器
if (i == j) f = g + h; else f = g - h;

// 首先需要检测 i 和 j 是否相等（beq）。不过一般情况，测试不相等然后跳转到其他位置，下一行紧跟着 if 内的语句更高效
bne x22, x23, Else      // go to Else if i != j

// 下一个指令是 if 内的语句
add x19, x20, x21       // f = g + h (skipped if i != j)

// 现在我们要到 if-else 块的结束位置，这里要只用无条件分支（`unconditional branch`）做这个事情，意思是总是跳转。在 RISC-V 中可以用永远为真的条件分支来实现无条件分支。
beq x0, x0, Exit        // if 0 == 0, go to Exit

// 现在处理 else 部分。我们需要在执行前面加上 Else 标签，同时，最后面需要加上 Exit 表示整个 if-else 代码块的结束。
Else: sub x19, x20, x21     // f = g - h (skipped if i == j)
Exit:
```
汇编器使得汇编语言不必总是无聊的计算分支的地址。12 小节会解释这一点。

编译器会经常性的创建分支和标签，但是高级语言却无需写这些，从这个层次上说，高级编程语言编写的更快。

### 循环
编程语言中常见的循环也可以使用分支跳转实现。

```c
// 假定 i 和 k 在寄存器 x22 和 x25 中，save 的基地址在 x25 中存放
while (save[i] == k)
    i += 1;

// 第一步，加载 save[i] 到寄存器中。首选需要获得 i 对应位置的地址：将 i 和 save 基地址相加即可。不过在此之前，需要将 i 乘以 4 得到字节数，这一点可以用上一节中的逻辑运算完成。指令前面加一个 Loop 标签使得我们可以回到循环的开始。
Loop: slli x10, x22, 2      // Temp reg x10 = i * 4
add x10, x10, x25           // x10 = address of save[i]
lw x9, 0(x10)               // Temp reg x9 = save[i]

// 下一步进行比较。和之前类似，我们使用否定的比较指令
bne x9, x24, Exit           // go to Exit if save[i] != k

// 接下来是 while 循环体中的内存，将 i 加一，然后回到循环开始
addi x22, x22, 1            // i = i + 1
beq x0, x0, Loop            // go to Loop

// 最后添加 Exit 标签表示循环结束
Exit:
```

像上面 `while` 循环内的无分支的顺序执行的代码我们称为基本块（`basic block`）。除了在开头有标签，末尾有跳转外，中间没有任何标签和跳转。编译器很早起的工作就是把代码切分成这样的基本块。

等于和不等于的比较是最常见的比较，不过两个数还有很多其他关系。完整的比较集合是小于、小于等于、大于、大于等于、等于、不等于。

位模式的比较需要考虑符号位。对于无符号数来说，最高为是 1 比最高位是 0 的数字大；但是对于有符号数来说，最高位是 1 表示符号，比所有的符号位是 0 的正数要小。除了等于不等于以外，RISC-V 提供了 `blt` 和 `bge` 来比较两个有符号数的关系是否是小于和大于等于，对于无符号数，对应的指令是 `bltu` 和 `bgeu`。

一个替代方案是将比较结果写到一个临时寄存器中，然后 `beq` `bne` 根据这个寄存器中的内容跳转。MIPS 就是使用这种方法，这种方法使得处理器数据路径简单一点，但是需要更多的指令。

ARM 指令使用另外一种替代方案。使用额外的位来存执行指令时发生的一些事情，比如算术结果为负、为零或者溢出，这些特殊的位称为条件码或条件标记（` condition codes or flags`）。条件分支指令根据这些条件标记来测试以达到跳转的目的。

条件分支过多会影响流水线执行，第四章会解释分析。

### Bounds Check Shortcut
将有符号数比较可以看做是无符号数的比较，可以效率更高地检查是否有 $0<x<y$，常用于检查是否在数组索引范围内。二进制补码的负数当做无符号数的话是一个很大的数。因此，无符号方式比较 $x<y$ 就等价于 $x$ 是否是符号且满足 $0<x<y$。

```c
bgeu x20, x11, IndexOutOfBounds     // if x20 >= x11 or x20 < 0, goto IndexOutOfBounds
```

### Case/Switch Statement
编程语言往往支持 `switch-case` 语法。一个简单的方式是把它们转化成一系列 `if-else` 语句。

更高效的方式是编译成一系列顺序指令的地址表，即分支地址表或分支表（`branch address table`, `branch table`），程序只需要选择某个索引然后执行对应的顺序指令即可。分值表是包含对应标签的地址数组。程序将合适的项加载到寄存器中，然后根据寄存器内容进行跳转。为了支持这一点，像 RISC-V 的计算机包含了间接跳转（`indirect jump`）指令，无条件跳转到寄存器指向的地址。下一节会讲解 `jalr` 指令。
