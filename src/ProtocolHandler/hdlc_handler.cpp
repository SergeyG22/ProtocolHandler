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
	
	if ((bit_package_index - bit_buffer_index + m_number_of_bits) - m_numbers_of_delete_bit.size() == 32) {
	//	return 35; // 31 = 1 // 32 = 1 // 33 = 0 // 34 = 1 // 35 = 1
	}
	/*
	if ((bit_package_index - bit_buffer_index + m_number_of_bits) - m_numbers_of_delete_bit.size() == 284) {
		return 285;
	}
	if ((bit_package_index - bit_buffer_index + m_number_of_bits) - m_numbers_of_delete_bit.size() == 290) {
		return 291;
	}
	*/
	//std::cout << "Размер контейнера = " << m_numbers_of_delete_bit.size() <<'\n';
	return (bit_package_index - bit_buffer_index + m_number_of_bits) - m_numbers_of_delete_bit.size();
}

void HDLC_Handler::addBitToPackage(std::vector<uint8_t>& m_bit_buffer, int current_index) {
//	std::cout << (int)m_bit_buffer[current_index];
	m_package.push_back(m_bit_buffer[current_index]);
}

void HDLC_Handler::addBitToByteBuffer(uint8_t bit) {
	m_byte_buffer.emplace_front(bit);
}

void HDLC_Handler::MakeStepInSequenceOfBitBuffer(std::list<int>& bit_sequence, int& current_index) {
	bit_sequence.pop_front();	
	current_index++;
	bit_sequence.push_back(m_bit_buffer[current_index]);
}

bool HDLC_Handler::checkSequenceforDuplicate(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::vector<int>next_bit_flag;
	std::copy(m_bit_buffer.begin() + bit_package_index, m_bit_buffer.begin() + bit_package_index + m_bit_stuffing_flag.size(), std::back_inserter(next_bit_flag));
	if (std::equal(next_bit_flag.begin(), next_bit_flag.end(), m_bit_stuffing_flag.begin(), m_bit_stuffing_flag.end())) {
		m_numbers_of_delete_bit.push_back(drop_element_index + m_bit_stuffing_flag.size()); //39
	//	std::cout << "Дубликат с индексом = " << drop_element_index + m_bit_stuffing_flag.size() <<'\n';
		return true;
	}
	return false; 
}

