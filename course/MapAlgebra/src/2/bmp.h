#pragma once

#include <windows.h>
#include <cstdio>
#include <iostream>

using uint  = unsigned int;
using uchar = unsigned char;

#define LogF(fmt, ...) printf(##fmt"(%s)<%d> <%s>\n", ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__) 
#define Log(x) std::cout << x << " (" << __FILE__ << ")<" << __LINE__ << "> <" << __FUNCTION__ << ">" << std::endl;

class Bmp 
{
public:
	Bmp() {}
	Bmp(uint bitCount, uint biWidth, uint biHeight);
	Bmp(const Bmp& rhs);
	~Bmp();

	bool read(const char* filename);
	bool write(const char* filename);
	uint width() const { return infoHeader.biWidth; }
	uint height() const { return infoHeader.biHeight; }
	uint bitCount() const { return infoHeader.biBitCount; }

	bool copyMtxTo(uchar*** destMtx, uint* destMtxWidth, uint* destMtxHeight);		// 拷贝this->mtx到destMtx
	bool copyMtxFrom(uchar** srcMtx, uint destMtxWidth, uint destMtxHeight);		// 拷贝srcMtx到this->mtx
	bool copyClrTabFrom(uchar* srcClrTab, uint size);		// 拷贝srcClrTab到this->clrTab

public:
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	uchar* clrTab = nullptr;
	uint clrTabSize = 0;

	uchar** mtx = nullptr;
	uint mtxWidth = 0;
	uint mtxHeight = 0;

private:
	void cleanup_();
	uint getClrTabSize_(uint biCount) const;
	uint getMtxWidth_(uint biCount, uint biWidth) const;
	void headerFit_(uint biCount, uint biWidth, uint biHeight);
};


// 灰度化
bool BmpGray(const char* src256ColorBmpName, const char* destBmpName);

// 二值化
bool BmpThreshold(const char* srcGrayBmpName, const char* destBmpName, int thres);



// 清理工作
//void CleanUp(FILE** fp, uchar** clrTab = nullptr,
//	uchar*** mtxBuf1 = nullptr, uint mtxHeight1 = 0,
//	uchar*** mtxBuf2 = nullptr, uint mtxHeight2 = 0);
//
// 写入bmp数据到文件
//bool BmpWrite(const char* fileName, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
//	uchar* clrTab, uint clrTabSize,
//	uchar** mtxBuf, uint mtxWidth, uint mtxHeight);
//
//// 读取bmp文件
//bool BmpRead(FILE* fp, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
//	uchar**  clrTab = nullptr, uint* clrTabSize = nullptr,
//	uchar*** mtxBuf = nullptr, uint* mtxWidth = nullptr, uint* mtxHeight = nullptr);