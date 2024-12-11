编程中，我们经常会遇到一个场景，可能会传递、返回、使用一个确定类型的值，有可能是没有值的。对于指针类型，`nullptr` 表示没有值。其他类型需要返回一个确定类型和一个 `bool` 值表示是否有值。`std::optional<>` 就是类型安全的实现。

可选对象内部包含了一个对象和一个布尔变量，因此多占一个字节，考虑到对其等因素，额外占用空间会略大。可选对象无需堆分配，对其方式与包含的对象一致。

可选对象的实现并不等于一个布尔变量加一个值对象。如果没有值，不会调用包含类型的构造函数，没有默认构造函数的对象也能处于一个默认状态。

`std::variant<>` `std::any` 一样，结果对象是值类型。拷贝的时候是深拷贝，创建了一个完全独立的对象。如果没有包含值，那么拷贝 `std::optional<>` 开销很低，如果有包含值，那么拷贝 `std::optional<>` 的成本和直接拷贝对象一样。`std::optional<>` 支持移动语义。

## Using `std::optional<>`
`std::optional<>` 表示的是可空的任意类型。实例可以是成员、参数和返回值。

### Optional Return Values
下面是使用 `std::optional<>` 作为返回类型的例子。`asInt()` 将传入的字符串转换成整数。由于可能不成功，因此使用 `std::optional<>` 作为返回类型，这样可以表示没有整数这个语义，而不是返回一个特殊约定的整数或者抛出异常。
```cpp
#include <optional>
#include <string>
#include <iostream>

// convert string to int if possible:
std::optional<int> asInt(const std::string &s)
{
    try
    {
        return std::stoi(s);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

int main()
{
    for (auto s : {"42", " 077", "hello", "0x33"})
    {
        // try to convert s to int and print the result if possible:
        std::optional<int> oi = asInt(s);
        if (oi)
        {
            std::cout << "convert '" << s << "' to int: " << *oi << "\n";
        }
        else
        {
            std::cout << "can't convert '" << s << "' to int\n";
        }
    }
}
```
