很多程序都有闪烁的光标，定期刷新内容，很多系统监控软件都是如此，比如 `top`、`iostat` 等。这就需要程序能够追踪时间，并在预定的时刻执行特定的任务。这一章学习和讨论时间、时间度量、定时器等相关内容。

## 追踪时间的需求
有很多场景是固定时间间隔然后执行特定的任务，因此必须有办法追踪时间的流逝。定时器（`timer`）能够告诉我们时间过去了多久。当我们设置的时间量过去时，我们称之为超时（`timeout`）。我们给定时器指定一个时间量，当超时时定时器给我们一个信号，然后我们可以执行特定的任务。

定时器和闹钟不同，闹钟的参数是指定一个时间，到这个时间响铃，而定时器是设定一个时间量，在这个时间量过去后，定时器给我们一个信号。

## 硬件时钟和软件时钟
大多数计算机都有专门的硬件时钟，称为实时时钟（`real-time clock`, `RTC`），它能够在计算机断电后仍然保持时间，它保存的是挂钟时间（`wall-clock time`）。

Linux 系统用 `RTC` 来初始化各种结构体，为 `time()` `gettimeofday()` 等函数存储时间和日期。不过 `RTC` 也有其他功能。第一个功能是在专用中断线上以 2Hz 到 8,192 Hz 之间选定的频率产生周期性的中断（`interrupt`）。第二个功能是为每个时钟节拍（`clock tick`）产生中断，通常是每秒一次。最后，它在到了预先指定的节拍数时产生中断，类似闹铃的工作原理。

许多计算机还有可编程间隔定时器（`programmable interval timer`, `PIT`），它每次超时都会产生一个中断，称为定时器中断（`timer interrupt`）。`PIT` 本质是一个硬件定时器，Linux 内核通常把它编程为每毫秒中断一次，即 1,000 HZ 的频率。相邻中断之间称为一个节拍（`tick`）。内核常量 `HZ` 是产生节拍的频率，`jiffy` 是一个节拍的时间长度。

第三种计时设备是时间戳计数器（`time stamp counter`, `TSC`），Linux 系统有时使用这个硬件计数器做更高精度的计时，该设备中的振荡器频率比 `PIT` 高得多，高达 1 GHz。

最后，许多现代计算机有高精度事件计时器（`high precision event timer`, `HPET`），至少每 10 微秒更新一次，即至少 100 KHz 的频率。

系统时钟本质上是一个软件时钟，意味着时间完全由软件记录和更新。当计算机重启时，内核就初始化系统时钟，通常是通过读取 `RTC` 来获得当前的挂钟时间，或者从网络时间服务器同步时间。在初始化之前，系统时钟上的时间记录的是机器重启以来流逝的时间。一旦完成初始化，系统时钟就存储了当前的挂钟时间，即自纪元（`epoch`）以来的时间。

系统时钟每次收到来自 `PIT` 的中断就更新。系统时钟在计算机启动时从硬件时钟初始化，此后通过记录来自 `PIT` 的节拍来计时。如果 `PIT` 以 100 Hz 的速率产生节拍，每个 `jiffy` 就是 0.01 秒。`jiffy` 没有单一的值，取决于机器。

## 高精度 `sleep` 函数
`sleep()` 函数的精度是 1 秒，对于很多应用而言这个粒度太大了。`usleep()` 函数可以以微秒为单位进行休眠，这里 `u` 是希腊字母 `μ`，表示微秒（`microsecond`），不过这个系统调用不能保证间隔恰好是微秒，因为底层定时器不够精细，或者是有其他系统活动影响了其精度。一般情况下，这两个函数是使用软件定时器实现的。它们共享同一个软件定时器，那么多次调用 `usleep()`、`sleep()` 可能会互相影响，导致意外结果。其次，它们还与 `alarm()` 共享一个定时器，影响类似。此外，其他信号的产生和送达也可能会对它们造成干扰。

