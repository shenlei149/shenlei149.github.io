栈是先进后出，队列先进先出，问题在于如何在队列中找到最后一个进队列的元素。好在有两个队列。从一个队列往另外一个队列里面转移数据，转移到剩余一个的时候，就弹出该元素。压入栈比较简单，向正在使用的队列里面插入一个元素就可以了。

入栈的时间复杂度和普通的栈一致 $O(1)$，但是出栈就是要把装有数据的队列操作一遍，复杂度是 $O(n)$，其中 $n$ 是队列中元素的个数。

下面是 C# 实现代码：
``` csharp
public class StackWith2Queues<T>
{
	private Queue<T> curQueue;
	private Queue<T> otherQueue;

	public StackWith2Queues()
	{
		curQueue = new Queue<T>();
		otherQueue = new Queue<T>();
	}

	public int Count { get; private set; }

	public bool Empty { get { return Count == 0; } }

	public void Push(T item)
	{
		curQueue.Enqueue(item);
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
			while (curQueue.Count > 1)
			{
				otherQueue.Enqueue(curQueue.Dequeue());
			}

			T result = curQueue.Dequeue();
			otherQueue.Enqueue(result);
			SwapQueue();
			return result;
		}
	}

	public T Pop()
	{
		if (Empty)
		{
			throw new InvalidOperationException();
		}
		else
		{
			Count--;
			while (curQueue.Count > 1)
			{
				otherQueue.Enqueue(curQueue.Dequeue());
			}

			T result = curQueue.Dequeue();
			SwapQueue();
			return result;
		}
	}

	private void SwapQueue()
	{
		var tmp = curQueue;
		curQueue = otherQueue;
		otherQueue = tmp;
	}
}
```
