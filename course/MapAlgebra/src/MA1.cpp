#include "MA1.h"
#include "bmp.h"

#include <cstdio>
#include <ctime>
#include <windows.h>

#include <algorithm>
#include <vector>

#pragma warning(disable : 4996)

#define SCHEME		1
#define BIG_NUM		999999999

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


// 反色处理（索引值），16色
// 要求调色板必须是"标准"调色板（ i 和 size - i 处表示的颜色互为反色）
bool Bmp16ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode inverseMode/* = InverseColorTable*/)
{
	// 打开源Bmp文件
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source filen\n");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	uchar* clrTab = nullptr;
	uchar** mtxBuf = nullptr;
	uint clrTabeSize = 0, mtxWidth = 0, mtxHeight = 0;

	BmpRead(fpSrc, &bmpFileHeader, &bmpInfoHeader,
		&clrTab, &clrTabeSize,
		&mtxBuf, &mtxWidth, &mtxHeight);

	uint bmpWidth = bmpInfoHeader.biWidth;
	uint bmpHeight = bmpInfoHeader.biHeight;

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", mtxWidth, mtxHeight);

	switch (inverseMode) {
	default:
		break;
	case InverseColorTable:
		// 反色调色板
		for (uint i = 0; i < 16; ++i) {
			clrTab[i * 4] = 255 - clrTab[i * 4];
			clrTab[i * 4 + 1] = 255 - clrTab[i * 4 + 1];
			clrTab[i * 4 + 2] = 255 - clrTab[i * 4 + 2];
		}
		break;
	case InverseColorIndex:
		// 写入文件头，bmp信息头，调色板
		for (uint i = 0, halfBmpWidth = bmpWidth / 2; i < mtxHeight; ++i) {
			for (uint j = 0; j < halfBmpWidth; ++j) {
				int first = mtxBuf[i][j] / 16;
				int second = mtxBuf[i][j] % 16;
				first = 15 - first;
				second = 15 - second;
				mtxBuf[i][j] = first * 16 + second;
			}
			// 宽度为奇数
			if (bmpWidth % 2 == 1) {
				int first = mtxBuf[i][halfBmpWidth] / 16;
				first = 15 - first;
				mtxBuf[i][halfBmpWidth] = first * 16;
			}
		}
		break;
	}

	// 写入文件
	BmpWrite(destBmpName, &bmpFileHeader, &bmpInfoHeader,
		clrTab, 64,
		mtxBuf, mtxWidth, mtxHeight);

	// 清理工作
	CleanUp(&fpSrc, &clrTab, &mtxBuf, mtxHeight);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");
	return true;
}


// 反色处理（索引值） 8bit 256色
// 要求调色板必须是"标准"调色板（ i 和 size - i 处表示的颜色互为反色）
bool Bmp256ColorInverse(const char* srcBmpName, const char* destBmpName, InverseMode inverseMode/* = InverseColorTable*/)
{
	// 打开源Bmp文件
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source file");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	uchar* clrTab = nullptr;
	uchar** mtxBuf = nullptr;
	uint clrTabeSize = 0, mtxWidth = 0, mtxHeight = 0;

	BmpRead(fpSrc, &bmpFileHeader, &bmpInfoHeader,
		&clrTab, &clrTabeSize,
		&mtxBuf, &mtxWidth, &mtxHeight);

	uint bmpWidth = bmpInfoHeader.biWidth;
	uint bmpHeight = bmpInfoHeader.biHeight;

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", mtxWidth, mtxHeight);

	switch (inverseMode) {
	default:
		break;
	case InverseColorTable:
		// 反转调色板
		for (uint i = 0; i < 256; ++i) {
			clrTab[i * 4] = 255 - clrTab[i * 4];
			clrTab[i * 4 + 1] = 255 - clrTab[i * 4 + 1];
			clrTab[i * 4 + 2] = 255 - clrTab[i * 4 + 2];
		}
		break;
	case InverseColorIndex:
		// 修改颜色索引
		for (uint i = 0; i < mtxHeight; ++i) {
			for (int j = 0, len = bmpInfoHeader.biWidth; j < len; ++j) {
				mtxBuf[i][j] = 255 - mtxBuf[i][j];
			}
		}
		break;
	}

	// 写入到文件
	BmpWrite(destBmpName, &bmpFileHeader, &bmpInfoHeader,
		clrTab, 1024,
		mtxBuf, mtxWidth, mtxHeight);

	// 清理工作
	CleanUp(&fpSrc, &clrTab, &mtxBuf, mtxHeight);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");
	return true;
}


