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
#include "Serializer.h"
#include "Socket_WIN.h"
#include "PeerNode.h"

#define MAX_CONN	3
#define PACKAGE_SIZE sizeof(eronic::Package)

eronic::TCPListener* setup_tcp_listener(int port, int alt_ports);
eronic::UDPListener* setup_udp_listener(int port, int alt_ports);
void listen_for_connections(eronic::TCPListener * l, eronic::TCPClient * conns[], int conns_length, bool * flag);
void start_echo(eronic::UDPListener * listener, bool * flag);
eronic::TCPClient * setup_tcp_sender(int port);
eronic::UDPClient * setup_udp_sender(std::string ip, int port);

void create_package(char * data_package, std::string& message, int type, int sender);

int main() {

	bool flag = true;
	int listen_port = 9171;

	eronic::PeerNode * peer = new eronic::PeerNode(true,9001,2);
	peer->find_networks();


	system("Pause");
	return 0;
}
