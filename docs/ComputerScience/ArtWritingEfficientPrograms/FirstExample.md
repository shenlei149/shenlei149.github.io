这是第一个例子，也是一个反直觉的例子。目的在于介绍一点优化程序的思想和步骤。

这个例子做的事情是生成一个很大的字符串，接着生成一些子串，这些子串的生成方式是生成一个随机数，作为字符串的开始，结束就是大字符串结束，然后对这些子串进行排序。

下面是核心代码，完成代码参考 [SubstringSort.cpp](/ArtWritingEfficientPrograms/src/SubstringSort.md)。这里使用 `std::sort` 作为排序算法，传入了 `lambda` 表达式作为自定义比较函数。使用 `std::chrono::system_clock` 即时。
```cpp
std::sort(vs.begin(),
		  vs.end(),
		  [&](const char *a, const char *b)
		  {
			  ++count;
			  return compare(a, b, L);
		  });

bool
compare(const char *s1, const char *s2, unsigned int l)
{
	if (s1 == s2)
	{
		return false;
	}

	for (unsigned int i1 = 0, i2 = 0; i1 < l; ++i1, ++i2)
	{
		if (s1[i1] != s2[i2])
		{
			return s1[i1] > s2[i2];
		}
	}

	return false;
}
```
程序输出如下
```
Sort time: 226ms (621229 comparisons)
```
在怎么优化之前，需要先问两个问题。
1. 需要优化吗？
2. 优化什么？

实践中，可能由于各种原因，比如客户可以接受当前的性能，当前程序耗时在整个业务中占比很低，人力资源问题等等，是不需要优化的，或者现在不需要优化。

在需要优化的前提下，需要知道优化什么？优化哪里？这就需要性能分析。通过一些手段，找出瓶颈点。

这里我们使用 Google profiler 进行性能分析。只需要在编译的时候加上 `-lprofiler` 参数，运行可执行文件前面加上环境变量 `LD_PRELOAD=/path/to/libprofiler.so CPUPROFILE=/path/to/perf/data` 指定 profile 文件的路径，就能够采集 CPU 相关数据了，最后使用 `google-pprof --text /path/to/binary /path/to/perf/data` 命令输出结果。
```
Using local file ./SubStringSort.
Using local file prof.out.
Total: 19 samples
      18  94.7%  94.7%       18  94.7% compare (inline)
       1   5.3% 100.0%        1   5.3% std::__introsort_loop (inline)
       0   0.0% 100.0%        7  36.8% __final_insertion_sort (inline)
       0   0.0% 100.0%       12  63.2% __introsort_loop
       0   0.0% 100.0%       19 100.0% __libc_init_first@@GLIBC_2.2.5
       0   0.0% 100.0%       19 100.0% __libc_start_main@GLIBC_2.2.5
       0   0.0% 100.0%        7  36.8% __sort (inline)
       0   0.0% 100.0%        7  36.8% __unguarded_insertion_sort (inline)
       0   0.0% 100.0%        7  36.8% __unguarded_linear_insert (inline)
       0   0.0% 100.0%       12  63.2% __unguarded_partition (inline)
       0   0.0% 100.0%       12  63.2% __unguarded_partition_pivot (inline)
       0   0.0% 100.0%       19 100.0% _start
       0   0.0% 100.0%       19 100.0% main
       0   0.0% 100.0%       12  63.2% main (inline)
       0   0.0% 100.0%       18  94.7% operator (inline)
       0   0.0% 100.0%       19 100.0% sort (inline)
       0   0.0% 100.0%       12  63.2% std::__introsort_loop
```

从分析结果可以看出，绝大部分时间都是 `compare` 消耗的，所以优化点就是这个函数。结果中显示 `Total: 19 samples`，19 个采样点有点少，不过对于这个程序而言，不影响结论。

想要优化一个函数，基本思路就两条：运行的更快，或者调用次数更少（这一点往往容易被忽视）。

这里，`compare` 函数的调用次数有 `std::sort` 的算法决定，而我们又不会修改这个库函数，也没打算换其他的库函数，所以只能想办法让函数运行的更快。

仔细看 `compare` 函数，`for` 循环中的 `i1 < l` 是不必要的。因为能够进入 `for` 循环，说明两个字符串起始位置不同，否则在开始的 `if` 分支那里就会返回。起始位置不同，结束位置相同，说明长度不一样，那么一定有一个字符串先到达尾部，0 和长的字符串中的某个字符比较，一定不相等，就会返回。所以无需做这个判断，可以让函数少做一些事情。

“优化” `compare` 程序，还可以调整接口。
```cpp
bool
compare(const char *s1, const char *s2)
{
	if (s1 == s2)
	{
		return false;
	}

	for (unsigned int i1 = 0, i2 = 0;; ++i1, ++i2)
	{
		if (s1[i1] != s2[i2])
		{
			return s1[i1] > s2[i2];
		}
	}

	return false;
}
```
运行结果如下，确实快了不少。书中运行结果是变慢了很多，约两倍。
```
Sort time: 165ms (621229 comparisons)
```
如果将 `unsigned int` 修改成 `int`，又快了不少。
```
Sort time: 134ms (621229 comparisons)
```
这两组实验的运行结果和书上不一样。这里的结果更符合直觉，也符合我对 CPU 的认知。TODO 增加书中第三章 CPU 的描述，看书中如何解释反直觉的问题，又如何解释这里的运行结果。
