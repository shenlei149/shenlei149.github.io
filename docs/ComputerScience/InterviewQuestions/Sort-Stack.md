按照升序对栈进行排序，也就是说，最大元素在栈顶。最多只能使用一个额外的栈来存放临时数据，但是不能使用额外的数据结构，比如数组来保存临时数据。严格要求只能使用 $O(1)$ 的额外临时空间外加一个 $O(n)$ 的栈。

假设我们待排序的栈是 `s1`，额外有一个栈 `s2`，`s1`是无序的，`s2`是有序的。

考察 `s1` 栈顶的元素，`pop` 出来放到临时变量 `tmp` 里面，`peek` `s2` 的栈顶元素，和 `tmp` 进行比较，如果比 `tmp` 大，那么就 `pop` 出来并且 `push` 到 `s1` 里面，循环，直到 `s2` 的栈顶比 `tmp` 小，然后把 `tmp` `push` 到 `s2` 里面，至此，`s1` 里面的 `tmp` 按照顺序进入到了 `s2` 中。外层再来一层循环，对 `s1` 的每个元素进行同样的处理。

``` csharp
static Stack<int> Sort(Stack<int> s)
{
    var result = new Stack<int>();

    while (s.Count != 0)
    {
        var tmp = s.Pop();
        while (result.Count != 0 && result.Peek() > tmp)
        {
            s.Push(result.Pop());
        }

        result.Push(tmp);
    }

    return result;
}
```

这个算法的时间复杂度是 $O(n^2)$，空间复杂度是 $O(n)$，用了一个额外的栈，再有就只有了一个临时变量存放数据。
