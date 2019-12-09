
#include "MA2.h"
#include "bmp.h"
#include "MA1.h"
#include "array2D.hpp"

#include <algorithm>

#pragma warning(disable : 4996)

#define MaxFloat 9999999.0f
#define MinFloat 0.0000001f

#define IsZero(x)  fabs(x) < MinFloat


int DisTrans256(const char* srcBmpName, DistanceTemplate* pDisT, const char* disBmpName, const char* locBmpName)
{
	Bmp bmpSrc;
	bmpSrc.read(srcBmpName);

	int bmpWidth = bmpSrc.width();
	int bmpHeight = bmpSrc.height();
	int mtxWidth = bmpSrc.mtxWidth;

	// 分配场
	uchar** locMtx = nullptr;
	bmpSrc.copyMtxTo(&locMtx, nullptr, nullptr);

	// 距离场
	float** disMtx = New2D<float>(bmpHeight, bmpWidth);

	// 根据分配场的值初始化距离矩阵
	for (uint y = 0; y < bmpHeight; ++y) {
		for (uint x = 0; x < mtxWidth; ++x) {
			if (locMtx[y][x] == 0xff)
				disMtx[y][x] = MaxFloat;
			else
				disMtx[y][x] = 0.0f;
		}
	}

	// 两次扫描计算距离矩阵和分配矩阵；
	// 第一次扫描（左下->右上）
	for (uint y = 0; y < bmpHeight; ++y) {
		for (uint x = 0; x < bmpWidth; ++x) {
			// 遍历所有矩阵位置（距离矩阵值为0跳过）
			if (IsZero(disMtx[y][x]))
				continue;

			float minDis = MaxFloat;
			//对任一（x, y）位置，遍历每一个模板元素
			for (uint i = 0, size = pDisT->getSize() / 2 + 1; i < size; ++i) {
				// 取出当前模板元素的参数（tmpDIS，OFFX，Y）
				int offX = pDisT->getOffX(i);
				int offY = pDisT->getOffY(i);
				float temDis = pDisT->getDis(i);

				// 计算当前模板元素在大矩阵中的位置（CX，CY）
				int CX = x + offX;
				int CY = y + offY;

				// 位置合法性检验
				if (CX < 0 || CX > bmpWidth - 1 || CY < 0 || CY > bmpHeight - 1)
					continue;

				// 计算距离矩阵对应距离值+模板元素的距离值，比较最小值
				if ((disMtx[CY][CX] + temDis) < minDis) {
					// 把当前（x,y）位置的距离值更新为最小值；同时，分配矩阵的值更新为最小距离来源的分配场值
					minDis = disMtx[CY][CX] + temDis;
					disMtx[y][x] = minDis;
					locMtx[y][x] = locMtx[CY][CX];
				}
			} // end for i
		}	// end for x
	}// end for y

	// 第二次扫描（右上->左下）
	for (int y = bmpHeight - 1; y >= 0; --y) {
		for (int x = bmpWidth - 1; x >= 0; --x) {
			if (IsZero(disMtx[y][x]))
				continue;

			float minDis = MaxFloat;
			//对任一（x, y）位置，遍历每一个模板元素
			for (int i = pDisT->getSize() / 2 + 1; i >= 0; --i) {
				// 取出当前模板元素的参数（tmpDIS，OFFX，Y）
				int offX = pDisT->getOffX(i);
				int offY = pDisT->getOffY(i);
				float temDis = pDisT->getDis(i);

				// 计算当前模板元素在大矩阵中的位置（CX，CY）
				int CX = x + offX;
				int CY = y + offY;

				// 位置合法性检验
				if (CX < 0 || CX > bmpWidth - 1 || CY < 0 || CY > bmpHeight - 1)
					continue;

				// 计算距离矩阵对应距离值+模板元素的距离值，比较最小值
				if ((disMtx[CY][CX] + temDis) < minDis) {
					// 把当前（x,y）位置的距离值更新为最小值；同时，分配矩阵的值更新为最小距离来源的分配场值
					minDis = disMtx[CY][CX] + temDis;
					disMtx[y][x] = minDis;
					locMtx[y][x] = locMtx[CY][CX];
				}
			} // end for i
		} // end for x
	} // end for y

	// 保存数据，释放空间。
	Bmp bmpDest(32, bmpWidth, bmpHeight);
	for (uint y = 0; y < bmpHeight; ++y) {
		for (uint x = 0; x < bmpWidth; ++x) {
			bmpDest.mtx[y][x * 4] = disMtx[y][x];
			bmpDest.mtx[y][x * 4 + 1] = disMtx[y][x];
			bmpDest.mtx[y][x * 4 + 2] = disMtx[y][x];
		}
	}
	bmpDest.write(disBmpName);

	Delete2D<uchar>(locMtx, bmpHeight);
	Delete2D<float>(disMtx, bmpHeight);

	return 0;

}

