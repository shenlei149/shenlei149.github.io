如果不想让用户使用某个函数，不声明它就行了。不过，C++ 会帮你生成一些函数，即某些特殊的类成员函数。[Item 17](./17_Understand_special_member_function_generation.md) 会详细解释这些函数。这里只涉及拷贝构造函数和拷贝赋值运算符。C++98 的做法在 C++11 中有更好的替代方案。

C++98 通过将成员函数声明为 `private` 且不定义它们来禁止使用这些函数。下面是标准库中禁止拷贝 I/O 流的实现：
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

将其声明为 `private` 可使客户端代码无法调用；又由于故意不提供定义，如果其他成员函数或友元（`friend`）调用这些函数，链接时便会出错。

C++11 可以使用 `= delete` 将拷贝构造函数和拷贝赋值运算符标记为已删除函数（`deleted function`）。`basic_ios` 在 C++11 中可以这么写：
```cpp
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base
{
public:
    basic_ios(const basic_ios &) = delete;
    basic_ios &operator=(const basic_ios &) = delete;
};
```

已删除函数不能被任何函数调用；其他成员函数或友元调用它们时，会在编译期报错。

这里的访问修饰符是 `public`，而不是 `private`。若将已删除函数声明为 `private`，客户端代码使用它时，编译器可能只报访问权限错误，而不会指出函数已被删除。为获得更恰当的诊断信息，更新旧代码时应记得修改其可见性。

`= delete` 可以用于任何函数，而 `private` 方案仅对类成员函数有效。假定我们有如下函数：
```cpp
bool isLucky(int number);
```

C++ 从 C 继承了隐式转换，因此下面的代码虽然可能没有意义，但都是合法的。
```cpp
if (isLucky('a'))   // is 'a' a lucky number?
if (isLucky(true))  // is "true"?
if (isLucky(3.5))   // should we truncate to 3 before checking for luckiness?
```

如果要求参数必须是 `int`，就必须想办法阻止上述代码。一个方法是删除这些重载函数。
```cpp
bool isLucky(int number); // original function
bool isLucky(char) = delete; // reject chars
bool isLucky(bool) = delete; // reject bools
bool isLucky(double) = delete; // reject doubles and floats
```

注意，最后一个注释对 `double` 和 `float` 都生效。对于 `float` 参数，转换为 `double` 属于浮点提升，优先级高于转换为 `int` 的标准转换，因此会选择这个重载函数；但该重载函数已被删除，故会报错。

虽然已删除函数不能被调用，但它们仍是代码的一部分，并会参与重载解析。这正是有了上述函数声明后，下面这些代码无法通过编译的原因。
```cpp
if (isLucky('a'))   // error! call to deleted function
if (isLucky(true))  // error!
if (isLucky(3.5f))  // error!
```

删除函数还可以禁用某些函数模板特化，而 `private` 无法做到这一点。假定有一个接受指针的函数模板。
```cpp
template <typename T>
void processPointer(T *ptr);
```

这里有两种比较特殊的指针类型。一种是 `void*`，不能解引用，也不能进行指针算术运算；另一种是 `char*`，它往往表示 C 风格字符串，而非指向单个 `char` 对象的指针。通常会为后者提供特殊实现；但这里假定不希望用户使用这两种指针来实例化这个函数模板。可以通过删除这些函数来实现这一目的。
```cpp
template <>
void processPointer<void>(void *) = delete;
template <>
void processPointer<char>(char *) = delete;
```

这样便不能向函数 `processPointer` 传入 `void*` 或 `char*` 类型的参数。若要更严格地禁止这些类型，可能还需要删除如下函数。
```cpp
template <>
void processPointer<const void>(const void *) = delete;
template <>
void processPointer<const char>(const char *) = delete;
```

如果再严谨一些，可能还需要删除 `const volatile void*`、`const volatile char*`，以及指向 `wchar_t`、`char16_t` 和 `char32_t` 的指针等类型。

如果这个函数模板属于某个类，也无法通过将其声明为 `private` 来解决这个问题，因为不能为函数模板特化指定不同于主模板的访问级别。
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

这是因为显式模板特化必须在命名空间作用域中声明，而不能在类作用域中声明。这时可以使用已删除函数，因为无需为特化的函数模板指定其他访问级别。
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
* Prefer deleted functions to private, undefined ones.
* Any function may be deleted, including non-member functions and function template specializations.
