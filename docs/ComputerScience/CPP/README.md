
## 目录
```
cpp/
├── README.md                     # 目录，C++ 体系总览
│
```

## 准大纲
下面是 deepseek 帮忙生成的大纲，我可以参考这个目录结构来构建 C++ 知识体系。
```
cpp/
├── _index.md                     # C++体系总览：学习路径、前置要求、经典书籍索引

├── 01-core/                      # 核心语言基础（11个文件）
│   ├── _index.md                 # 本部分概览：所有不涉及类与模板的基础语言构件
│   │
│   ├── type-system.md            # 基础类型(int/double/char等)的字面量与表示范围、
│   │                             # 类型别名(using/typedef)、cv限定符(const/volatile)、
│   │                             # auto类型推导规则与陷阱(大括号初始化、auto&、auto&&)、
│   │                             # decltype推导规则与decltype(auto)
│   │
│   ├── expressions.md            # 表达式求值顺序与副作用(前置vs后置++的差异)、
│   │                             # 算术/逻辑/位运算符的完整覆盖、条件运算符(?:)的
│   │                             # 类型确定规则(两个分支的公共类型)、逗号运算符、
│   │                             # 三路比较<=>(C++20)：强序/弱序/部分序的语义区别、
│   │                             # 默认生成规则(成员逐一按声明顺序比较)、与==的交互、
│   │                             # 自定义类型的<=>实现、运算符优先级与结合性速查
│   │
│   ├── control-flow.md           # if/switch/for/while基础控制流、范围for的底层原理
│   │                             # (begin/end非成员查找与ADL)、if constexpr(C++17)与
│   │                             # 普通if的区别(编译期分支消除)、初始化语句(C++17/20)
│   │
│   ├── functions.md              # 函数声明与定义、参数传递方式(值/引用/指针)的选择原则、
│   │                             # 默认实参与重载的交互陷阱、inline的现代语义(
│   │                             # 链接而非优化提示)、函数指针与std::function的使用场景
│   │
│   ├── overload-resolution.md    # 重载决议完整流程：名字查找→ADL(实参依赖查找)→
│   │                             # 模板推导→可行函数集→最佳匹配(标准转换vs用户定义转换)、
│   │                             # 二义性与消歧(f(v) vs f({v}))、SFINAE在重载中的作用、
│   │                             # 已删除函数的重载参与(导致编译错误)
│   │
│   ├── lambdas.md                # Lambda语法演进(C++11/14/17/20)：捕获列表([=]/[&]/[this]/
│   │                             # [*this](C++17)/显式捕获)、泛型lambda(auto参数C++14)、
│   │                             # 模板lambda(C++20)、初始化捕获([x = std::move(val)])、
│   │                             # 无状态lambda与函数指针的隐式转换
│   │
│   ├── structured-bindings.md    # 结构化绑定(C++17)：绑定到数组(按索引)、元组(按get<>)、
│   │                             # 结构体/类(按public成员)、值绑定vs引用绑定、
│   │                             # 与auto推导的交互、底层实现(编译器生成的匿名对象)
│   │
│   ├── constexpr.md              # constexpr变量(编译期常量)与函数(C++11→20进化)、
│   │                             # consteval立即函数(C++20)：必须产生编译期结果、
│   │                             # constinit(C++20)保证静态初始化、编译期new(C++20)、
│   │                             # 编译期vector/string使用限制(不可传递到运行时)
│   │
│   ├── preprocessor.md           # 宏定义与展开(#字符串化/##拼接)、条件编译(#if/#ifdef/
│   │                             # #elif/#else/#endif)、include guard与#pragma once、
│   │                             # __has_include(C++17)、预定义宏(__LINE__/__func__/
│   │                             # __DATE__/__cplusplus)、宏的常见陷阱(多次求值)
│   │
│   └── modules.md                # 模块(C++20)：import/export语法、模块分区(:part)、
│                                 # 全局模块片段(module;)、头文件单元(import <...>)、
│                                 # 编译速度优势实测、从#include到import的渐进迁移策略
│
├── 02-class/                     # 类设计与运行时多态（14个文件）
│   ├── _index.md                 # 本部分概览：定义新类型的编译期机制与运行时行为
│   │
│   ├── class-fundamentals.md     # class vs struct的唯一区别(默认访问权限)、
│   │                             # 成员函数声明/定义、this指针、静态成员与常量成员、
│   │                             # 成员函数限定符(const/&/&&/volatile重载)、
│   │                             # mutable与const成员函数
│   │
│   ├── constructors-destructors.md # 默认/转换构造函数、委托构造(C++11)避免代码重复、
│   │                             # 继承构造(using Base::Base C++11)、
│   │                             # 析构函数与析构顺序(基类→成员→自身)、
│   │                             # 拷贝/移动构造的简要引入(详细讨论见03-resource)
│   │
│   ├── member-layout.md          # 成员对齐(alignof/alignas)、填充字节(padding)与
│   │                             # 内存浪费、位域声明与跨平台问题(符号位/字节序)、
│   │                             # 空基类优化(EBO)与no_unique_address(C++20)、
│   │                             # POD/trivial/standard-layout类型特征与用途
│   │
│   ├── access-control.md         # public/protected/private的访问规则(作用于类而非对象)、
│   │                             # friend类/函数/成员函数、protected继承时的访问变化、
│   │                             # 友元的传递性与封装权衡
│   │
│   ├── conversions.md            # 隐式转换路径(转换构造函数/类型转换运算符)、
│   │                             # explicit与explicit(bool)(C++20条件explicit)、
│   │                             # 上下文转换到bool(if/while/!中的隐式bool)、
│   │                             # 安全布尔惯用法(从operator bool到explicit operator bool)
│   │
│   ├── operator-overloading.md   # 可重载运算符一览(42个)、成员vs非成员的选择原则、
│   │                             # 对称运算符(==/<=>/-/+)的最佳实践(非成员函数)、
│   │                             # 不能重载的运算符(. / .* / :: / ?: / sizeof/typeid等)、
│   │                             # operator new/delete重载与分配器接口
│   │
│   ├── inheritance.md            # public/protected/private继承的语义区别(is-a/has-a/
│   │                             # 实现继承)、名字隐藏(派生类名字遮蔽基类)、
│   │                             # using声明引入基类重载集、final类(C++11)阻止继承、
│   │                             # 继承与组合的选择原则(组合优先于继承)
│   │
│   ├── multiple-inheritance.md   # 多继承语法与构造顺序(基类声明顺序)、
│   │                             # 菱形继承问题(最派生类含多个基类子对象)、
│   │                             # 虚基类初始化规则(最派生类负责初始化虚基类)、
│   │                             # 虚基类vs非虚基类的内存布局差异(vbptr vs 直接嵌入)
│   │
│   ├── virtual-functions.md      # 虚函数声明与覆盖、vtable与vptr的典型实现(单继承模型)、
│   │                             # override保证覆盖意图、final阻止进一步覆盖、
│   │                             # 虚析构函数的重要性(delete基类指针时的UB)、
│   │                             # 虚函数的默认参数陷阱(静态绑定默认值)
│   │
│   ├── nvi-and-interfaces.md     # 纯虚函数(=0)与抽象类(不能实例化)、
│   │                             # 接口类的最佳实践(纯虚+虚析构+无数据成员)、
│   │                             # 非虚接口惯用法(NVI)：公开非虚函数调用私有虚函数、
│   │                             # NVI的ABI稳定性优势与前置/后置条件保证
│   │
│   ├── rtti.md                   # typeid运算符与type_info类(name()/hash_code())、
│   │                             # dynamic_cast工作原理(运行时遍历继承树)、
│   │                             # 向下转型的安全条件(必须有虚函数)、
│   │                             # RTTI的性能开销(字符串比较/vtable查找)与禁用场景(-fno-rtti)
│   │
│   ├── exceptions.md             # throw/try/catch语法、异常对象拷贝与切片(值捕获vs引用捕获)、
│   │                             # noexcept声明(C++11)与noexcept运算符(条件noexcept)、
│   │                             # 异常安全三个保证(基本/强/不抛)、栈展开(销毁局部对象)、
│   │                             # 异常中性(exception neutral)设计与构造函数中的异常
│   │
│   └── object-lifetime.md        # 自动/静态/动态/线程存储期、thread_local(C++11)、
│                                 # 程序启动时全局对象初始化顺序问题(SIOF)与constexpr规避、
│                                 # 生命周期中的vtable切换(基类构造时指向基类vtable)
│
├── 03-resource/                  # 值语义与资源管理（9个文件）
│   ├── _index.md                 # 本部分概览：C++最独特的语言哲学——值语义
│   │
│   ├── value-categories.md       # C++值类别体系全景：左值(lvalue)/纯右值(prvalue)/
│   │                             # 将亡值(xvalue)/泛左值(glvalue)/右值(rvalue)、
│   │                             # C++17简化模型：值与对象的明确区分、
│   │                             # decltype与值类别的对应规则((x) vs (expr))
│   │
│   ├── copy-control.md           # 拷贝构造/赋值、移动构造/赋值的完整语法、
│   │                             # 三/五/零法则(如果自定义其一，则需全部关注)、
│   │                             # =delete(禁止)与=default(恢复默认)、
│   │                             # 浅拷贝vs深拷贝(资源所有权)、
│   │                             # 编译器隐式生成规则表(何时自动生成何时抑制)
│   │
│   ├── move-semantics.md         # std::move的本质(static_cast到右值引用)、
│   │                             # 移动后对象的状态保证(有效但未指定，如nullptr)、
│   │                             # RVO/NRVO返回值优化(复制消除的编译器保证C++17)、
│   │                             # 隐式移动(C++20)：return局部变量时自动尝试移动
│   │
│   ├── forwarding-references.md  # 转发引用(万能引用T&&)的推导规则(右值遇T→T,遇T&→T&)、
│   │                             # 引用折叠规则(T& &→T&, T&& &→T&, T& &&→T&, T&& &&→T&&)、
│   │                             # 转发引用的重载陷阱(与普通函数重载的交互)
│   │
│   ├── perfect-forwarding.md     # std::forward实现原理(条件转换右值)、
│   │                             # 完美转发工厂函数模式(make_shared/emplace_back)、
│   │                             # 参数包完美转发(Args&&... args)、
│   │                             # 常见失败场景(不能完美转发的表达式：0/NULL/位域)
│   │
│   ├── raii.md                   # 资源获取即初始化：构造获取、析构释放、
│   │                             # 标准库RAII示例(lock_guard/fstream)、
│   │                             # ScopeGuard惯用法(手动实现与std::experimental::scope_exit)、
│   │                             # RAII与异常安全的天然契合(栈展开自动调用析构)
│   │
│   ├── unique-ptr.md             # unique_ptr：独占所有权语义、删除器(函数指针vs无状态
│   │                             # 函数对象vs有状态函数对象)对大小的影响、make_unique(C++14)、
│   │                             # 数组特化unique_ptr<T[]>与shared_ptr<T[]>的区别、
│   │                             # 与raw指针的互操作(release/get/reset)、
│   │                             # 使用最佳实践(工厂函数/容器/PIMPL)
│   │
│   ├── shared-ptr.md             # shared_ptr：引用计数机制(控制块)、
│   │                             # make_shared的内存合并优势(一次分配对象+控制块)、
│   │                             # aliasing构造函数(共享所有权但指向不同对象)、
│   │                             # enable_shared_from_this的CRTP用法与陷阱、
│   │                             # 使用最佳实践(避免循环引用/线程安全)
│   │
│   └── weak-ptr.md               # weak_ptr：打破循环引用的唯一方式、lock()与expired()、
│                                 # 观察者模式(Observer)的现代实现、缓存模式的实现技巧、
│                                 # 使用最佳实践(检查lock()返回值/expired()的TOCTOU问题)
│
├── 04-template/                  # 模板与编译期编程（12个文件）
│   ├── _index.md                 # 本部分概览：参数化类型与编译期计算，分基础(前7)与进阶(后5)
│   │
│   ├── function-templates.md     # 函数模板定义与模板参数列表、模板参数推导规则、
│   │                             # auto推导(C++17简化函数模板)、模板参数vs函数参数、
│   │                             # 函数模板重载与偏序(partial ordering)规则
│   │
│   ├── class-templates.md        # 类模板定义与实例化(隐式/显式)、
│   │                             # 成员函数定义位置(类内inline vs类外模板声明)、
│   │                             # 成员模板与模板成员的区别、模板模板参数、
│   │                             # friend与模板的组合(友元注入)
│   │
│   ├── template-deduction.md     # 模板实参推导(ADT)的完整规则(非推导上下文)、
│   │                             # CTAD(C++17)：类模板实参推导(隐式推导指引)、
│   │                             # 用户定义推导指引(自定义构造函数到模板参数的映射)、
│   │                             # 聚合的CTAD(C++20)
│   │
│   ├── specialization.md         # 全特化语法(函数的全特化 vs 模板函数重载)、
│   │                             # 偏特化的匹配规则与偏序(partial ordering)、
│   │                             # 显式实例化(explicit instantiation)减少编译时间、
│   │                             # 隐式实例化点(POI)与ODR问题(跨翻译单元的一致实例化)
│   │
│   ├── variadic-templates.md     # 参数包声明(typename... Args/auto...)、
│   │                             # 包展开位置(表达式/类型/基类/初始化列表)、
│   │                             # sizeof...(Args)获取参数个数、
│   │                             # 递归解包(头元素+递归)vs折叠表达式(C++17一元/二元折叠)
│   │
│   ├── alias-and-variable-templates.md # 别名模板(using Alias = ...)作为类型元函数、
│   │                             # 变量模板(C++14)template<T> constexpr T value = ...、
│   │                             # 标准库_t(类型别名)/_v(变量模板)便捷接口、
│   │                             # 模板参数默认值
│   │
│   ├── sfinae.md                 # SFINAE规则详解：替换失败不是错误(仅直接上下文)、
│   │                             # std::enable_if的三种放置位置(返回值/模板参数/默认值)、
│   │                             # void_t检测惯用法与detection idiom(C++20前)、
│   │                             # if constexpr替代SFINAE的场景
│   │
│   ├── concepts.md               # Concepts(C++20)：concept定义语法(constexpr bool)、
│   │                             # requires表达式(简单/类型/复合/嵌套)、requires子句、
│   │                             # 标准库concept全览(iterator/range/invocable/regular等)、
│   │                             # concept vs SFINAE的对比(错误信息/重载选择)
│   │
│   ├── type-traits.md            # <type_traits>完整体系：类型查询(is_void/is_pointer/
│   │                             # is_base_of/is_same等)、类型变换(remove_const/
│   │                             # remove_reference/add_pointer/decay等)、
│   │                             # 属性查询(is_trivial/is_standard_layout/
│   │                             # is_trivially_copyable)、类型运算(conditional/
│   │                             # common_type/underlying_type)
│   │
│   ├── tmp-basics.md             # 编译期计算入门：递归模板终止条件(全特化)、
│   │                             # 整数常量与integral_constant(值→类型映射)、
│   │                             # 编译期选择conditional_t(类型)/conditional(值)、
│   │                             # 编译期循环(模板递归展开/折叠表达式)
│   │
│   └── tmp-advanced.md           # 进阶元编程：表达式模板(expression templates)
│                                 # 优化数值计算(Eigen库)、Policy-Based Design与Mixin、
│                                 # tag dispatch惯用法(iterator_category)、
│                                 # 元函数与高阶元函数、Typelist与相关算法(查找/排序/变换)
│
├── 05-containers-and-algorithms/ # 容器与算法（13个文件）
│   ├── _index.md                 # 本部分概览：标准库数据组织与操作的核心组件
│   │
│   ├── sequence-containers.md    # vector(动态数组、增长因子1.5→2、reserve避免重新分配)、
│   │                             # deque(双端队列、块状内存布局)、list(双向链表、splice
│   │                             # 移动节点O(1))、array(C++11编译期大小)、
│   │                             # forward_list(C++11单向链表)、容器选择决策树
│   │
│   ├── associative-containers.md # set/map(红黑树实现、节点稳定地址)、
│   │                             # multiset/multimap(允许重复键)、
│   │                             # 自定义比较器(透明比较C++14：is_transparent)、
│   │                             # lower_bound/upper_bound/equal_range、等价vs相等
│   │
│   ├── unordered-containers.md   # unordered_set/map(哈希表、分离链接法)、
│   │                             # 桶操作(rehash/reserve预分配桶)、负载因子(max_load_factor)
│   │                             # 与性能、自定义哈希函数(std::hash特化)、
│   │                             # 等价键(KeyEqual)、hash与equal的一致性要求
│   │
│   ├── container-adaptors.md     # stack(LIFO默认用deque)、queue(FIFO默认用deque)、
│   │                             # priority_queue(堆默认用vector)、底层容器要求、
│   │                             # flat_map/flat_set(C++23)的缓存友好优势与有序要求
│   │
│   ├── algorithms-overview.md    # 算法库总览：分类体系(非修改/修改/排序/数值/重排)、
│   │                             # 迭代器范畴与算法要求的映射、执行策略(C++17 seq/
│   │                             # par/par_unseq)、复杂度约定(O(n log n)等)
│   │
│   ├── sorting-algorithms.md     # sort(内省排序=快排+堆排+插入排序的混合)、
│   │                             # partial_sort(部分排序)、nth_element(第n个元素)、
│   │                             # stable_sort(归并排序保序)、is_sorted/sorted_until
│   │
│   ├── searching-algorithms.md   # binary_search(二分查找)、lower_bound/upper_bound/
│   │                             # equal_range(查找位置)、find/find_if/find_if_not、
│   │                             # search(Boyer-Moore算法)、mismatch
│   │
│   ├── numeric-algorithms.md     # iota(生成递增序列)/accumulate(累积)/inner_product、
│   │                             # reduce/transform_reduce(C++17并行友好)、
│   │                             # partial_sum(前缀和)、adjacent_difference(差分)、
│   │                             # gcd/lcm(C++17整数最大公约数/最小公倍数)
│   │
│   ├── mutating-algorithms.md    # copy/move/fill/generate、remove/unique与
│   │                             # erase-remove惯用法(remove不改变容器大小)、
│   │                             # rotate/shuffle/sample、partition/stable_partition
│   │
│   ├── heap-algorithms.md        # make_heap(建堆O(n))/push_heap(O(log n))/
│   │                             # pop_heap/sort_heap(堆排序)、
│   │                             # priority_queue的内部实现(make_heap+push_heap+pop_heap)、
│   │                             # 自定义比较器(max heap vs min heap)
│   │
│   ├── iterator-system.md        # 迭代器类别(输入/输出/正向/双向/随机访问/连续)、
│   │                             # iterator_traits(iterator_category/value_type/
│   │                             # difference_type)、迭代器适配器(back_inserter/
│   │                             # front_inserter/inserter/stream_iterator/reverse_iterator)
│   │
│   └── ranges.md                 # Ranges(C++20)：range概念(sized_range/borrowed_range/
│                                 # viewable_range)、view的惰性求值特性(不拥有数据)、
│                                 # 适配器(filter/transform/take/drop/join/split)、
│                                 # 管道操作符|、action(C++23)即时求值、自定义view
│
├── 06-standard-library-utilities/ # 标准库设施（14个文件）
│   ├── _index.md                 # 本部分概览：标准库的支撑性通用组件
│   │
│   ├── string.md                 # std::string内部实现(SSO机制：典型15/22字节阈值)、
│   │                             # 常用操作(查找find/rfind、替换replace、子串substr)、
│   │                             # 与C字符串互操作(c_str/data)、std::pmr::string(多态分配器)
│   │
│   ├── string-view.md            # std::string_view(C++17)：零拷贝视图(char*+长度)、
│   │                             # 作为函数参数的最佳实践(替代const string&)、
│   │                             # dangling view陷阱(绑定到临时string)、substr的O(1)效率
│   │
│   ├── iostream.md               # 流I/O体系：iostream(控制台)/fstream(文件)/
│   │                             # sstream(字符串流)、格式化标志(flags/precision/width)、
│   │                             # 与printf的效率对比、同步关闭(sync_with_stdio)
│   │
│   ├── formatting.md             # std::format(C++20)：占位符语法{}、格式说明符(:<宽度>/
│   │                             # :<精度>/:<填充>)、自定义格式化器(std::formatter特化)、
│   │                             # std::print(C++23直接输出)、编译期格式字符串检查
│   │
│   ├── pair-and-tuple.md         # std::pair(二元组)、std::tuple(可变元组)、std::tie(解包)、
│   │                             # make_tuple、tuple_cat(拼接)、tuple_element/tuple_size、
│   │                             # 结构化绑定与tuple的紧密关系
│   │
│   ├── optional.md               # std::optional(C++17)：有值或空、value()(抛异常) vs
│   │                             # operator*(未定义行为) vs value_or()(默认值)、
│   │                             # and_then/or_else(C++23单子操作链式调用)、
│   │                             # std::optional<std::reference_wrapper<T>>包装引用
│   │
│   ├── variant.md                # std::variant(C++17)：类型安全的联合体(知道当前存储类型)、
│   │                             # std::visit与访问者模式(overloaded惯用法)、
│   │                             # std::get/std::get_if(按类型或索引)、
│   │                             # valueless_by_exception异常状态、递归variant的惯用实现
│   │
│   ├── any-and-expected.md       # std::any(C++17)：任意类型容器(SBO小对象优化)、any_cast、
│   │                             # std::expected(C++23)：成功值或错误值的类型安全返回、
│   │                             # and_then/or_else/transform单子操作
│   │
│   ├── regex.md                  # std::regex：正则表达式语法(ECMAScript默认/awk/grep/egrep)、
│   │                             # 匹配(match/search/replace)、regex_iterator(遍历匹配)、
│   │                             # 编译时性能优化、与string_view的交互
│   │
│   ├── random.md                 # <random>：随机数引擎(mt19937/linear_congruential)、
│   │                             # 分布(均匀/正态/伯努利/泊松)、std::random_device(真随机)、
│   │                             # seed管理(固定seed可复现)、避免rand()的原因(周期/分布/线程安全)
│   │
│   ├── chrono.md                 # std::chrono：duration时间段(算术与转换cast)、
│   │                             # time_point时间点(与duration的关系)、
│   │                             # clock类型(system_clock/steady_clock/high_resolution_clock)、
│   │                             # 字面量(h/min/s/ms/us/ns C++14)
│   │
│   ├── calendar-and-timezone.md  # 日历类型(C++20)：year/month/day/weekday/month_day等、
│   │                             # 日期算术(months/years +/-)、
│   │                             # 时区(time_zone/zoned_time)、格式化输出(%F/%T/%z)
│   │
│   ├── filesystem.md             # std::filesystem(C++17)：path类操作(拼接/=、分解/
│   │                             # filename/extension、规范化canonical)、
│   │                             # 文件状态查询(exists/is_directory/file_size/permissions)、
│   │                             # 目录遍历(recursive_directory_iterator)
│   │
│   └── utility-and-other.md      # 其他实用工具：std::move/std::forward(原理见03-resource，
│                                 # 此处总结使用场景)、std::swap与ADL、std::exchange、
│                                 # std::as_const、std::invoke(C++17)、
│                                 # std::reference_wrapper与std::ref/cref、
│                                 # std::to_underlying(C++23)
│
└── 07-concurrency/               # 并发与内存模型（12个文件）
    ├── _index.md                 # 本部分概览：多核时代的C++并发编程完整链路
    │
    ├── thread-management.md      # std::thread构造与生命周期、join(等待) vs detach(分离)、
    │                             # 线程标识(get_id)、std::jthread(C++20)自动join与
    │                             # stop_token协作式取消(request_stop/stop_requested)
    │
    ├── mutex-and-lock.md         # std::mutex/timed_mutex/recursive_mutex(递归锁)、
    │                             # lock_guard(简单RAII锁)/unique_lock(可延迟/转移所有权)/
    │                             # scoped_lock(C++17可变参数免死锁)、
    │                             # std::lock同时锁定多个互斥锁(死锁避免算法)
    │
    ├── shared-mutex.md           # std::shared_mutex(C++17)：读写锁、
    │                             # shared_lock(读锁 多个线程可同时持有)与
    │                             # unique_lock(写锁 独占)、读写锁的性能权衡(
    │                             # 读多写少才有优势，否则普通mutex更优)
    │
    ├── condition-variable.md     # std::condition_variable：wait(原子释放锁+等待)/notify_one
    │                             # /notify_all、虚假唤醒(spurious wakeup)的成因与处理(
    │                             # 必须用while而非if)、生产者-消费者模式的多种实现
    │
    ├── semaphore-and-barrier.md  # std::counting_semaphore(C++20)：计数信号量(max计数)、
    │                             # binary_semaphore(二元信号量)、std::latch(一次性倒计数
    │                             # 等待所有任务完成)、std::barrier(可重用同步点C++20)
    │
    ├── atomics.md                # std::atomic模板与特化(atomic_int/bool/pointer)、
    │                             # 原子操作(load/store/exchange/compare_exchange_strong/
    │                             # weak与CAS循环)、std::atomic_ref(C++20非原子对象包装)、
    │                             # is_lock_free与平台相关性
    │
    ├── memory-order.md           # memory_order六种枚举(relaxed/consume/acquire/release/
    │                             # acq_rel/seq_cst)、happens-before与synchronizes-with关系、
    │                             # release-acquire同步模式(生产者-消费者配对)、
    │                             # seq_cst的全局一致序(所有线程看到同一顺序)、
    │                             # relaxed的适用场景(简单计数器)与陷阱
    │
    ├── fences.md                 # std::atomic_thread_fence(内存栅栏)、
    │                             # acquire/release/seq_cst fence与原子操作的对比(
    │                             # fence作用于线程的所有后续/前序操作而非单个变量)、
    │                             # 典型使用场景(非原子对象的同步)
    │
    ├── lock-free-intro.md        # 无锁编程的意义(避免锁的开销：阻塞/优先级反转)与代价
    │                             # (复杂度+ABA问题+内存回收)、CAS循环(compare-and-swap loop)
    │                             # 的基本模式、ABA问题与解决方案(带标记的指针tagged pointer)、
    │                             # 内存回收(hazard pointer/epoch/RP技术简介)
    │
    ├── async-and-future.md       # std::async启动策略(launch::async强制新线程 vs
    │                             # launch::deferred延迟执行)、std::future与
    │                             # std::shared_future(多等待者)、std::promise(设置值)、
    │                             # std::packaged_task(包装可调用对象)
    │
    └── coroutine-basics.md       # 协程(C++20)：co_await(等待)/co_yield(产出)/
                                  # co_return(返回)、promise_type(协程控制块 自定义行为)、
                                  # awaiter(await_ready/await_suspend/await_resume)、
                                  # std::generator(C++23简化生成器)、
                                  # 异步协程与io_uring结合的前瞻(零系统调用异步I/O)
```
