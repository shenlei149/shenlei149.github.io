## 条件显式构造函数
有的时候我们的构造函数可以接受多种类型的参数，但是我们希望部分类型的实参能够隐式调用构造函数（因为语义足够直接），而其他类型则必须显式调用。同时，作为程序员，我们可能会偷懒并不想实现这些不同的构造函数，那么往往会采用模板来实现。C++20 引入了条件显式构造函数（`conditionally explicit constructor`），允许我们根据模板参数的特性来决定构造函数是否为显式的。比如下面的 `MyBool` 的例子，从 `bool` 隐式转换是很直接的，而从其他类型则必须显式调用。
```cpp
struct MyBool
{
	template<typename T>
	explicit(!std::is_same<T, bool>::value) MyBool(T t)
	{ // do something with t
	}
};
```
当 `explicit` 括号内的表达式 `!std::is_same<T, bool>::value` 值为 `true` 时，构造函数为显式的；为 `false` 时，构造函数为隐式的。也就是说，当 `T` 为 `bool` 时，构造函数可以隐式调用，而其他类型则必须显式调用。
