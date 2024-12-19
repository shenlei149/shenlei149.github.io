在 C++17 之前，不能省略模版参数。
```cpp
std::complex<double> c{5.1, 3.3};

std::mutex mx;
std::lock_guard<std::mutex> lg(mx);
```
不过 C++17 放宽了这个限制，通过类模版参数推导（`class template argument deduction`）机制，如果构造函数能够推导出所有模板参数的类型，那么可以省略模板参数，甚至容器也可以这么做。
```cpp
std::complex c{5.1, 3.3}; // OK: std::complex<double> deduced

std::mutex mx;
std::lock_guard lg{mx}; // OK: std::lock_guard<std_mutex> deduced

std::vector v1{1, 2, 3}           // OK: std::vector<int> deduced
std::vector v2{"hello", "world"}; // OK: std::vector<const char*> deduced
```

## Use of Class Template Argument Deduction
只要能通过构造函数的参数推导出参数类型，就可以使用类模板参数推导。比如下面几种合法的初始化。
```cpp
std::complex c1{1.1, 2.2}; // deduces std::complex<double>
std::complex c2(2.2, 3.3); // deduces std::complex<double>
std::complex c3 = 3.3;     // deduces std::complex<double>
std::complex c4 = {4.4};   // deduces std::complex<double>
```
`c3` `c4` 能够被推导的原因是能够用一个参数初始化 `std::complex<>`，可以推导出类型 `T`，这个类型用于实数和复数部分。
```cpp
namespace std
{
    template <typename T>
    class complex
    {
        constexpr complex(const T &re = T(), const T &im = T());
    }
};
```
对于 `std::complex c1{1.1, 2.2};`，编译器找到
```cpp
constexpr complex(const T &re = T(), const T &im = T());
```
两个参数类型都是 `double`，因此推导结果是
```cpp
complex<double>::complex(const double &re = double(),
                         const double &im = double());
```
推导不能有歧义，如下的初始化就无法推导出正确的结果。和通常的模板一样，推导模板参数不会用类型转换。
```cpp
std::complex c5{5, 3.3}; // ERROR: attempts to int and double as T
```
对于可变参数模版也可以使用类模板参数推导。比如 `std::tuple<>` 定义为
```cpp
namespace std
{
    template <typename... Types>
    class tuple
    {
    public:
        constexpr tuple(const Types &...);
    };
};
```
声明
```cpp
std::tuple t{42, 'x', nullptr};
```
推导的类型是
```cpp
std::tuple<int, char, std::nullptr_t>
```

还可以推导非类型模板参数。比如初始化数组时，元素类型和大小都能推导出来。
```cpp
template <typename T, int SZ>
class MyClass
{
public:
    MyClass(T (&)[SZ])
    {
    }
};

MyClass mc("hello"); // deduces T as const char and SZ as six
```

### Copying by Default
类模板参数推导首先会考虑拷贝方式的初始化。使用一个元素初始化 `std::vector`
```cpp
std::vector v1{42}; // vector<int> with one element
```
用一个 `std::vector` 初始化另一个 `std::vector` 会被解释为创建了一个拷贝，而不是 `std::vector<std::vector<int>>`。所有的初始化形式都遵循这个原则。
```cpp
std::vector v2{v1};        // v2 also is a vector<int>
std::vector v3(v1);        // v3 also is a vector<int>
std::vector v4 = {v1};     // v4 also is a vector<int>
auto v5 = std::vector{v1}; // v5 also is a vector<int>
```
这是花括号初始化总是将列表参数当做元素的例外，如果传入一个 `std::vector` 参数，那么得到的是这个 `std::vector` 的拷贝。如果用一个以上元素初始化，就没有这个问题了。
```cpp
std::vector vv{v1, v2}; // vv is vector<vector<int>>
```
那么对于可变参数模板进行类模板参数推导的话，结果是什么呢？结果有争议，仍在讨论。使用 g++ 的结果是 `std::vector<int>`，相当于一个元素初始化的情况。
```cpp
template <typename... Args>
auto make_vector(const Args &...elems)
{
    return std::vector{elems...};
}

std::vector<int> v{1, 2, 3};
auto x1 = make_vector(v, v); // vector<vector<int>>
auto x2 = make_vector(v);    // vector<int> or vector<vector<int>> ?
```

