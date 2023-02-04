#include "ProtocolHandler/hdlc_handler.h"
#include "Facade/facade.h"

int main(){
	setlocale(LC_ALL,"russian");
	
	std::shared_ptr<Facade>hdlc_handler = std::make_shared<Facade>();
	std::string command;
	while (std::cin >> command) {
		if (command == "start") {
			std::cout << "Wait, in progress....";
			hdlc_handler->exec("../data/HDLC_TEST_.BIT", "../result/HDLC_TEST_RESULT.sig");
			std::cout << "completed!\n";
			return 0;
		}
		std::cout << "invalid command\n";
	}

	system("pause");
	return 0;
}