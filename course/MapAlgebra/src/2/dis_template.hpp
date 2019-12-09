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
template<int width, int height>
class ManhattanTemplate : public DistanceTemplate {
public:
	ManhattanTemplate() {
		static_assert(width % 2 == 1 && height % 2 == 1);
		for (uint i = 0; i < width * height; ++i) {
			int offx = getOffX(i);
			int offy = getOffY(i);
			disTemplate_[i] = offx + offy;
		}
	}

	size_t getSize() { return width * height; }
	int  getOffX(uint i) { return i % width - width / 2; }
	int  getOffY(uint i) { return i / height - height / 2; }
	float getDis(uint i) { return disTemplate_[i]; }

private:
	float disTemplate_[width * height];
};


// 欧式距离
template<int width, int height>
class EurTemplate : public DistanceTemplate {
public:
	EurTemplate() {
		static_assert(width % 2 == 1 && height % 2 == 1);
		for (uint i = 0; i < width * height; ++i) {
			int offx = getOffX(i);
			int offy = getOffY(i);
			disTemplate_[i] = sqrt(double(offx * offx + offy * offy));
		}
	}

	size_t getSize() { return width * height; }
	int  getOffX(uint i) { return i % width - width / 2; }
	int  getOffY(uint i) { return i / height - height / 2; }
	float getDis(uint i) { return disTemplate_[i]; }

private:
	float disTemplate_[width * height];
};