//	反色处理，24bit
bool Bmp24BitInverse(const char* srcBmpName, const char* destBmpName)
{
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source file\n");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	uchar** mtxBuf = nullptr;
	uint mtxWidth = 0, mtxHeight = 0;

	BmpRead(fpSrc, &bmpFileHeader, &bmpInfoHeader,
		nullptr, nullptr,
		&mtxBuf, &mtxWidth, &mtxHeight);

	uint bmpWidth = bmpInfoHeader.biWidth;
	uint bmpHeight = bmpInfoHeader.biHeight;

	LOG("Inverting color...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", mtxWidth, mtxHeight);

	// 读取数据并反色处理
	for (uint i = 0; i < mtxHeight; ++i) {
		for (uint j = 0, len = bmpWidth * 3; j < len; j += 3) {
			mtxBuf[i][j]   = 255 - mtxBuf[i][j];
			mtxBuf[i][j+1] = 255 - mtxBuf[i][j+1];
			mtxBuf[i][j+2] = 255 - mtxBuf[i][j+2];
		}
	}

	BmpWrite(destBmpName, &bmpFileHeader, &bmpInfoHeader,
		nullptr, 0,
		mtxBuf, mtxWidth, mtxHeight);

	CleanUp(&fpSrc, nullptr, &mtxBuf, mtxHeight);

	LOG("Invert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");
	return true;
}


// 24bit 真彩色 转为 256色
bool Bmp24BitTo256Color(const char* srcBmpName, const char* destBmpName)
{
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source file\n");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	uchar** mtxBuf = nullptr;
	uint mtxWidth = 0, mtxHeight = 0;

	BmpRead(fpSrc, &bmpFileHeader, &bmpInfoHeader,
		nullptr, nullptr,
		&mtxBuf, &mtxWidth, &mtxHeight);

	uint bmpWidth = bmpInfoHeader.biWidth;
	uint bmpHeight = bmpInfoHeader.biHeight;
	uint pixelCount = bmpHeight * bmpWidth;		// 总像素数目

	// 存储所有像素点的颜色值（取BGR值的高4位组成12bit整数）
	uint* clrBuf = new uint[pixelCount];
	for (uint i = 0; i < mtxHeight; ++i) {
		for (uint j = 0; j < bmpWidth; ++j) {
			uchar bValue = mtxBuf[i][j * 3] & 0xf0;
			uchar gValue = mtxBuf[i][j * 3 + 1] & 0xf0;
			uchar rValue = mtxBuf[i][j * 3 + 2] & 0xf0;
			clrBuf[i*bmpWidth + j] = (bValue << 4) + gValue + (rValue >> 4);
		}
	}

	LOG("Converting 24bit bmp to 256 color bmp...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", mtxWidth, mtxHeight);

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
		clrCountVec.push_back(std::make_pair(colorValue, count));

		if (j >= pixelCount) {
			break;
		}
	}

	// 对clrCountVec按count值排序
	std::sort(clrCountVec.begin(), clrCountVec.end(), [](const std::pair<int, int>& x, const std::pair<int, int>& y) { 
		return x.second > y.second; 
	});

#if SCHEME==1	// 方案一  稍微慢

	// 将出现频率最大的256中颜色作为调色板
	uchar clrTab[1024];		// 拆分为 R G B α
	for (uint i = 0, vecSize = clrCountVec.size(); i < 256; ++i) {
		if (i < vecSize) {
			uint color = clrCountVec[i].first;
			clrTab[i * 4] = (color & 0xf00) >> 4;	clrTab[i * 4 + 1] = color & 0x0f0;
			clrTab[i * 4 + 2] = (color & 0x00f) << 4;	clrTab[i * 4 + 3] = 0x0;
		}
		else {
			clrTab[i * 4] = 0x0;	clrTab[i * 4 + 1] = 0x0;
			clrTab[i * 4 + 2] = 0x0;	clrTab[i * 4 + 3] = 0x0;
		}
	}

	// 获得输出图像（256Color）的像素矩阵
	uint  mtx256ColorWidth = (bmpWidth + 3) / 4 * 4;
	uint  mtx256ColorHeight = bmpHeight;
	uchar** mtx256ColorBuf = new uchar*[mtx256ColorHeight];		// 存储调色板中的索引值
	for (int i = 0; i < mtx256ColorHeight; ++i) {
		mtx256ColorBuf[i] = new uchar[mtx256ColorWidth];
	}

	// 分别取颜色表中颜色，与剩余颜色信息进行最小平方误差计算。
	uint errorMin = 0;
	uint error = 0;
	uint index = 0;
	for (uint i = 0; i < bmpHeight; ++i) {
		for (uint j = 0; j < bmpWidth; ++j) {
			errorMin = 9999999;
			index = 0;
			// 计算最小平方误差
			for (int k = 0; k < 256; ++k) {
				uint bValueErr = (mtxBuf[i][j * 3] & 0xf0) - clrTab[k * 4];
				uint gValueErr = (mtxBuf[i][j * 3 + 1] & 0xf0) - clrTab[k * 4 + 1];
				uint rValueErr = (mtxBuf[i][j * 3 + 2] & 0xf0) - clrTab[k * 4 + 2];
				// 如果在颜色表中（即误差均为0），结束循环
				if (bValueErr == 0 && gValueErr == 0 && rValueErr == 0) {
					index = k;
					break;
				}
				else if ((error = bValueErr * bValueErr + gValueErr * gValueErr + rValueErr * rValueErr) < errorMin) {
					errorMin = error;
					index = k;
				}
			}
			// 输出8bit的bmp图像（256色）
			mtx256ColorBuf[i][j] = index;

			//// 输出24bit的bmp图像，但是只有256种颜色
			//mtxBuf[i][j * 3]	   = clrTab[index * 4];
			//mtxBuf[i][j * 3 + 1] = clrTab[index * 4 + 1];
			//mtxBuf[i][j * 3 + 2] = clrTab[index * 4 + 2];
		}
		// 如果行像素数不是4的倍数，补零
		for (uint j = bmpWidth; j < mtx256ColorWidth; ++j) {
			mtx256ColorBuf[i][j] = 0;
		}
	}

