有两个链表，代表两个数，其中每个结点都包含一个数字。每个链表是倒序排列，比如数字 617，那么链表是 7 -> 1 -> 6。如何求和呢？如果是正序排列，又该如何求和呢？

如果反序排列的话，链表头就是个位，接着是十位，以此类推。就和普通的手算一致，先算低位，如果大于 10，需要进位，接着计算高位。
``` csharp
private static LinkedListNode<int> AddLists(LinkedListNode<int> l1, LinkedListNode<int> l2, int carry)
{
    if (l1 == null && l2 == null && carry == 0)
    {
        return null;
    }

    int value = (l1 != null ? l1.Data : 0) + (l2 != null ? l2.Data : 0) + carry;
    var result = new LinkedListNode<int>(value % 10);
    if (l1 != null || l2 != null)
    {
        var more = AddLists(l1 != null ? l1.Next : null, l2 != null ? l2.Next : null, value >= 10 ? 1 : 0);
        result.Next = more;
    }

    return result;
}
```

如果链表是正序表示一个数字，该如何做呢？一个可行的方案是，反转两个链表，然后就回到了上一种情况。

这里，采用和上面类似的思想去解决这个问题，显然，比上面的问题要复杂，需要考虑的更多：
1. 如果一个链表比另外一个长，该怎么办？比如 1 -> 2 -> 3 -> 4 和 5 -> 6 -> 7 两个链表，显然，5 应该和 2 相加而不是 1。我们可以遍历两个链表，向短的前面补充上零。
2. 在上面那种情况下，计算结果被加到了链表的尾部，这意味着递归程序本身就可以处理进位，但是现在不行了。所以我们需要添加一些数据结构对象来处理进位。把后面的进位结果带到前面来。

``` csharp
private static LinkedListNode<int> AddLists(LinkedListNode<int> l1, LinkedListNode<int> l2)
{
    int length1 = GetLength(l1);
    int length2 = GetLength(l2);

    if (length1 < length2)
    {
        l1 = PadList(l1, length2 - length1);
    }
    else
    {
        l2 = PadList(l2, length1 - length2);
    }

    PartialSum sum = AddListsHelper(l1, l2);

    return sum.Carry == 0 ? sum.Sum : InsertBefort(sum.Sum, 1);
}

private static PartialSum AddListsHelper(LinkedListNode<int> l1, LinkedListNode<int> l2)
{
    if (l1 == null && l2 == null)
    {
        return new PartialSum();
    }

    PartialSum sum = AddListsHelper(l1.Next, l2.Next);
    int value = sum.Carry + l1.Data + l2.Data;
    var fullResult = InsertBefort(sum.Sum, value % 10);
    sum.Sum = fullResult;
    sum.Carry = value / 10;

    return sum;
}

private static LinkedListNode<int> InsertBefort(LinkedListNode<int> l, int p)
{
    var node = new LinkedListNode<int>(p);
    if (l != null)
    {
        node.Next = l;
    }

    return node;
}

private static LinkedListNode<int> PadList(LinkedListNode<int> l, int p)
{
    var head = l;
    for (int i = 0; i < p; i++)
    {
        LinkedListNode<int> newNode = new LinkedListNode<int>(0);
        newNode.Next = head;
        head = newNode;
    }

    return head;
}

private static int GetLength(LinkedListNode<int> l1)
{
    int count = 0;
    while (l1 != null)
    {
        count++;
        l1 = l1.Next;
    }

    return count;
}

private class PartialSum
{
    public LinkedListNode<int> Sum = null;
    public int Carry = 0;
}
```
