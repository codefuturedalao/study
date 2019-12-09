#pragma once

#include <cmath>

using uint = unsigned int;
using uchar = unsigned char;


class DistanceTemplate {
public:
	virtual size_t getSize() = 0;
	virtual int getOffX(uint i) = 0;
	virtual int getOffY(uint i) = 0;
	virtual float getDis(uint i) = 0;
};


// 曼哈顿距离
class ManhattanTemplate : public DistanceTemplate {
public:
	ManhattanTemplate() {}

	size_t getSize() { return 13; }
	int  getOffX(uint i) { return offX_[i]; }
	int  getOffY(uint i) { return offY_[i]; }
	float getDis(uint i) { return disTemplate_[i]; }

private:
	uchar disTemplate_[13] = {  2,   2, 1, 2,   2, 1, 0, 1, 2,   2,1,2,2 };
	char		 offX_[13] = {  0,  -1, 0, 1,  -2,-1, 0, 1, 2,  -1,0,1,0 };
	char		 offY_[13] = { -2,  -1,-1,-1,   0, 0, 0, 0, 0,   1,1,1,2 };
};


// 欧式距离
template<int sideLength>
class EurTemplate : public DistanceTemplate {
public:
	EurTemplate() {
		// 正方形边长必须是奇数
		static_assert(sideLength >= 3 && sideLength % 2 == 1);
		for (uint i = 0; i < sideLength * sideLength; ++i) {
			int offx = getOffX(i);
			int offy = getOffY(i);
			disTemplate_[i] = sqrt(double(offx * offx + offy * offy));
		}
	}

	int  getOffX(uint i) { return i % sideLength - sideLength / 2; }
	int  getOffY(uint i) { return i / sideLength - sideLength / 2; }
	float getDis(uint i) { return disTemplate_[i]; }
	size_t getSize()	 { return sideLength * sideLength; }

private:
	float disTemplate_[sideLength * sideLength];
};


// 棋盘距离
template<int sideLength>
class ChessTmp : public DistanceTemplate {
public:
	ChessTmp() {
		for (unsigned i = 0; i < sideLength * sideLength; ++i) {
			float offx = abs(getOffX(i));
			float offy = abs(getOffY(i));
			disTemplate_[i] = offx > offy ? offx : offy;
		}
	}

	int  getOffX(uint i) { return i % sideLength - sideLength / 2; }
	int  getOffY(uint i) { return i / sideLength - sideLength / 2; }
	float getDis(uint i) { return disTemplate_[i]; }
	size_t getSize()	 { return sideLength * sideLength; }

private:
	float disTemplate_[sideLength * sideLength];
};


class  OctagoTemplate : public DistanceTemplate {
public:
	OctagoTemplate() :
		disTemplate_{  3, 3, 3, 3, 2, 2, 2, 3, 3, 2, 2, 1, 2, 2, 3, 3, 2, 1,
					0, 1, 2, 3, 3, 2, 2, 1, 2, 2, 3, 3, 2, 2, 2, 3, 3, 3, 3 }
	{
	}
	float getDis(uint i) { return disTemplate_[i]; }
	size_t getSize() { return 37; }
	int getOffX(uint i) { return 0; }
	int getOffY(uint i) { return 0; }
private:
	int disTemplate_[37];
};
