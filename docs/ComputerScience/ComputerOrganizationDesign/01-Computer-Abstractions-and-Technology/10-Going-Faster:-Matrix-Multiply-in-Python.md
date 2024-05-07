每一章的 "Going Faster" 小节以矩阵乘法为例，说明书中的各种想法是如何提升性能的。

```py
for i in xrange(n):
    for j in xrange(n):
        for k in xrange(n):
            C[i][j] += A[i][k] * B[k][j]
```
第二章，使用更贴近硬件的 C 语言重写，性能能够提升近 200 倍。这里作者有说明，这里为了对比 C 和距离硬件更远一点的 python 自身的性能。现实情况，python 编码使用高效的库，如果使用 numpy 重写，性能有极大的提升。

第三章，数据并行，性能提高 8 倍。

第四章，指令并行，差不多能提高 2 倍。

第五章，利用存储层次，提高 1.5 倍。

第六章，多线程并行，提升 12 - 17 倍。