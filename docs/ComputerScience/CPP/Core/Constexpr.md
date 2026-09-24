## `constexpr`
`constexpr` 函数既可以在编译期求值，也可以在运行时执行。

在 C++20 中，`constexpr` 可以用于虚函数。双向的 `override` 都是可以的。基类是 `constexpr` 虚函数，子类可以非 `constexpr` 方式来 `override`，反之，基类是普通虚函数，子类可以使用 `constexpr` 来 `override`。如果基类是 `constexpr` 虚函数，子类实现的时候可能依赖了运行时的调用无法满足 `constexpr`，因而可以使用非 `constexpr` 的形式来 `override`。基类可能是历史遗留下来的类，是一个非 `constexpr` 虚函数，但是子类是新写的使用了 C++20 的标准，那么可以用 `constexpr` 来 `override`，这样指向子类的指针或引用可以享受到编译期求值的好处。

## `consteval` `constinit`
`consteval` 创建的是一个立即函数（`immediate function`）。比如：
```cpp
consteval int sqr(int n) { return n * n; }
```
立即函数的调用会创建一个编译期常量，也就是说 `consteval` 函数必须在编译期求值。因此它不能用于析构函数、执行内存分配和释放的函数。

在同一个声明中，最多只能使用 `consteval` `constexpr` `constinit` 中的一个。

`consteval` 隐式包含了 `inline` 的语义，并且必须满足 `constexpr` 函数的所有要求，因此 `consteval` 可以

- 包含跳转语句 `if` `switch` 和循环语句 `while` `for`
- 包含多条语句
- 调用其他 `constexpr` 函数，但是 `constexpr` 不能在非编译期上下文调用 `consteval` 函数
- 使用基本数据类型作为变量，并且这些变量必须使用常量表达式初始化

`consteval` 函数不能

- 包含 `static` 或者 `thread_local` 变量
- 包含 `try/catch` 或 `goto`
- 调用非 `consteval` 函数或使用非 `constexpr` 数据

一个有意思的事情是可以使用运行时的函数，但是在编译期执行这个运行时函数会报错。

`constinit` 用于声明具有静态存储（`static storage`）或线程局部存储（`thread-local storage`）的变量，并且这些变量必须在编译期初始化。比如全局变量、`static` 变量或类中的 `static` 成员变量。

`constinit` 编译期初始化但是并不意味着变量是常量。也就是说，`constinit` 变量可以在运行时被修改，只是它必须在编译期完成初始化。它需要一个编译期的常量来初始化，但是它不能用于初始化另一个 `constinit` 变量。

### 对比
`consteval` 与 `constexpr` 修饰函数时，前者必须在编译期求值，而后者可以在编译期或运行时求值。

`const` 修饰的变量可以在运行时初始化，而 `constinit` `constexpr` 修饰的变量必须在编译期初始化。`const` `constexpr` 修饰的变量是常量，而 `constinit` 修饰的变量不是常量。

`const` `constexpr` 可以修饰局部变量，而 `constinit` 不能修饰局部变量。

### 静态初始化顺序
静态初始化顺序问题是 C++ 中一个极其隐蔽且容易引发错误的问题。当一个静态变量无法在编译期完成常量初始化时，它会被零初始化（`zero-initialized`）。在随后的运行期内，对于这些静态变量会进行动态初始化（`dynamic initialization`）。同一个编译单元（`translation unit`）中的静态变量会按照它们在源文件中出现的顺序进行初始化，而不同编译单元中的静态变量初始化顺序是不确定的，这就可能导致静态初始化顺序问题。比如下面两个 `cpp` 文件中的 `staticB` 就有可能是 0 也有可能是 25，依赖于链接顺序或者其他因素。
```cpp
// file1.cpp
int square(int n) { return n * n; }

auto staticA = square(5);

// file2.cpp
extern int staticA;

auto staticB = staticA;
```
但这通常说明代码设计不合理，需要重构。但如果由于某些客观原因无法重构时，可以使用延迟初始化（`lazy initialization`）和单例模式保证初始化顺序。
```cpp
// file1.cpp
int square(int n) { return n * n; }

int &staticA()
{
	static int value = square(5);
	return value;
}

// file2.cpp
int& staticA();

auto staticB = staticA();
```
有了 `constinit`，我们有了更简洁的解决方案，可以让 `staticA` 在编译期完成初始化（注意 `square` 需声明为 `constexpr` 才能用于常量初始化）。
```cpp
// file1.cpp
constexpr int square(int n) { return n * n; }

constinit int staticA = square(5);

// file2.cpp
extern constinit int staticA;

auto staticB = staticA;
```
