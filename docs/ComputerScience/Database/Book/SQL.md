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
SQL 查询的基本结构包含三个子句 `select` `from` `where`。`from` 子句列出输入的关系，有 `where` `select` 描述其上的操作，输出一个关系作为结果。

附录可以找到建表和导入数据的 [SQL 语句](./Appendix.md)，方便实验。

### Queries on a Single Relation
下面是一个最简单的查询，返回 instructor 中教师的名字。
```sql
select name from instructor;
```
下面是返回 instructor 所有老师属于哪一个部门，结果中一个部门可能出现多次。
```sql
select dept_name from instructor;
```
如果想去除重复，可以使用 `distinct` 关键词。
```sql
select distinct dept_name from instructor;
```
SQL 允许使用 `all` 显式指定不要去重，但是这是默认行为，因此一般不会使用 `all`。需要去重的时候指定 `distinct` 即可。

`select` 可以包含算数表达式，涉及 + - * /，比如下面返回的结果将工资乘了 1.1。
```sql
select ID, name, dept_name, salary * 1.1 from instructor;
```
`where` 可以只选择满足谓词的元组。下面的 SQL 结果是计算机学院工资大于 70000 的教师名字。
```sql
select name
from instructor
where
    dept_name = 'Comp. Sci.'
    and salary > 70000;
```
`where` 支持逻辑运算 `and` `or` `not`，比较运算符支持 `>` `>=` `<` `<=` `=` `<>`。

### Queries on Multiple Relations
上述讨论的都是涉及一个关系的查询，实际上查询往往涉及多个关系。

比如，想知道教师的名字，部门名以及工作的地点。从 instructor 模式中看，无法从这个关系查询到工作地点。因此 `from` 子句需要包含 instructor 和 department 两个关系。dept_name 在两个关系中都出现了，因此需要额外指定关系名以满足结果属性的名字都是唯一的这一个要求。
```sql
select name, instructor.dept_name, building
from instructor, department
where
    instructor.dept_name = department.dept_name;
```

下面更正式的讨论 SQL 的基本结构。查询包含三个子句：

* `select` 列出了查询结果期望的属性
* `from` 列出了查询要访问的关系
* `where` 是涉及各个关系的属性的谓词

典型的查询形式如下
```
select A_1,A_2,...,A_n
from r_1,r_2,...,r_m
where P;
```
$A_i$ 表示属性，$r_i$ 表示属性，$P$ 是谓词。如果没有 `where` 子句，谓词 $P$ 是 `true`。

尽管写的顺序是 `select` `from` `where`，但是操作理解的顺序是 `from` `where` `select`。

`from` 子句定义了关系的笛卡尔积。从关系代数的角度看有正式的定义，不过从迭代的生成元组的角度思考更容易理解。
```
for each tuple t_1 in relation r_1
  for each tuple t_2 in relation r_2
    ...
      for each tuple t_m in relation r_m
        Concatenate t_1, t_2, ..., t_m into a single tuple t
        Add t into the result relation
```
结果中有 `from` 子句列出的所有关系的属性。由于 $r_i$ 和 $r_j$ 可能有同名属性，因此需要加上关系名作为前缀。

笛卡尔积自身可能没有太多意义，比如 instructor 和 teaches 会产生大量无意义的结果，也就是两边不是同一个教师的情况。通常，会使用 `where` 添加谓词来限制笛卡尔积使得结果有意义。比如下面的查询就是返回每个教师教的名字和教的课程 ID。如果一个老师没有教任何课程，那么不会出现结果集中。
```sql
select name, course_id
from instructor, teaches
where
    instructor.ID = teaches.ID;
```
如果想知道计算机学院的老师教哪些课程，`where` 子句增加一个过滤条件即可。
```sql
select name, course_id
from instructor, teaches
where
    instructor.ID = teaches.ID
    and instructor.dept_name = 'Comp. Sci.';
```
一个 SQL 查询可以认为是执行了以下三个步骤：

1. `from` 子句的关系生成一个笛卡尔积
2. `where` 过滤掉不符合条件的元组
3. 根据 `select` 输出指定的属性

这个顺序帮助理解 SQL 查询结果，但是不是真实执行的步骤。真实的实现不会按照这种方式执行，而是会优化，只生成部分需要的笛卡尔积结果。后续查询处理和查询优化会讨论相关内容。

## Additional Basic Operations
### The Rename Operation
SQL
```sql
select name, course_id
from instructor, teaches
where
    instructor.ID = teaches.ID;
```
中的 `name, course_id` 可以从 `from` 的关系的属性中推导出来。

但是并不是总能推导出来。第一种情况是 `from` 的关系有同名的属性，那么结果中需要指定是哪一个关系的属性。第二种情况是 `select` 中包含算术表达式，结果属性并没有一个名字。第三种情况是即使能够推导出来名字，但是我们期望修改结果集中的属性名。因此，SQL 中提供了 `as` 这个重命名操作。它可以出现在 `select` 和 `from` 中。

比如下面的查询将 instructor 中的 name 改成了 instructor_name。
```sql
select name as instructor_name, course_id
from instructor, teaches
where
    instructor.ID = teaches.ID;
```
使用 `as` 重命名关系是很常见、很有用的。一个原因是将很长的关系名使用短的别名在查询的其他地方使用。比如下面的例子
```sql
select T.name, S.course_id
from instructor as T, teaches as S
where
    T.ID = S.ID;
```
更重要的原因是用于在查询中想要比较同一个关系中不同的元组的场景。需要求关系与自身的笛卡尔积，如果没有重命名就无法区别元组来自哪一个关系。比如下面的例子，如果直接写 instructor.salary，那么谓词两边相同，无法区分关系了。这个例子是要查找比薪水生物系最低薪水要高的教师。
```sql
select distinct
    T.name
from instructor as T, instructor as S
where
    T.salary > S.salary
    and S.dept_name = 'Biology';
```
类似 T、S 这样用于重命名关系的标识符，在 SQL 中被称为相关名（`correlation name`），也常被称为表别名（`table alias`）、相关变量（`correlation variable`）或元组变量（`tuple variable`）。

