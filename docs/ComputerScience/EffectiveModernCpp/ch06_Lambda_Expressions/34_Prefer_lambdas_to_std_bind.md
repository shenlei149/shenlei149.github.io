`std::bind` 历史悠久，可能早期不是这个名字罢了。但是到了 C++11，大部分场景下，lambda 都是更好的选择。到了 C++14，更强大，完全应该使用 lambda 而不是 `std::bind`。

## Things to Remember
* Lambdas are more readable, more expressive, and may be more efficient than using `std::bind`.
* In C++11 only, `std::bind` may be useful for implementing move capture or for binding objects with templatized function call operators.

