# 背景与动机
## 行业趋势与挑战
随着数据规模增长，数据分析场景正经历着从“传统数仓”向“云原生湖仓一体”演进。我们面临的数据特征也发生了根本性变化：

- 超大宽表常态化：机器学习特征工程中，单表列数突破 10,000 列已成常态。
- 数据结构复杂化：半结构化数据（JSON、Map、List）占比激增，分析深度不断加深。
- 存算分离架构：数据存储在 HDFS/S3 等对象存储上，高延迟、随机 I/O 差成为核心瓶颈。

## 现有方案 (Parquet) 的局限性
Parquet 作为十年前设计的列存标准，在面对上述新挑战时难以满足性能需求，成为制约计算引擎性能的瓶颈：

- 元数据瓶颈: Parquet 采用集中式 Thrift Footer 设计。对于万列大宽表，仅 Footer 序列化完仍旧几十甚至上百 MB，在内存中大小达到几百 MB 甚至 GB 级别。读取任何一列都必须串行反序列化整个 Footer，导致高延迟且内存占用高。
- 嵌套类型处理低效: 基于 Dremel 的 Repetition/Definition Levels 编码虽然存储紧凑，但在读取时需要复杂的有限状态机重组数据，难以向量化。
- 云原生适配差: 元数据与数据耦合，缺乏针对对象存储的高延迟优化，导致大量小 I/O 请求。

## Fermion 设计初衷
为了解决上述技术瓶颈，Fermion 应运而生：它不是对现有格式的修补，而是基于“元数据解耦”与“物理流分离”理念的架构重构。

| 特性 | Parquet |  Fermion | 优势说明 |
| --- | --- | --- | --- |
| **元数据架构** | **集中式 (Centralized):** 单一巨大的 Thrift Footer 位于文件末尾。 | **分布式 (Decoupled):** 分离的、每列独立的 CMB + CIT 索引。 | 避免了巨大元数据对象的解析开销，支持超宽列场景。 |
| **元数据格式** | **Thrift:** 消耗大量 CPU 进行序列化与反序列化 | **POD:** 磁盘与内存布局一致 | 序列化反序列化成本极低 |
| **元数据读取** | **全量加载:** 打开文件必须反序列化所有列的元数据。 | **按需加载:** 查询某列时才读取解析该对应元数据。 | I/O 开销与“查询列数”成正比，而非“总列数”。 |
| **并行能力** | **受限:** 元数据解析是单线程瓶颈；RowGroup 间并行度有限。 | **原生并行:** 元数据、数据解压均可多线程并行执行。 | 充分利用多核 CPU 和磁盘的高吞吐。 |
| **数据组织** | **Row Group:** 物理上的行组，元数据存储在 Footer。 | **Stripe:** 逻辑行组，元数据存储在 CMB。 | 同样提供了数据分片、过滤的能力，但元数据组织更优。 |
| **过滤效率** | **依赖 PageHeader:** 往往需要读取数据页头才能过滤，或依赖分散的 ColumnIndex。 | **依赖统一的元数据:** 元数据与数据完全隔离。 | 极致的谓词下推。在读取任何数据前，仅凭元数据即可剔除无效 Page，节省大量 I/O 或解析成本。 |

# 设计目标

- 线性可扩展的元数据管理：元数据独立，反序列化成本极低。支持超宽表格场景，按需加载。
- 高性能读写: 为大规模数据的顺序扫描和随机访问提供高吞吐与低延迟。
- 高效的压缩: 通过列式存储和先进的编码技术，实现业界领先的数据压缩率。
- 元数据解耦: 针对高延迟/高吞吐存储特性进行深度优化，最大限度地减少 I/O 操作次数（IOPS）。
- 灵活的数据模型: 原生支持包括 `List`, `Struct`, `Map` 在内的复杂嵌套数据类型。
- 并行：文件布局考虑并行化问题，元数据和数据都能够并行读取、解析、使用。
- 生态兼容性：Arrow 原生兼容，确保 Spark/Flink 等计算引擎能够低成本接入。

# 嵌套类型设计
常见的嵌套类型使用了 Dremel 模型，使用 Repetition Levels 和 Definition Levels (R/D Levels) 将嵌套结构“拍平”为单一的数据流。Parquet 使用 Dremel 模型，其核心是针对存储压缩率优化，而非读取性能（参考附录中“嵌套类型示例”一节）。

Fermion 采用 Decomposed Columnar Streams (DCS) 模型，与 Apache Arrow 的内存布局严格对齐。任何嵌套类型均被拆解为三个基础流的组合，独立压缩存储：
```cpp
enum class StreamType : uint8_t {
    DATA = 0,
    VALIDITY = 1,
    OFFSETS = 2,
};
```

* `DATA` 存放数据。
* `VALIDITY` 表示数据是否有效，`null` 取反。
* `OFFSETS` 存放 `List` 的偏移数组。

此外，为了高效支持 batch 模式读写，引入了专用的 `BATCH_META`。该 Stream 可以看作是系统列，存储 batch id 和 length 这些元信息。详见附录“Batch 模式支持”一节。

