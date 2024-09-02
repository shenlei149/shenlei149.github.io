流水线使得指令并行成为可能，自然称之为指令级并行（`instruction-level parallelism`, `ILP`）。有两个方法增加指令级并行的指令数量。第一个个增加流水线的长度使得更多指令能够重叠执行，那么并行数量也就越高。时钟周期能够更短，性能也就能够得到进一步的提升。

第二个方法是复制一份计算机内部各个组件，在每个阶段都能有更多的指令在执行。这种技术成为多发射（`multiple issue`），或者超标量（`superscalar`）。

每个阶段执行多个指令使得指令执行的速率可以超过时钟速率，或者说 CPI 可以小于 1。有时，也使用其倒数每周期指令（`instructions per clock cycle`, `IPC`）表示。一个 3GHz 4 路多发射的 CPU 最高峰值可以每秒执行 12 B（120 亿）条指令，CPI 0.25，IPC 是 4。假定流水线是五个阶段，那么每个时钟周期最多能完成 20 条指令。当前，高端 CPU 试图每个时钟周期发生 3 到 6 条指令。最适度的涉及也会尝试使得 IPC 为 2。当然，也有一些限制，比如同时能够执行的指令类型，比如依赖产生时会发生什么。

实现多发射有两种方法，主要区别在于编译器与硬件之间的分工。分工决定了决策是静态的（在编译时）还是动态的（在执行时），因此这两种方法也被称为静态多发射（`static multiple issue`）和动态多发射（`dynamic multiple issue`）。稍后会看到，这两种方法都有更常用的名字，不过更不精确或者有些许限制。

在多发射流水线中，有两个主要且独特的职责：

1. 将指令打包到发射槽中：在一个时钟周期内，处理器需要决定有多少指令、哪些指令能被发射。在大多数静态发射处理器中，至少部分工作由编译器处理；在动态发射处理器中，由处理器运行时决定，尽管编译器已经尝试重排指令顺序以提高发射率。
2. 处理数据冒险和控制冒险：在静态发射处理器中，编译器处理部分或者全部数据冒险和控制冒险问题；相对而言，动态发射处理器在运行时通过硬件技术尝试缓解某些冒险问题。

虽然这里描述为不同的方法，但是实际上一个技术往往会借鉴另一个方法的思想。

### The Concept of Speculation

### Static Multiple Issue

#### An Example: Static Multiple Issue with the RISC-V ISA

#### Simple Multiple-Issue Code Scheduling

#### Loop Unrolling for Multiple-Issue Pipelines

### Dynamic Multiple-Issue Processors

#### Dynamic Pipeline Scheduling

### Energy Efficiency and Advanced Pipelining

