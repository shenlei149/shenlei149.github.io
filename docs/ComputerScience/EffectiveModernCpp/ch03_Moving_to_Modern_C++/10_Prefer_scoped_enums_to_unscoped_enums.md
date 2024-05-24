一个一般规则是大括号里面声明的变量的可见性会被限制在大括号范围内。但是 C++11 之前的枚举 `enum` 是一个例外，它的范围是所有包含这个枚举的地方。比如
```cpp
enum Color
{
    black,
    white,
    red
};                  // black, white, red are in same scope as Color
auto white = false; // error! white already declared in this scope
```

官方称呼是未限定范围的（`unscoped`）的枚举，与此相对，C++11 引入了限定范围的枚举（` scoped enum`）。
```cpp
enum class Color
{
    black,
    white,
    red
};                      // black, white, red are scoped to Color
auto white = false;     // fine, no other "white" in scope
Color c = white;        // error! no enumerator named "white" is in this scope
Color c = Color::white; // fine
auto c = Color::white;  // also fine (and in accord with Item 5's advice)
```

由于关键字是 `enum class` 所有有时也称为枚举类。

除了限定范围外，枚举类还有一个好处：更强的类型。未限定范围的枚举能够隐式地转化成整数（进而可以转化为浮点数）。所以下面代码的语义看似荒唐，但是是合法的。
```cpp
enum Color
{
    black,
    white,
    red
}; // unscoped enum

std::vector<std::size_t>     // func. returning
primeFactors(std::size_t x); // prime factors of x

Color c = red;

if (c < 14.5) // compare Color to double (!)
{
    auto factors = primeFactors(c); // compute prime factors of a Color (!)
}
```

只要将 `enum` 后添加一个 `class` 关键字，一切都变了。
```cpp
enum class Color
{
    black,
    white,
    red
}; // enum is now scoped

Color c = Color::red; // as before, but with scope qualifier
if (c < 14.5)         // error! can't compare Color and double
{
    // error! can't pass Color to function expecting std::size_t
    auto factors = primeFactors(c);
}
```

如果真的想转化成其他类型，需要显式地实现。
```cpp
if (static_cast<double>(c) < 14.5) // odd code, but it's valid
{
    // suspect, but it compiles
    auto factors = primeFactors(static_cast<std::size_t>(c));
}
```

第三个好处是限定范围的枚举可以前置声明。
```cpp
enum Color;       // error!
enum class Color; // fine
```

其实非限定范围的枚举也可以前置声明，不过需要一些额外的工作。利用的是对于 C++ 中的枚举类型，编译器会确定使用某个长度的整数作为其底层类型（`underlying type`）。比如
```cpp
enum Color { black, white, red };
```

编译器可能会使用 `char` 类型，因为只有三种值。不过有的枚举的值域范围会比较大，比如
```cpp
enum Status
{
    good = 0,
    failed = 1,
    incomplete = 100,
    corrupt = 200,
    indeterminate = 0xFFFFFFFF
};
```

值域范围从 `0` 到 `0xFFFFFFFF`，那么编译器使用比 `char` 长的整数类型。

编译器会尽可能选择一个短的整数类型，有的时候会兼顾性能，不管怎么，编译器希望能够优化大小。为了能够优化，规定枚举只能定义不能声明。

不能前向声明有很多缺点。一个很大的问题是编译依赖问题。比如有如下枚举
```cpp
enum Status
{
    good = 0,
    failed = 1,
    incomplete = 100,
    corrupt = 200,
    indeterminate = 0xFFFFFFFF
};
```

这个枚举大概率系统的很多地方都在使用，所以都依赖这个头文件。我们需要加一个新的状态
```cpp
enum Status
{
    good = 0,
    failed = 1,
    incomplete = 100,
    corrupt = 200,
    audited = 500,
    indeterminate = 0xFFFFFFFF
};
```

那么近乎整个系统都需要重新编译。如果使用前向声明
```cpp
enum class Status;                 // forward declaration
void continueProcessing(Status s); // use of fwd-declared enum
```

