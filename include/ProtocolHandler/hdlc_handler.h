#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <list>

class HDLC_Handler {
	const int m_number_of_bits = 8;
	const int m_ignore_start_bits = 3;
	std::list<uint8_t>m_byte_buffer;
	std::vector<int>m_index_elements;
	std::vector<uint8_t>m_bit_buffer;
	const std::string m_frame_border = "0x7e";	
	std::list<uint8_t>m_bit_flag{ 0,1,1,1,1,1 };
	std::list<uint8_t>m_package;
	void addBitToPackage(std::vector<uint8_t>&, int current_index);
	void MakeStepInSequenceOfBuffer(std::list<int>&, int&);
	int checkSequenceforDuplicate(int, int, int);
	int fillBitBuffer(const std::string&);
	void selectPackagesFromBitBuffer(const std::string&);	
	void writeToFileInSigFormat(std::list<uint8_t>&, const std::string&);
	void removeBitTransparencyFromPackage();
public:
	unsigned char byteConverter(std::list<uint8_t>&);
	int exec(const std::string& , const std::string&);
};