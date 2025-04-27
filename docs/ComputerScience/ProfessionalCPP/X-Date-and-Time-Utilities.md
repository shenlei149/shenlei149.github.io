## 编译期有理数 `std::ratio`
`std::ratio` 是编译期有理数，需要传入两个参数分别表示分子和分母。`std::ratio<>` 是类模板，不是普通的类，实例化之后也只是一个具体的类，是类型而不是对象。对于分子和分母，会进行最简化，其中分母一定是整数，分子会根据实际情况调整符号。

`num` `den` 这两个变量表示分子和分母。`ratio_add` `ratio_subtract` `ratio_multiply` `ratio_divide` 返回的也是具体的 `std::ratio<>` 的类型。`ratio_equal` `ratio_not_equal` `ratio_less` `ratio_less_equal` `ratio_greater` `ratio_greater_equal` 用于比较，由于是比较两个类型，因为返回类型是 `std::bool_constant` 这种新类型，是 `std::integral_constant` 的具体类型，`std::bool_constant<true>` 是 `integral_constant<bool, true>`。通过 `::value` 获取值。

标准库定义了一系列常量。
```cpp
using yocto = ratio<1, 1'000'000'000'000'000'000'000'000>;
using zepto = ratio<1, 1'000'000'000'000'000'000'000>;
using atto = ratio<1, 1'000'000'000'000'000'000>;
using femto = ratio<1, 1'000'000'000'000'000>;
using pico = ratio<1, 1'000'000'000'000>;
using nano = ratio<1, 1'000'000'000>;
using micro = ratio<1, 1'000'000>;
using milli = ratio<1, 1'000>;
using centi = ratio<1, 100>;
using deci = ratio<1, 10>;
using deca = ratio<10, 1>;
using hecto = ratio<100, 1>;
using kilo = ratio<1'000, 1>;
using mega = ratio<1'000'000, 1>;
using giga = ratio<1'000'000'000, 1>;
using tera = ratio<1'000'000'000'000, 1>;
using peta = ratio<1'000'000'000'000'000, 1>;
using exa = ratio<1'000'000'000'000'000'000, 1>;
using zetta = ratio<1'000'000'000'000'000'000'000, 1>;
using yotta = ratio<1'000'000'000'000'000'000'000'000, 1>;
```

## 持续时间 `duration`
`std::chrono` 命名空间下提供类 `duration` 表示持续时间，这也是一个类模板，第一个参数是 `duration` 存储周期个数的类型，第二个参数是周期类型，默认值是 `class Period = ratio<1>` 表示一秒。

`duration` 支持加减乘除模运算等。`count()` 返回有多少个周期，`zero()` 表示 `duration` 是否为零，`min()` `man()` 返回支持的最小和最大的周期数。还支持 `floor()` `ceil()` `round()` `abs()`。

`duration` 支持不同类型之间的转化，但是有些许限制，主要原因是周期类型，可以向更精确的方向转化而不能反之，除非显式使用 `duration_cast`。

标准库定义了如下预定义类型。
```cpp
using nanoseconds = duration<X 64 bits, nano>;
using microseconds = duration<X 55 bits, micro>;
using milliseconds = duration<X 45 bits, milli>;
using seconds = duration<X 35 bits>;
using minutes = duration<X 29 bits, ratio<60>>;
using hours = duration<X 23 bits, ratio<3'600>>;
using days = duration<X 25 bits, ratio_multiply<ratio<24>, hours::period>>;
using weeks = duration<X 22 bits, ratio_multiply<ratio<7>, days::period>>;
using years = duration<X 17 bits, ratio_multiply<ratio<146'097, 400>, days::period>>;
using months = duration<X 20 bits, ratio_divide<years::period, ratio<12>>>;
```
其中 `X` 依赖于编译器，它是有符号整数，保证有足够的 bits 数。

`h` `min` `s` `ms` `us` `ns` 是常量后缀。

`hh_mm_ss` 也是一个模板类，可以将一个 `duration` 切分成 `hours()` `minutes()` `seconds()` `subseconds()` 几个部分。`is_negative() ` `true` 的话表示 `duration` 是负数。

## 时钟 `clock`
标准库提供了多种时钟，不过常用的是 1） `system_clock`，表示 UTC 系统时间，是挂钟时钟时间；2）`steady_clock` 与真实时间无关，确保递增，用于测量持续时间。

静态函数 `now()` 返回当前时间，`is_steady()` 表示时钟是否稳定递增。`system_clock` 还有两个函数可以方便地与 C 风格 `time_t` 交互，分别是 `from_time_t()` `to_time_t()`。

`high_resolution_clock` 是 `system_clock` `steady_clock` 之一，依赖于编译器。

## 时间点 time_point
`std::time_point<Clock, Duration = typename Clock::duration>` 是一个模板类，表示从对应 `clock` 的 epoch 开始的 `duration`。一个时间点和持续时间进行加减运算相当合理，但是两个时间点相加没有意义，因此不支持。
