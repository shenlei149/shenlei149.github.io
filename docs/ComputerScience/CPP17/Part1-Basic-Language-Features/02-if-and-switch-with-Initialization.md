`if` 和 `switch` 控制流程语句可以在条件表达式前面添加一个初始化语句。比如
```cpp
if (status s = check(); s != status::success)
{
    return s;
}
```
`status s = check()` 在整个 `if` 语句内有效（包括 `else` 分支）。

## `if` with Initialization
下面是一个更完整的例子。`strm` 会在 `if` 结束之后被销毁。
```cpp
if (std::ofstream strm = getLogStrm(); coll.empty())
{
    strm << "<no data>\n";
}
else
{
    for (const auto &elem : coll)
    {
        strm << elem << '\n';
    }
}
```
下面是一个锁的例子，第二段代码利用类模板参数推导进行了简化。
```cpp
if (std::lock_guard<std::mutex> lg{collMutex}; !coll.empty())
{
    std::cout << coll.front() << '\n';
}

if (std::lock_guard lg{collMutex}; !coll.empty())
{
    std::cout << coll.front() << '\n';
}
```
上述代码大约等价于下面的实现，不过 `lg` 的作用范围有一丢丢差异。
```cpp
{
    std::lock_guard<std::mutex> lg{collMutex};
    if (!coll.empty())
    {
        std::cout << coll.front() << '\n';
    }
}
```
初始化必须指定一个变量名，否则临时变量创建之后立即就会被销毁。比如下面的例子，`std::lock_guard` 在初始化完之后就被销毁了，后续代码并不在锁的保护范围内。
```cpp
if (std::lock_guard<std::mutex>{collMutex}; // runtime ERROR:
    !coll.empty())                          // - no longer locked
{
    std::cout << coll.front() << '\n'; // - no longer locked
}
```
原则上讲，这里可以使用名字 `_`。
```cpp
if (std::lock_guard<std::mutex> _{collMutex}; // OK, but...
    !coll.empty())
{
    std::cout << coll.front() << '\n';
}
```
可以有多个声明，也可以初始化它们。
```cpp
if (auto x = qqq1(), y = qqq2(); x != y)
{
    std::cout << "return values " << x << " and " << y << " differ\n";
}

if (auto x{qqq1()}, y{qqq2()}; x != y)
{
    std::cout << "return values " << x << " and " << y << " differ\n";
}
```
下面是一个向 `std::map` 插入数据的例子。
```cpp
std::map<std::string, int> coll;

if (auto [pos, ok] = coll.insert({"new", 42}); !ok)
{
    // if insert failed, handle error using iterator pos:
    const auto &[key, val] = *pos;
    std::cout << "already there: " << key << '\n';
}
```
在 C++17 之前，上述代码实现如下。
```cpp
auto ret = coll.insert({"new", 42});
if (!ret.second)
{
    // if insert failed, handle error using iterator ret.first
    const auto &elem = *(ret.first);
    std::cout << "already there: " << elem.first << '\n';
}
```

编译期 `if` 也可以使用这个特性。

## `switch` with Initialization
使用带初始化的 `switch` 的语句，可以在条件表达式求值之前写一个初始化一个对象。

比如下面的例子，先声明一个文件系统路径然后根据其路径进行处理。注意，`p` 在整个 `switch` 都有效。
```cpp
namespace fs = std::filesystem;

switch (fs::path p{name}; status(p).type())
{
case fs::file_type::not_found:
    std::cout << p << " not found\n";
    break;
case fs::file_type::directory:
    std::cout << p << ":\n";
    for (const auto &e : std::filesystem::directory_iterator{p})
    {
        std::cout << "- " << e.path() << '\n';
    }
    break;
default:
    std::cout << p << " exists\n";
    break;
}
```