#elif scheme==2		// 方案二：

	// 将出现频率最大的256中颜色作为调色板
	uint clrTabUi[256];		// 调色板每种颜色rgb组成的unsigned int值
	uchar clrTab[1024];		// 调色板每种颜色r,g,b,α
	for (uint i = 0, vecSize = clrCountVec.size(), color = 0; i < 256; ++i) {
		if (i < vecSize) {
			color = clrCountVec[i].first;
			clrTabUi[i] = color;
			clrTab[i * 4] = (color & 0xf00) >> 4;
			clrTab[i * 4 + 1] = color & 0x0f0;
			clrTab[i * 4 + 2] = (color & 0x00f) << 4;
			clrTab[i * 4 + 3] = 0x0;
		} else {
			clrTabUi[i] = 0x0;
			clrTab[i * 4] = 0x0;
			clrTab[i * 4 + 1] = 0x0;
			clrTab[i * 4 + 2] = 0x0;
			clrTab[i * 4 + 3] = 0x0;
		}
	}

	// 输出图像（256Color）的像素矩阵
	uint  mtx256ColorWidth = (bmpWidth + 3) / 4 * 4;
	uint  mtx256ColorHeight = bmpHeight;
	uchar** mtx256ColorBuf = new uchar*[mtx256ColorHeight];		// 存储调色板中的索引值
	for (uint i = 0; i < mtx256ColorHeight; ++i)
		mtx256ColorBuf[i] = new uchar[mtx256ColorWidth];

	// 分别取颜色表中颜色，与剩余颜色信息进行最小平方误差计算。
	uint errorMin = 0;
	uint error = 0;
	uint index = 0;

	// 颜色映射 oldColor(12bit number bwtween 0~4095) ==> newColorIndexInClrTab
	// 每个下标对应一种颜色
	uint color4096Map[4096];		
	for (int i = 0; i < 4096; ++i) 
		color4096Map[i] = 0;
	//std::map<uint, uint> clrMap;	// 颜色映射 <oldColor, newColorIndexInClrTab>	very slow...
	//std::vector<std::pair<uint, uint>> clrMap;	// very very slow

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
			color = ((mtxBuf[i][j * 3] & 0xf0) << 4) + (mtxBuf[i][j * 3 + 1] & 0xf0) + ((mtxBuf[i][j * 3 + 2] & 0xf0) >> 4);
			mtx256ColorBuf[i][j] = color4096Map[color];		// clrMap  oldColor-->newColorIndexInClrTab 一一映射
		}
		// 如果行像素数不是4的倍数，补零
		for (uint j = bmpWidth; j < mtx256ColorWidth; ++j) {
			mtx256ColorBuf[i][j] = 0;
		}
	}