结合 Schema 定义的逻辑树结构，深度优先遍历，序列化所有流。比如存储 `List<int64>` 类型的数据 `[[1, 2], null, [3]]`，那么被拆分成三个流：

* Stream 1 (Validity): `1, 0, 1` (第2行为空)
* Stream 2 (Offsets): `0, 2, 2, 3` (定义了 List 的长度分别为 2, 0, 1)
* Stream 3 (Data): `1, 2, 3` (纯数值，紧凑排列)

更复杂的例子详见附录中“嵌套类型示例”一节。

虽然逻辑上拆分了流，但在物理存储时，将属于同一逻辑列的所有流紧邻存放，需要时可以整块读取。

下面是两种方案的对比。

| 特性 | Parquet (Dremel 模型) | Arrow (采纳方案) |
| --- | --- | --- |
| 核心思想 | 完全扁平化。使用 Repetition/Definition Levels (R/D Levels) 状态机来重建嵌套结构。 | 物理嵌套/分解。保留结构的物理层级，使用 Offsets 数组和 Validity Bitmap 描述结构。 |
| 读取过程 | CPU 密集型。需要逐个值解析 R/D Levels，通过状态机重组对象，无法向量化，也无法并行。 | 文件中的各个流解压解码到内存就能组成成为 Arrow 对象。 |
| 统计信息* | 混合在一起，难以分离。 | 完全解耦。Offsets 流不存统计，Data 流存统计。 |
| 随机访问 | O(N)。必须从头扫描 Levels 才能找到第 N 个元素。 | O(1)。直接通过 Offsets 数组定位。 |
| 理解难度 | 高 | 低，非常直观。 |
| 实现难度 | 极高，R/D Levels 逻辑非常晦涩。 | 中等/低，直接对应内存模型。 |
| 元数据 | 简单（只有列的概念）。 | 结构化（CMB 包含流列表）。 |

* 统计信息：List 的 offset 隐含了 List 的统计信息，可用于基于长度的过滤。在训练场景的全量读取模式下，该统计信息通常被忽略。

# 文件布局
文件主要包含两个部分，数据部分和元数据部分，并且这两个部分是完全分离的。

元数据采用 POD 布局，磁盘二进制格式与内存对象布局一致，无需反序列化即可直接访问。

元数据部分自下而上是

1. Footer：记录 Schema、CIT 的偏移量，版本信息，Settings，Magic Number 用于文件检验。
```cpp
uint64_t schemaOffset_;
uint64_t citOffset_;
uint32_t version_;
uint32_t settings_;
uint64_t magicBytes_;
```

2. CIT（`Column Index Tree`）：索引每列对应的 CMB 的偏移量。假定有 n 列，那么是 n 个 `int64_t` 值。
3. Schema：序列化后的 Schema。
4. CMB（`Column Metadata Block`）：列级元数据自治单元。包含该列所有 Stripes 的索引与统计信息，支持独立加载。具体参考后续小节。

寻址流程：
1. 解析 Footer：获取 Schema 和 CIT 的物理偏移量。
2. 定位列元数据：根据 Column ID 在 CIT 中查找对应的 CMB Offset。
3. 加载 CMB：读取并解析目标列的 CMB。
4. 定位数据流：根据 CMB 中的 ChunkDesc 定位物理数据流。

数据部分依旧保留了逻辑 RowGroup/Stripe 的概念以保持数据对齐。每一列数据紧密排列，物理亲和。列与列之间也是紧密排布的，Stripe 之间亦然。

Parquet 中元数据索引是 `List<RowGroup> -> List<ColumnChunk>`，先找到 RowGroup 再找到对应的列。与 Parquet 不同，Fermion 的设计顺序相反，`List<Column> -> List<Stripe(Chunk)>`，先找到列，然后分成多个 Stripes。

```
[File Start]
+-----------------------------------------------------------+
|  Data Area                                                |
|                                                           |
| +-------------------------------------------------------+ |
| | Stripe 0                                              | |
| |   [Col 0 Stream 0] [Col 0 Stream 1]                   | |
| |   [Col 1 Stream 0] ...                                | |
| +-------------------------------------------------------+ |
| | Stripe 1 ...                                          | |
| +-------------------------------------------------------+ |
|                                                           |
+-----------------------------------------------------------+
| CMB Area (Column Metadata Blocks)                         |
|   [CMB for Col 0]                                         |
|   [CMB for Col 1]                                         |
|   ...                                                     |
+-----------------------------------------------------------+
| Schema Area (Serialized Schema)                           |
+-----------------------------------------------------------+
| CIT Area (Column Index Table)                             |
|   [Offset Array pointing to CMBs]                         |
+-----------------------------------------------------------+
| File Footer (Fixed Size)                                  |
|   [Schema Offset] [CIT Offset]                            |
|   [Version] [Settings] [Magic]                            |
+-----------------------------------------------------------+
[File End]
```

