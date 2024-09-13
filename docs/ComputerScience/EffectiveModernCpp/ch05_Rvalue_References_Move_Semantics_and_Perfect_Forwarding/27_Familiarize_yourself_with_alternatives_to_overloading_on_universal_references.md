[Item 26](./26_Avoid_overloading_on_universal_references.md) 告诉我们重载通用引用的函数不是一个好主意。但是有些时候重载很有用。这一章阐述一些方法来解决这些问题。

## Abandon overloading
对于 [Item 26](./26_Avoid_overloading_on_universal_references.md) 的第一个例子 `logAndAdd`，我们可以避免重载。比如我们将两个函数分别取名为 `logAndAddName` 和 `logAndAddNameIdx`。但是这个方法对于第二个例子 `Person` 类的构造函数不适用，因为构造函数的名字是确定的。

## Pass by const T&
另一个可选方式是回到 C++98，参数类型是 `const` 左值引用。其实 [Item 26](./26_Avoid_overloading_on_universal_references.md) 最开始的方案就是这个。缺点是性能略微差一点。在知道通用引用和重载导致的问题后，放弃一点效率，选择简单正确的方式，也不失为一种好的折衷。

## Pass by value
一种不损失性能又不增加复杂度的方式是按值传递，这或许有点违反直觉。当你知道将会拷贝的时候，可以使用这种方法，[Item 41](../ch08_Tweaks/41_Consider_pass_by_value_for_copyable_parameters_that_are_cheap_to_move_and_always_copied.md)。会讨论如何实现这种方法并且会分析效率。这里，仅仅展示如何使用这种方法解决 `Person` 类重载的问题。
```cpp
class Person
{
public:
    // replaces T&& ctor; see Item 41 for use of std::move
    explicit Person(std::string n)
        : name(std::move(n))
    {
    }

    explicit Person(int idx) // as before
        : name(nameFromIdx(idx))
    {
    }

private:
    std::string name;
};
```
由于 `std::string` 没有接受整型的构造函数，所以 `int` 和类似 `int` 的参数（比如 `std::size_t` `long` `short`）都会调用 `int` 重载的 `Person` 构造函数。类似的，`std::string` 及能够创建 `std::string` 的类型，比如 "Ruth" 字面量，都会调用 `std::string` 重载的 `Person` 构造构造函数。对传入了 0 或者 `NULL` 表示空指针但是又调用了 `int` 构造函数表示吃惊的话，参考 [Item 8](../ch03_Moving_to_Modern_C++/08_Prefer_nullptr_to_0_and_NULL.md)，不要这么用。

## Use Tag dispatch
如果我们的目的就是要使用完美转发，那么就不得不使用通用引用，没有别的选择。但是又不想放弃重载，改怎么做呢？

方法也比较简单。通用引用函数的参数列表的一部分是非通用引用，这就能使得非精确匹配的重载函数能够被选中，进而不调用通用引用版本的重载。这就是 `tag dispatch` 的原理。下面使用这种方法来解决之前 `logAndAdd` 函数遇到的问题。先回忆一下之前的实现。
```cpp
std::multiset<std::string> names; // global data structure

template <typename T>    // make log entry and add
void logAndAdd(T &&name) // name to data structure
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}
```
上述代码没有问题。但是如果引入一个 `int` 版本的重载，根据索引查找字符串，各种问题就出来了。这里我们保持这个函数不动，它接受所有的实参类型，包括整型和非整型。我们会将 `logAndAdd` 重新实现为两个函数，一个处理整型，一个处理非整型。

两个实际干活的函数新的名字是 `logAndAddImpl`，一个接受通用引用，这里又涉及通用引用和重载。但是新的函数接受第二个参数，表示传递的参数是否是整型。这个新的参数避免重蹈 [Item 26](./26_Avoid_overloading_on_universal_references.md) 的覆辙，因为这个新的参数决定了哪个重载会被调用。

下面是接近正确的新的实现。
```cpp
template <typename T>
void logAndAdd(T &&name)
{
    logAndAddImpl(std::forward<T>(name),
                  std::is_integral<T>()); // not quite correct
}
```
这个函数转发参数到 `logAndAddImpl`，第二个参数表明 `T` 是不是整型。对于整型右值，第二个参数是正确的。但是对于整型左值，根据 [Item 28](./28_Understand_reference_collapsing.md) 的解释，`T` 会被推导为左值引用。如果 `int` 的左值传递给 `logAndAdd`，`T` 是 `int&`，不是整数类型。因此 `std::is_integral<T>` 对于传入左值的情况都返回 `false`，这与期望不一致。

