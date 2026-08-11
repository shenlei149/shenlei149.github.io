## 动态内存分配
现代 C++ 代码中应该尽可能避免底层的内存分配，比如 `new` `new[]` `delete` `delete[]`，在涉及生命周期的时候避免使用裸指针，避免 C 风格的构造和相关的函数。使用更安全的 C++ 替代项，比如类似 C++ 的 `std::string` 类自动管理内存，使用只能指针等等。

下面动态分配回收内存的示例，如果只分配不回收就会导致内存泄漏。
```cpp
int *ptr { nullptr };
ptr = new int;

int *ptr { new int };

delete ptr;
ptr = nullptr;
```
每次声明指针，不要不初始化，设置成 `nullptr` 或合适的值。删除指针之后，设置成 `nullptr` 也是好习惯。这样后续不太可能被误用，`delete nullptr` 也没任何副作用，因为什么也不会做，但是 double free 会出问题。

在 C++ 代码中，C 风格的 `malloc` 和 `free` 就更不推荐使用了，因为 `new` `delete` 不仅仅分配内存，还会构造和析构对象，使对象可用（状态符合预期），申请、释放必要的资源。`realloc` 可能会分配新空间而不是在尾部扩展内存，然后拷贝原始数据过去，和 `malloc` 问题一样，拷贝过去并没有调用构造函数。

C++ 中推荐使用统一初始化列表来初始化数据，当然，更推荐使用 `std::array`，也依旧推荐使用初始化列表。
```cpp
int myArray[5] { 1, 2, 3, 4, 5 };
int myArray[5] { 1, 2, 3, 4, 5 };
int myArray[5] {}; // 0, 0, 0, 0, 0
int myArray[] { 1, 2, 3, 4, 5 };
```
动态创建和删除数组。
```cpp
int *myArrayPtr {
	new int[] { 1, 2, 3, 4, 5 }
};

delete[] myArrayPtr;
myArrayPtr = nullptr;
```

如果数组元素类型是类类型，那么 `new` 时会调用（默认）构造函数，`delete` 会调用析构函数。

编译期决定大小的多维数组如下。
```cpp
char board[3][3] {};

board[2][1] = 'O'; // O puts marker in position (2,1)
```
但是运行时决定大小的多维数组就稍微难用一些了。
```cpp
char **board { new char *[x] }; // Allocate first dimension
for (size_t i { 0 }; i < x; ++i)
{
	myArray[i] = new char[y]; // Allocate ith subarray
}

for (size_t i { 0 }; i < x; ++i)
{
	delete[] board[i]; // Delete ith subarray
	board[i] = nullptr;
}

delete[] board; // Delete first dimension
board = nullptr;
```

这样使用二维数组有一个问题，次级数组在内存中不连续。一个替代办法是申请 `x * y` 这么多元素的一维数组，然后访问时候使用 `i * y + j` 访问。也可以利用 C++ 模板写一个 `Table<T>` 或 `Matrix<T>` 封装这些操作，使之更加好用。

## 数组与指针
数组会退化成指针，比如下面两个例子，第二个例子还使用了指针运算（向前或者向后移动若干个对象）。
```cpp
int myIntArray[10] {};
int *myIntPtr { myIntArray };
// Access the array through the pointer.
myIntPtr[4] = 5;

void doubleInts(int *theArray, size_t size);
doubleInts(myIntArray, 10);
doubleInts(myIntArray + 4, 6);
```
但是不是所有的指针都是数组，当然，也可以看作是长度为 1 的数组，不过这么写是代码充满了迷惑性，还容易出错。
```cpp
int *ptr { new int };

ptr[0] // ok
```

## 内存陷阱
下面总结一些导致 bugs 的用法，这也说明我们尽可能要避免这些用法，C++ 语言提供了更好的机制来管理内存，STL 也提供了更易用的组件，要善于用更好的工具。

使用了未分配足够空间的缓冲区，往往后导致缓冲区溢出，然后将数据写到了不该写的地方，破坏了其他数据。

内存泄漏，申请内存忘记释放了。有的时候是代码写的相当隐晦导致有 `delete` 但是行为依旧不对。

双重删除一个指针，会导致未定义行为，程序可能正常也可能会出错，而且问题相当难排查，因为现象可能是不固定的，且距离有双重删除 bug 的地方相当远。

## 智能指针
智能指针能够动态分配内存，还能在不需要时自动释放，避免内存泄漏。

C++ 提供两种智能指针，独占所有权（`unique ownership`）时使用 `std::unique_ptr`，共享所有权（`shared ownership`）时使用 `std::shared_ptr`。默认使用 `std::unique_ptr`，当真的需要共享资源的时候再使用 `std::shared_ptr`。

`std::unique_ptr` 在销毁或重置的时候释放资源。在一个函数中有多个返回值或者可能有异常时很好用，不用担心某个分支漏掉释放资源。

推荐使用 `std::make_unique<>()` 构造 `std::unique_ptr` 而不是先构造裸指针再传入其构造函数，除非需要使用自定义删除器。

`std::unique_ptr` 的使用和裸指针几乎没有差异，其他一些常用函数如下所示。
```cpp
uniquePtr->callFunc();

uniquePtr.get();			   // get raw pointer
uniquePtr.reset();			   // free resource and set to nullptr
uniquePtr.reset(new Object()); // free resource and set to a new instance

auto rawPtr = uniquePtr.release(); // release ownership and set to nullptr

// use the rawPtr then...
delete rawPtr;
rawPtr = nullptr;
```
`std::unique_ptr` 不支持复制只支持移动。比如
```cpp
void Func(std::unique_prt<int> data);

Func(std::move(uniquePtr));
```

