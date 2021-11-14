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

## 顺序视图
`std::span` 包含指向第一个元素的指针和长度，表示一段连续的数据。如果一个函数，想对 `std::vector` `std::array` C 风格数组都能适用，可以考虑参数是 `std::span`。`subspan()` 可以方便的做切片。`first()` `last()` 方便放回前或后 `n` 个元素的 `std::span`。`std::span` 还提供了 `begin()` `end()`
`rbegin()` `rend()` `front()` `back()` `operator[]` `data()` `size()` `empty()`。与 `std::string_view` 不同的是，`std::span` 是可写的，如果不想修改，可以用 `const` 修饰类型 `T`。

`std::span` 是 C++23 提供的多维顺序视图，有四个模板参数，分别是元素类型，各个维度长度，布局（行优先、列优先、自定义）和访问策略（控制 `operator[]`）。这样，可以像访问多维数据（比如矩阵）这样访问线性数据。

## 容器适配器
C++ 提供三种容器：`std::queue` `std::priority_queue` `std::stack`。这是一个绝佳的接口和实现的例子，更换底层容器，无需改动其余代码。

队列是先进先出的数据结构。`std::queue` 定义如下
```cpp
template<typename T, typename Container = deque<T>>
class queue;
```
默认容器是 `deque`，还可以是 `list`，因为要求支持 `push_back()` `pop_front()`，因此不能使用 `vector`。

队列支持 `push()` `emplace()` 在尾部插入数据，C++23 提供了函数 `push_range()`。`pop()` 从头尾移出数据。访问头部和尾部使用 `front()` `back()` 函数，返回引用类型，如果队列是 `const`，那么返回的是 `const` 引用。还支持 `size()` `empty()` `swap()`。

优先级队列保持最高优先级的元素的队列头部。`std::priority_queue` 定义如下
```cpp
template<typename T, typename Container = vector<T>, typename Compare = less<T>>
class priority_queue;
```

默认容器是 `vector`，使用 `deque` 也是可行的，不过不能使用 `list`，因为需要随机访问。`Compare` 默认是 `T` 类型的 `operator<`，也可以自定义。

`std::priority_queue` 提供的接口更少，插入函数有 `push()` `emplace()` `push_range()`，`pop()` 删除头部元素，`top()` 返回 `const` 引用，因为需要保持有序所以这个不会返回非 `const` 的引用，否则调用者修改了内容导致无法保序。还支持 `size()` `empty()` `swap()`。

栈和队列恰好相反，先进后出的数据结构。`std::stack` 定义如下
```cpp
template<typename T, typename Container = deque<T>>
class stack;
```
默认是 `deque`，也可以使用 `vector` `list`。

`std::stack` 提供插入函数有 `push()` `emplace()` `push_range()`，`pop()` 删除元素，`top()` 返回栈顶的引用，是否是 `const` 依赖于栈对象。还支持 `size()` `empty()` `swap()`。

## 关联容器
### 有序关联容器
标准库提供四种有序关联容器（`ordered associative container`） `map` `multimap` `set` `multiset`。

首先，我们分析 `map`。它和 `vector` 类似，只不过存储的是 `pair`，同时底层数据结构按照 `pair.first`，即 `key` 排序了。

`map` 支持统一初始化，比如下面的例子。不过不支持类型推导，因此不能省略 `map` 后面的类型。
```cpp
std::map<std::string, int> m {
	{ "Marc G.",	12 },
	{ "Warren B.",  34 },
	{ "Peter V.W.", 56 }
};
```
有如下几种方式向 `map` 插入数据。第一种方式是调用 `insert()` 函数插入一个 `pair`，返回类型是一个 `pair`，`first` 是一个迭代器，`second` 表名是否插入成功，如果 `key` 已经存在，那么插入失败。C++23 提供了类似插入范围的 API `insert_range()`。第二种方式是 `operator[]`，总是会插入成功，如果存在，替换之前的旧值。这种方式总会创建一个对象，如果一个 `map` 是 `const` 的，因为 `const` 无法创建对象，编译会报错。`emplace()` `emplace_hint()` 会就地构造对象，`try_emplace()` 仅在 `key` 不存在的时候插入对象。

`find()` 返回查找的 `key` 对应的迭代器，如果没有找到，返回的是 `end()`。`at()` 类似与 `vector`，如果 `key` 不存在，会抛异常。`contains()` 函数返回是否包含次 `key`。

关联容器都是基于节点（`node`）的数据结构，`node_type` 表示存储的节点类型。节点是存储数据的所有者，通过 `extract()` 可以将指定节点抽出来，也就将存储数据从容器中删除了，通过 `insert()` 可以将节点直接插入另一个容器，这样无需拷贝或者移动数据就能完成数据在不同容器的转移。`merge()` 就是通过节点操作合并两个 `map`，合并后，参数 `map` 仅仅剩下与目标 `map` `key` 重复的数据。

`std::multimap` 允许 `key` 重复，因此与 `map` 不同，不提供 `operator[]` `at()` `insert_or_assign()` `try_emplace()` 函数，`insert()` 始终会成功。`find()` 会返回指向其中一个满足条件的数据，不一定是第一个。`lower_bound()` 和 `upper_bound()` 会返回一组迭代器，两者之间是满足条件的所有数据。`equal_range()` 是找一个 `key` 对应的迭代器对效率更高的选择。

