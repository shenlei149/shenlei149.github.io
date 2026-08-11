默认情况下，`std::unique_ptr` 的大小和裸指针一样，大部分操作（包括解引用）的指令数和裸指针也是一样的。这意味在时间和空间受限的情况下也可以使用。如果一个裸指针足够快且足够小，那么就可以使用 `std::unique_ptr`。

`std::unique_ptr` 的语义是拥有排他性的所有权。一个非 `null` 的 `std::unique_ptr` 指针，拥有指向的对象。移动 `std::unique_ptr` 指针就转移了所有权，原始 `std::unique_ptr` 被设置成了 `null`。不允许拷贝 `std::unique_ptr`，因为如果允许的话，会出现两个 `std::unique_ptr` 指向同一个对象，且都拥有这个对象的所有权，和之前的语义矛盾。`std::unique_ptr` 析构时，默认情况下会调用 `delete` 销毁指向的对象。

一个使用 `std::unique_ptr` 的典型场景是工厂函数返回基类指针。假定有如下继承关系的一些类：
```cpp
class Investment
{
};
class Stock : public Investment
{
};
class Bond : public Investment
{
};
class RealEstate : public Investment
{
};
```
工厂函数根据条件构造一个对象，返回这个对象，调用者有责任销毁这个对象。`std::unique_ptr` 完美匹配这个场景，调用者获得了所有权，同时，在其析构时，会 `delete` 对象。工厂函数的签名如下：
```cpp
template <typename... Ts>       // return std::unique_ptr
std::unique_ptr<Investment>     // to an object created
makeInvestment(Ts &&...params); // from the given args
```
调用者在某个作用域内使用 `std::unique_ptr`
```cpp
{
    auto pInvestment =             // pInvestment is of type
        makeInvestment(arguments); // std::unique_ptr<Investment>
} // destroy *pInvestment
```

也可以用于所有权转移的场景。比如工厂函数返回 `std::unique_ptr`，将其移动到一个容器，容器又移动到某个对象的成员变量中。当这个对象析构的时候，最后会调用 `std::unique_ptr` 的析构并销毁工厂函数创建的对象。

我们可以自定义 `std::unique_ptr` 的删除器（`deleter`），传入某个函数（可执行对象，lambda 表达式等）即可，当 `std::unique_ptr` 对象销毁时，会调用自定义的删除器（函数）。假定我们需要再上面工厂函数创建的对象销毁前打一行日志，那么可以这么实现。
```cpp
// custom deleter (a lambda expression)
auto delInvmt = [](Investment *pInvestment)
{
    makeLogEntry(pInvestment);
    delete pInvestment;
};

template <typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)> // revised return type
makeInvestment(Ts &&...params)
{
    std::unique_ptr<Investment, decltype(delInvmt)> // ptr to be
        pInv(nullptr, delInvmt);                    // returned
    if (/* a Stock object should be created */)
    {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if (/* a Bond object should be created */)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (/* a RealEstate object should be created */)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
    return pInv;
}
```
先从调用者角度分析，将 `makeInvestment` 返回结果放到一个 `auto` 变量中，我们什么也不要做，`std::unique_ptr` 会在合适的时机调用自定义的删除器销毁对象，且能保证只销毁一次。

