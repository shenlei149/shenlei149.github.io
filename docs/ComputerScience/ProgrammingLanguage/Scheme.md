本文仅总结了一些 Scheme 的一些点，并不是完整的教程。

## 基础
`car` 返回列表的第一个元素，列表不能是空 `'()`
``` scheme
> (car '(1 2 3))
1
> (car '((1 2) 3))
'(1 2)
```

`cdr` 返回一个列表但不包含第一个元素，同样，列表不能为空 `'()`。
``` scheme
> (cdr '(1 2 3))
'(2 3)
> (cdr '((1 2) (2 3) 4))
'((2 3) 4)
```

`car` 和 `cdr` 可以组合使用，比如 `cadr` 表示从不包含第一个元素的列表中返回第一个，即返回第二个元素。
``` scheme
> (cadr '(1 (2 3) (3) 4))
'(2 3)
```
为什么可以组合用呢？Scheme的内存模型决定了这一点，TODO。

`cons` 把第一个参数（可以是列表）作为第二个参数的第一个元素，并返回。
``` scheme
> (cons '(1) '(2 3))
'((1) 2 3)
```

`append` 把后续的参数加到第一个参数列表后面。
``` scheme
> (append '(1) '((2 3) 4))
'(1 (2 3) 4)
```

`map` 的作用就是遍历每一个列表，然后用第一个参数来对它们进行求值。
``` scheme
> (map + '(1 2) '(3 4) '(5 6))
'(9 12)
```
后续的参数是长度为 2 的列表，那么结果也是一个长度为 2 的列表，第一项就是把后面每个列表的第一项拿出来，使用 + 来就行求值，得到了 9，类似的，第二项是 12。


`apply` 和 `eval` 函数。
``` scheme
> (apply + '(1 2 3))
6

> '(+ 1 2 3)
'(+ 1 2 3)
> (eval '(+ 1 2 3))
6
```

`let`，给表达式起一个别名。下面两种写法等价。
``` scheme
(let ((x exp1) (y exp2) (z exp3)) (fn x y z))

((lambda (x y z) (fn x y z)) exp1 exp2 exp3)
```

### 自定义函数
自定义函数。
``` scheme
(define (double x) (* x 2))
```

lambda 表达式
``` scheme
(define sum (lambda (x y)
  (+ x y)))
```

`map` 与自定义函数结合。
``` scheme
> (map double '(1 2 3))
'(2 4 6)
```

求平均数的小例子。
``` scheme
(define (average num-list)
  (/ (apply + num-list)
     (length num-list)))
```

使用 lambda 表达式往往能够简化实现。
``` scheme
(define (translate point delta)
  (define (shift-by x)
    (+ x delta))
  (map shift-by point))

(define (translate point delta)
  (map (lambda (x)
         (+ x delta))
       point))
```


定义任意参数的函数。使用 . 来表示把后面的参数打包起来。
``` scheme
(define (bar a b c . d)
  (list a b c d))

> (bar 1 2 3 4 5 6)
'(1 2 3 (4 5 6))
> (bar 1 2 3)
'(1 2 3 ())
```

## 实现一些内置函数
### flatten
`flatten` 将所有元素平铺，不管有多少层列表的嵌套，都会展开为一维列表。
``` scheme
(define (my-flatten seq)
  (if (not (list? seq)) (list seq)
      (apply append
             (map my-flatten seq))))

> (my-flatten '((1 2) (3 ((4) 5)) 6))
'(1 2 3 4 5 6)
```
如果 `seq`（传入的列表）不是一个列表，就返回一个列表，包含元素自己；否则，使用 `map` 函数，对 `seq` 中的每一个子列表进行 `flatten`操作，最后，使用 `append` 把这些结果连接起来。这里有一个隐式的递归，处理列表嵌套的情况。

