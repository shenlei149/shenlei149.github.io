结构化绑定可以用一个对象的成员或元素初始化多个变量。比如
```cpp
struct MyStruct
{
    int i = 0;
    std::string s;
};

MyStruct ms;

// structured binding
auto [u, v] = ms;
```
某种程度上说，分解了某个对象来初始化其他变量。

下面两种初始化语法都是支持的。
```cpp
auto [u2, v2]{ms};
auto [u3, v3](ms);
```
当函数返回结构体或数组的时候，结构化绑定非常有用。
```cpp
MyStruct getStruct()
{
    return MyStruct{42, "hello"};
}

// id and val name i and s of returned struct
auto [id, val] = getStruct();

// using
if (id > 30)
{
    std::cout << val;
}
```
`id` `val` 是返回的结构体中的 `i` `s`。

使用结构化绑定，可以提高可读性。
```cpp
for (const auto &elem : mymap)
{
    std::cout << elem.first << ": " << elem.second << '\n';
}

for (const auto &[key, val] : mymap)
{
    std::cout << key << ": " << val << '\n';
}
```

## in Detail
#### Binding to an Anonymous Entity
结构化绑定的背后有一个匿名的对象，变量指向这个匿名对象的元素或成员。
```cpp
// initialization
auto [u, v] = ms;

auto e = ms;
aliasname u = e.i;
aliasname v = e.s;

// for return values
auto [u, v] = getStruct();

auto e = getStruct();
aliasname u = e.i;
aliasname v = e.s;
```
这里 `u` `v` 是 `ms` 对应成员变量的拷贝，不是引用类型，`decltype(u)` `decltype(v)` 分别是 `i` `s` 的类型。

匿名对象 `e` 的生命周期与结构化绑定生命周期一致，结构化绑定离开作用域，临时对象被销毁。

除非使用引用，否则修改结构化绑定变量的值不影响结构体的成员，反之亦然。
```cpp
MyStruct ms{42, "hello"};
auto [u, v] = ms;
ms.i = 77;
std::cout << u; // prints 42
u = 99;
std::cout << ms.i; // prints 77
```

#### Using Qualifiers
使用 `const` 和引用修饰作用于匿名对象 `e` 这个整体。
```cpp
const auto &[u, v] = ms; // a reference, so that u/v refer to ms.i/ms.s

ms.i = 77;      // affects the value of u
std::cout << u; // prints 77
```
非 `const` 的话，可以双向修改。
```cpp
MyStruct ms{42, "hello"};
auto &[u, v] = ms; // the initialized entity is a reference to ms
ms.i = 77;         // affects the value of u
std::cout << u;    // prints 77
u = 99;            // modifies ms.i
std::cout << ms.i; // prints 99
```
如果结构化绑定一个临时对象的引用，那么临时对象的生命周期扩展到结构化绑定的生命周期。
```cpp
MyStruct getStruct();

const auto &[a, b] = getStruct();
std::cout << "a: " << a << '\n'; // OK
```

#### Qualifiers Do Not Necessarily Apply to the Structured Bindings
下面的 `u` 和 `v` 并不是引用类型，只有匿名对象 `e` 是引用。不过是 `const` 的，无法修改。因此，`u` `v` 的类型分别是 `const int` 和 `const std::string`。
```cpp
const auto &[u, v] = ms; // a reference, so that u/v refer to ms.i/ms.s
```
对齐也是类似的，下面代码中对齐的匿名对象，也就是说第一个变量 `u` 按照 16 字节对齐了，但是 `v` 并没有。
```cpp
alignas(16) auto [u, v] = ms; // align the object, not v
```
即使使用 `auto` 进行结构化绑定，没有类型退化。因为 `auto` 作用于匿名对象整体，而整体不会类型退化。这与 `auto` 初始化不同。
```cpp
struct S
{
    const char x[6];
    const char y[3];
};

S s1{};
auto [a, b] = s1; // a and b get the exact member types: const char[6]

auto a2 = a; // a2 gets decayed type of a
```

#### Move Semantics
移动语义也符合上述规则。

`u` `v` 指向匿名对象，它是 `ms` 的右值引用。此时 `ms` 还持有原来的值。
```cpp
MyStruct ms = {42, "Jim"};
auto &&[v, n] = std::move(ms); // entity is rvalue reference to ms

std::cout << "ms.s: " << ms.s << '\n'; // prints "Jim"
```
不过可以对 `n` 进行移动赋值，此时会将 `ms.s` 移走，这样，被移动的数据处于未定义的状态。
```cpp
std::string s = std::move(n);          // moves ms.s to s
std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints unspecified value
std::cout << "s: " << s << '\n';       // prints "Jim"
```
注意，这与通过移动来初始化结构化绑定不同。这里匿名对象是用 `ms` 移动的值来初始化，`ms` 已经不再持有原来的值了。
```cpp
MyStruct ms = {42, "Jim"};
auto [v, n] = std::move(ms); // new entity with moved-from values from ms

std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints "Jim"
```
后续可以给 `n` 赋予新值，或者移动赋值给其他变量，但是这都与 `ms.s` 无关了。
```cpp
std::string s = std::move(n); // moves n to s
n = "Lara";
std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
std::cout << "n: " << n << '\n';       // prints "Lara"
std::cout << "s: " << s << '\n';       // prints "Jim"
```

## Where Can Be Used
