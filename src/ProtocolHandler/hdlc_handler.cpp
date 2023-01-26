#include "ProtocolHandler/hdlc_handler.h"

void HDLC_Handler::exec() {
	
	std::ofstream out("../result/HDLC_TEST_RESULT.bin", std::ios::binary | std::ios::out);		//��� �����
	std::ifstream file("../data/HDLC_TEST_.BIT", std::ios::app | std::ios::binary);
	
	unsigned char current_bite;
	const int number_of_bits = 8;
	while (file >> current_bite) {
		file.read((char*)&current_bite, sizeof(current_bite));
		std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_bite)).str();
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
		std::cout << hex << '\n';
		std::bitset<8>bit_buffer(current_bite);			//[01111110]  //if (0x7E == 01111110)
		for (int i = 0; i < number_of_bits; ++i) {		//�������� � ���� ����� ��������� BIN �� ������� 0x7e
		//	std::cout << bit_buffer[i] << ' ';			
		}
	//	std::cout << '\n';	
		out.write((char*)&current_bite, sizeof(current_bite));
	}
	
	
	/*
	unsigned char current_byte;											//697091 ����� ����� �������
	std::ifstream file;													//���� �����
	file.open("../result/HDLC_TEST_RESULT.bin", std::ios::binary); 
	while (file >> current_byte) {
		file.read((char*)&current_byte, sizeof(current_byte));
		std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(current_byte)).str();
		std::cout << hex <<'\n';
	}	
	*/
}

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