#endif // scheme==1

	// 新文件(256色位图）
	BITMAPFILEHEADER bmp256ColorFileHeader = bmpFileHeader;
	BITMAPINFOHEADER bmp256ColorInfoHeader = bmpInfoHeader;

	bmp256ColorFileHeader.bfOffBits = 14 + 40 + 1024;
	bmp256ColorFileHeader.bfSize = bmp256ColorFileHeader.bfOffBits + mtx256ColorWidth * mtx256ColorHeight;
	bmp256ColorInfoHeader.biBitCount = 8;

	// 输出8bit的bmp图像，256种颜色
	BmpWrite(destBmpName, &bmp256ColorFileHeader, &bmp256ColorInfoHeader,
		clrTab, 1024,
		mtx256ColorBuf, mtx256ColorWidth, mtx256ColorHeight);

	//// 输出24bit的bmp图像，但是只有256种颜色
	//BmpWrite(destBmpName, &bmpFileHeader, &bmpInfoHeader,
	//	NULL, 0,
	//	mtxBuf, mtxWidth, mtxHeight);

	CleanUp(&fpSrc, nullptr, &mtxBuf, mtxHeight, &mtx256ColorBuf, mtx256ColorHeight);

	LOG("Convert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");

	return true;
}


// 256色 转 32 bit
bool Bmp256ColorTo32Bit(const char* srcBmpName, const char* destBmpName)
{
	FILE* fpSrc = fopen(srcBmpName, "rb");
	if (!fpSrc) {
		printf("Failed to open source file\n");
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	uchar* clrTab = nullptr;
	uchar** mtxBuf = nullptr;
	uint clrTabeSize = 0, mtxWidth = 0, mtxHeight = 0;

	BmpRead(fpSrc, &bmpFileHeader, &bmpInfoHeader,
		&clrTab, &clrTabeSize,
		&mtxBuf, &mtxWidth, &mtxHeight);

	uint bmpWidth = bmpInfoHeader.biWidth;
	uint bmpHeight = bmpInfoHeader.biHeight;
	uint pixelCount = bmpHeight * bmpWidth;		// 总像素数目

	LOG("Converting 256 color bmp to 32bit bmp...\n");
	LOG("bmp file size:\t %ux%u\n", bmpWidth, bmpHeight);
	LOG("matrix size:\t %ux%u\n", mtxWidth, mtxHeight);

	// 32bit 位图
	uint mtx32BitWidth = bmpWidth * 4;
	uint mtx32BitHeight = bmpHeight;
	uchar** mtx32BitBuf = new uchar*[mtx32BitHeight];

	// 填充数据区
	uchar colorIndex = 0;
	for (uint i = 0; i < mtxHeight; ++i) {
		mtx32BitBuf[i] = new uchar[mtx32BitWidth];
		for (uint j = 0; j < bmpWidth; ++j) {
			colorIndex = mtxBuf[i][j];
			mtx32BitBuf[i][j * 4] = clrTab[colorIndex * 4];
			mtx32BitBuf[i][j * 4 + 1] = clrTab[colorIndex * 4 + 1];
			mtx32BitBuf[i][j * 4 + 2] = clrTab[colorIndex * 4 + 2];
			mtx32BitBuf[i][j * 4 + 3] = clrTab[colorIndex * 4 + 3];
		}
	}

	// 32bit位图的文件头和信息头
	bmpFileHeader.bfOffBits = 14 + 40;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + pixelCount * 4;	// 每个像素4个字节
	bmpInfoHeader.biBitCount = 32;

	BmpWrite(destBmpName, &bmpFileHeader, &bmpInfoHeader, nullptr, 0,
		mtx32BitBuf, mtx32BitWidth, mtx32BitHeight);

	CleanUp(&fpSrc, &clrTab, &mtx32BitBuf, mtx32BitHeight);

	LOG("Convert Successfully! %s\n", srcBmpName);
	LOG("---------------------------------------------------------------------\n");

	return true;
}


// 清理工作
void CleanUp(FILE** fp, uchar** clrTab/* = nullptr*/,
	uchar*** mtxBuf1/* = nullptr*/, uint mtxHeight1/* = 0*/,
	uchar*** mtxBuf2/* = nullptr*/, uint mtxHeight2/* = 0*/)
{
	if (fp && *fp) {
		fclose(*fp);
		*fp = nullptr;
	}

	if (clrTab && *clrTab) {
		delete *clrTab;
		*clrTab = nullptr;
	}

	if (mtxBuf1 && *mtxBuf1) {
		for (uint i = 0; i < mtxHeight1; ++i) {
			if ((*mtxBuf1)[i]) {
				delete[] (*mtxBuf1)[i];
				(*mtxBuf1)[i] = nullptr;
			}
		}
		delete[] *mtxBuf1;
		*mtxBuf1 = nullptr;
	}

	if (mtxBuf2 && *mtxBuf2) {
		for (uint i = 0; i < mtxHeight2; ++i) {
			if ((*mtxBuf2)[i]) {
				delete[] (*mtxBuf2)[i];
				(*mtxBuf2)[i] = nullptr;
			}
		}
		delete[] *mtxBuf2;
		*mtxBuf2 = nullptr;
	}
}


