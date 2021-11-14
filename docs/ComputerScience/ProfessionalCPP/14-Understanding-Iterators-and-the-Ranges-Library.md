## 迭代器
标准库使用迭代器模式作为访问容器的抽象接口。有六种迭代器，提供的操作和能力如下表所示。

| ITERATOR CATEGORY | OPERATIONS REQUIRED | COMMENTS |
|--|--|--|
| Input (also known as Read) | `operator++`, `*`, `->`, `=`, `==`, `!=` <br> copy constructor | Provides read-only access, forward only (no `operator--` to move backward). <br> Iterators can be assigned, copied, and
compared for equality. |
| Output (also known as Write) | `operator++`, `*`, <br> copy constructor | Provides write-only access, forward only. <br> Iterators can be assigned, but cannot be compared for equality. <br> Specific to output iterators is that you can do `*iter = value`. <br> Note the absence of `operator->`. <br> Provides both prefix and postfix `operator++`. |
| Forward | Capabilities of input iterators, plus default constructor | Provides read access, forward only. <br> Iterators can be assigned, copied, and compared for equality. |
| Bidirectional | Capabilities of forward iterators, plus `operator--` | Provides everything a forward iterator provides. <br> Iterators can also move backward to a previous element. <br> Provides both prefix and postfix `operator--`. |
| Random access | Bidirectional capability, plus the following: <br> `operator+`, `-`, `+=`, `-=`, `<`, `>`, `<=`, `>=`, `[]` | Equivalent to raw pointers: support pointer arithmetic, array index syntax, and all forms of comparison. |
| Contiguous | Random-access capability and logically adjacent elements of the container must be physically adjacent in memory | Examples of this are iterators of `std::array`, `vector` (not `vector<bool>`), `string` and `string_view`. |

这些迭代器之间没有继承关系，不过除了 Output 之外，能力是顺序增加的。算法的接口往往指定迭代器类型，模板类型参数分别是 `InputIterator` `OutputIterator` `ForwardIterator` `BidirectionalIterator` `RandomAccessIterator` `ContiguousIterator`，不过并不对类型本身做校验。因此一个算法需要随机访问迭代器，但是传入了双向迭代器，那么会报错，而且很可能报错信息离真实错误距离相当远。

范围（`range`）类库有类型限制，报错信息会更清晰一些。

容器提供四种迭代器：`iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator`，区别是正向和反向迭代，是否是 `const`。有四组成员函数 `begin()` `end()` `cbegin()` `cend()` `rbegin()` `rend()` `crbegin()` `crend()` 得到对应的迭代器。也有四组非成员函数。迭代器 `begin()` 和 `end()` 是左闭右开区间，好处是可以容易表示空集合。

有时我们想临时存一下数据需要知道迭代器引用的元素类型，有时可能需要知道迭代器类型，是随机访问迭代器还是双向迭代器。STL 提供了特征类 `std::iterator_traits<T>` 可以提供这些，这个类提供下面几个类型：`value_type` 引用的元素的类型，`difference_type` 表示两个迭代器距离的类型，`iterator_category` 迭代器类型，具体值有 `input_iterator_tag` `output_iterator_tag`
`forward_iterator_tag` `bidirectional_iterator_tag` `random_access_iterator_tag` `contiguous_iterator_tag`，`pointer` 元素的指针类型，`reference` 元素的引用类型。

