原子操作（`atomic operation`）是不可再分的操作。任何线程都不会看到执行到一半的结果，要么没有执行，要么执行完了。如果读操作对象是原子的（`atomic`），那么所有的写也是原子的，所以读到的结果要么是初始值，要么是某次写操作的结果。

反面就是非原子操作会被某个线程看到执行了一半的结果。如果一个非原子操作包含原子操作，比如更新一个`struct`，其中包含`atomic`成员，也包含非原子成员，那么其他线程会看到原子成员更新要么是完成的，要么没有开始，但是其他成员就可能被看到更新到一半的结果。在这个层面上讲，这会导致数据竞争和未定义行为。

### The standard atomic types
`<atomic>`中定义了标准的原子类型。虽然可以使用互斥使得读写看起来是原子操作，但是当且仅当在这些类型上的操作是原子的这件事是被标准化定义的。绝大部分原子类型都有一个成员函数`is_lock_free()`告诉用户操作是一条指令实现的（`true`）还是内部使用了互斥（`false`）。

原子操作的一个关键用途是作为某个操作的替代或者是使用互斥来确保同步。如果原子操作内部使用了互斥，为了性能可能没有物化，那么最好用一个基于互斥的实现而不是这些原子操作。第七章会讨论这些。

有一系列宏来在编译期检查各种整数类型是否是`lock-free`的。C++17开始，每一个类型有一个`static constexpr`的变量`is_always_lock_free`表示在当前编译器支持的所有平台上都是`lock-free`的。比如`std::atomic<int>`总是`lock-free`的，那么`std::atomic<int>::is_always_lock_free`就是`true`，`std::atomic<uintmax_t>`可能需要某些平台的特定支持才能是`lock-free`的，那么`std::atomic<uintmax_t>::is_always_lock_free`就是`false`。

这些宏是 `ATOMIC_BOOL_LOCK_FREE`, `ATOMIC_CHAR_LOCK_FREE`, `ATOMIC_CHAR16_T_LOCK_FREE`, `ATOMIC_CHAR32_T_LOCK_FREE`, `ATOMIC_WCHAR_T_LOCK_FREE`, `ATOMIC_SHORT_LOCK_FREE`, `ATOMIC_INT_LOCK_FREE`, `ATOMIC_LONG_LOCK_FREE`, `ATOMIC_LLONG_LOCK_FREE`, `ATOMIC_POINTER_LOCK_FREE`。它们指定了整数类型及相应`unsigned`类型是否是`lock-free`的。0表示永远不是，2表示总是`lock-free`的，1的话需要用上述运行时属性来判断。

唯一没有提供`is_lock_free()`的类型是`std::atomic_flag`。这是一个简单的布尔类型，必须是`lock-free`的。有了这个类型，可能简单地实现锁和其他原子类型。这里的简单指的是`std::atomic_flag`只能检查并设置值（`test_and_set()`）或者清除标记（`clear()`），没有赋值，没有复制构造，没有检查并清除的操作等等。

其余的原子类型有更全面的功能，不过可能不是`lock-free`的。在流行的平台上，内置类型基本都是`lock-free`的，比如`std::atomic<int>`和`std::atomic<void*>`，不过不是必须的。每一个特化版本都反映了类型本身的属性，比如指针不支持`&=`操作，那么原子指针也不支持。

除了直接使用`std::atomic<>`之外，还可以使用下表的类型名。由于历史原因，最好不要在一个程序里面混用以影响可以执行。

| Atomic type | Corresponding specialization |
|--|--|
| `atomic_bool` | `std::atomic<bool>` |
| `atomic_char` | `std::atomic<char>` |
| `atomic_schar` | `std::atomic<signed char>` |
| `atomic_uchar` | `std::atomic<unsigned char>` |
| `atomic_int` | `std::atomic<int>` |
| `atomic_uint` | `std::atomic<unsigned>` |
| `atomic_short` | `std::atomic<short>` |
| `atomic_ushort` | `std::atomic<unsigned short>` |
| `atomic_long` | `std::atomic<long>` |
| `atomic_ulong` | `std::atomic<unsigned long>` |
| `atomic_llong` | `std::atomic<long long>` |
| `atomic_ullong` | `std::atomic<unsigned long long>` |
| `atomic_char16_t` | `std::atomic<char16_t>` |
| `atomic_char32_t` | `std::atomic<char32_t>` |
| `atomic_wchar_t` | `std::atomic<wchar_t>` |

C++标准还提供一系列`typedef`的原子类型，和非原子类型相对应，见下表。

