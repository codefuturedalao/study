

## Task1

+ 将一篇乱序的英文文章正确解码；
+ 在控制台显示解码后的信息；
+ 将解码信息写入文件；
+ 必须使用类及标准的成员函数完成对应功能；
+ 在主函数中能通过简单的调用执行该功能；

#### 数据说明

+ 乱序的二进制文件；
+ 1`char`+1`int`，其中`char`为原文字符，`int`为其所在位置的标示量（该标示量只标示其相对位置，非绝对位置）；
+ 根据标示量的大小升序排序，排序后即为正确结果



## Method1:

```C++
struct Pair
{
	Pair() : 
		character(0), index(0x7FFFFFFF) { }	// MAX_INT=0x7FFFFFFF
	char character;
	int index;
};	/* 由于内存对齐，Pair占用8个字节(3个空字节) */

class Decoder {
public:
    // ...
private:
    void  sort_(Pair* pair, int left, int right);	// 快速排序
    Pair* content_;
    int lenght_;
};
```

由于内存对齐，`Pair`占用8个字节(3个空字节)，因此动态申请内存空间时要申请`1.67倍`于文件大小的空间。



## Method2

```C++
class Decoder {
public:
    // ...
private:
    void  sort_(int* intArr, char* charArr, int left, int right);	// 快速排序
    char* content_;
	int*  index_;
	int	  length_;
};
```

`content_` 和 `index_`一一对应，排序中只比较`index_`每个元素大小，但交换时，`content_` 和 `index_`都要交换。



## [PS]

对于内置类型（`POD类型`，如int，char，double、自己定义的符合POD类型的结构体和类，等等），`delete`和`delete[]`没有区别。对于需要析构的结构体、类来说，`delete`只对第一个元素调用**析构函数**，` delete[]` 则会对所有元素调用析构函数。 

```C++
int* pInt = new int[128];
delete pInt;	// OK
delete[] pInt;	// OK

// ---------------------------------------------------

class MyClass {
public:
    MyClass() { pInt = new int; }
    ~MyClass() { if (pInt) delete pInt; }
private:
    int* pInt;
};

MyClass* pMyClass = new MyClass[128];
delete pMyClass;	// Error
delete[] pMyClass;	// OK
```

不过为了统一，无论什么类型，`new`和`delete`配对，`new[]`和`delete[]`配对，就能避免错误。

