给定一个 $N \times N$ 的矩阵，要求就地，空间复杂度为 $O(1)$，旋转九十度。

旋转矩阵，由外向内看，元素所在的层数是不变的，所以我们一层一层的迭代对每层进行旋转。对于每层来说，上面的元素到了右边，右边的元素到了下面，以此类推。

可以把上面的元素全部拷贝到某个数组，然后左边的元素拷贝过来，下面的元素拷贝到左边，以此类推，但是这样就占用了$O(N)$ 的空间。与逐层移动思想类似，一次只移动一个元素即可，对应四边的四个元素交换位置，这样不会影响到其他元素，也同样达到了目的。

下面是算法实现：
``` csharp
public static void Rotate(int[,] matrix)
{
    int n = matrix.GetLength(0);
    for (int layer = 0; layer < n / 2; layer++)
    {
        int first = layer;
        int last = n - 1 - layer;
        for (int i = first; i < last; i++)
        {
            int offset = i - first;

            // save top
            int top = matrix[first, i];

            // left -> top
            matrix[first, i] = matrix[last - offset, first];

            // bottom -> left
            matrix[last - offset, first] = matrix[last, last - offset];

            // right -> bottom
            matrix[last, last - offset] = matrix[i, last];

            // top -> right
            matrix[i, last] = top;
        }
    }
}
```

算法的时间复杂度是 $O(N^2)$，应该说已经是最优的了，因为旋转矩阵，至少要把 $N^2$ 个元素都移动一次。