| Atomic `typedef` | Corresponding Standard Library `typedef` |
|--|--|
| `atomic_int_least8_t` | `int_least8_t` |
| `atomic_uint_least8_t` | `uint_least8_t` |
| `atomic_int_least16_t` | `int_least16_t` |
| `atomic_uint_least16_t` | `uint_least16_t` |
| `atomic_int_least32_t` | `int_least32_t` |
| `atomic_uint_least32_t` | `uint_least32_t` |
| `atomic_int_least64_t` | `int_least64_t` |
| `atomic_uint_least64_t` | `uint_least64_t` |
| `atomic_int_fast8_t` | `int_fast8_t` |
| `atomic_uint_fast8_t` | `uint_fast8_t` |
| `atomic_int_fast16_t` | `int_fast16_t` |
| `atomic_uint_fast16_t` | `uint_fast16_t` |
| `atomic_int_fast32_t` | `int_fast32_t` |
| `atomic_uint_fast32_t` | `uint_fast32_t` |
| `atomic_int_fast64_t` | `int_fast64_t` |
| `atomic_uint_fast64_t` | `uint_fast64_t` |
| `atomic_intptr_t` | `intptr_t` |
| `atomic_uintptr_t` | `uintptr_t` |
| `atomic_size_t` | `size_t` |
| `atomic_ptrdiff_t` | `ptrdiff_t` |
| `atomic_intmax_t` | `intmax_t` |
| `atomic_uintmax_t` | `uintmax_t` |

原子类型没有复制构造函数和赋值操作符，所有不能被复制或者赋值。不过可以使用成员函数`load()` `store()` 或函数 `exchange()` `compare_exchange_weak()` `compare_exchange_strong()` 达到使用内置类型赋值或者是转成内置类型的目的。也支持复合赋值运算符，比如`+=, -=, *=, |=`等，与之对应也有命名函数，比如`fetch_add()` `fetch_or()`等。这些操作符和函数的返回值存储的值，或者是操作之前存储的值。这避免了非原子类型存在的问题。赋值操作符返回对象的引用，那么为了获取引用的值，有一个分离的读操作，那么在读和赋值之间其他线程就可能修改数据。

`std::atomic<>`为自定义类型实现一个原子变种提供了模板，其操作被限定在 `load()` `store()` `exchange()` `compare_exchange_weak()` `compare_exchange_strong()`。

每个原子操作还有一个可选参数，就是内存顺序，可以是`std::memory_order`枚举的某个值：`std::memory_order_relaxed`, `std::memory_order_acquire`, `std::memory_order_consume`, `std::memory_order_acq_rel`, `std::memory_order_release`, `std::memory_order_seq_cst`。

每个操作允许传入的枚举值取决于其操作类别。默认情况下是` std::memory_order_seq_cst`，这个顺序要求是最强的。下个小节会详细解释这些内存顺序的语义。现在只需要明白可以把它们归为三类即可。

- 写 `store` 操作：`memory_order_relaxed`, `memory_order_release`, `memory_order_seq_cst`
- 读 `load` 操作：`memory_order_relaxed`, `memory_order_consume`,
`memory_order_acquire`, `memory_order_seq_cst`
- 读修改写 `read-modify-write` 操作：全部六种。

### Operations on std::atomic_flag
`std::atomic_flag`是最简单的标准原子类型，表示布尔标记。只有两种状态：被设置或者被清理。这么简单的目的是作为其他类型的组成部分。实际工作中可能没有用，但是这里讨论的目的是有一些一般性的原则适用于所有原子类型。

`std::atomic_flag`必须使用`ATOMIC_FLAG_INIT`初始化，是被清理的状态。这一点是没有选择余地的。
```cpp
std::atomic_flag f = ATOMIC_FLAG_INIT;
```
这是作用域无关。只有这一个类型这么特殊，也只有这个类型能保证`lock-free`。如果`std::atomic_flag`是静态变量，那么在第一个操作之前一定会被初始化，所有没有初始化顺序的问题。

一旦初始化了对象，只有三件事可以作：销毁，清除，设置并返回之前的值，分别对应析构函数，`clear()`，`test_and_set()`。后面两个成员还是可以指定内存顺序。`clear()`是`store`操作，`test_and_set()`是读修改写操作。
```cpp
f.clear(std::memory_order_release);
bool x = f.test_and_set();
```
`std::atomic_flag`不能复制构造也不能赋值，这是原子类型共性。原子类型的所有操作必须是原子的，但是这两个操作涉及两个对象，首先从一个对象读，然后写到另一个对象去。这两个在不同对象上的操作是无法原子化的。因此这俩操作是不允许的。

