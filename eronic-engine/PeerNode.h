#pragma once

#include <thread>
#include <future>
#include <list>
#include <vector>
#include <map>
#include <chrono>
#include <atomic>
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
		PeerNode(int id, bool same_machine, int app_port, int max_connections);
		~PeerNode();

		void open_network(int network_port);
		void join_network(int network_id, int network_port);
		void leave_network();
		void find_networks(int milliseconds, bool join_first_network);

		bool app_broadcast_data(DataPackage* data);
		bool net_broadcast_data(DataPackage* data);
		char * receive_udp_data(char * sender);
		//DataPackage * receive_tcp_data();

		void receive_udp_data_loop();
		void run_peer_network();

		void broadcast_network_exists_loop();
		//TCPClient * accept_network_connections();

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
		std::map<int, Network*> _networks;

		int _app_udp_port;
		int _app_tcp_port;
		int _max_connections;
		
		UDPClient * _app_broadcaster;
		UDPClient * _net_broadcaster;

		std::vector<DataPackage *> _received_data;

		UDPListener * _net_udp_listener;
		TCPListener * _net_tcp_listener;

		std::map<int, TCPClient *> _net_connections;
		std::map<int, std::thread> _connection_threads;

		std::thread _network_broadcast_thread;
		std::thread _network_connector_thread;
		std::thread _udp_network_receive_thread;

		// setup and utility functions
		// TO DO: remove parts that are only there to test it on the same machine
		void setup_udp_app_listener(std::string ip, int port);
		void setup_udp_net_listener(std::string ip, int port);
		void setup_tcp_listener(std::string ip, int port);

		void setup_app_broadcaster(std::string ip, int port);
		void setup_net_broadcaster(std::string ip, int port);
		TCPClient * setup_connection(std::string& ip, int port);
		//void setup_connection(std::promise<TCPClient *> && p, std::string & ip, int port);
		
	};

} // namespace eronic