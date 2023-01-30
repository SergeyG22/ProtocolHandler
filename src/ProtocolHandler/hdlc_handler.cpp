#include "ProtocolHandler/hdlc_handler.h"



void HDLC_Handler::exec() {
//	std::ofstream out("../result/HDLC_TEST_RESULT.sig", std::ios::out | std::ios::binary);		
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ifstream::in | std::ifstream::binary);
	unsigned char current_bite;
	const int number_of_bits = 8;
	while (!file.eof()) {	
		file.read((char*)&current_bite, sizeof(current_bite));
		std::bitset<8>buffer(current_bite);
		for (int i = 0; i < number_of_bits; ++i) {
			bit_buffer.emplace_back(buffer[i]);
		}
	}
	int test = 0;
	std::vector<size_t>byte_buffer;
	for (int i = ignore_start_bits; i < bit_buffer.size(); ++i) {
		if (byte_buffer.size() != number_of_bits) {
			byte_buffer.emplace_back(bit_buffer[i]);  // ���� �� ����� 8 ����� ��������� ����
		}
		else {
			
		//	for (int i = 0; i < byte_buffer.size(); ++i) { 
		//		std::cout << byte_buffer[i] <<' ';
		//	}
		//	std::cout << '\n';
			std::reverse(byte_buffer.begin(),byte_buffer.end());
			std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverter(byte_buffer))).str();
			std::cout << hex << '\n';				 // ����� ������������ ����� ������� ���� ���� �� �����
													// ���� ����������� �� ����� (��������� ��� ���� ������ �� ���� ����� 00111100)
			byte_buffer.clear();
			byte_buffer.emplace_back(bit_buffer[i]);
			//std::cout << bit_buffer[i];
			if (test == 123) {					//122
				break;
			}
			test++;
			
			if (hex == frame_border) {   // ���� ������ �� ���� 
				if (!frame_is_open) {	 //�������� ��� ����� �������
					frame_is_open = true;
					std::cout << "border open\n";
					continue;
				}
				if (frame_is_open) {
					frame_is_open = false;
					std::cout << "border close\n";
					//������� �����-�� �������� � �������
					continue;
				}
			}
			
			
			

			


		}

	}





	file.close();


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

unsigned char HDLC_Handler::byteConverter(std::vector<size_t>&buffer) {
	std::vector<unsigned char>result((8 + 7) >> 3);							//���� ���������
	for (int j = 0; j < int(8); j++) {
		result[j >> 3] |= (buffer[j] << (j & 7));
	}
	return result[0];
}




/*
void HDLC_Handler::exec() {

	std::ofstream out("../result/HDLC_TEST_RESULT.sig",  std::ios::out | std::ios::binary);		//��� �����
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ios::in | std::ios::binary);
	
	unsigned char current_bite;
	const int number_of_bits = 8;
	while (file >> current_bite) {
		file.read((char*)&current_bite, sizeof(current_bite));
		std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_bite)).str();
		if (hex == frame_border) {
			if (!frame_is_open) {					
				frame_is_open = true;
				continue;
			}
			if (frame_is_open) {
				frame_is_open = false;
				if (!byte_buffer.empty()) {
					package_size = byte_buffer.size();
					out.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));
					for (auto get_byte : byte_buffer) {
						out.write((char*)&get_byte, sizeof(get_byte));
					}
					byte_buffer.clear();
				}
				continue;
			}
		}
		if (frame_is_open) {
			byte_buffer.emplace_back(current_bite);
		}		

			std::bitset<8>bit_buffer(current_bite);			
			for (int i = 0; i < number_of_bits; ++i) {		
				buffer.emplace_back(bit_buffer[i]);
			}
	}
	std::cout << buffer.size() <<'\n';
}
*/




//����� ������ ����
//������� ������
//����� ������ ����
//���������� ������ ������ ����� ����� �������
//�������� �� � uint16_t (������������ �����)
//�������� uint16_t � �������� ���� - �������� ���� �����.
//��������� ��������

//0x7e 0x7e 0x7e [data] 0x7e 0x7e
//���� ���� ������ �� (���������� ����) continue � ��������� ������
//out.write((uint16_t*)&current_bite, sizeof(uint16_t)); <<----------- �������� � ���� �������� ���

	//		std::cout << current_bite << '\n';
	//		out.write((char*)&current_bite, sizeof(current_bite));
	//		std::cout << byte_buffer.size() <<'\n';

	//	std::cout << hex << '\n';
	//	std::bitset<8>bit_buffer(current_bite);			//[01111110]  //if (0x7E == 01111110)
	//	for (int i = 0; i < number_of_bits; ++i) {		//�������� � ���� ����� ��������� BIN �� ������� 0x7e
		//	std::cout << bit_buffer[i] << ' ';			
	//	}
	//	std::cout << '\n';	

	/*
	unsigned char current_byte;
	std::ifstream file;													//���� �����
	file.open("../result/HDLC_TEST_RESULT.bin", std::ios::binary);
	while (file >> current_byte) {
		file.read((char*)&current_byte, sizeof(current_byte));
		std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_byte)).str();
		std::cout << hex <<'\n';
	}
	*/