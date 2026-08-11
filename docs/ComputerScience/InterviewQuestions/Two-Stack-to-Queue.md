---
title: 使用两个栈实现队列
date: 2015-01-29 11:33:24
tags: [面试题, 队列, 栈]
categories:
 - 计算机
 - 算法
---
栈是先进后出，队列是先进先出。考虑把栈 A 的数据弹出再压入栈 B 中，顺序就完全反过来了，如果再把 B 中的元素弹出，就按照进栈 A 的顺序了。此题解题思路大致如此。进队列的时间复杂度就是压入栈 A 的时间复杂度 $O(1)$，出栈的话，如果栈 B 有数据，也是 $O(1)$，但是没有数据，对于当次操作而言是 $O(n)$，其中 $n$ 为栈 A 中的元素个数，但是平摊分析一下，每个元素进栈 A，倒到栈 B，然后出栈，所以平均时间复杂度是 $O(1)$。

下面是C#代码：
``` csharp
public class QueueWith2Stacks<T>
{
	private Stack<T> enqueueStack;
	private Stack<T> dequeueStack;

	public QueueWith2Stacks()
	{
		enqueueStack = new Stack<T>();
		dequeueStack = new Stack<T>();
	}

	public int Count { get; private set; }

	public bool Empty { get { return Count == 0; } }

	public void Enqueue(T item)
	{
		enqueueStack.Push(item);
		Count++;
	}

	public T Peek()
	{
		if (Empty)
		{
			throw new InvalidOperationException();
		}
		else
		{
			if (dequeueStack.Count == 0)
			{
				EnqueueToDequeue();
			}

			return dequeueStack.Peek();
		}
	}

	public T Dequeue()
	{
		if (Empty)
		{
			throw new InvalidOperationException();
		}
		else
		{
			Count--;
			if (dequeueStack.Count==0)
			{
				EnqueueToDequeue();
			}

			return dequeueStack.Pop();
		}
	}

	private void EnqueueToDequeue()
	{
		while (enqueueStack.Count>0)
		{
			dequeueStack.Push(enqueueStack.Pop());
		}
	}
}
```
