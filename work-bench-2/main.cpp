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
	int app_port = 9174;

	eronic::PeerNode * peer = new eronic::PeerNode(2, app_port, 10, true);
	peer->find_networks(6000, true);
	peer->run_peer_network();

	system("Pause");
	return 0;
}
