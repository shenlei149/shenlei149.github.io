这一节会综述影响计算机发展最伟大的七个思想，本书其他部分的阐述也就是对这些核心思想的展开。

### Use Abstraction to Simplify Design
软硬件的设计都是通过抽象来设计不同层次的表示，下层屏蔽这一层的细节并向上层提供一个简单的模型。

### Make the Common Case Fast
优化最常用的情况收益很大。通常，最常用的情况往往最简单，更容易优化。

### Performance via Parallelism

### Performance via Pipelining
并行的一种模式。

### Performance via Prediction
大多数情况都能预测对的话，那么就可以提前执行，提高性能。预测失败的惩罚不太大也是必要前提之一。

### Hierarchy of Memories
我们希望存储的速度越快越好、容量越大越好、价格越便宜越好。层次架构就是为了达到这个目的设计的。缓存（`cache`）在其中扮演着重要角色。

### Dependability via Redundancy
增加一些冗余，使得可以检测失败。