### Deducing the Type of Lambdas
通过类模板参数推导，可以用 lambda 表达式（准确说是其闭包类型）来初始化类模板。比如我们可以实现一个通用类来自己任意类型回调函数被调用的次数。
```cpp
#include <utility> // for std::forward()

template <typename CB>
class CountCalls
{
private:
    CB callback;    // callback to call
    long calls = 0; // counter for calls

public:
    CountCalls(CB cb) : callback(cb)
    {
    }
    template <typename... Args>
    decltype(auto) operator()(Args &&...args)
    {
        ++calls;
        return callback(std::forward<Args>(args)...);
    }
    long count() const
    {
        return calls;
    }
};
```
我们可以用一个 lambda 表达式来构造 `CountCalls`，此时从 lambda 推导 `CB` 的具体类型。
```cpp
CountCalls sc{[](auto x, auto y)
              {
                  return x > y;
              }};
```
`sc` 是排序的标准，其类型会被推导为 `CountCalls<TypeOfTheLambda>`，可以将其传给 `std::sort` 函数。注意这里需要按引用传递，因为 `std::sort` 按值传递排序标准，所以不传递应用的话 `std::sort` 会使用计数器的拷贝。
```cpp
std::sort(v.begin(), v.end(), // range
          std::ref(sc));      // sorting criterion

std::cout << "sorted with " << sc.count() << " calls\n";
```
不过，我们可以直接传递对象本身给 `std::for_each()`，因为它会返回传递给它的回调函数，这样可以获取回调内部的信息。
```cpp
auto fo = std::for_each(v.begin(), v.end(),
                        CountCalls{[](auto i)
                                   {
                                       std::cout << "elem: " << i << '\n';
                                   }});

std::cout << "output with " << fo.count() << " calls\n";
```

### No Partial Class Template Argument Deduction
与函数模版不同的是，不能传递部分类模板参数，然后自动推导剩余参数，使用空的模板参数列表也是不行的。下面是例子，注意第三个模板参数有默认值，因此第二个参数指定的情况下，可以不指定第三个参数。
```cpp
template <typename T1, typename T2, typename T3 = T2>
class C
{
public:
    C(T1 x = {}, T2 y = {}, T3 z = {})
    {
    }
};

// all deduced:
C c1(22, 44.3, "hi"); // OK: T1 is int, T2 is double, T3 is const char*
C c2(22, 44.3);       // OK: T1 is int, T2 and T3 are double
C c3("hi", "guy");    // OK: T1, T2, and T3 are const char*

// only some deduced:
C<string> c4("hi", "my"); // ERROR: only T1 explicitly defined
C<> c5(22, 44.3);         // ERROR: neither T1 not T2 explicitly defined
C<> c6(22, 44.3, 42);     // ERROR: neither T1 nor T2 explicitly defined

// all specified:
C<string, string, int> c7;           // OK: T1,T2 are string, T3 is int
C<int, string> c8(52, "my");         // OK: T1 is int,T2 and T3 are strings
C<string, string> c9("a", "b", "c"); // OK: T1,T2,T3 are strings
```
下面的例子可以解释为什么不能部分推导的原因。`std::tuple` 是可变参数模板，编译器不知道下面的代码是应该推导成 `std::tuple<int, int>` 还是一个多写了一个参数的笔误。
```cpp
std::tuple<int> t(42, 43); // still ERROR
```
不支持部分类模板参数推导意味着有些编码需求没有得到满足。比如一个简单 lambda 仍旧无法作为关联容器的排序标准或者是无序容器的哈希函数，而一定要指定类型。
```cpp
std::set<Cust> coll([](const Cust &x, const Cust &y) { // still ERROR
    return x.getName() > y.getName();
});

auto sortcrit = [](const Cust &x, const Cust &y)
{
    return x.getName() > y.getName();
};
std::set<Cust, decltype(sortcrit)> coll(sortcrit); // OK
```

