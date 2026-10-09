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
