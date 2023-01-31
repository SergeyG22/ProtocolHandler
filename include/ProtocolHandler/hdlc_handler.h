#pragma 
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <list>

class HDLC_Handler {
	uint16_t package_size;
	const int number_of_bits = 8;
	std::list<size_t>byte_buffer;
	const std::string frame_border = "0x7e";
	bool frame_is_open = false;
	std::vector<size_t>bit_buffer;
	std::vector<int>index_elements;
	const int ignore_start_bits = 3;
	std::list<size_t>stuff_combination{ 0,1,1,1,1,1 };
	std::list<size_t>package;
	std::vector<int>pack;
public:
	void exec();
	unsigned char byteConverterVec(std::vector<size_t>& );
	unsigned char byteConverterLst(std::list<size_t>& );
	void fillBuffer(std::ifstream&);
};