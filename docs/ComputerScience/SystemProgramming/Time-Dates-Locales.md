这一章讨论时间、日期和本地化。

## 时间和日期
Unix 中有两个类型的时间：进程时间（`process time`）和真实时间（`real time`）。进程时间指的是进程在 CPU 上消耗的时间，真实时间指的是从某个时间点到现在的时间间隔。日历时间（`calendar time`）是指从 1970 年 1 月 1 日 00:00:00 UTC 到现在的时间间隔。这里的 1970 年 1 月 1 日 00:00:00 UTC 被称为 Unix 纪元（`Unix epoch`），每个特殊的时间点都被称为纪元（`epoch`）。UTC 是 `Coordinated Universal Time` 的缩写，中文称为协调世界时，是一种时间标准，这个缩写本身没错，字母顺序是标准委员会妥协的产物。

还有一种被称为分解时间（`broken-down time`）的时间表示形式，它是将日历时间分解为年、月、日、时、分、秒等组成部分的表示形式。这些信息被存在 `struct tm` 结构体中，这个结构体用于各种转换函数。`struct tm` 结构体的定义如下：
```c
struct tm {
    int tm_sec;     /* Seconds (0-60)                       */
    int tm_min;     /* Minutes (0-59)                       */
    int tm_hour;    /* Hours (0-23)                         */
    int tm_mday;    /* Day of the month (1-31)              */
    int tm_mon;     /* Month (0-11)                         */
    int tm_year;    /* Year - 1900                          */
    int tm_wday;    /* Day of the week (0-6, Sunday = 0)    */
    int tm_yday;    /* Day in the year (0-365, 1 Jan = 0)   */
    int tm_isdst;   /* Daylight saving time                 */
};
```
字段都是不言自明的。不过需要强调两个点，`tm_sec` 一般是 0-59，但是在闰秒（`leap second`）的时候可以是 60。`tm_isdst` 字段是一个布尔值，表示是否处于夏令时（`daylight saving time`）状态，正数表示是，零表示否，负数表示无法获取这个信息。

假定现在要写一个简单的类似 `date` 命令的程序，打印当前的时间和日期，需要了解哪些 API 呢？

首先需要获取当前的时间，有两个函数可以做到这一点。
```c
#include <time.h>
time_t time(time_t *tloc);

struct timespec {
    time_t tv_sec;  /* Seconds        */
    long tv_nsec;   /* Nanoseconds    */
};
int clock_gettime(clockid_t clockid, struct timespec *tp);
```
其中 `time_t` 是一个整数类型，至少是 `int`，一般情况是 `long`，表示从纪元到现在的秒数。`clock_gettime` 函数可以获取更高精度的时间，返回值是一个 `struct timespec` 结构体，包含秒和纳秒两个字段。`clockid_t` 是一个枚举类型，表示要获取哪种类型的时间，下面代码（部分枚举值）来自 `time.h` 头文件：
```c
/* Identifier for system-wide realtime clock.  */
# define CLOCK_REALTIME			0
/* Monotonic system-wide clock.  */
# define CLOCK_MONOTONIC		1
/* High-resolution timer from the CPU.  */
# define CLOCK_PROCESS_CPUTIME_ID	2
/* Thread-specific CPU-time clock.  */
# define CLOCK_THREAD_CPUTIME_ID	3
```
在当前的例子中，使用 `time()` 函数就可以了。另一个理由是下面和时间相关的转换函数都使用 `time_t` 类型作为参数。

下一步就是要把它转成一个字符串，有两个函数可以做到这一点。
```c
#include <time.h>

char *ctime(const time_t *timep);
char *asctime(const struct tm *tm);
```
第二个函数接受的是分解时间 `struct tm`，将 `time_t` 转换为 `struct tm` 的函数是：
```c
#include <time.h>

struct tm *localtime(const time_t *timep);
struct tm *gmtime(const time_t *timep);
```
两者的区别在于 `localtime()` 会根据当前的时区设置来转换，而 `gmtime()` 则是转换为 UTC 时间，后者 `gm` 就是格林威治标准时间（`Greenwich Mean Time`）的缩写。

