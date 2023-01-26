#pragma 
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

class HDLC_Handler {
	const std::string frame_border = "0x7e";
	bool frame_is_open = false;
public:
	void exec();
};