### Class Template Argument Deduction Instead of Convenience Functions
通过类模板参数推导，我们可以不再使用一些根据传入参数来进行类模板参数推导的快捷函数。比如 `std::make_pair()`。以前使用它的好处是简化代码，但是现在可以直接使用构造函数而无需指定模板参数。
```cpp
std::vector<int> v;
auto p = std::make_pair(v.begin(), v.end()); // better
std::pair<typename std::vector<int>::iterator,
          typename std::vector<int>::iterator>
    p(v.begin(), v.end());

// now
std::pair p(v.begin(), v.end());
// or
std::pair p{v.begin(), v.end()};
```
不过这些快捷函数不仅仅能够推导类型，还会使传入的参数退化。比如字符串字面量会被推导为 `const char*`。
```cpp
// q has type std::pair<const char*, const char*>
auto q = std::make_pair("hi", "world"); // deduces pair of pointers
```
而使用类模板参数推导会使得问题复杂化。比如 `std::pair` 的简单类声明如下。
```cpp
template <typename T1, typename T2>
struct Pair1
{
    T1 first;
    T2 second;
    Pair1(const T1 &x, const T2 &y) : first{x}, second{y}
    {
    }
};
```
参数类型是引用类型，此时模板参数类型不会退化，比如数组退化成指针。所以下面的代码使得 `T1` 类型是 `char [3]`，`T2` 类型是 `char[6]`，这本身是合理的。
```cpp
Pair1 p1{"hi", "world"}; // deduces pair of arrays of different size, but...
```
但是使用左值数组来初始化另一个数组是不行的，所以上述构造函数就类似于下面这段无法编译的代码。
```cpp
const char x[3] = "hi";
const char y[6] = "world";
char first[3]{x};  // ERROR
char second[6]{y}; // ERROR
```
一个可行的办法是按值传递。
```cpp
template <typename T1, typename T2>
struct Pair2
{
    T1 first;
    T2 second;
    Pair2(T1 x, T2 y) : first{x}, second{y}
    {
    }
};
```
不过 `std::pair` 的构造函数传递的是引用，但是还是可以做到退化，原因是下一个主题。

## Deduction Guides
我们可以定义推导规则（`deduction guide`）提供附加的类模板参数推导或者修正构造函数的推导结果。回到之前的例子，我们定义一个推导规则，使得推导规则看起来像是按值传递的样子。
```cpp
template <typename T1, typename T2>
struct Pair3
{
    T1 first;
    T2 second;
    Pair3(const T1 &x, const T2 &y) : first{x}, second{y}
    {
    }
};

// deduction guide for the constructor:
template <typename T1, typename T2>
Pair3(T1, T2) -> Pair3<T1, T2>;
```
`->` 左边声明我们想要推导什么，这里是通过值传递的方式，参数类型是 `T1` `T2` 的 `Pair3` 的构造函数。`->` 右边声明我们想要的推导结果，是 `T1` `T2` 的 `Pair3` 的模板实例化。

这个新定义的推导规则和构造函数最大的不同是一个按值传递，一个按引用传递，而按值传递会使得参数类型退化，数组退化成指针，顶层 `const` 或引用会被忽略。

有了这个推导规则后，如下声明就会得到我们预期的结果。
```cpp
Pair3 p3{"hi", "world"};

Pair3<const char *, const char *> p3{"hi", "world"};
```
注意，构造函数仍旧是按引用传递，推导规则只和模板类型的推导相关，与推导之后的实际使用的构造函数无关。

### Using Deduction Guides to Force Decay
如果想要参数类型退化，就可以添加必要的推导规则。因此，任何一个构造函数有按引用传参的模板类，就需要添加推导规则。
```cpp
template <typename T>
struct C
{
    C(const T &)
    {
    }
};

C x{"hello"}; // T deduced as char[6]

template <typename T>
C(T) -> C<T>;
C x{"hello"}; // T deduced as const char*
```

### Non-Template Deduction Guides
推导规则可以用于非模板的地方，也可以用于非构造函数。比如下面为结构体添加推导规则，帮助推导结构体的模板参数 `T`，相当于显式指定了模版参数。

因此，下面的声明都是合法的，因为 `T` 被推导为 `std::string`，同时字符串字面量可以隐式转成 `std::string`。
```cpp
S s1{"hello"};     // OK, same as: S<std::string> s1{"hello"};
S s2 = {"hello"};  // OK, same as: S<std::string> s2 = {"hello"};
S s3 = S{"hello"}; // OK, both S deduced to be S<std::string>
```
注意，聚合体使用列表初始化，下面的代码中类型能够正确推导，但是由于没有圆括号初始化语法，所以是非法的。
```cpp
S s4 = "hello"; // ERROR: can't initialize aggregates without braces
S s5("hello");  // ERROR: can't initialize aggregates without braces
```

### Deduction Guides versus Constructors
推导规则和构造函数有竞争关系，会根据重载匹配最优的推导规则或构造函数。如果二者优先级一样，使用推导规则。
```cpp
template <typename T>
struct C1
{
    C1(const T &)
    {
    }
};
C1(int) -> C1<long>;

C1 x1{42}; // T deduced as long

// constructor is a better match because no type conversion is necessary
C1 x3{'x'}; // T deduced as char
```

重载规则中，按引用传递和按值传递参数是相同的优先级，所以会使用推导规则。因此，通常推导规则往往是按值传递（还能类型退化）。

