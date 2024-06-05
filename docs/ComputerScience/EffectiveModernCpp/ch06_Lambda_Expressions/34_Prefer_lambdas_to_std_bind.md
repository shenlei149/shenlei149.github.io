`std::bind` 历史悠久，可能早期不是这个名字罢了。但是到了 C++11，大部分场景下，lambda 都是更好的选择。到了 C++14，更强大，完全应该使用 lambda 而不是 `std::bind`。

`std::bind` 返回的函数对象称为绑定对象（`bind objects`）。

使用 lambda 而不是 `std::bind` 最重要的原因是更可读。假定我们有一个设置警报的函数：
```cpp
// typedef for a point in time (see Item 9 for syntax)
using Time = std::chrono::steady_clock::time_point;

// see Item 10 for "enum class"
enum class Sound
{
    Beep,
    Siren,
    Whistle
};

// typedef for a length of time
using Duration = std::chrono::steady_clock::duration;

// at time t, make sound s for duration d
void setAlarm(Time t, Sound s, Duration d);
```
假定在程序运行到某个时刻，我们需要设置一个一小时之后持续 30s 的报警。但是报警的声音没有确定。可以先一个 lambda 表达式包裹一下 `setAlarm`，调用 lambda 只需要指定声音即可。
```cpp
// setSoundL ("L" for "lambda") is a function object allowing a
// sound to be specified for a 30-sec alarm to go off an hour
// after it's set
auto setSoundL =
    [](Sound s)
{
    // make std::chrono components available w/o qualification
    using namespace std::chrono;
    setAlarm(steady_clock::now() + hours(1), // alarm to go off
             s,                              // in an hour for
             seconds(30));                   // 30 seconds
};
```
在 lambda 内部，就是一个正常的函数调用，即使不熟悉 lambda 表示式，也能看出来传递给 lambda 的 `s` 参数被传递给了 `setAlarm`。

C++14 提供了字面量的标准后缀，秒 `s`，毫秒 `ms`，小时 `h` 等。这些后缀实现在 `std::literals` 命名空间，所以上面的代码可以改写为
```cpp
auto setSoundL =
    [](Sound s)
{
    using namespace std::chrono;
    using namespace std::literals;     // for C++14 suffixes
    setAlarm(steady_clock::now() + 1h, // C++14, but
             s,                        // same meaning
             30s);                     // as above
};
```
下面我们使用 `std::bind` 完成相同的事情。下面的简化版本有一些问题，稍后会修复，正确的代码会更复杂，不过即使简单的版本也能说明一些问题。
```cpp
using namespace std::chrono; // as above
using namespace std::literals;
using namespace std::placeholders; // needed for use of "_1"

auto setSoundB = // "B" for "bind"
    std::bind(setAlarm,
              steady_clock::now() + 1h, // incorrect! see below
              _1,
              30s);
```
调用 `setSoundB` 会使用 `std::bind` 中指定的时间和持续时间来调用 `setAlarm`。对于不熟悉的来说，`_1` 是黑魔法。对于熟悉的人而言，也需要有一个转化，这个是传递给 `setSoundB` 的第一个参数，作为 `setAlarm` 的第二个参数。`std::bind` 不会给出 `setSoundB` 的参数类型，所以调用的人不得不查看 `setAlarm` 的参数列表然后确定传递给 `setSoundB` 的参数类型。

不过上述代码有点 bug。在 lambda 表达式中，`steady_clock::now() + 1h` 是 `setAlarm` 的参数，在调用 `setAlarm` 的时候求值。但是后面的实现中，`steady_clock::now() + 1h` 是传递给 `std::bind` 的参数，而不是传递给 `setAlarm` 的参数。这意味着当调用 `std::bind` 的时候，`steady_clock::now() + 1h` 就求值完成了。结果就是警报会在调用 `std::bind` 一个小时之后响而不是在调用 `setAlarm` 一个小时之后。

为了修复这个问题，必须告诉 `std::bind` 要延迟求值，直到 `setAlarm` 被调用的时候。结果就是必须嵌套一个的 `std::bind`。
```cpp
auto setSoundB =
    std::bind(setAlarm,
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s)
```
C++14 中允许忽略模板参数，但是 C++11 中不行，那么在 C++11 中完成的代码是
```cpp
using namespace std::chrono; // as above
using namespace std::placeholders;

auto setSoundB =
    std::bind(setAlarm,
              std::bind(std::plus<steady_clock::time_point>(),
                        std::bind(steady_clock::now),
                        hours(1)),
              _1,
              seconds(30));
```
这明显要比 lambda 表达式要复杂，且可读性更差。

