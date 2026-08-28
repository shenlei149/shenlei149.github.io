C++11 最大的胜利就是并发支持进入了语言和类库标准中。现在，程序员可以编写在所有平台都具有标准行为的多线程程序。这为构建具有极强表达力的库奠定了坚实的基础。标准库的并发组件（任务 `task`、`future`、线程 `thread`、互斥 `mutex`、条件变量 `condition variable`、原子操作 `atomic` 等）仅仅是一个开始，未来工具集会更加丰富。

标准库提供了两个 `future`：`std::future` 和 `std::shared_future`。大部分时候无需区分二者，都使用术语 `future` 表示。
