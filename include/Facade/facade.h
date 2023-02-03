#pragma once
#include "ProtocolHandler/hdlc_handler.h"
#include <string>

class Facade {
	std::shared_ptr<HDLC_Handler>m_hdlc_handler;
public:
	void exec(const std::string&, const std::string&);
	Facade();
};