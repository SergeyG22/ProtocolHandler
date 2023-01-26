#include "ProtocolHandler/hdlc_handler.h"

void HDLC_Handler::exec() {
	/*
																		 //בטע נטהונ
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ios::app | std::ios::binary);
	unsigned char current_bite;
	const int number_of_bits = 8;
	while (file >> current_bite) {
		std::bitset<8>bit_buffer(current_bite); //[01011100]  //if (0x7E == 01011100)
		for (int i = 0; i < number_of_bits; ++i) {
			std::cout << bit_buffer[i] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';	
	*/

	unsigned char current_byte;											//697091 מבשוו קטסכמ פנוילמג
	std::ifstream file;													//באיע נטהונ
	file.open("../data/HDLC_TEST_.BIT", std::ios::binary); 
	while (file >> current_byte) {
		file.read((char*)&current_byte, sizeof(current_byte));
		std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_byte)).str();
	//	std::cout << hex <<'\n';
		if (hex == frame_border) {
			frame_counter++;
		}
	}
	std::cout << frame_counter << '\n';
}