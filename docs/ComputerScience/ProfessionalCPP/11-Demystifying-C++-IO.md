## 流
流是一个抽象概念，分为缓冲的和非缓冲的两种模式，可以与控制台、文件、字符串、固定大小的缓冲区绑定，进行输入或输出操作。输入流要关联一个源，输出流要关联一个目的地。流有位置概念，表示下一个要读取或者写入的位置。

`<<` 运算符是最常用的输出方式，除此之外，`put()` `write()` 分别输出一个字符和一个数组。

大部分的流是缓冲式的，`flush()` 的作用是立即写到输出流中。当遇到 `std::endl`、流要被销毁了、流缓冲区满了、相应的输入流有读请求，那么也会进行 `flush()`。

`good()` `bad()` `fail()` 可以用于判断流当前的状态。注意 `good() == (!fail() && !eof())`。也可以设置相应的异常 `cout.exceptions(ios::failbit | ios::badbit | ios::eofbit);`。`clear()` 可以清除标记。`cout` 自身也可以用于判断状态是否正常。输入判断状态的方式类似。

有一些控制输出的格式的方式。`boolalpha` `noboolalpha` 设置输出 `bool` 是数字还是 `true` `false`，默认是 `noboolalpha`。`hex` `oct` `dec` 设置输出数字的进制。`fixed` `scientific` `defaultfloat` 设置输出浮点数的格式。`setprecision` 设置浮点数的精度。`setw` 设置输出的宽度。`setfill` 设置填充字符。`showpoint` `noshowpoint` 设置无小数部分的数要不要输出小数点。`put_money` `put_time` 按照 locale 设置输出金额和时间格式。`quoted` 
处理字符串中的特殊字符。这些都可以用 `std::fmt` 搞定。

最常见的输出方式是使用 `>>`，以空白字符做分割。如果不想遇到空白字符终止，可以使用 `get()`，返回当前字符并移动到下一个为止。`unget()` 与 `get()` 相应，放回一个字符，可以通过 `fail()` 检查是否成功。`putback()` 与 `unget()` 类似，不过可以放回去一个指定字符。`peek()` 仅读取字符但是不移动位置。由于读取一行数据是一个常用需求，因此 `cin` 有一个成员函数是 `getline()`，传入一个数组，读取一行数据，直到数组长度减一的数据，还有一个非成员函数版本，`getline(cin, std::string)`，无需管理 buffer，这两个版本还可以自定义分隔符，默认是 `\n` 换行符。

与输出类似，有一些控制输入的方式，比如 `quoted` `get_money` `get_time` `boolalpha` `noboolalpha` `dec` `hex` `oct` 这些与输出含义一致。`skipws` `noskipws` 是否要跳过空白，默认是 `skipws`。`ws` 表示要跳过当前空白。

特化 `std::formatter` 可以自定义输出用于 `std::print()` `std::println()`。
```cpp
template <>
class std::formatter<YourClass>
```
还可以重载 `<<` `>>` 来自定义流的输入输出。

## 字符串流
`<sstream>` 中提供了 `std::istringstream` `std::ostringstream` 用于从 `std::string` 读取、写数据到 `std::string`，`std::ostringstream` 成员函数 `str()` 得到 `std::string`。

这两个继承自 `istream` `ostream`，因此用法类似。

使用字符串流而不是字符串有几个优势：有当前位置信息，可以知道下一个要读写的地方；支持更强大的控制和本地化功能；性能更好。

## 基于 span 的流
C++23 引入了基于 span 的流，定义在 `<spanstream>`，`ispanstream` `ospanstream` `spanstream`，可以使用流的统一接口来读写一段固定大小的内存。

## 文件流
`<fstream>` 中定义了 `std::ofstream` `std::ifstream`。当处理文件的时候，检查和处理错误非常重要。当打开文件的时候，需要指定文件名和打开模式，模式如下表所示。

| CONSTANT | DESCRIPTION |
|--|--|
| `ios_base::app` | Open, and go to the end before each write operation. |
| `ios_base::ate` | Open, and go to the end once immediately after opening. |
| `ios_base::binary` | Perform input and output in binary mode as opposed to text mode. |
| `ios_base::in` | Open for input, start reading at the beginning. |
| `ios_base::out` | Open for output, start writing at the beginning, overwriting existing data. |
| `ios_base::trunc` | Option for `out`. Delete all existing data (truncate). |
| `ios_base::noreplace` | Option for `out`. Open in exclusive mode. Open will fail if the file already exists. |

`ifstream` 默认模式是 `ios_base::in`，`ofstream` 默认模式是 `ios_base::out`。

这些模式可以组合使用，比如以二进制模式打开文件并截断已有内容
```cpp
ios_base::out | ios_base::binary | ios_base::trunc
```

`std::ofstream` `std::ifstream` 对象析构时会自动关闭底层文件，无需显式调用 `close()`。

`seekp()` `seekg()` 分别对应输出和输入流，可以跳转到任意位置。提供两个函数的原因是可能流既要输出又要输入。这俩函数有两个版本，第一个是跳转到绝对偏移，第二个额外提供一个位置，相对这个位置跳转多少偏移。位置类型是 `std::streampos`，偏移类型是 `std::streamoff`。整数类型可以隐式转到这两个类型。预定了三个很有用的位置 `ios_base::beg` `ios_base::end` `ios_base::cur`。

`tellp()` `tellg()` 分别对应输出和输入流，报告当前位置。

可以通过 `input.tie(&output)` 将输入流和输出流绑定起来，这样一旦需要访问输入流，那么输出流就必须 `flush`。也可以将两个文件输出连起来，这样做文件同步。`cin` 和 `cout` 也可以连起来。`cerr` 和 `cout` 连起来，这样有错误信息，`cout` 会 `flush`。解除绑定的方式是传入 `nullptr`。

## 双向流
即可以读也可以写，比如 `fstream` `stringstream`，支持 `<<` 和 `>>` 两种操作，这也是 `seek` 和 `tell` 有两个版本 `p` `g` 的原因。

## 文件系统类
C++ 标准库包含文件系统相关的类库，定义在头文件 `<filesystem>`，`std::filesystem` 命名空间。

最基本的组件是 `path`，一个绝对路径或者相对路径，可以包含文件名，也可以不包含。`string()` 函数根据平台返回表示路径的字符串。`append()` `/=` 加一级路径，`concat()` `+=` 仅仅是字符串操作，前者会插入路径分隔符而后者只是字符串操作。除此之外，还有 `remove_filename()` `replace_filename()` `replace_extension()` `root_name()` `parent_path()` `extension()` `stem()` `filename()` `has_extension()` `is_absolute()` `is_relative()` 等函数。

用 `path` 可以构造一个 `directory_entry`，支持 `exists()` `is_directory()` `is_regular_file()` `file_size()` `last_write_time()` 等操作。

类库还提供了很多帮助方法。比如 `copy()` 可以拷贝文件或者目录，`create_directory()` 新建一个目录，`exists()` 查询文件或目录是否存在，`file_size()` 返回文件大小，`last_write_time()` 获取最后更新时间，`remove()` 删除文件，`temp_directory_path()` 得到一个存放临时文件的目录，`space()` 查询文件系统的可用空间。

使用 `path` 可以构造一个 `recursive_directory_iterator`，然后从 `path` 开始遍历，`++` 移动到下一个对象，`*` 用于访问。结束的迭代器是默认构造一个 `recursive_directory_iterator`。`path` 也能构造一个 `directory_iterator`，遍历当前目录，但是不递归，我们用后者自己实现前者。
