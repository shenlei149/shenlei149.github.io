最早 MMX（`MultiMedia eXtension`）是为了处理短整数类型向量的。SSE（`Streaming SIMD Extension`）可以处理单精度浮点数的短向量。

SSE2 引入了 128 比特寄存器，`xmm`，可以放 4 个单精度浮点数或 2 个双精度浮点数。如果数据在内存中按照 128 比特对其，那么一个指令就能加载或者存储 128 比特数据。这种打包的浮点数格式支持算数运算，即同时计算四个单精度（PS）或两个双精度（PD）浮点数。

2011 年，Intel 引入了更宽的寄存器 `ymm` 和 AVX（`Advanced Vector Extensions`）指令，可以同时计算 8 个 32 比特单精度浮点数和 4 个 64 比特双精度浮点数。SSE 和 SSE2 使用 `ymm` 的低 128 比特。对于 128 比特和 256 比特操作，汇编指令前面会有前缀 `v`，表示 `vector`。

2015 年，Intel 再次将寄存器宽度翻倍，512 比特，称为 `zimm`，AVX512 指令。