### Explicit Deduction Guides
推导规则可以定义为 `explicit` 的。当 `explicit` 引用了初始化或者转换，推导规则会被忽略。比如
```cpp
template <typename T>
struct S
{
    T val;
};
explicit S(const char *) -> S<std::string>;
```
拷贝初始化（`=`）会忽略推导规则，不过列表初始化或显式推导都是可以的。
```cpp
S s1 = {"hello"}; // ERROR (deduction guide ignored and otherwise invalid)

S s2{"hello"};       // OK, same as: S<std::string> s2{"hello"};
S s3 = S{"hello"};   // OK
S s4 = {S{"hello"}}; // OK
```
下面是另一个例子。
```cpp
template <typename T>
struct Ptr
{
    Ptr(T) { std::cout << "Ptr(T)\n"; }

    template <typename U>
    Ptr(U) { std::cout << "Ptr(U)\n"; }
};

template <typename T>
explicit Ptr(T) -> Ptr<T *>;

Ptr p1{42};  // deduces Ptr<int*> due to deduction guide
Ptr p2 = 42; // deduces Ptr<int> due to constructor
int i = 42;
Ptr p3{&i};  // deduces Ptr<int**> due to deduction guide
Ptr p4 = &i; // deduces Ptr<int*> due to constructor
```

### Deduction Guides for Aggregates
推导规则可以用于泛型聚合体的类模板类型推导。比如
```cpp
template <typename T>
struct A
{
    T val;
};
```
没有推导规则，尝试类模板类型推导都是错误的，需要显式指定模板类型。
```cpp
A i1{42};      // ERROR
A s1("hi");    // ERROR
A s2{"hi"};    // ERROR
A s3 = "hi";   // ERROR
A s4 = {"hi"}; // ERROR

A<int> i2{42};
A<std::string> s5 = {"hi"};
```
而添加推导规则后，可以如下初始化聚合体。
```cpp
A(const char *) -> A<std::string>;

A s2{"hi"};    // OK
A s4 = {"hi"}; // OK
```

### Standard Deduction Guides
C++17 的标准库引入了大量推导规则。

#### Deduction Guides for Pairs and Tuples
`std::pair` 利用推导规则使得参数类型退化。
```cpp
namespace std
{
    template <typename T1, typename T2>
    struct pair
    {
        constexpr pair(const T1 &x, const T2 &y); // take argument by-reference
    };

    template <typename T1, typename T2>
    pair(T1, T2) -> pair<T1, T2>; // deduce argument types by-value
}

std::pair p{"hi", "world"}; // takes const char[3] and const char[6]
// equivalent to:
std::pair<const char *, const char *> p{"hi", "world"};
```
可变参数类模板 `std::tuple` 也使用了相同的方法。
```cpp
namespace std
{
    template <typename... Types>
    class tuple
    {
    public:
        constexpr tuple(const Types &...); // take arguments by-reference

        template <typename... UTypes>
        constexpr tuple(UTypes &&...);
    };

    template <typename... Types>
    tuple(Types...) -> tuple<Types...>; // deduce argument types by-value
};

// std::tuple<int, const char*, std::nullptr_t>
std::tuple t{42, "hello", nullptr};
```

#### Deduction from Iterators
为了能够从一对表示范围的迭代器推导出元素的类型，容器都有如下 `std::vector<>` 的推导规则。
```cpp
// let std::vector<> deduce element type from initializing iterators:
namespace std
{
    template <typename Iterator>
    vector(Iterator, Iterator)
        -> vector<typename iterator_traits<Iterator>::value_type>;
}
```
示例
```cpp
std::set<float> s;
std::vector v1(s.begin(), s.end()); // OK, deduces std::vector<float>
```
这里必须使用圆括号，如果使用花括号的话，那么会看作是两个元素的初始化列表，结果是一个 `std::vector` 中有两个元素，分别是指向首尾两个迭代器。
```cpp
std::vector v2{s.begin(), s.end()}; // BEWARE: doesn't deduce std::vector<float>
std::vector<std::set<float>::iterator> v2{s.begin(), s.end()};
```
另一个方面，考虑下面这种情况
```cpp
std::vector v3{"hi", "world"}; // OK, deduces std::vector<const char*>
std::vector v4("hi", "world"); // OOPS: fatal runtime error
```
`v3` 是两个元素的 `std::vector`，`v4` 的初始化会导致错误，字符串字面量被转成了字符指针，是合法的迭代器。但是是指向不同对象的，是一对无效的迭代器。

总而言之，使用花括号是初始化 `std::vector` 元素的最佳方式，当然，传递一个 `std::vector` 是一个例外。其他语义最好使用圆括号。

对于类似 `std::vector` 这种复杂的模板容器，最好不要使用类模板类型推导，而是显式指定。

