#include "ProtocolHandler/hdlc_handler.h"

void HDLC_Handler::exec(const std::string& input_file_path, const std::string& output_file_path) {	
	std::ifstream file(input_file_path, std::ifstream::in | std::ifstream::binary);	
	fillBuffer(file);
	selectPackagesFromBuffer(output_file_path);
	file.close();
}

void HDLC_Handler::fillBuffer(std::ifstream& file) {
	unsigned char current_bite;
	while (!file.eof()) {
		file.read((char*)&current_bite, sizeof(current_bite));
		std::bitset<8>buffer(current_bite);
		for (int i = 0; i < m_number_of_bits; ++i) {
			m_bit_buffer.emplace_back(buffer[i]);
		}
	}
}

void HDLC_Handler::selectPackagesFromBuffer(const std::string& output_file_path) {
	for (int i = m_ignore_start_bits; i < m_bit_buffer.size(); ++i) {
		if (m_byte_buffer.size() != m_number_of_bits) {
			m_byte_buffer.emplace_front(m_bit_buffer[i]);
		}
		else {
			std::string hex = static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << int(byteConverterLst(m_byte_buffer))).str();
			m_byte_buffer.clear();
			m_byte_buffer.emplace_front(m_bit_buffer[i]);

			if (hex != m_frame_border) {
				int current_index = i - 8;
				std::list<int>bit_sequence;
				for (int i = 0; i < 6; ++i) {
					bit_sequence.push_back(m_bit_buffer[current_index]);
					m_package.push_back(m_bit_buffer[current_index]);
					current_index++;
				}
				while (true) {
					bit_sequence.pop_front();
					m_package.push_back(m_bit_buffer[current_index]);
					current_index++;
					bit_sequence.push_back(m_bit_buffer[current_index]);
					if (std::equal(bit_sequence.begin(), bit_sequence.end(), m_stuff_combination.begin(), m_stuff_combination.end())) {
						m_package.push_back(m_bit_buffer[current_index]);
						current_index++;
						if (m_bit_buffer[current_index] == 0) {
							int drop_element_index = (current_index - i + 8);
							drop_element_index -= m_index_elements.size();	// When deleting elements, the package container is compressed and the indexes are shifted
							m_index_elements.push_back(drop_element_index);
							continue;
						}

						if (m_bit_buffer[current_index] == 1) {
							if (m_bit_buffer[current_index + 1] == 0) {
								for (int i = 0; i < m_index_elements.size(); ++i) {
									auto it = m_package.begin();
									std::advance(it, m_index_elements[i]);
									m_package.erase(it);

								}

								for (int i = 0; i < m_stuff_combination.size(); ++i) {
									m_package.pop_back();
								}

								uint16_t package_size = m_package.size() / m_number_of_bits;
								writeInSigFormat(package_size, m_package, output_file_path);

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

void HDLC_Handler::writeInSigFormat(uint16_t& package_size, std::list<uint32_t>& package, const std::string& output_file_path) {

	std::ofstream out(output_file_path, std::ios::app | std::ios::binary);
	out.write(reinterpret_cast<const char*>(&package_size), sizeof(package_size));

	int current_bit = 0;
	std::list<uint32_t>current_byte;
	for (auto it : package) {
		current_byte.emplace_back(it);
		current_bit++;
		if (current_bit == m_number_of_bits) {
			current_bit = 0;
			unsigned char byte = byteConverterLst(current_byte);
			out.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
			current_byte.clear();
		}
	}
}

unsigned char HDLC_Handler::byteConverterLst(std::list<uint32_t>& buffer) {
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