### String Operations
SQL 中，字符串由单引号 ' 包围起来，如果想在字符串内部表示单引号，那么写两个单引号 '' 即可。

SQL 标准中字符串比较是大小写敏感的，比如 'comp. sci.' = 'Comp. Sci.' 返回 `false`。不过有的数据库并没有按照标准实现。

SQL 提供了各种各样的字符串操作，比如连接字符串 `||`，求子字符串，求字符串长度，转成小写（`lower`），转成大写（`upper`），清除两端的空白字符（`trim`）等等。

`like` 表达式可以用于模式匹配。有两种特殊的模式：

* 百分号 `%`：匹配任意子字符串
* 下划线 `_`：匹配任意字符

标准中模式匹配也是大小写敏感的，不过有的数据库并没有按照标准实现。下面的查询是要查找 building 包含子字符串 `Watson` 的部门的部门名。
```sql
select dept_name from department where building like '%Watson%';
```
为了让模式本身能够包含特殊的模式字符，使用转移字符 `\`。使用关键词 `escape` + 字符的形式可以自定义转义字符。 `like 'ab∖%cd%' escape '∖'` 表示匹配以 `ab%cd` 开始的字符串。`like 'ab∖∖cd%' escape '∖'` 表示匹配以 `ab\cd` 开始的字符串。

`not like` 返回不匹配的结果。

### Attribute Specification in the Select Clause
`select` 中的 `*` 表示所有属性，比如下面的查询就要求返回 instructor 的所有属性。
```sql
select instructor.*
from instructor, teaches
where
    instructor.ID = teaches.ID;
```
查询中如果写 `select *` 要求返回 `from` 列出的所有关系的所有属性。

### Ordering the Display of Tuples
`order by` 的作用让对输出结果按照某些属性排序。比如下面的查询要求名字按照字母序输出。
```sql
select name
from instructor
where
    dept_name = 'Physics'
order by name;
```
默认是按照升序排列。可以在属性名后面添加 `desc`（降序）或 `asc`（升序）来指定顺序。比如下面的查询找到薪水降序排列，如果薪水一样，那么按照名字升序排列。
```sql
select * from instructor order by salary desc, name asc;
```

### Where-Clause Predicates
SQL 提供 `between` 来简化 `where` 的表达，语义是小于等于某个值且大于等于某个值。比如查询所有薪水大于等于 90,000 小于等于 100,000 的教师名字，下面两种写法等价。
```sql
select name from instructor where salary between 90000 and 100000;

select name
from instructor
where
    salary <= 100000
    and salary >= 90000;
```
类似的，也支持 `not between` 操作。

SQL 允许使用 $(v_1,v_2,\cdots,v_n)$ 将值 $v_1,v_2,\cdots,v_n$ 看做一行，这个记号称为行构造器（`row constructor`）。比较可以用于元组，顺序是字典序。比如 $(a_1,a_2)\leq(b_1,b_2)$ 等价于 $a_1\leq b_1$ 并且 $a_2\leq b_2$。类似的，两个元组相等等价于每一个属性都相等。下面两个 SQL 是等价的。
```sql
select name, course_id
from instructor, teaches
where
    instructor.ID = teaches.ID
    and instructor.dept_name = 'Biology';

select name, course_id
from instructor, teaches
where (
        instructor.ID,
        instructor.dept_name
    ) = (teaches.ID, 'Biology');
```

## Set Operations
SQL 提供 `union`（并集）、`intersect`（交集）和 `except`（差集）三个算子，输入是两个关系，返回结果集。默认情况下，这三个算子会去重，比如并集操作，在两个集合都存在的元组在结果集中只会出现一次。如果需要保留所有元组，加上 `all` 关键字，算子全名分别是 `union all` `intersect all` `except all`。下面是几个示例。
```sql
(
    select course_id
    from section
    where
        semester = 'Fall'
        and year = 2017
)
union all
(
    select course_id
    from section
    where
        semester = 'Spring'
        and year = 2018
);

(
    select course_id
    from section
    where
        semester = 'Fall'
        and year = 2017
)
intersect all
(
    select course_id
    from section
    where
        semester = 'Spring'
        and year = 2018
);

