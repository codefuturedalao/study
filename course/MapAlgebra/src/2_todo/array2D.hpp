
// 申请二维数组
template<typename T>
T** New2D(unsigned int i, unsigned int j) {
	T** buf = new T*[i];
	for (unsigned int k = 0; k < i; ++k)
		buf[k] = new T[j];

	return buf;
}

// 销毁二维数组
template<typename T>
void Delete2D(T** buf, unsigned int i) {
	for (unsigned int k = 0; k < i; ++k)
		delete[] buf[k];

	delete[] buf;
}