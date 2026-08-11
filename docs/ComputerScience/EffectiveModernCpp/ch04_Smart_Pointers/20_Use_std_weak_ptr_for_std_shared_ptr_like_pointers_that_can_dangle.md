如果有一种智能指针，类似 `std::shared_ptr`，但是不参与资源的共享所有权，即不改变引用计数。这种指针将会面临一个对 `std::shared_ptr` 而言不存在的问题：指向的对象已经被析构了。这就是 `std::weak_ptr`。

`std::weak_ptr` 不能解引用，也不能判空。这是因为 `std::weak_ptr` 不是独立的智能指针，而是 `std::shared_ptr` 的增强。

这种关系从一开始就存在了。通常使用 `std::shared_ptr`  创建 `std::weak_ptr`，指向同一个对象，但是不改变引用计数。
```cpp
// after spw is constructed, the pointed-to Widget's ref count (RC) is 1.
auto spw = std::make_shared<Widget>();

// wpw points to same Widget as spw. RC remains 1
std::weak_ptr<Widget> wpw(spw);

// RC goes to 0, and the Widget is destroyed. wpw now dangles
spw = nullptr;
```

`std::weak_ptr` 通过 `expired()` 接口暴露是否是悬垂指针。
```cpp
if (wpw.expired()) // if wpw doesn't point to an object...
```
通常，你想通过检查是否过期，如果没过期，就访问指向的对象。但是，没有解引用。即使有解引用，将检查和访问分开也会导致数据竞争。假定一个线程检查是否过期，没有过期，同时另一个线程析构了对象，前一个线程继续访问，解引用会出错。

所以这里需要一个原子操作来做这个事情。通过从 `std::weak_ptr` 构造 `std::shared_ptr` 完成这件事。有两种方式，略有不同，取决于其过期时的行为。第一种是用 `std::weak_ptr::lock` 得到 `std::shared_ptr`，如果过期了，返回值是 `nullptr`。
```cpp
std::shared_ptr<Widget> spw1 = wpw.lock(); // if wpw's expired, spw1 is null
auto spw2 = wpw.lock();                    // same as above, but uses auto
```
第二种方式是将 `std::weak_ptr` 作为参数构造一个 `std::shared_ptr`，如果过期了，会抛出异常。
```cpp
std::shared_ptr<Widget> spw3(wpw); // if wpw's expired, throw std::bad_weak_ptr
```

下面解释 `std::weak_ptr` 有什么用。假定有一个工厂方法根据唯一的 ID 构造一个只读独享。那么这个接口的 API 可以设计如下：
```cpp
std::unique_ptr<const Widget> loadWidget(WidgetID id);
```
如果 `loadWidget` 的开销很大，那么合理的做法是实现一个函数，和 `loadWidget` 功能类似，但是有缓存。如果缓存很大，会拖累缓存的性能，所以一个常见改进就是淘汰不用的缓存。

有缓存功能的工厂函数的返回值就不宜是 `std::unique_ptr` 了。调用者需要得到一个智能指针，确定其生命周期，缓存函数自己也需要保存一个指向对象的智能指针。缓存函数的调用者可能会析构对象，那么缓存函数的智能指针要能检测是否是悬垂指针，因此缓存函数使用 `std::weak_ptr` 指针，而返回类型是 `std::shared_ptr`。

下面是一个临时凑合能用的版本。
```cpp
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
    static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;

    auto objPtr = cache[id].lock(); // objPtr is std::shared_ptr to cached
                                    // object (or null if object's not in cache)

    if (!objPtr)
    {                            // if not in cache,
        objPtr = loadWidget(id); // load it
        cache[id] = objPtr;      // cache it
    }

    return objPtr;
}
```

`fastLoadWidget` 的实现会积累没有用的 `std::weak_ptr`，它们指向不被析构的对象。讨论方法不会增进对 `std::weak_ptr` 的了解，故省略。`std::weak_ptr` 第二个应用场景是观察者模式。有一个主体（状态可能发生变化的对象）和若干个观察者（接受状态变化的对象）。一般的实现是主体有一个指向各个观察者的指针。不过主体并不关心观察者的生命周期。一个合理的设计就是主体持有一个容器，里面是指向观察者的 `std::weak_ptr` 指针。

下面是最后一个使用 `std::weak_ptr` 的例子。考虑有三个结构体 `A, B, C`，`A, C` 共享 `B` 的所有权，所以有指向 `B` 的 `std::shared_ptr`。假定如果 `B` 有一个指针指回 `A`，这个指针是什么类型呢？

有三种选择：
1. 裸指针。如果 `A` 析构了，`C` 继续指向 `B`，那么 `B` 有一个指向 `A` 的悬垂指针，那么就可能解引用而导致未定义行为。
2. `std::shared_ptr`。那么 `A, B` 相互有一个 `std::shared_ptr` 指向对方，即使没有对象再能访问到这两个对象，比如 `C` 不再指向 `B` 或者析构了，但是相互指向的 `std::shared_ptr` 的引用计数还是 1，导致 `A, B` 无法析构，资源无法回收。
3. `std::weak_ptr`。能够避免上述两个问题，`A` 析构了，`B` 能检测出来而不是访问悬垂指针。尽管还是有相互指向的指针，但是 `B` 不会影响指向 `A` 的 `std::shared_ptr` 的引用计数，当其他 `std::shared_ptr` 都不指向 `A` 时，`A` 会销毁。

`std::weak_ptr` 是最好的选择。不过一般来说，使用 `std::weak_ptr` 来打破 `std::shared_ptr` 循环是不常见的做法。在严格分层的数据结构中，父节点拥有指向子节点的 `std::unique_ptr` 指针就足够了。如果需要反向指针，由于子节点的生命周期往往小于父节点，因此使用裸指针也不会有问题。

当然，不是所有基于指针的数据结构都是分层的，此时，使用 `std::weak_ptr` 是不错的选择。

从效能角度看，`std::weak_ptr` 和 `std::shared_ptr` 是一致的。大小是一样的，指向相同的控制块，构造、析构、拷贝涉及原子操作。不过前面说过 `std::weak_ptr` 不会涉及引用计数，这里指的是共享所有权的引用计数，但是会涉及次级引用计数。参见 Item 21。

## Things to Remember
* Use `std::weak_ptr` for `std::shared_ptr`-like pointers that can dangle.
* Potential use cases for `std::weak_ptr` include caching, observer lists, and the prevention of `std::shared_ptr` cycles.