(
    select course_id
    from section
    where
        semester = 'Fall'
        and year = 2017
)
except
(
    select course_id
    from section
    where
        semester = 'Spring'
        and year = 2018
);
```

## Null Values
`null` 值的出现导致很多问题，设计算数、比较和集合操作。

`+` `-` `*` `/` 的任意一个操作数是 `null`，那么结果就是 `null`。

比较的话更复杂。比如比较 `1 < null`，如果结果是 `true`，完全不知道其含义，如果是 `false`，那么 `not(1 < null)` 就是 `true`，也没有意义。因此 SQL 引入了第三种逻辑值 `unknown`，表示涉及 `null` 的比较。

`where` 中的谓词可以使用 `and` `or` `not`，因此这些对 `unknown` 也有特殊规定。

* `and`：`true and unknown` 的结果是 `unknown`，`false and unknown` 的结果是 `false`，`unknown and unknown` 的结果是 `unknown`。
* `or`：`true or unknown` 的结果是 `true`，`false or unknown` 的结果是 `unknown`，`unknown or unknown` 的结果是 `unknown`。
* `not`：`not unknown` 的结果是 `unknown`。

这样结果就自洽了，`1 < null` 和 `not(1 < null)` 都返回 `unknown`。

对于 `where`，谓词结果是 `false` 或 `unknown`，元组都不会出现在结果集中。

`null` 是关键字，可以用于 `null` 值的比较。比如下面查询 salary 是 `null` 的教师的名字。
```sql
select name from instructor where salary is null;
```
`is unknown` `is not unknown` 可以知道比较结果是不是 `unknown`，而不仅仅是 `true` `false`。
```sql
select name from instructor where salary > 10000 is unknown;
```
当使用 `select distinct` 时，需要删除重复的元组。当比较两个元组的时候，如果对应值都不是 `null` 并且值相同或者都是 `null`，那么视为相同。比如 `{('A',null), ('A',null)}` 被视为相同。使用 `distinct` 时只会输出一个元组。注意，这里对 `null` 的处理和谓词中是不同的，谓词中 `null = null` 会返回 `unknown`，而不是 `true`。

这种比较元组的方式也适用于集合操作。

## Aggregate Functions
聚合函数输入是值的集合，输出一个值。SQL 提供五个标准的聚合函数。

* `avg()`：平均值
* `min()`：最小值
* `max()`：最大值
* `sum()`：求和
* `count()`：计数

`sum` `avg` 必须是数的集合，其他操作可以应用于非数值类型，比如字符串。

### Basic Aggregation
下面的查询返回计算机学院教师的平均工资。
```sql
select avg(salary) from instructor where dept_name = 'Comp. Sci.';
```
这个查询只有一个属性，也只有一个元组。数据库会为新的属性生成一个名字，我们也可以通过 `as` 给一个更有意义的名字。
```sql
select avg(salary) as avg_salary
from instructor
where
    dept_name = 'Comp. Sci.';
```
在有的场景中，需要去重，那么在聚合表达式中加上 `distinct` 关键字。比如下面的查询，返回 2018 年秋季有课的教师数，不管一个教师教了几门课。
```sql
select count(distinct ID)
from teaches
where
    semester = 'Spring'
    and year = 2018;
```
我们常常使用 `count` 得到元组数， SQL 提供表达式 `count(*)` 用于此目的。
```sql
select count(*) from course;
```
SQL 不允许 `distinct` 用于 `count(*)`，不过将 `distinct` 用于 `max` `min` 是合法的，但是并不会改变结果。`distinct` 的位置也可以写 `all`，表示不去重，但是这是默认行为，因此不需要显式写出来。

### Aggregation with Grouping
有时需求不是对所有元组求聚合，而是按照某些条件分组，SQL 中 `group by` 就是用于此需求。`group by` 的属性用于分组，这些属性上相等的元组会被分为一组。

下面的查询返回每个部门的平均薪水。
```sql
select dept_name, avg(salary) as avg_salary
from instructor
group by
    dept_name;
```
如果没有 `group by`，那么整个关系视为一组，返回的是所有教师的平均工资。
```sql
select avg(salary) as avg_salary from instructor;
```
下一个例子是查询 2018 年春季每个部门授课教师的个数。
```sql
select dept_name, count(distinct instructor.ID) as instr_count
from instructor, teaches
where
    instructor.ID = teaches.ID
    and semester = 'Spring'
    and year = 2018
group by
    dept_name;
```
当使用分组时，`select` 中没有聚集的属性必须出现在 `group by` 中。也就是说，不在 `group by` 子句中的属性必须出现在聚集函数中。下面的例子中，id 不在 `group by` 子句中但是出现在了 `select` 子句中，但是不在聚集函数中，因此是非法的 SQL。每组只会输出一个元组，但是每组教师的 id 是不同，但是一种唯一的方式选择其中一个作为输出，因此 SQL 禁止这么做。
```sql
select dept_name, ID, avg(salary)
from instructor
group by
    dept_name;
```

### The Having Clause
有的时候，我们需要在分组上过滤而不是单个元组。SQL 提供了 `having` 语句，跟在 `group by` 后面。下面的例子是查询部门教师的平均薪水高于 42,000 的部门和平均薪水。
```sql
select dept_name, avg(salary) as avg_salary
from instructor
group by
    dept_name
having
    avg(salary) > 42000;
```
和 `select` 子句要求一致，`having` 子句中不在聚集函数的属性必须出现在 `group by` 中。

带有聚集的查询执行逻辑如下：

1. 和不带聚集的查询一样，`from` 生成一个新的关系。
2. 如果存在 `where`，谓词应用于上一步生成的新关系上。
3. 如果 `group by` 存在，按照列出的属性分组，否则看成一组。
4. 如果有 `having`，应用于每一组，如果不满足那么从结果集移除。
5. `select` 生成最后结果，计算聚集函数。

下面看一个包含 `having` 和 `where` 的例子。下面查询的语义：对于 2017 年开设的每个班，如果该班至少有 2 名学生选修，那么找出所有选修该班学生的平均总学分。
```sql
select course_id, semester, year, sec_id, avg(tot_cred)
from student, takes
where
    student.ID = takes.ID
    and year = 2017
group by
    course_id,
    semester,
    year,
    sec_id
having
    count(student.ID) >= 2;
