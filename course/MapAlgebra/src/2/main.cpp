
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
void test_dis_tran();
void test_voronoi();
void test_get_boundary();
void test_get_buffer();


int main() {
//	test_others();
//
//	test_inverse();
//	test_convert();

//	test_gray();
//	test_thres();

	test_dis_tran();
	test_get_boundary();
	test_get_buffer();
	test_voronoi();

	system("pause");
	return 0;
}


void test_voronoi()
{
	scanSrcPointCoords("data/2/voronoi/SrcTIN.bmp", "data/2/voronoi/coords.txt");
	EurTemplate<13> eDisT13;
	DisTrans32bit("data/2/voronoi/srcTin.bmp", &eDisT13, "data/2/voronoi/dis_eu13_32bit.bmp", 
		"data/2/voronoi/loc_eu13_32bit.bmp");
	getTinPointPairs("data/2/voronoi/loc_eu13_32bit.bmp", "data/2/voronoi/PointPairs.txt");
	linkPoints("data/2/voronoi/SrcTin.bmp", "data/2/voronoi/PointPairs.txt", 
		"data/2/voronoi/coords.txt", "data/2/voronoi/TIN.bmp");
}

void test_get_boundary()
{
	EurTemplate<13> eDisT13;
	DisTrans256("data/2/buffer/test.bmp", &eDisT13, "data/2/boundary/dis_eu13_256color.bmp",
		"data/2/boundary/loc_eu13_256color.bmp");

	getBoundaryFromBmp("data/2/boundary/loc_eu13_256color.bmp", "data/2/boundary/boundary1.bmp");

	getBoundaryFromBmp("data/2/boundary/test.bmp", "data/2/boundary/boundary2.bmp");
}

void test_get_buffer()
{
	EurTemplate<13> eDisT13;
	DisTrans32bit("data/2/buffer/test.bmp", &eDisT13, "data/2/buffer/dis_eu13_32bit.bmp",
		"data/2/buffer/loc_eu13_32bit.bmp");
	getBufferFromBmp("data/2/buffer/dis_eu13_32bit.bmp", "data/2/buffer/loc_eu13_32bit.bmp",
		"data/2/buffer/buffer.bmp", 15);
}


void test_dis_tran() {
	EurTemplate<5> eDisT5;
	DisTrans256("data/2/dis_tran/test.bmp", &eDisT5, "data/2/dist_tran/dis_eu5_256color.bmp", 
		"data/2/dis_tran/loc_eu5_256color.bmp");
	DisTrans32bit("data/2/dis_tran/test.bmp", &eDisT5, "data/2/dis_tran/dis_eu5_32bit.bmp",
		"data/2/dis_tran/loc_eu5_32bit.bmp");

	EurTemplate<13> eDisT13;
	DisTrans256("data/2/dis_tran/test.bmp", &eDisT13, "data/2/dist_tran/dis_eu13_256color.bmp", 
		"data/2/dis_tran/loc_eu13_256color.bmp");
	DisTrans32bit("data/2/dis_tran/test.bmp", &eDisT13, "data/2/dis_tran/dis_eu13_32bit.bmp",
		"data/2/dis_tran/loc_eu13_32bit.bmp");

	ManhattanTemplate manhtDisT;
	DisTrans256("data/2/dis_tran/test.bmp", &manhtDisT, "data/2/dist_tran/dis_manht_256color.bmp", 
		"data/2/dis_tran/loc_manht_256color.bmp");
	DisTrans32bit("data/2/dis_tran/test.bmp", &manhtDisT, "data/2/dis_tran/dis_manht_32bit.bmp",
		"data/2/dis_tran/loc_manht_32bit.bmp");

	ChessTmp<13> chessT;
	DisTrans256("data/2/dis_tran/test.bmp", &chessT, "data/2/dist_tran/dis_chess_256color.bmp", 
		"data/2/dis_tran/loc_chess_256color.bmp");
	DisTrans32bit("data/2/dis_tran/test.bmp", &chessT, "data/2/dis_tran/dis_chess_32bit.bmp",
		"data/2/dis_tran/loc_chess_32bit.bmp");
}


// 二值化
void test_thres() {
	BmpThreshold("data/2/landscape_gray.bmp", "data/2/landscape_gray_thres.bmp", 128);
	BmpThreshold("data/2/shape_gray.bmp", "data/2/shape_gray_thres.bmp", 128);
}

// 灰度化
void test_gray() {
	BmpGray("data/2/landscape.bmp", "data/2/landscape_gray.bmp");
	BmpGray("data/2/shape.bmp", "data/2/shape_gray.bmp");
}


void test_inverse() {
	// 4bit 16色
	Bmp16ColorInverse("data/1/Data/inverse/4bit_16color/shape.bmp",
		"data/1/Data/inverse/4bit_16color/shape_inverse_clr.bmp", InverseColorTable);

	Bmp16ColorInverse("data/1/Data/inverse/4bit_16color/shape.bmp",
		"data/1/Data/inverse/4bit_16color/shape_inverse_idx.bmp", InverseColorIndex);

	// 8bit 256色
	Bmp256ColorInverse("data/1/Data/inverse/8bit_256color/shape.bmp",
		"data/1/Data/inverse/8bit_256color/shape_inverse.bmp");

	Bmp256ColorInverse("data/1/Data/inverse/8bit_256color/landscape.bmp",
		"data/1/Data/inverse/8bit_256color/landscape_inverse.bmp");

	// 24bit 真彩色
	Bmp24BitInverse("data/1/Data/inverse/24bit_trueColor/shape.bmp",
		"data/1/Data/inverse/24bit_trueColor/shape_inverse.bmp");

	Bmp24BitInverse("data/1/Data/inverse/24bit_trueColor/landscape.bmp",
		"data/1/Data/inverse/24bit_trueColor/landscape_inverse.bmp");

	// 自动判断bmp类型
	BmpInverse("data/1/Data/inverse/8bit_256color/landscape.bmp",
		"data/1/Data/inverse/8bit_256color/landscape_auto_inverse.bmp");

	BmpInverse("data/1/Data/inverse/24bit_trueColor/landscape.bmp",
		"data/1/Data/inverse/24bit_trueColor/landscape_auto_inverse.bmp");
}

void test_convert() {
	// 8bit（256色）转 32bit
	Bmp256ColorTo32Bit("data/1/Data/convert/8bit_to_32bit/shape.bmp",
		"data/1/Data/convert/8bit_to_32bit/shape_convert.bmp");

	Bmp256ColorTo32Bit("data/1/Data/convert/8bit_to_32bit/crystal_x.bmp",
		"data/1/Data/convert/8bit_to_32bit/crystal_x_convert.bmp");

	// 24bit 转 8bit（256色）
	Bmp24BitTo256Color("data/1/Data/convert/24bit_to_8bit/shape.bmp",
		"data/1/Data/convert/24bit_to_8bit/shape_convert.bmp");

	Bmp24BitTo256Color("data/1/Data/convert/24bit_to_8bit/crystal.bmp",
		"data/1/Data/convert/24bit_to_8bit/crystal_convert.bmp");

	Bmp24BitTo256Color("data/1/Data/convert/24bit_to_8bit/landscape.bmp",
		"data/1/Data/convert/24bit_to_8bit/landscape_convert.bmp");
}