这里意识到问题就解决了问题，[Item 9](../ch03_Moving_to_Modern_C++/09_Prefer_alias_declarations_to_typedefs.md) 告诉我们 C++ 标准库中有 type traits，其中包含函数 `std::remove_reference`，作用是去掉类型的引用修饰。那么实现 `logAndAdd` 的正确方式是
```cpp
template <typename T>
void logAndAdd(T &&name)
{
    logAndAddImpl(
        std::forward<T>(name),
        std::is_integral<typename std::remove_reference<T>::type>());
}
```
接着，我们实现 `logAndAddImpl` 函数。第一个重载处理非整型类型，也就是 `std::is_integral<typename std::remove_reference<T>::type>()` 返回 `false` 的情况。

一旦明白了 `std::false_type`，也就理解了整个函数。从概念上将，`logAndAdd` 给 `logAndAddImpl` 传入一个 `bool` 类型，表明参数是否是整型，但是 `true` he `false` 是运行时的值，我们这里需要编译期的值进行重载选择。也就是说，我们需要一个类型，对于 `true` 时与 `false` 时要所有区别。C++ 标准库提供了 `std::true_type` 和 `std::false_type`，能够满足需求。如果 `T` 是整型，那么 `logAndAdd` 传递给 `logAndAddImpl` 是继承自 `std::true_type` 的对象，否则是继承自 `std::false_type` 的对象。也就是说 `logAndAdd` 根据 `T` 类型来调用不同的 `logAndAddImpl` 重载。

第二个重载处理整型的情况。`logAndAddImpl` 的实现很简单，根据 `idx` 查找对应的 `name`，然后再次调用 `logAndAdd` 即可，最后会调用到第一个重载。这样可以避免一些代码重复，保持简洁。
```cpp
std::string nameFromIdx(int idx); // as in Item 26

// integral argument: look up name and call logAndAdd with it
void logAndAddImpl(int idx, std::true_type)
{
    logAndAdd(nameFromIdx(idx));
}
```

这里，`std::true_type` 和 `std::false_type` 是标签 `tag`，目的是找到正确的重载函数。注意，我们甚至没有写参数名，因为在运行时用不到这些参数。我们希望编译器意识到这些值是无用的，进而做运行时的优化。通过创建标签对象，`logAndAdd` 将任务分发（`dispatch`）给正确的函数。因此这种模式的名字是 tag dispatch。这是元模板编程的标准模块，现代 C++ 库中大量这种用法。

tag dispatch 使得通用引用和重载能够一起使用。分发函数 `logAndAdd` 的参数是无限制的通用引用，没有重载。实现函数 `logAndAddImpl` 接受通用引用或其他类型和一个标签参数，有重载，标签参数的目的是使得最多只有一个重载能够匹配。也就是说，标签决定了哪个重载被调用。通用引用总是可以生成精确匹配这件事就不再重要了。

## Constraining templates that take universal references
tag dispatch 方法很容易应用。不过 [Item 26](./26_Avoid_overloading_on_universal_references.md) 的第二个例子是完美转发构造函数，由于编译器可能会自动生成拷贝和移动构造函数，所以即使只有一个 `Person` 完美转发构造函数，使用了 tag dispatch，但是有时会调用编译器生成的构造函数而绕过分发系统。

真实的问题是不一定会绕过分发函数。[Item 26](./26_Avoid_overloading_on_universal_references.md) 中提到的，如果提供了通用引用的构造函数，那么对于非 `const` 左值，会使用通用引用构造函数，而 `const` 左值又会使用拷贝构造函数。另外，继承类的拷贝和移动构造函数，会调用基类的完美转发构造函数而不是调用对应的拷贝和移动构造函数。

这种情况下，通用引用比我们预期的要贪婪，但是又没有贪婪到只有一个通用引用函数就足够了。解决这个问题需要一种新技术：`std::enable_if`。

`std::enable_if` 强制编译器禁用（`disable`）某些模板函数。默认情况下，所有模板都是启用的（`enabled`），不过添加了 `std::enable_if` 之后，只有在其条件满足的情况下模板函数才是启用的。对于 `Person` 完美转发函数，我们期望在对象不是 `Person` 类型的时候才被启用，在对象是 `Person` 类型的时候，我们期望拷贝和移动构造函数来处理这个问题。

