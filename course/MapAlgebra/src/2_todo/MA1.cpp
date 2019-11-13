#include "MA1.h"
#include "bmp.h"

#include <cstdio>
#include <ctime>
#include <windows.h>

#include <algorithm>
#include <vector>

#pragma warning(disable : 4996)

//#define NDEBUG
#define BIG_NUM	 999999999

#ifndef NDEBUG
#define LOG(...) printf(__VA_ARGS__)
#else 
#define LOG(...)
#endif  

// using uint = unsigned int;
// using uchar = unsigned char;

// 反色处理，所有bmp类型，
bool BmpInverse(const char* srcBmpName, const char* destBmpName, InverseMode mode/* = InverseColorTable*/)
{
	// 打开源Bmp文件
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source file");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	// 读取文件头，bmp信息头，调色板
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fpSrc);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fpSrc);
	fclose(fpSrc);
	fpSrc = NULL;

	bool ret = false;
	switch (bmpInfoHeader.biBitCount) {
	default:
		break;
	case 1:
		break;
	case 4:
		ret = Bmp16ColorInverse(srcBmpName, destBmpName, mode);
		break;
	case 8:
		ret = Bmp256ColorInverse(srcBmpName, destBmpName, mode);
		break;
	case 16:
		break;
	case 24:
		ret = Bmp24BitInverse(srcBmpName, destBmpName);
		break;
	case 32:
		break;
	}

	return ret;
}


// 反色处理，4bit 16色
// （InverseColorIndex）要求调色板必须是"标准"调色板（ i 和 size - i 处表示的颜色互为反色）
bool Bmp16ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode inverseMode/* = InverseColorTable*/)
{
	Bmp bmp;
	if (!bmp.read(srcBmpName)) {
		printf("Failed to open source file\n");
		return false;
	}

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmp.width(), bmp.height());
	LOG("matrix size:\t %ux%u\n", bmp.mtxWidth, bmp.mtxHeight);

	switch (inverseMode) {
	default:
		break;
	case InverseColorTable:
		// 反色调色板
		for (uint i = 0; i < 16; ++i) {
			bmp.clrTab[i * 4] = 255 - bmp.clrTab[i * 4];
			bmp.clrTab[i * 4 + 1] = 255 - bmp.clrTab[i * 4 + 1];
			bmp.clrTab[i * 4 + 2] = 255 - bmp.clrTab[i * 4 + 2];
		}
		break;
	case InverseColorIndex:
		// 写入文件头，bmp信息头，调色板
		for (uint i = 0, halfBmpWidth = bmp.width() / 2; i < bmp.mtxHeight; ++i) {
			for (uint j = 0; j < halfBmpWidth; ++j) {
				int first = bmp.mtx[i][j] / 16;
				int second = bmp.mtx[i][j] % 16;
				first = 15 - first;
				second = 15 - second;
				bmp.mtx[i][j] = first * 16 + second;
			}
			// 宽度为奇数
			if (bmp.width() % 2 == 1) {
				int first = bmp.mtx[i][halfBmpWidth] / 16;
				first = 15 - first;
				bmp.mtx[i][halfBmpWidth] = first * 16;
			}
		}
		break;
	}

	bmp.write(destBmpName);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");

	return true;
}


// 反色处理 8bit 256色
// （InverseColorIndex）要求调色板必须是"标准"调色板（ i 和 size - i 处表示的颜色互为反色）
bool Bmp256ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode inverseMode/* = InverseColorTable*/)
{
	Bmp bmp;
	if (!bmp.read(srcBmpName)) {
		printf("Failed to open source file\n");
		return false;
	}

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmp.width(), bmp.height());
	LOG("matrix size:\t %ux%u\n", bmp.mtxWidth, bmp.mtxHeight);

	switch (inverseMode) {
	default:
		break;
	case InverseColorTable:
		// 反转调色板
		for (uint i = 0; i < 256; ++i) {
			bmp.clrTab[i * 4] = 255 - bmp.clrTab[i * 4];
			bmp.clrTab[i * 4 + 1] = 255 - bmp.clrTab[i * 4 + 1];
			bmp.clrTab[i * 4 + 2] = 255 - bmp.clrTab[i * 4 + 2];
		}
		break;
	case InverseColorIndex:
		// 修改颜色索引
		for (uint i = 0; i < bmp.mtxHeight; ++i) {
			for (int j = 0, width = bmp.width(); j < width; ++j) {
				bmp.mtx[i][j] = 255 - bmp.mtx[i][j];
			}
		}
		break;
	}

	bmp.write(destBmpName);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");
	return true;
}


