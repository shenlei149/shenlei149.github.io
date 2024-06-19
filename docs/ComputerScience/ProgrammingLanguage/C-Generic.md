在C语言中，没有提供任何泛型能力，但是，有神奇 `void*`，只要运用恰当，能写出通用的『泛型』函数。在这里，『泛型』打了引号，表示并非真的是 C# 这种支持泛型语言中的含义，而是表示一种通用的意思。

这篇文章以函数和结构体及其方法为例，实现了两个函数 `swap` 和 `lsearch`，以此说明如何利用 `void*` 写出通用的函数，以及一个通用的 `Stack` 来说明在 C 语言里面也能写出通用的数据结构。

首先来看第一个例子，`swap` 函数不难写，很多人在刚开始学习 C 语言时候就写过。比如想要交换两个 `int`，可以写一个函数，函数声明大致如下：
```c
void swap(int*, int*)
```

现在又想交换两个 `double`，交换两个自定义的结构体，怎么办呢？再写两个类似的函数，显然是不合适的。

我们可以写一个通用的函数来做这个事情。

我们传入两个要交换对象的地址。对于 `int` 版本的 `swap` 函数，编译器知道拿 4 个字节做交换，`double` 版本的 `swap` 函数是 8 个字节，但是 `void*` 不包含类似的信息，所以我们要传入一个 `int` 来表示要交换对象的大小。下面是通用版的 `swap` 函数
```c
void swap(void *p1, void *p2, int size)
{
    char buffer[size];
    memcpy(buffer, p1, size);
    memcpy(p1, p2, size);
    memcpy(p2, buffer, size);
}
```
我们先开辟一个大小和对象大小一致的空间用来缓存数据。然后就是经典的三步：把 `p1` 放到 `buffer` 里面，`p2` 放到 `p1` 里面，最后把 `buffer` 里面的放到 `p2` 里面。

这里需要注意一下，函数第一句需要编译器的支持，幸好，现代编译器基本都支持了。

可以写几行代码简单的测试一下：
```cpp
int x = 17;
int y = 27;
swap(&x, &y, sizeof(int));
std::cout << x << "\t" << y <<std::endl;
```

下面来写个线性搜索函数。

搜索，要有关键字，所以我们要传入待比较对象的地址，还要传入一个数组和它的大小，和前面说的一样，`void*` 几乎不包含什么信息，所以要传入待搜索对象的大小，最后，要能比较两个对象，需要传入一个比较函数。对于线性搜索来说，可以不传，因为用 `memcmp` 比较就能知道两个对象是否一样，但是对于二分搜索就不行了。虽然只是一个简单的示例，还是要充分的考虑通用性的。另外还有一种可能性，就是对于一个自定义结构体，内存可能不是完全一样的，但是针对这个类型而言，含义是一样的，这样也需要一个自定义的比较函数。
```c
void* lsearch(void *key, void *base, int n, int eleSize, int(*cmp)(void*, void*))
{
    for (int i = 0; i < n; ++i)
    {
        void *eleAddr = (char*)base + i * eleSize;
        if (cmp(key, eleAddr) == 0)
        {
            return eleAddr;
        }
    }

    return NULL;
}
```
首先遍历数据，每一个对象都要和 `key` 进行比较。`void*` 是不能做指针的加减运算的，我们转成 `char*`，然后根据当前遍历的 `i` 和对象大小，向后移动若干个字节，获得待比较对象的地址。然后就是比较，调用传入的比较函数比较，如果一致，就返回该地址。最后，遍历完成还没有找到则返回 `NULL`。

这次写一个稍微复杂的代码来测试：找相同的字符串。我们需要一个比较函数：
``` cpp
int strCmp(void *p1, void *p2)
{
    char *s1 = *(char**)p1;
    char *s2 = *(char**)p2;

    return strcmp(s1, s2);
}
```
需要注意一下，字符串本来就是 `char*`，对于搜索函数而言，要比较的就是 `char*`，`lsearch` 需要的对象的地址，那么传入的就是 `char*` 的地址，也就是说，`key` 的类型是 `char**`，`base` 数组里面是 `char*`。所以我们的比较函数的 `p1` `p2` 是 `char**`，先强转再解引用。

如果写成 `char *s1 = (char*)p1;`，那么 `s1` 的含义就和预期的不一致，把 `p1` 当做 `char*`，解析地址，跳转到对应的地方，鬼知道那里是什么，很可能压根你就没权访问，假定能访问，从那里开始一直找到 `\0` 结束，没人知道 `s1` 是个什么鬼字符串。

简单地说，这里需要理解指针，要理解跳一次和跳两次的区别。

```cpp
char *notes[] = {"Ab", "F#", "B", "Gb", "D"};
char *favoriteNote = "Eb";
char **found = (char**)lsearch(&favoriteNote, notes, 5, sizeof(char*), strCmp);
std::cout << (found == NULL) << std::endl;
favoriteNote = "Gb";
found = (char**)lsearch(&favoriteNote, notes, 5, sizeof(char*), strCmp);
std::cout << *found << std::endl;
```
从测试代码就更容易理解 `char**` 了，倒数第二参数是 `char*` 的大小，说明要比较的对象类型是 `char*`。如前所述，第一个参数是对象的地址，`favoriteNote` 是 `char*`，我们要比较它（`char*`），所以再取地址传进去。

