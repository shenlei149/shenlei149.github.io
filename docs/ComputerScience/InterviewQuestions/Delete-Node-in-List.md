只给了一个参数，就是要删除的结点。由于是单链表，可以访问下一个结点，但是无法访问上一个结点。解决的方法是把下一个结点的内容复制到当前的结点，然后删除下一个结点。
``` csharp
private static bool DeleteNode<T>(LinkedListNode<T> n)
{
    if (n == null || n.Next == null)
    {
        return false;
    }

    LinkedListNode<T> next = n.Next;
    n.Data = next.Data;
    n.Next = next.Next;

    return true;
}
```

这道题目看似简单，但是有一个坑，就是如果给定结点是链表的最后一个结点该怎么办。上述代码排除这种情况，返回了 `false`，意味着删除失败。面试时应该指出存在这个问题，并和面试官讨论如何解决这种特殊情况。
