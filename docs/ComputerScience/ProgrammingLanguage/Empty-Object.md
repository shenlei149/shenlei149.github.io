智能指针 `unique_ptr` 的大小是多少？

为了处理任意自定义情况，`unique_ptr` 包含两个模板参数：
```cpp
template<
    class T,
    class Deleter = std::default_delete<T>
> class unique_ptr;
```

最朴素的实现，`unique_ptr` 包含两个成员变量，类型分别是 `T*, Deleter`。

`T*` 大小是 8 字节，`Deleter` 大小是多少呢？这个类是无状态的，没有成员变量，大小可以为零，但是实际上并不是这样。

> C++ 规定一个空类至少有占用一个字节，目的是声明两个空类对象，这两个对象都有唯一的内存地址。
```
class Empty
{
};

Empty e1;
Empty e2;
static_assert(&e1 != &e2);
```

那么 `Deleter` 至少也要有一个字节大小，那么 `unique_ptr` 至少占用 9 个字节，加上填充，很可能是 16 个字节，相比裸指针，这浪费了一倍的空间。

如何优化呢？利用下面这条规定：
> 基类子对象的大小可以为零

也就是说，如果一个类继承了一个空类，那么基类可以被优化成占用零字节。继承类可能还是空类，至少一字节，或者有成员变量，不管是那种情况，继承类都不违背上一条规定。
```cpp
class Sub : public Empty
{
	int x;
};

static_assert(sizeof(Sub) == 4);
```

那么可以使用继承类这个优化来优化 `unique_ptr` 空间。不管使用什么结构存放 `T*, Deleter`，比如 `compressed_pair` 或者 `tuple`，对于第二个模板参数，不要放一个额外的成员变量，没有必要，而是以继承的方式 `: public Deleter` 实现，`Deleter` 自身需要一个字节，但是继承类由于有其他变量，就可以把这一个字节优化掉。

所以
```cpp
static_assert(sizeof(std::unique_ptr<int>) == 8);
```

C++20 新增了 `[[no_unique_address]]` 这个属性，目的就是直接解决空类必须占用至少一个字节的问题。这样 `unique_ptr` 的实现就能简化很多，不用弯弯绕，直接声明成员变量保存 `Deleter` 即可，同时加上这个属性。

> 使用 `[[no_unique_address]]` 避免无状态的成员变量占用额外空间