`std::atomic_flag`有限的操作使得它适合实现一个自旋互斥锁。初始值是清除状态，表示没有上锁，循环调用`test_and_set()`直到返回值是`false`，说明是当前线程将值设置成了`true`，加锁成功。解锁就是简单的`clear()`。
```cpp
class spinlock_mutex
{
    std::atomic_flag flag;

public:
    spinlock_mutex() : flag(ATOMIC_FLAG_INIT)
    {
    }

    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};
```
实现很简单，不过足够和`std::lock_guard<>`配合使用。忙等可能不是最优解，但是能确保实现了互斥功能。

`std::atomic_flag`太简单了，连单纯的非修改查询功能都没有，也就无法胜任一般的布尔类型的工作。

### Operations on std::atomic<bool>
相比`std::atomic_flag`，`std::atomic<bool>`的功能更完善。尽管不能复制构造和赋值运算符，但是可以用非原子的`bool`类型构造，所以初始化成`true`或`false`，并且可以用非原子类型赋值
```cpp
std::atomic<bool> b(true);
b = false;
```
非原子`bool`的赋值运算符返回对其分配给的对象的引用，这是非原子类型的约定，不过原子类型`bool`返回一个`bool`类型的值。这是原子类型的另一种常见模式：赋值运算符返回值而不是引用。如果返回对原子变量的引用，那么任何依赖于赋值结果的代码都必须显式加载这个值，可能会得到另一个线程修改的结果。通过将赋值的结果作为非原子值返回，可以避免这种额外的加载，并且知道获得的值就是存储的值。

通过`store()`写数据，`exchange()`可以写数据且得到原始数据，通过`load()`（也可以隐式地）得到普通`bool`类型。这三个操作分别是`store` `read-modify-write` `load`操作。
```cpp
std::atomic<bool> b;
bool x = b.load(std::memory_order_acquire);
b.store(true);
x = b.exchange(false, std::memory_order_acq_rel);
```

#### STORING A NEW VALUE (OR NOT) DEPENDING ON THE CURRENT VALUE
`std::atomic<bool>`提供了新的操作比较交换`compare-exchange`，有两个成员函数：`compare_exchange_weak()` `compare_exchange_strong(T& expected, T desired)`。比较交换操作是原子类型编程的基石。它将原子变量的值与提供的期望值进行比较，如果它们相等，则存储提供的想要的新值。如果值不相等，则使用原子变量的值更新期望值。比较交换函数的返回类型是`bool`，如果执行了存储则为`true`，否则为`false`。如果存储完成（因为值相等），则称操作成功，否则失败；那么返回值为`true`表示成功，`false`表示失败。

`compare_exchange_weak()`可能会出现存储的值和期望值一样但是没有更新而返回`false`的情况。如果处理器没有提供`compare-and-exchange`的指令，就有可能出现这种情况。可能是因为执行到了一半操作系统切换了线程。这称为意外失败`spurious failure`，因为导致失败的原因不是变量的值而是执行函数的时机。

因此，需要使用要给循环
```cpp
bool expected = false;
extern atomic<bool> b; // set somewhere else
while (!b.compare_exchange_weak(expected, true) && !expected)
    ;
```
如果`expected`是`false`，说明遇到了意外失败，那么就接着循环。

`compare_exchange_strong()`就保证了返回`false`一定是值和`expected`不一致导致的，也就无需循环。

如果不管初始值是什么，你都想改变变量的值（可能有一个依赖于当前值的更新值），`expected`参数被更新就变得有用了。每次循环时，都会重新加载`expected`，因此如果在此期间没有其他线程修改该值，则`compare_exchange_weak()`或`compare_exchange_strong()`在下一次循环中应该会成功，因为你知道了原值是什么。如果要存储的值的计算很简单，那么使用`compare_exchange_weak()`可能会有所帮助，可以避免在`compare_exchange_weak()`可能会意外失败的平台上出现双循环（因此`compare_exchange_strong()`需要一个循环）。另一方面，如果要存储的值的计算非常耗时，则使用`compare_exchange_strong()`避免在预期值未更改时必须重新计算要存储的值。对于`std::atomic<bool>`无所谓——毕竟只有两个可能的值——但对于更大的原子类型，这可能会有所不同。

比较交换函数接受两个内存顺序参数。这使得在成功或者失败时，有不同的内存顺序语义，比如成功时是`memory_order_acq_rel`语义，而失败时是`memory_order_relaxed`语义。失败时不是`store`操作，所以不能是`memory_order_release`和`memory_order_acq_rel`。同时，不能令失败时的内存顺序比成功时的内存顺序更严格。如果失败时指定了`memory_order_acquire`和`memory_order_seq_cst`，那么成功时也必须一样。

