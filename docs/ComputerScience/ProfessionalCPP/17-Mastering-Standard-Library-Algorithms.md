## 算法概述
算法基于迭代器而不是具体的容器。算法对迭代器的类型有要求，比如 `copy_backward()` 从一个容器中逆序拷贝元素到另一个容器，需要双向迭代器，`stable_sort()` 是稳定排序算法，需要随机访问迭代器，那么 `forward_list` 仅能向前迭代，不能传给上述两个算法。大部分的算法是 `constexpr` 的，因此可以用于编译期。

`std::find()` `std::find_if()` 从一对迭代器中，找到第一个等于的元素或者是满足传入的谓词的元素。对于后者，C++23 开始可以传入一个函子。由于 C++23 开始类的 `operator()` 可以是 `static`，因此用起来相当方便。

`accumulate()` 很强大，默认接受一对迭代器和一个初始值，然后求和，还可以传入一个算子，那么就可以自定义累积的方式。

算法会尽可能利用移动语义加速算法，因此实现移动拷贝和赋值，同时标记为 `noexcept` 能有性能提升。

传入算法的回调函数对象可能会被拷贝多次，因此大部分算法要求回调函数是无状态的，`const`。`generate()` `generate_n()` 是个例外，接受有状态的回调。如果不想算法拷贝，通过 `std::ref()` 传入函数对象的引用。

## 算法细节
### 只读序列算法
除了上面的 `find()` `find_if()` 之外，还有很多类似的查找算法。这些算法使用 `operator==` `operator<` 作为默认比较运算符，也可以提供一个自定义的。`find_if_not()`，找到第一个不满足条件的元素；`adjacent_find()` 返回连续相同的元素；`find_first_of()` 额外接受一对迭代器，返回第一个出现的元素，类似 SQL 中的 `in` 表达式；`search` 额外接受一对迭代器，要求序列相等；`find_end()`，额外接受一对迭代器，从后面查找序列相等的情况；`search_n()` 查找连续 `n` 个指定的元素。

如果容器有与这些泛型算法等价的实现，使用成员函数，因为这些与容器相关，速度会更快。

`search()` 可以选择不同的搜索算法，默认是 `default_searcher`，还有 `boyer_moore_searcher` 和 `boyer_moore_horspool_searcher`，使用的是 Boyer-Moore 和 Boyer-Moore-Horspool 算法，序列长度是 $N$，查找模式长度是 $M$，那么最差情况分别是 $O(N+M)$（没找到）和 $O(N*M)$（找到）。实际情况中可能比线性复杂度还低，因为会根据状态机太多一些字符。这两个算法的区别是后者初始化和每次迭代的常量开销更低，不过最差情况比较差。

算法提供了几个比较函数。`equal` 接受三个迭代器，第一对是第一个容器的起始迭代器，第三个迭代器是第二个容器的开始迭代器，如果所有元素都相等，返回 `true`，这要求两个序列元素个数一样。C++14 开始提供一个两队迭代器的重载版本，更安全，可以比较不同大小的序列，更推荐。`mismatch()` 返回两个迭代器，分别指向两个不同序列中第一个不同元素，和 `equal()` 类似，有两个版本的重载。`lexicographical_compare()`，如果第一个范围内第一个不相等的元素小于第二个范围对应的元素，或者都相等但是第一个范围元素更少，那么返回 `true`。`lexicographical_compare_three_way()` 类似，不过执行三路比较，返回比较类型（`strong_ordering` `weak_ordering` `partial_ordering`）。如果两个容器类型相同，那么使用 `operator=` `operator<` 进行比较就好。

`all_of()` `any_of()` `none_of()` 接受一个范围，使用传入的谓词进行比较。

`count()` `count_if()` 统计容器中元素的个数或满足条件的个数。

### 修改序列算法
修改序列的算法可以分成两类，一类是涉及两个序列 `source` 和 `destination`，从第一个序列读然后修改目标序列，第二类是就地修改，仅接受一个序列。

注意修改算法不创建也不拓展容器，是修改，因此使用前需要保证迭代器有效。

`generate()` 接受一对迭代器，调用回调函数多次，依次修改迭代器范围内的元素。

`transform()` 有两个重载版本。第一个接受一对迭代器，表示 `src`，一个迭代器表示 `dst`，一个回调函数，依次读取 `src` 元素，调用回调函数，将结果依次写到 `src`。如果 `src` `dst` 相同，那么就地修改。第二个版本接受一对迭代器，是 `src1`，再一个迭代器表示 `src2`，最后一个迭代器表示 `dst`，一个回调函数。依次读取两个 `src` 元素，调用回调函数，写到 `dst`。

`copy()` 函数从一个对象向另一个队列拷贝对象，范围可以有交集，比如 `copy(b, e, d)`，`d` 可以在 `b` 之前，不过在 `[b,e)` 之间行为未定义。再次声明，拷贝前需要确保 `d` 有足够的大小。`copy_backward()` 类似，不过逆序访问数据源。`copy_if()` 仅仅拷贝满足条件的，同时返回目标范围的下一个迭代器，这样很容易从这个迭代器开始删除元素。`copy_n()` 拷贝 `n` 个元素，这里不是范围，因此可能越界，需要自己保证数据源有长度足够。

`move()` `move_backward()` 与 `copy()` `copy_backward()` 类似，不过是移动元素到目标范围。

`replace()` `replace_if()` 将指定元素或满足条件的元素替换成某个指定元素。`replace_copy()` `replace_copy_if()` 将结果拷贝到目标范围。

`erase()` `erase_if()` 删除指定元素或满足条件的元素。它的参数是容器而不是迭代器对或范围。

