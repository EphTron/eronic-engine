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
		_peer_connections(std::map<int, PeerPartner *>())
	{

		_ip = get_external_ip((std::string)"api.ipify.org");
		std::cout << "My IP:" << _ip << std::endl;
		std::size_t found = _ip.find_last_of(".");
		_broadcast_ip = _ip.substr(0, found) + ".255";
		std::cout << "Broadcast Address: " << _broadcast_ip << std::endl;
		_id = std::stoi(_ip.substr(found + 1));
		std::cout << "Setup PeerNode " << _id << std::endl;

		std::cout << "about to bind UDP listener to " << _app_udp_port << std::endl;
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
		std::cout << _id << " OPENING NETWORK " << network_id << " on Port:" << _network_port << std::endl;

		_net_udp_listener->close();
		_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);

		_net_broadcaster->connect(_broadcast_ip, _network_port, false);

		std::cout << _id << "Setting up TCP Listener" << std::endl;
		setup_tcp_listener((std::string)"ADDR_ANY", _network_port);
		int listen_result = _net_tcp_listener->start(100); // start listening
		_connected = true;
	}

	void PeerNode::join_network(int network_id, int network_port)
	{
		std::cout << "JOINING NETWORK " << network_id << " ON PORT: " << network_port << std::endl;
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
			DataPackage enter_pack = DataPackage(2, _id, _network_port, _network_id, _ip, (std::string)"joining\0");
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
		std::chrono::duration<double, std::milli> duration;

		_net_udp_listener->set_blocking(true);
		while (elapsed_time < (double)milliseconds) {
			auto t1 = std::chrono::high_resolution_clock::now();
			// receive network existence broadcast
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage temp_pack = receive_udp_data(sender_ip);
			if (temp_pack.type == 1 && temp_pack.sender_id != _id) { // if data
				std::cout << "Recv UDP data from " << temp_pack.sender_port << std::endl;
				Network* new_network = new Network();
				new_network->network_port = temp_pack.sender_port;
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

				std::cout << "Found network " << new_network->network_id << ". Sender was " << temp_pack.sender_id << std::endl;
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

	bool PeerNode::tcp_send_data(TCPClient * client, eronic::DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = client->send(pack, sizeof(DataPackage));
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
		int recv_result = _net_udp_listener->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving udp data " << WSAGetLastError() << std::endl;
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);
			if (data.type < 0) {
				return DataPackage();
			}
			else if (data.sender_id == _id) {
				std::cout << "catched own tcp pack" << std::endl;
				return DataPackage();
			}
			else {
				std::cout << "UDP data Type: " << data.type << " Sender: " << data.sender_id << " Message: " << data.message << std::endl;
				return data;
			}
		}
	}

	DataPackage & const PeerNode::receive_tcp_data(TCPClient * client, char * sender_ip)
	{
		char recv_buffer[sizeof(DataPackage)];
		int recv_result = client->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving tcp data " << WSAGetLastError() << std::endl;
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);

			if (data.type < 0) {
				return DataPackage();
			}
			else if (data.sender_id == _id) {
				std::cout << "catched own ucp pack" << std::endl;
				return DataPackage();
			}
			else {
				std::cout << "Tcp data Type: " << data.type << " Sender: " << data.sender_id << " Message: " << data.message << std::endl;
				return data;
			}
		}
	}

	void PeerNode::receive_tcp_data_loop(TCPClient* client)
	{
		while (_connected) {
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage data_pack = receive_tcp_data(client, sender_ip);

			if (data_pack.type == 3) { // if got accepted
				_peer_connections.at(data_pack.sender_id)->answered_alive = true;
				DataPackage accepted_response = DataPackage(4, _id, _network_port, _network_id, _ip, (std::string)"established\0");
				accepted_response.int_data_1 = data_pack.sender_id * _id;
				tcp_send_data(client, &accepted_response);
			}
			else if (data_pack.type == 4) { // if got accepted
				int quiz = data_pack.int_data_1 / data_pack.sender_id;
				_peer_connections.at(data_pack.sender_id)->peer_connection = true;
				_peer_connections.at(data_pack.sender_id)->answered_alive = true;
				std::cout << "Connection successfully established! QUIZ: " << quiz << " = " << _id << std::endl;
			}
			else if (data_pack.type == 6) { // if got accepted
				DataPackage alive_response = DataPackage(7, _id, _network_port, _network_id, _ip, (std::string)"alive\0");
				tcp_send_data(client, &alive_response);
			}
			else if (data_pack.type == 7) { // if got accepted
				_peer_connections.at(data_pack.sender_id)->answered_alive = true;
				std::cout << data_pack.sender_id << " is alive" << std::endl;
			}
		}
	}

	void PeerNode::receive_udp_data_loop()
	{
		while (_connected) {
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage data_pack = receive_udp_data(sender_ip);

			if (data_pack.type == 2) { // if join received

				if (_connection_threads.find(data_pack.sender_id) == _connection_threads.end()) {
					std::string ip = data_pack.sender_ip;
					std::cout << "Setting up client through udp looop #" << std::endl;
					TCPClient * client = setup_connection(ip, _network_port);
					PeerPartner * peer_partner = new PeerPartner(data_pack.sender_id, _network_id, client);
					_peer_connections.insert(std::pair<int, PeerPartner*>(data_pack.sender_id, peer_partner));
					std::thread * t = new std::thread(&PeerNode::receive_tcp_data_loop, this, client);
					_connection_threads.insert(std::pair<int, std::thread*>(data_pack.sender_id, t));
				}
			}
			else if (data_pack.type == 3) {
				std::cout << "type 3" << std::endl;
			}

		}
	}

	void PeerNode::run_peer_network()
	{
		_running = true;
		std::cout << "runnning peer network " << std::endl;
		_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists_loop, this);
		_udp_network_receive_thread = std::thread(&PeerNode::receive_udp_data_loop, this);
		_network_connector_thread = std::thread(&PeerNode::accept_network_connections_loop, this);
		while (_running && _connected) {
			std::vector<int> peers_to_delete = std::vector<int>();
			for (auto const& connection : _peer_connections)
			{
				if (connection.second->answered_alive == false) {
					std::cout << "Guess " << connection.first << " died..." << std::endl;
					connection.second->peer_connection = false;
				}
				else {
					connection.second->answered_alive = false;
					std::cout << "Send alive msg to " << connection.first << std::endl;
					DataPackage alive_question = DataPackage(6, _id, _network_port, _network_id, _ip, (std::string)"u ded?");
					alive_question.int_data_1 = alive_question.sender_id * _id;
					tcp_send_data(connection.second->connection, &alive_question);
				}
				if (connection.second->peer_connection == false) {
					// close connections
					_connection_threads.at(connection.first)->join();
					connection.second->connection->close();
					std::cout << "Deleting..." << std::endl;
				}
			}
			for (auto const& id : peers_to_delete) {
				delete _peer_connections.at(id)->connection;
				_peer_connections.at(id)->connection = nullptr;
				delete _peer_connections.at(id)->connection;
				_peer_connections.erase(id);
				std::cout << "Deleted " << id << std::endl;
			}
			std::cout << "client list:" << _peer_connections.size() << std::endl;
			Sleep(5000);
		}
	}

	void PeerNode::broadcast_network_exists_loop()
	{
		while (_connected) {
			DataPackage dp = DataPackage(1, _id, _network_port, _network_id, _ip, (std::string)"exists\0");
			app_broadcast_data(&dp);
			Sleep(2000);
			//std::cout << "client list:" << _peer_connections.size() << std::endl;
		}

	}

	void PeerNode::accept_network_connections_loop()
	{
		while (_connected) {
			TCPClient * client = _net_tcp_listener->accept();
			if (client != nullptr) {
				DataPackage accepted_pack = DataPackage(3, _id, _network_port, _network_id, _ip, (std::string)"accepted\0");
				tcp_send_data(client, &accepted_pack);
				char sender_ip[INET_ADDRSTRLEN];
				DataPackage accepted_response = receive_tcp_data(client, sender_ip);
				if (accepted_response.type == 4) {
					// send that you got established message
					DataPackage established = DataPackage(4, _id, _network_port, _network_id, _ip, (std::string)"established\0");
					established.int_data_1 = accepted_response.sender_id * _id;
					//client->send(&established, sizeof(DataPackage));
					tcp_send_data(client, &established);

					// add connection
					PeerPartner * peer_partner = new PeerPartner(accepted_response.sender_id, _network_id, client);
					_peer_connections.insert(std::pair<int, PeerPartner*>(accepted_response.sender_id, peer_partner));
					peer_partner->peer_connection = true;
					peer_partner->answered_alive = true;
					// add thread
					std::thread * t = new std::thread(&PeerNode::receive_tcp_data_loop, this, client);
					_connection_threads.insert(std::pair<int, std::thread*>(accepted_response.sender_id, t));
					int quiz = accepted_response.int_data_1 / accepted_response.sender_id;

					std::cout << "Connection successfully established! QUIZ: " << quiz << " = " << _id << std::endl;
				}
			}
			else {
				//std::cout << "Error accepting client!" << std::endl;
			}
			Sleep(1000);
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

	TCPClient * PeerNode::setup_connection(std::string & ip, int port)
	{
		TCPClient * tcp_client = new TCPClient(new Socket_WIN());
		int res = tcp_client->connect(ip, port, true);
		if (res != SOCKET_ERROR) {

		}
		else {
			std::cout << "Error connecting TCPClient to " << tcp_client->get_address()->get_port() << std::endl;
		}
		return tcp_client;
	}


}

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