因此很多时候推荐使用 `nanosleep()` 函数，正如名字所示，它精确到纳秒（`nanosecond`）。下面是函数原型。
```c
#include <time.h>

int nanosleep(const struct timespec *req, struct timespec *rem);
```
结构体 `timespec` 用于表示时间间隔，定义如下，其中 `tv_nsec` 的值必须在 0 到 999,999,999 之间，从而不超过 1 秒。
```c
struct timespec {
	time_t tv_sec;  /* Seconds */
	long   tv_nsec;  /* Nanoseconds */
};
```
`nanosleep()` 函数的第一个参数 `req` 是必须的，指定了期望的休眠时间间隔。第二个参数 `rem` 是可选的，如果不为 `NULL`，当 `nanosleep()` 被信号中断时，它会存储剩余的时间间隔。

下面是一个简单的示例，捕获了 `Ctrl+C` 信号但是什么也不做，目的仅仅是打断 `nanosleep()` 的休眠，然后输出剩余的时间。同时，捕获信号能够保证程序不被终止。
```c
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sigint_handler(int signum)
{
	return; /* Just catch the signal and return to main(). */
}

void fatal_error(int errnum, const char *msg)
{
	fprintf(stderr, "%s: %d\n", msg, errnum);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int delay = 5; /* Delay in seconds */

	struct timespec init_sleep ={delay, 0};

	struct sigaction act;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
	{
		fatal_error(errno, "sigaction failed");
	}

	printf("about to sleep for %d seconds\n", delay);
	struct timespec remainder;
	if (nanosleep(&init_sleep, &remainder) == -1 && (errno == EINTR))
	{
		printf("Sleep interrupted, %ld seconds and %ld nanoseconds remaining\n",
			   remainder.tv_sec,
			   remainder.tv_nsec);
	}

	return 0;
}
```
有了剩余时间，我们可以再次调用 `nanosleep()` 来完成剩余的休眠时间，进而保证程序能够按照预期的时间间隔休眠。如果再次被打断呢？因此这里需要使用循环。
```c
do
{
	retval = nanosleep(&init_sleep, &remainder);
	if (retval == -1)
	{						/* An error or an interruption occured. */
		if (errno == EINTR) /* Received SIGINT */
		{
			init_sleep = remainder;
		}
		else /* Some other non-recoverable error occurred. */
		{
			fatal_error(errno, "nanosleep");
		}
	}
} while (retval < 0);
```
下一个问题是进程睡眠的时间是否等于最初的期望时间。手册上说睡眠时间至少是请求的时间，但可能会更长。如果间隔不是底层时钟粒度的整数倍，会向上取整。另一个可能增加的原因是内核调度有一点点延迟。在循环中，这些微小的延迟会累积，这就是人们常说的定时器漂移（`time drift`）。

