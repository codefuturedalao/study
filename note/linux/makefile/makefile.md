

## 一、文件搜寻

Makefile 文件中的特殊变量“VPATH”就是完成这个功能的，如果没有指明这个变量，make 只会在当前的目录中去找寻依赖文件和目标文件。如果定义了这个变量，那么，make 就会在当当前目录找不到的情况下，到所指定的目录中去找寻文件了。

```makefile
SRC_DIR = src
DIRS = $(shell find $(SRC_DIR) -maxdepth 3 -type d)
# 目录由冒号分隔
VPATH = $(DIRS):../headers  
```

## 二、函数

### 1. foreach

```makefile
$(foreach <var>, <list>, <text>)
```

把参数`<list>`中的单词逐一取出来放到参数`<var>`所指定的变量中，然后再执行`<text>`所包含的表达式。每一次`<text>`会返回一个字符串，循环过程中，`<text>`的所返回的每个字符串会以空格分隔，最后当整个循环结束时，`<text>`所返回的每个字符串所组成的整个字符串（以空格分隔）将会是`foreach`函数的返回值。

### 2. wildcard

```makefile
$(wildcard include/*.h)
```

展开通配符

### 3. addprefix

```makefile
$(addprefix <prefix>, <names...>)
```

名称：加前缀函数——addprefix。
功能：把前缀`<prefix>`加到`<names>`中的每个单词后面。
返回：返回加过前缀的文件名序列。
示例：$(addprefix src/,foo bar)返回值是`src/foo src/bar`。

### 3. dir

```makefile
$(dir <names...> )
```

名称：取目录函数——dir。
功能：从文件名序列`<names>`中取出目录部分。目录部分是指最后一个反斜杠（“/”）之
前的部分。如果没有反斜杠，那么返回`./`
返回：返回文件名序列`<names>`的目录部分
示例： $(dir src/foo.c hacks)返回值是`src/ ./`

### 4. notdir

```makefile
$(notdir <names...> )
```

名称：取文件函数——notdir。
功能：从文件名序列`<names>`中取出非目录部分。非目录部分是指最后一个反斜杠（“/”）之后的部分
返回：返回文件名序列`<names>`的非目录部分。
示例： $(notdir src/foo.c hacks)返回值是`foo.c hacks`

### 5. suffix

```makefile
$(suffix <names...> )
```

名称：取后缀函数——suffix。
功能：从文件名序列`<names>`中取出各个文件名的后缀。
返回：返回文件名序列`<names>`的后缀序列，如果文件没有后缀，则返回空字串。
示例：$(suffix src/foo.c src-1.0/bar.c hacks)返回值是`.c .c`

### 6. join

```makefile
$(join <list1>,<list2> )
```

名称：连接函数——join。
功能：把`<list2>`中的单词对应地加到`<list1>`的单词后面。如果`<list1>`的单词个数要比`<list2>`的多，那么，`<list1>`中的多出来的单词将保持原样。如果`<list2>`的单词个数要比`<list1>`多，那么，`<list2>`多出来的单词将被复制到`<list2>`中。
返回：返回连接过后的字符串。
示例：$(join aaa bbb , 111 222 333)返回值是“aaa111 bbb222 333”。

### 7. subst

```makefile
$(subst <from>, <to>, <text>)
```

名称：字符串替换函数——subst。
功能：把字串`<text>`中的`<from>`字符串替换成`<to>`
返回：函数返回被替换过后的字符串

### 8. patsubst

```makefile
$(patsubst <pattern>,<replacement>,<text> )
```

名称：模式字符串替换函数——patsubst。
功能：查找`<text>`中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式`<pattern>`，如果匹配的话，则以`<replacement>`替换。这里，`<pattern>`可以包括通配符“%”，表示任意长度的字串。如果`<replacement>`中也包含“%”，那么，`<replacement>`中的这个“%”将是`<pattern>`中的那个“%”所代表的字串。（可以用“\”来转义，以“\%”来表示真实含义的“%”字符）
返回：函数返回被替换过后的字符串。

### 8. filter

```makefile
$(filter%.o,$(files))
```

```makefile
files = foo.elc bar.o lose.o
$(filter %.o,$(files)): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
$(filter %.elc,$(files)): %.elc: %.el
	emacs -f batch-byte-compile $<
```

\$(filter\%.o,​\$(files))表示调用 Makefile 的 filter 函数，过滤“​\$filter”集，只要其中模式为“%.o”的内容。其的它内容，我就不用多说了吧。这个例字展示了 Makefile 中更大的弹性。

## 三、自动化变量

### 1. $@

表示规则中的目标文件集。在模式规则中，如果有多个目标，那么，"$@"就是匹配于目标中模式定义的集合。

### 2. $%

仅当目标是函数库文件中，表示规则中的目标成员名。例如，如果一个目标是"foo.a(bar.o)"，那么，"$%"就是"bar.o"，"$@"就是"foo.a"。如果目标不是函数库文件（Unix 下是[.a]，Windows 下是[.lib]），那么，其值为空。

### 3. $<

依赖目标中的第一个目标名字。如果依赖目标是以模式（即"%"）定义的，那么"$<"将是符合模式的一系列的文件集。注意，其是一个一个取出来的。

### 4. %?

所有比目标新的依赖目标的集合。以空格分隔。

### 5. $^

所有的依赖目标的集合。以空格分隔。如果在依赖目标中有多个重复的，那个这个变量会去除重复的依赖目标，只保留一份。

### 6. $+

这个变量很像"$^"，也是所有依赖目标的集合。只是它不去除重复的依赖目标。

### 7. $*

这个变量表示目标模式中"%"及其之前的部分。如果目标是"dir/a.foo.b"，并且目标的模式是"a.%.b"，那么，"\$\*"的值就是"dir /a.foo"。这个变量对于构造有关联的文件名是比较有较。如果目标中没有模式的定义，那么"\$\*"也就不能被推导出，但是，如果目标文件的后缀是 make 所识别的，那么"\$\*"就是除了后缀的那一部分。例如：如果目标是"foo.c"，因为".c"是 make 所能识别的后缀名，所以，"​\$\*"的值就是"foo"。这个特性是 GNU make 的，很有可能不兼容于其它版本的 make，所以，你应该尽量避免使用"\$\*"，除非是在隐含规则或是静态模式中。如果目标中的后缀是make 所不能识别的，那么"\$\*"就是空值。



## 四、set -e

set -e表示一旦脚本中有命令的返回值为非0，则脚本立即退出，后续命令不再执行;









