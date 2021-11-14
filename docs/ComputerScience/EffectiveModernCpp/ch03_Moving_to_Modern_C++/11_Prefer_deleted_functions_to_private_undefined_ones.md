如果不想让用户不能使用某个函数，不写就完事了。不过 C++ 会帮你生成一些函数，即一些特殊的类的成员函数。[Item 17](./17_Understand_special_member_function_generation.md) 会详细解释这函数。这里只会涉及拷贝构造和拷贝赋值函数。C++98 的做法，在 C++11 中有更好的实现方式。

C++98 通过声明成员函数为 `private` 且不定义这些函数来使得无法使用这些函数。下面是 STL 中禁止拷贝 I/O 流的实现：
```cpp
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base
{
public:
private:
    basic_ios(const basic_ios &);            // not defined
    basic_ios &operator=(const basic_ios &); // not defined
};
```

声明成 `private` 使得客户端代码无法调用，故意没有定义，那么如果其他成员函数或者友元（`friend`）调用这些函数，链接时会出错。

C++11 可以使用 `= delete` 将拷贝构造和拷贝赋值函数标记成已删除函数（`deleted function`）。`basic_ios` 在 C++11 中可以这么写
```cpp
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base
{
public:
    basic_ios(const basic_ios &) = delete;
    basic_ios &operator=(const basic_ios &) = delete;
};
```

删除掉的函数不能被任何函数调用，其他成员函数或者友元调用的话，会在编译期报错。

这里访问修饰符是 `public` 而不是 `private`。这是因为当客户端代码使用一个成员函数的时候，先检查访问权限然后检查函数是否被删除了。如果客户端代码使用了这样的 `private` 的已删除函数，编译器可能报访问性错误而不是函数被删除了。所以在更新旧代码的时候，记得修改可见性。

`= delete` 可以对任意函数使用，而 `private` 的方案仅对类成员函数有效。假定我们有如下函数：
```cpp
bool isLucky(int number);
```

C++ 继承了 C 的隐式转化，所以下面的代码可能没有意义，但是都是合法代码。
```cpp
if (isLucky('a'))   // is 'a' a lucky number?
if (isLucky(true))  // is "true"?
if (isLucky(3.5))   // should we truncate to 3 before checking for luckiness?
```

如果要求参数必须是 `int`，那么我们必须想办法阻止写出上述的代码。一个方法是把这些重载函数都删除。
```cpp
bool isLucky(int number); // original function
bool isLucky(char) = delete; // reject chars
bool isLucky(bool) = delete; // reject bools
bool isLucky(double) = delete; // reject doubles and floats
```

注意，最后一个注释对 `double` 和 `float` 都生效。相比 `float` 隐式地转化为 `int`，C++ 更倾向于先尝试转化成 `double`，然后发现能够匹配这么一个重载函数。不过这个重载函数被删除了，会报错。

虽然被删除的函数不能被调用，但是仍旧是代码的一部分，会参与到重载解析的过程中。这就是有了上述声明的函数，下面这些代码无法通过编译的原因。
```cpp
if (isLucky('a'))   // error! call to deleted function
if (isLucky(true))  // error!
if (isLucky(3.5f))  // error!
```

删除函数还可以禁用某些模板实例化，而 `private` 无法做到这一点。假定有一个模板函数接受一个指针。
```cpp
template <typename T>
void processPointer(T *ptr);
```

这里有两种比较特殊的函数指针类型。一个是 `void*`，不能解引用，不能自增或者自减。一个是 `char*`，往往表示 C 风格字符串而不是单独的指向一个 `char` 的指针。通常我们对于后者会有特殊的实现，不过这里假定我们不希望用户使用这两种指针来实例化这个模板函数。我们可以通过删除这些函数来实现我们的目的。
```cpp
template <>
void processPointer<void>(void *) = delete;
template <>
void processPointer<char>(char *) = delete;
```

这样，不能向函数 `processPointer` 传入 `void*` 或 `char*` 类型的参数。稍微严格一些，可能还需要删除如下函数。
```cpp
template <>
void processPointer<const void>(const void *) = delete;
template <>
void processPointer<const char>(const char *) = delete;
```

如果再严谨一些，可能需要删除如下类型：`const volatile void*` `const volatile char*`，甚至是 `std::wchar_t` `std::char16_t` `std::char32_t`。

如果这个模板函数属于某个类，也是无法通过声明成 `private` 的方式来解决这个问题的，因为我们不能标记模板函数特化为一个不同于模板函数访问级别的访问修饰符。
```cpp
class Widget
{
public:
    template <typename T>
    void processPointer(T *ptr)
    {
    }

private:
    template <> // error!
    void processPointer<void>(void *);
};
```

这是因为模板特化属于 `namesapce` 的范围而不是 `class`。这个时候可以使用删除函数来做，因为这并不需要使得特化的模板函数有其他访问级别。
```cpp
class Widget
{
public:
    template <typename T>
    void processPointer(T *ptr)
    {
    }
};
template <>                                         // still public,
void Widget::processPointer<void>(void *) = delete; // but deleted
```

## Things to Remember
* Prefer deleted functions to private undefined ones.
* Any function may be deleted, including non-member functions and template instantiations.
