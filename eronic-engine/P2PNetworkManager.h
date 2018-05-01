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
#include "Network.h"
#include "PeerPartner.h"

namespace eronic {

	class P2PNetworkManager
	{
	public:
		P2PNetworkManager();
		P2PNetworkManager(int id, int app_port, int max_connections, bool same_machine = false);
		~P2PNetworkManager();

		void open_network(int network_id, int network_port);
		void join_network(int network_id, int network_port);
		void leave_network();
		std::vector<Network *> find_networks(int milliseconds, bool join_first_network);

		bool app_broadcast_data(DataPackage* data);
		bool net_broadcast_data(DataPackage* data);
		bool tcp_send_data(TCPClient* client, DataPackage* data);

		DataPackage& const receive_udp_data(char * sender);
		DataPackage& const receive_tcp_data(TCPClient * client, char * sender);

		void receive_udp_data_loop();
		void receive_tcp_data_loop(TCPClient* client);
		void broadcast_network_exists_loop();
		void accept_network_connections_loop();

		void run_peer_network();
		void close_connection(int id);

		std::map<int, Network*> const& get_networks() const;

	private:
		bool _running;
		bool _connected;
		bool _same_machine;

		int _id;
		std::string _ip;
		std::string _broadcast_ip;
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

		std::map<int, PeerPartner*> _peer_connections;
		std::map<int, std::thread *> _connection_threads;

		std::thread _network_broadcast_thread;
		std::thread _network_connector_thread;
		std::thread _udp_network_receive_thread;

		// setup and utility functions
		void setup_tcp_listener(std::string ip, int port);
		void setup_udp_listener(std::string ip, int port);
		void setup_app_broadcaster(std::string ip, int port);
		void setup_net_broadcaster(std::string ip, int port);
		TCPClient * setup_connection(std::string& ip, int port, bool blocking);
	};

} // namespace eronic