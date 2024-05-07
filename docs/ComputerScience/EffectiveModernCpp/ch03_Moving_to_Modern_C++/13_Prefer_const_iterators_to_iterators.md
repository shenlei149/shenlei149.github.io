STL 中的 `const_iterator` 等价于指向 `const` 的指针，它们指向的值不会被修改。标准实践中尽可能的使用 `const`，因此也应该尽可能的使用 `const_iterator`。

但是由于 C++98 中对 `const_iterator` 支持的很差劲，所以不好用。很难创建，即使创建出来了，能够使用的地方也非常有限。比如下面的例子，在 `vector<int>` 中寻找第一个 1983 出现的地方，在其后插入 1998。如果没有找到，那么在最后插入 1998。C++98 使用 `iterator` 的例子如下
```cpp
std::vector<int> values;
std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
values.insert(it, 1998);
```

在这里，`iterator` 不是一个很好的选择，因为我们不打算修改它。在 C++98 中使用 `const_iterator` 非常繁琐，下面是可能可行的方法
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

`typedef` 不是必须的，如 [Item 9](/EffectiveModernCpp/ch03_Moving_to_Modern_C++/09_Prefer_alias_declarations_to_typedefs.md) 所言，更应该使用 `using`，不过那是 C++11 的语法而这是 C++98 的例子。

`std::find` 中有两次类型转化，原因是 `values` 不是 `const` 容器，没有很简单的方式获取 `const_iterator`。另一个可选的方法是将 `values` 绑定到一个 `const` 引用上，然后用这个引用替代 `values`。不管怎么说，都很麻烦。

即使获取了 `const_iterator`，但是 C++98 中 `insert` 只能接受 `iterator` 作为类型来指定位置。好不容易得到的 `const_iterator`，又不得不转化回 `iterator`。

上面的代码可能无法通过编译，因为没有可移植的稳定的方法从 `const_iterator` 到 `iterator`。`static_cast`，甚至是 `reinterpret_cast` 都不行，即使到了 C++11 也不行，虽然这件事看起来挺合理。因为这种种限制，尽可能使用 `const_iterator` 这条原则在 C++98 时代不实际。

C++11 就完全不一样了。即使对于非 `const` 容器，`cbegin,cend` 也能返回 `const_iterator` 对象。STL 成员函数，比如 `insert` `erase` 也都接受 `const_iterator` 作为参数。这使得在 C++11 中使用 `const_iterator` 使用和 C++98 中使用 `iterator` 一样容易。
```cpp
std::vector<int> values;                                   // as before
auto it = std::find(values.cbegin(), values.cend(), 1983); // use cbegin and cend
values.insert(it, 1998);
```

如果我们编写更通用的代码，需要考虑容器或者类容器对象只提供了非成员函数 `begin` `end` 等。其中一种情况就是原生的数组，或者是一些只提供非成员函数的类库。更通用的类库需要考虑这些。

之前的例子可以写成一个更通用的模板函数。
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