我们可以利用 `clock_gettime()` 函数获取当前时间，从而计算出实际的睡眠时间，看看是否与期望的时间间隔一致。这个函数的原型如下
```c
#include <time.h>

int clock_gettime(clockid_t clk_id, struct timespec *tp);
```
第一个参数可以是实际时钟 ID `CLOCK_REALTIME` 或单调时钟 ID `CLOCK_MONOTONIC`。`CLOCK_REALTIME` 表示系统的实时时钟，有时会有不连续的跳变甚至向后调整，而 `CLOCK_MONOTONIC` 表示单调递增的时钟，非常平滑，不过度量时间的方式不同。由于我们需要测量时间差，因此应该使用单调时钟 `CLOCK_MONOTONIC`，以避免系统时间的跳变对测量结果产生影响。这里我们定义了一个计算差值的函数 `timespec_diff()`，用于计算两个 `timespec` 结构体表示的时间差。
```c
void
timespec_diff(struct timespec ts1, struct timespec ts2, struct timespec *diff)
{
	long temp;
	diff->tv_sec = ts1.tv_sec - ts2.tv_sec;
	temp = ts1.tv_nsec - ts2.tv_nsec;
	if (temp < 0)
	{
		/* Because temp < 0, we need to borrow 1 sec from tv_sec and
		   add it to tv_nsec as 1000000000 nanoseconds. */
		diff->tv_sec--;
		diff->tv_nsec = 1000000000 + temp;
	}
	else
	{
		diff->tv_nsec = temp;
	}
}

int main(int argc, char *argv[])
{
	int delay = 5; /* Delay in seconds */

	struct timespec init_sleep ={delay, 0};

	struct sigaction act;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
	{
		fatal_error(errno, "sigaction failed");
	}

	printf("about to sleep for %d seconds\n", delay);

	struct timespec start_time, end_time;
	if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1)
	{
		fatal_error(errno, "clock_gettime failed");
	}

	struct timespec remainder;
	int retval;

	do
	{
		retval = nanosleep(&init_sleep, &remainder);
		if (retval == -1)
		{						/* An error or an interruption occured. */
			if (errno == EINTR) /* Received SIGINT */
			{
				init_sleep = remainder;
			}
			else /* Some other non-recoverable error occurred. */
			{
				fatal_error(errno, "nanosleep");
			}
		}
	} while (retval < 0);

	if (clock_gettime(CLOCK_MONOTONIC, &end_time) == -1)
	{
		fatal_error(errno, "clock_gettime failed");
	}

	struct timespec diff;
	timespec_diff(end_time, start_time, &diff);

	printf("Actual sleep time: %ld seconds and %ld nanoseconds\n",
		   diff.tv_sec,
		   diff.tv_nsec);

	return 0;
}
```
下面是两次运行的结果，很明显，尝试打断的话实际休眠的总时长更长，如果没有打断，实际休眠时间接近期望的时间。
```
about to sleep for 5 seconds
Actual sleep time: 5 seconds and 72614 nanoseconds

about to sleep for 5 seconds
^C^C^C^C^C^C^C^CActual sleep time: 5 seconds and 580531 nanoseconds
```

为了解决漂移的问题，可以使用 `clock_nanosleep()` 函数，它的函数原型如下
```c
#include <time.h>

int clock_nanosleep(clockid_t clockid, int flags,
                    const struct timespec *request,
                    struct timespec *remain);
```
第一个参数是时钟 ID，可以传入 `CLOCK_REALTIME` 或 `CLOCK_MONOTONIC` 等。第二个参数是标志位，取值可以是 0 或 `TIMER_ABSTIME`。当标志位为 0 时，请求的时间被解释为相对时间间隔，此时 `clock_nanosleep()` 的行为类似 `nanosleep()`。当标志位为 `TIMER_ABSTIME` 时，请求的时间被解释为绝对时间，进程会睡到指定的时刻，此时最后一个参数可以设置为 NULL。如果进程没有被打断，它保证睡到指定的时间；如果被打断，可以再次传入 `request`，继续睡到指定的绝对时间。

与 `nanosleep()` 相比，`clock_nanosleep()` 的另一个区别是返回值，它在被信号打断时返回错误码 `EINTR`，而不是 -1。所以检查错误时不需要先判断是 -1 再检查 `errno`，只需直接检查返回值是否为 `EINTR` 即可。

## 软件定时器
软件定时器易用易理解，因此先介绍软件定时器的使用方法。早在 1979 年，Unix 中新增了 `alarm()` 系统调用，它是最早的定时器。它的原型是
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```
它的特性如下：

- 最小精度是 1 秒。
- 发送 `SIGALRM` 信号给调用进程。
- 再次调用并传入参数 `0` 可以取消之前设置的闹钟，同时 `alarm(0)` 会返回剩余的秒数。
- 如果先前设置的闹钟还没有到期，再次调用 `alarm()` 会取消前一个闹钟，并返回剩余的秒数。

下面看一个具体的示例，捕获 `SIGALRM` 信号并打印一些信息出来，同时 `main()` 中循环打印流逝的时间。
```c
char MESSAGE[] = "Received a wake-up signal!\n";