## Column Metadata Blocks
CMB 采用如下布局，采取固定长度加对齐的设计思路，解析速度快。
```
[CMB START] (Base Address)
+-----------------------------------------------------------------------+
| 1. HEADER AREA                                                        |
|    - numStripes_ (4B)                                                 |
|    - numStreams_ (4B)                                                 |
+-----------------------------------------------------------------------+
| 2. STRIPES                                                            |
|    - StripeMeta[0] { rowCount_: 1000 }                                |
|    - StripeMeta[1] { rowCount_: 2000 }                                |
|    ...                                                                |
|    (size = numStripes_ * sizeof(StripeMeta))                          |
+-----------------------------------------------------------------------+
| 3. STREAM DIRECTORY (Fixed size array)                                |
|    - StreamMeta[0]                                                    |
|      { chunksOffset_: 1024, descType_: SIMPLE }                       |
|    - StreamMeta[1]                                                    |
|      { chunksOffset_: 0,    descType_: SIMPLE }                       |
|    ...                                                                |
|    - StreamMeta[N]                                                    |
|    (size = numStreams_ * sizeof(StreamMeta))                          |
+-----------------------------------------------------------------------+
| 4. PAYLOAD AREA (Variable length)                                     |
|    +-------------------------------------------------------------+    |
|    | [Stream 0 ChunkDesc Array] (SimpleDesc[numStripes_])        |    |
|    | - Stripe 0: { offset: 500, len: 100 }                       |    |
|    | - Stripe 1: { offset: 0,   len: 0   }                       |    |
|    +-------------------------------------------------------------+    |
|    | [Stream 2 ChunkDesc Array] (FullChunkDesc[numStripes_])     |    |
|    | ...                                                         |    |
|    +-------------------------------------------------------------+    |
|    | [PageMeta Pool] (reffed by FullChunkDesc )                  |    |
|    +-------------------------------------------------------------+    |
+-----------------------------------------------------------------------+
[CMB END]
```

结构体定义参考附录“Column Meta 结构体定义”。

下面是对全部 NULL 和全部 Valid 场景的优化以及约定。

最极端的场景：列的所有 Stripes 都是 NULL，那么直接不写 CMB，也就是说对应的 CIT Offset 与指向下一个 CMB 的 Offset 一致。
> 在典型业务场景中（如 Schema 只增不减），全 NULL 列占比很高，该优化收益显著。

下面是对非极端场景的优化设计：

**枚举定义：**
*   `USE_DEFAULT`: 使用流的默认语义（Validity=全Valid, Data/Offsets=全Null/Empty），Payload 为空。
*   `FORCE_NULL`: 强制全 Null（专门优化 Validity 流的全 Null 场景），Payload 为空。
*   `SIMPLE`: 混合状态，Payload 为 `SimpleChunkDesc` 数组。
*   `FULL`: 混合状态，Payload 为 `FullChunkDesc` 数组。

| 流类型 (Stream) | 场景 (Stripe 状态) | StreamMeta.descType_ | StreamMeta.chunksOffset_ | Payload 内容 | 解释与备注 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Validity** | **全 Valid** (默认) | `USE_DEFAULT` | **0** | **无** (0 Bytes) | **极致优化**。整列所有行均有效。 |
| **Validity** | **全 NULL** (整列无效) | `FORCE_NULL` | **0** | **无** (0 Bytes) | **极致优化**。整列所有行均无效，无需存储压缩 0。 |
| **Validity** | **混合状态** (有空有值) | `SIMPLE` | **> 0** | `SimpleChunkDesc[3]` | 标准存储。每个 Chunk 指向对应的位图数据。 |
| | | | | | |
| **Offsets** | **全 Empty** (整列空 List) | `USE_DEFAULT` | **0** | **无** (0 Bytes) | **极致优化**。整列都是空列表。 |
| **Offsets** | **非全 Empty** (有数据) | `SIMPLE` | **> 0** | `SimpleChunkDesc[3]` | 标准存储。若某 Stripe 为空，则对应 `length=0`。 |
| | | | | | |
| **Data** | **全 NULL** (整列无值) | `USE_DEFAULT` | **0** | **无** (0 Bytes) | **极致优化**。整列无数据。 |
| **Data** | **非全 NULL** (有数据) | `FULL` | **> 0** | `FullChunkDesc[3]` | 标准存储。若某 Stripe 为空，则对应 `length=0`。 |

对于每一个 Stripe，Validity 也需要区分全有效（length = 0）和全部无效（length = max）两种情况，然后决定 StreamMeta 的 descType_ 和 chunksOffset_。

## 数据
数据存储可以看做是三级：Stripe -> Stream Chunk -> Page。这三级都是紧密存储的。

页是最小的压缩编码单元，默认大小是 512KB，可配置。对于 Stream Chunk 比较大的块，拆分成多个 Page。元数据中记录着 Chunk 的 Offset 和每个页的大小，因此可以得到每个页的 Offset，可以实现页级别的读取、解码和解压，降低内存和 CPU 的开销。

