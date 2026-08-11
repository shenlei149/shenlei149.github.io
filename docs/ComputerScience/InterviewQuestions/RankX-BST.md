问题：你正在读一个整数数据流。在读的过程中，你可能需要要知道某个值 `x` 在已读数据的排位，实现这个功能。也就是说，你需要实现一个方法 `track(int x)`，每读入一个整数，就会调用一次该方法。实现方法 `getRankOfNumber(int x)`，返回小于等于 `x` 的值的数量。

简单的方式，维护一个有序的数组。`getRankOfNumber` 方法很高效，二分查找就好了。但是 `track` 方法很慢，插入到合适的地方，可能要移动很多元素。

换种数据结构，二分查找树。`track` 复杂度是 
$log(n)$，平均情况，而且我们可以假设数据流是随机的，树大致平衡。`getRankOfNumber` 方法的效率就不高了，需要中序遍历，得到 `x` 的排序。

想一想，如果 `x` 比某个节点的值还小的话，其实就不用去遍历左边的子树了，因为左边的都比 `x` 小。所以，我们往节点里面添加一个值，记录该节点左边有多少个节点。这样就不用遍历左边的子树，直接加上这个值就可以了。

下面是完整的代码：
``` csharp
private static RankNode root = null;

public static void Track(int number)
{
    if (root == null)
    {
        root = new RankNode(number);
    }
    else
    {
        root.Insert(number);
    }
}

public static int GetRankOfNumber(int number)
{
    return root.GetRank(number);
}

class RankNode
{
    public int LeftSize { get; set; }
    public RankNode Left { get; set; }
    public RankNode Right { get; set; }
    public int Data { get; set; }

    public RankNode(int d)
    {
        this.Data = d;
    }


    public int GetRank(int d)
    {
        if (d == this.Data)
        {
            return this.LeftSize;
        }
        else if (d < this.Data)
        {
            if (this.Left == null)
            {
                return -1;
            }
            else
            {
                return this.Left.GetRank(d);
            }
        }
        else
        {
            int rightRank = this.Right == null ? -1 : this.Right.GetRank(d);
            if (rightRank == -1)
            {
                return -1;
            }
            else
            {
                return this.LeftSize + 1 + rightRank;
            }
        }
    }

    public void Insert(int d)
    {
        if (d <= this.Data)
        {
            if (this.Left == null)
            {
                this.Left = new RankNode(d);
            }
            else
            {
                this.Left.Insert(d);
            }

            this.LeftSize++;
        }
        else
        {
            if (this.Right == null)
            {
                this.Right = new RankNode(d);
            }
            else
            {
                this.Right.Insert(d);
            }
        }
    }
}
```
