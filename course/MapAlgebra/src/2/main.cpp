
#include "MA1.h"
#include "MA2.h"
#include "bmp.h"
#include "dis_template.hpp"

#include <cstdio>
#include <cstdlib>


void test_inverse();
void test_convert();
void test_gray();
void test_thres();
void test_others();
void test_dis_tran();


int main() {
	//test_others();

	//test_inverse();
	//test_convert();

	test_gray();
	test_thres();

	test_dis_tran();

	system("pause");
	return 0;
}

void test_dis_tran() {
	EurTemplate<5, 5> eDisT5;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &eDisT5, "Z:/MA/2/Data/dis_tran/eur/shape_dis_5.bmp", "Z:/MA/2/Data/dis_tran/eur/shape_loc_5.bmp");
	EurTemplate<9, 9> eDisT9;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &eDisT9, "Z:/MA/2/Data/dis_tran/eur/shape_dis_9.bmp", "Z:/MA/2/Data/dis_tran/eur/shape_loc_9.bmp");
	EurTemplate<15, 15> eDisT15;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &eDisT15, "Z:/MA/2/Data/dis_tran/eur/shape_dis_15.bmp", "Z:/MA/2/Data/dis_tran/eur/shape_loc_15.bmp");
	EurTemplate<25, 25> eDisT25;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &eDisT25, "Z:/MA/2/Data/dis_tran/eur/shape_dis_25.bmp", "Z:/MA/2/Data/dis_tran/eur/shape_loc_25.bmp");

	ManhattanTemplate<5, 5> mDisT5;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &mDisT5, "Z:/MA/2/Data/dis_tran/manha/shape_dis_5.bmp", "Z:/MA/2/Data/dis_tran/manha/shape_loc_5.bmp");
	ManhattanTemplate<15, 15> mDisT15;
	DisTrans256("Z:/MA/2/Data/dis_tran/shape.bmp", &mDisT15, "Z:/MA/2/Data/dis_tran/manha/shape_dis_15.bmp", "Z:/MA/2/Data/dis_tran/manha/shape_loc_15.bmp");
}


void test_others() {
	Bmp16ColorInverse("Z:/bmp16.bmp", "Z:/bmp16_inverse.bmp");
	Bmp256ColorInverse("Z:/bmp256.bmp", "Z:/bmp256_inverse.bmp");
	Bmp24BitInverse("Z:/bmp24.bmp", "Z:/bmp24_inverse.bmp");
}


void test_thres() {
	BmpThreshold("Z:/MA/2/Data/landscape_gray.bmp", "Z:/MA/2/Data/landscape_gray_thres.bmp", 128);
	BmpThreshold("Z:/MA/2/Data/shape_gray.bmp", "Z:/MA/2/Data/shape_gray_thres.bmp", 128);
}


void test_gray() {
	BmpGray("Z:/MA/2/Data/landscape.bmp", "Z:/MA/2/Data/landscape_gray.bmp");
	BmpGray("Z:/MA/2/Data/shape.bmp", "Z:/MA/2/Data/shape_gray.bmp");
}


void test_inverse() {
	// 4bit 16色
	Bmp16ColorInverse("Z:/MA/1/Data/inverse/4bit_16color/shape.bmp",
		"Z:/MA/1/Data/inverse/4bit_16color/shape_inverse_clr.bmp", InverseColorTable);

	Bmp16ColorInverse("Z:/MA/1/Data/inverse/4bit_16color/shape.bmp",
		"Z:/MA/1/Data/inverse/4bit_16color/shape_inverse_idx.bmp", InverseColorIndex);

	// 8bit 256色
	Bmp256ColorInverse("Z:/MA/1/Data/inverse/8bit_256color/shape.bmp",
		"Z:/MA/1/Data/inverse/8bit_256color/shape_inverse.bmp");

	Bmp256ColorInverse("Z:/MA/1/Data/inverse/8bit_256color/crystal.bmp",
		"Z:/MA/1/Data/inverse/8bit_256color/crystal_inverse.bmp");

	Bmp256ColorInverse("Z:/MA/1/Data/inverse/8bit_256color/crystal_x.bmp",
		"Z:/MA/1/Data/inverse/8bit_256color/crystal_x_inverse.bmp");

	Bmp256ColorInverse("Z:/MA/1/Data/inverse/8bit_256color/landscape.bmp",
		"Z:/MA/1/Data/inverse/8bit_256color/landscape_inverse.bmp");

	// 24bit 真彩色
	Bmp24BitInverse("Z:/MA/1/Data/inverse/24bit_trueColor/shape.bmp",
		"Z:/MA/1/Data/inverse/24bit_trueColor/shape_inverse.bmp");

	Bmp24BitInverse("Z:/MA/1/Data/inverse/24bit_trueColor/crystal.bmp",
		"Z:/MA/1/Data/inverse/24bit_trueColor/crystal_inverse.bmp");

	Bmp24BitInverse("Z:/MA/1/Data/inverse/24bit_trueColor/landscape.bmp",
		"Z:/MA/1/Data/inverse/24bit_trueColor/landscape_inverse.bmp");

	// 自动判断bmp类型
	BmpInverse("Z:/MA/1/Data/inverse/8bit_256color/landscape.bmp",
		"Z:/MA/1/Data/inverse/8bit_256color/landscape_auto_inverse.bmp");

	BmpInverse("Z:/MA/1/Data/inverse/24bit_trueColor/landscape.bmp",
		"Z:/MA/1/Data/inverse/24bit_trueColor/landscape_auto_inverse.bmp");
}

void test_convert() {
	// 8bit（256色）转 32bit
	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/shape.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/shape_convert.bmp");

	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/crystal.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/crystal_convert.bmp");

	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/crystal_x.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/crystal_x_convert.bmp");

	// 24bit 转 8bit（256色）
	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/shape.bmp",
		"Z:/MA/1/Data/convert/24bit_to_8bit/shape_convert.bmp");

	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/crystal.bmp",
		"Z:/MA/1/Data/convert/24bit_to_8bit/crystal_convert.bmp");

	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/landscape.bmp",
		"Z:/MA/1/Data/convert/24bit_to_8bit/landscape_convert.bmp");
}