```

### Aggregation with Null and Boolean Values
`null` 的存在给聚集处理带来了额外的复杂度。假定有一列有 `null`，对其求和，那么按照之前的逻辑，加法遇到 `null` 结果是 `null`，那么求和结果是 `null`。

除了 `count(*)` 之外的聚集函数会忽略 `null`。忽略了 `null`，那么集合可能为空，`count` 返回 0，其余聚合函数返回 `null`。`null` 对更复杂的 SQL 结构影响会更微妙。

`boolean` 类型取值有 `false` `true`，SQL 1999 标准引入了 `unknown`。聚集函数 `some` `every` 类似于 `or` `and` 的语义。

## Nested Subqueries
SQL 提供了一种嵌套子查询的机制。子查询是一个嵌套在另一个查询内部的 `select-from-where` 表达式。子查询的一个常见用法是将其嵌套在 `where` 子句中，以便进行集合的测试、集合比较以及确定集合的基数。子查询页可以放到 `from` 子句。

### Set Membership
SQL 使用 `in` 表达式测试属性是否满足条件，`not in` 逻辑相反。这里的 `in` 的集合往往由 `select` 产生。

回到查询 2017 秋季和 2018 年都有开课的课程，之前使用两个查询加交集操作完成的，下面使用 `in` 表达式来完成。`intersect` 默认是去重的，因此要加上 `distinct`。
```sql
select distinct
    course_id
from section
where
    semester = 'Fall'
    and year = 2017
    and course_id in (
        select course_id
        from section
        where
            semester = 'Spring'
            and year = 2018
    );
```
这个例子也说明同一个事情可能写成不同的 SQL，这种灵活性使得用户可以使用更直接、更自然的方式思考问题。

下面使用 `not in` 来实现之前 `except` 表达的查询。
```sql
select distinct
    course_id
from section
where
    semester = 'Fall'
    and year = 2017
    and course_id not in (
        select course_id
        from section
        where
            semester = 'Spring'
            and year = 2018
    );
```
`in` `not in` 还可以用于枚举集合。比如下面查找名字不是 `'Mozart', 'Einstein'` 的教师名。
```sql
select distinct
    name
from instructor
where
    name not in ('Mozart', 'Einstein');
```
`in` 表达式可以用于多个属性。下面的例子查询教师 ID 为 10101 教的学生数。
```sql
select count(distinct ID)
from takes
where (
        course_id,
        sec_id,
        semester,
        year
    ) in (
        select course_id, sec_id, semester, year
        from teaches
        where
            teaches.ID = '10101'
    );
```

### Set Comparison
回到查询所有薪水比生物学院最低薪水高的教师的名字这个查询，之前的查询如下
```sql
select distinct
    T.name
from instructor as T, instructor as S
where
    T.salary > S.salary
    and S.dept_name = 'Biology';
```
这里比最低薪水高，意思是比任意一个生物学院的教师薪水高，SQL 提供了 `> some` 来表达这个语义，那么查询可以写作
```sql
select name
from instructor
where
    salary > some (
        select salary
        from instructor
        where
            dept_name = 'Biology'
    );
```
除此之外，SQL 中还有 `< some` `<= some` `>= some` `= some` `<> some`。`=some ` 等价于 `in`，不过 `<> some` 不等价于 `not in`。

修改上面的查询，使用 `> all`，查询结果就变成了比每个生物系教师薪水都高。
```sql
select name
from instructor
where
    salary > all (
        select salary
        from instructor
        where
            dept_name = 'Biology'
    );
```
与 `some` 类似，SQL 还有 `< all` `<= all` `>= all` `= all` `<> all`。`<> all` 等于与 `not in`，但是 `= all` 不等价于 `in`。

查询平均薪水最高的部门也可以使用子查询的方式。
```sql
select dept_name
from instructor
group by
    dept_name
having
    avg(salary) >= all (
        select avg(salary)
        from instructor
        group by
            dept_name
    );
```

### Test for Empty Relations
`exists` 可以测试一个结果集中是否有元组，如果子查询不为空，那么返回 `true`。我们可以重写查询“找到所有 2017 秋季和 2018 春季都开课的课程”。
```sql
select course_id
from section as S
where
    semester = 'Fall'
    and year = 2017
    and exists (
        select *
        from section as T
        where
            semester = 'Spring'
            and year = 2018
            and S.course_id = T.course_id
    );
```
外层查询中的相关名 `S` 在内层查询的 `where` 中也使用了。使用了外层查询中相关名的子查询，称为相关子查询（`correlated subquery`）。

一个查询包含另一个查询，相关名有范围规则。和编程语言类似，内层可以访问外层的变量名（数据库中称为相关名、相关变量），如果内层和外层变量名相同，使用内层的变量名。

`not exists` 可以测试集合为空。比如下面的查询：查询上了所有生物学院课程的学生。
```sql
select S.ID, S.name
from student as S
where
    not exists (
        (
            select course_id
            from course
            where
                dept_name = 'Biology'
        )
        except
        (
            select T.course_id
            from takes as T
            where
                S.ID = T.ID
        )
    );
```
子查询
```sql
select course_id
from course
where
    dept_name = 'Biology'
```
返回所有生物学院的课程。子查询
```sql
select T.course_id
from takes as T
where
    S.ID = T.ID
```
返回 S.ID 这个学生选修的所有课程。如果差集为空，说明该同学选修了所有生物学院的课程。

之前“查询教师 ID 为 10101 教的学生数”的例子也可以用 `exists` 表达。
```sql
select count(distinct ID)
from takes
where
    exists (
        select course_id, sec_id, semester, year
        from teaches
        where
            teaches.ID = '10101'
            and takes.course_id = teaches.course_id
            and takes.sec_id = teaches.sec_id
            and takes.semester = teaches.semester
            and takes.year = teaches.year
    );