即使 `Status` 修改了，包含这些声明的头文件也不需要重新编译。如果 `continueProcessing` 的实现没有变，那么也不需要重新编译。

既然现需要知道长度，C++11 是如何实现前向声明的呢？很简单，对于限定范围的枚举，总是有长类型信息即可。对于非限定范围的枚举，必须指定一个类型信息。
```cpp
enum class Status;                 // underlying type is int
enum class Status : std::uint32_t; // underlying type for Status is
                                   // std::uint32_t (from <cstdint>)
```

不管是上述那种情况，编译器都知道底层类型。

给未限定范围的枚举指定底层类型，可以在前向声明的时候使用相同的语法指定，也可以在定义的时候指定。
```cpp
enum Color : std::uint8_t; // fwd decl for unscoped enum;
// underlying type is std::uint8_t
enum class Status : std::uint32_t
{
    good = 0,
    failed = 1,
    incomplete = 100,
    corrupt = 200,
    audited = 500,
    indeterminate = 0xFFFFFFFF
};
```

不过，当和 `std::tuple` 配合的时候，未限定范围的枚举很有用。比如下面的例子，使用一个 `tuple` 保存用户的名字、邮箱地址和声望值。
```cpp
using UserInfo =             // type alias; see Item 9
    std::tuple<std::string,  // name
               std::string,  // email
               std::size_t>; // reputation
```

虽然有各个字段的注释，但是如果在其他地方看到下面的代码，还是很难理解。
```cpp
UserInfo uInfo;                // object of tuple type
auto val = std::get<1>(uInfo); // get value of field 1
```

使用 1 表示用户的邮箱地址，实在是不靠谱。这时声明一个未限定范围的枚举就很有用了。
```cpp
enum UserInfoFields
{
    uiName,
    uiEmail,
    uiReputation
};
UserInfo uInfo;                      // object of tuple type
auto val = std::get<uiEmail>(uInfo); // ah, get value of email field
```

这里 `UserInfoFields` 隐式地转化成了 `std::size_t`。

如果使用限定范围的枚举，就会麻烦很多。
```cpp
enum class UserInfoFields
{
    uiName,
    uiEmail,
    uiReputation
};
UserInfo uInfo; // as before
auto val = std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>(uInfo);
```

可以写一个接受 `UserInfoFields` 返回 `std::size_t` 的函数来简化这个写法。不过由于 `std::get` 是一个模板函数，那么需要在编译期确定这个值，那么这里需要一个 `constexpr` 修饰的函数，参考 [Item 15](./15_Use_constexpr_whenever_possible.md)。

实际上，我们需要的是 `constexpr` 模板函数，接受任意枚举，然后返回其底层类型，也就是一般化上述函数的输入和输出。`std::underlying_type` 能够帮助我们得到枚举的底层类型。下面的 `toUType` 函数接受任意枚举类型，在编译期返回它的值。
```cpp
template <typename E> // C++14
constexpr auto
toUType(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}
```

这里使用了 [Item 9](./09_Prefer_alias_declarations_to_typedefs.md) 提到的 `std::underlying_type_t` 和 [Item 3](../ch01_Deducing_Types/03_Understand_decltype.md) 提到的使用 `auto` 表示返回类型。

下面是简写的客户端代码。
```cpp
auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);
```

这仍旧比使用未限定范围的枚举略微复杂一些。不过限定范围的枚举有它的好处，所以很多时候，多写几个字符也是可以接受的，以此来规避长期存在的未限定范围的枚举的陷阱。

## Things to Remember
* C++98-style `enums` are now known as unscoped `enums`.
* Enumerators of scoped `enums` are visible only within the `enum`. They convert to other types only with a cast.
* Both scoped and unscoped `enums` support specification of the underlying type. The default underlying type for scoped `enums` is `int`. Unscoped `enums` have no default underlying type.
* Scoped `enums` may always be forward-declared. Unscoped `enums` may be forward-declared only if their declaration specifies an underlying type.
