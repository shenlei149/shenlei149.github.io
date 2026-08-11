# 值得一读

这里收集一些值得一读的文章，或者是值得花时间看的视频。

## [std::simd: How to Express Inherent Parallelism Efficiently Via Data-parallel Types](https://www.youtube.com/watch?v=LAJ_hywLtMA)
这是 [CppCon](https://cppcon.org/) 上的一个讲座，主要介绍了 `std::simd` 的思想。
* `std::simd` is for you - because you care about performance.
  * 如果工作中需要和性能打交道，那么 `std::simd`，特别是背后的思想，就值得关注。
* `std::simd` expresses data-parallelism - compiled to SIMD and ILP.
  * 数据并行，硬件层面使用了 SIMD 指令和指令级并行的能力。
* Focus on data-parallelism not SIMD instructions/registers!
  * 关注的焦点应该是数据并行这一思想，而不是 SIMD 指令或寄存器。如果思想不变，只是单纯使用 SIMD 指令，可能性能不会提高，甚至回退。
* `std::simd` enables separation of (1) serial execution; (2) synchronously parallel execution (SIMD and ILP); (3) asynchronously parallel execution (threads).
* `std::simd` guides you to design scalable and portable parallelization.
* `std::simd` enables high-level & specialized parallelism abstractions with full control where loop-vectorization fails.
  * 在循环向量化不起作用的情况下，`std::simd` 是个好选择。
* Consider type-vectorization not only loop-vectorization.
* `std::simd` provides an API & ABI for vectorization across translation units (including library boundaries).

## [Let's Branch? Unlikely...](https://lucisqr.substack.com/p/lets-branch-unlikely)
最核心的结论：对于现代 CPU 而言，通常都有动态分支预测能力；写不写 `likely` 往往意义不大，因为它通常不会对性能产生明显影响。

此外，文章中还介绍了一些 CPU、编译器、汇编指令等知识。总而言之，值得一读。

## [Should You Use AI for a Task? Here’s a Simple Way to Decide](https://www.schneier.com/blog/archives/2026/07/should-you-use-ai-for-a-task-heres-a-simple-way-to-decide.html)
我们什么时候应该使用 AI，什么时候不应该使用呢？这篇文章给出了一个简单的判断方法：这是工作，还是健身？工作中的事情要利用 AI 提升效率；健身相关的事情则要自己完成。比如，搬一堆东西上楼是工作，应该使用电梯或其他工具来提高效率；去健身房锻炼身体则是自己的事情，应该亲自去做。再比如，学习算法、掌握算法以及思考问题本身，都属于健身类型的事情，应该自己去做；但在工作中用算法解决问题时，则应该使用 AI 来提高效率。