void catchalarm(int signum)
{
	write(STDOUT_FILENO, MESSAGE, sizeof(MESSAGE));
	exit(EXIT_SUCCESS); /* Exit the program. */
}

int main(int argc, char *argv[])
{
	int delay = 5; /* Delay in seconds */

	struct timespec sleeptime = { 1, 0 };

	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = catchalarm;
	if (sigaction(SIGALRM, &act, NULL) == -1)
	{
		fatal_error(errno, "sigaction failed");
	}

	printf("about to sleep for %d seconds\n", delay);
	alarm(delay); /* Set an alarm for the specified delay */
	while (1)
	{
		nanosleep(&sleeptime, NULL);
		printf("Sleeping...\n");
	}

	return 0;
}
```

## 间隔定时器
间隔定时器（`interval timer`）最早出现在 4.2BSD 以及 1988 年的 SVR4 中，在 POSIX.1-1994 中标准化。最初的间隔定时器接口定义了两个系统调用 `setitimer()` 和 `getitimer()`。POSIX 在 2008 年将它们标记为已过时，建议使用 POSIX 定时器 API，特别是 `timer_gettime()` 和 `timer_settime()`。

间隔定时器由初始值（`initial value`）和重复间隔（`repeat interval`）组成。初始值是第一次定时器到期之前流逝的时间，重复间隔是第一次到期之后相邻两次到期之间的时间间隔。$t_0$ 时刻启动，初始是 $\alpha$，重复间隔是 $\beta$，则定时器在 $t_0 + \alpha$ 时第一次到期，在 $t_0 + \alpha + \beta$ 时第二次到期，以此类推。

定时器到期时，会向启动该定时器的进程或者线程发送一条定时器通知（`timer notification`）。使用旧接口用 `setitimer()` 设置的间隔定时器，通知方式是信号 `SIGALRM` `SIGVTALRM` `SIGPROF` 之一，依赖于设置定时器的类型。使用新的 POSIX 定时器，通知方式可以由进程自己选择，更为通用。

我们先看创建 API
```c
#include <time.h>
#include <signal.h>

int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
```

第三个参数是一个出参，用于返回创建的定时器 ID。第一个参数是定时器使用的时钟 ID，通常是 `CLOCK_REALTIME` 或 `CLOCK_MONOTONIC`。第二个参数是一个指向 `struct sigevent` 的指针，用于指定定时器到期时的通知方式，这也是新接口提供了更多灵活性的地方。下面是该结构体的定义：
```c
struct sigevent {
	int sigev_notify; /* Notification method */
	int sigev_signo;  /* Notification signal */
	union sigval sigev_value; /* Data passed with notification */
	void (*sigev_notify_function) (union sigval);
	/* Function used for thread notification (SIGEV_THREAD) */
	void *sigev_notify_attributes;
	/* Attributes for notification thread (SIGEV_THREAD) */
	pid_t sigev_notify_thread_id; /* ID of thread to signal; Linux-specific */
};
```
`sigev_notify` 可以取以下值：

- `SIGEV_NONE`：不发送通知。
- `SIGEV_SIGNAL`：通过信号发送通知，使用 `sigev_signo` 指定信号。如果进程使用 `sigaction()` 设置了信号处理函数，那么传给处理函数的 `siginfo_t` 结构体中的 `si_code` 字段是 `SI_TIMER`，`si_signo` 字段是信号编号。
- `SIGEV_THREAD`：通过创建一个新的线程来发送通知，使用 `sigev_notify_function` 指定线程函数，该函数接受 `sigev_value` 作为参数。
- `SIGEV_THREAD_ID`：类似于 `SIGEV_SIGNAL`，但可以指定发送信号的线程 ID，使用 `sigev_notify_thread_id` 指定线程 ID。

如果接受默认的方式，那么把 `sevp` 参数设置为 `NULL`，它等价于 `sigev_notify` 设置为 `SIGEV_SIGNAL`，`sigev_signo` 设置为 `SIGALRM`。

下面的代码示例创建了一个基于 `CLOCK_MONOTONIC` 的定时器，并设置为在到期时通过信号 `SIGUSR1` 通知进程，并且通过设置 `sigev_value` 让处理函数能够接收到定时器 ID。
```c
timer_t timerid;		   /* To store returned timerid */
	struct sigevent sig_event; /* sigevent to pass to timer_create() */
	sig_event.sigev_notify = SIGEV_SIGNAL;		/* Notify by signal. */
	sig_event.sigev_signo = SIGUSR1;			/* Send SIGUSR1. */
	sig_event.sigev_value.sival_ptr = &timerid; /* Make timer ID available. */
	if (timer_create(CLOCK_MONOTONIC, &sig_event, &timerid) == -1)
	{
		fatal_error(errno, "timer_create failed");
	}
