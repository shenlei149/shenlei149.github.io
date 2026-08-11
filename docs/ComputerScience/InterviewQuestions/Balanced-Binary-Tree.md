给定一个二叉树，判断其是否平衡。平衡的定义是，从任何一个节点去考虑，它的左子树和右子树的高度差不超过 1。

很明显，需要一个递归函数来做这个事情。

如果这个函数返回树高度，判断是否平衡，再去递归调用这个函数去判断左右子树是否平衡，这显然是一个解，但是略微暴力了一点。因为递归左右子树时，又要计算其左右子树的高度，而之前以前计算过了，很明显的有着重复的部分。

给定了根节点，想直接自底向上计算，也不是一件容易的事情。

我们稍稍改造一下返回高度的函数，如果其左右子树平衡，才返回高度，否则返回 -1，这样子，程序的递归性就自然而然的帮我们自底向上进行判断每个节点处是否平衡了。

下面的代码定义了一个树节点的数据结构，`CheckHeight` 函数就是上面说的函数了，最后，计算平衡就是把根节点传入，检查返回值是否等于 -1。
``` csharp
class TreeNode
{
    public TreeNode Left { get; set; }
    public TreeNode Right { get; set; }
}

static int CheckHeight(TreeNode node)
{
    if (node == null)
    {
        return 0; // Empty
    }

    int leftHeight = CheckHeight(node.Left);
    if (leftHeight == -1)
    {
        return -1; // Not balanced
    }

    int rightHeight = CheckHeight(node.Right);
    if (rightHeight == -1)
    {
        return -1; // Not balanced
    }

    /* Check if current node is balanced. */
    int diff = Math.Abs(leftHeight - rightHeight);
    if (diff > 1)
    {
        return -1; // Not balanced
    }
    else
    {
        return Math.Max(leftHeight, rightHeight) + 1; // Return height
    }
}

bool IsBalance(TreeNode root)
{
    return CheckHeight(root) != -1;
}
```
算法的时间复杂度是 $O(n)$，只需要遍历每个节点一次。空间复杂度是 $O(h)$，$h$ 是树的高度，就是在递归过程中，最深就是 $h$ 层罢了，每层需要常量空间去保存节点相关的信息。