不过 `ctime()` `asctime()` 这两个函数已经废弃了，现代的做法是使用 `strftime()` 函数来格式化时间字符串：
```c
#include <time.h>

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
```
下面是打印当前时间的示例代码：
```c
#include <stdio.h>
#include <string.h>
#include <time.h>

const int MAXLEN = 100;

int main()
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	if (local_time == NULL)
	{
		printf("Failed to get local time.\n");
		return 1;
	}

	char formatted_date[MAXLEN];
	char format_str[MAXLEN];
	strcpy(format_str, "%c");
	size_t bytes_written = strftime(formatted_date, MAXLEN, format_str, local_time);
	if (bytes_written == 0)
	{
		printf("Failed to format date.\n");
		return 1;
	}

	printf("Current date and time: %s\n", formatted_date);

	return 0;
}
```
`time` 返回值也是当前时间，所以可以传入 `NULL`。`localtime` 返回的内存是一块共享的静态缓冲区，因此不能重入，多次调用会覆盖之前的。`strftime()` 函数的第三个参数是格式化字符串，`%c` 表示使用本地化的、标准的、人类可读的日期和时间表示形式。

## 地域设置
地域设置（`locale`）是一组类别（`category`）的集合。每个类别用于控制用户文化与语言设置相关层面的各个组成部分。比如 `LC_CTYPE` 制定了字符分类、大小写转换以及其他字符信息，比如哪些字符是字母、数字，哪些是标点等等。

这些类别的标识都是以 `LC_` 开头，是 `locale category` 的缩写。这些名称在 `<locale.h>` 头文件中定义供程序使用，同时也被放到了环境中，也就是说它们还是环境变量。POSIX.1-2024 规定了 6 个类别，GNU C 库从 `glibc 2.2` 开始又增加了 6 个。如下表所示。

| Category | Availability | Meaning |
|--|--|--|
| LC_COLLATE | POSIX.1-2024 | Collation order (how characters are sorted) |
| LC_CTYPE | POSIX.1-2024 | Character classification and case conversion, such as which characters are in the character set and what their classes are |
| LC_MESSAGES | POSIX.1-2024 | Formats of informative and diagnostic messages and interactive responses |
| LC_MONETARY | POSIX.1-2024 | Monetary formatting, such as currency symbols and conventions |
| LC_NUMERIC | POSIX.1-2024 | Numeric, nonmonetary formatting |
| LC_TIME | POSIX.1-2024 | Date and time formats |
| LC_ADDRESS | GLIBC-2.2 | Formats of locations and geography-related items, such as names of places |
| LC_IDENTIFICATION | GLIBC-2.2 | Metadata for the locale |
| LC_MEASUREMENT | GLIBC-2.2 | Measurement systems (for example, metric vs. US customary units) |
| LC_NAME | GLIBC-2.2 | Words used to address people (for example, "Frau," "Mme") |
| LC_PAPER | GLIBC-2.2 | Dimensions of standard paper sizes (for example, US letter vs. A4) |
| LC_TELEPHONE | GLIBC-2.2 | Formats used with telephone services |

下面是除了这些以外的一些环境变量也控制本地化的一些行为。`TZ` 是一个特例，稍后解释。

| Variable | Availability | Meaning |
|--|--|--|
| LC_ALL | POSIX.1-2024 | Represents the set of all locale categories and has special meaning and precedence |
| LANG | POSIX.1-2024 | Determines the locale category for native language, local customs, and coded character set in the absence of the LC_ALL and other LC_* variables |
| LANGUAGE | GLIBC-2.2 | Used by the glibc function gettext in language translation |
| TZ | POSIX.1-2024 | Time zone information |
| NLSPATH | POSIX.1-2024 | A path variable (same format as PATH) used for finding message catalogs for translation to other languages |
| LOCPATH | POSIX.1-2024 | A path variable for finding locale data files |

`TZ` 存储了当前用户的时区信息，可以和系统时区不同。比如一个大型机，从世界各地都有用户登录，服务器运行在所在的时区，不同的用户根据自己的时区设置 `TZ` 环境变量，然后看到的就是本地时间而不是服务器时间。它的值由 RFC 8536 定义。这些数据都放在 `/usr/share/zoneinfo` 目录下，`TZ` 的值就是这个目录下的相对路径。比如 `TZ=Asia/Shanghai`，表示使用上海时间。

幸运的是我们无需太关注这个信息，C 库都已经考虑时区的事情了。除了 `gmtime()` 函数这样明确忽略了时区的函数之外，全部能正确运行，不要我们做什么特殊的处理。