下面实现一个通用的 `Stack` 结构体及其方法。这里还会讨论为什么会内存泄漏以及如何修复这个问题。

首先，我们定义头文件。
```c
struct Stack {
    void *elements;
    int eleSize;
    int logicalLength;
    int allocLength;
};

void StackNew(Stack *s, int eleSize);

void StackDispose(Stack *s);

void StackPush(Stack *s, void *eleAddr);

void StackPop(Stack *s, void *eleAddr);
```
我们需要用 `void*` 来保存内容；由于 `void*` 不包含其他信息，所以需要 `eleSize` 来指定要进出栈的对象的大小；内部两个量，用于记录有多少个元素在栈里面，分配了多少空间来存放这些对象。

接下来定义的几个函数很容易理解，新初始化一个栈，销毁处理栈，进栈、出栈操作，`eleAddr` 有着不同的含义，进栈操作指的是待进栈对象的地址，出栈操作指的是会把待出栈对象拷贝到的地址。

下面我们来实现 `New` 这个操作，初始化各个变量，为 `elements` 分配默认大小的空间。
```c
void StackNew(Stack *s, int eleSize)
{
    assert(s->eleSize > 0);
    s->eleSize = eleSize;
    s->logicalLength = 0;
    s->allocLength = 4;
    s->elements = malloc(s->allocLength * eleSize);
    assert(s->elements);
}
```
`Dispose` 这个函数很简单，释放 `elements` 占用的内存。
``` cpp
void StackDispose(Stack *s)
{
    free(s->elements);
}
```
`Push` 函数稍微复杂一点，把要进栈的对象拷贝到栈空间里面，同时，要考虑到分配的空间可能已经不够大了，要动态增加。
``` cpp
void StackPush(Stack *s, void *eleAddr)
{
    if (s->logicalLength == s->allocLength)
    {
        StackGrow(s);
    }

    void *target = (char*)s->elements + s->eleSize * s->logicalLength;
    memcpy(target, eleAddr, s->eleSize);
    s->logicalLength++;
}
```
其中 `Grow` 函数如下：
``` cpp
static void StackGrow(Stack *s)
{
    s->allocLength *= 2;
    s->elements = realloc(s->elements, s->allocLength * s->eleSize);
    assert(s->elements);
}
```
这里，使用了 `realloc` 函数重新分配内存，它的基本实现是如果当前内存后面有足够的空间，直接扩大内存，修改这块内存前的四字节或者八字节的标记块，返回与传入指针相同的地址；如果不够，开辟另一块内存，拷贝内容过去，释放前一块内存，返回新内存地址。

最后是 `Pop` 函数，要先判断栈里面是不是有东西，如果有，把最上面的对象拷贝到给定的地址去。
``` cpp
void StackPop(Stack *s, void *eleAddr)
{
    assert(s->logicalLength > 0);
    s->logicalLength--;
    void *source = (char*)s->elements + s->eleSize * s->logicalLength;
    memcpy(eleAddr, source, s->eleSize);
}
```
目前为止，这个 `Stack` 实现像模像样，我们写个简单的函数来测试一下：
``` cpp
Stack s;
StackNew(&s, 4);

int x = 12;
int y = 1;
int z = 15;

StackPush(&s, &x);
StackPush(&s, &z);

int output;

StackPop(&s, &output);
std::cout << output << std::endl;

StackPush(&s, &y);

StackPop(&s, &output);
std::cout << output << std::endl;
StackPop(&s, &output);
std::cout << output << std::endl;

StackDispose(&s);
```
输出是 15 1 12，符合预期。

但是这有一个严重的问题，比如传入的是 `char*`，指针对应的地址是在 `Stack` 外 `malloc` 的内存，绝大多数情况，当时指向的指针早就过了生命周期或者指向了其他地方，那么，栈里面这个指针副本是唯一指向当初被分配的那块内存的指针。如果外部没有把所有指针对象都 `Pop` 出来并且释放（外部调用程序往往也没有这个义务做这些事情），那么，内存泄露就此产生了。所以，我们 `Stack` 实现要负责这个事情。

我们给 `Stack` 增加一个字段，同时修改以下 `New` 接口，让外界传入释放内存的函数，然后保存起来。
```c
void (*freeFn)(void*);

void StackNew(Stack *s, int eleSize, void (*freeFn)(void*));
```
重新实现 `Dispose` 函数，如果 `Stack` 里面还有对象，且 `freeFn` 不是 `NULL` 的话，释放内存，以防内存泄漏。
```c
void StackDispose(Stack *s)
{
    if (s->freeFn != NULL)
    {
        for (int i = 0; i < s->logicalLength; ++i)
        {
            s->freeFn((char*)s->elements + i * s->eleSize);
        }
    }

    free(s->elements);
}
```
