线程标识符的类型是`std::thread::id`，有两种方式获取，调用`std::thread`对象的`get_id()`方法，或者获取当前线程的标识符`std::this_thread::get_id()`。如果`std::thread`对象没有关联线程，会返回默认构造的`std::thread::id`对象。

`std::thread::id`对象能够任意复制和比较。如果两个对象相同，那么是同一个线程或者都表示没有线程的默认值；否则，说明两个线程不是同一个线程，或者其中一个表示一个线程另一个是默认值。

`std::thread::id`支持完成的比较，等于不等于，和全序比较。允许作为关联容器的 Key，排序，或者和其他类型比较。不等值的全序蕴含$a<b,b<c$，那么$a<c$。STL 还提供了`std::hash<std::thread::id>`，使得其可以作为无序关联容器的 Key。

`std::thread::id`经常用于检查是否需要执行某些操作。比如算法切分成多个部分，而主线程需要做一些和其他线程不一样的事情。在运行其他线程之前先保存`std::this_thread::get_id()`，然后算法自身检查线程ID是否和存储的一致，选择不同的执行路径。
```cpp
std::thread::id master_thread;
void some_core_part_of_algorithm()
{
    if (std::this_thread::get_id() == master_thread)
    {
        do_master_thread_work();
    }

    do_common_work();
}
```
此外，可以把`std::thread::id`放到某个数据结构中作为操作的一部分。后续多个线程操作同一个的数据结构，可以检查是否有权限或者有必要执行操作。

考虑如下情况，线程需要特定的数据，但是放到线程局部空间存储又不合适，那么使用`std::thread::id`作为 key 的关联容器就很有用了。能够控制线程访问数据和在线程间传递数据。

通常`std::thread::id`作为标识符足够通用了。除非用于包含语义的场景，比如数组的下标。可以`std::thread::id`传给输出流，比如`std::cout`
```cpp
std::cout << std::this_thread::get_id();
```
输出内容依赖于实现，标准只能保证相同的ID输出是相同的，不用的ID输出时不同的。这主要用于 debug 和 log，没有语义。
