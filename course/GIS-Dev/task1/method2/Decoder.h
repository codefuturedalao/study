#pragma once
#pragma warning(disable : 4996)


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
	void  sort_(int* intArr, char* charArr, int left, int right);
	template<typename T> void swap_(T& t1, T& t2) { T t = t1; t1 = t2; t2 = t; }
private:
	char* content_;
	int*  index_;
	int	  length_;		// 实际字符数
};