```

### Test for the Absence of Duplicate Tuples
如果子查询没有包含重复的元组，那么 `unique` 返回 `true`。使用 `unique`，可以写出如下查询，返回 2017 年至多只开一次的课程 ID。但是 Postgresql 貌似还没有实现这个算子，报错信息是 `UNIQUE predicate is not yet implemented`。
```sql
select T.course_id
from course as T
where
    unique (
        select R.course_id
        from section as R
        where
            T.course_id = R.course_id
            and R.year = 2017
    );
```
如果一个课程没有在 2017 年开课，那么结果集是空，`unique` 返回结果也是 `true`。

不使用 `unique` 的等价写法是
```sql
select T.course_id
from course as T
where
    1 >= (
        select count(R.course_id)
        from section as R
        where
            T.course_id = R.course_id
            and R.year = 2017
    );
```
也可以使用 `not unique` 来测试存在重复元组。比如下面的查询返回 2017 年至少开课两次的课程 ID。
```sql
select T.course_id
from course as T
where
    not unique (
        select R.course_id
        from section as R
        where
            T.course_id = R.course_id
            and R.year = 2017
    );
```
`unique` 测试等价于存在关系是 $t_1=t_2$ 的两个元组 $t_1,t_2$。如果 $t_1,t_2$ 中有的属性是 `null`，$t_1=t_2$ 返回 `false`，那么即便存在多个元组的拷贝，但是属性中包含 `null`，`unique` 也返回 `true`。

### Subqueries in the From Clause
子查询可以在 `from` 子句中。`select-from-where` 返回的是一个关系，那么任意需要关系的地方，都可以出现子查询。

下面的查询改写了之前求平均薪水大于 42000 的部门的查询，这一次我们不使用 `having`。
```sql
select dept_name, avg_salary
from (
        select dept_name, avg(salary) as avg_salary
        from instructor
        group by
            dept_name
    )
where
    avg_salary > 42000;
```
我们可以使用 `as` 来重命名关系名和属性名，比如
```
select dept_name, avg_salary
from (
        select dept_name, avg(salary) as avg_salary
        from instructor
        group by
            dept_name
    )
where
    avg_salary > 42000;
```
下面的例子是求所有部门中，部门教师总薪水最高的部门。
```sql
select max(tot_salary)
from (
        select dept_name, sum(salary) as tot_salary
        from instructor
        group by
            dept_name
    ) as dept_total (dept_name, tot_salary);
```
`from` 子句中的嵌套子查询无法使用同一 `from` 子句中其他关系的相关变量。从 SQL 2003 开始，SQL 允许在子查询上添加 `lateral` 关键字，从而使得能够访问同一个 `from` 子句中前面的关系或子查询的属性。比如下面的例子，会输出教师、其薪水和所在部门的平均薪水。
```sql
select name, salary, avg_salary
from instructor I1, lateral (
        select avg(salary) as avg_salary
        from instructor I2
        where
            I2.dept_name = I1.dept_name
    );
```

### The With Clause
`with` 提供了一种定义临时关系的方式，这个定义仅对包含了这个 `with` 语句的查询可见。比如下面的查询返回预算最多的部门。
```sql
with
    max_budget (value) as (
        select max(budget)
        from department
    )
select budget
from department, max_budget
where
    department.budget = max_budget.value;
```
我们可以改写上面的查询，在 `where` 或 `from` 子句中包含子查询，这会使得查询难以理解。`with` 子句使得查询逻辑更清晰，还能让同一个临时变量在查询的不同地方使用。

比如，下面的查询返回总薪水大于各个部门总薪水的平均值的部门名。
```sql
with
    dept_total (dept_name, value) as (
        select dept_name, sum(salary)
        from instructor
        group by
            dept_name
    ),
    dept_total_avg (value) as (
        select avg(value)
        from dept_total
    )
select dept_name
from dept_total, dept_total_avg
where
    dept_total.value > dept_total_avg.value;
```

### Scalar Subqueries
如果子查询返回一个元组，且只有一个属性，那么这个子查询允许出现在使用表达式的地方，这样的子查询称为标量子查询（`scalar subquery`）。比如，比如下面的查询列出了部门名字和教师数量，在 `select` 中包含标量子查询。
```sql
select dept_name, (
        select count(*)
        from instructor
        where
            department.dept_name = instructor.dept_name
    ) as num_instructors
from department;
```
这个子查询只会返回一个值，因为使用了聚合函数 `count(*)` 且没有分组 `group by`。

标量查询可以出现在 `select` `where` `having` 这些地方。标量子查询也可以不使用聚合函数定义。编译期无法确定一个子查询是否只返回一个元组。如果运行时子查询返回多个元组，会发生运行时错误。

从技术角度讲，标量子查询返回的仍旧是一个关系，尽管只包含一个元组。当标量子查询用于期望一个值的表达式中时，SQL 隐式地抽取这个元组唯一的属性的值。

### Scalar Without a From Clause
一些查询需要的是计算而无需引用任何关系。类似的查询可能包含有 `from` 的子查询，但是顶层没有 `from` 子句。下面的例子计算平均每个教师教几门课。`count(*)` 返回整数，而整数除以整数有精度损失，因此可以乘以浮点数 1.0 也修正这个问题。
```sql
select (
        select count(*)
        from teaches
    ) * 1.0 / (
        select count(*)
        from instructor
    );
