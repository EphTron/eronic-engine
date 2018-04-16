#pragma comment (lib, "eronic-engine.lib")
#pragma comment (lib, "ws2_32.lib")

#include <iostream>
#include <thread>
#include <cstring>
#include <sstream>

#include "TCPClient.h"
#include "UDPClient.h"
#include "TCPListener.h"
#include "UDPListener.h"
#include "Socket_WIN.h"
#include "PeerNode.h"

#define MAX_CONN	3

eronic::UDPClient * setup_udp_sender(std::string ip, int port);
eronic::TCPClient * setup_tcp_sender(int port);
eronic::UDPListener* setup_udp_listener(std::string ip, int port);
eronic::TCPListener* setup_tcp_listener(int port, int alt_ports);
void listen_for_connections(eronic::TCPListener * l, eronic::TCPClient * conns[], int conns_length, bool * flag);

int main() {

	bool flag = true;
	int app_port = 9173;
	
	eronic::PeerNode * peer = new eronic::PeerNode(app_port,10);
	peer->find_networks(6000, true);
	peer->run_peer_network();

	system("Pause");
	return 0;
}

//eronic::UDPListener * a = setup_udp_listener((std::string)"ADDR_ANY", listen_port);
////eronic::UDPClient * b = setup_udp_sender((std::string)"150.237.93.255", listen_port);
//while (true) {
//	//b->send("1", 1);
//	a->set_blocking(true);
//	char sender_ip[INET_ADDRSTRLEN];
//	char recv[1];
//	int res = a->receivefrom(recv, 1, sender_ip);
//	
//	std::cout << res << "received from" << recv << " " << sender_ip << std::endl;
//	Sleep(500);
//}


