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
当折叠表达式用于空的参数包时，适用以下规则。

* 操作符是 `&&` 时，值是 `true`。
* 操作符是 `||` 时，值是 `false`。
* 操作符是 `,` 时，值是 `void()`。
* 对于其他情况，格式错误。

对于其他情况，参数包可能为空，我们可以添加一个初始值 `value`。C++17 中允许如下写法。

* 二元左折叠：`(value op ... op args) // (((value op arg1) op arg2) op arg3) op ...`
* 二元右折叠：`(args op ... op value) // arg1 op (arg2 op .... (argN op value)))`

比如如下的写法允许传入空的参数包。这个例子中 0 的位置不应该影响结果，因此左折叠和右折叠写法是等价的。
```cpp
template <typename... T>
auto foldSum(T... s)
{
    return (0 + ... + s); // even works if sizeof...(s)==0
}

template <typename... T>
auto foldSum(T... s)
{
    return (s + ... + 0); // even works if sizeof...(s)==0
}
```
和一元折叠表达式一样，这里更倾向于左折叠写法：`(val + ... + args); // preferred syntax for binary fold expressions`。

有的时候，第一个参数比较特殊。比如下面的例子。

下面的代码中，第一个参数传递给 `std::cout` 后，返回流本身，能够接着处理后面的参数。
```cpp
template <typename... T>
void print(const T &...args)
{
    (std::cout << ... << args) << '\n';
}
```
如果写成其他形式，可能无法编译或者非期望的结果。比如如果像下面这样实现，`print(1)` 会将 1 左移 `\n`，通常是 10，比特，然后输出，那么输出结果是 1024。
```cpp
std::cout << (args << ... << '\n');
```
另外，这个例子中，各个参数中间没有分隔符，比如 `print("hello", 42, "world")` 的输出是 `hello42world`。
为了让每个参数中间有一个空格，那么输出时除了第一个参数之外其他参数前面需要先输出一个空格。可以使用一个帮助函数来实现。
```cpp
template <typename T>
const T &spaceBefore(const T &arg)
{
    std::cout << ' ';
    return arg;
}

template <typename First, typename... Args>
void print(const First &firstarg, const Args &...args)
{
    std::cout << firstarg;
    (std::cout << ... << spaceBefore(args)) << '\n';
}
```
此时，折叠表达式展开形式如下
```cpp
(std::cout << ... << spaceBefore(args));
std::cout << spaceBefore(arg1) << spaceBefore(arg2) << ...;
```
对在参数包 `args` 中每一个参数，帮助方法先输出一个空格，然后返回对应 `arg` 交给 `std::out` 输出。为了不影响第一个参数，`print` 专门新增了一个参数处理第一个参数的问题，并且不调用 `spaceBefore`。

我们可以在 lambda 表达式在 `print` 内部定义 `spaceBefore`。
```cpp
template <typename First, typename... Args>
void print(const First &firstarg, const Args &...args)
{
    std::cout << firstarg;
    auto spaceBefore = [](const auto &arg)
    {
        std::cout << ' ';
        return arg;
    };

    (std::cout << ... << spaceBefore(args)) << '\n';
}
```
lambda 表达式返回对象默认是按值传递，这里 `arg` 就被拷贝了一次。显式指明返回类型为 `const auto&` 或 `decltype(auto)` 可以避免这个问题。
```cpp
template <typename First, typename... Args>
void print(const First &firstarg, const Args &...args)
{
    std::cout << firstarg;
    auto spaceBefore = [](const auto &arg) -> const auto &
    {
        std::cout << ' ';
        return arg;
    };

    (std::cout << ... << spaceBefore(args)) << '\n';
}
```
当然，C++ 允许我们写到一行。
```cpp
template <typename First, typename... Args>
void print(const First &firstarg, const Args &...args)
{
    std::cout << firstarg;
    (std::cout << ... << [](const auto &arg) -> decltype(auto)
     {
         std::cout << ' ';
         return arg;
     }(args))
        << '\n';
}
```
这里是为了展示二元折叠，其实 `print` 更容易的实现是使用一元折叠，lambda 表示大不仅输出空格，还输出当前参数。如果添加一个 `auto` 类型的新参数，可以让用户指定分隔符。
```cpp
template <typename First, typename... Args>
void print(First first, const Args &...args)
{
    std::cout << first;
    auto outWithSpace = [](const auto &arg)
    {
        std::cout << ' ' << arg;
    };

    (..., outWithSpace(args));
    std::cout << '\n';
}
```

### Supported Operators
折叠表达式可以用于 `.` `->` `[]` 之外的所有二元运算符。

#### Folded Function Calls
折叠表达式应用于逗号表达式 `,`，可以将多个函数调用写作一样。下面的例子中，会将传入的参数逐个调用 `foo()` 函数。
```cpp
template <typename... Types>
void callFoo(const Types &...args)
{
    (..., foo(args)); // calls foo(arg1), foo(arg2), foo(arg3), ...
}
```
支持移动语义。
```cpp
template <typename... Types>
void callFoo(Types &&...args)
{
    (..., foo(std::forward<Types>(args))); // calls foo(arg1), foo(arg2), ...
}
```
如果 `foo()` 的返回值类型重载了 `,` 表达式，需要将结果转成 `void`。
```cpp
template <typename... Types>
void callFoo(const Types &...args)
{
    (..., (void)foo(std::forward<Types>(args))); // calls foo(arg1), foo(arg2), ...
}
```
对于逗号表达式，左折叠和右折叠是一样的，总是从左向右依次调用。右折叠写法 `(foo(args) , ...);` 与左折叠的不同仅仅在于这种写法括号会把后面的调用先结合起来，类似 `foo(arg1) , (foo(arg2) , foo(arg3));`，求值还是从左到右进行的。

