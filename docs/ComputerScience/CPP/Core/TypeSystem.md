
## `enum`
枚举类 `enum class` 改善了传统 C 语言中枚举的类型安全性问题，使得枚举值不会隐式转换为整数类型，并且不同枚举类的值不会相互混淆。

C++20 允许在局部使用 `using enum` 来引入枚举类的所有枚举值，从而在该作用域内可以直接使用枚举值而无需加上枚举类的限定。写代码更简洁、灵活，同时 `using` 限定在很小的作用域内，不会影响其他代码。
```cpp
enum class Color
{
	Red,
	Green,
	Blue
};

std::string ColorToString(Color color)
{
	using enum Color;
	switch (color)
	{
	case Red:
		return "Red";
	case Green:
		return "Green";
	case Blue:
		return "Blue";
	default:
		return "Unknown";
	}
}
```

## `volatile`
`volatile` 是一种类型限定符，用于告诉编译器该变量可能会在程序的其他部分被意外修改，因此每次访问该变量时都必须从内存中重新读取，而不能依赖寄存器中的缓存。这意味着对于单个执行的线程而言，编译器必须按照源代码中出现的次数，在可执行文件中如实地执行加载（`load`）和存储（`store`）操作。`volatile` 既不能被优化消除，也不能被重排。

`volatile` 用于避免编译器进行激进的优化，并且它本身没有多线程并发语义。因此 C++20 保留了其核心作用，废弃（`deprecate`）了容易引起混淆和误用的用法：

- 废弃了复合赋值运算符、前缀和后缀的自增与自减操作
- 废弃了在函数参数和返回值中使用 `volatile` 修饰
- 废弃结构化绑定中的 `volatile` 修饰

下面的代码说明了这三种情况。
```cpp
int main()
{
	// (1)
	int neck, tail;
	volatile int brachiosaur;
	brachiosaur = neck; // OK, a volatile store
	tail = brachiosaur; // OK, a volatile load

	// deprecated: does this access brachiosaur once or twice
	tail = brachiosaur = neck;
	// deprecated: does this access brachiosaur once or twice
	brachiosaur += neck;
	// OK, a volatile load, an addition, a volatile store
	brachiosaur = brachiosaur + neck;

	// (2)
	// deprecated: a volatile return type has no meaning
	volatile struct amber jurassic();

	// deprecated: volatile parameters aren't meaningful to the
	//             caller, volatile only applies within the function
	void trex(volatile short left_arm, volatile short right_arm);

	// OK, the pointer isn't volatile, the data it points to is
	void fly(volatile struct pterosaur * pterandon);

	// (3)
	struct linhenykus
	{
		volatile short forelimb;
	};

	void park(linhenykus alvarezsauroid)
	{
		// deprecated: does the binding copy the forelimbs?
		auto [what_is_this] = alvarezsauroid; // structured binding
	}

	return 0;
}
```
