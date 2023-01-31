#include "ProtocolHandler/hdlc_handler.h"



void HDLC_Handler::exec() {

//			
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ifstream::in | std::ifstream::binary);	
	fillBuffer(file);
	for (int i = ignore_start_bits; i < bit_buffer.size(); ++i) {
		if (byte_buffer.size() != number_of_bits) {		
			byte_buffer.emplace_front(bit_buffer[i]);
		}
		else {
			std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverterLst(byte_buffer))).str();
			byte_buffer.clear();
			byte_buffer.emplace_front(bit_buffer[i]);
	
			if (hex != frame_border) {
				int current_index = i - 8;
				std::list<int>bit_sequence;
				for (int i = 0; i < 6; ++i) {  
					bit_sequence.push_back(bit_buffer[current_index]);
					package.push_back(bit_buffer[current_index]);
					current_index++;
				}
				while (true) {					
					bit_sequence.pop_front();	
					package.push_back(bit_buffer[current_index]);
					current_index++;					
					bit_sequence.push_back(bit_buffer[current_index]);
					if (std::equal(bit_sequence.begin(), bit_sequence.end(), stuff_combination.begin(), stuff_combination.end())) {
						package.push_back(bit_buffer[current_index]);
						current_index++;
							if (bit_buffer[current_index] == 0) { 
								int drop_element_index = (current_index - i + 8);
								index_elements.push_back(drop_element_index);
								continue;
							}

							if (bit_buffer[current_index] == 1) {  
								if (bit_buffer[current_index + 1] == 0) {
									for (int i = 0; i < index_elements.size(); ++i) {
										auto it = package.begin();
										std::advance(it,index_elements[i]);
										package.erase(it);
									}

									for (int i = 0; i < stuff_combination.size(); ++i) {
										package.pop_back();
									}

									pack.push_back(package.size() / number_of_bits);

									for (auto it : pack) {
								//		std::cout << it << ' ';
									}

									uint16_t package_size = package.size() / number_of_bits;
									writeInSigFormat(package_size, package); // размер + пакет

									break;
								}
							}
							
						return;
					}


				}

				i += package.size() + index_elements.size();
				package.clear();
				index_elements.clear();

				std::cout << "вышли\n";

			//	for (auto it : pack) {
			//		std::cout << it <<' ';
			//	}
			}

		}

	}
	file.close();
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

void HDLC_Handler::writeInSigFormat(uint16_t& package_size, std::list<size_t>& package) {

	std::cout << "Размер пакета в байтах = " << package_size << '\n';
	std::cout << "Размер пакета в битах = " << package.size() << '\n';
	std::ofstream out("../result/HDLC_TEST_RESULT.sig", std::ios::app | std::ios::binary);
	out.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));

	int current_bit = 0;
	std::list<size_t>current_byte;
	for (auto it : package) {
		current_byte.push_front(it);
		current_bit++;
		if (current_bit == number_of_bits) {
			current_bit = 0;
			std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << (int)byteConverterLst(current_byte)).str();
			std::cout << hex << '\n';
			std::reverse(std::begin(current_byte),std::end(current_byte));
			unsigned char byte = byteConverterLst(current_byte);
			out.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
			current_byte.clear();
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


/*
std::ofstream out("../result/HDLC_TEST_RESULT.sig", std::ios::app | std::ios::binary);
out.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));


for (int i = 0; i < package.size(); ++i) {
	std::list<size_t>current_byte;
	auto it = package.begin();
	for (int j = 0; j < number_of_bits; ++j) {

		std::advance(it, j);
		std::cout << *it;
		current_byte.push_back(*it);
	}
	//	unsigned char byte = byteConverterLst(current_byte);
	//	std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << (int)byteConverterLst(current_byte)).str();
	//	std::cout << hex;
	//	out.write((char*)&byte, sizeof(byte));
}
*/



