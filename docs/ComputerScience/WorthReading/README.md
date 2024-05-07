这里收集一些值得一读的文章，或者是值得花时间看的视频。

## [std::simd: How to Express Inherent Parallelism Efficiently Via Data-parallel Types](https://www.youtube.com/watch?v=LAJ_hywLtMA)
这是一个 [CppCon](https://cppcon.org/) 上的一个讲座。主要介绍了 `std::simd` 的思想。
* `std::simd` is for you - because you care about performance.
  * 如果工作中需要和性能打交道，那么 `std::simd`，特别是背后的思想，就值得关注。
* `std::smid` expresses data-parallelism - compiled to SIMD and ILP.
  * 数据并行，硬件层面使用了 SIMD 指令和指令级并行的能力。
* Focus on data-parallelism not SIMD instructions/registers!
  * 关注的焦点应该是数据并行这一思想，而不是 SIMD 指令或寄存器。如果思想不变，只是单纯使用 SIMD 指令，可能性能不会提高，甚至回退。
* `std::simd` enables separation of (1) serial execution; (2) synchronously parallel execution (SIMD and ILP); (3) asynchronously parallel execution (threads).
* `std::simd` guides you to design scalable and portable parallelization.
* `std::simd` enables high-level & specialized parallelism abstractions with full control where loop-vectorization fails.
  * 在循环向量化不起作用的情况下，`std::simd` 是个好选择。
* Consider type-vectorization not only loop-vectorization.
* `std::simd` provides and API & ABI for vectorization across translation units (including library boundaries).

## [Let's Branch? Unlikely...](https://lucisqr.substack.com/p/lets-branch-unlikely)
最核心的结论：对于现代 CPU 而言，都有动态分支预测的能力，你写 likely 或者不写 likely，没有意义，也不重要，因为对性能没有任何影响。

此外，这个文章中还介绍了一些 CPU、编译器、汇编指令等知识。言而总之，值得一读。
