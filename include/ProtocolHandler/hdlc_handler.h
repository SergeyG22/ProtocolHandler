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
	const int ignore_start_bits = 3;
	std::list<size_t>byte_buffer;
	std::vector<size_t>bit_buffer;
	const std::string frame_border = "0x7e";	
	std::vector<int>index_elements;
	std::list<size_t>stuff_combination{ 0,1,1,1,1,1 };
	std::list<size_t>package;
	std::vector<int>pack;
	int test = 0;
public:
	void exec();
	unsigned char byteConverterVec(std::vector<size_t>& );
	unsigned char byteConverterLst(std::list<size_t>& );
	void fillBuffer(std::ifstream&);
	void writeInSigFormat(uint16_t&,std::list<size_t>&);
};