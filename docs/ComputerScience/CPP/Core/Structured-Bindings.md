C++17 引入了结构化绑定（`structured bindings`），可以把一个对象的多个元素或成员绑定到独立的名字上，从而简化代码。语法如下：
```cpp
auto [a, b, c, ...] = expression;
```
对于 `std::pair`、`std::tuple` 等 tuple-like 类型，编译器生成的代码大致如下：
```cpp
auto &&__tuple = expression;
auto &a = std::get<0>(__tuple);
auto &b = std::get<1>(__tuple);
auto &c = std::get<2>(__tuple);
```
下面的代码中，`x` 和 `y` 的类型分别是 `int` 和 `double`，是 `p` 的两个成员的拷贝。
```cpp
std::pair<int, double> p { 42, 3.14 };
auto [x, y] = p;
```

结构化绑定支持修饰符，比如引用和 `const`。
```cpp
std::pair<int, double> p { 42, 3.14 };
const auto &[x, y] = p; // x and y are const references
auto &[a, b] = p;		// a and b are non-const references
a = 10;					// now p.first is 10, and x is 10!
```

结构化绑定除了 `std::pair`，还可用于 `std::tuple`、`std::array`，以及满足聚合体条件的结构体（所有非静态数据成员都是公开的）。
```cpp
std::tuple<int32_t, int32_t, int32_t> t { 1, 2, 3 };
auto [length, width, height] = t;

std::array<int, 3> arr { 4, 5, 6 };
auto [a, b, c] = arr;

struct Point
{
	int x;
	int y;
};

Point p { 1, 2 };
auto [px, py] = p;
```


有了结构化绑定，能够写出可读性更高的代码。比如
```cpp
std::set<int64_t> s { 1, 2, 3, 4, 5 };
auto [it, inserted] = s.insert(6);

const std::map<std::string, int> mapCityPopulation {
	{ "Beijing",  21'707'000 },
	{ "London",	  8'787'892	 },
	{ "New York", 8'622'698	 }
};

for (auto &[city, population] : mapCityPopulation)
{
	std::cout << city << ": " << population << '\n';
}
```

如果想要让自定义类型支持结构化绑定，需要增加一组 API 来告诉编译器如何访问对象的各个成员。这组 API 包括：

- std::tuple_size<T> 返回类型 `T` 的元素个数。
- std::tuple_element<I, T> 返回类型 `T` 的第 `I` 个元素的类型。
- get<I>(t) 返回类型 `T` 的第 `I` 个元素的值或引用，`t` 是类型为 `T` 的对象。

下面是一个完整的示例。
```cpp
class User
{
public:
	User(const std::string &name, int32_t age)
		: name(name)
		, age(age)
	{}

	std::string GetName() const { return name; }

	int32_t GetAge() const { return age; }

private:
	std::string name;
	int32_t age;
};

namespace std
{
template<>
struct tuple_size<User> : std::integral_constant<std::size_t, 2>
{};

// template<std::size_t I>
// struct tuple_element<I, User>;

template<>
struct tuple_element<0, User>
{
	using type = std::string;
};

template<>
struct tuple_element<1, User>
{
	using type = int32_t;
};

} // namespace std

template<std::size_t I>
decltype(auto) get(const User &user);

template<>
decltype(auto) get<0>(const User &user)
{
	return user.GetName();
}

template<>
decltype(auto) get<1>(const User &user)
{
	return user.GetAge();
}

int main()
{
	User user { "Alice", 30 };
	auto [name, age] = user;
	std::cout << "Name: " << name << ", Age: " << age << '\n';
}
```

上面的例子是只读的结构化绑定。如果想通过结构化绑定修改成员变量，需要类先具备返回引用的能力（例如增加如下接口），并在 `get<I>` 中提供非常量引用的重载版本 `User &user`。在上一版本的只读实现中，返回类型虽然写的是 `decltype(auto)`，其实写 `auto` 也可以，但对于返回引用的版本必须使用 `decltype(auto)`（或显式写为引用类型），因为 `auto` 会忽略引用，而 `decltype(auto)` 会保留引用。
```cpp
std::string &GetName() { return name; }
int32_t &GetAge() { return age; }
```