数据的布局如下
```
+-----------------------------------------------------------------------+
| DATA AREA                                                             |
+-----------------------------------------------------------------------+
|                                                                       |
| [STRIPE 0] (Logical Separation, Physical Affinity)                    |
|                                                                       |
|   +---------------------------------------------------------------+   |
|   | Stream 0 (Validity) Chunk                                     |   |
|   | [ Page 0 ]                                                    |   |
|   +---------------------------------------------------------------+   |
|                                                                       |
|   +---------------------------------------------------------------+   |
|   | Stream 1 (Offsets) Chunk                                      |   |
|   | [ Page 0 (Delta/FOR + ZSTD) ]                                 |   |
|   +---------------------------------------------------------------+   |
|                                                                       |
|   +---------------------------------------------------------------+   |
|   | Stream 2 (Data) Chunk                                         |   |
|   |                                                               |   |
|   |  +---------------------------+  +---------------------------+ |   |
|   |  | Page 0                    |  | Page 1                    | |   |
|   |  | (Encoding + ZSTD)         |  | (Encoding + ZSTD)         | |   |
|   |  | Size: 50KB                |  | Size: 48KB                | |   |
|   |  +---------------------------+  +---------------------------+ |   |
|   |  ^                              ^                             |   |
|   |  |                              |                             |   |
|   +--|------------------------------|-----------------------------+   |
|      |                              |                                 |
|      | CMB.FullChunkDesc Offset     | Offset + CMB.PageMeta[0].size   |
|                                                                       |
+-----------------------------------------------------------------------+
| [STRIPE 1] ...                                                        |
+-----------------------------------------------------------------------+
```

# 写入流程
Writer 采用 "Data First, Meta Last"（先写数据，后写元数据）的策略。这意味着 Writer 必须在内存中维护元数据，直到文件关闭。

## 流程图
```
+-----------------+      +------------------+      +------------------+
|   User Input    | ---> |   Stripe Buffer  | ---> |   Page Encoder   |
| (Rows/Batches)  |      |                  |      | (Encoding/ZSTD)  |
+-----------------+      +------------------+      +------------------+
                                                            |
        +---------------------------------------------------+
        | (Page Data)
        v
+------------------+      +------------------+
|   File Stream    | ---> | Disk: Data Region|  <=== [Flush]
| (Append Write)   |      +------------------+
+------------------+
        |
        | (Current Offset & Length)
        v
+------------------+      +------------------+
| Metadata Recorder| ---> | List<ChunkDesc>  |  <=== [In Memory]
+------------------+      +------------------+
                                |
        +-----------------------+
        | (Write all stripes)
        v
+------------------+      +------------------+
|    CMB Builder   | ---> | Disk: CMB Region |  <=== [Flush]
+------------------+      +------------------+
        |
        v
+------------------+      +------------------+
|  Schema Builder  | ---> | Disk: Schema     |  <=== [Flush]
+------------------+      +------------------+
        |
        v
+------------------+      +------------------+
|    CIT Builder   | ---> | Disk: CIT        |  <=== [Flush]
+------------------+      +------------------+
        |
        v
+------------------+      +------------------+
|   File Footer    | ---> | Disk: Footer     |  <=== [Flush]
+------------------+      +------------------+
```

## 详细步骤
### 阶段一：数据缓冲与持久化
1. 缓冲: 用户写入的数据首先进入 `Stripe Buffer`。Writer 按照预设的行数（如 10,000 行）或者是缓冲区大小（如 512MB）将数据划分为 Stripe。
2. 编码: 当一个 Stripe 满时，Writer 遍历该 Stripe 的所有 Stream（Validity, Data 等）。将数据切分为 Page，并进行编码和压缩。
  - 如果开启了 batch 模式，Writer 会执行 Page 对齐策略”。即在切分 Page 时进行检查，确保同一个 Batch 的数据不会跨越 Page 边界，以保证 Batch 读取的原子性。详见附录“Batch 模式支持”一节
3. 写入数据区: 将压缩后的 Page 二进制流直接追加写入文件。
  - 关键动作： 在写入前，获取当前偏移量 `Offset`。写入后，计算数据长度 `Length`。
### 阶段二：元数据收集
4. 记录索引: 将上一步获取的 `{Offset, Length}` 以及统计信息（Min/Max/NullCount）封装成 `ChunkDesc` 结构体，存入内存中的 `std::vector<ChunkDesc>` 列表。每个 Stream 都有自己的列表。
### 阶段三：构建 CMB 并关闭文件
5. 构建  CMB: 将内存中的 `ChunkDesc` 列表转换为二进制的 Payload 数据块。此时记录的 `pageMetaOffset` 是相对于 Payload 起始位置的相对偏移。
6. 构建 生成 `StreamMeta` 数组。记录每个 Stream 在 Payload 中的相对位置。
7. 重定位: 最关键的一步！
  - 计算 Payload 之前的总大小 `MetaSize`。
  - 遍历所有 `StreamMeta`，将 `chunksOffset` 加上 `MetaSize`。
  - 遍历 Payload 中的 `FullChunkDesc`，将 `pageMetaOffset` 加上 `MetaSize`。
