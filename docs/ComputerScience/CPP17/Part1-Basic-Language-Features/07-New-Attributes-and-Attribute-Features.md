C++11 引入了特性（`attribute`），允许或禁用某些警告的注解。C++17 引入了新的特性，还扩展了一些场景。

## Attribute `[[nodiscard]]`
新的特性 `[[nodiscard]]` 鼓励编译器对没有使用函数返回值的情况给出警告。

`[[nodiscard]]` 通常用于防范某些因为未用返回值导致的不当行为。

* 内存泄露；
* 未知的或出乎意料的行为；
* 不必要的开销；

下面举一些例子说明。

* 申请了资源但是没有释放，通过返回值返回给调用者，然后调用其他函数释放，此类函数应该标记为 `[[nodiscard]]`。比如 `malloc` `allocate`。
* `std::async` 是一个很好的例子说明不使用返回值会无意间改变函数的行为，这就是上文所述的出乎意料的行为。`std::async()` 异步开始工作，返回值是等待结束的句柄。如果不使用返回值，会调用临时对象的析构函数，而析构函数会等待任务结束，结果异步调用变成了同步执行。所以这里很适合标记成 `[[nodiscard]]`。
* `empty()` 是一个容易让人误解的函数，有的人认为它是清空容器。如果不使用返回值，相当于付出了代价却什么也没做。

使用 `(void)` 可以消除警告。

特性不会被继承，所以如果需要，需要重新添加 `[nodiscard]]`。
```cpp
struct B
{
    [[nodiscard]] int *foo();
};

struct D : B
{
    int *foo();
};

B b;
b.foo();       // warning
(void)b.foo(); // no warning

D d;
d.foo(); // no warning
```

你可以把这个特性标签放到所有的修饰符之前，或者函数名之后，但是不能放到其他地方。
```cpp
class C
{
    [[nodiscard]] friend bool operator==(const C &, const C &);
    friend bool operator!= [[nodiscard]] (const C &, const C &);
};
```

## Attribute `[[maybe_unused]]`
`[[maybe_unused]]` 的作用是避免编译器发出警告。这个特性可以用于类的声明、`typedef` 或 `using` 声明的变量、变量、非静态成员、函数、枚举声明、枚举的项。

下面是两个使用的例子。
```cpp
void foo(int val, [[maybe_unused]] std::string msg)
{
#ifdef DEBUG
    log(msg);
#endif
}

class MyStruct
{
    char c;
    int i;
    [[maybe_unused]] char makeLargerSize[100];
};
```
这个特性不能用于语句，所以无法直接抵消 `[[nodiscard]]` 的效果。
```cpp
[[nodiscard]] void *foo();

int main()
{
    foo();                           // WARNING: return value not used
    [[maybe_unused]] foo();          // ERROR: attribute not allowed here
    [[maybe_unused]] auto x = foo(); // OK
}
```

## Attribute `[[fallthrough]]`
如果一个 `switch` 语句的某个 `case` 没有 `break`，即想要往下继续，可以使用 `[[fallthrough]]` 避免编译器警告。

这个特性是一个单独的语句，以分号 `;` 结束。另外，不能用于最后一个分支。
```cpp
void commentPlace(int place)
{
    switch (place)
    {
    case 1:
        std::cout << "very ";
        [[fallthrough]];
    case 2:
        std::cout << "well\n";
        break;
    default:
        std::cout << "OK\n";
        break;
    }
}
```

## General Attribute Extensions
C++17 还扩展了特性使用的场景。

1. 特性可以用于命名空间。比如下面要废弃一个命名空间。
```cpp
namespace [[deprecated]] DraftAPI
{
}
```

2. 特性可以用于枚举项。比如下面有一个替代选项之后废除了被替代的项。
```cpp
enum class City
{
    Berlin = 0,
    NewYork = 1,
    Mumbai = 2,
    Bombay [[deprecated]] = Mumbai,
};
```

3. 对于自定义属性，往往在自己的命名空间内，可以使用 `using` 简化代码。
```cpp
[[MyLib::WebService, MyLib::RestService, MyLib::doc("html")]] void foo();

[[using MyLib: WebService, RestService, doc("html")]] void foo();

[[using MyLib: MyLib::doc("html")]] void foo(); // ERROR
```