使用 `locale` 命令可以查看当前的本地化设置，`locale -a` 可以列出所有可用的本地化设置。通过名字可以猜个大概，不过可以通过 `-v` 参数查看详细信息。

设置的名字的形式是 `language[_territory][.codeset][@modifier]`，其中 `language` 是语言代码，由 ISO 639 定义，`territory` 是国家或地区代码，由 ISO 3166 定义，`codeset` 是字符集编码，比如 UTF-8，`modifier` 是一个可选的修饰符。

字符集编码（`codeset`）是从图形字符到数值的映射。这些数值被称为码点（`code point`），字符集编码有时也被称为字符映射表（`character map`）或字符集（`character set`）。ASCII 就是早期的一种字符集，将键盘上字符和一些不可打印的字符映射为 7 位的无符号整数。UTF-8 是变长字符集，每个码点从 1 到 4 个字节不等，表示所有的 Unicode 字符。Unicode 包含了几乎已知的古代和现代语言文字和数字、符号的表示，包括中文、希腊语、加拿大原住民语言、阿拉伯语等等。

使用下面的函数可以修改、获取当前进程的本地化设置：
```c
#include <locale.h>
char *setlocale(int category, const char *locale);
```
这个函数只会影响当前进程的本地化设置或从这个进程派生出来的子进程，不会影响其他进程或者是系统的设置。`category` 参数是上面提到的类别，`locale` 参数是本地化设置的名字。如果第二个参数是 `NULL`，则返回当前的本地化设置。如果第二个参数是一个可用的本地化的名字，则设置当前进程的本地化设置为这个名字，否则返回 `NULL`。如果第二个参数是空字符串 `""`，则设置当前进程的本地化设置为环境变量中指定的本地化设置。查找环境变量的顺序是首先看 `LC_ALL` 是不是非空，如果非空就用这个值，否则是否存在同名的 `LC_*` 环境变量，如果存在就用这个值，否则就看 `LANG` 环境变量是否非空。

对于之前的小程序，需要调用 `setlocale(LC_TIME, "")` 来设置时间的本地化。

当我们设置了本地化之后，很多函数就会根据 `LC_*` 类别的设置表现出不同的行为。下面这些函数的行为会受到本地化设置的影响：
```c
fprintf()
islower()
fscanf()
isprint()
isalnum()
ispunct()
isalpha()
isspace()
isblank()
isupper()
iswcntrl()
iswupper()
strcoll()
iswctype()
iswxdigit()
strerror()
iswdigit()
isxdigit()
iswgraph()
mblen()
iswlower()
mbstowcs()
strfmon()
strftime()
toupper()
towlower()
towupper()
wcscoll()
strsignal()
wcstod()
iscntrl()
iswalnum()
iswprint()
mbtowc()
isdigit()
iswalpha()
iswpunct()
perror()
strtod()
strxfrm()
wcstombs()
wcsxfrm()
```
举一个具体的例子，`strcoll` 和 `strcmp` 都是比较两个字符换的函数，但是 `strcoll` 会根据当前的本地化设置来比较两个字符串，而 `strcmp` 则是直接比较两个字符串的字节值，因此对于同样的输入，两个函数可能会给出不同的结果。

有的时候没有类库可以依赖本地化设置来解决我们要处理的问题，此时需要自己访问本地化数据。有如下两个函数可以获取本地化数据：
```c
#include <locale.h>
struct lconv *localeconv(void);

char *nl_langinfo(int);
```
`localeconv()` 函数返回一个指向 `struct lconv` 结构体的指针，这个结构体很大，这个函数也很慢，并且不通用，只能获取 `LC_MONETARY` 和 `LC_NUMERIC` 的信息。而 `nl_langinfo()` 函数可以获取指定的本地化信息，仅仅获取需要的信息，同时还提供了 `LC_TIME` 类别的支持和部分 `LC_MESSAGES` 的支持。下面这个例子使用 `nl_langinfo()` 获取日期相关的信息，打印星期几。
```c
setlocale(LC_TIME, "");

for (int dayofweek = DAY_1; dayofweek <= DAY_7; dayofweek++)
{
	char *dayname = nl_langinfo(dayofweek);
	printf("%s\n", dayname);
}
```