8. 写入 CMB: 按照之前的文件布局图依次将各个部分写入文件，同时记录偏移量。
9. 写入 Schema: 序列化 Schema 并写入，同时记录偏移量。
10. 写入 CIT: 使用之前记录的 CMB 的 Offset 写入 CIT 部分。
9. 写入 Footer: 最后写入一个定长的 Footer，包含 CIT 和 Schema 的 Offset。

# 读取流程
```
      [Step 1: Init]                  [Step 2: Parse]                 [Step 3: Access]

+--------------------+                                            +---------------------+
|    Open File       |                                            |   User Request      |
+--------------------+                                            | (Read Col A, Str 0) |
          |                                                       +---------------------+
          v                                                                  |
+--------------------+                                                       v
|    Read Footer     |                                            +---------------------+
|   (Get Offsets)    |                                            |   Read/Parse CMB    |
+--------------------+                                            +---------------------+
          |                                                                  |
          v                                                                  v
+--------------------+          +----------------------+          +---------------------+
|    Read Schema     | -------> |    Parse Schema      |          |   Read Data Area    |
+--------------------+          +----------------------+          |  (pread at Offset)  |
          |                                                       +---------------------+
          v
+--------------------+          +----------------------+
|     Read CIT       | -------> |      Parse CIT       |
+--------------------+          +----------------------+
```

## 详细流程
### 初始化
1. 读取 Footer:
  - 调用 `pread` 读取文件末尾的固定长度（如 4KB）。
  - 解析 Footer 结构，提取 `schemaOffset_` 和 `citOffset_`。
2. 读取 Schema:
  - 调用 `pread` 读取 Schema 区域。
  - 反序列化 Schema，构建逻辑列名（Column Name）到物理流 ID（Stream ID）的映射表。
3. 读取 CIT
### CMB 加载
Reader 根据配置参数 `EAGER_LOAD_THRESHOLD`（建议默认 8MB）决定加载策略。

策略 1：全量加载

- 适用场景： 文件较小，或 Stripe 数量较少。
- 操作：
  1. 一次性读取 CMB 所有数据。
  2. 解析 Header, Stripe, Stream Directory 等等元信息。

策略 2：按需加载 (Lazy Load)

- 适用场景： 大宽表（数千列），或 Stripe 极多（元数据巨大）。
- 操作：
  1. 根据 CIT 和 Schema 信息，确定要读取的列对应的 CMB。
  2. 读取 CMB 数据。
  3. 解析 Header, Stripe, Stream Directory 等等元信息。

### 数据访问
1. 流定位 (Stream Lookup):
  - 根据 Schema 映射找到对应的 CMB 和 `StreamMeta`。
  - 快速返回： 若 `chunksOffset == 0`，说明该流在文件中不存在（全 Null 或全 Default），直接返回构造的默认向量，零 IO。
  - 对于 Batch 模式，Reader 会读取 `BATCH_META` 流，并在内存中实时计算生成 `batch_id` 和 `index_in_batch` 虚拟列。详见附录“Batch 模式支持”一节。
2. 索引获取
  - 根据 `chunksOffset` 找到对应 `ChunkDesc`。
  - 若 `ChunkDesc.length == 0`，跳过该 Stripe。
  - 谓词下推：如果是 `FullChunkDesc`，根据 Min/Max 信息进行 Page 级别过滤。
  - 如果需要读取数据，计算 Offset 和 Length。
3. 数据读取
  - 优化点：在此处应用 IO 合并（`IO Coalescing`），如果多个 Stripe 或多个流的 Chunk 相邻，合并为一次读取。
  - IO 合并绝对是读取性能提升的重要一环，怎么强调都不过分。
    - CMB 和数据都要支持 IO 合并
    - 全表扫描和读取若干列场景均能兼顾
    - 避免大量小的 IO，导致 IOPS 过高
4. 解压解码
  - 解压： 使用 ZSTD 解压 Page 数据。
  - 解码： 逐层解码。支持分层解码，比如保留字典编码以节省内存。
  - 组装： 将数据填充到结果 Arrow Array 中返回。
### 异常处理
1. 文件截断:
  - 在读取 Footer 时，若文件实际大小小于 Footer 长度，抛出 `InvalidFileException`。
  - 在读取 CMB/Data 时，若 Offset + Length 超过文件大小，抛出 `EOFException`。
2. 校验和错误:
  - 若启用了 Checksum（CRC32），读取 Data 后需进行校验。校验失败抛出 `ChecksumException`。
3. 版本不兼容:
  - 检查 Header 中的 Magic Number 和 Version。若版本不支持，抛出 `UnsupportedVersionException`。

# 存储介质
存储介质的特性对列存系统的设计，特别是读写的优化，会有本质的影响。下面分 HDFS 和本地存储两种情况讨论。主要集中在前者，因为我们的数据存储在 HDFS 上而不是本地。

## HDFS
HDFS 主要特点是块存储（128MB 甚至更大）、网络延迟高、带宽大、写一次读多次（`Write-once, read-many`, `WORM`）。HDFS 底层可能是 HDD 或 SSD 磁盘。前者寻址成本高，尽可能不要随机访问，顺序读写。后者允许更高的并发读取，瓶颈可能会转移到带宽或 CPU（解压解码等）。