通过迭代器类型可以实现函数分发，以调用到最匹配的函数。比如下面的例子。
```cpp
#include <iterator>

template<typename Iter, typename Distance>
void
advanceHelper(Iter &iter, Distance n, std::input_iterator_tag)
{
	while (n > 0)
	{
		++iter;
		--n;
	}
}

template<typename Iter, typename Distance>
void
advanceHelper(Iter &iter, Distance n, std::bidirectional_iterator_tag)
{
	while (n > 0)
	{
		++iter;
		--n;
	}
	while (n < 0)
	{
		--iter;
		++n;
	}
}

template<typename Iter, typename Distance>
void
advanceHelper(Iter &iter, Distance n, std::random_access_iterator_tag)
{
	iter += n;
}

template<typename Iter, typename Distance>
void
myAdvance(Iter &iter, Distance n)
{
	using category = typename std::iterator_traits<Iter>::iterator_category;
	advanceHelper(iter, n, category {});
}
```
当有了概念之后，上面的例子可以更直观简洁一些。
```cpp
#include <iterator>

template<std::input_iterator Iter, typename Distance>
void
myAdvance(Iter &iter, Distance n)
{
	while (n > 0)
	{
		++iter;
		--n;
	}
}

template<std::bidirectional_iterator Iter, typename Distance>
void
myAdvance(Iter &iter, Distance n)
{
	while (n > 0)
	{
		++iter;
		--n;
	}
	while (n < 0)
	{
		--iter;
		++n;
	}
}

template<std::random_access_iterator Iter, typename Distance>
void
myAdvance(Iter &iter, Distance n)
{
	iter += n;
}
```

## 流迭代器
有四种流迭代器。

* `ostream_iterator`: Output iterator writing to a `basic_ostream`
* `istream_iterator`: Input iterator reading from a `basic_istream`
* `ostreambuf_iterator`: Output iterator writing to a `basic_streambuf`
* `istreambuf_iterator`: Input iterator reading from a `basic_streambuf`

下面是 `ostream_iterator` 和 `istream_iterator` 的例子，注意 `istream_iterator` 默认构造的是表示结束的迭代器。
```cpp
template<std::input_iterator InputIter,
		 std::output_iterator<std::iter_reference_t<InputIter>> OutputIter>
void
Copy(InputIter begin, InputIter end, OutputIter target)
{
	for (auto iter { begin }; iter != end; ++iter, ++target)
	{
		*target = *iter;
	}
}

std::vector myVector { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
Copy(std::cbegin(myVector), std::cend(myVector), std::ostream_iterator<int> { std::cout, " " });
```

```cpp
template<std::input_iterator InputIter>
auto
Sum(InputIter begin, InputIter end)
{
	auto sum { *begin };
	for (auto iter { ++begin }; iter != end; ++iter)
	{
		sum += *iter;
	}

	return sum;
}

std::istream_iterator<int> numbersIter { std::cin };
std::istream_iterator<int> endIter;
int result { Sum(numbersIter, endIter) };
```

## 迭代适配器
迭代器分成两类，一类是以容器为参数构造，有以下三个。

* `back_insert_iterator`: Uses `push_back()` to insert elements into a container
* `front_insert_iterator`: Uses `push_front()` to insert elements into a container
* `insert_iterator`: Uses `insert()` to insert elements into a container

第二类是以其他迭代器为参数，有如下两个。

* `reverse_iterator`: Reverse the iteration order of another iterator.
* `move_iterator`: The dereferencing operator for a `move_iterator` automatically converts the value to an rvalue reference, so it can be moved to a new destination.

三个插入迭代器的使用例子如下，`std::back_inserter()` 可以方便创建 `std::back_insert_iterator` 对象。
```cpp
std::vector vectorOne { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
std::vector<int> vectorTwo;
std::back_insert_iterator<std::vector<int>> inserter { vectorTwo };
Copy(std::cbegin(vectorOne), std::cend(vectorOne), inserter);

Copy(std::cbegin(vectorOne), std::cend(vectorOne), std::back_inserter(vectorTwo));
```

`std::make_move_iterator()` 用一个迭代器构造出移动迭代器，当解引用的时候，会移动对象到另一个容器或者需要的地方，那么当前容器中的对象将不再可用。

