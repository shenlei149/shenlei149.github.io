对 C 语言的新手程序员而言，理解指针略有难度。下面通过清除数组的例子对比使用数组和指针时汇编的差异，以加深对指针的理解。下面是两个版本的代码，不同之处在于使用数组还是指针来解决问题。
```c
void clear1(int array[], size_t size)
{
    size_t i;
    for (i = 0; i < size; i += 1)
        array[i] = 0;
}

void clear1(int *array, size_t size)
{
    int *p;
    for (p = &array[0]; p < &array[size]; p = p + 1)
        *p = 0;
}
```

### Array Version of Clear
这里，我们只关注函数体。假定参数 `array, size` 位于寄存器 `x10,x11`，变量 `i` 位于 `x5`。

首先初始化 `i`。
```
addi x5, x0, 0 // i = 0 (register x5 = 0)
```
为了将 `array[i]` 赋值为 0，我们需要通过 `i` 乘以 4 然后加上 `array` 的基地址获得地址。
```
loop1:
  slli x6, x5, 2    // x6 = i * 4
  add x7, x10, x6   // x7 = address of array[i]
```
然后将其赋值为 0。
```
sw x0, 0(x7) // array[i] = 0
```
接着是循环的最后一个部分，自增 `i`
```
addi x5, x5, 1 // i = i + 1
```
最后，判断是否需要回到循环开始。
```
blt x5, x11, loop1 // if (i < size) go to loop1
```
整个程序如下。这里假定 `size` 是大于 0 的，这里略过该检查。
```
  addi x5, x0, 0        // i = 0
loop1:
  slli x6, x5, 2        // x6 = i * 4
  add x7, x10, x6       // x7 = address of array[i]
  sw x0, 0(x7)          // array[i] = 0
  addi x5, x5, 1        // i = i + 1
  blt x5, x11, loop1    // if (i < size) go to loop1
```

### Pointer Version of Clear
和数组版本类似，假定参数 `array, size` 位于寄存器 `x10,x11`，指针 `p` 位于 `x5`。

首先，`p` 指向 `array` 首地址。
```
mv x5, x10 // p = address of array[0]
```
接着，将 `p` 指向的内容赋值成 0。
```
loop2:
  sw x0, 0(x5) // Memory[p] = 0
```
移动 `p`，类型是 `int*`，所以移动一个字（4B）的长度。
```
addi x5, x5, 4 // p = p + 4
```
循环最后是判断 `for` 条件是否成立。首先获得 `array` 尾部的地址。
```
slli x6, x11, 2     // x6 = size * 4
add x7, x10, x6     // x7 = address of array[size]
```
尾部地址与 `p` 的地址做比较。
```
bltu x5, x7, loop2  // if (p<&array[size]) go to loop2
```
整个代码如下所示。这里和上一个例子一样，假定 `size` 是大于 0 的。
```
  addi x5, x10, 0       // p = address of array[0]
loop2:
  sw x0, 0(x5)          // Memory[p] = 0
  addi x5, x5, 4        // p = p + 4
  slli x6, x11, 2       // x6 = size * 4
  add x7, x10, x6       // x7 = address of array[size]
  bltu x5, x7, loop2    // if (p<&array[size]) go to loop2
```
循环体内，我们每次都要获取 `array` 尾部的地址，但是这个值不会改变，所以我们可以放到循环外部提升性能。
```
  addi x5, x10, 0       // p = address of array[0]
  slli x6, x11, 2       // x6 = size * 4
  add x7, x10, x6       // x7 = address of array[size]
loop2:
  sw x0, 0(x5)          // Memory[p] = 0
  addi x5, x5, 4        // p = p + 4
  bltu x5, x7, loop2    // if (p < &array[size]) go to loop2
```

### Comparing the Two Versions of Clear
数组版本必须在循环体内自增 `i` 然后乘以 `4` 而指针版本可以直接比较 `p` 和 `array` 尾部地址。循环内部的指令从五个减少到了三个。

很久之前，人们被教导在 C 语言中使用指针可以获得比数组更高的效率：“即使你无法理解代码，也要使用指针。”现代编译器可以为数组版本生成同样好的代码。所以使用直观的写法最重要。身为程序员，更偏向于让编译器承担繁重的工作。
