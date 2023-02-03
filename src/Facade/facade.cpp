#include "Facade/facade.h"

Facade::Facade() {
	m_hdlc_handler = std::make_shared<HDLC_Handler>();
}

void Facade::exec(const std::string& input_file_path, const std::string& output_file_path) {
	m_hdlc_handler->exec(input_file_path, output_file_path);
}