### map
首先，我们实现一个一元的 `map`，也就是说，传给 `map` 的函数只接受一个参数。
``` scheme
(define (unary-map fn seq)
  (if (null? seq) '()
      (cons (fn (car seq))
            (unary-map fn (cdr seq)))))
```

`map` 把每一个 `list` 里面的第一个元素拿出来传入 `fn` 让其运算，然后呢，把每一个 `list` 里面的第一个元素删除，得到一系列的 `list`，然后递归调用。

说起来容易，如何设计参数呢？第一个参数是一个 `list`，第二个参数是 .，也就是可变参数，把后面的所有 `list` 打包传进来。拿第一个 `list` 的第一个元素简单，如何把后面每个 `list` 的第一个元素取出呢？利用前面实现的一元 `map`，`(unary-map car other-list)`，就得到其余 `list` 的第一个元素，和第一个 `list` 的第一个元素连接起来，传给 `fn` 去做计算。

后续的移除第一个元素之后递归也是类似的。最后把结果连接成一个 `list`。
``` scheme
(define (my-map fn first-list . other-list)
  (if (null? first-list) '()
      (cons (apply fn
                   (cons (car first-list)
                         (unary-map car other-list)))
            (apply my-map
                   (cons fn
                         (cons (cdr first-list)
                               (unary-map cdr other-list)))))))
```
两个简单的测试用例：
``` scheme
> (my-map + '(1 2 3) '(4 5 6) '(7 8 9))
'(12 15 18)

> (my-map (lambda (x) (+ x 1)) '(1 2 3))
'(2 3 4)
```

## 示例
### 所有子集算法
``` scheme
(define (ps set)
  (if (null? set) '(())
      (let ((ps-rest (ps (cdr set))))
        (append ps-rest
                (map (lambda (subset)
                       (cons (car set) subset))
                     ps-rest)))))
```
首先，如果 `set` 为空，返回一个由空列表组成的列表；下面运用递归的思想，定义 `ps-rest` 是 `set` 排除第一个元素的集合的所有子集，比如传入的 `set` 是 `'(1 2 3)`，排除第一个元素就是 `'(2 3)`，它的所有子集组成的列表是 `'(() (3) (2) (2 3))`，列表里面共四个元素，这就是 `ps-rest`。我们往`ps-rest` 里面 `append` 包含第一个元素的集合。具体 `append` 什么呢？我们 `map` `ps-rest`，在每一项前面加上第一个元素。继续前面的例子，`ps-rest` 第一项是空集，连接完之后是 `(1)`，`append`到 `ps-rest` 里面，第二项是 `(3)`，连接完之后是 `(1 3)`，放到 `ps-rest` 里面，第三项第四项类似。最终，我们就得到了结果 `'(() (3) (2) (2 3) (1) (1 3) (1 2) (1 2 3))`。

### 全排列算法
``` scheme
(define (permute items)
  (if (null? items) '(())
      (apply append
             (map (lambda (ele)
                    (map (lambda (pers)
                           (cons ele pers))
                         (permute (remove ele items))))
                  items))))
```
用 `apply` 函数将一系列结果 `append` 起来，也就是把后面 `map` 里面的求值结果串起来。`map` 的求值结果是什么呢？它遍历了输入的 `items`，比如我们输入 `'(1 2 3)`，第一次遍历使用的是 1，也就是向 `map` 的第一个参数 lambda 表达式传入了 1，也就是 `ele` 是 1。接着，对 1 做了什么呢？又是一个 `map`，遍历了 `(permute (remove ele items))`，`remove` 是从 `items` 中移出 `ele`，这个表达式求值结果是 `'((2 3) (3 2))`，遍历它，把 1 加到每项的前面，得到的结果是 `'((1 2 3) (1 3 2))`。至此，第一次遍历完成，回到外层的 `map`，开始第二次遍历，传入 `ele` 为 2，后续就不再赘述了。最终得到的结果是 `'((1 2 3) (1 3 2) (2 1 3) (2 3 1) (3 1 2) (3 2 1))`。
