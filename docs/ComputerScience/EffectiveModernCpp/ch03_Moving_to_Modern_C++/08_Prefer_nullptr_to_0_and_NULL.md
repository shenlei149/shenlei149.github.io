`0` 是 `int` 类型，但是 C++ 在检查指针是否有效时，只能和 `0` 进行比较。但是 `0` 是 `int` 不是指针。

同样地，`NULL` 也是一个整数，甚至可能是 `long` 或者其他整数类型，这里重点不是具体类型，而是整数类型不是指针类型。`0` 和 `NULL` 都不是。

在 C++98 中，一个主要的影响是一个函数如果重载了整型和指针类型会导致奇怪的结果。传递 `0` 或者 `NULL` 并不会调用到接受指针作为参数类型的函数。
```cpp
void f(int); // three overloads of f
void f(bool);
void f(void *);

f(0);    // calls f(int), not f(void*)
f(NULL); // might not compile, but typically calls
         // f(int). Never calls f(void*)
```

当 `NULL` 是 `long` 时，`f(NULL)` 会编译失败，因为从 `long` 转化为 `int` `bool` `void*` 完全没有区别。但是这个语句实际想做的事情是调用 `f(void*)`。所以对于 C++98 程序员而言，推荐的做法是不要这样写重载函数，这对于 C++11 仍旧有效，因为这里推荐使用 `nullptr`，但是并不能阻碍程序员使用 `NULL` 或者 `0`。

`nullptr` 的优势是它的类型不是一个 `int` 类型。不过它也不是指针类型，可以把它想象成指向任意类型的指针。它的具体类型是 `std::nullptr_t`，它被定义为 `nullptr`，循环定义哈哈。它能转化成任意类型的裸指针。

使用 `nullptr` 作为参数调用 `f` 就会调用到 `void*` 这个重载版本，因为它不能被转化成整型。
```cpp
f(nullptr); // calls f(void*) overload
```

使用 `nullptr` 还有其他好处，比如能够让代码更整洁易读，特别是和 `auto` 配合的时候。假设有如下代码：
```cpp
auto result = findRecord(/* arguments */);
if (result == 0)
{
}
```

我们不能一眼看出来 `findRecord` 的返回类型具体是指针类型还是 `int` 类型。如果写成下面形式，就能很容易知道是指针类型。
```cpp
auto result = findRecord(/* arguments */);
if (result == nullptr)
{
}
```

当出现模板的时候，`nullptr` 尤其有用。假设我们需要拿到锁之后再调用某个函数，每个函数参数是不同的指针类型。
```cpp
int f1(std::shared_ptr<Widget> spw);    // call these only when
double f2(std::unique_ptr<Widget> upw); // the appropriate
bool f3(Widget *pw);                    // mutex is locked
```

调用代码如下：
```cpp
std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3
using MuxGuard =          // C++11 typedef; see Item 9
    std::lock_guard<std::mutex>;

{
    MuxGuard g(f1m);     // lock mutex for f1
    auto result = f1(0); // pass 0 as null ptr to f1
} // unlock mutex
{
    MuxGuard g(f2m);        // lock mutex for f2
    auto result = f2(NULL); // pass NULL as null ptr to f2
} // unlock mutex
{
    MuxGuard g(f3m);           // lock mutex for f3
    auto result = f3(nullptr); // pass nullptr as null ptr to f3
}
```

前面两个调用使用了 `0` 和 `NULL` 表示空指针，不是好的做法，但是能工作。不过，我们这里还有一个问题：很多重复代码，获取锁，调用函数，释放锁。我们使用模板来消除代码冗余。
```cpp
template <typename FuncType,
          typename MuxType,
          typename PtrType>
decltype(auto) lockAndCall(FuncType func, // C++14
                           MuxType &mutex,
                           PtrType ptr)
{
    using MuxGuard = std::lock_guard<MuxType>;

    MuxGuard g(mutex);
    return func(ptr);
}
```

现在调用新的函数
```cpp
auto result1 = lockAndCall(f1, f1m, 0);       // error!
auto result2 = lockAndCall(f2, f2m, NULL);    // error!
auto result3 = lockAndCall(f3, f3m, nullptr); // fine
```

前面两个调用无法通过编译。当传入 `0` 的时候，`lockAndCall` 对 `ptr` 的类型推导是 `int`，当调用 `f1` 的时候，`int` 不能转化成 `std::shared_ptr<Widget>` 类型。传入 `NULL` 的类似。

当传入 `nullptr` 的时候，`ptr` 的类型被推导为 `std::nullptr_t`，当传入 `f3` 时，`std::nullptr_t` 可以隐式地转化成 `Widget*`。

由于有以上好处，推荐使用 `nullptr` 替代 `NULL` 和 `0`。

## Things to Remember
* Prefer `nullptr` to `0` and `NULL`.
* Avoid overloading on integral and pointer types.
