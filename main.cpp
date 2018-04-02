#include <iostream>
#include "TransferSocket_WIN.h"
#include "ListeningSocket_WIN.h"

int main() {
	eronic::TransferSocket* s = new eronic::TransferSocket_WIN();
	eronic::ListeningSocket* l = new eronic::ListeningSocket_WIN(1);
	std::string listen_to_any = "ANY";
	l->bind_to(listen_to_any, 9171);
	std::string a = "127.0.0.1";
	s->connect_to(a, 9171);
	char buffer;

	if (s->send_to("11", 2)) {
		std::cout << "sent something" << std::endl;
	}
	else if (s->receive_from(&buffer, 1)) {
		std::cout << " y es " << buffer << std::endl;
	}
	else {
		std::cerr << " PROBLEM " << WSAGetLastError() << std::endl;
	}



	std::cout << "HALLO";
	eronic::TransferSocket* r = l->accept_connection();
	r->send_to("1", 1);
	system("Pause");

	return 0;
}

