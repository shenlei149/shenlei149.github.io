使用 `if constexpr(...)` 可以在编译期哪个分支可以被丢弃掉。被丢弃掉的部分不会生成代码。不过丢弃不意味着完全消失，也会进行模板语法检查。
比如下面的例子，根据传入参数的类型，一定会有两个分支被丢弃，因此传入不同类型的参数能正常编译运行。
```cpp
#include <string>
template <typename T>
std::string asString(T x)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return x; // statement invalid if no conversion to string
    }
    else if constexpr (std::is_arithmetic_v<T>)
    {
        return std::to_string(x); // statement invalid if x is not numeric
    }
    else
    {
        return std::string(x); // statement invalid if no conversion to string
    }
}

#include "ifcomptime.hpp"
#include <iostream>
int main()
{
    std::cout << asString(42) << '\n';
    std::cout << asString(std::string("hello")) << '\n';
    std::cout << asString("hello") << '\n';
}
```

## Motivation for Compile-Time `if`
如果上述代码使用运行时 `if`，则无法通过编译。当模板实例化的时候，整个模板函数作为整体进行编译。即使 `if` 条件能够在编译期确定，但是对应的代码还是需要编译，因为现在是运行时确定分支。此时，不管 `T` 是什么类型，都会有分支无法适配而导致变异错误。
```cpp
#include <string>
template <typename T>
std::string asString(T x)
{
    if (std::is_same_v<T, std::string>)
    {
        return x; // ERROR if no conversion to string
    }
    else if (std::is_numeric_v<T>)
    {
        return std::to_string(x); // ERROR if x is not numeric
    }
    else
    {
        return std::string(x); // ERROR if no conversion to string
    }
}
```
但是对于编译期 `if` 的版本，当实例化模板函数的时候，总会有两个不合法的分支被丢弃，因此能通过编译。

注意，不能忽视被丢弃的代码。被丢弃的代码也必须语法正确，与模版参数无关的调用也必须正确。事实上，编译模板的第一个阶段（定义期）会检查语法和与模板无关的名字是否有效。即使在被丢弃的分支，`static_assert` 也必须有效。
```cpp
template <typename T>
void foo(T t)
{
    if constexpr (std::is_integral_v<T>)
    {
        if (t > 0)
        {
            foo(t - 1); // OK
        }
    }
    else
    {
        undeclared(t);                       // error if not declared and not discarded (i.e., T is not integral)
        undeclared();                        // error if not declared (even if discarded)
        static_assert(false, "no integral"); // always asserts (even if discarded)
    }
}
```
上面代码中有两处导致无法通过编译。

第一处是 `undeclared();`，因为其与模板参数无关，即使实例化时 `T` 是整数类型，`else` 分支被丢弃，未定义的函数也会导致错误。

第二处是 `static_assert(false, "no integral");`，原因和上述一致。如果使用与模板相关的断言，比如 `static_assert(!std::is_integral_v<T>, "no integral");` 就不会有问题。

## Using Compile-Time `if`
实践中，可以像使用运行时 `if` 一样使用编译期 `if`，提供一个编译期的表达式即可，同时可以混用运行时 `if` 和编译期 `if`。
```cpp
if constexpr (std::is_integral_v<std::remove_reference_t<T>>)
{
    if (val > 10)
    {
        if constexpr (std::numeric_limits<char>::is_signed)
        {
        }
        else
        {
        }
    }
    else
    {
    }
}
else
{
}
```
注意，无法在函数外使用编译期 `if`，因此无法替代预处理时的条件编译。

### Caveats for Compile-Time `if`
使用编译期 `if` 可能会非预期的结果。

#### Compile-Time `if` Impacts the Return Type
编译期 `if` 可能回影响函数的返回值，因此这里在返回类型处使用 `auto`。
```cpp
auto foo()
{
    if constexpr (sizeof(int) > 4)
    {
        return 42; // int
    }
    else
    {
        return 42u; // unsigned int
    }
}

// return type might be int or void
auto foo()
{
    if constexpr (sizeof(int) > 4)
    {
        return 42;
    }
}
```

#### else Matters Even if then Returns
运行时 `if` 有一个模式不能套用于编译期 `if`：如果 `if` 和 `else` 分支都有返回语句的话，可以忽略 `else`。下面两种写法是等价的。
```cpp
if (...)
{
    return a;
}
else
{
    return b;
}

if (...)
{
    return a;
}

return b;
```
编译期 `if` 不能写成第二种形式的原因是 `else` 分支一定会留下，不会被丢弃，而第一种写法 `else` 是可能会被丢弃的。这样两种写法不完全等价。另外，返回类型可能会不同，比如下面的写法，有可能（也可能可以）无法通过编译。
```cpp
auto foo()
{
    if constexpr (sizeof(int) > 4)
    {
        return 42;
    }

    return 42u;
}
```

### Short-Circuit Compile-Time Conditions
下面的代码使用两个编译期条件来决定函数返回传入参数的两倍还是传入的参数本身。
```cpp
template <typename T>
constexpr auto foo(const T &val)
{
    if constexpr (std::is_integral<T>::value)
    {
        if constexpr (T{} < 10)
        {
            return val * 2;
        }
    }

    return val;
}

constexpr auto x1 = foo(42);   // yields 84
constexpr auto x2 = foo("hi"); // OK, yields "hi"
```
运行时 `if` 有短路优化，但是编译期 `if` 没有，会将表达式作为一个整体求值。下面的写法会导致传入不支持 `< 10` 的参数时编译错误。
```cpp
template <typename T>
constexpr auto bar(const T &val)
{
    if constexpr (std::is_integral<T>::value && T{} < 10)
    {
        return val * 2;
    }

    return val;
}

constexpr auto x2 = bar("hi"); // compile-time ERROR
```
因为没有短路，如果一个编译期条件依赖于另一个编译期的条件，那么不得不像第一个例子中那样分开写。下面也是一个正确一个错误的例子。
```cpp
if constexpr (std::is_same_v<MyType, T>)
{
    if constexpr (T::i == 42)
    {
    }
}

if constexpr (std::is_same_v<MyType, T> && T::i == 42)
{
}
```

