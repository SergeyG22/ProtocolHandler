#include "ProtocolHandler/hdlc_handler.h"

int HDLC_Handler::exec(const std::string& input_file_path, const std::string& output_file_path) {
	if (fillBitBuffer(input_file_path)) {
		return 1;
	}
	selectPackagesFromBitBuffer(output_file_path);
	return 0;
}

void HDLC_Handler::incrementPackageIndex(int & package_index) {
	++package_index;
}

int HDLC_Handler::getIndexOfDeletedBit(int bit_package_index, int bit_buffer_index) {
	return (bit_package_index - bit_buffer_index + m_number_of_bits) - m_index_elements.size();
}

void HDLC_Handler::addBitToPackage(std::vector<uint8_t>& m_bit_buffer, int current_index) {
	m_package.push_back(m_bit_buffer[current_index]);
}

void HDLC_Handler::MakeStepInSequenceOfBuffer(std::list<int>& bit_sequence, int& current_index) {
	bit_sequence.pop_front();	
	current_index++;
	bit_sequence.push_back(m_bit_buffer[current_index]);
}

int HDLC_Handler::checkSequenceforDuplicate(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::vector<int>next_bit_flag;
	std::copy(m_bit_buffer.begin() + bit_package_index, m_bit_buffer.begin() + bit_package_index + m_bit_flag.size(), std::back_inserter(next_bit_flag));
	if (std::equal(next_bit_flag.begin(), next_bit_flag.end(), m_bit_flag.begin(), m_bit_flag.end())) {
		m_index_elements.push_back(drop_element_index + m_bit_flag.size());
		std::cout << "ƒубликат найден!" << drop_element_index + m_bit_flag.size() <<'\n';
	}
	return drop_element_index + m_bit_flag.size();
}

int HDLC_Handler::checkSequenceForFirstEntryBitFlag(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::list<uint8_t>m_flag{ 1,1,1,1,1,0 };
	if (m_package.size() == m_flag.size() && 
		std::equal(m_package.begin(), m_package.end(), m_flag.begin(), m_flag.end())) {
		m_index_elements.push_back(m_flag.size() - 1);
		checkSequenceforDuplicate(bit_package_index - 1, bit_buffer_index, drop_element_index);
	}
	return m_flag.size() - 1;
}

int HDLC_Handler::fillBitBuffer(const std::string& input_file_path) {
	std::ifstream file(input_file_path, std::ifstream::in | std::ifstream::binary);
	unsigned char current_bite;
	if (file.is_open()) {
		while (!file.eof()) {
			file.read((char*)&current_bite, sizeof(current_bite));
			std::bitset<8>buffer(current_bite);
			for (int i = 0; i < m_number_of_bits; ++i) {
				m_bit_buffer.emplace_back(buffer[i]);
			}
		}
	}
	else {
		std::cerr << "Error: input file name or path is not correct\n";
		file.close();
		return 1;
	}
	return 0;
}

