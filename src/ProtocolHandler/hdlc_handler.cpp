#include "ProtocolHandler/hdlc_handler.h"



void HDLC_Handler::exec() {

//	std::ofstream out("../result/HDLC_TEST_RESULT.sig", std::ios::out | std::ios::binary);		
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ifstream::in | std::ifstream::binary);	
	fillBuffer(file);
	int test = 0;
	
	for (int i = ignore_start_bits; i < bit_buffer.size(); ++i) {
		if (byte_buffer.size() != number_of_bits) {		
			byte_buffer.emplace_back(bit_buffer[i]);
		//	std::cout << bit_buffer[i] << ' ';
		}
		else {
			std::reverse(byte_buffer.begin(),byte_buffer.end());
			std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverterLst(byte_buffer))).str();
			std::cout << hex << '\n';				
			byte_buffer.clear();
			byte_buffer.emplace_back(bit_buffer[i]);
			if (test == 2) {					//122
				break;
			}
			test++;
			
			if (hex == frame_border) { 
				if (!frame_is_open) {	 
					frame_is_open = true;
					std::cout << "border open\n";
					int current_index = i;
					
					std::list<int>bit_sequence;
					for (int i = 0; i < 6; ++i) {  //first entry
						bit_sequence.push_back(bit_buffer[current_index]);
						current_index++;		
					}

					while (true) {
						bit_sequence.pop_front();
						bit_sequence.push_back(bit_buffer[current_index++]);
						//условие выхода будет в std::equal();
						if (std::equal(bit_sequence.begin(), bit_sequence.end(), stuff_combination.begin(), stuff_combination.end())) {
							//если седьмой бит равен единице и восьмой бит равен нулю - это флаг
							//если седьмой бит равен нулю удалить его.
							int index = current_index;
							std::cout << "next bit = " << bit_buffer[index] <<'\n';
							if (bit_buffer[index] == 1) {
								if (bit_buffer[index + 1] == 0) {
									std::cout << "get flag combination\n";
								}
							}

							break;
						}
					}

					for (auto it : bit_sequence) {
						std::cout << it << ' ';
					}
					std::cout << '\n';

			//		std::cout << current_index <<' ' << current_index + 1 <<' '<<current_index + 2 <<' ' << current_index + 3 << ' ' << current_index + 4;
					continue;
				}
				if (frame_is_open) {
					frame_is_open = false;
					std::cout << "border close\n";
					continue;
				}
			}

		}

	}
	file.close();
	
	/*
if (std::equal(test1.begin(), test1.end(), stuff_combination.begin(), stuff_combination.end())) {
	std::cout << "true\n";
}*/
//считать первые 5 бит
//сравниваем их с эквивалентом который у нас есть
//	std::cout << bit_buffer[i] << bit_buffer[i+ 1] << bit_buffer[i + 2] << bit_buffer[i + 3] << bit_buffer[i + 4];



	/*
	std::vector<size_t>byte_buffer; 
	for (int i = ignore_start_bits; i < bit_buffer.size(); ++i) { 

			if (byte_buffer.size() != number_of_bits) {				
				byte_buffer.emplace_back(bit_buffer[i]);
			}
			else {
				std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverter(byte_buffer))).str();
				std::cout << hex << '\n';
				byte_buffer.clear();
				byte_buffer.emplace_back(bit_buffer[i]);

				if (hex == frame_border) {
					if (!frame_is_open) {
						frame_is_open = true;
						std::cout << "border open\n";
						continue;
					}
					if (frame_is_open) {
						frame_is_open = false;
						std::cout << "border close\n";
						continue;
					}
				}
			}
	}
	*/
	


//	std::cout << '\n';
//	std::cout << (int)byteConverter(bit_buffer) <<'\n';
//	unsigned char test = byteConverter(bit_buffer);
//	std::cout << result[0];

//	std::string res = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverter(bit_buffer))).str();
//	std::cout << res <<'\n';

//	std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_bite)).str();
//	std::cout << hex <<'\n';


}



void HDLC_Handler::fillBuffer(std::ifstream& file) {
	unsigned char current_bite;
	while (!file.eof()) {
		file.read((char*)&current_bite, sizeof(current_bite));
		std::bitset<8>buffer(current_bite);
		for (int i = 0; i < number_of_bits; ++i) {
			bit_buffer.emplace_back(buffer[i]);
		}
	}
}

unsigned char HDLC_Handler::byteConverterLst(std::list<size_t>& buffer) {
	std::list<unsigned char>result((8 + 7) >> 3);
	for (int j = 0; j < int(8); j++) {
		auto buffer_it = buffer.begin();
		auto result_it = result.begin();
		int pos = j >> 3;
		std::advance(result_it, pos);
		std::advance(buffer_it, j);
		*result_it |= (*buffer_it << (j & 7));
	}
	return result.front();
}

unsigned char HDLC_Handler::byteConverterVec(std::vector<size_t>& buffer) {
	std::vector<unsigned char>result((8 + 7) >> 3);
	for (int j = 0; j < int(8); j++) {
		result[j >> 3] |= (buffer[j] << (j & 7));
	}
	return result[0];
}






