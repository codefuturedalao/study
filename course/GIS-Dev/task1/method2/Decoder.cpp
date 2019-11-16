#include "Decoder.h"

#include <cstdio>
//#include <algorithm>


Decoder::Decoder()
	: content_(nullptr), index_(nullptr), length_(0)
{

}

Decoder::~Decoder()
{
	if (content_)
		delete content_;
	if (index_)
		delete index_;
}


// 解码
bool Decoder::decode()
{
	if (!content_) {
		printf("Decode error! No content to be decoded!\n");
		return false;
	}

	// 排序，按照index从小到大排序
	sort_(index_, content_, 0, length_ - 1);

	//// 或者调用 std::sort()
	//std::sort(content_, content_ + length_, [](Pair pair1, Pair pair2) {
	//	return pair1.index < pair2.index;
	//});

	return true;
}


// 打印到控制台
void Decoder::print()
{
	if (!content_)
		return;

	for (int i = 0; i < length_; ++i) 
		putchar(content_[i]);
}


// 读取文件
bool Decoder::read(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp) {
		printf("Read error! Open file failed!\n");
		return false;
	}

	// 获取文件长度
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);	
	rewind(fp);

	// 文件长度必须是5的倍数
	if (length % 5 != 0) {
		printf("Read error! Format error!\n");
		fclose(fp);
		return false;
	}
	else {
		length_ = length / 5;		// char+int 一对儿，5个字节
	}

	if (content_)
		delete content_;
	if (index_)
		delete index_;
	content_ = new char[length_ + 1];	// 有意多申请一个空间，为快速排序准备（最后一个数据的位置始终不变）
	index_ = new int[length_ + 1];

	for (int i = 0; i < length_; ++i) {
		fread(content_ + i, 1, 1, fp);	// char
		fread(index_ + i, 4, 1, fp);	// int
	}
	content_[length_] = 0;
	index_[length_] = 0x7FFFFFFF;

	fclose(fp);
	return true;
}


// 写入文件
bool Decoder::write(const char* fileName)
{
	if (!content_) {
		printf("Write error! No content to write!\n");
		return false;
	}

	FILE* fp = fopen(fileName, "wb+");
	if (!fp) {
		printf("Write error! Open file failed!\n");
		return false;
	}

	for (int i = 0; i < length_; ++i) {
		fputc(content_[i], fp);
	}

	fclose(fp);
	return true;
}


// 快速排序
// 按照intArr升序（同时charArr也对应升序）
void Decoder::sort_(int* intArr, char* charArr, int left, int right)
{
	if (left >= right)
		return;

	int i = left;
	int j = right + 1;
	int pivot = intArr[left];	// 枢轴（将数据分为两部分）

	do {
		do (i++); while (intArr[i] < pivot);
		do (j--); while (intArr[j] > pivot);
		if (i < j) {
			swap_(intArr[i], intArr[j]);
			swap_(charArr[i], charArr[j]);
		}
	} while (i < j);

	swap_(intArr[j], intArr[left]);
	swap_(charArr[j], charArr[left]);

	// 递归
	sort_(intArr, charArr, left, j - 1);	// 小于枢轴的部分
	sort_(intArr, charArr, j + 1, right);	// 大于枢轴的部分
}