```

## Modification of the Database
### Deletion
`delete` 也是一个表达式，和查询一样。删除 SQL 如下所示
```sql
delete from r where P;
```
`delete` 首先找到关系 `r` 中满足谓词 `P(t)` 的元组 `t`，然后从 `r` 中删除这些元组 `t`。可以忽略 `where`，那么 `r` 的所有元组都会被删除。

`delete` 只能用于一个关系，如果需要删除多个关系的数据，那么要为每一个关系写一个 `delete` 命令。`where` 谓词可以相当复杂，另一个极端，也可以没有 `where`。下面的语句删除 instructor 中所有的元组。
```
delete from instructor;
```
下面再看几个例子。

删除财经系教师。
```sql
delete from instructor where dept_name = 'Finance';
```
删除薪水在 13000 和 15000 之间的教师。
```sql
delete from instructor where salary between 13000 and 15000;
```
删除在 Watson 的系部的教师。
```sql
delete from instructor
where
    dept_name in (
        select dept_name
        from department
        where
            building = 'Watson'
    );
```
虽然我们一次只能从一个关系中删除元组，但是嵌套在 `delete` 的 `where` 子句中的子查询 `select-from-where` 可以引用任意数量的关系。下面的嵌套子查询又引用了要删除元组的关系。
```sql
delete from instructor
where
    salary < (
        select avg(salary)
        from instructor
    );
```
`delete` 首先会测试检查 instructor 中薪水小于平均工资，然后所有满足谓词的元组会被删除。在删除之前执行所有元组的检查这一点很重要，因为如果一些元组在其他元组检查前被删除了，那么平均薪水会有变化，那么最终删除的元组依赖于处理的顺序。

### Insertion
可以指定一个元组或一个查询的结果插入到关系中。插入的属性值必须在属性的定义域内。同样，插入的元组属性数必须和模式定义一致。

最简单的 `insert` 是插入一个元组。比如下面的例子是插入数据库这门课程。
```sql
insert into
    course
values (
        'CS-437',
        'Database Systems',
        'Comp. Sci.',
        4
    );
```
用户可能不记得属性的顺序，SQL 允许在 `insert` 语句中指定属性。比如如下两个 `insert` 语句和前面例子等价。
```sql
insert into
    course (
        course_id,
        title,
        dept_name,
        credits
    )
values (
        'CS-437',
        'Database Systems',
        'Comp. Sci.',
        4
    );

insert into
    course (
        title,
        course_id,
        credits,
        dept_name
    )
values (
        'Database Systems',
        'CS-437',
        4,
        'Comp. Sci.'
    );
```
更常见的情况是插入一个查询的结果集。下面的查询将学分 144 以上的音乐系的学生变成教师，薪水是 18000。
```sql
insert into
    instructor
select ID, name, dept_name, 18000
from student
where
    dept_name = 'Music'
    and tot_cred > 144;
```
`select` 会求值完成之后再执行插入操作。如果不是这个顺序，下面的插入操作在没有主键的情况下会插入无限多的元组。
```sql
insert into student select * from student;
```
上面的例子都是要插入所有的属性值，下面的例子直插入部分属性，一些属性插入 `null`。
```sql
insert into student values ( '3003', 'Green', 'Finance', null );
```
大部分的关系数据库都支持批量加载，一次性从文件中导入一批数据。

### Updates
`update` 用于更新元组的多个值而不必是全部值，和 `insert` 和 `delete` 类似，可以通过查询指定需要更新哪些元组。

下面的查询给所有老师加薪 5%。
```sql
update instructor set salary = salary * 1.05;
```
下面的查询仅给薪水小于 70000 的教师加薪 5%。
```sql
update instructor set salary = salary * 1.05 where salary < 70000;
```
`update` 的 `where` 子句可以包含子查询，和 `insert` `delete` 一样，`select` 中引用的关系可能是要被更新的关系。和之前一致，SQL 首先测试哪些元组需要更新，然后再更新这些元组。比如下面的查询是给小于平均薪水的人加薪 5%。
```sql
update instructor
set
    salary = salary * 1.05
where
    salary < (
        select avg(salary)
        from instructor
    );
```
下面的查询给薪水大于 100000 的加薪 3%，其他教师加薪 5%。注意，这两个 SQL 的执行顺序很重要，否则一些薪水恰好小于 100000 的教师可能会加薪 8+%。
```sql
update instructor set salary = salary * 1.03 where salary > 100000;

update instructor set salary = salary * 1.05 where salary <= 100000;
```
SQL 提供 `case` 来解决上述问题以避免更新顺序带来的影响。
```sql
update instructor
set
    salary = case
        when salary <= 100000 then salary * 1.05
        else salary * 1.03
    end;
```
`case` 的形式如下
```
case
    when pred_1 then result_1
    when pred_2 then result_2
    ...
    when pred_n then result_n
    else result_0
end
```
如果 $pred_1,pred_2,\cdots,pred_n$ 第一个为真的谓词是 $pred_i$，那么结果是 $result_i$，如果没有谓词为真，那么结果是 $result_0$。在期望一个值的地方都可以使用 `case`。

标量子查询在更新语句中非常有用。假定一个学生在某门课的评价不是 `F` 或者 `null`，那么说明通过了这门课，应该得到相应的学分，下面的查询更新学生的总学分。
```sql
update student
set
    tot_cred = (
        select sum(credits)
        from takes, course
        where
            student.ID = takes.ID
            and takes.course_id = course.course_id
            and takes.grade <> 'F'
            and takes.grade is not null
    );
