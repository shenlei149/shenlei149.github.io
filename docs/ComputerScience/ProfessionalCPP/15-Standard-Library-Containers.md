## 概述
C++ 标准库提供很多容器。顺序容器有 `std::vector` `std::deque` `std::list` `std::forward_list` `std::array`；顺序视图 `std::span` `std::mdspan`；容器适配器有 `std::queue` `std::priority_queue` `stack`；关联容器有三类，有序的、无序的和平铺的，分别是 `std::map` `std::multimap` `std::set` `std::multiset` `std::unordered_map` `std::unordered_multimap` `std::unordered_set` `std::unordered_multiset` `std::flat_map` `std::flat_multimap` `std::flat_set` `std::flat_multiset`。另外某种程度上 `std::string` 和流也能作为容器使用，`std::bitset` 存储固定个数的 bits。

容器是值语义，因此放进去的类型要支持拷贝，请求访问容器内的元素，返回的是拷贝的引用。如果想保存引用，可以这样声明 `std::vector<std::reference_wrapper<T>>`，可以使用 `std::ref` `std::cref` 得到对象的引用。只能移动的元素也可以放到容器中，此时有些操作会导致无法编译。模板支持自定义分配器，但是有默认值，因此大部分场景无需关心。对于关联容器，可以自定义比较，也有默认值。标准库容器内部会经常使用移动或者拷贝，因此要放入容器的类型最好实现移动语义。容器析构的时候，会依次析构存储的元素。

标准库容器只做了相当有限的错误检查，使用的时候需要操作确保有效。

## 顺序容器
### `std::vector`

`<vector>` 头文件提供的 `std::vector` 是最常用的容器，本质是动态数组。最常用的函数有 `operator[]` 返回元素的引用，如果 `std::vector` 是 `const` 的，那么 `operator[]` 返回的是 `const` 引用，无法赋值；`at()` 类似，不过会检查传入的索引是否有效，无效会抛出异常；`size()` 返回有多少个元素；`front()` `back()` 返回第一个和最后一个元素的引用，如果 `std::vector` 为空，行为未定义。

C++23 开始 `std::format()` `std::print()` 支持了容器。

`std::vector` 有几个常用的构造函数。默认构造函数创建一个零个元素的 `std::vector`；`std::vector(count, default_value)` 构造包含 `count` 个值为 `default_value` 的 `std::vector`，如果不传递第二个参数，使用对应类型的零值初始化。可以使用初始化列表方便的初始化，比如 `std::vector<int> numbers = { 1, 2, 3, 4 }`。这里列表初始化的构造函数高于其他构造函数，因此 `std::vector<int> numbers{10, 100}` 构造的是两个元素 10, 100 的 `std::vector` 而不是 10 个元素值为 100 的 `std::vector`，如果想要后者，必须写成 `std::vector<int> numbers(10, 100)`。

`std::vector` 的拷贝会拷贝每一个元素，因此最好避免 `std::vector` 的拷贝，比如函数参数是 `std::vector` 的引用或 `const` 引用。`assign()` 会清除所有数据然后存放传入的数据。`swap()` 会交换两个 `std::vector` 的数据。

`std::vector` 支持六种比较运算符，当两个 `std::vector` 的长度一样，对应元素也相等，那么两个 `std::vector` 相等，否则不相等。从 0 开始逐个元素比较，第一个不相同的元素决定了哪个 `std::vector` 更小。

有两种常见方式来遍历 `std::vector`，如果无需修改，可以添加 `const`。
```cpp
for (auto &element : vec)
{
}

for (auto iter { std::begin(vec) }; iter != std::end(doubleVector); ++iter)
{
}
```

使用迭代器要注意两个问题，第一是 `end()` 返回的迭代器不指向任何有效数据，不要解引用，第二个是不要比较两个不同容器的迭代器。

`std::vector` 的迭代器是随机迭代器，可以向前也可以向后迭代，还可以使用 `+=` `-=` 跳跃。