//	反色处理，24bit
bool Bmp24BitInverse(const char* srcBmpName, const char* destBmpName)
{
	Bmp bmp;
	if (!bmp.read(srcBmpName)) {
		printf("Failed to open source file\n");
		return false;
	}

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmp.width(), bmp.height());
	LOG("matrix size:\t %ux%u\n", bmp.mtxWidth, bmp.mtxHeight);

	// 读取数据并反色处理
	for (uint i = 0; i < bmp.mtxHeight; ++i) {
		for (uint j = 0, len = bmp.width() * 3; j < len; j += 3) {
			bmp.mtx[i][j]   = 255 - bmp.mtx[i][j];
			bmp.mtx[i][j+1] = 255 - bmp.mtx[i][j+1];
			bmp.mtx[i][j+2] = 255 - bmp.mtx[i][j+2];
		}
	}

	bmp.write(destBmpName);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");
	return true;
}


// 24bit 真彩色 转为 256色
bool Bmp24BitTo256Color(const char* srcBmpName, const char* destBmpName)
{
	Bmp bmpSrc;
	if (!bmpSrc.read(srcBmpName)) {
		printf("Failed to open source file\n");
		return false;
	}

	LOG("Converting 24bit bmp to 256 color bmp...\n");
	LOG("bmp file size:\t %ux%u\n", bmpSrc.width(), bmpSrc.height());
	LOG("matrix size:\t %ux%u\n", bmpSrc.mtxWidth, bmpSrc.mtxHeight);

	uint bmpWidth = bmpSrc.width();
	uint bmpHeight = bmpSrc.height();
	uint pixelCount = bmpWidth * bmpHeight;		// 总像素数目

	// 输出图像（256Color）
	Bmp bmpDest(8, bmpWidth, bmpHeight);

	// 存储所有像素点的颜色值（取BGR值的高4位组成12bit整数）
	uint* clrBuf = new uint[pixelCount];
	for (uint i = 0; i < bmpSrc.mtxHeight; ++i) {
		for (uint j = 0; j < bmpWidth; ++j) {
			uchar bValue = bmpSrc.mtx[i][j * 3] & 0xf0;
			uchar gValue = bmpSrc.mtx[i][j * 3 + 1] & 0xf0;
			uchar rValue = bmpSrc.mtx[i][j * 3 + 2] & 0xf0;
			clrBuf[i*bmpWidth + j] = (bValue << 4) + gValue + (rValue >> 4);
		}
	}

	// 统计4096种颜色出现次数最多的前256种颜色
	// 按颜色值从大到小排序
	std::sort(clrBuf, clrBuf + pixelCount, [](uint x, uint y) { return x > y; });

	// 统计每种颜色出现的次数
	std::vector<std::pair<int, int>> clrCountVec;		// pair<颜色值，出现次数>
	uint colorValue = 0;
	uint count = 0;
	for (uint i = 0, j = 0; i < pixelCount; ++i) {
		colorValue = clrBuf[j];
		count = 1;
		j++;
		while (clrBuf[j] == colorValue) {
			++count;
			if (++j >= pixelCount) {
				break;
			}
		}
		clrCountVec.emplace_back(colorValue, count);

		if (j >= pixelCount) {
			break;
		}
	}

	// 对clrCountVec按count值排序
	std::sort(clrCountVec.begin(), clrCountVec.end(), [](const std::pair<int, int>& x, const std::pair<int, int>& y) {
		return x.second > y.second;
	});

	// 将出现频率最大的256中颜色作为调色板
	uint clrTabUi[256];		// 调色板每种颜色rgb组成的unsigned int值
	uint vecSize = clrCountVec.size();
	for (uint i = 0, color = 0; i < vecSize && i < 256; ++i) {
		color = clrCountVec[i].first;
		clrTabUi[i] = color;
		bmpDest.clrTab[i * 4] = (color & 0xf00) >> 4;
		bmpDest.clrTab[i * 4 + 1] = color & 0x0f0;
		bmpDest.clrTab[i * 4 + 2] = (color & 0x00f) << 4;
		bmpDest.clrTab[i * 4 + 3] = 0x0;
	}
	for (uint i = vecSize; i < 256; ++i) {
		clrTabUi[i] = 0x0;
		bmpDest.clrTab[i * 4] = 0x0;
		bmpDest.clrTab[i * 4 + 1] = 0x0;
		bmpDest.clrTab[i * 4 + 2] = 0x0;
		bmpDest.clrTab[i * 4 + 3] = 0x0;
	}

	// 分别取颜色表中颜色，与剩余颜色信息进行最小平方误差计算。
	uint errorMin = 0;
	uint error = 0;
	uint index = 0;

	// 颜色映射 oldColor(12bit number bwtween 0~4095) ==> newColorIndexInClrTab
	// 每个下标对应一种颜色
	uint color4096Map[4096];
	for (int i = 0; i < 4096; ++i)
		color4096Map[i] = 0;

	// 填充颜色映射表color4096Map
	bool flag = false;
	for (uint color = 0; color < 4096; ++color) {
		flag = false;
		errorMin = BIG_NUM;

		// 判断是否在256中颜色中
		for (int j = 0; j < 256; ++j) {
			if (color == clrTabUi[j]) {
				color4096Map[color] = j;
				flag = true;
				break;
			}
		}
		// 如果不在，根据最小平方误差，计算颜色映射（新颜色代替旧颜色）
		if (!flag) {
			for (int j = 0; j < 256; ++j) {
				int bValueErr = ((color & 0xf00) >> 4) - ((clrTabUi[j] & 0xf00) >> 8);
				int gValueErr = (color & 0x0f0) - (clrTabUi[j] & 0x0f0);
				int rValueErr = ((color & 0x00f) << 4) - ((clrTabUi[j] & 0x00f) << 4);
				if ((error = bValueErr * bValueErr + gValueErr * gValueErr + rValueErr * rValueErr) < errorMin) {
					errorMin = error;
					index = j;
				}
			}
			color4096Map[color] = index;
		}
	}

	// 计算每个像素对应的颜色索引
	for (uint i = 0, color = 0; i < bmpHeight; ++i) {
		for (uint j = 0; j < bmpWidth; ++j) {
			color = ((bmpSrc.mtx[i][j * 3] & 0xf0) << 4) + (bmpSrc.mtx[i][j * 3 + 1] & 0xf0) + ((bmpSrc.mtx[i][j * 3 + 2] & 0xf0) >> 4);
			bmpDest.mtx[i][j] = color4096Map[color];		// clrMap  oldColor-->newColorIndexInClrTab 一一映射
		}
		// 如果行像素数不是4的倍数，补零
		for (uint j = bmpWidth; j < bmpDest.mtxWidth; ++j) {
			bmpDest.mtx[i][j] = 0;
		}
	}

	// 输出8bit的bmp图像，256种颜色
	bmpDest.write(destBmpName);

	LOG("Convert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");

	delete[] clrBuf;
	return true;
}