```

当程序动态创建资源时，都应该在不需要的时候删除它。定时器使用了系统资源，及时删除可以让程序更高效，并且让其他进程可以使用这些资源。删除定时器的系统调用原型如下
```c
#include <time.h>

int timer_delete(timer_t timerid);
```
如果定时器已经设置了定时事件，那么该函数会将其取消。当定时器还有挂起（`pending`）的信号时，POSIX 没有规定要怎么做。Linux 让这些信号继续处于挂起的状态。

创建好定时器之后我们可以通过下面的系统调用设置定时器的初始值和重复间隔。
```c
#include <time.h>

int timer_settime(timer_t timerid, int flags,
                  const struct itimerspec *new_value,
                  struct itimerspec *old_value);
```
第一个参数是定时器 ID，即 `timer_create()` 返回的定时器标识符。第二个参数 `flags` 用于指定定时器的行为，通常设置为 0，表示传入的时间按照相对时间来解释，如果设置为 `TIMER_ABSTIME`，则 `new_value` 中的时间被解释为绝对时间，如果这个时间已经过去了的话，定时器立即到期。第三个参数和第四个参数分别是新的定时器值和用于保存旧的定时器值的结构体指针。`struct itimerspec` 的定义如下：
```c
struct itimerspec {
	struct timespec it_interval; /* Timer interval */
	struct timespec it_value;    /* Initial expiration */
};
```
`it_value` 指定初始值，`it_interval` 指定时间间隔，也称为周期（`period`）。如果初始值 `it_value` 是 0 并且定时器此前已经设置了，那么会解除设置，这是关闭定时器的方式。如果 `it_value` 不为 0，即 `it_value.tv_sec` 或 `it_value.tv_nsec` 不为 0，那么定时器会在指定的时间到期。如果初始值不是 0 并且定时器之前已经被设置了，旧值被覆盖使用新值。`it_interval` 为 0 表示定时器只触发一次，为非 0 表示定时器周期性触发。

如果 `old_value` 不是 `NULL`，那么上次设置的 `it_interval` 被复制到 `old_value->it_interval`，距离定时器下一次到期的时间复制到 `old_value->it_value`。如果不需要旧值可以传 `NULL`。

该函数可能会调用失败，失败时返回 -1 并设置 `errno`。

如果由于内核调度或者其他活动，定时器产生信号和信号到达之间可能有很长的延迟。在此期间，定时器可能产生了更多的信号。又或者是进程阻塞了该信号使之无法送达。这都可能导致向进程发送的信号超出它能处理的数量。内核为每个进程的每个定时器最多排队一个信号，超出的信号将不会递送。

因此会出现产生而没有发送或接收的事件，它们称为定时器超限（`timer overruns`）。进程可以通过系统调用 `timer_getoverrun()` 获取该计数。其原型如下：

```c
#include <time.h>

