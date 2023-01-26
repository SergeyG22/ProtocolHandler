#pragma 
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

class HDLC_Handler {
	const std::string frame_border = "0x7e";
	int frame_counter = 0;
public:
	void exec();
};