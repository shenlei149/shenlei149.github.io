给一个单链表和一个键值 `k`，分割单链表，使得链表的前面都是小于 `k` 的值，链表的后面都是大于或等于 `k` 的值。

我们创建两个链表，一个用于存放小于 `k` 的结点，一个用于存放大于 `k` 的结点。遍历原来的链表，把结点分到两个新的链表上，然后把这两个链表链接起来即可。
``` csharp
private static LinkedListNode<int> Partition(LinkedListNode<int> node, int k)
{
    LinkedListNode<int> beforeStart = null;
    LinkedListNode<int> beforeEnd = null;
    LinkedListNode<int> afterStart = null;
    LinkedListNode<int> afterEnd = null;

    while (node != null)
    {
        LinkedListNode<int> next = node.Next;
        node.Next = null;
        if (node.Data < k)
        {
            if (beforeStart == null)
            {
                beforeStart = node;
                beforeEnd = beforeStart;
            }
            else
            {
                beforeEnd.Next = node;
                beforeEnd = node;
            }
        }
        else
        {
            if (afterStart == null)
            {
                afterStart = node;
                afterEnd = afterStart;
            }
            else
            {
                afterEnd.Next = node;
                afterEnd = node;
            }
        }

        node = next;
    }

    if (beforeStart == null)
    {
        return afterStart;
    }

    beforeEnd.Next = afterStart;
    return beforeStart;
}
```
