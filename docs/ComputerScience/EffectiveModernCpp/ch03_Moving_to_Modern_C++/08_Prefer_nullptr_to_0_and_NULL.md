`0` 的类型是 `int`，但在需要空指针的上下文中，C++ 会将它转换为空指针。因此，`0` 本身并不是指针。

同样地，`NULL` 也是一个整数，甚至可能是 `long` 或其他整数类型。重点不在于它的具体类型，而在于整数类型不是指针类型；`0` 和 `NULL` 都不是指针。

在 C++98 中，如果函数同时重载了整数类型和指针类型，可能会产生令人意外的结果。传递 `0` 或 `NULL` 通常不会调用接受指针参数的重载。
```cpp
void f(int); // three overloads of f
void f(bool);
void f(void *);

f(0);    // calls f(int), not f(void*)
f(NULL); // might not compile, but typically calls
         // f(int). Never calls f(void*)
```

当 `NULL` 的类型是 `long` 时，`f(NULL)` 会编译失败，因为转换为 `int`、`bool` 或 `void*` 在重载决议中没有优先级差别。但这条语句实际上想调用的是 `f(void*)`。因此，对 C++98 程序员而言，推荐的做法是避免以整数类型和指针类型进行重载。这个建议对 C++11 仍然有效：尽管推荐使用 `nullptr`，程序员仍可能使用 `NULL` 或 `0`。

`nullptr` 的优势在于它的类型不是 `int`。不过，它也不是指针类型；可以将它视为能转换为任意指针类型的空指针值。它的类型是 `std::nullptr_t`，即 `decltype(nullptr)`。它可以隐式转换为任意对象指针、函数指针或成员指针类型。

使用 `nullptr` 作为参数调用 `f` 会调用 `void*` 重载，因为它不能转换为整数类型。
```cpp
f(nullptr); // calls f(void*) overload
```

使用 `nullptr` 还有其他好处，例如能让代码更整洁、更易读，尤其是在与 `auto` 配合时。假设有如下代码：
```cpp
auto result = findRecord(/* arguments */);
if (result == 0)
{
}
```

我们无法一眼看出 `findRecord` 的返回类型究竟是指针类型还是 `int` 类型。如果写成下面的形式，就能很容易知道它返回的是指针类型。
```cpp
auto result = findRecord(/* arguments */);
if (result == nullptr)
{
}
```

在模板中，`nullptr` 尤其有用。假设我们需要在获取锁后调用某个函数，而每个函数的参数都是不同的指针类型。
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

前两个调用使用 `0` 和 `NULL` 表示空指针。虽然能正常工作，但这不是好的做法。此外，这里还有一个问题：获取锁、调用函数和释放锁的代码重复很多。我们可以使用模板来消除这些重复代码。
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

现在调用这个新函数：
```cpp
auto result1 = lockAndCall(f1, f1m, 0);       // error!
auto result2 = lockAndCall(f2, f2m, NULL);    // error!
auto result3 = lockAndCall(f3, f3m, nullptr); // fine
```

前两个调用无法通过编译。传入 `0` 时，`lockAndCall` 将 `ptr` 推导为 `int`；调用 `f1` 时，`int` 不能转换为 `std::shared_ptr<Widget>`。传入 `NULL` 时也是类似的情况。

传入 `nullptr` 时，`ptr` 的类型被推导为 `std::nullptr_t`；调用 `f3` 时，`std::nullptr_t` 可以隐式转换为 `Widget*`。

鉴于上述好处，推荐使用 `nullptr` 替代 `NULL` 和 `0`。

## Things to Remember
* Prefer `nullptr` to `0` and `NULL`.
* Avoid overloading on integral and pointer types.
