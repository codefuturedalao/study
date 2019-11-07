#pragma once


#include <windows.h>
#include <cstdio>

using uint  = unsigned int;
using uchar = unsigned char;

// 写入bmp数据到文件
bool BmpWrite(const char* fileName, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
	uchar* clrTab, uint clrTabSize,
	uchar** mtxBuf, uint mtxWidth, uint mtxHeight);

// 读取bmp文件
bool BmpRead(FILE* fp, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
	uchar**  clrTab = nullptr, uint* clrTabSize = nullptr,
	uchar*** mtxBuf = nullptr, uint* mtxWidth = nullptr, uint* mtxHeight = nullptr);