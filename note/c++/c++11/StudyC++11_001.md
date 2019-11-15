# Sutdy C++11

## 一、保证稳定性和兼容性

### 1. 保证和C99的兼容性

#### 1.1 预定义宏

C：         1999(C99)

C++: 1998(C++98) ---->  2011(C++11)

| 宏名称                 | 功能描述                                                     |
| ---------------------- | ------------------------------------------------------------ |
| \_\_STDC_HOSTED\_\_    | 如果编译器的目标系统环境中包含完整的标准C库，那么这个宏就定义为1，否则宏的值为0 |
| \_\_STDC\_\_           | C编译器通常用这个宏的值来表示编译器的实现是否和C标准一致。C++ll标准中这个宏是否定义以及定成什么值由编译器来决定 |
| \_\_STDC_VERSION\_\_   | C编译器通常用这个宏来表示所支持的C标准的版本，比如1999mmL。C++l1标准中这个宏是否定义以及定成什么值将由编译器来决定 |
| \_\_STDC_ISO_10646\_\_ | 个宏通常定义为一个yyyymmL格式的整数常量，例如199712L，用来表示C++编译环境符合某个版本的ISO/IEC10646标准 |

#### 1.2 `__func__`

```C++
void printFuncName() { printf("%s", __func__); }	// 输出 printFuncName

void FuncFail(string func_name = __func__)		// Error，在参数声明时，_func_还未被定义
```

#### 1.3 _Pragma

```C++
_Pragma("once")		//#pragma once
```

#### 1.4 __VA_ARGS__

```C++
#define LOG(...)  {\
	fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__);\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\
}
```

#### 1.5 宽窄字符串的连接

<无>

### 2. long long整型

```C++
long long LongLongNumber = 0;		// LLONG_MAX
```

### 3. 扩展的整型

C++1l规定，扩展的整型必须和标准类型一样，有符号类型和无符号类型占用同样大小的内存空间。

### 4. 宏cplusplus

### 5. 静态断言

#### 5.1 断言：运行时与预处理时

> 断言（assertion）是一种编程中常用的手段。在通常情况下，断言就是将一个返回值总是需要为真的判别式放在语句中，用于排除在设计的逻辑上不应该产生的情况。比如一个函数总需要输入在一定的范围内的参数，那么程序员就可以对该参数使用断言，以迫使在该参数发生异常的时候程序退出，从而避免程序陷入逻辑的混乱。



#### 5.2 静态断言与static_asser

```C++
template<typename T, typename U>
int bit_copy(T& t, U& u) {
	static_assert(sizeof(t) == sizeof(u), "Error");
	//...
	return sizeof(t);
}

int main() {
    int from = 825;
	double dest = 0;
	bit_copy(from, dest);	// compile error
}
```

### 6. noexcept 与 noexcept

noexcept形如其名地，表示其修饰的函数不会抛出异常。不过与throw动态异常声明不同的是，在C++11中如果noexcept修饰的函数抛出了异常，编译器可以选择直接调用stdterminate()函数来终止程序的运行，这比基于异常机制的throwo在效率上会高一些。
这是因为异常机制会带来一些额外开销，比如函数抛出异常，会导致函数栈被依次地展开（unwind），并依帧调用在本帧中已构造的自动变量的析构函数等。

### 7. 快速初始化成员变量

就地花括号

### 8. 非静态成员的 sizeof

```C++
class Foo {
    public:
    	int intNumber;
    	static float floatNumber;
    // ...
}

int main() {
	std::cout << sizeof(Foo.floatNumber) << std::endl; 		// OK
    std::cout << sizeof((Foo*)0)->intNumber << std::endl; 	// OK
    std::cout << sizeof(Foo.intBumber) << std::endl; 	   // Error in C++98. But OK in C++11
}
```

### 9. 扩展的friend 语法

```C++
class P;
class Q;

class R {
	friend class P;		// OK
    friend Q;			// Error in C++98. OK in C++11;
}
```



### 10. final/override控制



### 11. 模板函数的默认模板参数



### 12. 外部模板

#### 12.1 为什么需要外部模板

#### 12.2 显式的实例化与外部模板的声明

```c++
// test.h
template <typename T> void func(T) {}
```

```C++
// test1.cpp
#include "test.h"
template void func<int>(int);
void test1() {
    func(3);
}
```

```C++
// test2.cpp
#include "test.h"
extern template void func<int>(int);	// 只生成一份实现
void test2() {
    func(3);
}
```

### 13. 局部和匿名类型作模板实参

在C++98中，局部的类型和匿名的类型在C++98中都不能做模板类的实参。在c++11中可以。

```C++
template <typename T> class X {};
template <typename T> void TempFun(T t) {};
struct A {}a;
struct { int i; }b;//b是匿名类型变量
typedef struct { int i; }B;//B是匿名类型

void func() {
	struct C {}c;	//c是局部类型
	X<A> ×1;		//C++98通过，C++11通过
	X<B> ×2;		//C++98错误，C++11通过
	X<C> ×3;		//C++98错误，c++11通过
	TempFun(a);		//C++98通过，c++11通过
	TempFun(b);		//C++98错误，c++11通过
	TempFun(c);		//C++98错误，c++11通过
}
```


