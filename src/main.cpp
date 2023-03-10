#include "ProtocolHandler/hdlc_handler.h"

int main(){
	setlocale(LC_ALL,"russian");
	std::shared_ptr<HDLC_Handler>hdlc_handler = std::make_shared<HDLC_Handler>();

	std::string command;
	while (std::cin >> command) {
		if (command == "start") {
			std::cout << "Wait, in progress....\n";
			hdlc_handler->exec("../data/HDLC_TEST_.BIT", "../result/HDLC_TEST_RESULT.sig");
			std::cout <<"correct packages = " << hdlc_handler->getNumberOfCorrectPackets() << '\n';
			std::cout <<"broken packages = " << hdlc_handler->getNumberOfBrokenPackets() << '\n';
			std::cout << "completed!\n";
			system("pause");
			return 0;
		}
		std::cout << "invalid command\n";
	}
	
	system("pause");
	return 0;
}
