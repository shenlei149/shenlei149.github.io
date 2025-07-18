数据库通常用于不同的目的：有一些放临时的热数据、一些服务于长期的冷数据，一些处理复杂的分析查询，一些只能通过 key 访问，有的为时序数据做了专门优化，一些放大对象非常高效。为了理解这些数据库的不同，从一些分类和概述开始，这会帮助我们进一步的讨论。

我们首先会看一下数据库的架构，然后分析使用不同存储介质的数据库和数据布局不同的数据库。

上述两种分类是不完整的。有时会根据场景将数据库分为三类 OLTP（`Online transaction processing`）、OLAP（`Online analytical processing`） 和 HTAP（`Hybrid transactional and analytical processing`）。还有一些分类，比如 kv 数据库，关系数据库，文档数据库、图数据库。

第一部分聚焦于存储和索引，本节也会做简要的介绍。

## DBMS Architecture

## Memory- Versus Disk-Based DBMS

### Durability in Memory-Based Stores

## Column- Versus Row-Oriented DBMS

### Row-Oriented Data Layout

### Column-Oriented Data Layout

### Distinctions and Optimizations

### Wide Column Stores

## Data Files and Index Files

### Data Files

### Index Files

### Primary Index as an Indirection

## Buffering, Immutability, and Ordering

## Further Reading