现在从实现角度分析：
* `delInvmt` 是 `makeInvestment` 返回对象的自定义删除器。自定义删除器接受指向对象的裸指针，然后做必要的事情。这里我们先调用 `makeLogEntry` 输出日志，然后调用 `delete` 删除对象。使用 lambda 对象很方便。后面会给出更好的例子。
* 当使用自定义删除器的时候，`std::unique_ptr` 的第二个模板参数必须要指定。这里是 `delInvmt` 的类型，所以`makeInvestment` 返回类型是 `std::unique_ptr<Investment, decltype(delInvmt)>`。
* `makeInvestment` 基本策略就是先构造一个指向 `null` 的 `std::unique_ptr`，然后指向合适的类型的对象，返回。这里需要将自定义删除器 `delInvmt` 与 `pInv` 关联，因此将 `delInvmt` 作为第二个实参传入。
* 不能将原始指针直接赋值给 `std::unique_ptr`，原因这里涉及隐式转化，但是隐式转化可能会出问题，所以 C++11 禁止这么做。这里通过 `reset` 让 `std::unique_ptr` 接管新创建的对象的所有权。
* 调用 `new` 的时候，使用 `std::forward` 完美转发传给 `makeInvestment` 的参数。参见 [Item 25](../ch05_Rvalue_References_Move_Semantics_and_Perfect_Forwarding/25_Use_std_move_on_rvalue_references_std_forward_on_universal_references.md)。这么做的目的是将调用者传递的所有信息用于构造对象。
* 不管 `makeInvestment` 具体创建的对象类型是什么，`Stock, Bond, RealEstate` 之一，但是 lambda 表达式只接受类型 `Investment*` 的参数。需要通过指向基类的指针销毁派生类，那么基类析构函数必须是虚函数。
```cpp
class Investment
{
public:
    virtual ~Investment(); // essential design component
};
```
C++14 中可以推导函数的返回类型，那么可以简化函数签名
```cpp
template<typename... Ts>
auto makeInvestment(Ts&&... params) // C++14
```

如果有了自定义删除器，那么 `std::unique_ptr` 的大小可能会从一个字的大小增长到两个字。增长多少主要依赖于有多少状态需要存储。无状态的函数对象（比如不捕获任何变量的 lambda 表达式）就不会使得 `std::unique_ptr` 的大小增加。所以，既可以用 lambda 又可以用不捕获变量的 lambda 表达式时，推荐后者。
```cpp
auto delInvmt1 = [](Investment *pInvestment) // custom
{                                            // deleter
    makeLogEntry(pInvestment);               // as
    delete pInvestment;                      // stateless
};                                           // lambda

template <typename... Ts>                        // return type
std::unique_ptr<Investment, decltype(delInvmt1)> // has size of
makeInvestment(Ts &&...args);                    // Investment*

void delInvmt2(Investment *pInvestment) // custom
{                                       // deleter
    makeLogEntry(pInvestment);          // as function
    delete pInvestment;
}

template <typename... Ts>               // return type has
std::unique_ptr<Investment,             // size of Investment*
                void (*)(Investment *)> // plus at least size
makeInvestment(Ts &&...params);         // of function pointer!
```
有状态的函数对象会使 `std::unique_ptr` 大小增加不少，如果过大而不能接受，需要重新考虑相关的设计了。

除了工厂函数场景适合 `std::unique_ptr` 之外，Pimpl Idiom 场景也试用。[Item 22](./22_When_using_the_Pimpl_Idiom_define_special_member_functions_in_the_implementation_file.md) 会讲解。

`std::unique_ptr` 有两种形式，一种是指向一个对象，`std::unique_ptr<T>`，一种是指向数组，`std::unique_ptr<T[]>`。指向那种数据类型是没有歧义的，`std::unique_ptr` 的 API 设计会匹配相应的类型。比如指向一个对象时，是没有索引 `[]` 操作的，反之指向数组的话，没有解引用操作 `->, *`。

使用指向数组的 `std::unique_ptr` 的情况不多，因为此时 `std::array, std::vector, std::string` 是更好的选择。唯一使用 `std::unique_ptr<T[]>` 的场景是和 C 接口打交道，想将其创建的堆上数组接管。

`std::unique_ptr` 是排他性的，不过，可以很容易的转化为 `std::shared_ptr`。
```cpp
// converts std::unique_ptr to std::shared_ptr
std::shared_ptr<Investment> sp = makeInvestment(arguments);
```
这也是 `std::unique_ptr` 适合作为工厂函数返回类型的一个关键原因。工厂函数不知道调用者想要排他性的所有权还是共享性的所有权。返回 `std::unique_ptr`，调用者可以根据需要自行选择。

## Things to Remembe
* `std::unique_ptr` is a small, fast, move-only smart pointer for managing resources with exclusive-ownership semantics.
* By default, resource destruction takes place via delete, but custom deleters can be specified. Stateful deleters and function pointers as deleters increase the size of `std::unique_ptr` objects.
* Converting a `std::unique_ptr` to a `std::shared_ptr` is easy.
