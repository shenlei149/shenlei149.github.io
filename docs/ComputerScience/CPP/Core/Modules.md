模块（`module`）是 C++20 引入的一个新特性，它提供了一种新的方式来组织和管理代码。C++23 又将 std 模块化，使得标准库也可以通过模块的方式导入。首先看一个经典的例子。
```cpp
import std;

int main()
{
	std::print("Hello, World!\n");

	return 0;
}
```
第一行导入 std 模块，后续使用 `std::print` 来打印字符串。使用 g++ 15 版本编译时，需要先编译出 std 模块，然后再编译 HelloWorld.cpp 文件，编译命令如下：
```bash
g++ -std=c++23 -fmodules -fsearch-include-path -c bits/std.cc
g++ -std=c++23 -fmodules HelloWorld.cpp -o HelloWorld
```
下面定义一个简单的 `math` 模块，提供一个 `Add` 函数来计算两个整数的和。
```cpp
// math.ixx
export module math;

export int Add(int a, int b) { return a + b; }
```
接着在 `main.cc` 中导入 `math` 模块，并使用 `Add` 函数：
```cpp
import math;
import std;

int main()
{
	std::print("5 + 3 = {}\n", Add(5, 3));

	return 0;
}
```

模块有很多优势。首先，以前 `#include` 的方式会将头文件的内容直接插入到源文件中，导致编译时间长、重复编译等问题。模块只导入一次，近乎是零开销的。其次，模块可以隐藏实现细节，只暴露接口，导出时代了模块的名字，减少了命名冲突的可能性。导入顺序不会影响编译结果，而 `#include` 的顺序可能会影响编译结果和程序的行为，导致很多编译和运行的问题。有了模块，无需再将代码分成头文件和源文件，模块可以直接导出函数、类、变量等，简化了代码结构，同时，模块提供了表达代码逻辑结构的能力，可以更好的组织代码。

一个模块包含一个或多个模块单元（`module unit`），一个模块单元是包含模块声明的特殊编译单元（`translation unit`）。除了全局模块片段（`global module fragment`）之外，模块声明必须是特殊编译单元的第一条声明。每个模块单元都要与一个模块名关联。声明语法是
```cpp
[export] module module-name[:partition-name];
```

- `module-name` 是模块的名字，可以包含 `.`，这个没有特殊含义，但是可以表达组织层级结构。
- `export`：使用了这个关键字的称为模块接口单元（`module interface unit`），否则称为模块实现单元（`module implementation unit`）。
- `partition-name`：模块分区的名字。模块分区是一个内部分区（`internal partition`）或者接口分区（`interface partition`）。内部分区在模块外是不可见的，用于提供该模块内部的声明与定义。接口分区参与并扩展了模块的公开导出接口。
- 命名模块（`named module`）是具有相同模块名称的所有模块单元（`module unit`）的集合。
- 主模块接口（`primary module interface`）是每个命名模块必须有、且只有一个非模块分区的模块接口单元。它导出的内容对导入该模块的客户端完全可见。

我们可以导出说明符、导出组、导出命名空间，示例如下。我们也可以在命名空间内 `export`，但是更推荐 `export namespace`。
```cpp
export module math;
export int Mult(int fir, int sec);
export void DoTheMath();

export module math;
export // export group
{
	int Mult(int fir, int sec);
	void DoTheMath();
}

export module math;

export namespace math
{
int Mult(int fir, int sec);
void DoTheMath();
} // namespace math
```
内部链接性（`internal linkage`）不能导出，比如 `static` 函数、`const` 变量、匿名命名空间等。

使用 `import`，可以导入一个模块、一个模块分区或者是头文件单元（`header unit`）。`import` 是一个上下文关键字，也就是说它本质上是一个标识符（`identifier`），只有在特定的上下文语境中它才会被视为关键字。在导入之前，应该对被导入的模块进行编译，否则可能会导入旧版本。导入模块的语法如下：
```cpp
export module math;
import math.sin;
import math:cos;
import <vector>;
```

对于模块文件，推荐使用以下顺序。
```cpp
module; // global module fragment

#include <header> // headers for libraries not modularized so far

export module math; // exporting module declaration

import <importing of other modules>

<non-exported declarations> // only visible within this module unit

export namespace math
{
	<exported declarations>
}

module :private; // not part of the module interface
```

开始是一个以 `module;` 开头的全局模块片段，这个是可选的。在此之后在模块声明之前是包含头文件的位置。这里只允许使用预处理指令。

必需的导出模块声明（`export module`）后面是模块前导区（`module preamble`），接着是模块作用域（`module purview`）。模块前导区由导入声明（`import`）组成，模块作用域由导出声明（`export`）组成。

模块作用域可以包含私有模块片段（`private module fragment`），是模块实现的一部分，只能在主模块接口单元中使用。修改了这个区域不需要重新编译模块的客户端。

非导出的声明和导入其他模块都只能在当前模块可见，在模块外部不可见。

和传统的组织文件的方式类似，如果一个模块很大，应该将接口和实现分开，接口单元和实现单元可以在不同的文件中。比如我们将 `math` 模块分开，模块接口在 `ixx` 文件中，模块实现单元在 `cc` 文件中。
```cpp
// math.ixx
module;

import std;

export module math;

export namespace math
{

int Add(int first, int second);

int GetProduct(const std::vector<int> &vec);

} // namespace math

// math.cc
module math;

import std;

namespace math
{

int Add(int first, int second) { return first + second; }

int GetProduct(const std::vector<int> &vec)
{
	return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}

} // namespace math
```

