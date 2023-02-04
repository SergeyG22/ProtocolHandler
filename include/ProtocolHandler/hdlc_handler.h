#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <list>

class HDLC_Handler {
	uint16_t m_package_size;
	const int m_number_of_bits = 8;
	const int m_ignore_start_bits = 3;
	std::list<uint32_t>m_byte_buffer;
	std::vector<uint32_t>m_bit_buffer;
	const std::string m_frame_border = "0x7e";	
	std::vector<int>m_index_elements;
	std::list<uint32_t>m_stuff_combination{ 0,1,1,1,1,1 };
	std::list<uint32_t>m_package;
	bool checkSequenceforDuplicate(int, int);
	void fillBuffer(std::ifstream&);
	void selectPackagesFromBuffer(const std::string&);	
	void writeInSigFormat(uint16_t&, std::list<uint32_t>&, const std::string&);
public:
	unsigned char byteConverterLst(std::list<uint32_t>&);
	void exec(const std::string& , const std::string&);
};