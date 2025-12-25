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

### Subqueries in the From Clause

### The With Clause

### Scalar Subqueries

### Scalar Without a From Clause
