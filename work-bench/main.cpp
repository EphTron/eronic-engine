#pragma comment (lib, "eronic-engine.lib")
#pragma comment (lib, "ws2_32.lib")

#include <iostream>
#include <thread>
#include <cstring>

#include "TCPClient.h"
#include "UDPClient.h"
#include "TCPListener.h"
#include "Serializer.h"
#include "Socket_WIN.h"

#define MAX_CONN	3

#define PACKAGE_SIZE sizeof(eronic::Package)

eronic::TCPListener* setup_listener(int port, int alt_ports);
void create_sender(int connect_port);
void listen_for_connections(eronic::TCPListener * l, eronic::TCPClient * conns[], int conns_length, bool * flag);
eronic::TCPClient * setup_sender(int port);

int main() {

	// setup connections
	eronic::TCPClient * connections[MAX_CONN] = { nullptr };

	// setup serializer
	eronic::Package * package = new eronic::Package();
	eronic::Serializer packer = eronic::Serializer();
	int data_size = packer.get_package_size();
	char* data;
	bool flag = true;
	int listen_port = 9171;

	//setup listener
	eronic::TCPListener * listener = setup_listener(listen_port, 3);
	listen_port = listener->get_address()->get_port();
	std::thread listening_thread(listen_for_connections, listener, connections, MAX_CONN, &flag);
	
	eronic::TCPClient * sender = new eronic::TCPClient(new eronic::Socket_WIN());
	std::cout << "Created sender. Connecting... " << listen_port << std::endl;
	if (sender->connect((std::string)"127.0.0.1", listen_port) == 0) {
		std::cout << "Connected to port: " << listen_port << std::endl;

		eronic::Package* data_pack = new eronic::Package;
		data_pack->type = 1;
		data_pack->sender = 2;
		///strcpy(data_pack->message, "hello from sender\0"); 
		strcpy_s(data_pack->message, 128, "hello from sender\0");
		packer.print_package(data_pack);

		char data[PACKAGE_SIZE];
		packer.serialize(data_pack, data);
		sender->send(data, PACKAGE_SIZE);
	}
	else {
		std::cout << "Error: TCPClient could not connect." << std::endl;
	}
	
	Sleep(200);
	//std::thread sender_thread(setup_sender, 9171);
	flag = false;
	//listening_thread.join();

	std::cout << "JOINED" << std::endl;

	int c = 0;
	for (auto con : connections) {
		std::cout << " c = " << con << std::endl;
		if (con != nullptr) {
			c++;
		}
	}
	std::cout << c << " connections accepted" << std::endl;
	system("Pause");
	return 0;
}

eronic::TCPListener* setup_listener(int port, int alt_ports) {
	eronic::TCPListener * listener = new eronic::TCPListener(new eronic::Socket_WIN());
	int listen_port = port;
	int binding_result = -1;
	while (listen_port <= port + alt_ports && binding_result != 0) {
		binding_result = listener->bind((std::string)"ANY", listen_port);
		if (binding_result == 10048) {
			listen_port++;
			std::cout << "increase port to " << listen_port << std::endl;
		}
		else if (binding_result == SOCKET_ERROR) {
			std::cout << "Error binding listener on port " << listen_port << std::endl;
		}
		else {
			std::cout << "Bound listener with result " << binding_result << std::endl;
			std::cout << "Juicy port " << listener->get_address()->get_port() << std::endl;
		}
	}
	return listener;
}

void listen_for_connections(eronic::TCPListener * l, eronic::TCPClient * conns[], int conns_length, bool * flag) {
	int start_result = l->start(3);
	if (start_result == 0)
		std::cout << "Started listening on port: " << l->get_address()->get_ip() << ":"
		<< l->get_address()->get_port() << std::endl;
	else
		std::cout << "Error " << start_result << std::endl;

	int conn_idx = 0;
	while (conn_idx < conns_length && *flag) {
		std::cout << "accepting... " << conn_idx << std::endl;
		if ((conns[conn_idx] = l->accept()) != nullptr) {
			std::cout << "Connection found !!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			
			char data[PACKAGE_SIZE];
			eronic::Package* temp = new eronic::Package;

			if (conns[conn_idx]->receive(data, 128) == 0) {
				eronic::deserialize(data, temp);
				std::cout << "Received msg: " << temp->message << std::endl;
			}
			conn_idx++;
		}
		else {
			//std::cout << "no connection found" << std::endl;
		}
	}
}

void create_sender(int connect_port) {
	eronic::TCPClient * sender = new eronic::TCPClient(new eronic::Socket_WIN());
	if (sender->connect((std::string)"127.0.0.1", connect_port) == 0) {
		std::cout << "Connected to port: " << connect_port << std::endl;
	}
	else {
		std::cout << "Error: TCPClient could not connect." << std::endl;
	}

	if (sender->send("1", 1) == SOCKET_ERROR) {
		std::cout << "error while sending" << std::endl;
	}
	else {
		std::cout << "sent message" << std::endl;
	}

	char* receive_buffer;
	if (sender->receive(&receive_buffer, sizeof(*receive_buffer) == SOCKET_ERROR)) {
		std::cout << "error while receiving" << std::endl;
	}
	else {
		std::cout << "received message " << receive_buffer << std::endl;
	}
}

eronic::TCPClient * setup_sender(int port) {
	eronic::TCPClient * sender1 = new eronic::TCPClient(new eronic::Socket_WIN());
	std::cout << "Created sender1. Connecting... " << port << std::endl;
	if (sender1->connect((std::string)"127.0.0.1", port) == 0) {
		std::cout << "Connected to port: " << port << std::endl;
	}
	else {
		std::cout << "Error: TCPClient could not connect." << std::endl;
	}
	return sender1;
}