```
如果一个学生一门课都没有通过，那么总学分会被设置成 `null`，可以使用上面提到的 `case` 表达式做一个优化，对于这这种情况设置成 0 而不是 `null`。许多系统支持 `coalesce` 函数，可以替换 `case`，比如 `coalesce(sum(credits), 0)`。

## Join Expressions
### The Natural Join
下面的查询返回学生要上哪些课。
```sql
select name, course_id
from student, takes
where
    student.ID = takes.ID;
```
这里要满足条件 `student.ID = takes.ID`，这个属性也是两个关系唯一名字相同的属性。这是一种常见的情形，因此 SQL 提供称为 `natural join` 的算子。`natural join` 的返回结果也是一个关系，在这个关系中的元组满足在两个输入关系中同名列中的值相同这一条件。因此下面的查询与上述查询等价。
```sql
select name, course_id from student natural join takes;
```
下面的例子再 join 上 course 这个关系，将 course_id 换成 title 提升可读性。
```sql
select name, title
from student
    natural join takes, course
where
    takes.course_id = course.course_id;
```
但是，下面的查询并不是等价查询。因为 student 和 takes join 之后新的关系有 course_id 和 dept_name 两个属性都在 course 中存在，因此 natural join 的语义要求两个属性都要相等，那么一个学生上外系的课程就会被排除在外。
```sql
select name, title
from student
    natural join takes
    natural join course;
```
SQL 提供了一种语法能够指定那些属性参与比较元组是否应该被 join，即便两个关系有其他同名属性也不会参与等值比较。
```sql
select name, title
from (
        student
        natural join takes
    )
    join course using (course_id);
```

### Join Conditions
`on` 用于指定 join 时的条件，和之前的例子中写在 `where` 子句后面的类似，也是一个谓词。
```sql
select * from student join takes on student.ID = takes.ID;
```
上面的例子和之前 natural join 的例子输出非常类似，不过会输出两遍 id。上面的例子还等价于下面使用 `where` 的写法。
```sql
select * from student, takes where student.ID = takes.ID;
```
如果想要达到和 natural join 的效果，通过关系名限定输出一次 id 即可。
```sql
select
    student.ID as ID,
    name,
    dept_name,
    tot_cred,
    course_id,
    sec_id,
    semester,
    year,
    grade
from student
    join takes on student.ID = takes.ID;
```
`on` 可以表达任意谓词，因此比 natural join 的表达能力更强。前面展示的例子都可以通过改写去掉 `on` 而使用 `where`。不过，`on` 有两个优势，首先对于下面要分析的 outer join 而言，`on` 和 `where` 有些许不同，齐次是 SQL 更多的是给人读的，`on` 写 join 条件，`where` 写其他条件，提升可读性。

### Outer Joins
如果某学生没有选任何一个课程，那么下面的查询结果不会出现该学生的信息。
```sql
select * from student natural join takes;
```
为了让该学生的出现在结果集，需要使用 outer join，和 inner join 类似，不过它会将没有 join 上的元组也保留在结果集中，其他属性补成 NULL。outer join 有下面三种类型

1. left outer join - 保留左边关系没有 join 上的元组。
2. right outer join - 保留右边关系没有 join 上的元组。
3. full outer join - 保留两边的关系中没有 join 上的元组。

和 outer join 相对，之前讨论的 join 称为 inner join。

下面看使用 left outer join 来解决上面的问题。查询结果就会包含没有参与任何课程的学生信息，对应的选课信息填充为 NULL。
```sql
select * from student natural left outer join takes;
```
如果通过指定 course_id 为 NULL，可以查询没有选课的学生。
```sql
select *
from student
    natural left outer join takes
where
    course_id is null;
```
right outer join 和 left outer join 恰好相反。上面的例子也可以用 right outer join 实现。
```sql
select * from takes natural right outer join student;
```
full outer join 是 left 和 right 的结合。从结果上看，full join 是 left join 和 right join 的并集。

下面是 full outer join 的例子。查询的目的是计算机系的学生在 2017 年春季的选课列出来，如果某个学生没有选课，也要输出，反过来，2017 年的课程也要都在结果集中，即使没有计算机系学生选该课。
```sql
select *
from (
        select *
        from student
        where
            dept_name = 'Comp. Sci.'
    ) natural full outer
    join (
        select *
        from takes
        where
            semester = 'Spring'
            and year = 2017
    );
```
`on` 可以用于 outer join，之前的例子 `student natural left outer join takes` 差不多（投影多一列 id）等价写法是
```sql
select *
from student
    left outer join takes on student.id = takes.id;
```
对于 outer join，`on` 和 `where` 有略微不同，主要是因为 outer join 会把不匹配的元组保留并且对另一边的关系补 NULL。`on` 是 `join` 语义的一部分而 `where` 不是。上面的查询会保留没有选课学生的信息，但是下面的查询不会，因为没有匹配的元组中 takes 的 id 是 null，不等于 student 的 id。
```sql
select *
from student
    left outer join takes on true
where
    student.id = takes.id;
```

### Join Types and Conditions
SQL 默认就是 inner join，因此写 join 和 inner join 是等价的。总结下，join 有四种类型 `inner join` `left outer join` `right outer join` `full outer join`，join 条件有三种写法，`natural`，`on predicate`, `using (c1, c2, ..., cn)`。

## Views
从安全的角度看，有些用户可能可以访问教职员工的基本信息，比如姓名、系信息等，但是不能看到薪水信息，那么需要一个如下 SQL 定义的关系
```sql
select ID, name, dept_name from instructor;
```
有时，用户需要自定义一个集合，相当于一个虚拟关系。比如想知道物理系 2017 年秋季的开课列表，那么需要一个类似下面 SQL 的关系
```sql
select course.course_id, sec_id, building, room_number
from course, section
where
    course.course_id = section.course_id
    and course.dept_name = 'Physics'
    and section.semester = 'Fall'
    and section.year = 2017;
