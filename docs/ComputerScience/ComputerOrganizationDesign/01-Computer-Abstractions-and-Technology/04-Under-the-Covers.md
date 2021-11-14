计算机由输入、输出、内存、控制器和数据通路五个部分组成，后面两者统称为处理器。书中的插图不错，形象的描绘了这几个部分的关系，包括之前小节提到的软件。

![](0401.png)

输入设备（`input device`）和输出设备（`output device`）会在第五章和第六章详细说明。这里先简要看下全局。

### Through the Looking Glass
现在个人移动设备或者 PC 配的显示器往往使用 LCD （`liquid crystal display`）技术，薄，功耗低。

最小显式单元是像素（`pixel`）。

### Touchscreen
个人移动设备使用触摸屏作为输入，替代了 PC 的键盘和鼠标。

### Opening the Box
拆开一个智能手机，也包含计算机的五个部分。

I/O 包含触摸屏、前/后置摄像头、麦克风、扬声器、加速计、陀螺仪等等。

其中还有一块集成电路（`integrated circuit`），也称芯片（`chip`），其中包含若干个处理器，大家称之为 CPU（`central processor unit`）。

处理器中就包含数据通路（`datapath`），执行算数操作，和控制器（`control`），告诉其他组件执行什么命令。

智能手机中还有内存（`memory`），是 DRAM（`dynamic random-access 
memory`）芯片，用于存储指令和数据。RAM 部分说明访问任意一个部分的时间是一样的。

再深入看，处理器中还包含缓存（`cache memory`），使用 SRAM（`static random-access memory`）技术，用于缓存 DRAM 的数据。SRAM 密度更低，更快，更贵。SRAM 与 DRAM 组成了两层的存储层次结构（`memory hierarchy`）。

这又是一个抽象的例子，定义了硬件和底层软件之间的交互接口。这就是指令集架构（`instruction set architecture`），或者简称架构（`architecture`）。操作系统会对底层做一些封装，我们一般将基本的指令集和操作系统提供的接口统称为 ABI（`application binary interface`）。

架构本身与实现（`implementation`）无关，具体的实现遵循抽象的架构即可。

### A Safe Place for Data
存储分为易失性（`volatile`）和非易失性（`nonvolatile memory`）两种存储，DRAM 是易失的，DVD 是非易失的。前者往往称为主存（`main memory`, `primary memory`），后者称为辅助存储器（`secondary memory`），处于存储层次结构的下层。常见的辅助存储器有磁盘（`magnetic disks`）和闪存（`flash memory`）。