eronic::TCPListener* setup_tcp_listener(int port, int alt_ports) {
	eronic::TCPListener * listener = new eronic::TCPListener(new eronic::Socket_WIN());
	int listen_port = port;
	int binding_result = -1;
	while (listen_port <= port + alt_ports && binding_result != 0) {
		binding_result = listener->bind((std::string)"ADDR_ANY", listen_port, false);
		if (binding_result == 10048) {
			listen_port++;
			std::cout << "increase port to " << listen_port << std::endl;
		}
		else if (binding_result != 0) {
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
	//eronic::Package* temp;
	while (conn_idx < conns_length && *flag) {
		std::cout << "accepting... " << conn_idx << std::endl;
		if ((conns[conn_idx] = l->accept()) != nullptr) {
			std::cout << "Connection found !" << std::endl;
			
			char data[3];
			//temp = new eronic::Package;

			if (conns[conn_idx]->receive(data, 128) == 0) {
				//eronic::deserialize(data, temp);
				//std::cout << "Received msg: " << temp->message << std::endl;
			}
			conn_idx++;
		}
		else {
			//std::cout << "no connection found" << std::endl;
		}
	}
}


eronic::TCPClient * setup_tcp_sender(int port) {
	eronic::TCPClient * tcp_sender = new eronic::TCPClient(new eronic::Socket_WIN());
	std::cout << "Created tcp_sender. Connecting... " << port << std::endl;
	if (tcp_sender->connect((std::string)"127.0.0.1", port, false) == 0) {
		std::cout << "Connected to port: " << port << std::endl;
	}
	else {
		std::cout << "Error: TCPClient could not connect." << std::endl;
	}
	return tcp_sender;
}

eronic::UDPClient * setup_udp_sender(std::string ip, int port) {
	eronic::UDPClient * udp_sender = new eronic::UDPClient(new eronic::Socket_WIN());
	std::cout << "Created udp_sender. Connecting... " << port << std::endl;
	if (udp_sender->connect(ip, port, false) == 0) {
		std::cout << "Connected to port: " << port << std::endl;
	}
	else {
		std::cout << "Error: UDPClient could not connect." << std::endl;
	}
	return udp_sender;
}

eronic::UDPListener* setup_udp_listener(std::string ip, int port) {
	eronic::UDPListener * listener = new eronic::UDPListener(new eronic::Socket_WIN());
	int listen_port = port;
	int binding_result = -1;
	binding_result = listener->bind(ip, listen_port, false);
	if (binding_result == 10048) {
		std::cout << "Port used already" << listen_port << std::endl;
	}
	else if (binding_result == SOCKET_ERROR) {
		std::cout << "Error binding listener on port " << listen_port << std::endl;
	}
	else {
		std::cout << "Bound UDP listener with result " << binding_result  << " to port " << listener->get_address()->get_port() << std::endl;
	}
	return listener;
}

void send_and_receive_loop() {
	eronic::UDPListener* a = setup_udp_listener((std::string)"ADDR_ANY", 9555);
	a->set_blocking(true);

	eronic::UDPClient * b = setup_udp_sender((std::string)"150.237.93.255", 9555);
	b->set_blocking(true);

	eronic::DataPackage* temp;
	while (0) {
		// prepare data package
		eronic::DataPackage dp = eronic::DataPackage();
		char c[sizeof(eronic::DataPackage)];
		dp.type = 1;
		dp.network_id = 2;
		dp.int_data_1 = 3;
		dp.set_ip((std::string)"127.0.0.1");
		dp.serialize(c);
		// send package
		int res_s = b->send(c, sizeof(eronic::DataPackage));

		//receive package
		char r[sizeof(eronic::DataPackage)];
		int res_r = a->receive(r, sizeof(eronic::DataPackage));
		//deserialize it
		temp = new eronic::DataPackage(r);
		std::cout << "recv res " << res_r << " " << temp->sender_ip << std::endl;
		Sleep(500);
	}
}


///////////////////////////////////////////////////////////////////
// OLD Code snippets that might be useful (but probably not)
/* OLD MAIN CODE
//eronic::get_external_ip((std::string)"api.ipify.org");

// setup connections
eronic::TCPClient * connections[MAX_CONN] = { nullptr };
// setup serializer
eronic::Package * package = new eronic::Package();

bool flag = true;
int listen_port = 9171;
// setup listener and sender
eronic::UDPClient * udp_sender = setup_udp_sender((std::string)"127.0.0.1", listen_port);
eronic::UDPListener * listener = setup_udp_listener(listen_port, 1);
//std::thread t1(start_echo, listener, &flag);
start_echo(listener, &flag);

eronic::Package* data_pack = new eronic::Package;
char data[PACKAGE_SIZE];
for (int i = 0; i < 10; ++i) {
	std::ostringstream convert;
	convert << "hello " << i << "\0";
	std::string msg = convert.str().c_str();
	create_package(data, msg, 1, 3);
	udp_sender->send(data, PACKAGE_SIZE);
}

//setup tcp listener
eronic::TCPListener * tcp_listener = setup_tcp_listener(listen_port, 3);
listen_port = tcp_listener->get_address()->get_port();
//listening_thread(listen_for_connections, tcp_listener, connections, MAX_CONN, &flag);

eronic::TCPClient * tcp_sender = new eronic::TCPClient(new eronic::Socket_WIN());
std::cout << "Created tcp_sender. Connecting... " << listen_port << std::endl;
if (tcp_sender->connect((std::string)"127.0.0.1", listen_port) == 0) {
	std::cout << "Connected to port: " << listen_port << std::endl;

	eronic::Package* data_pack = new eronic::Package;
	data_pack->type = 1;
	data_pack->sender = 2;
	strcpy_s(data_pack->message, 128, "hello from sender\0");
	eronic::print_package(data_pack);

	char data[PACKAGE_SIZE];
	eronic::serialize(data_pack, data);
	tcp_sender->send(data, PACKAGE_SIZE);
}
else {
	std::cout << "Error: TCPClient could not connect." << std::endl;
}

Sleep(200);
flag = false;
t1.join();
listening_thread.join();

std::cout << "JOINED" << std::endl;

int c = 0;
for (auto con : connections) {
	std::cout << " c = " << con << std::endl;
	if (con != nullptr) {
		c++;
	}
}
std::cout << c << " connections accepted" << std::endl;
*/