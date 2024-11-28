从 C++17 开始，可以使用折叠表达式对参数包内所有参数应用一个二元操作符。比如下面的例子，注意，这里的圆括号是折叠表达式的一部分，不能省略。
```cpp
template <typename... T>
auto foldSum(T... args)
{
    return (... + args); // ((arg1 + arg2) + arg3) ...
}
```
效果如下
```cpp
// return 47 + 11 + val + -1;
foldSum(47, 11, val, -1);

// return std::string("hello") + "world" + "!";
foldSum(std::string("hello"), "world", "!");
```
折叠表达式的参数位置很重要，三个点 `...` 写前面表示前面的元素先结合，写在后面表示后面的元素先结合。
```cpp
// ((arg1 + arg2) + arg3) ...
(... + args)

// (arg1 + (arg2 + arg3)) ...
(args + ...)
```

## Motivation for Fold Expressions
折叠表达式使得我们不再需要通过递归实例模版的方式处理参数包中所有的参数。减少了程序员和编译器的工作。
```cpp
template <typename T>
auto foldSumRec(T arg)
{
    return arg;
}

template <typename T1, typename... Ts>
auto foldSumRec(T1 arg1, Ts... otherArgs)
{
    return arg1 + foldSumRec(otherArgs...);
}

// now
template <typename... T>
auto foldSum(T... args)
{
    return (... + args); // arg1 + arg2 + arg3 ...
}
```

## Using Fold Expressions
给定参数 `args` 和操作符 `op`，C++17 允许我们使用

1. 一元左折叠：`(... op args)  // ((arg1 op arg2) op arg3) op ...`
`
2. 一元右折叠：`(args op ...)  // arg1 op (arg2 op ... (argN-1 op argN))`

这里的一元指的是折叠表达式的参数个数，而不是 `op`。

左折叠和右折叠的差异很大。比如 `+` 操作符，对于整数而言无所谓，但是对于 `std::string` 和字符串字面量而言，`+` 的操作数中必须至少有一个是 `std::string`，否则会报错。

下面是左折叠的例子。
```cpp
template <typename... T>
auto foldSumL(T... args)
{
    return (... + args); // ((arg1 + arg2) + arg3) ...
}

foldSumL(1, 2, 3); // ((1 + 2) + 3)

// (std::string("hello") + "world") + "!"
std::cout << foldSumL(std::string("hello"), "world", "!") << '\n'; // OK

// ("hello" + "world") + std::string("!")
std::cout << foldSumL("hello", "world", std::string("!")) << '\n'; // ERROR
```

下面是右折叠写法。
```cpp
template <typename... T>
auto foldSumR(T... args)
{
    return (args + ...); // (arg1 + (arg2 + arg3)) ...
}

foldSumR(1, 2, 3); // (1 + (2 + 3))

std::cout << foldSumR(std::string("hello"), "world", "!") << '\n'; // ERROR

std::cout << foldSumR("hello", "world", std::string("!")) << '\n'; // OK
```
对于大部分情况而言，从左往右计算更直观，因此应该倾向于使用左折叠语法。

### Dealing with Empty Parameter Packs