```
可以考虑把结果集存下来，但是这样的话如果底层数据发生了变化，那么存储的结果集就无效了。SQL 为了这种场景，提供了通过查询语句定义虚拟关系的能力，它不会提前计算结果并存下来，而是使用的时候实时查询。之前讨论过 `with`，将一个查询给一个别名，然后使用，其范围不能超过当前查询，但是这里的虚拟关系可以跨多个查询。这就是视图（`view`）。

### View Definition
通过 `create view` 可以定义一个视图，后面跟着视图的名字和查询。命令形式如下
```
create view v as <query expression>;
```
那么前面的两个例子就可以创建两个视图。
```sql
create view faculty as select ID, name, dept_name from instructor;

create view physics_fall_2017 as
select course.course_id, sec_id, building, room_number
from course, section
where
    course.course_id = section.course_id
    and course.dept_name = 'Physics'
    and section.semester = 'Fall'
    and section.year = 2017;
```
视图关系包含了查询结果中的元组，但是并不提前计算好存储起来。相反，数据库只存储这个视图的表达式。当使用这个视图的时候，数据库计算相应的查询来生成结果集。视图关系按需创建即可。

在创建视图的时候，可以显式的指定属性名。
```sql
create view departments_total_salary (dept_name, total_salary) as
select dept_name, sum(salary)
from instructor
group by
    dept_name;
```

### Using Views in SQL Queries
视图是一个关系，在需要关系的地方都可以使用视图。比如下面的 SQL 使用上面定义的视图。
```sql
select course_id from physics_fall_2017 where building = 'Watson';
```
既然是一种关系，那么可以基于视图来创建视图。
```sql
create view physics_fall_2017_watson as
select course_id, room_number
from physics_fall_2017
where
    building = 'Watson';
```
这个视图等价于
```sql
create view physics_fall_2017_watson as
select course_id, room_number
from (
        select course.course_id, building, room_number
        from course, section
        where
            course.course_id = section.course_id
            and course.dept_name = 'Physics'
            and section.semester = 'Fall'
            and section.year = 2017
    )
where
    building = 'Watson';
```

### Materialized Views
数据库允许存储视图的结果，当使用的关系发生了变化，视图需要保持更新。这样的视图称为物化视图（`materialized view`）。

比如物化了 `departments_total_salary` 的结果，当查询视图时无需计算。不过当 instructor 表插入或删除了元组，或者更新了某个教师的薪水，那么需要更新该视图。

保持物化视图数据最新的过程称为物化视图维护（`materialized view maintenance`），或者简称为视图维护（`view maintenance`）。当定义该视图的关系发生更新时可以立即维护视图。不过有的数据库会延迟计算，有的数据库周期性的更新视图。这种情况下，物化视图的数据过期了，如果应用必须使用最新的数据，那么就无法使用这些数据了。

如果需要高频访问一个视图，或者需要对大型关系表进行聚合计算且需要的响应速度很高，那么创建物化视图能够有很大的收益。聚合结果通常很小，存储下来成本不高，但是能够有效提升效应速度，因为无需访问大型的关系表。物化视图的收益应该比存储开销和更新开销要大。

### Update of a View
通过视图查询非常方便，但是通过视图进行更细、插入和删除操作就会遇到很多问题，因为通过视图对数据库做修改本质上是修改实际的关系。

比如向视图 faculty 插入数据，那么 salary 怎么处理？数据库拒绝插入或者保持这个字段为 NULL。
```sql
insert into faculty values ('30765', 'Green', 'Music');
```
下面看一个更复杂的例子，有如下涉及两个关系的视图
```sql
create view instructor_info as
select ID, name, building
from instructor, department
where
    instructor.dept_name = department.dept_name;
```
如果向视图插入一条数据会发生什么呢？
```sql
insert into instructor_info values ('69987', 'White', 'Taylor');
```
instructor 关系里面插入一行只有两个属性的元组，department 插入一行只有一个属性的元组，而这违反了 department 中 dept_name 不能为 NULL 的约束。

由于有上述种种问题，通常 SQl 满足如下条件才被认为是可更新的（`updatable`），比如支持插入、更新、删除。

* `from` 仅涉及一个关系
* `select` 只包含关系的属性名，没有表达式、聚集和 `distinct`
* `select` 属性可以为 NULL，即不能有 `not null` 约束，也不能是主键的一部分
* 查询不包含 `group by` 和 `having` 子句

根据上述要求，可以通过下面的视图更新 instructor 关系。
```sql
create view history_instructors as
select *
from instructor
where
    dept_name = 'History';
```
如果要插入元组 `('25566', 'Brown', 'Biology', 100000)` 会发生什么呢？这里特殊之处在于 Biology 不是 History。SQL 可能会允许插入。不过，视图可以通过可选检查 `with check option` 来增强视图，此时如果要插入的元组不满足 `where` 条件，那么数据库拒绝插入数据。更新操作也类似。

除了通过视图修改数据库，更好的方案是使用后续要讨论的触发器（`trigger`）。声明触发器的时候使用 `instead  of` 就可以用针对特定情况设计的操作来替换上述视图的插入、更新和删除操作。

## Transactions
