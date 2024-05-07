实现一个栈，要求 `Push` `Pop` `GetMin` 这些方法都是常数时间的。

仔细想下这个问题，最小值不会经常的变，除非我们插入了一个更小的值，或者是当前的最小值出栈了。

这个栈额外维护一个保存最小值的栈。当新元素插入的时候，如果新元素的值小于等于当前最小值，除了往存放所有数据的栈里面存一份，还要往最小值栈里面存一份。当弹出元素的时候，如果弹出元素和最小值一样的话，最小值栈也弹出。这种做法利用的额外空间比较小，虽然是 $O(n)$，但是，从概率上说，最小值栈只用保存一小部分数据就足够了。

``` csharp
public class StackWithMin : Stack<int>;
{
    private Stack<int>; minStack;

    public StackWithMin()
    {
        this.minStack = new Stack<int>();
    }

    public new void Push(int value)
    {
        base.Push(value);
        if (value <= this.GetMin())
        {
            minStack.Push(value);
        }
    }

    public new int Pop()
    {
        int value = base.Pop();
        if (value == this.GetMin())
        {
            minStack.Pop();
        }

        return value;
    }

    public int GetMin()
    {
        if (minStack.Count == 0)
        {
            return int.MaxValue;
        }
        else
        {
            return minStack.Peek();
        }
    }
}
```
