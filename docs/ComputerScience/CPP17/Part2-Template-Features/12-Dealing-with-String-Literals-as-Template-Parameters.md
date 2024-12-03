C++ 一直在放宽模板参数的规则，C++17 也如此。现在无需在当前范围之外定义就可以使用模板了。

## Using Strings in Templates
非类型模板可以使用常量整数（包括枚举）、指向对象/函数/成员的指针、对象或者函数的左值引用、`std::nullptr_t` 实例化。

对于指针，C++17 必须使用外部链接或者内部链接的指针，但是 C++17 移除了这个限制。不过，我们仍然不能使用字符串字面量实例化模板，也就是说我们仍旧需要写两行代码使用字符串字面量实例化模板，不过这两行可以在一个范围内。
```cpp
template <const char *str>
class Message
{
};

extern const char hello[] = "Hello World!"; // external linkage
const char hello11[] = "Hello World!";      // internal linkage

void foo()
{
    Message<hello> msg;     // OK (all C++ versions)
    Message<hello11> msg11; // OK since C++11

    static const char hello17[] = "Hello World!"; // no linkage
    Message<hello17> msg17;                       // OK since C++17

    Message<"hi"> msgError; // ERROR
}
```
这消除了 C++11 中的一个使用限制：可以使用指针实例化模板，但是不能使用返回指针的编译期函数实例化模板。
```cpp
template <int *p>
struct A
{
};

int num;
A<&num> a; // OK since C++11

int num;
constexpr int *pNum()
{
    return &num;
}

A<pNum()> b; // ERROR before C++17, now OK
```