基于上述特点，可以制定针对性的优化。

1. Stripe 大小向 HDFS Block 对齐。
   - Stripe 在内存中大小可以设置为 512MB，如果考虑编码压缩，可以更大。
   - 读取时不会跨 HDFS 的 Block。
   - 劣势是 Writer 需要使用更多的内存。
2. 缓冲写
   - 编码压缩的单位是页，但是不要按照页为单位写数据。
   - 页在内存缓存，满一定大小，比如 4MB、8MB 再调用 HDFS 的写操作。
3. 元数据紧凑化
   - 更少的读写次数。
4. Footer 激进预读
   - Footer、CIT 和 Schema 是必读的，没有必要分三次读取。
   - 当打开文件时，直接读取 512KB 甚至更多的数据（2-4MB？）。有概率能够将所需信息一次性读取完。
5. CMB 读取
   - CMB 很小：利用 HDFS 高吞吐，直接全读。
   - CMB 较大：结合 IO 合并。
6. 移除 CIT
   - 如果 CMB 总大小比较小，比如小于 4MB，移除 CIT。
   - Footer 中 settings 可以记录这一变化，此时 CIT offset 就是 CMB 的 offset。
7. IO 合并（`IO Coalescing`）
   - Stream 是相邻紧密存储的，Validity、Offset、Data 在一起，无需多次 IO。
   - 不同列的 Stream，如果物理距离相近，比如 < 1MB，也可以合并成一个 IO。

总结，HDFS 自身的设计优化方向是高吞吐，因此文件读写优化的方向是尽可能减少读取的次数，每次读写的数据量尽可能大，发挥高吞吐的优势，避开高延迟的劣势。

## Local FS
相比 HDFS，本地读写有相当大的不同。

| 特性 | HDFS (分布式) | 本地磁盘 (Local FS) | 影响 |
| --- | --- | --- | --- |
| 延迟 (Latency) | 高 (毫秒级)。受网络 RPC 和协议栈影响。 | 极低 (微秒级)。仅受系统调用和硬件影响。 | **本地读写对“小 IO”更宽容。** |
| 吞吐 (Throughput) | 大。 | 更大。 |  |
| 接口 (API) | `read/write` (Socket 通信)。 | `pread/pwrite`, `mmap`, `io_uring`。 | 本地有更丰富的 IO 模式可选。 |
| 并发 (Concurrency) | 受限于网络带宽和连接数。 | 受限于 SATA/NVMe 队列深度。 | 本地 NVMe SSD 支持极高的并发 IO。 |

如果在本地存储，那么优化策略可能会有些许调整。

1. Stripe 大小可以更小
   - 更小的 Stripe 意味着更细力度的索引，查询过滤掉过更好。
   - 考虑编码压缩的效率，不易过小。
2. 写缓存
   - 4KB 对齐文件系统页
   - 默认写就好，OS 自带缓存
3. 读取优化
   - 元数据 CMB 直接内存映射即可
   - 数据异步 IO，可以利用 io_uring 等机制，尽可能用满 IO 带宽
   - 预读，`fadvise` 机制
   - 全表扫描使用 `O_DIRECT` 绕过系统缓存，防止将热数据（比如索引、元数据）挤占出去
4. HDD 的优化
   - seek 成本依旧很高，考虑 IO 合并
   - `mmap` 仅对元数据使用，避免随机访问数据导致大量不可控 seek

针对本地磁盘还有很多优化机制，由于不是重点，不再展开。

# 风险与迁移策略
1. 提供 Parquet 与 Fermion 格式互转工具
   - Parquet -> Fermion: 方便迁移历史数据
   - Fermion -> Parquet: 在生态不完善的时候方便 debug
2. 生态适配
   - 作为高优先级工作，优先适配 Paimon/Flink/Spark 等上下游工具，尽快端到端打通
3. 建议在一些小数据量场景使用，逐步扩大到全场景使用

# 总结
Fermion 列存格式的设计与实现，标志着我们在底层存储技术上迈出了关键一步。

## 核心突破
通过引入 CMB (Column Metadata Block) 和 CIT (Column Index Tree) 的分离架构，Fermion 成功将元数据的管理粒度从“文件级”下沉到了“列级”和“Stripe 级”。这一架构创新使得 Fermion 在处理超大宽表时，展现出了 Parquet 无法比拟的灵活性和稳定性。同时，基于 Stream 的物理存储模型，使得 Fermion 天然契合 Arrow 内存标准，实现从磁盘到计算的“零拷贝”通路。

## 业务价值
- 降本：
   - 更小的文件体积（10%+，有 demo 验证），存储、带宽收益
   - 降低计算节点的内存消耗（30%-50%，预期）
   - 高效的编码，减少 ZSTD 依赖，入湖任务和训练 CPU 消耗更少（待定）
- 增效：
   - 查询速度：提升 x 倍？
   - 在训练场景中提升 y 倍？(y>1)