`std::set` 和 `map` 近乎一样，只是只存储 `key` 不存储 `value`，因此没有 `operator[]` `insert_or_assign()` `try_emplace()` 函数。另外，不能修改保存在 `set` 中的数据，否则顺序可能就无法保证了。

`std::multiset` 之于 `std::set`，就是 `multimap` 之于 `map`。

### 无序关联容器
标准库提供四种无序关联容器，也称为哈希表，分别是 `std::unordered_map` `std::unordered_multimap` `std::unordered_set` `std::unordered_multiset`，与之前分析的 `map` `multimap` `set` `multiset` 对应。因此下面仅分析 `std::unordered_map`，包括与 `map` 的差异，其余三种容器类似，不再赘述。

`std::unordered_map` 定义如下。有五个模板参数，分别是 `key` 的类型，`value` 的类型，哈希函数，等于比较函数和分配器，后三个有默认参数。
```cpp
template<typename Key,
		 typename T,
		 typename Hash = hash<Key>,
		 typename Pred = std::equal_to<Key>,
		 typename Alloc = std::allocator<std::pair<const Key, T>>>
class unordered_map;
```

与 `map` 的差异如下表。`unordered_map` 支持一些和哈希相关的操作。比如 `load_factor` 表示每个桶有多少个元素，由此推测冲突次数。`bucket_count()` 返回桶的个数。`bucket(key)` 返回第几个桶，`begin(index)` `end(index)` 返回 `local_iterator`，这样就可以遍历桶里面的数据了。与 `local_iterator` 对应，也有 `const_local_iterator`。

| OPERATION | `map` | `unordered_map` |
|--|--|--|
| `at()` | Y | Y |
| `begin()` | Y | Y |
| `begin(n)` | N | Y |
| `bucket()` | N | Y |
| `bucket_count()` | N | Y |
| `bucket_size()` | N | Y |
| `cbegin()` | Y | Y |
| `cbegin(n)` | N | Y |
| `cend()` | Y | Y |
| `cend(n)` | N | Y |
| `clear()` | Y | Y |
| `contains()` | Y | Y |
| `count()` | Y | Y |
| `crbegin()` | Y | N |
| `crend()` | Y | N |
| `emplace()` | Y | Y |
| `emplace_hint()` | Y | Y |
| `empty()` | Y | Y |
| `end()` | Y | Y |
| `end(n)` | N | Y |
| `equal_range()` | Y | Y |
| `erase()` | Y | Y |
| `extract()` | Y | Y |
| `find()` | Y | Y |
| `insert()` | Y | Y |
| `insert_or_assign()` | Y | Y |
| `insert_range() (C++23)` | Y | Y |
| `iterator / const_iterator` | Y | Y |
| `load_factor()` | N | Y |
| `local_iterator / const_local_iterator` | N | Y |
| `lower_bound()` | Y | N |
| `max_bucket_count()` | N | Y |
| `max_load_factor()` | N | Y |
| `max_size()` | Y | Y |
| `merge()` | Y | Y |
| `operator[]` | Y | Y |
| `rbegin()` | Y | N |
| `rehash()` | N | Y |
| `rend()` | Y | N |
| `reserve()` | N | Y |
| `reverse_iterator / const_reverse_iterator` | Y | N |
| `size()` | Y | Y |
| `swap()` | Y | Y |
| `try_emplace()` | Y | Y |
| `upper_bound()` | Y | N |

### 平铺关联容器适配器
这里平铺是 `flat`，也有人翻译成平坦。

C++23 提供了四种适配器，分别是 `std::flat_set` `std::flat_multiset` `std::flat_map` `std::flat_multimap`。和之前讨论的数据结构类似。`flat_set` `flat_multiset` 需要一个顺序容器，`flat_map` `flat_multimap` 需要两个顺序容器，分别保存 `key` `value`。要求容器支持随机访问，因此只能是 `deque` 和 `vector`，默认是后者。

适配器存储并不是基于节点存储，没有节点的概念和相关函数。同时，提供随机访问迭代器。

平铺适配器添加和删除元素较慢，线性复杂度，查询是对数时间，和非平铺复杂度一致。不过平铺容器迭代更高效，cache 友好，同时内存使用量更低。

## 其他容器
字符串 `std::string` 可以看作是容器，和 `vector` 更类似，提供了 `insert()` `push_back()` `erase()` `size()` `reserve()` `capacity()` 等函数。

`std::bitset` 和容器有相似之处。

`bitset` 是固定长度的一系列比特。C++23 开始，这个类是 `constexpr`，因此可以用于编译期。

`bitset` 模板参数需要指定长度，默认构造函数会将所有比特初始化成零，也可以从一个包含 `0` `1` 的 `string` 构造。

`set()` `reset()` `flip()` 分别是设置、清除和反转比特。`operator[]` 也可以用于读写比特。`flip()` 与 `operator~` 等价。`to_string()` 返回由 `0` `1` 组成的 `string`，注意，第 0 个比特在字符串最后。

`bitset` 表示一系列比特，因此支持 `&` `|` `^` `~` `<<` `>>` `&=` `|=` `^=` `<<=` `>>=` 这些比特操作。
