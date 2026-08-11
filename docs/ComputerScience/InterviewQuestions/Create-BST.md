给一个有序的数组，升序，没有重复的数字，生成一个高度最小的二叉搜索树。

二叉搜索树具有的特点就是左边节点的值比当前节点的小，右边节点的值比当前节点的大。而一个有序数组，恰恰是二叉搜索树的中序遍历结果。

扯了点没用的，想要高度低，就要尽可能的平衡，左右两个子树尽可能的平衡，那么，数组中间的数就是根节点了。左边的树就是数组中间左边的半个数组组成的，右边的树就是数组右边的半个数组组成的。这一段描述，是一个递归的过程，按照这个思路，代码也就有了。

``` csharp
class TreeNode<T>
{
	public TreeNode<T> Left { get; set; }
	public TreeNode<T> Right { get; set; }
	public T Data { get; set; }
}

static TreeNode<T> CreateMinimalBst<T>(T[] arr, int begin, int end)
{
	if (begin >= end)
	{
		return null;
	}

	int mid = (end - begin) / 2 + begin;
	var node = new TreeNode<T>();
	node.Data = arr[mid];
	node.Left = CreateMinimalBst(arr, begin, mid);
	node.Right = CreateMinimalBst(arr, mid + 1, end);

	return node;
}

static TreeNode<T> CreateMinimalBst<T>(T[] arr)
{
	return CreateMinimalBst(arr, 0, arr.Length);
}
```

O(n)的时间复杂度就搞定了这个题目，空间复杂度呢？表面看来，没有使用额外的存储空间，但是递归导致栈空间的增长，O(lg(n))。
