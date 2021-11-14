如何从单链表里面删除内容一样的节点呢？

为了能够删除内容重复的节点，首先我们要能追踪到那些重复的内容。使用 `Set` 这种数据结构就很不错。

我们的解法很简单，就是去遍历整个链表，如果集合中没有该节点内容，就放入集合，如果已经存在，说明是重复内容，删除该节点。

算法时间复杂度是 $O(n)$，还需要额外的存储空间 $O(n)$。
``` csharp
private static void DeleteDups<T>(LinkedListNode<T> n)
{
    HashSet<T> datas = new HashSet<T>();
    LinkedListNode<T> previous = null;
    while (n != null)
    {
        if (datas.Contains(n.Data))
        {
            previous.Next = n.Next;
        }
        else
        {
            datas.Add(n.Data);
            previous = n;
        }

        n = n.Next;
    }
}
```

现在，增加一个限制条件，不能使用额外的存储空间。又该如何做呢？

我们使用两个指针去遍历这个链表，`current` 节点去遍历，再有一个节点 `runner` 从 `current` 之后的一个去遍历，如果两者内容一样，就删除 `runner` 指向的节点。

该算法时间复杂度是 $O(n^2)$，好处是不用占用额外的存储空间。  
``` csharp
private static void DeleteDups<T>(LinkedListNode<T> n)
{
    LinkedListNode<T> current = n;
    while (current != null)
    {
        LinkedListNode<T> runner = current;
        while (runner.Next != null)
        {
            if (runner.Next.Data.Equals(current.Data))
            {
                runner.Next = runner.Next.Next;
            }
            else
            {
                runner = runner.Next;
            }
        }

        current = current.Next;
    }
}
```
