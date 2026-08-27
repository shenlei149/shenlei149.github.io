C++11 提供了丰富的对象初始化方式，令人困惑，不知该如何选择。初始化对象基本有三种方式：大括号 `{}`、小括号 `()` 和等号 `=`。
```cpp
int x(0);   // initializer is in parentheses
int y = 0;  // initializer follows "="
int z{0};   // initializer is in braces
```

大括号和等号可以一起使用。
```cpp
int z = {0}; // initializer uses "=" and braces
```
这种形式等价于只使用大括号；下文不再单独讨论。

使用等号初始化往往会让新手感到困惑：他们以为这是赋值，但实际上不是。对于像 `int` 这样的类型，这种差异纯属理论；不过对于用户自定义类型，区分初始化和赋值非常重要，因为调用的是不同的函数。
```cpp
Widget w1;          // call default constructor
Widget w2 = w1;     // not an assignment; calls copy ctor
w1 = w2;            // an assignment; calls copy operator=
```

尽管有许多初始化语法，C++98 仍无法表达某些所需的初始化方式，例如通过指定元素来创建容器。

为解决上述问题，C++11 引入了统一初始化（`uniform initialization`）。它在概念上提供了一种几乎可用于任何位置的初始化方式。由于其语法使用大括号 `{}`，作者也将其称为大括号初始化（`braced initialization`）。前者着眼于概念，后者着眼于语法。

使用统一初始化可以实现此前无法实现的事情，例如指定容器包含哪些元素。
```cpp
std::vector<int> v{ 1, 3, 5 };      // v's initial content is 1, 3, 5
```

C++11 允许在类内直接初始化非静态数据成员，可以使用统一初始化或等号，但不能使用小括号。
```cpp
class Widget
{
private:
    int x{0};  // fine, x's default value is 0
    int y = 0; // also fine
    int z(0);  // error!
};
```

不能拷贝的对象（比如 `std::atomic`，参考 Item 40（TODO link）），可以使用大括号或者小括号初始化，但是不能使用等号。
```cpp
std::atomic<int> ai1{0};  // fine
std::atomic<int> ai2(0);  // fine
std::atomic<int> ai3 = 0; // error!
```

通过上面两个例子，可以更好地理解统一初始化的意义：其他初始化方式总有不适用的场景。

统一初始化还有一个功能：禁止内置类型发生隐式窄化转换。
```cpp
double x, y, z;
int sum1{x + y + z}; // error! sum of doubles may not be expressible as int
```

等号初始化不会检查这一点，否则会使大量旧代码无法通过编译。小括号初始化也不会进行类似检查。
```cpp
int sum2(x + y + z);    // okay (value of expression truncated to an int)
int sum3 = x + y + z;   // ditto
```

大括号初始化还能避免 C++ 中令人困扰的解析问题。一个常见情况是，本想调用默认构造函数创建对象，结果却声明了一个函数。例如，可以用以下方式调用带参数的构造函数：
```cpp
Widget w1(10); // call Widget ctor with argument 10
```

如果使用类似的语法调用无参数构造函数，则会声明一个函数。
```cpp
Widget w2(); // most vexing parse! declares a function named w2 that returns a Widget!
```

由于函数声明不能使用这种大括号形式，统一初始化可以调用无参数构造函数。
```cpp
Widget w3{}; // calls Widget ctor with no args
```

统一初始化几乎可以用于任何场景，能够阻止窄化转换，还能调用无参数构造函数。它有很多优点，但为什么不应始终首选这种方式呢？

因为统一初始化的一些特性会导致非预期的行为。[Item 2](../ch01_Deducing_Types/02_Understand_auto_type_deduction.md) 介绍了它与 `auto` 配合时可能产生的问题。

当构造函数不涉及 `std::initializer_list` 时，使用大括号和小括号调用构造函数的行为是一致的。
```cpp
class Widget
{
public:
    Widget(int i, bool b);   // ctors not declaring
    Widget(int i, double d); // std::initializer_list params
};

Widget w1(10, true); // calls first ctor
Widget w2{10, true}; // also calls first ctor
Widget w3(10, 5.0);  // calls second ctor
Widget w4{10, 5.0};  // also calls second ctor
```

然而，如果有一个或多个接受 `std::initializer_list` 参数的构造函数，编译器会尽一切可能使用它们。现在给上面的类增加一个接受 `std::initializer_list<long double>` 参数的构造函数。
```cpp
class Widget
{
public:
    Widget(int i, bool b);                         // as before
    Widget(int i, double d);                       // as before
    Widget(std::initializer_list<long double> il); // added
};
```

这时，尽管与其他构造函数相比，该构造函数的元素类型 `long double` 看似是更差的匹配，上述四个对象中的 `w2` 和 `w4` 仍会使用接受 `std::initializer_list<long double>` 参数的构造函数。
```cpp
Widget w1(10, true); // uses parens and, as before, calls first ctor
Widget w2{10, true}; // uses braces, but now calls std::initializer_list ctor
                     // (10 and true convert to long double)
Widget w3(10, 5.0);  // uses parens and, as before, calls second ctor
Widget w4{10, 5.0};  // uses braces, but now calls std::initializer_list ctor
                     // (10 and 5.0 convert to long double)
```

拷贝和移动构造函数也可能被接受 `std::initializer_list` 参数的构造函数劫持。
```cpp
class Widget
{
public:
    Widget(int i, bool b);                         // as before
    Widget(int i, double d);                       // as before
    Widget(std::initializer_list<long double> il); // as before
    operator float() const;                        // convert to float
};
Widget w5(w4); // uses parens, calls copy ctor
Widget w6{w4}; // uses braces, calls std::initializer_list ctor
               // (w4 converts to float, and float converts to long double)
Widget w7(std::move(w4)); // uses parens, calls move ctor
Widget w8{std::move(w4)}; // uses braces, calls std::initializer_list ctor
                          // (for same reason as w6)
```

