在 C++98/03 时代，表达式分为两类

- 左值（`lvalue`）
- 右值（`rvalue`）

C++11 为了支持移动语义，把值类别细分成五种：

- 左值（`lvalue`）
- 右值（`rvalue`）
- 纯右值（`prvalue`）
- 将亡值（`xvalue`）
- 广义左值（`glvalue`）

具体关系如下：`glvalue` 由 `lvalue` 和 `xvalue` 组成，`rvalue` 由 `xvalue` 和 `prvalue` 组成。`xvalue` 同时属于两边。
```
                 expression
                /          \
           glvalue          rvalue
           /     \          /     \
       lvalue   xvalue   xvalue   prvalue
```
三个基本的值类别：

- 左值（`lvalue`）：具有标识（`identity`）的表达式，例如有名字，通常可以取地址。
- 将亡值（`xvalue`）：具有标识、且资源可以被移动和复用的表达式，通常所指对象即将结束生命周期。
- 纯右值（`prvalue`）：没有标识的右值，不能取地址，可以移动。

为了支持拷贝省略（`copy elision`），C++17 重新定义了广义左值和纯右值：

- 广义左值（`glvalue`）：求值结果是确定某个对象、位域或函数的标识。对象和函数通常有内存地址，位域没有独立地址。
- 纯右值（`prvalue`）：求值的目的是初始化某个对象或位域，或计算运算符操作数的值。

C++17 规定，类或数组类型的 prvalue 不需要先创建临时副本（`temporary copies`）。用它初始化目标时，直接在目标地址构造。期间没有拷贝也没有移动，因此该类型不必支持这些操作，编译器可以安全地去掉这些步骤。这种直接在目标处初始化发生在以下两种场景：

- 函数返回一个 prvalue。
- 用 prvalue 初始化某个对象。

不过以下情况仍然会创建临时对象：

- 将 prvalue 绑定到引用。
- 访问 prvalue 对象的成员变量。
- 对数组类型的 prvalue 进行下标访问。
- 数组类型的 prvalue 退化为指针。
- 对类类型的 prvalue 进行派生类到基类的转换。
- prvalue 本身作为弃值表达式（`discarded-value expression`）出现时，例如单独写 `T();`，通常是为了构造函数或析构函数的副作用。
