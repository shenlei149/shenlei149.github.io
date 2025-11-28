关系模型至今仍然是商业数据处理应用的主要数据模型。与早期的网状模型或层次模型等模型相比，其由于简单性而获得了主导地位，这种简单性减轻了程序员的工作负担。在过去半个多世纪里，通过不断整合新特性和新功能来保持主导地位。这些新增功能中包括对象关系特性，比如复杂数据类型和存储过程、支持 XML 数据、支持半结构化数据的各种工具。关系模型独立于任何特定的底层数据结构，这一特性使其能够经久不衰，即便是在新的数据存储方法，包括为大数据而设计的现代列存，出现之后。

本章中我们讨论关系模型的基本原理。

## Structure of Relational Databases
一个关系型数据库由一组表（`table`）组成，每个表都有一个唯一的名字。下面是具体的例子，三个表，分别是 instructor, course, prereq。

| ID | name | dept_name | salary |
|--|--|--|--|
| 10101 | Srinivasan | Comp. Sci. | 65000 |
| 12121 | Wu | Finance | 90000 |
| 15151 | Mozart | Music | 40000 |
| 22222 | Einstein | Physics | 95000 |
| 32343 | El Said | History | 60000 |
| 33456 | Gold | Physics | 87000 |
| 45565 | Katz | Comp. Sci. | 75000 |
| 58583 | Califieri | History | 62000 |
| 76543 | Singh | Finance | 80000 |
| 76766 | Crick | Biology | 72000 |
| 83821 | Brandt | Comp. Sci. | 92000 |
| 98345 | Kim | Elec. Eng. | 80000 |

| course_id | title | dept_name | credits |
|--|--|--|--|
| BIO-101 | Intro. to Biology | Biology | 4 |
| BIO-301 | Genetics | Biology | 4 |
| BIO-399 | Computational Biology | Biology | 3 |
| CS-101 | Intro. to Computer Science | Comp. Sci. | 4 |
| CS-190 | Game Design | Comp. Sci. | 4 |
| CS-315 | Robotics | Comp. Sci. | 3 |
| CS-319 | Image Processing | Comp. Sci. | 3 |
| CS-347 | Database System Concepts | Comp. Sci. | 3 |
| EE-181 | Intro. to Digital Systems | Elec. Eng. | 3 |
| FIN-201 | Investment Banking | Finance | 3 |
| HIS-351 | World History | History | 3 |
| MU-199 | Music Video Production | Music | 3 |
| PHY-101 | Physical Principles | Physics | 4 |

| course_id | prereq_id |
|--|--|
| BIO-301 | BIO-101 |
| BIO-399 | BIO-101 |
| CS-190 | CS-101 |
| CS-315 | CS-101 |
| CS-319 | CS-101 |
| CS-347 | CS-101 |
| EE-181 | PHY-101 |

prereq 表的每行描述了一个课程是另一个课程的前置课程的关系。instructor 表的每行表示一个指定的 ID 与 name, dept_name, salary 之间的关系。

一般来说，表中的一行表示一组值之间的一种关系（`relationship`）。由于表表示这些关系的集合，因此表（`table`）与数学概念关系（`relation`）有对应关系，关系型数据模型也因此得名。数学术语中，元组（`tuple`）是一系列值，即值的列表，对应表的一行。术语属性（`attribute`）对应着一行的一列。使用术语关系实例（`relation instance`）指一个关系的特定实例，即包含一组特定的行的集合。比如上面的三个实例。

关系中元组的顺序不重要，因为关系是元组的集合（`set`）。

对于一个关系中的每一个属性，有一个可能取值的集合，这个集合称为域（`domain`）或者定义域。比如 instructor 关系中 salary 属性的域是所有可能的薪水的值的集合。

我们要求对于所有关系 `r` 的所有属性的域都必须是原子的（`atomic`）。如果一个域的元素被认为是不可再分的单元，那么这个域是原子的。假定 instructor 表有一个 phone_number 属性，它可以存储一组电话号码，那么就不是原子的，因为一个元素（一组电话号码）自身还包含子部分，即列表中各个独立的电话号码。

事情的关键不是域本身是什么，而是我们怎么使用域中的元素。假定现在 phone_number 属性存的是一个单独的电话号码，但是如果查询中要将 phone_number 拆成国家代码、区号和本地号码三个部分，那么应该将其视为非原子的。反之，每个号码是一个不可再分的单元，那么 phone_number 属性的域就是原子的。

null 值（`null value`）是一个特殊值，表示未知或者不存在的情况。稍后会看到 null 值使得访问或更新数据库时会遇到一些问题，如果不能为 null，要尽可能消除 null 的影响。这里先假定 null 值不存在，后续会描述 null 对不同操作的影响。

关系中如果有严格的结构，那么存储和查询都很有优势。这种严格的结构适合预先有明确定义且变化不大的应用，但是不适合数据和数据类型、结构会随时间变化的应用。现代化的企业需要在结构化数据的高效率和预定义结构带来限制之间取得平衡。

## Database Schema
讨论数据库的时候，需要区分数据库模式（`database schema`）和数据库实例（`database instance`），前者是数据库的逻辑设计，后者是某个时刻数据库中数据的快照。一般而言，关系模式由一组属性及其域组成。

关系相当于编程语言的变量，关系模式（`relation schema`）相当于类型定义，关系实例（`relation instance`）相当于变量的值。值可能会变，相当于实例的内容会变化，不过一般情况下关系的模式不会变化。

尽管知道关系模式和关系实例的区别很重要，不过在上下文确定的时候，只说关系即可，否则强调是模式还是实例。

department 模式是
```
department (dept_name, building, budget)
```
dept_name 在 department 和 instructor 两个关系中都出现了，这并不是偶然的。在不同的关系模式中使用相同的属性，是将不同关系的元组关联起来的一种方式。比如想知道在 Watson 工作的老师，可以先查 department  表，找到所有 building 是 Watson 的部门，然后再查 instructor 找到这些部门的老师。

| dept_name | building | budget |
|--|--|--|
| Biology | Watson | 90000 |
| Comp. Sci. | Taylor | 100000 |
| Elec. Eng. | Taylor | 85000 |
| Finance | Painter | 120000 |
| History | Painter | 50000 |
| Music | Packard | 80000 |
| Physics | Watson | 70000 |

除此之外，还可以有描述每门课的开课信息和与之关联的教师的信息，它们的模式如下。
```
section(course_id, sec_id, semester, year, building, room_number, time_slot_id)
teaches(ID, course_id, sec_id, semester, year)
```

## Keys
我们需要一种方式来指定一个元组在给定关系中有别于其他元组，也就是说，元组的属性值的部分可以由于唯一识别这个元组。不能有两条元组的所有属性的值都相同。

超键（`superkey`）就是这样一个或多个属性的集合，用于唯一确定一个元组。比如 instructor 的 ID 属性足够确定一个元组，因此 ID 是超键。name 就不是超键，因为教师可以重名。

在关系 $r$ 中，$R$ 是属性的集合。如果 $R$ 的一个子集 $K$ 是 $r$ 的超键，那么 $r$ 中不会存在两个不同元组其 $K$ 上的值都相等。也就是说如果 $t_1\in r,t_2\in r$ 并且 $t_1\neq t_2$ 那么 $t_1.K\neq t_2.K$。

超键可以包含额外的属性，比如 ID 和 name 是 instructor 的超键。如果 $K$ 是超键，那么 $K$ 的超集也是超键。通常我们讨论没有子集是超键的超键，这样最小化的超键称为候选键（`candidate key`）。