如果没有指定失败的内存顺序，那么会和成功时的保持一致，除了`release`语义会被移出。`memory_order_release`变成`memory_order_relaxed`，`memory_order_acq_rel`变成`memory_order_acquire`。如果都没有提供，那么默认值是`memory_order_seq_cst`，保证完全是顺序的。下面两种调用方式是等价的。
```cpp
std::atomic<bool> b;
bool expected;
b.compare_exchange_weak(expected, true,
                        memory_order_acq_rel, memory_order_acquire);
b.compare_exchange_weak(expected, true, memory_order_acq_rel);
```
`std::atomic<bool>`和`std::atomic_flag`还有一个不同是，前者不一定是`lock-free`的。如果这一点很重要，那么调用`is_lock_free()`检查一下。这一点对其他原子类型也适用。

### Operations on std::atomic<T*>: pointer arithmetic
和`std::atomic<bool>`与`bool`的关系一样，且接口和语义也都一样。不能复制构造，不能赋值，但是可以用从非原子类型构造和赋值。有`is_lock_free()`,`load()`,`store()`,`exchange()`,`compare_exchange_weak()`,`compare_exchange_strong()`函数，只是参数和返回类型不同罢了。

新提供了指针运算操作符。成员函数`fetch_add()`和`fetch_sub()`能够原子地移动指针位置，同时还提供了一些方便的操作符：`+=` `-=` `--` `++`。这些和裸指针的运算一样。两个成员函数稍有不同，返回的是原始地址。这两函数称为`exchange-and-add`操作，也属于读修改写操作。这些操作符返回的类型是裸指针`T*`而不是原子指针`std::atomic<T*>`对象的引用。
```cpp
class Foo
{
};

Foo some_array[5];
std::atomic<Foo *> p(some_array);
Foo *x = p.fetch_add(2);
assert(x == some_array);
assert(p.load() == &some_array[2]);
x = (p -= 1);
assert(x == &some_array[1]);
assert(p.load() == &some_array[1]);
```
成员函数可以额外指定内存顺序。比如
```cpp
p.fetch_add(3, std::memory_order_release);
```
`fetch_add()`和`fetch_sub()`都是读修改写的操作，所以任意顺序都可以，同时参与`release sequence`。操作符就不行了，没有地方提供这些信息，所以都是`memory_order_seq_cst`。

###  Operations on standard atomic integral types
首先，`load()`,`store()`,`exchange()`,`compare_exchange_weak()`,`compare_exchange_strong()`这些函数都是有的。其次，原子整数类型还支持`fetch_add()`,`fetch_sub()`,`fetch_and()`,`fetch_or()`,`fetch_xor()`,`+=`,`-=`,`&=`,`|=`,`^=`,`++x`,`x++`,`--x`,`x--`。除了没有乘除和移位操作，其他操作都是全的。一般这些类型往往用于计数器或者是`bitmask`，没有这些运算影响不大。需要的话`compare_exchange_weak()`可以容易的实现一个。

和`std::atomic<T*>`语义近乎一样。成员函数返回旧值，复合赋值运算符返回新值。`++x`和`x++`也和非原子类型一样，前者返回新值，后者返回旧值。

### The std::atomic<> primary class template
这个模板可以帮助我们实现自定义的原子类型。给定一个自定义类型`UDT`，`std::atomic<UDT>`有和`std::atomic<bool>`一样的接口，除了参数类型和返回类型。不是任意类型都能原子化，需要满足一定的条件。不能有非普通的复制赋值操作符。不能有`virtual`函数，也不能有`virtual`基类，必须用编译器生成的赋值运算符。除此之外，非`static`成员变量也必须满足这个条件。由于不需要运行用户自定义的代码，这意味着编译器可以实用`memcpy()`或等价操作进行赋值。

比较交换操作使用类似`memcmp`的二进制比较，而不是自定义的比较运算。如果自定义比较有不同语义，或者类中有填充的字节不参与比较，那么即使两个对象一样，比较交换操作也可能会失败。

背后的原因是第三章提高的指导原则：不要将被保护数据的指针或者引用作为参数传递给用户写的函数使之超出锁的保护范围。一般情况下，编译器无法为`std::atomic<UDT>`生成`lock-free`的代码，那么内部会有一个锁，如果允许用户自定义比较和赋值操作，那么向这些函数传递引用，违反了这个原则。类库有只有一个锁保护所有原子操作的自由，持有锁的时候调用用户自定义的函数，可能会死锁，或者由于比较操作费时而导致其他线程被阻塞。最后，这些限制帮助编译器能够为`std::atomic<UDT>`生成原子指令，毕竟可以把对象当作一系列二进制。

