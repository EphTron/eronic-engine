#include "PeerNode.h"

namespace eronic {

	PeerNode::PeerNode()
	{
	}

	PeerNode::PeerNode(int id, bool same_machine, int app_port, int max_connections) :
		_running(false),
		_connected(false),
		_same_machine(same_machine),
		_id(id),
		_ip(),
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
		std::cout << "PeerNode id = " << _id << std::endl;
		if (_same_machine) {
			// get external ip address;
			_ip = "127.0.0.1";

			// setup udp listener to listen for networks in the app
			setup_udp_app_listener((std::string)"ADDR_ANY", app_port);

			// connect UDP Client (for same machine)
			setup_app_broadcaster((std::string)"127.0.0.255", app_port);
		}
		else {
			_net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port, true);
			_app_broadcaster->connect((std::string)"127.0.0.255", _app_udp_port, false);
		}
	}

	PeerNode::~PeerNode()
	{
		_network_broadcast_thread.join();
	}

	void PeerNode::open_network(int network_port)
	{
		_network_id = std::rand();
		_network_port = network_port;
		_udp_port = _network_port;
		if (_same_machine) {
			// get external ip address;
			_ip = "127.0.0.1";
			// close listener on app port
			_net_udp_listener->close();
			// setup udp listener on network port
			setup_udp_net_listener((std::string)"ADDR_ANY", _network_port);

			// connect UDP Client (for same machine)
			_net_broadcaster->stop(0);
			setup_net_broadcaster((std::string)"127.0.0.255", _network_port);
			_connected = true;
		}
		else {
			// get external ip address;
			//_ip = eronic::get_external_ip((std::string)"api.ipify.org");
			_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
			_net_broadcaster->connect((std::string)"127.0.0.255", _network_port, false);
			_connected = true;
		}
	}

	void PeerNode::join_network(int network_id, int network_port)
	{
		if (_connected) {
			return;
		}
		else {
			_network_id = network_id;
			_network_port = network_port;
			_udp_port = _network_port;
			if (_same_machine) {
				// get external ip address;
				_ip = "127.0.0.1";

				// setup udp listener
				_net_udp_listener->close();
				setup_udp_net_listener((std::string)"ADDR_ANY", _udp_port);

				// connect UDP Client (for same machine)

				setup_net_broadcaster((std::string)"127.0.0.255", _network_port);
				_connected = true;
				//_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
			}
			else {
				// get external ip address;
				//_ip = eronic::get_external_ip((std::string)"api.ipify.org");
				_net_udp_listener->close();
				_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
				_net_broadcaster->connect((std::string)"127.0.0.255", _network_port, false);
				_connected = true;
				//_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
			}
		}
		if (_connected) { // if join worked
			// listen with tcp for connections
			setup_tcp_listener((std::string)"127.0.0.255", _network_port);
			int listen_result = _net_tcp_listener->start(100);
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
		char * received_data;
		Network* new_network;
		double elapsed_time = 0;
		std::cout << "Trying to find network" << std::endl;
		std::chrono::duration<double, std::milli> duration;
		_net_udp_listener->set_blocking(true);
		while (elapsed_time < (double)milliseconds) {
			auto t1 = std::chrono::high_resolution_clock::now();
			// receive network existence broadcast
			received_data = receive_udp_data(NULL);
			if (received_data != nullptr) { // if data received
				DataPackage temp_pack = DataPackage(received_data);
				if (temp_pack.type == 1) { // if data
					new_network = new Network();
					new_network->network_port = temp_pack.int_data_1;
					new_network->ip = temp_pack.sender_ip;
					new_network->network_id = temp_pack.network_id;
					if (_networks.find(temp_pack.network_id) == _networks.end()) {
						_networks.insert(std::pair<int, Network*>(temp_pack.network_id, new_network));
						if (join_first_network) {
							join_network(temp_pack.network_id, new_network->network_port);
							if (_connected) {
								elapsed_time = (double)milliseconds;
							}
						}
					}
					std::cout << "Found network " << new_network->network_id << " Sender was " << temp_pack.sender_id << std::endl;
				}
			}
			else {
				std::cout << "No network found " << std::endl;
			}
			auto t2 = std::chrono::high_resolution_clock::now();
			duration = t2 - t1;
			elapsed_time += duration.count();
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

	char * PeerNode::receive_udp_data(char * sender_ip)
	{
		char data[sizeof(DataPackage)];
		int recv_result = _net_udp_listener->receivefrom(data, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving udp data " <<  WSAGetLastError() << std::endl;
			return nullptr;
		}
		else {
			return data;
		}
	}

	/*void PeerNode::start_receiving_events()
	{

	}*/

	void PeerNode::receive_udp_data_loop()
	{
		char * received_data;
		char sender_ip[INET_ADDRSTRLEN];
		while (_connected) {

			received_data = receive_udp_data(sender_ip);
			//DataPackage dp = DataPackage(1, 2, _network_port, _ip, std::to_string(_network_id));
			if (received_data != nullptr) {
				DataPackage dp = DataPackage(received_data);

				if (dp.type == 2) { // if join received
					std::cout << "type 2" << std::endl;

					if (_connection_threads.find(dp.sender_id) == _connection_threads.end()) {
						std::string ip = dp.sender_ip;
						TCPClient * client = setup_connection(ip, _network_port);
						_net_connections.insert(std::pair<int, TCPClient*>(dp.sender_id, client));
						//std::thread * t;
						//_connection_threads.insert(std::pair<int, std::thread*>(dp.sender_id, t));
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
				else if (dp.type == 3) {
					std::cout << "ttype 3" << std::endl;
				}
			}
		}
	}

	void PeerNode::run_peer_network()
	{
		std::cout << "runnning peer network " << _network_id << std::endl;
		_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists_loop, this);
		_udp_network_receive_thread = std::thread(&PeerNode::receive_udp_data_loop, this);
		//_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
		/*while (_running && _connected) {

		}*/
	}

	void PeerNode::broadcast_network_exists_loop()
	{
		while (_connected) {
			std::cout << "network " << _network_id << " exists" << std::endl;
			//DataPackage dp = DataPackage(1, 2, _network_port, _ip, std::to_string(_network_id));
			DataPackage dp = DataPackage();
			dp.type = 1;
			dp.network_id = _network_id;
			dp.int_data_1 = _network_port;
			dp.sender_id = _id;
			dp.set_ip(_ip);
			app_broadcast_data(&dp);

			Sleep(1500);
		}

	}
	void PeerNode::setup_udp_app_listener(std::string ip, int port)
	{
		// setup udp listener to listen for networks in the app
		int binding_result = -1;
		_app_udp_port = port;
		while (_app_udp_port <= port + 1 && binding_result != 0) {
			binding_result = _net_udp_listener->bind(ip, _app_udp_port, true);
			if (binding_result == 10048) {
				_app_udp_port++;
			}
			else if (binding_result != 0) {
				std::cout << "Error binding listener on port " << _app_udp_port << std::endl;
				throw std::invalid_argument("Error binding");
			}
			else {
				std::cout << "Peer UDP List listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
		}
	}

	void PeerNode::setup_udp_net_listener(std::string ip, int port)
	{
		// setup udp listener to listen for networks in the app
		int binding_result = -1;
		_udp_port = port;
		while (_udp_port <= port + 1 && binding_result != 0) {
			binding_result = _net_udp_listener->bind(ip, _udp_port, true);
			if (binding_result == 10048) {
				_udp_port++;
			}
			else if (binding_result != 0) {
				std::cout << "Error binding listener on port " << _udp_port << std::endl;
				throw std::invalid_argument("Error binding");
			}
			else {
				std::cout << "Peer UDP List listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
		}
	}
	void PeerNode::setup_tcp_listener(std::string ip, int port)
	{
		_tcp_port = port;
		int binding_result = _net_tcp_listener->bind(ip, _tcp_port, true);
		if (binding_result == SOCKET_ERROR) {
			std::cout << "Error binding listener on port " << _tcp_port << std::endl;
			throw std::invalid_argument("Error binding");
		}
		else {
			std::cout << "Peer UDP List listening on " << _net_tcp_listener->get_address()->get_port() << std::endl;
		}
	}

	void PeerNode::setup_app_broadcaster(std::string ip, int port)
	{
		int connect_result = -1;
		if (_udp_port == port) {
			//connect_result = _app_broadcaster->connect((std::string)"192.168.1.255", port + 1, false);
			connect_result = _app_broadcaster->connect(ip, port + 1, false);
			//connect_result = _app_broadcaster->connect((std::string)"127.0.0.1", port + 1, false);
		}
		else {

			//connect_result = _app_broadcaster->connect((std::string)"192.168.1.255", port, false);
			connect_result = _app_broadcaster->connect(ip, port, false);
			//connect_result = _app_broadcaster->connect((std::string)"127.0.0.1", port, false);
		}
		if (connect_result == 0) {
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
		int connect_result = -1;
		if (_udp_port == port)
			connect_result = _net_broadcaster->connect((std::string)"127.0.0.255", port + 1, false);
		else
			connect_result = _net_broadcaster->connect((std::string)"127.0.0.255", port, false);

		if (connect_result != SOCKET_ERROR) {
			std::cout << "UDP Broadcaster connected to " << _net_broadcaster->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting udp client to " << _net_broadcaster->get_address()->get_port() << std::endl;
		}
	}

	/*void PeerNode::setup_connection(std::promise<TCPClient *> && p, std::string & ip, int port)
	{
		TCPClient * tcp_client = new TCPClient(new Socket_WIN());
		tcp_client->connect(ip, port, true);
		p.set_value(tcp_client);
	}*/

	TCPClient * PeerNode::setup_connection( std::string & ip, int port)
	{
		TCPClient * tcp_client = new TCPClient(new Socket_WIN());
		tcp_client->connect(ip, port, true);
		return tcp_client;
	}
	

}