- 面向未来的架构：为未来支持更大规模的数据集、更复杂的 AI 数据类型做好了底层储备。

作为新一代存储底座，Fermion 将有效解决当前业务面临的性能与稳定性瓶颈，为数据平台的长期演进奠定坚实基础。

## 工作项
* 编码
   - dict, Delta, FOR, Constant, Bit-Packing, RLE, Delta+FOR 等等
   - 支持嵌套
   - 全局字典优化
   - SIMD
* 类型
   - 数据类型: int16/32/64, float, double, binary, string
   - 嵌套类型: List, Struct, Map
* 生态集成
   - Spark、Flink、Paimon 计算框架等等
   - Arrow 等内存格式
* IO 合并
* 数据 Prefetch
* Page 并行解压解码
* Page CRC 校验
* Bloom Filter:
   - 在 `FullChunkDesc` 中加入 Bloom Filer 支持
   - 支持等值查询（`where id = xx`）的快速过滤

# 附录
## 嵌套类型示例
假定有一列类型是 `List<List<int64>>`，逻辑数据有两行，`[[1, 2], [3]]` `[[4]]`。

### Parquet 存储方式
Parquet 把嵌套结构拍平为三个流。

1. 数据流：`[1, 2, 3, 4]`。
2. 为了简化讨论，假定没有 `null`，Definition Level 均为最大值。
3. Repetition Level 需要记录当前值在哪一层。
    - 0 表示新的一行
    - 1 表示外层 List
    - 2 表示内层 List

对应数据 `[1, 2, 3, 4]` 的 RL 流是 `[0, 2, 1, 0]`。写入的时候需要按照如下逻辑构造 RL。

- (RL=0): 新行开始 -> `Row 0` -> `List 0` -> `SubList 0` -> 写入 1
- (RL=2): 内层重复 -> 继续 `SubList 0` -> 写入 2
- (RL=1): 外层重复 -> 结束 `SubList 0`, 开启 `SubList 1` -> 写入 3
- (RL=0): 新行开始 -> 结束 `Row 0`, 开启 `Row 1` -> `List 0` -> `SubList 0` -> 写入 4

读取的时候，需要根据 RL 的值，然后构造出 Arrow 所需的两层 Offsets 数组。代码中出现大量的 `if-else`，并且 RL 的值取决于数据本身，可能是随机跳变的，比如 `(0, 2, 2, 1, 2, 0...)`，结果就是分支预测非常难，对 CPU 流水线相当不友好。

根据上述读取逻辑，只能串行，无法并行，也无法使用 SIMD。

### Arrow/Fermion 存储方式
同样是三个流，如果有 `null` 值，可能流数会增多。

1. 数据流：`[1, 2, 3, 4]`。
2. 内层 offset：`[0, 2, 3, 4]`，表示 List 长度分别是 2,1,1
3. 外层 offset：`[0, 2, 3]`

写入时，根据 Schema `List<List<int64>>` 递归处理。首先看到 List，写入 `VALIDITY`，如果没有 `null` 会跳过，接着写入 `OFFSETS` 流，这是外层的 offset。进入 List 内部，发现还是 List，写入 `VALIDITY` 和 `OFFSETS` 流，再递归一层。这里是 `int64`，写入 `DATA` 流。

读取时根据元信息，解析三个流（或者更多流），解压、解码完直接赋值给 Arrow 结构即可。整个过程没有分支操作，内存拷贝即可。而内存拷贝比 RL 解析构造 offset 数组高效很多。

## 可选 CMB 布局
正文中 CMB 布局的优化方向是解析速度快，理解成本低。如果从存储低的角度考虑，StreamMeta 或许没有必要存储，下面的布局使用 Bitmap 表示 StreamMeta 是否存在。

放弃这种布局的原因是这种变化带来的存储收益非常低，但是导致理解成本上升，代码复杂度变高。
```
[CMB START]
+-----------------------------------------------------------------------+
| 1. HEADER AREA                                                        |
|    - numStreams_ (4B)                                                 |
|    - numStripes_ (4B)                                                 |
+-----------------------------------------------------------------------+
| 2. STRIPE REGISTRY                                                    |
|    - StripeMeta[0] { rowCount_: 1000 }                                |
|    - StripeMeta[1] { rowCount_: 2000 }                                |
|    ...                                                                |
|    (size = numStripes_ * sizeof(StripeMeta))                          |
+-----------------------------------------------------------------------+
| 3. STREAM DIRECTORY                                                   |
|    A. Presence Bitmap                                                 |
|       - [1 0 1 ...] (size = (numStreams_ + 7) / 8 Bytes)              |
|       - Bit=0: not exist, Bit=1: exists                               |
|                                                                       |
|    B. StreamMeta Array                                                |
|       - StreamMeta[0]                                                 |
|       - StreamMeta[1]                                                 |
|       ...                                                             |
|       (size = popcount(bitmap) * sizeof(StreamMeta))                  |
+-----------------------------------------------------------------------+
| 4. PAYLOAD AREA                                                       |
|    - [Stream 0 ChunkDesc Array] (SimpleDesc[numStripes_])             |
|    - [Stream 2 ChunkDesc Array] (FullChunkDesc[numStripes_])          |
|    - [PageMeta Pool] (reffed by FullChunkDesc )                       |
+-----------------------------------------------------------------------+
[CMB END]
```

