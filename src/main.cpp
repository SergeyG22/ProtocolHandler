#include <iostream>
#include <fstream>
#include <string>
#include "ProtocolHandler/hdlc_handler.h"

int main(){
	HDLC_Handler handler;
	handler.print();
	/*
	std::cout << "start initialization\n";
	std::string line;
	std::ifstream file("../HDLC_sources/file.txt");

	if (file.is_open()) {
		while (std::getline(file,line)) {
			std::cout << line << '\n';
		}
	}
	else {
		std::cout << "file is not opened\n";
	}
	file.close();
	*/
	system("pause");
}