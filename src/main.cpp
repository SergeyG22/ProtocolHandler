#include "ProtocolHandler/hdlc_handler.h"

int main(){
	HDLC_Handler handler;
	handler.exec();
	/*
	std::vector<int>vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	vec.push_back(7);
	vec.push_back(8);
	vec.push_back(9);
	vec.push_back(10);
	vec.push_back(11);
	vec.push_back(12);
	vec.push_back(13);
	vec.push_back(14);
	vec.push_back(15);
	vec.push_back(16);
	vec.push_back(17);
	vec.push_back(18);
	vec.push_back(19);
	vec.push_back(20);
	vec.push_back(21);
	vec.push_back(22);
	std::vector<int>byte;
	std::copy(vec.begin() + 3, vec.end() - vec.size() + 8 + 3, std::back_inserter(byte));
	byte.clear();
	for (auto it : byte) {
		std::cout << it <<' ';
	}
	*/
	system("pause");
}