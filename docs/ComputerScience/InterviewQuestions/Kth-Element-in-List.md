如果是正向数第 $k$ 个节点，很容易，依次往后遍历就好，现在是倒数第 $k$ 个，该怎么办呢？

我们使用两个指针 `p1` 和 `p2`，`p1` 指向开始位置，`p2` 指向第 $k$ 个节点，使得两者之间距离是 $k$。然后同步地移动这两个指针，当 `p2` 指向最后一个节点的时候，`p1` 指向的就是倒数第 $k$ 个节点。

算法的时间复杂度是 $O(n)$，空间复杂度是 $O(1)$。
``` csharp
private static LinkedListNode<T> KthToLast<T>(LinkedListNode<T> head, int k)
{
    if (k <= 0)
    {
        return null;
    }

    LinkedListNode<T> p1 = head;
    LinkedListNode<T> p2 = head;

    for (int i = 0; i < k - 1; i++)
    {
        if (p2 == null)
        {
            return null;    // Link length is less than k
        }

        p2 = p2.Next;
    }

    if (p2 == null)
    {
        return null;    // Link length is less than k
    }

    while (p2.Next != null)
    {
        p1 = p1.Next;
        p2 = p2.Next;
    }

    return p1;
}
```
