#include "Decoder.h"

#include <cstdio>
//#include <algorithm>

#define SWAP(x, y, t) do{\
	(t) = (x);\
	(x) = (y);\
	(y) = (t);\
} while(0);


Decoder::Decoder()
	: content_(nullptr), length_(0)
{

}

Decoder::~Decoder()
{
	if (content_)
		delete[] content_;
}


// 解码
bool Decoder::decode()
{
	if (!content_) {
		printf("Decode error! No content to be decoded!\n");
		return false;
	}

	// 排序，按照pair.index从小到大
	sort_(content_, 0, length_ - 1);

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
		putchar(content_[i].character);
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
		delete[] content_;
	content_ = new Pair[length_ + 1];	// 有意多申请一个空间，为快速排序准备（最后一个数据的位置始终不变）

	for (int i = 0; i < length_; ++i) {
		fread(&content_[i].character, 1, 1, fp);	// char
		fread(&content_[i].index, 4, 1, fp);		// int
	}

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
		fputc(content_[i].character, fp);
	}

	fclose(fp);
	return true;
}


// 快速排序
void Decoder::sort_(Pair* pair, int left, int right)
{
	if (left >= right)
		return;

	int i = left;
	int j = right + 1;
	int pivot = pair[left].index;	// 枢轴（将数据分为两部分）
	Pair temp;

	do {
		do (i++); while (pair[i].index < pivot);
		do (j--); while (pair[j].index > pivot);
		if (i < j) {
			SWAP(pair[i], pair[j], temp);
		}
	} while (i < j);

	SWAP(pair[j], pair[left], temp);

	// 递归
	sort_(pair, left, j - 1);	// 小于枢轴的部分
	sort_(pair, j + 1, right);	// 大于枢轴的部分
}