`std::unique_ptr` 支持自定义自定义删除器。当和第三方 C 类库交互时可能能用上，比如类库需要调用 `c_alloc` `c_free` 来管理对象，那么 C++ 可以如下使用 `std::unique_ptr` 来做到自动管理。
```cpp
std::unique_ptr<T, decltype(&c_free)> uniquePtr { c_alloc(), c_free };
```
第二个模板参数是 `c_free` 指针函数的类型。

用这种方法，可以管理除了内存外的其他资源，比如自动关闭文件或者网络连接等。

如果需要共享所有权，那么使用 `std::shared_ptr`。和 `std::unique_ptr` 类似，推荐使用 `std::make_shared<T>()` 构造 `std::shared_ptr` 对象。使用也类似，不过 `reset()` 函数略有差别，只有在最后持有资源的 `std::shared_ptr` 被重置或者销毁时才会释放资源。另外，由于不是独占所有权，因此没有 `release()` 函数。另外，可以使用 `use_count()` 函数查看有多少个 `std::shared_ptr` 在贡献同一个资源。

`std::shared_ptr` 也可以自定义删除器，且比 `std::unique_ptr` 方便使用一点，这是因为底层机制不同。
```cpp
shared_ptr<int> smartPtr { c_alloc(42), c_free };
```

`std::shared_ptr` 使用引用计数（`reference counting`）来实现在最后一个实例销毁时释放资源。当构造一个新的 `std::shared_ptr` 实例，引用计数原子自增，`std::shared_ptr` 实例销毁或者重置时，引用计数原子自减。`std::shared_ptr` 可以正确使用可以解决双重删除的问题，但是一旦智能指针和裸指针混用，仍旧会有这个问题。

`std::shared_ptr` 也可以进行类型转化，转化函数是 ` const_pointer_cast()` `dynamic_pointer_cast()` `static_pointer_cast()` `reinterpret_pointer_cast()`，与不带 `pointer` 的版本使用类似。

`std::shared_ptr` 支持别名，可以通过所拥有的指针使得 `std::shared_ptr` 与另一个 `std::shared_ptr` 共享所有权，而这两个 `std::shared_ptr` 指向不同对象。下面的例子中，`foo` 和 `aliasing` 的 `use_count()` 返回都是 2，因此只有两个 `std::shared_ptr` 都销毁时，`Foo` 实例才会被销毁。
```cpp
class Foo
{
public:
	Foo(int value)
		: data_ { value }
	{
	}

	int data_;
};

auto foo { std::make_shared<Foo>(42) };
auto aliasing {
	std::shared_ptr<int> { foo, &foo->data_ }
};
```

`std::weak_ptr` 与 `std::shared_ptr` 类似，不过不拥有资源，也就是说不会对引用计数有影响。可以通过 `std::shared_ptr` 或 `std::weak_ptr` 构造一个 `std::weak_ptr` 对象。当想使用 `std::weak_ptr` 指向的对象时，可以 1）调用 `lock()` 方法得到一个 `std::shared_ptr`，如果对象不存在返回 `nullptr`，否则返回持有对象的 `std::shared_ptr`；2）用 `std::weak_ptr` 构造一个 `std::shared_ptr` 实例，不过如果资源已经释放了，会抛异常 `std::bad_weak_ptr`。

如果不涉及所有权的转移或者共享，函数参数就不应该是按值传递的智能指针类型，而是传递裸指针或者是 `const`/非 `const` 的引用，更推荐前者。如果智能指针作为返回值，正常写就行，借助于拷贝省略和移动语义，效率相当高。

一个类可以继承 `std::enable_shared_from_this` 类，主要为了解决从 `this` 返回 `std::shared_ptr` 的问题。`std::enable_shared_from_this` 有两个方法 `shared_from_this()` `weak_from_this()`。不过有一个前提，在调用这两个方法之前，需要通过 `std::make_shared` 或其他方法将对象放到一个 `std::shared_ptr` 中，然后用该对象实例调用上述方法得到智能指针，其他方法构造的对象调用上述方法会抛出 `std::bad_weak_ptr` 异常。

C++23 新增了一个和 C 风格函数交互的机制。C 风格函数往往通过返回错误码表示是否有错，那么附加的返回值只能通过出参实现。比如
```cpp
using errorcode = int;

errorcode
c_alloc(int value, int **data)
{
	*data = new int { value };
	return 0;
}

errorcode
c_free(int *data)
{
	delete data;
	return 0;
}
```
在 C++23 之前，不能直接使用智能指针作为出参，如果又想通过智能指针自动管理资源，那么只能如下这么做。
```cpp
std::unique_ptr<int, decltype(&c_free)> smartPtr(nullptr, c_free);
int *data { nullptr };
c_alloc(42, &data);
smartPtr.reset(data);
```

C++23 引入 `std::out_ptr()` `inout_ptr() ` 可以简化，如果确定传入指针是 `nullptr`，使用前者即可。
```cpp
std::unique_ptr<int, decltype(&c_free)> smartPtr(nullptr, c_free);
c_alloc(42, std::inout_ptr(smartPtr));
```
