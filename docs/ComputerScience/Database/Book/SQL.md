本章会讨论数据库查询（`query`）语言——SQL。

虽然称为查询语言，但是它能够定义数据结构，修改数据库的数据，指定约束。

这一章并不是提供一个完整的用户手册，而是讨论 SQL 的基础结构和概念。SQL 的实现细节有不同，或许仅支持子集。

## Overview of the SQL Query Language
IBM 在上世纪 70 年代做出 System R 项目的一部分开发了 SQL 的初始版本，成为 Sequel，不断演变，名字也改成了 SQL（`Structured Query Language`）。如今许多产品都支持 SQL，是关系型数据库的标准语言。ANSI 和 ISO 发布了一系列的标准。

SQL 包含以下几个部分：

* 数据定义语言（`Data-definition language`, `DDL`）：用于定义关系模式、删除关系和修改关系模式的命令。
* 数据操纵语言（`Data-manipulation language`, `DML`）：从数据库中查询信息，以及向数据库中插入、删除和修改元组。
* 完整性（`integrity`）：SQL DDL 包含用于指定数据库中的数据必须满足的完整性约束的命令。违反完整性约束的更新将被禁止。
* 视图定义（`view definition`）： SQL DDL 包含用于定义视图的命令。
* 事务控制（`transaction control`）：SQL 包含用于指定事务开始和结束的命令。
* 嵌入式（`embedded`） SQL 和动态（`dynamic`） SQL：定义了如何将 SQL 语句嵌入到通用编程语言中。
* 授权（`authorization`）：SQL DDL 包含用于指定对关系和视图的访问权限的命令。

这里会讨论 SQL 的 DML 和 DDL 功能，基于 SQL-92 标准，还包括 `join`、视图、事务、完整性约束、类型系统和授权、等等。

尽管大多数 SQL 实现都支持这里描述的标准功能，但不同实现之间仍存在差异。大多数实现支持某些非标准功能，同时可能省略了一些更高级或更新的功能支持.

## SQL Data Definition
数据库中的关系集合由 DDL 指定。SQL DDL 能够指定如下信息

* 每个关系的模式
* 每个属性关联的类型
* 完整性约束
* 每个关系上的索引
* 每个关系在磁盘上的物理存储结构

### Basic Types
SQL 支持如下内置类型

* `char(n)`：固定长度的字符串，长度是 `n`，全称字是 `character`。
* `varchar(n)`：变长字符串，最大长度是 `n`，全称是 `character varying`。
* `int`：依赖于机器的整数类型，一般是四个字节，全称是 `integer`。
* `smallint`：短类型整数，一般是两个字节。
* `numeric(p, d)`：指定精度的定点数。`p` 是指有多少个数字（包括符号），`d` 是指 `p` 个数字有多个数字在小数点后。`numeric(3, 1)` 可以精确保存 44.5，但是无法精确保存 444.5 或 0.32，会被截断。
* `real` `double`：单精度和双精度浮点数。
* `float(n)`：至少 `n` 个数字精度的浮点数。

每个类型都有一个特殊值，称为 `null`，表示值不存在或未知的。

`char` 存储固定长度，比如 `char(10)` 的属性存储了一个字符串 `Avi`，长度是 10，后面有 7 个空白字符。如果类型是 `varchar(10)`，那么长度是 3，没有额外的填充。当比较两个 `char` 类型的值，如果长度不同，会先填充空白字符到短的那个字符串然后再比较。比如比较 `char` 和 `varchar`，或许会填充空白字符到 `varchar` 类型到等长然后比较，但是不一定，这完全依赖于数据库，因此即使同样存储 `Avi`，但是类型不同，会返回 `false`。推荐总是使用 `varchar` 来避免这个问题。

### Basic Schema Definition
通过 `create table` 可以定义 SQL 关系，下面是创建 department 的 SQL
```sql
CREATE TABLE department (
    dept_name VARCHAR(20),
    building VARCHAR(15),
    budget NUMERIC(12, 2),
    PRIMARY KEY (dept_name)
);
```
这个关系有三个属性，dept_name，长度最大 20 的字符串，building，长度最大 15 的字符串，budget，12 个有效数字，小数点后两个数字。同时将 `dept_name` 指定为主键。

`create table` 的一般形式是
```sql
create table r
    (A_1 D_1,
     A_2 D_2,
     ...,
     A_n D_n,
     <integrity-constraint_1>,
     ...,
     <integrity-constraint_k>);
```
r 是关系名，$A_i$ 是属性名，$D_i$ 是对应的数据类型，即域。$A_i$ 的值支持的操作也有 $D_i$ 确定。语句最后有一个分号。

`primary key(A_j_1,A_j_2,...,A_j_m)` 定义了一组主键，主键不能是 null，并且要是唯一的。尽管主键是可选的，但是通常推荐每个关系都指定主键。

`foreign key(A_k_i,...,A_k_n) references s` 定义了外键约束，那么当前关系的这些属性的值对应 `s` 的某个元组的主键。

`not null` 表示某个属性不能有 null 值。
```sql
CREATE TABLE instructor (
    ID VARCHAR(5),
    name VARCHAR(20) NOT NULL,
    dept_name VARCHAR(20),
    salary NUMERIC(8, 2),
    PRIMARY KEY (ID),
    FOREIGN KEY (dept_name) REFERENCES department
);
```
SQL 会阻止违反完整性约束的更新。比如主键插入 null 值，或者有外键约束的属性上插入了引用关系主键没有的值。

`create table` 的逆操作是 `drop table`，后者会从数据库中删除这个关系的所有信息。
```sql
DROP TABLE r;
```
这个操作比
```sql
DELETE FROM r;
```
效果更强。后者删除所有的元组保留了关系 r，前者两者都删除了。关系 r 被删除之后，无法再插入数据，除非通过 `create table` 再次创建关系 r。

`alter table` 可以给已经存在的关系添加属性，新的属性的值会被设置为 null，命令形式是
```sql
ALTER TABLE r ADD A D;
```
其中 r 是已经存在的关系，类型 D 的属性 A 被添加到关系 r 中。这个命令也可以用于阐述属性，上面例子的逆操作是
```sql
ALTER TABLE r DROP A;
```
有的数据库不支持删除属性。

## Basic Structure of SQL Queries

### Queries on a Single Relation

### Queries on Multiple Relations
