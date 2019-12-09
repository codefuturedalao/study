#pragma once

#include <windows.h>
#include <cstdio>

enum InverseMode {
	InverseColorTable = 1,
	InverseColorIndex = 2
};

using uint = unsigned int;
using uchar = unsigned char;


// 反色处理，所有bmp类型
bool BmpInverse(const char* srcBmpName, const char* destBmpName, InverseMode mode = InverseColorTable);

//	反色处理（索引值），4bit，16色
bool Bmp16ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode mode = InverseColorTable);

//	反色处理（索引值），8bit，256色
bool Bmp256ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode inverseMode = InverseColorTable);

// 反色处理，24bit
bool Bmp24BitInverse(const char* srcBmpName, const char* destBmpName);

// 24bit 真彩色 转为 256色
bool Bmp24BitTo256Color(const char* srcBmpName, const char* destBmpName);

// 256色 转 32 bit
bool Bmp256ColorTo32Bit(const char* srcBmpName, const char* destBmpName);