这里再次强调一下，左折叠是更自然的顺序，因此还是鼓励写成左折叠的形式。

#### Combining Hash Functions
使用逗号表达式的另一个例子是结合哈希值。
```cpp
template <typename T>
void hashCombine(std::size_t &seed, const T &val)
{
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename... Types>
std::size_t combinedHashValue(const Types &...args)
{
    std::size_t seed = 0;           // initial seed
    (..., hashCombine(seed, args)); // chain of hashCombine() calls
    return seed;
}
```
通过调用 `combinedHashValue ("Hi", "World", 42);`，展开成 `hashCombine(seed,"Hi"), hashCombine(seed,"World"), hashCombine(seed,42);`。

有了这些定义，可以简单的定义一个函数，用于自定义类型，比如 `Customer`，的 `unordered_set` 或 `unordered_map`。
```cpp
struct CustomerHash
{
    std::size_t operator()(const Customer &c) const
    {
        return combinedHashValue(c.getFirstname(), c.getLastname(),
                                 c.getValue());
    }
};

std::unordered_set<Customer, CustomerHash> coll;
std::unordered_map<Customer, std::string, CustomerHash> map;
```

#### Folded Function Calls for Base Classes
折叠函数调用可以用于更复杂的例子。比如可以用逗号表达式调用可变基类的成员函数。
```cpp
#include <iostream>

// template for variadic number of base classes
template <typename... Bases>
class MultiBase : private Bases...
{
public:
    void print()
    {
        // call print() of all base classes:
        (..., Bases::print());
    }
};

struct A
{
    void print() { std::cout << "A::print()\n"; }
};

struct B
{
    void print() { std::cout << "B::print()\n"; }
};

struct C
{
    void print() { std::cout << "C::print()\n"; }
};

int main()
{
    MultiBase<A, B, C> mb;
    mb.print();
}
```
下面的模板类使得我们可以用可变个数基类来实例化对象。
```cpp
template <typename... Bases>
class MultiBase : private Bases...
{
};

MultiBase<A, B, C> mb;
```
下面这一句是折叠表达式，展开成调用各个基类的 `print` 函数。
```cpp
// (A::print() , B::print()) , C::print();
(..., Bases::print());
```

#### Folded Path Traversals
下面这个例子是折叠表达式使用运算符 `->*` 来遍历二叉树的一条路径。
```cpp
// define binary tree structure and traverse helpers:
struct Node
{
    int value;
    Node *subLeft{nullptr};
    Node *subRight{nullptr};

    Node(int i = 0)
        : value{i}
    {
    }

    int getValue() const
    {
        return value;
    }

    // traverse helpers:
    static constexpr auto left = &Node::subLeft;
    static constexpr auto right = &Node::subRight;
    // traverse tree, using fold expression:
    template <typename T, typename... TP>
    static Node *traverse(T np, TP... paths)
    {
        return (np->*...->*paths); // np ->* paths1 ->* paths2 ...
    }
};

#include "foldtraverse.hpp"
#include <iostream>

int main()
{
    // init binary tree structure:
    Node *root = new Node{0};
    root->subLeft = new Node{1};
    root->subLeft->subRight = new Node{2};

    // traverse binary tree:
    Node *node = Node::traverse(root, Node::left, Node::right);
    std::cout << node->getValue() << '\n';
    node = root->*Node::left->*Node::right;
    std::cout << node->getValue() << '\n';
    node = root->subLeft->subRight;
    std::cout << node->getValue() << '\n';
}
```

### Using Fold Expressions for Types
通过类型特征，使用折叠表达式可以处理模板参数包，包含任意个数的类型。比如下面折叠表达式的作用是返回传入的类型是否都相同。
```cpp
#include <type_traits>

// check whether passed types are homogeneous:
template <typename T1, typename... TN>
struct IsHomogeneous
{
    static constexpr bool value = (std::is_same_v<T1, TN> && ...);
};

// check whether passed arguments have the same type:
template <typename T1, typename... TN>
constexpr bool isHomogeneous(T1, TN...)
{
    return (std::is_same_v<T1, TN> && ...);
}
```
类型特征的使用示例
```cpp
// std::is_same_v<int, MyType> &&std::is_same_v<int, decltype(42)>
IsHomogeneous<int, Size, decltype(42)>::value;
```
模板函数的使用示例。
```cpp
// std::is_same_v<int, int> &&std::is_same_v<int, const char *>
//     &&std::is_same_v<int, std::nullptr_t>
isHomogeneous(43, -1, "hello", nullptr);
```
这里 `&&` 也具备短路特性，只要遇到 `false` 则停止判断。
