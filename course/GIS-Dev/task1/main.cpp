#include "Decoder.h"

#include <cstdio>


int main() {
	Decoder decoder;
	if (decoder.read("Z:/Data/Encode2.da")) {
		decoder.decode();
		decoder.print();
		decoder.write("Z:/Data/Decode2.da");
	}

	printf("\n\n----------------------------------------------\n\n");

	if (decoder.read("Z:/Data/Encode3.da")) {
		decoder.decode();
		decoder.print();
		decoder.write("Z:/Data/Decode3.da");
	}

	getchar();
	return 0;
}