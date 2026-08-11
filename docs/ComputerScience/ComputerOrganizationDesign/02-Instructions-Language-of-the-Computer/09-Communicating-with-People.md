最初，计算机只需要处理数字，但是很快，人们就发现计算机需要处理文本。最常用的表示文本的方式是 ASCII（`American Standard Code for Information Interchange (ASCII)`），占用 8 个比特。

RISC-V 提供了高效处理字节的指令。`lbu`（`load byte unsigned`）是从内存取一个字节，放到寄存器最右端的八个比特。`sb`（`store byte`）将一个寄存器中最右端的八个比特存到内存中。
```
lbu x12, 0(x10) // Read byte from source
sb x12, 0(x11)  // Write byte to destination
```

通常有三种方式表示字符串。1）以特殊的字符结尾，比如 C 语言中用 `\0` 结束；2）使用结构体，跟随字符串变量有一个长度变量；3）在字符串开头使用一定长度表示字符串长度，Java 就使用这种方式。

#### Compiling a String Copy Procedure, Showing How to Use C Strings
下面是 C 语言中经典的 `strcpy` 函数
```c
void strcpy(char x[], char y[])
{
    size_t i;
    i = 0;
    while ((x[i] = y[i]) != '\0') /* copy & test byte */
        i += 1;
}
```
RISC-V 的汇编代码是什么？

首先，我们假设 `x, y` 的基地址保存在寄存器 `x10, x11` 中，变量 `i` 使用 `x19`，那么需要保存 `x19` 的值。
```
strcpy:
    addi sp, sp, -4 // adjust stack for 1 more item
    sw x19, 0(sp)   // save x19
```
接着初始化 `i = 0`，通过 0 加 0 把结果放到 `x19` 中。
```
add x19, x0, x0 // i = 0+0
```
接着循环开始。先计算 `y[i]` 的地址，结果放到 `x5` 中。
```
L1: add x5, x19, x11 // address of y[i] in x5
```
这里并没有将 `i` 的值乘以 4，因为 `y` 就是存放字节的数组。

下面把 `y[i]` 的内容加载到 `x6`。
```
lbu x6, 0(x5) // x6 = y[i]
```
使用类似的方式计算 `x[i]` 的地址，结果放到 `x7`，然后把 `y[i]`，即 `x6` 的内容存到 `x7` 指向的内存上。
```
add x7, x19, x10 // address of x[i] in x7
sb x6, 0(x7)     // x[i] = y[i]
```
下面判断字符串是否结束了，如果结束了，准备退出函数。
```
beq x6, x0, L2
```
如果没有跳转，即字符串没有结束，`i++`，回到循环 `L1` 开始的地方。
```
addi x19, x19, 1 // i = i + 1
jal x0, L1       // go to L1
```
最后是循环退出的部分，恢复 `x19` 和栈指针，然后返回。
```
L2: lw x19, 0(sp)  // restore old x19
    addi sp, sp, 4 // pop 1 word off stack
    jalr x0, 0(x1) // return
```
函数 `strcpy` 是一个叶子过程，没有再调用其他过程，那么编译器可以使用临时寄存器保存 `i` 的值，那样就不再需要保存和恢复 `x19` 寄存器的值。

在 Java 中，使用 16 比特长度的 Unicode 来表示字符串。为此，RISC-V 提供了加载和存储 16 比特信息的指令。16 比特称为半字（`halfword`）。`lhu`（`load half unsigned`）从内存加载半字到寄存器的最右端 16 个比特，同时左边的比特位置零；`lh`（`load half`）类似，不过左边的比特位置符号位。`sh`（`store half`）取寄存器的最右端的 16 个比特信息写入内存。
```
lhu x19, 0(x10) // Read halfword (16 bits) from source
sh x19, 0(x11)  // Write halfword (16 bits) to dest
```
