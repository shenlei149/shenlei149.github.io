## 函数指针
在 C+++ 中，函数可以像数据一样使用，即使用指向函数的指针。下面声明了一个接受两个 `int` 返回 `bool` 的函数指针，这里 `*fun` 需要括号，否则会变成一个函数原型声明。
```cpp
bool (*fun)(int, int);
```
可以用类型别名来声明函数指针类型。
```cpp
using Matcher = bool (*)(int, int);
using MatchHandler = void (*)(size_t, int, int);
```
一个函数，接受函数作为参数或者返回一个函数，称为高阶函数（`higher-order function`）。下面就是一个使用函数的函数。
```cpp
void
findMatches(std::span<const int> values1,
			std::span<const int> values2,
			Matcher matcher,
			MatchHandler handler)
{
}
```
上面的函数也可以实现为模板函数，通过概念来约束模板参数，或者直接使用 `auto`。
```cpp
template<std::predicate<int, int> Matcher, std::invocable<size_t, int, int> MatchHandler>
void
findMatches(std::span<const int> values1,
			std::span<const int> values2,
			Matcher matcher,
			MatchHandler handler)
{
}

void
findMatches(std::span<const int> values1,
			std::span<const int> values2,
			std::predicate<int, int> auto matcher,
			std::invocable<size_t, int, int> auto handler)
{
}
```

## 函数对象
一个类，如果重载了 `operator()` 那么称为函数对象或函子（`functor`）。

标准库提供了很多有用的函子，在需要可调用对象的地方均可使用。算术相关的函子有 `plus` `minus` `multiplies` `divides` `modulus` `negate`，这些都是模板参数，需要提供类型，这些函子是算术运算的封装，需要指定的类型支持算数运算，比如 `+`。

这些函子可以透明化，即不传递类型，这样根据上下文确定类型。比如 `multiplies<>` 而不是 `multiplies<int>`。推荐使用这种形式。

标准库还提供六种比较函子：`equal_to` `not_equal_to` `less` `greater` `less_equal` `greater_equal`。逻辑函子三个：`logical_not` `logical_and` `logical_or`。四种比特运算：`bit_and` `bit_or` `bit_xor` `bit_not`。

标准库还提供函数对象适配器。比如 `not_fn()` 是将一个函数的语义取反。`mem_fn()` 将成员函数指针视为普通函数指针。另外还有 `std::bind`。不过 `std::bind` 完全可以用 lambda 表达式替代，使用 `mem_fn()` 的地方也推荐使用 lambda 表达式。

## 多态函数包装器
标准库提供两个多态函数包装器 `std::function` `std::move_only_function`，可以是任意可调用对象（函数、函数对象、lambda 表达式）的封装。

`std::function` 的语法如下
```cpp
std::function<R(ArgTypes...)>
```
任意参数列表与 `ArgTypes...` 兼容、返回值类型可以转化成 `R` 的可调用对象都可以初始化 `std::function`。`std::function` 要求对象可以拷贝，但是有时候函数对象包含只能移动的成员，那么此时需要使用 `std::move_only_function`。

## lambda 表达式
在很多场景，使用 lambda 表达式很方便。lambda 表达式以 `[]` 开始，这是 lambda `introducer`，后面跟着 `{}` 是表达式体。下面是一个最简单的 lambda 表达式。
```cpp
auto basicLambda { []
				   {
					   std::println("Hello from Lambda");
				   } };
basicLambda();
```
编译器会自动生成一个对应的函数对象，称为 lambda 闭包（`closure`）。默认 `operator()` 会标记 `const`，返回类型是 `auto`。
```cpp
class CompilerGeneratedName
{
public:
	auto
	operator()() const
	{
		std::println("Hello from Lambda");
	}
};
```
lambda 可以接受参数，那么闭包中 `operator()` 也就有参数列表。lambda 表达式使用尾部返回类型（`trailing return type`） `-> T`，也可以写作 `auto` 自行推断，不过 `auto` 会去掉引用和 `const` 修饰，此时需要用 `decltype(auto)`。

lambda 最重要的一个能力是能够捕获参数，写在开始的 `[]` 内。被捕获的参数列表就是编译器生成的闭包类的成员变量。由于 `operator()` 是 `const` 的，因此如果要修改捕获的变量，即类的成员变量，需要将 lambda 后面标记上 `mutable`。默认情况下按值捕获，拷贝一次。还支持按引用捕获，比如 `[&data]`。`[=]` `[&]` 分别是所有变量按值和按引用捕获，在之后，可以各个变量自己修改捕获方式，比如 `[=,&data]`，`data` 按引用捕获，其他 lambda 表达式中用到的外部变量按值捕获。`[this]` 按值捕获指针，本质拷贝了指针罢了，`[*this]` 拷贝了对象到表达式内。成员变量不能捕获，当捕获了 `this` 后，可以访问所有 `public` `protected` `private` 的成员函数和变量。不推荐默认捕获，因为后续会更新，可能无意捕获了新变量，但是捕获的方式不符合预期。注意，全局变量总是按引用捕获，且不能修改。

lambda 最常用的场景是需要传入函数的地方。比如 `std::invoke()` 就可以传入 lambda 表达式。另外 C++23 支持  `std::invoke_r()` 可以指定返回类型。与参数类似，在返回函数的地方，可以返回 lambda 表达式。

lambda 表达式可以是泛型的，参数指定为 `auto` `auto &` `auto *`，此时生成的闭包类的 `operator()` 是一个模板函数。

lambda 捕获时支持表达式。第一个作用是可以创建一个不存在的变量；第二个作用是通过 `std::move()` 捕获只能移动的对象。

lambda 表达式支持模板，在 `[]` 后使用 `<typename>` 来声明模板类型，其余使用与普通函数无异。

C++23 开始 lambda 表达式支持递归。
