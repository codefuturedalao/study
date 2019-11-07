#include "MA1.h"

#include <cstdio>
#include <cstdlib>


int main() {
	Bmp16ColorInverse("Z:/MA/1/Data/inverse/4bit_16color/shape.bmp",
		"Z:/MA/1/Data/inverse/4bit_16color/shape_inverse_clr.bmp", InverseColorTable);

	Bmp16ColorInverse("Z:/MA/1/Data/inverse/4bit_16color/shape.bmp",
		"Z:/MA/1/Data/inverse/4bit_16color/shape_inverse_idx.bmp", InverseColorIndex);

	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/shape.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/shape_convert.bmp");

	printf("here\n");

	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/crystal.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/crystal_convert.bmp");

	Bmp256ColorTo32Bit("Z:/MA/1/Data/convert/8bit_to_32bit/crystal_x.bmp",
		"Z:/MA/1/Data/convert/8bit_to_32bit/crystal_x_convert.bmp");

	//return 0;

	/******************************************** 反色处理 *****************************************************/

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


	/********************************************* 24bit 转 8bit(256色) ******************************************/
	// 24bit 转 8bit（256色）
	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/shape.bmp",
					   "Z:/MA/1/Data/convert/24bit_to_8bit/shape_convert.bmp");

	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/crystal.bmp",
					   "Z:/MA/1/Data/convert/24bit_to_8bit/crystal_convert.bmp");

	Bmp24BitTo256Color("Z:/MA/1/Data/convert/24bit_to_8bit/landscape.bmp",
					   "Z:/MA/1/Data/convert/24bit_to_8bit/landscape_convert.bmp");
				

	system("pause");
	return 0;
}