## Column Meta 结构体定义
```cpp
struct PageMeta
{
	uint32_t pageSize_;
	uint32_t rowCount_;
	int64_t minVal_;
	int64_t maxVal_;
};

struct SimpleChunkDesc
{
	uint64_t offset_; // absolute offset in file
	uint32_t length_; // 0 means empty chunk
};

struct FullChunkDesc
{
	uint64_t offset_; // absolute offset in file
	uint32_t length_; // 0 means empty chunk

	// statistics for the chunk
	int64_t chunkMin_;
	int64_t chunkMax_;
	uint32_t nullCount_;

	uint32_t numPages_;
	uint32_t pageMetaOffset_; // point to the first PageMeta in Page Meta Block
							  // relative to the start of Column Meta Block
							  // if num_pages <= 1, then pageMetaOffset_ is ignored
};

struct StripeMeta
{
	uint32_t rowCount_;
};

static_assert(sizeof(StripeMeta) == 4, "StripeMeta size is not correct");

enum class DescType : uint8_t
{
	SIMPLE = 0, // Payload is SimpleChunkDesc array
	FULL = 1,	// Payload is FullChunkDesc array

	USE_DEFAULT = 2, // Validity=AllValid, Data=AllNull
	FORCE_NULL = 3	 // All Null for Validity
};

enum class StreamType : uint8_t
{
	VALIDITY = 0,
	OFFSETS = 1,
	DATA = 2,
};

struct StreamMeta
{
	DescType descType_;
	StreamType streamType_;
	std::array<uint8_t, 2> padding_; // for alignment

	// relative to the start of Column Meta Block
	// 0 means empty stream, default value (e.g., for null columns)
	uint32_t chunksOffset_;
};

static_assert(sizeof(StreamMeta) == 8, "StreamMeta size is not correct");

struct CMBHeader
{
	uint32_t numStreams_;
	uint32_t numStripes_;
};
```

## Batch 模式支持
### 背景
在 AI 训练、流式计算场景中，数据往往不是“逐行”产生的，而是以 Batch（批）为单位产生的。每个 Batch 包含一组行（比如 10 行）。读写应该将其看做是一个整体，视为逻辑上的一行。物理存储时，不应该跨 Page，以免产生额外的 I/O 和拼接开销。

### 共享系统列
为了记录哪些行是同一个 Batch，至少需要记录行数，当前业务可能需要 id，或者已经生成了全局 id，那么需要保存 batch id 和 length 两个信息。因此，我们增加两个系统列来存储这些信息。逻辑视图如下
```
[System: Batch Control]      [C0]            [C1]
-----------------------      ----------      ----------
Batch 1001 (Length=3)  --->  Row 0           Row 0
                             Row 1           Row 1
                             Row 2           Row 2
-----------------------      ----------      ----------
Batch 1002 (Length=2)  --->  Row 3           Row 3
                             Row 4           Row 4
-----------------------      ----------      ----------
```
Stream 新增一个类型，`StreamType::BATCH_META`，约定两个 Stream 中，第一个是 batch id，第二个存放 batch length。这些元数据占用空间极少。

当前文件是否 batch 形式写入还是普通写入，可以在文件 settings 中使用一个 bit 来记录该信息。

### Writer 协议：物理对齐
为了保证读取性能，Writer 必须严格遵守**原子性写入**。一个 batch 的数据绝对不允许跨越边界。

假定每一页 512KB，已经写了 510KB，还剩 2KB 空间，而下一个 batch 需要 10KB。Writer 放弃当前 Page 的剩余空间直接截断，进行编码压缩并记录元数据信息，然后开启新的 Page，从头开始写 这 10KB 的数据。

Reader 读取时，永远只在一个 Page 读取一个 batch 的内容，不会出现“读 Page A 的尾巴 + 读 Page B 的头”这种低效的跨页 I/O。

从 API 层面考虑，提供一个 `AppendBatch` 的接口来保证物理对齐，与普通的写入区别开来。

### Reader
对于用户而言，batch id 可以像普通列一样操作。

Reader 读取之后，内存中有两列：
* batch id: 物理存储中的 batch id，如果应用层需要，可以按照 length 展开。
* batch index: 根据 length 生成数据，比如 length = 4，那么数据是 `0 1 2 3`。

batch id 可以用于索引。假定应用层的需求是 `select * from t where batch_id = 1002`。

第一步，定位 Stripe。根据 Stripe 元数据中的记录 `min_batch_id` 和 `max_batch_id`，快速定位到当前 batch 属于哪一个 Stripe。
第二步，定位 Row。根据 batch id + length，可以计算出 row offset。如果数据页只有一页，读取这一页，按照 row offset 返回数据。如果 数据有多页，页中 `rowCount_` 可以用于定位属于哪一页，调整 row offset，返回数据。