`std::vector` 最常用的插入函数是 `push_back()`，相应的移除元素的函数是 `pop_back()`，不会后者并不返回元素，因此需要那个元素，需要先调用 `back()` 取得最后一个元素。

`insert()` 函数有五种重载，可以插入一个或多个元素到指定位置：插入一个元素；插入 `n` 个相同元素；插入迭代器指定的范围；移动一个元素到容器内；插入一个初始化列表。C++23 开始提供了额外一组函数 `assign_range()` 替换所有现有元素，`insert_range()` 插入给定范围到指定为止，`append_range()` 插入范围元素到尾部。

`erase()` 删除元素，传入一个迭代器的话删除指定元素，传入一组迭代器删除迭代器范围的元素。`clear()` 删除所有元素。

如果写一个 `for` 循环删除满足条件的元素的话，是平方复杂度，使用 C++20 提供的非成员函数 `std::erase()` 和 `std::erase_if()`，删除所有等于参数的元素或者所有满足传入的谓词的元素，线性复杂度。

`push_back()` 接受右值引用，将对象移动进容器。`emplace_back()` 就地构造对象，既不拷贝也不移动。`emplace()` 会在指定位置构造对象。

插入或者删除元素，`std::vector` 会移动插入或删除点之后的元素，因此时间复杂度是线性的，同时改点即之后的迭代器可能会失效。`std::vector` 还可能重新分配一块内存，导致所有的迭代器都失效。

`empty()` 返回容器是否有元素，`size()` 返回有多少个元素，`capacity()` 返回容量，再放 `size() - capacity()` 个元素，无需分配空间。`reserve()` 会实现分配足够的空间，但是不创建元素，`resize()` 类似，但是会创建元素。`shrink_to_fit()` 会缩小到合适的大小释放内存，但这仅仅是建议，取决于标准库的实现。另一种回收内存的方式是与一个空容器 `swap()`。

通过 `data()` 成员函数和 `std::data()` 能够直接访问数据，另一种方式是 `&vec[0]`，但是可读性和维护性不足够好。

当容器内的元素的移动构造和移动赋值是 `noexcept` 的，那么整个容器作为返回值的时候可以移动而不是拷贝返回。

### `std::deque`
`deque` 是 `double-ended queue`，和 `vector` 类似，但是使用频率低非常多。和 `vector` 的主要区别有：1）非连续存放；2）两端插入都是常量时间；3）提供在头部操作的函数 `push_front()` `pop_front()` `emplace_front()` `prepend_range()`；4）不会移动内存因此不会是迭代器失效；5）没有内存管理函数，比如 `reserve()` `capacity()`。

### `std::list`
标准库 `std::list` 是双向链表，在任意位置插入删除都是常量时间，但是访问是线性时间复杂度，不提供 `operator[]` 随机访问运算符。

`list` 迭代器是双向迭代器不是随机访问迭代器。

提供 `std::vector` `std::deque` 的插入和移出元素的函数。提供 `size()` `resize()` `empty()` 但是没有 `reserve()` `capacity()` 函数。

`splice()` 可以将一个链表的一个元素或者一个范围转移到另一个链表的指定位置，参数可以是左值引用也可以是右值引用。

`list` 提供一些特化的算法函数，更高效，`remove()` `remove_if()` `unique()` `merge()` `sort()` `reverse()`。

### `std::forward_list`
`std::forward_list` 是单链表，每个节点占用空间比 `std::list` 小一个指针大小，只能向后迭代，提供了大部分 `std::list` 提供的功能。

### `std::array`
固定大小，不会初始化元素，基本就是类型安全的、带长度的 C 风格数组。随机访问。固定大小，因此不支持 `push_back()` `pop_back()` `insert()` `erase()` `clear()` `resize()` `reserve()` `capacity()`。提供 `fill()` 填充整个数组。`vector` 的 `swap()` 是常量时间，但是 `array` 的 `swap()` 是线性时间复杂度，也不能被移动。

`std::get<n>()` 返回第 `n` 个元素，会做编译期的索引检查。

`std::to_array()` 可以将 C 风格数组转成 `std::array`。