` std::atomic<float>`和`std::atomic<double>`是内置浮点类型，满足`memcpy`和`memcmp`的条件，不过使用`compare_exchange_strong`仍旧可能会有意外。如果存储的值有不同的表示，即使值是一样的，也有可能失败。要记得，对于浮点数没有原子算术操作。自定义的类型也可能有这种情况，`memcmp`和自定义的比较意义不同，相等的两个对象可能有不同的表示。

如果`UDT`大小和`int`或`void*`一样大或者更小，那么大部分平台会使用原子指令。如果是两倍大小，部分平台也有原子指令，对应`compare_exchange_xxx`的指令称为`double-word-compare-and-swap`。第七章会讲解这对编写`lock-free`代码很有用。

这些限制意味着你不能创建`std::atomic<std::vector<int>>`类型，不过可以用一些包括计数器、标记位、指针和简单数组的类实例化`std::atomic<>`。一般而言，越复杂的数据结构，往往无法使用简单的比较和赋值。这种情况，使用`mutex`来保护想要的操作。

| Operation | `atomic_flag` | `atomic<bool>` | `atomic<T*>` | `atomic<integral-type>` |` atomic<othertype>` |
|--|--|--|--|--|--|
| `test_and_set` | Y | | | | |
| `clear` | Y | | | | |
| `is_lock_free` | | Y | Y | Y | Y |
| `load` | | Y | Y | Y | Y |
| `store` | | Y | Y | Y | Y |
| `exchange` | | Y | Y | Y | Y |
| `compare_exchange_weak`, `compare_exchange_strong` | | Y | Y | Y | Y |
| `fetch_add`, `+=` | | | Y | Y | |
| `fetch_sub`, `-=` | | | Y | Y | |
| `fetch_or`, `\|=` | | | | Y | |
| `fetch_and`, `&=` | | | | Y | |
| `fetch_xor`, `^=` | | | | Y | |
| `++,` `--` | | | Y | Y | |

### Free functions for atomic operations
之前介绍的都是成员函数，现在介绍非成员函数。这些函数以`atomic_`为前缀，这些函数对每一个原子类型都有重载。同时，有另外一组变种，以`_explicit`为后缀，可以传入内存顺序。比如`std::atomic_store(&atomic_var, new_value)`和`std::atomic_store_explicit(&atomic_var, new_value, std::memory_order_release)`。作为成员函数，隐式地接受原子类型对象的引用，这里第一个参数是指针类型。

`std::atomic_is_lock_free(&a)`等价于`a.is_lock_free()`，`std::atomic_load(&a)`等价于`a.load()`，`a.load(std::memory_order_acquire)`等价于`std::atomic_load_explicit(&a, std::memory_order_acquire)`。

非成员函数是为了和 C 兼容，所以使用指针而不是引用。`std::atomic_compare_exchange_weak_explicit()`需要显式地传入成功和失败的内存顺序，原因是没有默认参数。

`std::atomic_flag`打破了这种趋势，保留了`flag`部分，`std::atomic_flag_test_and_set()`和`std::atomic_flag_clear()`，需要内存顺序参数，用`std::atomic_flag_test_and_set_explicit()`和`std::atomic_flag_clear_explicit()`。

C++标准库提供了以原子方式访问`std::shared_ptr<>`实例的接口。但是严格说来，`std::shared_ptr<>`不是原子类型。不过标准委员会觉得它们很重要。提供了`load`,`store`,`exchange`,`compare-exchange`，以`std::shared_ptr<>*`作为第一个参数。
```cpp
std::shared_ptr<my_data> p;
void process_global_data()
{
    std::shared_ptr<my_data> local = std::atomic_load(&p);
    process_data(local);
}

void update_global_data()
{
    std::shared_ptr<my_data> local(new my_data);
    std::atomic_store(&p, local);
}
```

Concurrency TS还提供了`std::experimental::atomic_shared_ptr<T>`，这是一个原子类型。和`std::atomic<UDT>`一样，有`load`,`store`,`exchange`,`compare-exchange`方法。单独的实现可能是`lock-free`的，那么相比`std::shared_ptr`没有额外开销。和模板类`std::atomic`一样，这依赖于平台，所以需要调用`is_lock_free`来检查。即使不是`lock-free`的，也推荐使用`std::experimental::atomic_shared_ptr`而不是在`std::shared_ptr`上使用这里介绍的函数，因为前者更清晰、简单，不会因为忘记用原子方法而出错（潜在的竞争）。如果使用原子类型和操作是为了性能，那么需要和其他同步机制做性能对比。
