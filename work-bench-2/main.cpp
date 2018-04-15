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
#define PACKAGE_SIZE sizeof(eronic::Package)


int main() {

	bool flag = true;
	int listen_port = 9171;

	eronic::PeerNode * peer = new eronic::PeerNode(listen_port,2);
	peer->find_networks(10000, true);


	system("Pause");
	return 0;
}
