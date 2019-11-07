#include "bmp.h"

#pragma warning(disable : 4996)

// using uint  = unsigned int;
// using uchar = unsigned char;


// 读取bmp文件
bool BmpRead(FILE* fp, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
	uchar** clrTab/* = nullptr*/, uint* clrTabSize/* = nullptr*/,
	uchar*** mtxBuf/* = nullptr*/, uint* mtxWidth/* = nullptr*/, uint* mtxHeight /*= nullptr*/)
{
	if (!fp)
		return false;

	fread(bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	uint bmpWidth = bmpInfoHeader->biWidth;
	uint bmpHeight = bmpInfoHeader->biHeight;

	uchar* lineBuf = nullptr;

	switch (bmpInfoHeader->biBitCount) {
	default:
		break;
	case 1:
		break;
	case 4:
	{
		if (clrTabSize)
			*clrTabSize = 64;

		if (clrTab) {
			if (!(*clrTab))
				*clrTab = new uchar[64];
			fread(*clrTab, 1, 64, fp);
		} else {
			uchar tempClrTab[1024];
			fread(tempClrTab, 1, 1024, fp);
		}

		if (mtxWidth)
			*mtxWidth = bmpWidth % 2 == 0 ?
				(bmpWidth / 2 + 3) / 4 * 4
				: (bmpWidth / 2 + 4) / 4 * 4;
		if (mtxHeight)
			*mtxHeight = bmpHeight;

		if (!mtxBuf)
			break;

		// 读取像素矩阵
		lineBuf = new uchar[*mtxWidth];	// 行缓冲
		if (!(*mtxBuf))
			*mtxBuf = new uchar*[*mtxHeight];	// 像素矩阵（包括补零）

		for (uint i = 0; i < *mtxHeight; ++i) {
			fread(lineBuf, sizeof(uchar), *mtxWidth, fp);
			(*mtxBuf)[i] = new uchar[*mtxWidth];
			for (uint j = 0; j < *mtxWidth; ++j) {
				((*mtxBuf)[i])[j] = lineBuf[j];
			}
		}

		break;
	}
	case 8:		// 8bit, 256color
	{
		if (clrTabSize)
			*clrTabSize = 1024;

		if (clrTab) {
			if (!(*clrTab))
				*clrTab = new uchar[1024];
			fread(*clrTab, 1, 1024, fp);
		} else {
			uchar tempClrTab[1024];
			fread(tempClrTab, 1, 1024, fp);
		}

		if (mtxWidth)
			*mtxWidth = (bmpWidth + 3) / 4 * 4;
		if (mtxHeight)
			*mtxHeight = bmpHeight;

		if (!mtxBuf)
			break;

		lineBuf = new uchar[*mtxWidth];	// 行缓冲
		if (mtxBuf && !(*mtxBuf))
			*mtxBuf = new uchar*[*mtxHeight];	// 像素矩阵（包括补零）

		for (uint i = 0; i < *mtxHeight; ++i) {
			// 行缓冲
			fread(lineBuf, sizeof(uchar), *mtxWidth, fp);
			(*mtxBuf)[i] = new uchar[*mtxWidth];
			// 像素区域
			for (uint j = 0; j < *mtxWidth; ++j) {
				((*mtxBuf)[i])[j] = lineBuf[j];
			}
		}
		break;
	}
	case 16:
		break;
	case 24:
	{
		if (mtxWidth)
			*mtxWidth = (bmpWidth * 3 + 3) / 4 * 4;
		if (mtxHeight)
			*mtxHeight = bmpHeight;

		if (!mtxBuf)
			break;

		lineBuf = new uchar[*mtxWidth];	// 行缓冲
		if (!(*mtxBuf))
			*mtxBuf = new uchar*[*mtxHeight];	// 像素矩阵

		// 读取数据并反色处理
		for (uint i = 0; i < *mtxHeight; ++i) {
			fread(lineBuf, sizeof(uchar), *mtxWidth, fp);
			(*mtxBuf)[i] = new uchar[*mtxWidth];
			for (uint j = 0; j < *mtxWidth; ++j) {
				((*mtxBuf)[i])[j] = lineBuf[j];
			}
		}

		break;
	}
	case 32:
		break;
	}

	if (lineBuf)
		delete lineBuf;

	return true;
}


// 写入bmp数据到文件
bool BmpWrite(const char* fileName, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader,
	uchar* clrTab, uint clrTabSize,
	uchar** mtxBuf, uint mtxWidth, uint mtxHeight)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Failed to open dest file\n");
		return false;
	}

	// 写入文件头，bmp信息头，调色板
	fwrite(bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	// 是否有调色板
	if (clrTabSize > 0)
		fwrite(clrTab, clrTabSize, 1, fp);

	// 写入数据
	for (uint i = 0; i < mtxHeight; ++i)
		fwrite(mtxBuf[i], mtxWidth, 1, fp);

	fclose(fp);
	return true;
}