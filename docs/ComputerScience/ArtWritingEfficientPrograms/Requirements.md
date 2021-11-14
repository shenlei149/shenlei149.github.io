为了进行书中的实验，需要做一些前期准备工作。

### 实验环境
硬件：
```
<!-- wsl2 -->
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         39 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  12
  On-line CPU(s) list:   0-11
Vendor ID:               GenuineIntel
  Model name:            Intel(R) Core(TM) i5-10500 CPU @ 3.10GHz
```

操作系统：
* OS: debian 12.5

相关软件：
* g++: 12.2
* perf: 6.1.76
* google-perftools: 2.10
* google benchmark: commit 06b4a070156a9333549468e67923a3a16c8f541b

google benchmark 是在 [github](https://github.com/google/benchmark) 下载安装，其他软件均为 apt 自带。

随着操作系统的更新和相关软件的更新，可能后面的实验所用的系统或软件版本会高。

### Google benchmark
https://github.com/google/benchmark

安装：https://github.com/google/benchmark#installation

### g++
整个实验中常用以下几个参数：
* `-g` - 添加调试信息，方便 gdb 调试
* `-O` - 优化级别，实验中基本使用 `-O3`，最高级别的优化
* `-march` - 指定计算机架构，实验中基本使用 `-march=native`，编译器会使用当前主机的架构
* `-Wall` - 打开所有警告，帮助我们提升代码质量
* `-pedantic` - 严格限制仅使用 ISO C++ 标准
* `-o` - 指定输出的名字

### perf profiler
大部分 Linux 平台自带的分析工具。最简单的使用方式是获取一个程序运行时的统计信息。
```sh
sudo perf stat ./SubStringSort
Sort time: 132ms (621229 comparisons)

 Performance counter stats for './SubStringSort':

            134.01 msec task-clock                       #    0.996 CPUs utilized          
                 0      context-switches                 #    0.000 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
               254      page-faults                      #    1.895 K/sec                  
       555,694,227      cycles                           #    4.147 GHz                    
     2,528,055,088      instructions                     #    4.55  insn per cycle         
       505,616,545      branches                         #    3.773 G/sec                  
           822,257      branch-misses                    #    0.16% of all branches        

       0.134495601 seconds time elapsed

       0.134588000 seconds user
       0.000000000 seconds sys
```
从中可以看到耗时、上下文切换次数等信息。那么我们能看到哪些统计信息呢？可以通过命令 `perf list` 查看，当运行 `perf stat` 时，可以使用参数 `-e` 的方式自定义信息。

### google-perftools
https://github.com/gperftools/gperftools

CPU 性能分析方法：
```
1) Link your executable with -lprofiler
2) Run your executable with the CPUPROFILE environment var set:
     $ CPUPROFILE=/tmp/prof.out <path/to/binary> [binary args]
3) Run pprof to analyze the CPU usage
     $ pprof <path/to/binary> /tmp/prof.out      # -pg-like text output
     $ pprof --gv <path/to/binary> /tmp/prof.out # really cool graphical output
```

根据更详细的文档，执行的时候可能需要添加环境变量 `LD_PRELOAD=/path/to/libprofiler.so` 在当前实验环境，具体变量是 `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libprofiler.so`。

debian 系统安装完后并没有 `pprof` 的软链接，需要使用原始名字 `google-pprof`。一般情况下输出文本，所以具体命令是 `google-pprof --text /path/to/binary /path/to/perf/data`。