模块接口单元和模块实现单元分离的好处是修改模块实现单元不影响模块接口单元，客户端不需要重新编译。模块接口单元和模块实现单元可以在不同的文件中，也可以在同一个文件中。如果是后者，使用私有模块片段来分离两者，比如前面的例子可以改写为：
```cpp
// math.ixx
module;

import std;

export module math;

export namespace math
{

int Add(int first, int second);

int GetProduct(const std::vector<int> &vec);

} // namespace math

module :private;

int Add(int first, int second) { return first + second; }

int GetProduct(const std::vector<int> &vec)
{
	return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}
```

随着模块的增长，我们需要按照功能将其划分成可管理的组件，模块提供了子模块（`submodule`）和分区（`partition`）来实现。

首先我们看子模块的例子。模块可以被导入之后紧接着被导出。主模块接口单元可以导入其他模块，并将其导出。
```cpp
// math.ixx
export module math;

export import math.math1;
export import math.math2;

// math1.ixx
export module math.math1;

export int Add(int first, int second) { return first + second; }

// math2.ixx
export module math.math2;

import std;

export // export group
{
	int GetProduct(const std::vector<int> &vec)
	{
		return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
	}
}
```
使用者可以像之前一样导入 `math` 模块，使用 `Add` 和 `GetProduct` 函数。同时，子模块也是模块，所以使用者可以更小粒度的控制导入的内容，比如只导入 `math.math1` 模块，只能使用 `Add` 函数。

一个模块还可以分成若干个分区，每个分区包含一个模块接口单元和零个、一个或多个模块实现单元。分区不能独立存在，也不能分成更小的分区了。分区的名字必须和模块的名字相同，因此导入导出的时候无需指定模块名。分区不是独立的模块单元，它们是模块的一部分，客户端无法使用分区的名字来导入分区，只能使用模块的名字来导入模块。下面将上面的例子改写为使用分区的方式。
```cpp
// math.ixx
export module math;

export import :math1;
export import :math2;

// math1.ixx
export module math:math1;

export int Add(int first, int second) { return first + second; }

// math2.ixx
export module math:math2;

import std;

export // export group
{
	int GetProduct(const std::vector<int> &vec)
	{
		return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
	}
}
```

对于模块我们需要区分可见性（`visibility`）和可达性（`reachability`）。导出的实体是可见的，客户端可以使用，非导出的实体不可见（无法使用）但是是可达的。比如下面的例子中 `Bar` 是可见的并且是可达的，但是 `Foo` 是不可见的但是是可达的。不能直接使用 `Foo`，但是可以通过 `Bar` 返回 `Foo` 的实例来使用 `Foo` 的对象。通过 `decltype` 可以获取 `Foo` 的类型，间接的能够创建其对象。
```cpp
// bar.ixx
module;

import std;

export module bar;

struct Foo
{
	void WriteName() const { std::print("Foo\n"); }
};

export struct Bar
{
	Foo GetFoo() const { return Foo {}; }
};

// main()
Bar bar;
// Foo foo; // error: ‘Foo’ was not declared in this scope

auto f = bar.GetFoo();
f.WriteName();

using FooAlias = decltype(std::declval<Bar>().GetFoo());
FooAlias foo;
foo.WriteName();
```

在 C++20 之前，C++ 支持两种链接性（`linkage`）：内部链接性（`internal linkage`）和外部链接性（`external linkage`）。内部链接性只能在当前编译单元中使用，包含 `static` 函数、`const` 变量、匿名命名空间等。外部链接性可以在其他编译单元中使用，包含全局变量、函数、类等。模块引入了模块链接性（`module linkage`），仅在当前模块中可见，如果一个符号没有导出，则它具有模块链接性。比如上面的例子中，`Bar` 是外部链接性，`Foo` 是模块链接性，如果是使用头文件的形式，那么两者都是外部链接性。

头文件单元（`header unit`）是头文件的一种二进制标识，是从头文件平滑过渡到模块的桥梁，我们可以使用 `import` 来导入头文件，比如
```cpp
import <vector>;
import "myheader.h";
```
首先，`import` 和 `#include` 的查找规则是一样的，比如引号的导入，先找本地目录，再找系统目录。其次，导入头文件并不是简单的文本替换，编译器根据 `import` 指令来生成类似于模块的东西，并将结果视为模块。导入该头文件会获取所有可导出的名字，包括宏。导入这些头文件比 `#include` 的方式更快，并且构建的速度和功能也基本媲美预编译的头文件。最后，在 `import` 之前的宏定义在其内部不可见。

这个方法的缺点是并不是所有的头文件都是可以导入的，这取决于实现，不过 C++ 标准要求所有的标准库的头文件是可以导入的。C 语言的头文件不能导入，不过它们在 `std` 命名空间中有对应的 C++ 版本的头文件是可以导入的。比如 `#include <stdio.h>` 对应的是 `import <cstdio>`。

头文件单元可以包含宏，不过模块会忽略宏。我们不能从模块中导出宏，但是可以在模块中 `#include` 头文件，而这个头文件可以包含宏。全局模块片段 `module;` 关键字下面就是插入头文件引入宏的地方。

模块中模板函数的使用抽象语法树（`abstract syntax tree`, `AST`）来表示，使得模板的定义和实现可以分开。
