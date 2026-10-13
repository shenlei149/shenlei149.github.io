
## `inline`
C++17 允许在头文件内部定义 `inline` 变量，这和函数的 `inline` 类似，目的是为了避免多重定义的问题。`inline` 变量在每个包含它的翻译单元中都是相同的实体，从而保证了全局唯一性。自此，无需再为头文件中声明的 `static` 变量单独在源文件中进行定义。使用方式如下：
```cpp
// header file example
struct MyClass
{
	static const int sValue;
};

inline const int MyClass::sValue = 777;

// or
struct MyClass
{
	static inline const int sValue = 777;
};
```
C++ 标准要求类的静态数据成员在声明为 `constexpr` 时隐式具有 `inline` 属性，因此在类内定义并初始化时无需额外加 `inline`。

在此之前很多 header-only 的类库需要 `inline` 函数来避免多重定义的问题。C++17 引入 `inline` 变量后，这类问题得以简化，比如下面这个例子，C++ 标准保证所有编译单元中 `MyClass::seed` 都是同一个实体（拥有相同的内存地址），并且是在运行时初始化的。
```cpp
// old
class MyClass
{
public:
	static inline int Seed();
};

inline int MyClass::Seed()
{
	static const int seed = rand();
	return seed;
}

// new
class MyClass
{
public:
	static inline int seed = rand();
};
```
