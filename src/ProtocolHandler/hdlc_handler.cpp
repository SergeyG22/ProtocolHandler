#include "ProtocolHandler/hdlc_handler.h"
#include "CRC/CRC.h"

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
	return (bit_package_index - bit_buffer_index + m_number_of_bits) - m_numbers_of_delete_bit.size();
}

void HDLC_Handler::addBitToPackage(std::vector<uint8_t>& m_bit_buffer, int current_index) {
	m_package.emplace_back(m_bit_buffer[current_index]);
}

void HDLC_Handler::addBitToByteBuffer(uint8_t bit) {
	m_byte_buffer.emplace_front(bit);
}

void HDLC_Handler::MakeStepInSequenceOfBitBuffer(std::list<int>& bit_sequence, int& current_index) {
	bit_sequence.pop_front();
	current_index++;
	bit_sequence.emplace_back(m_bit_buffer[current_index]); 
}

bool HDLC_Handler::isBitBufferLimit(int bit_package_index, std::vector<uint8_t>& m_bit_buffer) const {
	if (bit_package_index == m_bit_buffer.size() - 1) {	
		return true;
	}
	return false;
}

bool HDLC_Handler::checkSequenceforDuplicate(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::vector<int>next_bit_flag;
	std::copy(m_bit_buffer.begin() + bit_package_index, m_bit_buffer.begin() + bit_package_index + m_bit_stuffing_flag.size(), std::back_inserter(next_bit_flag));
	if (std::equal(next_bit_flag.begin(), next_bit_flag.end(), m_bit_stuffing_flag.begin(), m_bit_stuffing_flag.end())) {
		m_numbers_of_delete_bit.emplace_back(drop_element_index + m_bit_stuffing_flag.size());
		return true;
	}
	return false; 
}

int HDLC_Handler::checkSequenceForFirstEntryBitFlag(int bit_package_index, int bit_buffer_index, int drop_element_index) {
	std::list<uint8_t>m_bit_stuffing_reverce_flag{ 1,1,1,1,1,0 };
	if (m_package.size() == m_bit_stuffing_reverce_flag.size() && 
		std::equal(m_package.begin(), m_package.end(), m_bit_stuffing_reverce_flag.begin(), m_bit_stuffing_reverce_flag.end())) {
		m_numbers_of_delete_bit.emplace_back(m_bit_stuffing_reverce_flag.size() - 1);
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
					bit_sequence.emplace_back(m_bit_buffer[bit_package_index]);
					addBitToPackage(m_bit_buffer, bit_package_index);
					incrementPackageIndex(bit_package_index);
				}
				while (true) {		
					checkSequenceForFirstEntryBitFlag(bit_package_index, bit_buffer_index, getIndexOfDeletedBit(bit_package_index, bit_buffer_index));
					addBitToPackage(m_bit_buffer, bit_package_index);		
					if (isBitBufferLimit(bit_package_index, m_bit_buffer)) {
						return;
					}
					MakeStepInSequenceOfBitBuffer(bit_sequence, bit_package_index);					
					if (std::equal(bit_sequence.begin(), bit_sequence.end(), m_bit_stuffing_flag.begin(), m_bit_stuffing_flag.end())) {
					addBitToPackage(m_bit_buffer, bit_package_index);
						incrementPackageIndex(bit_package_index);

						if (m_bit_buffer[bit_package_index] == 0) {
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
								allocateBytesFromPackage(m_package);
								if (!dataCheckByCRC()) {
									break;
								}
								writeToFileInSigFormat(output_file_path);
								break;
							}
						}

					}
				}			

				shiftIndexOfBufferBit(bit_buffer_index);
				clearAllBuffers();
				addBitToByteBuffer(m_bit_buffer[bit_buffer_index]);

			}

		}

	}
}

void HDLC_Handler::writeToFileInSigFormat(const std::string& output_file_path) {
	std::ofstream out_file(output_file_path, std::ios::app | std::ios::binary);
	if (out_file.is_open()) {
		uint16_t package_size = getPackageSize();
		out_file.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));
		for (auto byte : m_allocate_bytes_from_package) {
			out_file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
		}
	}
	out_file.close();
}

void HDLC_Handler::removeBitTransparencyFromPackage() {	
	for (int bit_index = 0; bit_index < m_numbers_of_delete_bit.size(); ++bit_index) {
		auto it = m_package.begin();
		std::advance(it, m_numbers_of_delete_bit[bit_index]);
		m_package.erase(it);
	}
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
	m_allocate_bytes_from_package.clear();
}

bool HDLC_Handler::dataCheckByCRC() {
	int byte_index = 0;
	int size_of_fcs_in_bytes = 2;
	std::shared_ptr<unsigned char[]> package_without_FCS(new unsigned char[m_allocate_bytes_from_package.size() - size_of_fcs_in_bytes], std::default_delete<unsigned char[]>());
	for (auto byte : m_allocate_bytes_from_package) {		
		if (byte_index <= m_allocate_bytes_from_package.size() - size_of_fcs_in_bytes) {
			package_without_FCS[byte_index] = (int)byte;
			byte_index++;
		}
	}
	std::uint16_t first_hex_FCS = m_allocate_bytes_from_package[m_allocate_bytes_from_package.size() - 1]; 
	std::uint16_t second_hex_FCS = m_allocate_bytes_from_package[m_allocate_bytes_from_package.size() - 2];
	std::string first_hex_FCS_str = (std::stringstream() << std::hex << first_hex_FCS).str();
	std::string second_hex_FCS_str = (std::stringstream() << std::hex << second_hex_FCS).str();

	if (first_hex_FCS_str[0] == '0') {
		first_hex_FCS_str.pop_back();
	}

	if (second_hex_FCS_str.size() == 1) {
		second_hex_FCS_str.insert(0,"0");		
	}

	std::string FCS_str;
	FCS_str += first_hex_FCS_str;
	FCS_str += second_hex_FCS_str;

	std::uint16_t hex_CRC = CRC::CalculateBits(package_without_FCS.get(), (m_allocate_bytes_from_package.size() - size_of_fcs_in_bytes) * m_number_of_bits, CRC::CRC_16_X25());	
	std::string CRC_str = (std::stringstream() << std::hex << hex_CRC).str();
	
	if (FCS_str == CRC_str) {
		m_packet_counter.first += 1;
		return true;
	}
	m_packet_counter.second += 1;

	return false;
}

void HDLC_Handler::allocateBytesFromPackage(std::list<uint8_t>& package) {
	int current_bit = 0;
	std::list<uint8_t>current_byte;
	for (auto get_bit : package) {
		current_byte.emplace_back(get_bit);
		current_bit++;
		if (current_bit == m_number_of_bits) {
			current_bit = 0;
			m_allocate_bytes_from_package.emplace_back(BitToByteConverter(current_byte));			
			current_byte.clear();
		}
	}
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



