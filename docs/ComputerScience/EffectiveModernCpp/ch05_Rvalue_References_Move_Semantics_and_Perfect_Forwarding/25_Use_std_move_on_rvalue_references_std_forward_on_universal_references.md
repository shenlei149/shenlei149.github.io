右值引用只绑定可以移动的对象。如果参数是右值引用，那么绑定的对象可能会被移动。
```cpp
class Widget
{
    // rhs definitely refers to an object eligible for moving
    Widget(Widget &&rhs);
};
```
会给这样的函数传递一个具有右值性被函数使用的对象。那么需要将绑定的对象转换成右值。[Item 23](./23_Understand_std_move_and_std_forward.md) 告诉我们，`std::move` 就是为了这个目的而设计的。
```cpp
class Widget
{
public:
    Widget(Widget &&rhs) // rhs is rvalue reference
        : name(std::move(rhs.name)),
          p(std::move(rhs.p))
    {
    }

private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};
```
[Item 24](./24_Distinguish_universal_references_from_rvalue_references.md) 告诉我们，通用引用也可以绑定一个可以移动的对象。仅当右值绑定到通用引用上的时候，通用引用需要转换为右值。[Item 23](./23_Understand_std_move_and_std_forward.md) 告诉我们，`std::forward` 就是做这个事情的。
```cpp
class Widget
{
public:
    template <typename T>
    void setName(T &&newName) // newName is universal reference
    {
        name = std::forward<T>(newName);
    }
};
```
当把右值引用传递给其他函数的时候，因为右值引用总是绑定的右值，所以使用 `std::move` 无条件的将其转换为右值；当把通用引用传递给其他函数的时候，使用 `std::forward` 在其绑定了右值的时候转换成右值。

[Item 23](./23_Understand_std_move_and_std_forward.md) 解释了使用在右值引用上可以使用 `std::forward`，但是代码冗长，容易出错，所以应该避免在右值引用上使用 `std::forward`。不过在通用引用上使用 `std::move` 问题更大，可能会不经意的改变左值（比如局部变量）。
```cpp
class Widget
{
public:
    template <typename T>
    void setName(T &&newName) // universal reference
    {                         // compiles, but is bad, bad, bad!
        name = std::move(newName);
    }

private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

std::string getWidgetName(); // factory function

Widget w;
auto n = getWidgetName(); // n is local variable
w.setName(n);             // moves n into w!
                          // n's value now unknown
```
临时变量 `n` 传入了 `w.setName`，调用者可能会认为这个函数是只读的。但是 `setName` 的内部使用 `std::move` 无条件的把引用参数转成了右值，`n` 的值被移动到了 `w.name`，在 `w.setName` 完成之后，`n` 的值不确定，这完全出乎调用者的意料。

或许你会认为 `setName` 的参数不应该声明为通用引用。根据 [Item 24](./24_Distinguish_universal_references_from_rvalue_references.md)，通用引用不应该是 `const` 的。那么可能的方案是重载，一个是 `const` 左值，一个是右值。
```cpp
class Widget
{
public:
    void setName(const std::string &newName) // set from const lvalue
    {
        name = newName;
    }
    void setName(std::string &&newName) // set from rvalue
    {
        name = std::move(newName);
    }
};
```
这可以工作，但是有缺陷。首先，维护两份代码的成本比维护一个模板要高。第二，可能会有性能问题。比如如下使用 `setName`


##