`erase()` `erase_if()` C++20 提供的方法，在此之前，可以使用 `remove-erase` 模式，首先调用 `remove()` `remove_()` 移出元素，返回的是下一个位置的迭代器，然后调用 `erase()` 删除尾部元素。不建议直接用容器的 `erase()` 方法，因为（1）低效，每次删除需要挪动元素，平方时间复杂度；（2）要小心迭代器失效。`remove_copy()` `remove_copy_if()` 类似，不过不修改原始范围而是将结果拷贝到目标范围。

`unique()` 删除连续重复的元素，只保留一个，一般用于排序的容器，用于非排序的容器也不是不可以。`unique_copy()` 将结果拷贝到目标范围。

`shuffle()` 的参数是一对迭代器和一个随机数生成器，对范围内的数据洗牌改变分布。

`sample()` 接受一对迭代器，一个目标迭代器，采样数 `n` 和随机数生成器，会随机从序列中随机选择 `n` 个元素。

`reverse()` 翻转序列，`reverse_copy()` 不是就地翻转而是拷贝到目标迭代器。

`shift_left()` 向左移动序列，返回新的范围的 `end()`。`shift_right()` 向右移动序列，返回新的范围的 `begin()`。

### 操作算法
`std::for_each()` `std::for_each_n()` 遍历一对迭代器之间的元素或者是迭代器开始的 `n` 个元素，分别调用传入的回调函数，返回这个函子。

### 分区（`partition`）算法
`partition_copy()` 将源序列根据谓词结果分成两个部分，分别写到两个目标序列，返回值是一个迭代器对，分别是两个目标序列写入数据后的 `end()`。`partition()` 就地修改序列，将谓词返回 `true` 的元素排到谓词返回 `false` 的元素前面。

### 排序算法
`sort()` 算法对一对迭代器进行排序，可以自定义比较。`sort_stable()` 会保持相同元素的相对顺序不变。`is_sorted()` 返回一个序列是否排序了，`is_sorted_until()` 返回一个迭代器，在此之前的数据已经排好序了。

`nth_element()` 可以找到排序后处于第 `n` 个位置的元素，时间复杂度是线性的。第一个迭代器是序列的 `begin()`，第二个迭代器是排序后处于哪个位置，第三个迭代器是序列的 `end()`。

### 二分搜索算法
`binary_search()` `lower_bound` `upper_bound()` `equal_range()` 内部都是二分搜索算法，要求序列有序，用于不同的场景。第一个返回元素是否存在。第二个返回指向第一个不小于指定元素的迭代器。第三个返回最后一个不大于指定元素的迭代器。第四个返回一对迭代器，是给定元素的范围。

### 集合算法
`includes()` 判断一个有序范围是否包含另一个有序范围。

`set_union()` `set_intersection()` `set_difference()` `set_symmetric_difference()` 都是标准操作。分别求并集、交集、差集和对称差集。

` merge()` 实现了对两个有序序列的归并。

### 最大最小算法
`max()` `min()` 返回两个或多个元素中最大元素和最小元素。

`min_element()` `max_element()` 返回最小元素、最大元素对一个的迭代器。

`std::clamp()` 是一个挺有用的消防法，如果给定值在指定范围内，返回给定值，如果比最小值还小，返回最小值，如果比最大值还大，那么返回最大值。

### 并行算法
大部分的算法可以指定执行策略（`execution policy`），是算法的第一个桉树，可以选择是否并发和是否向量化。在 `std::execution` 命名空间下提供以下四种策略。

| EXECUTION POLICY TYPE | GLOBAL INSTANCE | DESCRIPTION |
|--|--|--|
| `sequenced_policy` | `seq` | The algorithm is not allowed to parallelize or vectorize its execution. |
| `parallel_policy` | `par` | The algorithm is allowed to parallelize but not vectorize its execution. |
| `parallel_unsequenced_policy` | `par_unseq` | The algorithm is allowed to parallelize and vectorize its execution. It's also allowed to migrate its execution across threads. |
| `unsequenced_policy` | `unseq` | The algorithm is allowed to vectorize but not parallelize its execution. |

这些并行需要自己处理竞争和死锁的问题。

对于 `parallel_unsequenced_policy` 和 `unsequenced_policy`，要生成向量化代码，对回调函数有要求。比如不能分配内存，获取互斥锁，有锁的 `std::atomic` 等等。

### 数据处理算法
前面分析的 `accumulate()` 就是一种数据处理的算法。

`iota()` 从指定数值开始，填充迭代器对内的元素，每填充一个调用 `operator++` 自增一次。

`reduce()` 和 `accumulate()` 类似，不过支持执行策略，同时默认运算符使用 `std::plus`。

`inner_product()` 计算两个序列的内积，默认算子是 `operator+` `operator*`。`transform_reduce()` 是类似的，支持执行策略，默认算子是 `std::plus` `std::multiplies`。

### 扫描算法
扫描算法也称为前缀和、部分和，有五种算法 `exclusive_scan()` `inclusive_scan()/partial_sum()` `transform_exclusive_scan()` `transform_inclusive_scan()`。

这里的 inclusive 与 exclusive 的区别是后者输出的第一个元素仅仅是初始值，而前者是初始值和第一个元素的计算结果。

### 受限算法
`std::ranges` 提供了大部分上述算法的版本，基于概念实现的，报错信息更友好，参数是 `std::range` 而不是一对迭代器。

C++23 开始提供了一些在 `std::ranges` 下独有的算法：`contains()` `contains_subrange()` `starts_with()` `ends_with()` `find_last()` `find_last_if()` `find_last_if_not()` `fold_left()` `fold_left_first()` `fold_right()` `fold_right_last()` `fold_left_with_iter()` `fold_left_first_with_iter()`。
