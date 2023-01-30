#pragma 
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <list>

class HDLC_Handler {
	uint16_t package_size;
	std::vector<unsigned char>byte_buffer;
	const std::string frame_border = "0x7e";
	bool frame_is_open = false;
	std::vector<size_t>bit_buffer;
	const int ignore_start_bits = 3;
	std::list<size_t>stuff_combination{ 0,1,1,1,1,1 };
public:
	void exec();
	unsigned char byteConverterVec(std::vector<size_t>&);
	unsigned char byteConverterLst(std::list<size_t>&);
};