#### std::array<> Deduction
为了能够同时推导元素类型和元素个数，有如下推导规则
```cpp
// let std::array<> deduce its number of elements (must have same type):
namespace std
{
    template <typename T, typename... U>
    array(T, U...)
        -> array<enable_if_t<(is_same_v<T, U> && ...), T>,
                 (1 + sizeof...(U))>;
}
```
这里使用折叠表达式 `(is_same_v<T,U> && ...)` 使得所有元素的类型一致。

下面是一个合法和一个非法的例子。另外，类模板参数推导可以在编译期生效。
```cpp
std::array a{42, 45, 77}; // OK, deduces std::array<int,3>

std::array a{42, 45, 77.7}; // ERROR: types differ

constexpr std::array arr{0, 8, 15}; // OK, deduces std::array<int,3>
```

#### (Unordered) Map Deduction
下面通过给关联容器（`map`, `multimap`, `unordered_map`,
`unordered_multimap`）定义推导规则来说明想要推导规则正常工作是很难的事情。

这些容易的元素类型是 `std::pair<const keytype, valuetype>`，`const` 是必须的，因为元素在容器内的位置依赖于 `key`，如果能够修改，容器内部状态就会不一致。

下面是 C++17 中第一版给 `std::map` 的构造函数添加一个推导规则。
```cpp
namespace std
{
    template <typename Key, typename T,
              typename Compare = less<Key>,
              typename Allocator = allocator<pair<const Key, T>>>
    class map
    {
        map(initializer_list<pair<const Key, T>>,
            const Compare & = Compare(),
            const Allocator & = Allocator());

        map(initializer_list<pair<const Key, T>>,
            Compare = Compare(),
            Allocator = Allocator())
            -> map<Key, T, Compare, Allocator>;
    };
}
```
所有的参数按值传递，推导规则允许比较器和分配器类型退化。不过推导规则和构造函数使用了相同的类型，那么意味着初始化的 `key` 必须是 `const` 的。这样下面的代码会编译出错。
```cpp
std::pair elem1{1, 2};
std::pair elem2{3, 4};

std::map m1{elem1, elem2}; // ERROR with original C++17 guides
```
`elem1` `elem2` 推导的类型是 `std::pair<int,int>`，推导 `m1` 是发现这与 `key` 必须是 `const` 的不匹配，所以不得不显式指定。
```cpp
std::map<int, int> m1{elem1, elem2}; // OK
```
那么就不得不去掉推导规则中的 `const`。
```cpp
map(initializer_list<pair<Key, T>>,
    Compare = Compare(),
    Allocator = Allocator())
    -> map<Key, T, Compare, Allocator>;
```
不过，为了支持比较器和分配器类型的退化，不得不为 `const` `key` 写一个重载。否则，当使用 `const` `key` 初始化的时候会使用构造函数推导，这就导致传递 `const` `key` 和非 `const` `key` 推导结果有细微差别。

#### No Deduction Guides for Smart Pointers
我们期望 C++ 标准库中的部分地方有推导规则，但是实际没有。比如智能指针，如果有的话，可以使得代码更简洁。
```cpp
std::shared_ptr<int> sp{new int(7)};
std::shared_ptr sp{new int(7)}; // not supported
```
不能这么做的原因是构造函数本身也是模版函数，这意味着没有隐式推导规则。构造函数如下，`Y` 和 `T` 的类型不同，即使能够通过构造函数参数推导出 `Y`，也不能确定 `T` 的类型。
```cpp
namespace std
{
    template <typename T>
    class shared_ptr
    {
    public:
        template <typename Y>
        explicit shared_ptr(Y *p);
    };
}
```
这么做的原因是为了支持下面这种写法
```cpp
std::shared_ptr<Base> sp{new Derived(...)};
```
假定要添加推导规则，很简单
```cpp
namespace std
{
    template <typename Y>
    shared_ptr(Y *) -> shared_ptr<Y>;
}
```
但是这会导致无法分配数组了，因为 C++ 会遇到 C 的一个问题：指向一个对象的指针和对象的数组会退化成相同的类型。
```cpp
std::shared_ptr sp{new int[10]}; // OOPS: would deduce shared_ptr<int>
```
C++ 委员会决定不支持规则推导。所以对于对象和数组，可以分别使用如下语法。
```cpp
std::shared_ptr<int> sp1{new int};  // OK
auto sp2 = std::make_shared<int>(); // OK

std::shared_ptr<std::string> p(new std::string[10],
                               [](std::string *p)
                               {
                                   delete[] p;
                               });
std::shared_ptr<std::string[]> p{new std::string[10]};
```
