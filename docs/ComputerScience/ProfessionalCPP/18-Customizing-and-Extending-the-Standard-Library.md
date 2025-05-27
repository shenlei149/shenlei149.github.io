## 分配器
容器有一个模板参数，可以指定分配器。
```cpp
template<class T, class Allocator = allocator<T>>
class vector;
```
任意实现了 `allocate()` `deallocate()` 和其他必备的几个函数，那么就能作为自定义的分配器。

这里有一个问题，哪怕元素类型相同而使用不同分配器，那么是不同容器，不能相互赋值。为了解决这个问题，标准库 ` <memory_resource>` 引入了 `std::pmr::polymorphic_allocator`，其行为依赖于传入构造函数的 `memory_resource`。标准库提供了三种方式 `synchronized_pool_resource` `unsynchronized_pool_resource` `monotonic_buffer_resource`。

## 扩展标准库
我们可以实现自己的容器，提供了迭代器后，那么标准算法就可以使用自定义的容器了。反之，也可以基于迭代器实现自己的算法，以对标准库的元素进行计算。不过，这些都不能放到 `std` 命名空间。

下面自己实现了找到所有符合谓词的元素。
```cpp
template<std::forward_iterator ForwardIterator,
		 std::output_iterator<ForwardIterator> OutputIterator,
		 std::indirect_unary_predicate<ForwardIterator> Predicate>
OutputIterator
find_all(ForwardIterator first, ForwardIterator last, OutputIterator dest, Predicate pred)
{
	while (first != last)
	{
		if (invoke(pred, *first))
		{
			*dest = first;
			++dest;
		}
		++first;
	}
	return dest;
}
```

[这里](https://github.com/shenlei149/learn_professional_cpp/blob/main/src/DirectedGraph/DirectedGraph.h)是有向图 `DirectedGraph` 的实现。

为了让我们的容器更向标准库容器，需要提供以下类型别名。

| TYPE NAME | DESCRIPTION |
|--|--|
| `value_type` | The element type stored in the container |
| `reference` | A reference to the element type stored in the container |
| `const_reference` | A reference-to-`const` to the element type stored in the container |
| `iterator` | The type for iterating over elements of the container |
| `const_iterator` | A version of iterator for iterating over `const` elements of the container |
| `size_type` | A type that can represent the number of elements in the container; this is usually just `size_t` (from `<cstddef>`) |
| `difference_type` | A type that can represent the difference of two iterators for the container; this is usually just `ptrdiff_t` (from `<cstddef>`) |

| MEMBER FUNCTION | DESCRIPTION | WORST-CASE COMPLEXITY |
|--|--|--|
| Default constructor | Constructs an empty container | Constant |
| Copy constructor | Performs a deep copy of the container | Linear |
| Move constructor | Performs a move constructing operation | Constant |
| Copy assignment operator | Performs a deep copy of the container | Linear |
| Move assignment operator | Performs a move assignment operation | Constant |
| Destructor | Destroys any elements left in the container and frees their heap-allocated memory, if any | Linear |
| `iterator begin();` <br> `const_iterator begin() const;` | Returns an iterator or `const` iterator referring to the first element in the container | Constant |
| `iterator end();` <br> `const_iterator end() const;` | Returns an iterator or `const` iterator referring to one-past-the-last element in the container | Constant |
| `const_iterator cbegin() const;` | Same as `begin() const` | Constant |
| `const_iterator cend() const;` | Same as `end() const` | Constant |
| `operator==` | Comparison operator that compares two containers | Linear |
| `void swap(Container&) noexcept;` | Swaps the contents of the container passed to the member function with the object on which the member function is called | Constant |
| `size_type size() const;` | Returns the number of elements in the container | Constant |
| `size_type max_size() const;` | Returns the maximum number of elements the container can hold | Constant |
| `bool empty() const;` | Returns whether the container has any elements | Constant |

还需要支持反向迭代器。

| TYPE NAME | DESCRIPTION |
|--|--|
| `reverse_iterator` | The type for iterating over elements of the container in reverse order |
| `const_reverse_iterator` | A version of `reverse_iterator` for iterating over const elements of the container in reverse order |

还可以学习标准库，增加一些诸如 `assign()` `insert_range()` 这些方法。
