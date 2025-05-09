什么是好代码？每个人的理解不同。但是至少应该包含以下几点：

1. 文档
2. 分解
3. 命名
4. 编程语言的最佳实践
5. 格式

下面通过一些小的例子来阐述这五点。这里并没有罗列所有应该做的事情，而仅仅是一个引子，目的是论证应该注意这些方面，尽可能写出好代码。

## 文档

首先是文档。文档之一就是注释。写注释是非常重要。要写哪些注释呢？

给用户的接口一般需要注释，特别是参数和返回值不能自说明的情况，如果会抛出异常，也值得单独写出来。有的时候，函数名已经能够自我说明了，但是有些公司还是要求每个 `public` 函数写注释，个人觉得没有必要。不过强制要求还是要遵守，因为各凭经验的话，合作的人多了，项目必然腐烂。

复杂代码值得写注释，那么是无法一眼看出来这些代码是做什么的。常见情况是一些不常见的算法、巧妙的实现等，这时可以添加注释，告诉大家这是什么算法，如果写上具体出处就更好了，比如论文、书籍中具体的地方。

有些历史文件中包含一些元信息，比如谁创建的，修改历史等等，这些不应该再出现在代码中了，而是使用版本管理软件，比如 git 来做。

每个文件的开头，可以包含一些版权信息和关于这个文件或者类的简单说明。后者挺有用的，帮助后来者迅速了解信息。

现在各个语言都有固定的注释格式，比如 c++ 中可以使用 Doxygen，统一注释格式，同时能够自动生成文档。

在添加注释之前，首先要考虑能够通过其他方式，比如重构、重命名、调整代码顺序等增加可读性，达到无需注释的目的。

修改了代码，同步更新注释，如果代码和注释不一致，惨了。

## 分解
将代码切分成逻辑小块，因为谁也不想看到一个 300 行的函数或者上万行代码的类。比如一个函数依次做 A B C D E 几件事情，那么就应该拆成几个函数，那么当前函数依次调用即可。

分解不是精准的科学，不能以多少行为标准，更多的是逻辑的划分和以提高可读性为标准作为准则。

一开始写的代码质量可能很高，但是随着添加新功能和修复问题，慢慢代码质量就下降了。此时需要重构。常见的重构方法有：1）封装数据类型，泛化类型以共享代码；2）抽取函数和类，更容易理解、维护、复用；3）调整类、函数到合适的地方，重命名使得意思更明确。重构是重新审视代码的好机会。重构需要配合测试，防止破坏功能。

写代码之前，应该有好的设计，至少要想清楚。

## 命名
首先命名要符合编程语言的规定。

在符合规定的前提下，选择一个好名字很重要，好的名字要简洁、表意、准确，也可以传递出其他一些信息，比如类型。

循环变量使用 `i` `j` 就足够好了，在合适的上下文，可以使用 `row` `column` 替代。

前缀，个人是不推荐的，比如 `bool` 前面要加 `b`，不如在合适的情况下加一个 `is`，再比如 `static` 变量前面加个 `s`，如果有一天不是 `static` 的了，还要满世界修改变量名。这些信息更多的应该交给编辑器和辅助工具。

关于大写小写、蛇形等等，这个完全看个人习惯，组织规定和编程语言的惯例。比如 C# 和 rust 就完全是两种风格。不管怎样，保持一致很重要。

## 编程语言的最佳实践
C++ 是非常强大的编程语言，如果不加限制，可以写出如下烂代码
```cpp
i++ + ++i;
```
因此，写代码的时候要合理使用语言提供的功能。具体到 C++，要合理使用 `const`，尽可能使用引用代替指针，涉及内存管理考虑智能指针，可以考虑使用异常处理错误，如果是使用传统的返回值，可以考虑添加 `[[nodiscard]]` 让调用者使用返回值。

语言功能是帮助程序员的，要理解和使用那么能够帮助写出好代码的功能。

## 格式
这是一个比命名风格还要有争议的领域。比如大括号放行末还是另起一行，使用空格还是 `Tab`，怎么折行等等。

和命名风格一样，保持一致很重要。一旦有一个统一的格式，可以考虑使用工具，比如 clang-format，来强制风格统一。