当 `setAlarm` 有重载的时候，新的问题出现了。假定有一个重载版本，有四个参数，第四个参数可以指定音量。
```cpp
enum class Volume
{
    Normal,
    Loud,
    LoudPlusPlus
};
void setAlarm(Time t, Sound s, Duration d, Volume v);
```
lambda 表达式无需改动，因为根据重载规则会选择三参的 `setAlarm`。
```cpp
auto setSoundL = // same as before
    [](Sound s)
{
    using namespace std::chrono;
    setAlarm(steady_clock::now() + 1h, // fine, calls
             s,                        // 3-arg version
             30s);                     // of setAlarm
};
```
但是 `std::bind` 则无法通过编译。
```cpp
auto setSoundB =        // error! which
    std::bind(setAlarm, // setAlarm?
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);
```
因为编译期无法仅仅通过一个函数名就区分出 `std::bind` 使用的是哪一个版本的 `setAlarm`。

为了通过编译，那么需要让 `setAlarm` 转成合适的函数指针类型。
```cpp
using SetAlarm3ParamType = void (*)(Time t, Sound s, Duration d);

auto setSoundB =                                         // now
    std::bind(static_cast<SetAlarm3ParamType>(setAlarm), // okay
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);
```
lambda 和 `std::bind` 之前还有另外一个差异。`setSoundL` 内部调用 `setAlarm` 是正常的函数调用，编译器有可能可以做内联处理。
```cpp
setSoundL(Sound::Siren); // body of setAlarm may well be inlined here
```
但是对于 `std::bind` 来说，传入的是 `setAlarm` 的函数指针，这就是说 `setSoundB` 内部是通过函数指针来调用 `setAlarm`。编译器对于函数指针通常无法进行内联优化。
```cpp
setSoundB(Sound::Siren); // body of setAlarm is less likely to be inlined here
```
这就意味着使用 lambda 可能会生成比 `std::bind` 更快的代码。

上面的例子仅仅是调用一个函数。如果做更复杂的事情，那么就更倾向于 lambda 表达式了。比如给定一个值，判断是否在最大值（`highVal`）和最小值（`lowVal`）之间，`highVal` 和 `lowVal` 是局部变量。
```cpp
auto betweenL =
    [lowVal, highVal](const auto &val) // C++14
{ return lowVal <= val && val <= highVal; };
```
但是如果使用 `std::bind`，那么代码将相当晦涩。
```cpp
using namespace std::placeholders; // as above
auto betweenB =
    std::bind(std::logical_and<>(), // C++14
              std::bind(std::less_equal<>(), lowVal, _1),
              std::bind(std::less_equal<>(), _1, highVal));
```
`std::bind` 中的占位符 `_1` `_2` 很晦涩。但这不意味着只有占位符的行为不透明。假定有一个函数，可以创建 `Widget` 的压缩对象。
```cpp
enum class CompLevel
{
    Low,
    Normal,
    High
}; // compression level

Widget compress(const Widget &w, CompLevel lev); // make compressed copy of w
```
我们希望创建一个函数对象，对特定的 `Widget` 进行压缩，但是可以指定压缩等级。使用 `std::bind` 创建如下对象。
```cpp
Widget w;
using namespace std::placeholders;
auto compressRateB = std::bind(compress, w, _1);
```
当我们传入 `w` 进 `std::bind`，后续由 `compress` 调用。这个对象存储在 `compressRateB` 内部，但是它是如何存储的，按值还是按引用呢？这是有区别的。如果在调用 `std::bind` 和 `compressRateB` 之间修改了 `w`，按引用存储的 `w` 也会随之变化，但是按值传递就不会再有变化了。

答案是按值传递，我们只能记忆 `std::bind` 是如何工作的这件事本身。在调用 `std::bind` 的时候，没有任何记号能让人知道这一点。如果想按引用传递的话，需要在参数上加上 `std::ref`。而 lambda 表达式中，按值或按引用捕捉是显式的。
```cpp
auto compressRateL =          // w is captured by
    [w](CompLevel lev)        // value; lev is
{ return compress(w, lev); }; // passed by value
```
lambda 也能显式的指定参数的传递方式。上面代码可以很清晰的看出 `lev` 是按值传递。
```cpp
compressRateL(CompLevel::High); // arg is passed by value
```
但是 `std::bind` 的参数传递方式呢？
```cpp
compressRateB(CompLevel::High); // how is arg passed?
```
答案仍旧是只能记住 `std::bind` 是如何工作的：参数使用是引用传递，因为这些函数调用使用了完美转发。

`std::bind` 可读性更差，表达力更低，或许更低效。C++14 以后再也没有理由使用 `std::bind` 了。

后面作者花了一些篇幅介绍 C++11 使用 `std::bind` 的两个场景，一个是 [Item 32](./32_Use_init_capture_to_move_objects_into_closures.md) 中提到的移动对象的捕获，另一个是多态函数对象，C++14 可以使用 `auto` 来解决。

## Things to Remember
* Lambdas are more readable, more expressive, and may be more efficient than using `std::bind`.
* In C++11 only, `std::bind` may be useful for implementing move capture or for binding objects with templatized function call operators.