下面是 `std::enable_if` 的用法展示，这里只写了函数声明，因为 `std::enable_if` 并不会影响函数实现。
```cpp
class Person
{
public:
    template <typename T,
              typename = typename std::enable_if<condition>::type>
    explicit Person(T &&n);
};
```
对 `std::enable_if` 的讨论超出了本书的范围，这里我们精力集中在 `condition` 部分。

这里的条件是 `T` 不是 `Person` 类型，那么可以写作 `!std::is_same<Person, T>::value`，但是正如 Item 28 TOOD link 解释的，如果用左值绑定到通用引用上，那么 `T` 是引用类型。而 `Person` 与 `Person&` 并不是一个类型，`std::is_same<Person, Person&>::value` 的结果是 `false` 也反映了这一点。

进一步考虑这个问题，当我们说 `T` 不是 `Person` 类型的时候，我们需要忽略

* 是否有引用。在这个场景下，`Person` `Person&` `Person&&` 与 `Person` 都一样的。
* 是否是 `const` 或者 `volatile`。`const Person` `volatile Person` `const volatile Person` 也应该被视为 `Person`。

这里需要使用 `std::decay`。`std::decay<T>::type` 与 `T` 同类型，会移除引用，cv 修饰符（`const` `volatile` 修饰符）。另外，它会将数组和函数退化成指针，但是这里不需要这个功能。因此这里的条件应写作
```cpp
!std::is_same<Person, typename std::decay<T>::type>::value
```
`Person` 完美转发函数声明如下
```cpp
class Person
{
public:
    template <
        typename T,
        typename = typename std::enable_if<
            !std::is_same<Person,
                          typename std::decay<T>::type>::value>::type>
    explicit Person(T &&n);
};
```
最后才介绍这个方法。如果有其他的方法避免通用引用与重载出问题，应该用其他方法。有了上述实现之后，当使用 `Person` 初始化一个 `Person` 对象，不管是左值还是右值，不管有没有 `const` `volatile`，都不会调用到通用引用的构造函数了。

还没有完全结束，[Item 26](./26_Avoid_overloading_on_universal_references.md) 还有一个场景，就是继承类的情况。
```cpp
class SpecialPerson : public Person
{
public:
    SpecialPerson(const SpecialPerson &rhs) // copy ctor; calls base class
        : Person(rhs)                       // forwarding ctor!
    {
    }

    SpecialPerson(SpecialPerson &&rhs) // move ctor; calls base class
        : Person(std::move(rhs))       // forwarding ctor!
    {
    }
};
```
很明显，`SpecialPerson` 与 `Person` 不是同一个类型，使用 `std::decay` 也不行，所以 `Person` 的完美转发构造函数并不是被禁用，因此，这里仍旧和之前一样，调用不到预期的拷贝和移动构造函数。

继承类使用最正规的方式调用基类的拷贝和移动构造函数，所以解决问题的方式还是要回到基类。`Person` 类的完美转发构造函数要在 `SpecialPerson` 类型的情况下也被禁用，或者说，类型是 `Person` 及其继承类都不行。

标准库的 type trait 提供了一个有用的接口：`std::is_base_of`，判定一个类型是否继承自另一个对象。如果 `T2` 继承自 `T1`，那么 `std::is_base_of<T1, T2>::value` 的值是 `true`。一个类型也能被想做是继承自自身，所以 `std::is_base_of<T, T>::value` 的结果总是 `true`。这个性质非常有用。当前我们要做的判断是，一个类型，在移除引用、`const` `volatile` 修饰之后，既不能是 `Person` 类型，也不能是继承自 `Person` 的类型。这里只需要使用 `std::is_base_of` 替换 ` std::is_same` 即可。
```cpp
class Person
{
public:
    template <
        typename T,
        typename = typename std::enable_if<
            !std::is_base_of<Person,
                             typename std::decay<T>::type>::value>::type>
    explicit Person(T &&n);
};
```
如果是 C++14，能够使用 `std::enable_if` `std::decay` 对应的 `_t` 的版本简化代码。
```cpp
class Person
{ // C++14
public:
    template <
        typename T,
        typename = std::enable_if_t< // less code here
            !std::is_base_of<Person,
                             std::decay_t<T> // and here
                             >::value>       // and here
        >
    explicit Person(T &&n);
};
```
至此，我们还剩余一个问题没有解决。就是要区别整型构造函数和非整型构造函数。因为我们最开始的目的就是要完全消除构造函数的歧义。