int HDLC_Handler::checkSequenceForFirstEntryBitFlag(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::list<uint8_t>m_bit_stuffing_reverce_flag{ 1,1,1,1,1,0 };
	if (m_package.size() == m_bit_stuffing_reverce_flag.size() && 
		std::equal(m_package.begin(), m_package.end(), m_bit_stuffing_reverce_flag.begin(), m_bit_stuffing_reverce_flag.end())) {
		m_numbers_of_delete_bit.push_back(m_bit_stuffing_reverce_flag.size() - 1);
		checkSequenceforDuplicate(bit_package_index - 1, bit_buffer_index, drop_element_index);
	}
	return m_bit_stuffing_reverce_flag.size() - 1;
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

void HDLC_Handler::shiftIndexOfBufferBit(int& bit_buffer_index) {
	bit_buffer_index += m_package.size() + m_numbers_of_delete_bit.size();
}

void HDLC_Handler::selectPackagesFromBitBuffer(const std::string& output_file_path) {
	int test = 0;
	for (int bit_buffer_index = getFirstFlagBit(m_bit_frame_flag, m_bit_buffer); bit_buffer_index < m_bit_buffer.size(); ++bit_buffer_index) {
		
		if (m_byte_buffer.size() != m_number_of_bits) {								
			m_byte_buffer.emplace_front(m_bit_buffer[bit_buffer_index]);			
		}
		else { 		
			bool equal_current_byte_with_flag = std::equal(m_byte_buffer.begin(), m_byte_buffer.end(), m_bit_frame_flag.begin(), m_bit_frame_flag.end());
			m_byte_buffer.clear();													 			
			addBitToByteBuffer(m_bit_buffer[bit_buffer_index]);

			if (!equal_current_byte_with_flag) {
				int bit_package_index = bit_buffer_index - m_number_of_bits;		
				std::list<int>bit_sequence;
				for (int bit_step = 0; bit_step < m_bit_stuffing_flag.size(); ++bit_step) {						
					bit_sequence.push_back(m_bit_buffer[bit_package_index]);
					addBitToPackage(m_bit_buffer, bit_package_index);
					incrementPackageIndex(bit_package_index);
				}
				while (true) {														
					checkSequenceForFirstEntryBitFlag(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
					addBitToPackage(m_bit_buffer, bit_package_index);
					MakeStepInSequenceOfBitBuffer(bit_sequence, bit_package_index);						
					if (std::equal(bit_sequence.begin(), bit_sequence.end(), m_bit_stuffing_flag.begin(), m_bit_stuffing_flag.end())) {

					addBitToPackage(m_bit_buffer, bit_package_index);
						incrementPackageIndex(bit_package_index);

						if (m_bit_buffer[bit_package_index] == 0) {		//ВСЯ ШНЯГА ПРОИСХОДИТ ПОСЛЕ ВЫЗОВА ФУНКЦИИ С ДУБЛИКАТОМ
																							//получаем индекс удаляемого бита

						//	checkSequenceforDuplicate(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
						//	m_numbers_of_delete_bit.push_back(getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
						//	continue;	

						//	int index = getIndexOfDeletedBit(bit_package_index, bit_buffer_index);
						//	std::cout << "Получаемый индекс = " << index << '\n';
						//	checkSequenceforDuplicate(bit_package_index, bit_buffer_index, index);
						//	m_numbers_of_delete_bit.push_back(getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
						//	continue;	

							if (checkSequenceforDuplicate(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index))) {
								m_numbers_of_delete_bit.push_back(getIndexOfDeletedBit(bit_package_index, bit_buffer_index) + 1);
								continue;
							}
							m_numbers_of_delete_bit.push_back(getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
							continue;

						}

						if (m_bit_buffer[bit_package_index] == 1) {			
							if (m_bit_buffer[bit_package_index + 1] == 0) { 
								removeBitTransparencyFromPackage();								
								writeToFileInSigFormat(m_package, output_file_path);
								break;
							}
						}

					}
				}				
				shiftIndexOfBufferBit(bit_buffer_index);
				clearAllBuffers();
				addBitToByteBuffer(m_bit_buffer[bit_buffer_index]);
				test++;
			//	std::cout << '\n';
				if (test == 100) {
				//	break;
				}
			}

		}

	}
}

void HDLC_Handler::writeToFileInSigFormat(std::list<uint8_t>& package, const std::string& output_file_path) {
	std::ofstream out_file(output_file_path, std::ios::app | std::ios::binary);
	if (out_file.is_open()) {
		uint16_t package_size = getPackageSize();
		out_file.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));
		int current_bit = 0;
		std::list<uint8_t>current_byte;
		for (auto get_bit : package) {
			current_byte.emplace_back(get_bit);
			current_bit++;
			if (current_bit == m_number_of_bits) {
				current_bit = 0;
				unsigned char byte = BitToByteConverter(current_byte);				
				out_file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
				current_byte.clear();
			}
		}
	}
	out_file.close();
}

void HDLC_Handler::removeBitTransparencyFromPackage() {
	
	for (int i = 0; i < m_numbers_of_delete_bit.size(); ++i) {
		auto it = m_package.begin();
		std::advance(it, m_numbers_of_delete_bit[i]);
	//	std::cout << "Индекс удаляемого элемента = " << m_numbers_of_delete_bit[i] << '\n';
	//	std::cout << "Удаляемый элемент = " << (int)*it << '\n';
		m_package.erase(it);
	}
	std::cout << '\n';
	removeTailPackageWithFlag();
}

void HDLC_Handler::removeTailPackageWithFlag() {
	for (int i = 0; i < m_bit_stuffing_flag.size(); ++i) {
		m_package.pop_back();
	}
}

int HDLC_Handler::getFirstFlagBit(const std::vector<uint8_t>& flag, const std::vector<uint8_t>& bit_buffer) {
	for (int current_bit = 0; current_bit < bit_buffer.size(); ++current_bit) {
		std::vector<uint8_t>byte;
		std::copy(bit_buffer.begin() + current_bit, bit_buffer.begin() + current_bit + flag.size(), std::back_inserter(byte));
		if (std::equal(flag.begin(), flag.end(), byte.begin(), byte.end())) {
			return current_bit;
		}
	}
	return 0;
}

void HDLC_Handler::clearAllBuffers() {
	m_package.clear();
	m_numbers_of_delete_bit.clear();
	m_byte_buffer.clear();
}


unsigned char HDLC_Handler::BitToByteConverter(std::list<uint8_t>& buffer) {
	std::list<unsigned char>result((m_number_of_bits + 7) >> 3);
	for (int j = 0; j < int(m_number_of_bits); j++) {
		auto buffer_it = buffer.begin();
		auto result_it = result.begin();
		int pos = j >> 3;
		std::advance(result_it, pos);
		std::advance(buffer_it, j);
		*result_it |= (*buffer_it << (j & 7));
	}
	return result.front();
}




