通常，在花括号内声明的变量，其可见性会被限制在该花括号的作用域内。然而，C++11 之前的枚举 `enum` 是一个例外：它的枚举项与该枚举处于相同的作用域中。例如：
```cpp
enum Color
{
    black,
    white,
    red
};                  // black, white, red are in same scope as Color
auto white = false; // error! white already declared in this scope
```

官方将这种枚举称为未限定作用域枚举（`unscoped enum`）。与之相对，C++11 引入了限定作用域枚举（`scoped enum`）。
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

由于使用 `enum class` 关键字，限定作用域枚举有时也被称为枚举类。

除了限定作用域外，枚举类还有一个好处：类型更强。未限定作用域枚举可以隐式转换为整数（进而转换为浮点数）。因此，下面代码的语义看似荒唐，却是合法的。
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

只要在 `enum` 后添加 `class` 关键字，一切都变了。
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

如果确实想转换为其他类型，需要进行显式转换。
```cpp
if (static_cast<double>(c) < 14.5) // odd code, but it's valid
{
    // suspect, but it compiles
    auto factors = primeFactors(static_cast<std::size_t>(c));
}
```

第三个好处是，限定作用域枚举可以前向声明。
```cpp
enum Color;       // error!
enum class Color; // fine
```

实际上，未限定作用域枚举也可以前向声明，不过需要一些额外的工作。对于未显式指定底层类型的未限定作用域枚举，编译器会选择某种整数类型作为其底层类型（`underlying type`）。例如：
```cpp
enum Color { black, white, red };
```

编译器可能会使用 `char` 类型，因为它只有三个枚举值。不过，有些枚举的取值范围会比较大，例如：
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

其取值范围从 `0` 到 `0xFFFFFFFF`，因此编译器会使用比 `char` 更宽的整数类型。

编译器会尽可能选择较短的整数类型，有时还会兼顾性能；无论如何，编译器都希望优化对象大小。由于在未看到枚举定义时无法确定其底层类型，C++98 风格的枚举不能前向声明。

不能前向声明会带来很多缺点，其中一个重大问题是编译依赖。例如，有如下枚举：
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

系统中的许多地方很可能都会使用这个枚举，因此都依赖定义它的头文件。现在需要添加一个新的状态：
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

那么几乎整个系统都需要重新编译。若使用前向声明：
```cpp
enum class Status;                 // forward declaration
void continueProcessing(Status s); // use of fwd-declared enum
```

即使 `Status` 的定义发生修改，包含这些声明的头文件也无需重新编译。若 `continueProcessing` 的实现没有变化，它也无需重新编译。

既然需要预先知道枚举的大小，C++11 是如何实现前向声明的呢？很简单：限定作用域枚举总是具有底层类型信息；对于未限定作用域枚举，则必须显式指定底层类型。
```cpp
enum class Status;                 // underlying type is int
enum class Status : std::uint32_t; // underlying type for Status is
                                   // std::uint32_t (from <cstdint>)
```

无论上述哪种情况，编译器都知道底层类型。

给未限定作用域枚举指定底层类型时，可以在前向声明中使用相同的语法，也可以在定义中指定。
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

不过，与 `std::tuple` 配合时，未限定作用域枚举很有用。例如，下面使用一个 `tuple` 保存用户的姓名、邮箱地址和声望值。
```cpp
using UserInfo =             // type alias; see Item 9
    std::tuple<std::string,  // name
               std::string,  // email
               std::size_t>; // reputation
```

虽然为各个字段添加了注释，但如果在其他地方看到下面的代码，还是很难理解。
```cpp
UserInfo uInfo;                // object of tuple type
auto val = std::get<1>(uInfo); // get value of field 1
```

用 1 表示用户的邮箱地址，实在不够可靠。这时，声明一个未限定作用域枚举就很有用了。
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

这里，`UserInfoFields` 的枚举项隐式转换为 `std::size_t`。

如果使用限定作用域枚举，写法就会麻烦很多。
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

可以编写一个接受 `UserInfoFields` 并返回 `std::size_t` 的函数来简化这种写法。不过，`std::get` 是模板函数，此处的索引作为模板实参必须在编译期确定，因此这里需要一个由 `constexpr` 修饰的函数，参见 [Item 15](./15_Use_constexpr_whenever_possible.md)。

实际上，我们需要的是一个 `constexpr` 模板函数：它接受任意枚举值并返回其底层类型，从而将上述函数的输入和输出泛化。`std::underlying_type` 可以帮助我们得到枚举的底层类型。下面的 `toUType` 函数接受任意枚举类型的值，并在编译期返回其底层类型的值。
```cpp
template <typename E> // C++14
constexpr auto
toUType(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}
```

这里使用了 [Item 9](./09_Prefer_alias_declarations_to_typedefs.md) 提到的 `std::underlying_type_t`，以及 [Item 3](../ch01_Deducing_Types/03_Understand_decltype.md) 提到的使用 `auto` 表示返回类型。

下面是简化后的客户端代码。
```cpp
auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);
```

这仍比使用未限定作用域枚举略微复杂一些。不过，限定作用域枚举有其优点，所以很多时候，多写几个字符也是可以接受的；这样可以规避未限定作用域枚举长期存在的陷阱。

## Things to Remember
* C++98-style `enums` are now known as unscoped `enums`.
* Enumerators of scoped `enums` are visible only within their `enum`. They convert to other types only with a cast.
* Both scoped and unscoped `enums` allow an underlying type to be specified. The default underlying type for scoped `enums` is `int`. Unscoped `enums` have no default underlying type.
* Scoped `enums` may always be forward-declared. Unscoped `enums` may be forward-declared only if their declaration specifies an underlying type.
