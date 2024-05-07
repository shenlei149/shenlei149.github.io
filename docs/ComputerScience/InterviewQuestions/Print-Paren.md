实现一个算法，打印所有的合法的 $n$ 对括号的组合。比如两对括号，`(()) | ()()`，两组解，三对括号，`(()()) | ((())) | ()(()) | (())() | ()()()`，共计五组解。

很明显，这是一个递归问题。每一次递归，我们向字符串中添加一个字符，左括号或者是右括号。如何决定添加的是左括号还是右括号呢？只要还有剩余的左括号，总是可以往里面添加的；至于右括号则不然，只有当前字符串中左括号的数量比右括号大的时候，才能插入右括号，对应剩余的可插入括号来说，就是剩余的右括号比左括号的数量多。

我们使用两个参数来记录剩余的左括号和右括号数量，初始值都是 $N$，用以表示上面分析的各个状态。

下面是实现代码：
``` csharp
static void AddParen(List<string> result, int leftRem, int rightRem, char[] str, int index)
{
    if (leftRem < 0 || rightRem < leftRem) // invalid state
    {
        return;
    }

    if (leftRem == 0 && rightRem == 0)
    {
        result.Add(new string(str));
    }
    else
    {
        if (leftRem > 0)
        {
            str[index] = '(';
            AddParen(result, leftRem - 1, rightRem, str, index + 1);
        }

        if (rightRem > leftRem)
        {
            str[index] = ')';
            AddParen(result, leftRem, rightRem - 1, str, index + 1);
        }
    }
}

static List<string> GenerateParens(int n)
{
    char[] str = new char[n * 2];
    var result = new List<string>();
    AddParen(result, n, n, str, 0);
    return result;
}
```
