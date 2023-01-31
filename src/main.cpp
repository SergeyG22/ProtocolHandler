#include "ProtocolHandler/hdlc_handler.h"

int main(){
	setlocale(LC_ALL, "russian");
	HDLC_Handler handler;
	handler.exec();
	system("pause");
}