// 256色 转 32 bit
bool Bmp256ColorTo32Bit(const char* srcBmpName, const char* destBmpName)
{
	Bmp bmpSrc;
	bmpSrc.read(srcBmpName);

	uint bmpWidth = bmpSrc.width();
	uint bmpHeight = bmpSrc.height();

	LOG("Converting 256 color bmp to 32bit bmp...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", bmpSrc.mtxWidth, bmpSrc.mtxWidth);

	// 32bit 位图
	Bmp bmpDest(32, bmpWidth, bmpHeight);

	// 填充数据区
	uchar colorIndex = 0;
	for (uint i = 0; i < bmpDest.mtxHeight; ++i) {
		for (uint j = 0; j < bmpWidth; ++j) {
			colorIndex = bmpSrc.mtx[i][j];
			bmpDest.mtx[i][j * 4] = bmpSrc.clrTab[colorIndex * 4];
			bmpDest.mtx[i][j * 4 + 1] = bmpSrc.clrTab[colorIndex * 4 + 1];
			bmpDest.mtx[i][j * 4 + 2] = bmpSrc.clrTab[colorIndex * 4 + 2];
			bmpDest.mtx[i][j * 4 + 3] = bmpSrc.clrTab[colorIndex * 4 + 3];
		}
	}

	bmpDest.write(destBmpName);

	LOG("Convert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");

	return true;
}





