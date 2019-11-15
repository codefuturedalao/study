## Shell

### 一、字符串

```sh
string="Taylor Swift is a singer."

# 字符串长度
echo ${#string}

# 提取字符串
echo ${string:8:5}

# 查找字符i出现的位置
echo `expr index "$string" i`

```

### 二、数组

```sh
star=("Crystal" "Taylor Swift" "Tamia")
star=(
"Crystal"
"Taylor Swift"
"Tamia"
)

# 访问数组	(编号从1开始)
echo ${star[1]}
# 获取全部元素
echo ${star[@]}

# 获取元素个数
echo ${#star[@]}  
echo ${#star[*]}

# 获取单个元素长度
length=${#star[2]}
```

### 三、注释

```bash
# 多行注释
:<<EOF
注释内容
注释内容
注释内容
EOF
```

### 四、参数

| 参数 | 说明                                                         |
| ---- | ------------------------------------------------------------ |
| $#   | 参数个数                                                     |
| $*   | 以一个单字符串显示所有向脚本传递的参数                       |
| $$   | 脚本运行的当前进程ID                                         |
| $！  | 后台运行的最后一个进程的ID号                                 |
| $@   | 与\$*相同，但是使用时加引号，并在引号中返回每个参数。<br /> 如“\$@"用「"」括起来的情况、以"​\$1" "\$2" … "​\$n" 的形式输出所有参数？ |
| $-   | 显示shell使用的当前选项                                      |
| $？  | 显示最后命令退出状态。0表示没有错误                          |

```bash
for i in "$*"; do
	echo $i
done

for i in "$@"; do
	echo $i
done

# 执行 ./test 1 2 3
# 输出
# 1 2 3
# 1
# 2
# 3
```

### 五、运算符

| 运算符    | 说明       |
| --------- | ---------- |
| -eq       | 相等       |
| -ne       | 不相等     |
| -gt       | 大于       |
| -lt       | 小于       |
| -ge       | 大于或等于 |
| -le       | 小于或等于 |
|           |            |
| -a   &&   | and        |
| -o   \|\| | or         |

<font color="blue">字符串</font>

| 运算符 | 说明                                                        |
| ------ | ----------------------------------------------------------- |
| =      | 相等                                                        |
| !=     | 不相等                                                      |
| -z     | 长度为0，返回true                                           |
| -n     | 长度不为0，返回true                                         |
| $      | 不为空返回true，（字符串未定义，如果只是长度为0，返回true） |

<font color="blue">文件测试运算符</font>

| 操作符 | 说明               |
| ------ | ------------------ |
| -b     | 块设备文件         |
| -c     | 字符设备文件       |
| -d     | 目录               |
| -f     | 普通文件           |
| -g     | 是否设置了`SGID`位 |
| -k     | 是够设置了`SBIT`位 |
| -p     | 有名管道           |
| -u     | 是否设置了`SUID`位 |
| -r     | 可读               |
| -w     | 可写               |
| -x     | 可执行             |
| -s     | 不为空返回true     |
| -e     | 存在               |
|        |                    |
| -S     | socket文件         |
| -L     | 符号链接           |

### 六、echo

```sh
# -e 开启转义
echo "Nice to meet you! \n Nice to meet you, too!"	# 换行
echo "Hello \c" && echo "world!"				# 续行
```

### 七、流程控制

<font color="blue" size="5">for</font>

```shell
for val in 1 2 3 4 5
do
	echo "The value is $val"
done
```

```shell
for((i=1; i<=5; i++)); do
	echo "The value is $i"
done;
```

<font color="blue" size="5">while</font>

```sh
# press Ctrl+D to exit
while read NUM
do
	echo "The number you input is $NUM"
done
```

```sh
# 无限循环
while :
do
	command
done

while true:
do
	command
done
```

<font color="blue" size="5">case</font>

```sh
echo "Please input a num between 1 and 4: "
read num
case $num in
	1) echo "number 1"
		echo "over"
		;;
	2) echo "number 2"
		echo "over"
		;;
	3) echo "number 3"
		;;
	4) echo "number 4"
		;;
	*) echo "Not between 1 and 4"
		echo "Error"
		;;
esac

# 可以写为
	1|2|3|4) echo $num
```

###  八、函数

```shell
demoFumc(){
	echo "第一个参数：$1"
	echo "第二个参数：$2"
	# ...
	echo "第10个参数：${10}"		# 注意使用大括号
	
	return 0
}

demoFunc 1 2 3 4 5 6 7 8 9 10
echo $?		# 获取函数返回值，0表示true
```

### 九、重定向

```shell
# 统计文件test.sh的行数
wc -l < ./test.sh
```

<font color="blue" size="5">Here Document</font>

```shell
wc -l << EOF
	printf("Hello World!\n")
	print("Hello World!")
	cout << "Hello World!" << endl;
EOF		# 必须顶格写

# 输出3	
```

### 十、文件包含

test1.sh

```shell
#!/bi/bash

url="https://www.google.com"
```

test2.sh

```shell
#!/bin/bash

# 引用test1.sh文件
. ./test1.sh
# 或者
# source ./test1.sh

echo "谷歌官网：$url"
```

> 被包含文件<font color="red">不需要</font>可执行权限