int timer_getoverrun(timer_t timerid);
```
这个函数也可以在捕获信号的处理函数里面调用。只要定时器仍活动，就可以调用 `timer_getoverrun()` 获取数据，不过一旦其解除，不管是否有过超限，返回值都是 0。

定时器超限计数很重要，它可以辅助跟踪定时器产生了多少通知，即使进程从未接收到这些通知。例如，如果每个通知使得某个值递增，那么通过定时器超限计数可以补上缺失的递增，确保计算的准确性。

下面我们给一个使用多个定时器的例子。我们每个定时器都使用唯一的实时信号编号，并通过单个信号处理函数捕获所有这些信号。Linux 中可以自由使用 `SIGRTMIN` 到 `SIGRTMAX` 范围内的实时信号，区间规范是左闭右闭，即包括 `SIGRTMIN` 和 `SIGRTMAX`。

注意，下面这段代码中 `long timerid = *(long *)(si->si_value.sival_ptr);` 是不符合标准的。标准规定 `sival_ptr` 是一个 `void *` 指针，直接将其转换为 `long` 并解引用可能会导致未定义行为。这个用法完全是一种依赖于实现的用法。对于现有实现，一个进程如果没有创建过定时器，那么下面创建 8 个的 ID 是从 0 到 7。
```c

#define CLOCKID CLOCK_MONOTONIC
#define NUMTIMERS 8

char *idstr[] = { " 2", " 3", " 5", " 7", " 11", " 13", " 17 ", " 19 " };

void sighandler(int sig, siginfo_t *si, void *uc)
{
	long timerid = *(long *)(si->si_value.sival_ptr);
	write(STDOUT_FILENO, idstr[timerid], strlen(idstr[timerid]));
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	int numtimers = NUMTIMERS;
	int interval[NUMTIMERS] = { 2, 3, 5, 7, 11, 13, 17, 19 };

	if (SIGRTMIN + NUMTIMERS > SIGRTMAX)
	{
		numtimers = SIGRTMAX - SIGRTMIN;
	}

	/* Install signal handlers. */
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	for (int i = 0; i < numtimers; i++)
	{
		sa.sa_sigaction = sighandler;
		sigfillset(&sa.sa_mask); /* Block all other signals. */
		if (-1 == sigaction(SIGRTMIN + 1 + i, &sa, NULL))
		{
			fatal_error(errno, "sigaction");
		}
	}

	/* Create the timers. */
	timer_t timerid[NUMTIMERS];
	struct sigevent sev;
	sev.sigev_notify = SIGEV_SIGNAL;
	for (int i = 0; i < numtimers; i++)
	{
		sev.sigev_signo = SIGRTMIN + 1 + i;
		sev.sigev_value.sival_ptr = &(timerid[i]);
		if (timer_create(CLOCKID, &sev, &(timerid[i])) == -1)
		{
			fatal_error(errno, "timer_create");
		}
	}

	struct itimerspec timer_setting[NUMTIMERS];
	for (int i = 0; i < numtimers; i++)
	{
		struct itimerspec *ts = &timer_setting[i];
		ts->it_value.tv_sec = interval[i] * 0.4;
		ts->it_value.tv_nsec = 0;
		ts->it_interval = ts->it_value;
	}

	for (int i = 0; i < numtimers; i++)
	{
		if (timer_settime(timerid[i], 0, &(timer_setting[i]), NULL) == -1)
		{
			fatal_error(errno, "timer_settime");
		}
	}

	printf("Enter 'q' to terminate this program.\n");
	while (1)
	{
		char c;
		ssize_t nbytes;
		if (-1 == (nbytes = read(STDIN_FILENO, &c, 1)))
		{
			fatal_error(errno, "read");
		}
		else if (c == 'q')
		{
			break;
		}
	}

	return 0;
}
```
