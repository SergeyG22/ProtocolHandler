#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <list>

class HDLC_Handler {
	const int m_number_of_bits = 8;
	std::list<uint8_t>m_byte_buffer;
	std::vector<int>m_numbers_of_delete_bit;
	std::vector<uint8_t>m_bit_buffer;
	const std::string m_frame_flag = "0x7e";
	std::vector<uint8_t>m_bit_frame_flag = { 0,1,1,1,1,1,1,0 };
	std::list<uint8_t>m_bit_stuffing_flag{ 0,1,1,1,1,1 };
	std::list<uint8_t>m_package;
	void incrementPackageIndex(int& );
	int getIndexOfDeletedBit(int, int);
	void addBitToPackage(std::vector<uint8_t>&, int current_index);
	void addBitToByteBuffer(uint8_t);
	void MakeStepInSequenceOfBuffer(std::list<int>&, int&);
	int checkSequenceforDuplicate(int, int, int);
	int checkSequenceForFirstEntryBitFlag(int, int, int);
	int fillBitBuffer(const std::string&);
	void selectPackagesFromBitBuffer(const std::string&);	
	void writeToFileInSigFormat(std::list<uint8_t>&, const std::string&);
	void removeBitTransparencyFromPackage();
	int getFirstFlagBit(const std::vector<uint8_t>& flag, const std::vector<uint8_t>& );
	void clearAllBuffers();
	inline uint16_t getPackageSize() { return m_package.size() / m_number_of_bits;}
public:
	unsigned char byteConverter(std::list<uint8_t>&);
	int exec(const std::string&, const std::string&);
};