假定有一个 $8\times 8$ 的象棋棋盘，去掉对角上的两个小格，剩余 62 个格子。给你 31 个 $2\times 1$ 的纸牌，你能完全覆盖整个象棋盘吗？如果能，给出一个解决方案；如果不能，给予证明。

这个题目在[离散数学与组合数学](https://book.douban.com/subject/2115207/)这本书中有介绍。

首先抛出答案，不能。

为什么不能呢？

我们给棋盘染色，黑白相间，那么去掉的两个格子是同一个颜色的，不妨设去掉了两个白色的，也就是说，象棋盘上有 32 个黑色格子和 30 个白色格子。用 $2\times 1$ 的条形纸牌去覆盖，不管你怎么放，都会盖住一个黑色的，一个白色的，也就是说，不管你怎么放，都无法覆盖 32 个黑色格子和 30 个白色格子。所以答案是不能。