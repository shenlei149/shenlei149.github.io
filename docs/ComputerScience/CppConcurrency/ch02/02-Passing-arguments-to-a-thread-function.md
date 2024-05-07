上一节最后展示了如何如何传递参数到线程中的函数。不过要牢记，`std::thread`构造函数先把传递的参数复制一次，然后以右值的形式传递给入口函数，仿佛是一个临时变量。即使函数需要的引用，这也会发生。如下代码
```cpp
void f(int i, std::string const &s);
std::thread t(f, 3, "hello");
```
创建了新的线程并关联到`t`变量，然后调用`f(3, "hello")`。传给`std::thread`构造函数的第三个参数是字符串字面量，`const char*`类型，在线程对象内被转换成`std::string`。对栈上的局部变量而言，需要注意。
```cpp
void f(int i, std::string const &s);
void oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(f, 3, buffer);
    t.detach();
}
```
指向`buffer`的指针传递给构造函数，但是很可能还没有来得及转成`std::string`的时候，`oops`就退出了，这会导致未定义的行为。解决方案是传递给构造函数之前，显式的转成`std::string`。
```cpp
void f(int i, std::string const &s);
void not_oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(f, 3, std::string(buffer));
    t.detach();
}
```
这里还有一个问题，构造函数复制了`std::string`对象，依赖隐式转化得到函数本身想要的类型，而不是直接转化成函数期望的类型。

如果对象已经复制了一份，但是函数期待非`const`引用，那么会编译错误。比如新的线程期望接受引用以修改对象本身
```cpp
void update_data_for_widget(widget_id w, widget_data &data);
void oops_again(widget_id w)
{
    widget_data data;
    std::thread t(update_data_for_widget, w, data);
    display_status();
    t.join();
    process_widget_data(data);
}
```
尽管`update_data_for_widget`第二个参数需要一个引用类型，但是`std::thread`的构造函数不知道这一点，复制了一份，然后以右值传递给`update_data_for_widget`，编译器会报错。因为不能把右值作为类型是非`const`引用的参数。解决方法和`std::bind`类似，使用`std::ref`传递引用。
```cpp
std::thread t(update_data_for_widget, w, std::ref(data));
```
那么`update_data_for_widget`接受到的就是引用而不是`data`的临时拷贝。

和`std::bind`类似，可以传递成员函数并且提供一个对象的指针。
```cpp
class X
{
public:
    void do_lengthy_work();
};
X my_x;
std::thread t(&X::do_lengthy_work, &my_x);
```
线程中就会调用`my_x.do_lengthy_work()`。如果成员函数需要参数，那么往后继续传递即可。其实这本质上就是调用某个对象上的成员函数的实现。

还有一个场景是对象只能移动不能复制，比如`std::unique_ptr`。如果源对象是临时变量，那么移动是自动的。如果是具名对象，需要显式使用`std::move`处理。下面展示了使用`std::move`向线程转移所有权
```cpp
void process_big_object(std::unique_ptr<big_object>);
std::unique_ptr<big_object> p(new big_object);
p->prepare_data(42);
std::thread t(process_big_object, std::move(p));
```
首先通过`std::move(p)`把`big_object`的所有权转移给线程，然后再转移给函数`process_big_object`。

`std::thread`自身和`std::unique_ptr`一样有所有权语义，拥有的不是动态对象，而是负责管理线程的执行。这个所有权可以在不同实例之间转移，同时保证任意时刻只有一个对象与线程关联。