我们需要做的事情就是 1）新增一个接受整数的 `Person` 构造函数；2）进一步限制通用引用构造函数。将之前所有的技术整合到一起，就得到了解决方案。
```cpp
class Person
{
public:
    template <
        typename T,
        typename = std::enable_if_t<
            !std::is_base_of<Person, std::decay_t<T>>::value &&
            !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit Person(T &&n)         // ctor for std::strings and
        : name(std::forward<T>(n)) // args convertible to std::strings
    {
    }

    explicit Person(int idx) // ctor for integral args
        : name(nameFromIdx(idx))
    {
    }

    // copy and move ctors, etc.
private:
    std::string name;
};
```
对于元模板编程的信徒而言，这可能很优雅。不过这种方式很好地解决了问题。使用了完美转发，最大化了效率，而且有效地使得通用引用与重载能够共存。这种方案应用于不得不重载（比如构造函数）的时候。

## Trade-offs
本章介绍的前三种技术——不用重载，按 `const T&` 传递、按值传递——指定了函数的每一个参数类型。后两种技术——tag dispatch、限制模板函数——都使用了完美转发，没有指定参数的类型。是否指定参数类型就导致了后续一系列的问题。

使用完美转发更高效，避免为了迎合参数的类型而创建临时对象。在 `Person` 构造函数这个例子中，完美转发将 `"Nancy"` 字符串字面量转发给 `Person` 内部的 `std::string`，这就避免创建一个 `std::string` 临时对象以满足 `Person` 参数类型的要求。

但是完美转发也有缺点。其中一个缺点是即使一些类型的参数能够传递给指定类型的函数，仍旧不能被完美转发，参考 [Item 30](./30_Familiarize_yourself_with_perfect_forwarding_failure_cases.md)。

另一个缺点是当客户端传了不合法的参数，报错信息十分难理解。比如，一个客户端使用 `char16_t` 的字面量传递给 `Person` 的构造函数：
```cpp
// "Konrad Zuse" consists of characters of type const char16_t
Person p(u"Konrad Zuse");
```
如果使用前三个方法解决问题，构造函数参数是 `int` 或 `std::string`，那么错误信息比较直接，是说不能从 `const char16_t[12]` 转换成 `int` 或 `std::string`。

如果使用了完美转发，`const char16_t` 可以绑定到构造函数的参数上，不会报错。然后转发给 `std::string` 成员变量，此时会出现类型不匹配，实际参数类型是 `const char16_t` 的数组，而期望值是 `std::string` 构造函数的参数类型。此时会出现上百行的编译错误信息。

在这个例子中，我们只是用了一次完美转发。更复杂的系统中，参数会转发好几次之后才达到最后需要的地方，然后才能确定参数是否会被接受。转发层次越多，报错信息与实际的错误偏差就越大。很多程序员发现在性能关键点使用完美转发处会发生这种情况。

在 `Person` 这个情况下，我们知道传入的参数类型应该能作为 `std::string` 的构造函数的参数，可以使用 `static_assert` 来实现这一点。type traits 中 `std::is_constructible` 可以在编译器的时候检查一个类型能够构造出另一个类型。因此这个断言如下：
```cpp
class Person
{
public: // as before
    template <
        typename T,
        typename = std::enable_if_t<
            !std::is_base_of<Person, std::decay_t<T>>::value &&
            !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit Person(T &&n)
        : name(std::forward<T>(n))
    {
        // assert that a std::string can be created from a T object
        static_assert(
            std::is_constructible<std::string, T>::value,
            "Parameter n can't be used to construct a std::string");

        // the usual ctor work goes here
    }

    // remainder of Person class (as before)
};
```
这样，如果传入的参数不能构造一个 `std::string`，会给出更有用的信息。不过，这个语句实在构造函数内，而完美转发位于初始化列表，因此这个有用的信息会出现在上百行错误信息之后。

## Things to Remember
* Alternatives to the combination of universal references and overloading include the use of distinct function names, passing parameters by lvalue-reference-to-`const`, passing parameters by value, and using tag dispatch.
* Constraining templates via `std::enable_if` permits the use of universal references and overloading together, but it controls the conditions under which compilers may use the universal reference overloads.
* Universal reference parameters often have efficiency advantages, but they typically have usability disadvantages.