## 范围 `range`
范围提供了一系列算法，基本上 STL 提供的，范围都提供了，好处是调用更简洁，同时报错信息更友好。另外，范围也提供几种迭代器，比如 `std::ranges::contiguous_range`。
```cpp
std::vector data { 33, 11, 22 };
std::sort(std::begin(data), std::end(data));

std::ranges::sort(data);
```
投影机制也能使代码更简洁。比如下面的 `Person` 类没有 `operator<`，使用 `sort` 需要一个比较 lambda 或者等价的可调用对象，使用范围的话可以使用投影，而投影的类型有 `std::ranges::less` 即可。
```cpp
class Person
{
public:
	explicit Person(std::string first, std::string last)
		: firstName_ { std::move(first) }
		, lastName_ { std::move(last) }
	{
	}

	const std::string &
	GetFirstName() const
	{
		return firstName_;
	}

	const std::string &
	GetLastName() const
	{
		return lastName_;
	}

private:
	std::string firstName_;
	std::string lastName_;
};

std::ranges::sort(persons,
				  {},
				  [](const Person &person)
				  {
					  return person.GetFirstName();
				  });

std::ranges::sort(persons, {}, &Person::GetFirstName);
```

范围提供视图（`view`）功能，特点是延迟求值，不持有任何元素，不修改任何内容。视图可以通过 `|` 连接起来。视图本身是范围，但是不是所有范围都是视图，后者的话需要用范围适配器来创建视图。下表是一系列适配器。第一列的名字分别位于 `std::ranges` 和 `std::ranges::views` 两个命名空间，`std::ranges::views` 有个别名是 `std::views`。构造范围适配器的方式可以是 `std::ranges::operation_view { range, arguments... }`，也可以是 `range | std::ranges::views::operation(arguments...)`。从 `as_const_view` 开始是 C++23 新增的。

| RANGE ADAPTER | DESCRIPTION |
|--|--|
| `views::all` | Creates a view that includes all elements of a range. |
| `filter_view` <br> `views::filter` | Filters the elements of an underlying sequence based on a given predicate. If the predicate returns `true`, the element is kept, otherwise it is skipped. |
| `transform_view` <br> `views::transform` | Applies a callback to each element of an underlying sequence to transform the element to some other value, possibly of a different type. |
| `take_view` <br> `views::take` | Creates a view of the first `n` elements of another view. |
| `take_while_view` <br> `views::take_while` | Creates a view of the initial elements of an underlying sequence until an element is reached for which a given predicate returns `false`. |
| `drop_view` <br> `views::drop` | Creates a view by dropping the first `n` elements of another view. |
| `drop_while_view` <br> `views::drop_while` | Creates a view by dropping all initial elements of an underlying sequence until an element is reached for which a given predicate returns `false`. | 
| `join_view` <br> `views::join` | Flattens a view of ranges into a view. For example, flatten a vector<vector<int>> into a vector<int>. |
| `lazy_split_view` <br> `views::lazy_split` <br> `split_view` <br> `views::split` | Given a delimiter, splits a given view into subranges on the delimiter. The delimiter can be a single element or a view of elements. |
| `reverse_view` <br> `views::reverse` | Creates a view that iterates over the elements of another view in reverse order. The view must be a bidirectional view. |
| `elements_view` <br> `views::elements` | Requires a view of tuple-like elements, creates a view of the `n`th elements of the tuple-like elements. |
| `keys_view` <br> `views::keys` | Requires a view of pair-like elements, creates a view of the first element of each pair. |
| `values_view` <br> `views::values` | Requires a view of pair-like elements, creates a view of the second element of each pair. |
| `common_view` <br> `views::common` | Depending on the type of range, `begin()` and `end()` might return different types, such as a begin iterator and an end sentinel. This means that you cannot, for example, pass such an iterator pair to functions that expect them to be of the same type. common_view can be used to convert such a range to a common range which is a range for which `begin()` and `end()` return the same type. |
| `as_const_view` <br> `views::as_const` | Creates a view through which the elements of an underlying sequence cannot be modified. |
| `as_rvalue_view` <br> `views::as_rvalue` | Creates a view of rvalues of all elements of an underlying sequence. |
| `enumerate_view` <br> `views::enumerate` | Creates a view where each element represents the position and value of all elements of an underlying sequence. |
| `zip_view` <br> `views::zip` | Creates a view consisting of tuples of reference to corresponding elements of all given views. |
| `zip_transform_view` <br> `views::zip_transform` | Creates a view whose `i`th element is the result of applying a given callable to the `i`th elements of all given views. |
| `adjacent_view` <br> `views::adjacent` | For a given `n`, creates a view whose `i`th element is a tuple of references to the `i`th through `(i + n - 1)`th elements of a given view. |
| `adjacent_transform_view` <br> `views::adjacent_transform` | For a given `n`, creates a view whose `i`th element is the result of applying a given callable to the `i`th through `(i + n - 1)`th elements of a given view. |
| `views::pairwise` <br> `views::pairwise_transform` | Helper types representing `views::adjacent<2>` and `views::adjacent_transform<2>` respectively. |
| `join_with_view` <br> `views::join_with` | Given a delimiter, flattens the elements of a given view, inserting every element of the delimiter in between elements of the view. The delimiter can be a single element or a view of elements. |
| `stride_view` <br> `views::stride` | For a given `n`, creates a view of an underlying sequence, advancing over `n` elements at a time, instead of one by one. |
| `slide_view` <br> `views::slide` | For a given `n`, creates a view whose `i`th element is a view over the `i`th through `(i + n - 1)`th elements of the original view. Similar to `views::adjacent`, but the window size, `n`, is a runtime parameter for slide, while it's a template argument for adjacent. |
| `chunk_view` <br> `views::chunk` | For a given `n`, creates a range of views that are `n`-sized nonoverlapping successive chunks of the elements of the original view, in order. |
| `chunk_by_view` <br> `views::chunk_by` | Splits a given view into subranges between each pair of adjacent elements for which a given predicate returns `false`. |
| `cartesian_product_view` <br> `views::cartesian_product` | Given a number of ranges, `n`, creates a view of tuples calculated by the `n`-ary cartesian product of the provided ranges. |