void HDLC_Handler::selectPackagesFromBitBuffer(const std::string& output_file_path) {
	for (int bit_buffer_index = m_ignore_start_bits; bit_buffer_index < m_bit_buffer.size(); ++bit_buffer_index) {
		
		if (m_byte_buffer.size() != m_number_of_bits) {								
			m_byte_buffer.emplace_front(m_bit_buffer[bit_buffer_index]);			
		}
		else { 			
			std::string hex = (std::stringstream() << "0x" << std::hex << int(byteConverter(m_byte_buffer))).str();
			m_byte_buffer.clear();													 
			m_byte_buffer.emplace_front(m_bit_buffer[bit_buffer_index]);			

			if (hex != m_frame_border) {

				int bit_package_index = bit_buffer_index - m_number_of_bits;		
				std::list<int>bit_sequence;
				for (int i = 0; i < m_bit_flag.size(); ++i) {						
					bit_sequence.push_back(m_bit_buffer[bit_package_index]);
					addBitToPackage(m_bit_buffer, bit_package_index);
					incrementPackageIndex(bit_package_index);
				}
				while (true) {														
					checkSequenceForFirstEntryBitFlag(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
					addBitToPackage(m_bit_buffer, bit_package_index);
					MakeStepInSequenceOfBuffer(bit_sequence, bit_package_index);						
					if (std::equal(bit_sequence.begin(), bit_sequence.end(), m_bit_flag.begin(), m_bit_flag.end())) {
					addBitToPackage(m_bit_buffer, bit_package_index);
						
						incrementPackageIndex(bit_package_index);										
						if (m_bit_buffer[bit_package_index] == 0) {					
							checkSequenceforDuplicate(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
							m_index_elements.push_back(getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
							continue;
						}

						if (m_bit_buffer[bit_package_index] == 1) {			//если он обнаруживает флаг
							if (m_bit_buffer[bit_package_index + 1] == 0) { //и ноль то он уже в пакете и его надо дропнуть!!!!!
								removeBitTransparencyFromPackage();								
								writeToFileInSigFormat(m_package, output_file_path);
								break;
							}
						}

					}
				}				

				if (m_package.size() == 8) {						   //1983448 - самый конец 102 пакета
					std::cout << "bit " << bit_buffer_index << '\n';  //1983464 - конец пакет 103 с 1 байтом 
					std::cout << "package index " << bit_package_index << '\n';// 64 56 48 сьехал и сломалс€
					std::cout << "next flag pack = ";
					std::cout << (int)m_bit_buffer[1983449];				//449 - 557   //с 1983449 отсчитывать по 8 бит в поисках флага
					std::cout << (int)m_bit_buffer[1983450];
					std::cout << (int)m_bit_buffer[1983451];
					std::cout << (int)m_bit_buffer[1983452];
					std::cout << (int)m_bit_buffer[1983453];
					std::cout << (int)m_bit_buffer[1983454];
					std::cout << (int)m_bit_buffer[1983455];
					std::cout << (int)m_bit_buffer[1983456];
					std::cout << (int)m_bit_buffer[1983457];
					std::cout << "\n";

					std::cout << (int)m_bit_buffer[1983440];				  
					std::cout << (int)m_bit_buffer[1983441];
					std::cout << (int)m_bit_buffer[1983442];
					std::cout << (int)m_bit_buffer[1983443];
					std::cout << (int)m_bit_buffer[1983444];
					std::cout << (int)m_bit_buffer[1983445];
					std::cout << (int)m_bit_buffer[1983446];
					std::cout << (int)m_bit_buffer[1983447];
					std::cout << (int)m_bit_buffer[1983448]; // конец пакета
				}

				bit_buffer_index += m_package.size() + m_index_elements.size();
			
				std::cout <<"bit " <<  bit_buffer_index << '\n';
				std::cout << (int)m_bit_buffer[1983472];
				std::cout << (int)m_bit_buffer[1983473];
				std::cout << (int)m_bit_buffer[1983474];
				std::cout << (int)m_bit_buffer[1983475];
				std::cout << (int)m_bit_buffer[1983476];
				std::cout << (int)m_bit_buffer[1983477];
				std::cout << (int)m_bit_buffer[1983478];
				std::cout << (int)m_bit_buffer[1983479];

				for (auto it : m_package) {
					std::cout << (int)it << ' ';
				}
				m_package.clear();									
				m_index_elements.clear();
				std::cout << "size = " << m_byte_buffer.size() <<'\n';
				m_byte_buffer.clear();										 // <----
				m_byte_buffer.emplace_front(m_bit_buffer[bit_buffer_index]); // <---
				test++;
				
				if (test == 103) {
				//	return;
				}

			}

		}

	}
}

void HDLC_Handler::writeToFileInSigFormat(std::list<uint8_t>& package, const std::string& output_file_path) {
	std::ofstream out_file(output_file_path, std::ios::app | std::ios::binary);
	if (out_file.is_open()) {
		uint16_t package_size = m_package.size() / m_number_of_bits; //ѕ–ќ¬≈–»“№  ј∆ƒџ… ѕј ≈“ Ќј Ѕ»“ќ¬”ё ѕќ—Ћ≈ƒќ¬ј“≈Ћ№Ќќ—“№
		out_file.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));
		int current_bit = 0;
		std::list<uint8_t>current_byte;
		for (auto bit : package) {
			current_byte.emplace_back(bit);
			current_bit++;
			if (current_bit == m_number_of_bits) {
				current_bit = 0;
				unsigned char byte = byteConverter(current_byte);				
				out_file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
				current_byte.clear();
			}
		}
	}
	out_file.close();
}

void HDLC_Handler::removeBitTransparencyFromPackage() {
	
	for (int i = 0; i < m_index_elements.size(); ++i) {
		auto it = m_package.begin();
		std::cout << "”дал€ю элемент # " << m_index_elements[i] <<'\n';
		std::advance(it, m_index_elements[i]);
		m_package.erase(it);
	}

	for (int i = 0; i < m_bit_flag.size(); ++i) {
		m_package.pop_back();
	}
	
}


unsigned char HDLC_Handler::byteConverter(std::list<uint8_t>& buffer) {
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




