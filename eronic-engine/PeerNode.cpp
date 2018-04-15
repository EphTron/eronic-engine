#include "PeerNode.h"

namespace eronic {

	PeerNode::PeerNode()
	{
	}

	PeerNode::PeerNode(int app_port, int max_connections) :
		_running(false),
		_connected(false),
		_id(-1),
		_ip(),
		_broadcast_ip(),
		_udp_port(-1),
		_tcp_port(-1),
		_network_id(-1),
		_network_port(-1),
		_networks(std::map<int, Network*>()),
		_app_udp_port(app_port),
		_app_tcp_port(app_port),
		_max_connections(max_connections),
		_received_data(std::vector<DataPackage*>()),
		_app_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN())),
		_net_udp_listener(new eronic::UDPListener(new eronic::Socket_WIN())),
		_net_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN())),
		_net_tcp_listener(new eronic::TCPListener(new eronic::Socket_WIN())),
		//_net_tcp_client(new eronic::TCPClient(new eronic::Socket_WIN())),
		_net_connections(std::map<int, eronic::TCPClient *>())
	{
		
		_ip = get_external_ip((std::string)"api.ipify.org");
		std::cout << "My IP:" << _ip << std::endl;
		std::size_t found = _ip.find_last_of(".");
		_broadcast_ip = _ip.substr(0, found) + ".255";
		std::cout << "Broadcast Address: " << _broadcast_ip << std::endl;
		_id = std::stoi(_ip.substr(found + 1));
		std::cout << "Setup PeerNode " << _id << std::endl;

		std::cout << "about to bind UDP listener to "<< _app_udp_port  << std::endl;
		_net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port, true);
		_app_broadcaster->connect(_broadcast_ip, _app_udp_port, false);
	}

	PeerNode::~PeerNode()
	{
		_network_broadcast_thread.join();
	}

	void PeerNode::open_network(int network_id, int network_port)
	{
		_network_id = network_id;
		_network_port = network_port;
		std::cout << "OPENING NETWORK " << network_id<< " on Port:"<< _network_port << std::endl;
		
		_net_udp_listener->close();
		_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);

		_net_broadcaster->connect(_broadcast_ip, _network_port, false);
		_connected = true;
		
	}

	void PeerNode::join_network(int network_id, int network_port)
	{
		std::cout << "JOINING ..." << std::endl;
		bool joined = false;
		if (_connected) {
			std::cout << "already in a network" << std::endl;
			return;
		}
		else {
			_network_id = network_id;
			_network_port = network_port;
			_udp_port = _network_port;
			
			// get external ip address;
			//_ip = eronic::get_external_ip((std::string)"api.ipify.org");
			_net_udp_listener->close();
			_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
			_net_broadcaster->connect(_broadcast_ip, _network_port, false);
			_connected = true;
			//_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
			
		}
		if (_connected) { // if join worked
			// listen with tcp for connections
			std::cout << "Setting up TCP Listener" << std::endl;
			setup_tcp_listener((std::string)"ADDR_ANY", _network_port);
			int listen_result = _net_tcp_listener->start(100); // start listening
			// message network that you are joining
			DataPackage enter_pack = DataPackage();
			enter_pack.type = 2;
			enter_pack.network_id = _network_id;
			enter_pack.sender_id = _id;
			enter_pack.sender_port = _tcp_port;
			enter_pack.set_ip(_ip);
			// send pack
			net_broadcast_data(&enter_pack);
		}
	}

	void PeerNode::leave_network()
	{
		_connected = false;
	}

	void PeerNode::find_networks(int milliseconds, bool join_first_network)
	{
		bool found = false;
		double elapsed_time = 0;
		std::cout << "Trying to find network " << _networks.size() <<std::endl;
		std::chrono::duration<double, std::milli> duration;

		_net_udp_listener->set_blocking(true);
		while (elapsed_time < (double)milliseconds) {
			auto t1 = std::chrono::high_resolution_clock::now();
			// receive network existence broadcast
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage temp_pack = receive_udp_data(sender_ip);
			if (temp_pack.type == 1) { // if data
				std::cout << "Collected data from " << sender_ip << std::endl;
				Network* new_network = new Network();
				new_network->network_port = temp_pack.int_data_1;
				new_network->network_id = temp_pack.network_id;
				new_network->network_ip = temp_pack.sender_ip;
				
				if (_networks.count(temp_pack.network_id) < 1) {
					_networks.insert(std::pair<int, Network*>(temp_pack.network_id, new_network));
					found = true;
					if (join_first_network) {
						_network_port = new_network->network_port;
						_network_id = temp_pack.network_id;
						join_network(_network_id, _network_port);
						if (_connected) {
							elapsed_time = (double)milliseconds;
						}
					}
				}
				
				std::cout << "Found network " << new_network->network_id << " Sender was " << temp_pack.sender_id << std::endl;
			}

			auto t2 = std::chrono::high_resolution_clock::now();
			duration = t2 - t1;
			elapsed_time += duration.count();
		}

		if (found == false) {
			std::cout << "NO NETWORK FOUND" << std::endl;
			_network_port = _app_udp_port + 1;
			_network_id = _id + 1000;
			open_network(_network_id, _network_port);
		}
	}

	bool PeerNode::app_broadcast_data(DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = _app_broadcaster->send(pack, sizeof(DataPackage));
		if (send_result != SOCKET_ERROR) {
			return true;
		}
		else {
			return false;
		}

	}

	bool PeerNode::net_broadcast_data(eronic::DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = _net_broadcaster->send(pack, sizeof(DataPackage));
		if (send_result != SOCKET_ERROR) {
			return true;
		}
		else {
			return false;
		}
	}

	DataPackage& const PeerNode::receive_udp_data(char * sender_ip)
	{
		char recv_buffer[sizeof(DataPackage)];
		//int recv_result = _net_udp_listener->receive(recv_buffer, sizeof(DataPackage));
		int recv_result = _net_udp_listener->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving udp data " << WSAGetLastError() << std::endl;
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);

			if (data.type < 0) {
				return DataPackage();
			} else {
				return data;
			}
		}
	}

	DataPackage & const PeerNode::receive_tcp_data(TCPClient * client, char * sender_ip)
	{
		char recv_buffer[sizeof(DataPackage)];
		int recv_result = client->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving udp data " << WSAGetLastError() << std::endl;
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);

			if (data.type < 0) {
				return DataPackage();
			} else {
				return data;
			}
		}
	}

	void PeerNode::receive_tcp_data_loop(TCPClient* client)
	{
		while (_connected) {
			//char *ip_addr[INET_ADDRSTRLEN];
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage data_pack = receive_tcp_data(client, sender_ip);

			if (data_pack.type == 3) { // if join received
				std::cout << "message" << std::endl;
			}
		}
	}

	void PeerNode::receive_udp_data_loop()
	{
		
		while (_connected) {
			//char *ip_addr[INET_ADDRSTRLEN];
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage data_pack = receive_udp_data( sender_ip);
			//char a[INET_ADDRSTRLEN] = { sender_ip };
			//DataPackage dp = DataPackage(1, 2, _network_port, _ip, std::to_string(_network_id));
//			if (data_pack != nullptr) {

				if (data_pack.type == 2) { // if join received
					std::cout << "type 2" << std::endl;

					if (_connection_threads.find(data_pack.sender_id) == _connection_threads.end()) {
						std::string ip = data_pack.sender_ip;
						TCPClient * client = setup_connection(ip, _network_port);
						_net_connections.insert(std::pair<int, TCPClient*>(data_pack.sender_id, client));
						std::thread * t = new std::thread(&PeerNode::receive_tcp_data_loop, this, client);
						_connection_threads.insert(std::pair<int, std::thread*>(data_pack.sender_id, t));
						//int func() { return 1; }
						//std::future<int> ret = std::async(&func);
						//int i = ret.get();
						//std::promise<TCPClient *> p;
						//auto future_client = p.get_future();
						//// TCPClient * tcp_c = setup_connection(sender_ip, _network_port);
						//std::thread thread(&PeerNode::setup_connection, std::move(p), sender_ip, _network_port);
						//// lock
						//_connection_threads.insert(std::pair<int, std::thread*>(dp.sender_id, &thread));
						//// unlock
						//thread.join();
						//TCPClient * client = future_client.get();
						//_net_connections.insert(std::pair<int, TCPClient*>(dp.sender_id, client));
						//std::cout << "WORKED" << std::endl;
					}
				}
				else if (data_pack.type == 3) {
					std::cout << "ttype 3" << std::endl;
				}
			//}
		}
	}

	void PeerNode::run_peer_network()
	{
		std::cout << "runnning peer network " << std::endl;
		_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists_loop, this);
		_udp_network_receive_thread = std::thread(&PeerNode::receive_udp_data_loop, this);
		_network_connector_thread = std::thread(&PeerNode::accept_network_connections_loop, this);
		//_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
		/*while (_running && _connected) {

		}*/
	}

	void PeerNode::broadcast_network_exists_loop()
	{
		while (_connected) {
			std::cout << "Broadcast: Network " << _network_id << " exists" << std::endl;
			//DataPackage dp = DataPackage(1, 2, _network_port, _ip, std::to_string(_network_id));
			DataPackage dp = DataPackage();
			dp.type = 1;
			dp.network_id = _network_id;
			dp.int_data_1 = _network_port;
			dp.sender_id = _id;
			dp.set_ip(_ip);
			app_broadcast_data(&dp);

			Sleep(2000);
		}

	}

	void PeerNode::accept_network_connections_loop()
	{
		while (_connected) {
			TCPClient * client = _net_tcp_listener->accept();
			if (client != nullptr) {
				std::cout << "Accepted client!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
				// TODO
			}
			else {
				std::cout << "Error accepting client!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			}
		}
	}
	
	void PeerNode::setup_tcp_listener(std::string ip, int port)
	{
		_tcp_port = port;
		int binding_result = _net_tcp_listener->bind(ip, _tcp_port, true);
		if (binding_result == SOCKET_ERROR) {
			std::cout << "Error binding tcp listener on port " << _tcp_port << std::endl;
			throw std::invalid_argument("Error binding");
		}
		else {
			std::cout << "TCP setup on port " << _net_tcp_listener->get_address()->get_port() << std::endl;
		}
	}

	void PeerNode::setup_app_broadcaster(std::string ip, int port)
	{
		int connect_result = connect_result = _app_broadcaster->connect(ip, port, false);
	
		if (connect_result != SOCKET_ERROR) {
			std::cout << "APP Broadcaster connected to " << _app_broadcaster->get_address()->get_ip() << ":" << _app_broadcaster->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting udp client to " << _app_broadcaster->get_address()->get_port() << std::endl;
			throw std::invalid_argument("Error connecting");
		}
	}
	void PeerNode::setup_net_broadcaster(std::string ip, int port)
	{
		_network_port = port;
		_net_broadcaster->stop(0);
		int connect_result = connect_result = _net_broadcaster->connect(ip, port, false);
		if (connect_result != SOCKET_ERROR) {
			std::cout << "UDP Broadcaster connected to " << _net_broadcaster->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting udp client to " << _net_broadcaster->get_address()->get_port() << std::endl;
		}
	}

	TCPClient * PeerNode::setup_connection( std::string & ip, int port)
	{
		TCPClient * tcp_client = new TCPClient(new Socket_WIN());
		int res = tcp_client->connect(ip, port, true);
		if (res != SOCKET_ERROR) {
			std::cout << "TCP CONNECTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << tcp_client->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting TCPClient to " << tcp_client->get_address()->get_port() << std::endl;
		}
		return tcp_client;
	}
	

}