编译器调用接受 `std::initializer_list` 参数的构造函数的意愿十分强烈：即使因窄化转换而无法调用此类构造函数，也不会再尝试其他构造函数。
```cpp
class Widget
{
public:
    Widget(int i, bool b);                  // as before
    Widget(int i, double d);                // as before
    Widget(std::initializer_list<bool> il); // element type is now bool
    // no implicit conversion funcs
};

Widget w{10, 5.0}; // error! requires narrowing conversions
```

只有当参数无法转换为 `std::initializer_list` 的元素类型时，才会考虑其他构造函数。例如，在上面的例子中，把 `std::initializer_list<bool>` 换成 `std::initializer_list<std::string>`。由于无法将 `int` 和 `double` 转换为 `std::string`，编译器会重新考虑不接受 `std::initializer_list` 参数的构造函数。
```cpp
class Widget
{
public:
    Widget(int i, bool b);   // as before
    Widget(int i, double d); // as before

    // std::initializer_list element type is now std::string
    Widget(std::initializer_list<std::string> il); // no implicit conversion funcs
};

Widget w1(10, true); // uses parens, still calls first ctor
Widget w2{10, true}; // uses braces, now calls first ctor
Widget w3(10, 5.0);  // uses parens, still calls second ctor
Widget w4{10, 5.0};  // uses braces, now calls second ctor
```

最后讨论一种情况。假设有一个无参数构造函数，也有一个接受 `std::initializer_list` 参数的构造函数，这时写一对空大括号是什么意思呢？是调用无参数构造函数，还是调用接受 `std::initializer_list` 参数、但初始化列表为空的构造函数？

答案是前者。空大括号表示没有参数，而不是空的初始化列表。
```cpp
class Widget
{
public:
    Widget();                              // default ctor
    Widget(std::initializer_list<int> il); // std::initializer_list ctor
                                           // no implicit conversion funcs
};

Widget w1;   // calls default ctor
Widget w2{}; // also calls default ctor
Widget w3(); // most vexing parse! declares a function!
```

如果想调用接受 `std::initializer_list` 参数且初始化列表为空的构造函数，需要写两层大括号。
```cpp
Widget w4({}); // calls std::initializer_list ctor with empty list
Widget w5{{}}; // ditto
```

统一初始化的晦涩规则、`std::initializer_list` 和构造函数重载可能已经让人困惑。不禁要问：这与日常开发有什么关系？其影响远超想象；一个直接例子就是使用 `std::vector`。`std::vector` 有一个不接受 `std::initializer_list` 的双参数构造函数，用于指定元素数量和每个元素的值；它也有一个接受 `std::initializer_list` 参数的构造函数，用于指定容器包含的元素。如果定义一个 `std::vector<int>` 并传递两个参数，小括号和大括号的含义完全不同。
```cpp
std::vector<int> v1(10, 20); // use non-std::initializer_list ctor: create 10-element
                             // std::vector, all elements have value of 20
std::vector<int> v2{10, 20}; // use std::initializer_list ctor: create 2-element
                             // std::vector, element values are 10 and 20
```

这里有两个问题需要探讨。

第一个问题与类的作者有关：如果有一个或多个接受 `std::initializer_list` 参数的构造函数重载，调用方可能无法调用不接受 `std::initializer_list` 参数的构造函数。应尽量避免这种设计；也就是说，`std::vector` 的接口容易让人犯错。如果一个类没有 `std::initializer_list` 构造函数，调用方可以使用大括号调用不接受 `std::initializer_list` 参数的构造函数来创建对象。当增加一个接受 `std::initializer_list` 参数的构造函数重载后，原有的某些大括号初始化行为可能会发生变化：这个新构造函数会几乎屏蔽其他构造函数。

第二个问题是：我们应该使用哪种方式？两者各有优缺点；选定一种并保持一致即可。

如果你编写函数模板，选择哪种方式构造对象会更令人头疼，因为你不知道调用方期望哪种方式！例如，有一个接受任意数量、任意类型参数的函数，且其中需要构造某个对象，那么可以使用变参模板。
```cpp
template <typename T,     // type of object to create
          typename... Ts> // types of arguments to use
void doSomeWork(Ts &&...params)
{
    // create local T object from params...
}
```

上述伪代码中，构造局部对象有两种写法（[Item 25](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md) 会讲解 `std::forward`）。
```cpp
T localObject(std::forward<Ts>(params)...); // using parens
T localObject{std::forward<Ts>(params)...}; // using braces
```

调用方可以这样使用该函数：
```cpp
std::vector<int> v;
doSomeWork<std::vector<int>>(10, 20);
```

`doSomeWork` 应使用哪种方式构造对象呢？`doSomeWork` 的作者不知道，只有调用方知道。

标准库的 `std::make_unique` 和 `std::make_shared`（[Item 21](../ch04_Smart_Pointers/21_Prefer_std_make_unique_and_std_make_shared_to_direct_use_of_new.md)）也面临这个问题。它们选择使用小括号构造对象，并在文档中说明这一选择；这也是接口的一部分。

## Things to Remember
* Braced initialization is the most widely usable initialization syntax. It prevents narrowing conversions and avoids C++'s most vexing parse.
* During constructor overload resolution, braced initializers are matched to `std::initializer_list` parameters if at all possible, even if other constructors offer seemingly better matches.
* An example of where the choice between parentheses and braces can make a significant difference is creating a `std::vector<numeric type>` with two arguments.
* Choosing between parentheses and braces for object creation inside templates can be challenging.
