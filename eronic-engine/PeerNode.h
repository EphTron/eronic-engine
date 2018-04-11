#pragma once

#include <thread>
#include <list>
#include <vector>
#include <stdexcept>

#include "DataPackage.h"
#include "UDPListener.h"
#include "UDPClient.h"
#include "TCPListener.h"
#include "TCPClient.h"
#include "Socket_WIN.h"

namespace eronic {

	typedef struct Network {
		int network_id;
		int network_port;
		std::string ip;
	}Network;

	class PeerNode
	{
	public:
		PeerNode();
		PeerNode(bool same_machine, int app_port, int max_connections);
		~PeerNode();

		void open_network(int network_port);
		void join_network(int network_id, int network_port);
		void leave_network();
		std::vector<Network*>& find_networks();
		

		bool app_broadcast_data(eronic::DataPackage* data);
		bool net_broadcast_data(eronic::DataPackage* data);
		eronic::DataPackage * receive_data();

		void run();

		void broadcast_network_exists();

	private:
		bool _running;
		bool _connected;
		bool _same_machine;

		int _id;
		std::string _ip;
		int _udp_port;
		int _tcp_port;

		int _network_id;
		int _network_port;

		int _app_udp_port;
		int _app_tcp_port;
		int _max_connections;

		eronic::UDPClient * _app_broadcaster;
		eronic::UDPListener * _net_udp_listener;
		eronic::UDPClient * _net_broadcaster;
		std::thread _network_broadcast_thread;

		//eronic::TCPListener * _net_tcp_listener;
		//eronic::TCPClient * _net_tcp_client;
		//std::list<eronic::TCPClient *> _net_connections;
		
	};

} // namespace eronic