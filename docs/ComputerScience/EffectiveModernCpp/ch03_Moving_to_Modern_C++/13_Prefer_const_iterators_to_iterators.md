STL 中的 `const_iterator` 等价于指向 `const` 的指针，不能通过它们修改所指向的值。标准实践是尽可能地使用 `const`，因此也应该尽可能地使用 `const_iterator`。

但是，由于 C++98 对 `const_iterator` 的支持很差，所以它不好用。`const_iterator` 很难创建；即使创建出来，能够使用它的地方也非常有限。例如，下面的例子在 `vector<int>` 中查找第一个值为 1983 的元素，并在其前插入 1998。如果没有找到，则在末尾插入 1998。C++98 中使用 `iterator` 的示例如下：
```cpp
std::vector<int> values;
std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
values.insert(it, 1998);
```

在这里，`iterator` 不是一个很好的选择，因为我们不打算通过它修改元素。在 C++98 中使用 `const_iterator` 非常繁琐，下面是一种可能可行的方法：
```cpp
typedef std::vector<int>::iterator IterT; // typedefs
typedef std::vector<int>::const_iterator ConstIterT;

std::vector<int> values;

ConstIterT ci =
    std::find(static_cast<ConstIterT>(values.begin()), // cast
              static_cast<ConstIterT>(values.end()),   // cast
              1983);

values.insert(static_cast<IterT>(ci), 1998); // may not compile; see below
```

`typedef` 不是必需的。如 [Item 9](./09_Prefer_alias_declarations_to_typedefs.md) 所言，应优先使用 `using`；不过那是 C++11 的语法，而这里是 C++98 的示例。

`std::find` 中有两次类型转换，原因是 `values` 不是 `const` 容器，因而没有简单的方式获取 `const_iterator`。另一种方法是将 `values` 绑定到一个 `const` 引用，然后用该引用替代 `values`。不管怎样，都很麻烦。

即使获取了 `const_iterator`，C++98 中的 `insert` 也只能接受 `iterator` 来指定插入位置。好不容易得到的 `const_iterator`，又不得不转换回 `iterator`。

上面的代码可能无法通过编译，因为没有可移植且可靠的方法将 `const_iterator` 转换为 `iterator`。`static_cast`，甚至 `reinterpret_cast` 都不行；即使到了 C++11 也是如此，尽管这种转换看起来合理。由于这些限制，尽可能使用 `const_iterator` 这条原则在 C++98 时代并不切实际。

C++11 则完全不同。即使对于非 `const` 容器，`cbegin`、`cend` 也能返回 `const_iterator`。STL 的成员函数，如 `insert`、`erase`，也都接受 `const_iterator` 作为参数。这使得在 C++11 中使用 `const_iterator` 与在 C++98 中使用 `iterator` 一样容易。
```cpp
std::vector<int> values;                                   // as before
auto it = std::find(values.cbegin(), values.cend(), 1983); // use cbegin and cend
values.insert(it, 1998);
```

如果要编写更通用的代码，就需要考虑那些只提供非成员 `begin`、`end` 等函数的容器或类容器对象。原生数组便是一例，某些类库也只提供此类非成员函数。更通用的代码需要考虑这些情况。

前面的例子可以写成一个更通用的函数模板。
```cpp
template <typename C, typename V>
void findAndInsert(C &container,       // in container, find first occurrence
                   const V &targetVal, // of targetVal, then insert insertVal
                   const V &insertVal)
{
    using std::cbegin;
    using std::cend;
    auto it = std::find(cbegin(container), // non-member cbegin
                        cend(container),   // non-member cend
                        targetVal);
    container.insert(it, insertVal);
}
```

## Things to Remember
* Prefer `const_iterator` to `iterator`.
* In maximally generic code, prefer non-member versions of `begin`, `end`, `rbegin`, etc., over their member function counterparts.
