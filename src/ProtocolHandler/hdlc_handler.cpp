#include "ProtocolHandler/hdlc_handler.h"

int HDLC_Handler::exec(const std::string& input_file_path, const std::string& output_file_path) {
	if (fillBitBuffer(input_file_path)) {
		return 1;
	}
	selectPackagesFromBitBuffer(output_file_path);
	return 0;
}

void HDLC_Handler::addBitToPackage(std::vector<uint8_t>& m_bit_buffer, int current_index) {
	m_package.push_back(m_bit_buffer[current_index]);
}

void HDLC_Handler::MakeStepInSequenceOfBuffer(std::list<int>& bit_sequence, int& current_index) {
	bit_sequence.pop_front();	
	current_index++;
	bit_sequence.push_back(m_bit_buffer[current_index]);
}

int HDLC_Handler::checkSequenceforDuplicate(int current_index, int i) {
	std::vector<int>next_bit_flag;
	std::copy(m_bit_buffer.begin() + current_index, m_bit_buffer.begin() + current_index + m_bit_flag.size(), std::back_inserter(next_bit_flag));
	if (std::equal(next_bit_flag.begin(), next_bit_flag.end(), m_bit_flag.begin(), m_bit_flag.end())) {
		int drop_element_index = current_index - i + m_number_of_bits + 1;
		m_index_elements.push_back(drop_element_index);
		return drop_element_index;
	}
	return 0;
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
	for (int i = m_ignore_start_bits; i < m_bit_buffer.size(); ++i) {
		if (m_byte_buffer.size() != m_number_of_bits) {
			m_byte_buffer.emplace_front(m_bit_buffer[i]);
		}
		else {
			std::string hex = (std::stringstream() << "0x" << std::hex << int(byteConverter(m_byte_buffer))).str();
			m_byte_buffer.clear();
			m_byte_buffer.emplace_front(m_bit_buffer[i]);

			if (hex != m_frame_border) {
				int current_index = i - m_number_of_bits;
				std::list<int>bit_sequence;
				for (int i = 0; i < m_bit_flag.size(); ++i) {
					bit_sequence.push_back(m_bit_buffer[current_index]);
					addBitToPackage(m_bit_buffer, current_index);
					current_index++;
				}
				while (true) {
					addBitToPackage(m_bit_buffer, current_index);
					MakeStepInSequenceOfBuffer(bit_sequence, current_index);

					if (std::equal(bit_sequence.begin(), bit_sequence.end(), m_bit_flag.begin(), m_bit_flag.end())) {
						addBitToPackage(m_bit_buffer, current_index);
						current_index++;
						if (m_bit_buffer[current_index] == 0) {
							int drop_element_index = (current_index - i + m_number_of_bits);
							drop_element_index -= m_index_elements.size();	// When deleting elements, the package container is compressed and the indexes are shifted
							checkSequenceforDuplicate(current_index, i);
							m_index_elements.push_back(drop_element_index);
							continue;
						}

						if (m_bit_buffer[current_index] == 1) {
							if (m_bit_buffer[current_index + 1] == 0) {
								removeBitTransparencyFromPackage();
								writeToFileInSigFormat(m_package, output_file_path);
								break;
							}
						}

					}
				}

				i += m_package.size() + m_index_elements.size();
				m_package.clear();
				m_index_elements.clear();
			}

		}

	}
}

void HDLC_Handler::writeToFileInSigFormat(std::list<uint8_t>& package, const std::string& output_file_path) {
	std::ofstream out_file(output_file_path, std::ios::app | std::ios::binary);
	if (out_file.is_open()) {
		uint16_t package_size = m_package.size() / m_number_of_bits;
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