标准库提供几个范围工厂方法，如下所示。

| RANGE FACTORY | DESCRIPTION |
|--|--|
| `empty_view` | Creates an empty view. |
| `single_view` | Creates a view with a single given element. |
| `iota_view` | Creates an infinite or a bounded view containing elements starting with an initial value, and where each subsequent element has a value equal to the value of the previous element incremented by one. |
| `repeat_view` | Creates a view that repeats a given value. The resulting view can be unbounded (infinite) or bounded by a given number of values. |
| `basic_istream_view` <br> `istream_view` | Creates a view containing elements retrieved by calling the extraction operator, `operator>>`, on an underlying input stream. |

下面是两个用例。`iota_view` 返回无限长度的范围，但是由于惰性求值，能够正常工作。第二个例子是从 `cin` 中构造 `istream_view`。
```cpp
auto result { views::iota(10) |
			  views::filter(
				  [](const auto &value)
				  {
					  return value % 2 == 0;
				  }) |
			  views::transform(
				  [](const auto &value)
				  {
					  return value * 2.0;
				  }) |
			  views::take(10) };

auto values = ranges::istream_view<int> { cin } |
			  views::take_while(
				  [](const auto &v)
				  {
					  return v < 5;
				  }) |
			  views::transform(
				  [](const auto &v)
				  {
					  return v * 2;
				  });
```

C++23 引入了 `std::ranges::to()` 这个函数，可以方便的从一个范围转成一个容器。同时，还为容器增加了一系列函数，用于范围和容器进行互操作，这些成员函数名字模式是 `xxx_range()`，其中 `xxx` 可以是 `insert` `append` `prepend` `assign` `replace` `push` `push_front` `push_back`。
