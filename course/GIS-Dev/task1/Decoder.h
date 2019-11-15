#pragma once
#pragma warning(disable : 4996)

struct Pair
{
	Pair() : 
		character(0), index(0x7FFFFFFF) { }	// MAX_INT=0x7FFFFFFF
	char character;
	int index;
};


class Decoder
{
public:
	Decoder();
	~Decoder();

	bool decode();
	void print();
	bool read(const char* fileName);
	bool write(const char* fileName);

private:
	void  sort_(Pair* pair, int left, int right);
	Pair* content_;
	int	  length_;		// 实际字符数
};