### Other Compile-Time `if` Examples

#### Perfect Return of a Generic Value
编译期 `if` 的一个使用场景是先对返回值进行一些处理，然后完美转发。由于 `decltype(auto)` 不能为 `void` 的变量进行推导类型，那么不得不如下实现，返回类型可以推导为 `void`，但是 `ret` 无法推导为 `void`，因此必须对 `op` 返回 `void` 的情况单独处理。
```cpp
#include <functional>  // for std::forward()
#include <type_traits> // for std::is_same<> and std::invoke_result<>

template <typename Callable, typename... Args>
decltype(auto) call(Callable op, Args &&...args)
{
    if constexpr (std::is_void_v<std::invoke_result_t<Callable, Args...>>)
    {
        // return type is void:
        op(std::forward<Args>(args)...);
        // do something before we return
        return;
    }
    else
    {
        // return type is not void:
        decltype(auto) ret{op(std::forward<Args>(args)...)};
        // do something (with ret) before we return
        return ret;
    }
}
```

#### Compile-Time `if` for Tag Dispatching
编译期 `if` 的另一个应用场景是类型转发。之前需要为每一个想要处理的类型写一个重载函数，但是现在可以放到一个函数里面。下面是两种不同的实现方式的例子。

```cpp
template <typename Iterator, typename Distance>
void advance(Iterator &pos, Distance n)
{
    using cat = std::iterator_traits<Iterator>::iterator_category;
    advanceImpl(pos, n, cat{}); // tag dispatch over iterator category
}

template <typename Iterator, typename Distance>
void advanceImpl(Iterator &pos, Distance n,
                 std::random_access_iterator_tag)
{
    pos += n;
}

template <typename Iterator, typename Distance>
void advanceImpl(Iterator &pos, Distance n,
                 std::bidirectional_iterator_tag)
{
    if (n >= 0)
    {
        while (n--)
        {
            ++pos;
        }
    }
    else
    {
        while (n++)
        {
            --pos;
        }
    }
}

template <typename Iterator, typename Distance>
void advanceImpl(Iterator &pos, Distance n, std::input_iterator_tag)
{
    while (n--)
    {
        ++pos;
    }
}

// from c++17
template <typename Iterator, typename Distance>
void advance(Iterator &pos, Distance n)
{
    using cat = std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_convertible_v<cat, std::random_access_iterator_tag>)
    {
        pos += n;
    }
    else if constexpr (std::is_convertible_v<cat,
                                             std::bidirectional_access_iterator_tag>)
    {
        if (n >= 0)
        {
            while (n--)
            {
                ++pos;
            }
        }
        else
        {
            while (n++)
            {
                --pos;
            }
        }
    }
    else
    { // input_iterator_tag
        while (n--)
        {
            ++pos;
        }
    }
}
```

这里我们像是有一个编译期的 `switch` `case` 语句，每一个 `if constexpr` 是一个 `case`。

不过两种实现有些许不同，重载方式是最佳匹配，而 `if constexpr` 的实现是最先匹配。

## Compile-Time `if` with Initialization
编译期 `if` 可以使用带初始化的 `if` 形式。

比如下面这个例子，`foo` 函数也是参数类型为 `T` 的 `constexpr` 函数，那么就可以根据返回值类型做一些分支判断。
```cpp
template <typename T>
void bar(const T x)
{
    if constexpr (auto obj = foo(x); std::is_same_v<decltype(obj), T>)
    {
        std::cout << "foo(x) yields same type\n";
    }
    else
    {
        std::cout << "foo(x) yields different type\n";
    }
}
```

下面的例子是使用 `foo` 的返回值做分支判断，此时，需要对 `obj` 这个返回值加上 `constexpr`。
```cpp
constexpr auto c = ...;
if constexpr (constexpr auto obj = foo(c); obj == 0)
{
    std::cout << "foo() == 0\n";
}
```

## Using Compile-Time `if` Outside Templates
编译期 `if` 可以用于任意函数，不仅仅是模板函数。唯一的要求就是编译期表达式能够转成 `bool` 类型。不过需要注意的是，此时 `if` `else` 任意分支都需要是合法的，即使会被丢弃。

下面这个例子中，虽然 `else` 分支会被丢弃，但是由于有没有声明的函数，编译错误。
```cpp
#include <limits>
template <typename T>
void foo(T t);
int main()
{
    if constexpr (std::numeric_limits<char>::is_signed)
    {
        foo(42); // OK
    }
    else
    {
        undeclared(42); // ALWAYS ERROR if not declared (even if discarded)
    }
}
```
下面的例子无法编译，因为总有一个 `static_assert` 会报错。
```cpp
if constexpr (std::numeric_limits<char>::is_signed)
{
    static_assert(std::numeric_limits<char>::is_signed);
}
else
{
    static_assert(!std::numeric_limits<char>::is_signed);
}
```
非模板函数中使用编译期 `if` 的收益是尽管所有分支都合法，但是会有一部分被丢弃，那么编译的二进制大小会小一些。比如下面的例子，最后 `arr1` 和 `arr2` 只会有一个在最后的执行文件中。
```cpp
#include <limits>
#include <string>
#include <array>
int main()
{
    if (!std::numeric_limits<char>::is_signed)
    {
        static std::array<std::string, 1000> arr1;
    }
    else
    {
        static std::array<std::string, 1000> arr2;
    }
}
```
