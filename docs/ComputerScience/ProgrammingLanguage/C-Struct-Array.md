申明一个结构体：
``` cpp
struct fraction {
	int denom;
	int num;
};
```

下面是一些对 `fraction` 结构体的操作：
``` cpp
fraction f;
f.denom = 22;
f.num = 7;

((fraction*)&(f.num))->denom = 12;
std::cout << f.num << std::endl;
((fraction*)&(f.num))->num = 33;
std::cout << (&f)[1].denom << std::endl;
```

下面逐行解释下上面的程序。

首先我们声明了类型是 `fraction` 的变量 `f`，在栈上分配了两个 4 字节去存储，低位是 `denom`，高位是 `num`：

| mem |
| --- |
| num |
| denom |

接下来是两个赋值语句，内存变成了：

| mem |
| --- |
| 7 |
| 22 |

接下来是个复杂的赋值语句，`&(f.num)` 拿到了 `num` 所在内存的地址，`(fraction*)` 告诉编译器这个地址是上放的是 `fraction` 结构体，`->denom` 是修改这个虚拟结构体的 `denom` 字段。好了，内存变成了：

| mem |
| --- |
| 12 |
| 22 |

所以紧接着那句输出是 12。接下来又是个类似的复杂赋值语句，不再赘述，内存变成了：

| mem |
| --- |
| 33 |
| 12 |
| 22 |

使用原来的 `f` 能不能访问到 33 这个值呢？能，方法就是最后一个语句。`(&f)` 取到 `f` 的地址，`(&f)[1]`，编译器会向后偏移 `f` 指向的对象的大小，`fraction` 大小是八字节，所以这句就是往后偏移八个字节，于是乎指向 33 所在的内存。最后，拿出 denom 的值。

数组在内存中的布局比较简单，很直接。需要注意的是C、C++并不检查数据越界，访问外面的数据会破坏上下文的数据或者访问不能访问的内存，很危险。

总结下数组和指针操作的等价性：
``` cpp
arr ≡ &arr[0]
arr + k ≡ &arr[k]
*arr ≡ arr[0]
*(arr + k) ≡ *arr[k]
```
其中，`k` 可以为负数。

下面看个示例：
``` cpp
int arr[5];
((short*)(((char*)(&arr[1])) + 8))[2] = 100;
((short*)(((char*)(&arr[1])) + 8))[3] = 1;
std::cout << arr[4] << std::endl;
```
申明了五个 `int` 的数组，`&arr[1]` 拿到第二个元素的地址， `(char*)` 让编译器认为当前指针是 `char*` 类型，`+8` 操作就是往后偏移 8 个字节，这时，指针指向了 `arr[3]`。`(short*)` 让编译器认为当前指针是 `short*` 类型，`[2]` 和 `+2` 是等价的，移动 $2*2$ 个字节，现在指针指向的是 `arr[4]`，但是编译器会认为这里是个 `short` 并赋值为 100。所以 `arr[4]` 的内存就是 `0x64 0x00 0x00 0x00`（小端）。下面一句类似，`[3]` 就是移动 $2*3$ 个字节，指向了 `arr[4]` 的后半段，赋值 1，内存就是 `0x64 0x00 0x01 0x00`。输出语句就是把 `arr[4]` 里面所有的内容以 `int` 的方式输出 $1 * 2 ^ 16 + 100 = 65636$。

最后，看一个综合的示例：
``` cpp
struct student {
	char* name;
	char suid[8];
	int numUnits;
};

student pupils[2];
pupils[0].numUnits = 21;
pupils[1].name = pupils[0].suid + 6;
strcpy(pupils[0].suid, "40415xx");
std::cout << pupils[0].suid << std::endl;
strcpy(pupils[1].name, "123456");
std::cout << pupils[0].suid << std::endl;
std::cout << std::hex << pupils[0].numUnits << std::endl;
```
内存布局类似下面的形式：

| mem | 4 bytes | per | row |
| --- |-|-|-|
| numUnits |
| s | s | u | u |
| i | i | d | d |
| name |
| name |
| numUnits |
| s | s | u | u |
| i | i | d | d |
| name |
| name |

下面的是 `pupils[0]` 上面的是 `pupils[1]`。经过第一个输出之前的三个赋值语句后：

| mem | 4 bytes | per | row |
| --- |-|-|-|
| numUnits |
| s | s | u | u |
| i | i | d | d |
| name |
| name |
| 21 |
| 5 | x | x | \0 |
| 4 | 0 | 4 | 1 |
| name |
| name |

另外，`pupils[1].name` 指向的是倒数第四行第二个 `x` 所在的内存。

第一个输出语句很简单，`40415xx`，遇到 `\0` 就结束了。
下面看第二个 `strcpy`。
```cpp
strcpy(pupils[1].name, "123456");
```

这个语句的操作结果是：

| mem | 4 bytes | per | row |
| --- |-|-|-|
| numUnits |
| s | s | u | u |
| i | i | d | d |
| name |
| \0 | name |
| 3 | 4 | 5 | 6 |
| 5 | x | 1 | 2 |
| 4 | 0 | 4 | 1 |
| name |
| name |

这时，输出的 `pupils[0].suid` 是 `40415x123456`，遇到 `pupils[1]` 的 `\0` 才结束。`pupils[0].numUnits` 也已经被修改了，16 进制输出是 36353433，数字 6 的 ASCII 码对应的就是 0x36，其他类似。

