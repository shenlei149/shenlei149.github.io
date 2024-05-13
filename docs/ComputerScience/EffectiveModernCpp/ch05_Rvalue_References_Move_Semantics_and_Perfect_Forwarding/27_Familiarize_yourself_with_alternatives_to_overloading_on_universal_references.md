[Item 26](./26_Avoid_overloading_on_universal_references.md) 告诉我们重载通用引用的函数不是一个好主意。但是有些时候重载很有用。这一章阐述一些方法来解决这些问题。

## Abandon overloading
对于 [Item 26](./26_Avoid_overloading_on_universal_references.md) 的第一个例子 `logAndAdd`，我们可以避免重载。比如我们将两个函数分别取名为 `logAndAddName` 和 `logAndAddNameIdx`。但是这个方法对于第二个例子 `Person` 类的构造函数不适用，因为构造函数的名字是确定的。

## Pass by const T&
另一个可选方式是回到 C++98，参数类型是 `const` 左值引用。其实 [Item 26](./26_Avoid_overloading_on_universal_references.md) 最开始的方案就是这个。缺点是性能略微差一点。在知道通用引用和重载导致的问题后，放弃一点效率，选择简单正确的方式，也不失为一种好的折衷。

## Pass by value
一种不损失性能又不增加复杂度的方式是按值传递，这或许有点违反直觉。当你知道将会拷贝的时候，可以使用这种方法，Item 41 TODO link。会讨论如何实现这种方法并且会分析效率。这里，仅仅展示如何使用这种方法解决 `Person` 类重载的问题。
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
这个函数转发参数到 `logAndAddImpl`，第二个参数表明 `T` 是不是整型。对于整型右值，第二个参数是正确的。但是对于整型左值，根据 Item 28 TODO 的解释，`T` 会被推导为左值引用。如果 `int` 的左值传递给 `logAndAdd`，`T` 是 `int&`，不是整数类型。因此 `std::is_integral<T>` 对于传入左值的情况都返回 `false`，这与期望不一致。

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


##
