#include "P2PNetworkManager.h"
#include <ctime>

namespace eronic {

	P2PNetworkManager::P2PNetworkManager()
	{
	}

	P2PNetworkManager::P2PNetworkManager(int id, int app_port, int max_connections, bool same_machine) :
		_running(false),
		_connected(false),
		_same_machine(same_machine),
		_id(id),
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
		_data_buffer(),
		_app_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN())),
		_net_udp_listener(new eronic::UDPListener(new eronic::Socket_WIN())),
		_net_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN())),
		_net_tcp_listener(new eronic::TCPListener(new eronic::Socket_WIN())),
		_peer_connections(std::map<int, PeerPartner *>())
	{
		if (!_same_machine) {
			_ip = get_external_ip((std::string)"api.ipify.org");
			std::cout << "My IP:" << _ip << std::endl;
			std::size_t found = _ip.find_last_of(".");
			// When sending to more than the local network
			//std::stringstream s(_ip);
			//int a, b, c, d; //to store the 4 ints
			//char ch; //to temporarily store the '.'
			//s >> a >> ch >> b >> ch >> c >> ch >> d;
			//std::ostringstream oss;
			//oss << a << "." << b << ".255.255";
			//_broadcast_ip = oss.str();

			_broadcast_ip = _ip.substr(0, found) + ".255";
			std::cout << "Broadcast Address: " << _broadcast_ip << std::endl;
			_id = std::stoi(_ip.substr(found + 1));
			std::cout << "Setup PeerNode " << _id << std::endl;

			_net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port, true);
			_app_broadcaster->connect(_broadcast_ip, _app_udp_port, false);
			std::cout << "PeerNode id = " << _id << std::endl;
			OutputDebugStringA("not the same");
		}
		else {
			_ip = "127.0.0.1";
			OutputDebugStringA("Setup Peer Manager");
			std::cout << "My IP:" << _ip << std::endl;
			std::size_t found = _ip.find_last_of(".");
			_broadcast_ip = _ip.substr(0, found) + ".255";
			std::cout << "Broadcast Address: " << _broadcast_ip << std::endl;
			//_id = std::stoi(_ip.substr(found + 1));
			_app_udp_port = app_port;
			int binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port, true);
			if (binding_result == 10048) {
				OutputDebugStringA("Port in use");
				_id += 1;
				_app_udp_port += 1;
				binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port, true);
			}
			if (_app_udp_port == app_port) {
				_udp_port = _app_udp_port + 1;
				int connect_result = _app_broadcaster->connect(_broadcast_ip, _udp_port, false);
			}
			else {
				_udp_port = app_port;
				int connect_result = _app_broadcaster->connect(_broadcast_ip, _udp_port, false);
			}
			std::cout << "Setup PeerNode " << _id << std::endl;
		}

		_current_time = clock();
		_alive_timer = 0.0;
		_frequency = 1.0;
		std::cout << "next" << std::endl;
	}

	P2PNetworkManager::~P2PNetworkManager()
	{
		stop_peer_network();
	}

	void P2PNetworkManager::open_network(int network_id, int network_port)
	{
		std::cout << "OPENING NETWORK " << network_id << " on Port:" << _network_port << std::endl;
		_network_id = network_id;
		_network_port = network_port;
		_net_udp_listener->close();
		int binding_result, connect_result;
		if (!_same_machine) {
			binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
			connect_result = _net_broadcaster->connect(_broadcast_ip, _network_port, false);
		}
		else
		{
			binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
			if (binding_result == 10048) {
				_id += 1;
				_network_port += 1;
				binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
			}
			if (_network_port == network_port) {
				_udp_port = network_port + 1;
				connect_result = _net_broadcaster->connect(_broadcast_ip, _udp_port, false);
			}
			else {
				_udp_port = network_port;
				connect_result = _net_broadcaster->connect(_broadcast_ip, _udp_port, false);
			}
		}
		// check if network got setup properly
		if (binding_result == 0 && connect_result == 0) {
			setup_tcp_listener((std::string)"ADDR_ANY", _network_port);
			int listen_result = _net_tcp_listener->start(100); // start listening
			if (listen_result != SOCKET_ERROR) {
				_connected = true;
				_tcp_port = _udp_port;
			}
			else {
				std::cout << "Error setting up TCP" << std::endl;
			}
		}
		else {
			std::cout << "Error setting up network" << std::endl;
		}
	}

	void P2PNetworkManager::join_network(int network_id, int network_port)
	{
		std::cout << "JOINING NETWORK " << network_id << " ON PORT: " << network_port << std::endl;
		bool joined = false;
		if (_connected) {
			std::cout << "already in a network" << std::endl;
			return;
		}
		else {
			if (!_same_machine) {
				_network_id = network_id;
				_network_port = network_port;
				_udp_port = _network_port;

				// setup udp
				_net_udp_listener->close();
				_net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
				_net_broadcaster->connect(_broadcast_ip, _network_port, false);
				_connected = true;
				// setup tcp
				setup_tcp_listener((std::string)"ADDR_ANY", _network_port);
			}
			else {
				_network_id = network_id;
				_network_port = network_port;
				_net_udp_listener->close();
				int binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
				if (binding_result == 10048) {
					_id += 1;
					_network_port += 1;
					binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _network_port, true);
				}
				if (_network_port == network_port) {
					_udp_port = network_port + 1;
					int connect_result = _net_broadcaster->connect(_broadcast_ip, _udp_port, false);
				}
				else {
					_udp_port = network_port;
					int connect_result = _net_broadcaster->connect(_broadcast_ip, _udp_port, false);

				}
				std::cout << "TCP PORT: " << _network_port << std::endl;
				setup_tcp_listener((std::string)"ADDR_ANY", _network_port);
				_connected = true;
			}
			int listen_result = _net_tcp_listener->start(100); // start listening
			if (listen_result == SOCKET_ERROR) {
				std::cout << "Error listening to tcp" << std::endl;
			}
			DataPackage enter_pack = DataPackage(2, _id, _network_port, _network_id, _ip, (std::string)"joining\0");
			net_broadcast_data(&enter_pack);
		}
	}

	void P2PNetworkManager::leave_network()
	{
		DataPackage close_information = DataPackage(5, _id, _network_port, _network_id, _ip, (std::string)"bye bye");
		broadcast_tcp_data(&close_information);

		_connected = false;
	}

	std::vector<Network*> P2PNetworkManager::find_networks(int milliseconds, bool join_first_network)
	{
		std::cout << "Trying to find network" << std::endl;
		std::vector<Network*> found_networks = std::vector<Network*>();
		bool found = false;
		double elapsed_time = 0;
		std::chrono::duration<double, std::milli> duration;

		_net_udp_listener->set_blocking(true);
		while (elapsed_time < (double)milliseconds) {
			auto t1 = std::chrono::high_resolution_clock::now();

			char sender_ip[INET_ADDRSTRLEN];
			DataPackage temp_pack = receive_udp_data(sender_ip);

			if (temp_pack.type == 1 && temp_pack.sender_id != _id) { // if 
				Network* new_network = new Network();
				new_network->network_port = temp_pack.sender_port;
				new_network->network_id = temp_pack.network_id;
				new_network->network_ip = temp_pack.sender_ip;

				if (_networks.count(temp_pack.network_id) < 1) {
					found_networks.push_back(new_network);
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
				std::cout << "Found network " << new_network->network_id << std::endl;
			}
			auto t2 = std::chrono::high_resolution_clock::now();
			duration = t2 - t1;
			elapsed_time += duration.count();
		}
		if (found == false) {
			std::cout << "NO NETWORK FOUND" << std::endl;
			if (join_first_network) {
				_network_port = _app_udp_port + 3;
				_network_id = _id + 1000;
				open_network(_network_id, _network_port);
			}
		}
		return found_networks;
	}

	std::vector<Network*> P2PNetworkManager::find_future_networks(std::promise<int> p, int milliseconds, bool join_first_network)
	{
		std::cout << "Trying to find network" << std::endl;
		std::vector<Network*> found_networks = std::vector<Network*>();
		bool found = false;
		double elapsed_time = 0;
		std::chrono::duration<double, std::milli> duration;

		_net_udp_listener->set_blocking(true);
		while (elapsed_time < (double)milliseconds) {
			auto t1 = std::chrono::high_resolution_clock::now();

			char sender_ip[INET_ADDRSTRLEN];
			DataPackage temp_pack = receive_udp_data(sender_ip);

			if (temp_pack.type == 1 && temp_pack.sender_id != _id) { // if 
				Network* new_network = new Network();
				new_network->network_port = temp_pack.sender_port;
				new_network->network_id = temp_pack.network_id;
				new_network->network_ip = temp_pack.sender_ip;

				if (_networks.count(temp_pack.network_id) < 1) {
					found_networks.push_back(new_network);
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
				std::cout << "Found network " << new_network->network_id << std::endl;
			}
			auto t2 = std::chrono::high_resolution_clock::now();
			duration = t2 - t1;
			elapsed_time += duration.count();
		}
		if (found == false) {
			std::cout << "NO NETWORK FOUND" << std::endl;
			if (join_first_network) {
				_network_port = _app_udp_port + 3;
				_network_id = _id + 1000;
				open_network(_network_id, _network_port);
			}
		}
		p.set_value(found_networks.size());
		return found_networks;
	}

	bool P2PNetworkManager::app_broadcast_data(DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = _app_broadcaster->send(pack, sizeof(DataPackage));
		if (send_result != SOCKET_ERROR) {
			//std::cout << "SENT: UDP MSG Type: " << data->type << " Sender: " << data->sender_id << " Message: " << data->message << std::endl;
			return true;
		}
		else {
			std::cout << "error sending " << WSAGetLastError() << std::endl;
			return false;
		}
	}

	bool P2PNetworkManager::net_broadcast_data(eronic::DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = _net_broadcaster->send(pack, sizeof(DataPackage));
		std::cout << "Net Broadcast to port:" << _net_broadcaster->get_address()->get_port() << std::endl;
		if (send_result != SOCKET_ERROR) {
			std::cout << "SENT: UDP MSG Type: " << data->type << " Sender: " << data->sender_id << " Message: " << data->message << std::endl;
			return true;
		}
		else {
			return false;
		}
	}

	bool P2PNetworkManager::tcp_send_data(TCPClient * client, eronic::DataPackage * data)
	{
		char pack[sizeof(DataPackage)];
		data->serialize(pack);
		int send_result = client->send(pack, sizeof(DataPackage));
		if (send_result != SOCKET_ERROR) {
			std::cout << "SENT: TCP MSG Type: " << data->type << " Sender: " << data->sender_id << " Message: " << data->message << std::endl;
			return true;
		}
		else {
			std::cout << "sent failed" << WSAGetLastError() << std::endl;
			return false;
		}
	}

	bool P2PNetworkManager::broadcast_tcp_data(DataPackage * data)
	{
		bool res = true;
		for (auto connection : _peer_connections)
		{
			OutputDebugStringA("SENT");
			res = tcp_send_data(connection.second->tcp_client, data);
			if (!res) {
				OutputDebugStringA("ERRRRRRRROR");
				return false;
			}
		}
		return true;
	}

	DataPackage& const P2PNetworkManager::receive_udp_data(char * sender_ip)
	{
		char recv_buffer[sizeof(DataPackage)];
		int recv_result = _net_udp_listener->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			std::cout << "Error receiving " << WSAGetLastError() << std::endl;
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);
			if (data.type < 0) {
				std::cout << "Invalid pack" << std::endl;
				return DataPackage();
			}
			else if (data.sender_id == _id) {
				std::cout << "catched own udp pack" << data.type << std::endl;
				return DataPackage();
			}
			else {
				std::cout << "RECV: UDP MSG Type: " << data.type << " Sender: " << data.sender_id << " Message: " << data.message << std::endl;
				return data;
			}
		}
	}

	DataPackage& const P2PNetworkManager::receive_tcp_data(TCPClient * client, char * sender_ip)
	{
		char recv_buffer[sizeof(DataPackage)];
		int recv_result = client->receivefrom(recv_buffer, sizeof(DataPackage), sender_ip);
		if (recv_result == SOCKET_ERROR) {
			return DataPackage();
		}
		else {
			DataPackage data = DataPackage(recv_buffer);
			if (recv_result == 0) {
				return DataPackage(5, _id, _network_port, _network_id, _ip, (std::string)"close\0");
			}
			if (data.type < 0) {
				//std::cout << "invalid pack" << data.type << std::endl;
				return DataPackage();
			}
			else if (data.sender_id == _id) {
				return DataPackage();
			}
			else {
				std::cout << "RECV: TCP MSG Type: " << data.type << " Sender: " << data.sender_id << " Message: " << data.message << std::endl;
				return data;
			}
		}
	}

	void P2PNetworkManager::receive_tcp_data_loop(TCPClient* client)
	{
		while (_connected && client->is_connected()) {
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
				_peer_connections.at(data_pack.sender_id)->answered_alive = true;
				std::cout << "Connection successfully established! QUIZ: " << quiz << " = " << _id << std::endl;
			}
			else if (data_pack.type == 5) { // received close call
				// setu connection and answered_alive to false, so that the connection gets deleted in the next iteration
				if (data_pack.sender_id = _id) {
					for (auto it = _peer_connections.begin(); it != _peer_connections.end(); ++it) {
						if (it->second->tcp_client == client) {
							it->second->peer_connection = false;
							it->second->answered_alive = false;
						}
					}
				}
				else {
					_peer_connections.at(data_pack.sender_id)->peer_connection = false;
					_peer_connections.at(data_pack.sender_id)->answered_alive = false;
				}
				client->stop(0);
			}
			else if (data_pack.type == 6) { // received alive question
				DataPackage alive_response = DataPackage(7, _id, _network_port, _network_id, _ip, (std::string)"alive\0");
				tcp_send_data(client, &alive_response);
			}
			else if (data_pack.type == 7) { // received alive response
				_peer_connections.at(data_pack.sender_id)->answered_alive = true;
				std::cout << data_pack.sender_id << " is alive" << std::endl;
			}
			else if (data_pack.type == 8) { // spawn tank
				_data_buffer.add_data(new DataPackage(data_pack));
			}
			else if (data_pack.type == 9) { // move tank
				_data_buffer.add_data(new DataPackage(data_pack));
			}

		}
	}

	void P2PNetworkManager::receive_udp_data_loop()
	{
		while (_connected) {
			char sender_ip[INET_ADDRSTRLEN];
			DataPackage data_pack = receive_udp_data(sender_ip);

			if (data_pack.type == 2) { // if join received
				std::cout << "received 2" << std::endl;
				if (_connection_threads.find(data_pack.sender_id) == _connection_threads.end()) {
					std::string ip = data_pack.sender_ip;
					TCPClient * client = setup_connection(ip, _tcp_port, false);

					PeerPartner * peer_partner = new PeerPartner(data_pack.sender_id, _network_id, client);
					_peer_connections.insert(std::pair<int, PeerPartner*>(data_pack.sender_id, peer_partner));
					peer_partner->peer_connection = true;
					peer_partner->answered_alive = true;

					_peer_connections.at(data_pack.sender_id)->peer_connection = true;
					std::thread * t = new std::thread(&P2PNetworkManager::receive_tcp_data_loop, this, client);
					_connection_threads.insert(std::pair<int, std::thread*>(data_pack.sender_id, t));
				}
			}
			else if (data_pack.type == 3) {
				std::cout << "type 3" << std::endl;
			}

		}
	}

	void P2PNetworkManager::run_peer_network()
	{
		_running = true;
		std::cout << "Running Peer Network " << _network_id << std::endl;
		_network_broadcast_thread = std::thread(&P2PNetworkManager::broadcast_network_exists_loop, this);
		_udp_network_receive_thread = std::thread(&P2PNetworkManager::receive_udp_data_loop, this);
		_network_connector_thread = std::thread(&P2PNetworkManager::accept_network_connections_loop, this);

		auto now = std::chrono::steady_clock::now();
		while (_running) {
			double temp_time = clock();
			_delta_time = (temp_time - _current_time) / CLOCKS_PER_SEC;
			_current_time = temp_time;
			_alive_timer += _delta_time;

			if (_connected && _alive_timer > 2.0) {
				std::cout << "Connected" << std::endl;
				handle_alive_connections();
				_alive_timer = 0.0;
			}
			std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::duration<double>{(1.0 / _frequency)});
			std::this_thread::sleep_for(duration);
			std::cout << "waited for " << std::to_string(duration.count()) << std::endl;
		}

		_running = false;
	}

	void P2PNetworkManager::stop_peer_network()
	{

		_network_broadcast_thread.join();
		_connected = false;
		_udp_network_receive_thread.join();
		_network_connector_thread.join();

		if (_connected)
			leave_network();

		_running = false;
	}

	void P2PNetworkManager::close_connection(int id)
	{
		std::cout << "Deleting Peer Connection" << id << std::endl;

		// make thread loop stop
		_peer_connections.at(id)->tcp_client->prepare_stop();
		// join thread
		_connection_threads.at(id)->join();

		// close client
		_peer_connections.at(id)->tcp_client->close();

		// delete client
		delete _peer_connections.at(id)->tcp_client;
		_peer_connections.at(id)->tcp_client = nullptr;
		delete _peer_connections.at(id);
		_peer_connections.erase(id);
		// delete thread
		delete _connection_threads.at(id);
		_connection_threads.erase(id);
		std::cout << "Deleted Peer Connection" << id << std::endl;
	}

	std::vector<DataPackage*> const & P2PNetworkManager::read_data()
	{
		_data_buffer.swap_buffers();
		return _data_buffer.get_data();
	}

	std::map<int, Network*> const& P2PNetworkManager::get_networks() const
	{
		return _networks;
	}

	void P2PNetworkManager::broadcast_message(int type, int d1, int d2, int d3,
		float x, float y, float z,
		float angle, float f1, float f2, float f3,
		std::string & msg)
	{
		DataPackage message_data(type, _id, _network_port, _network_id, d1, d2, d3, x, y, z, angle, f1, f2, f3, _ip, msg);
		broadcast_tcp_data(&message_data);
	}

	void P2PNetworkManager::handle_alive_connections()
	{
		std::vector<int> peers_to_delete = std::vector<int>();
		for (auto connection : _peer_connections)
		{
			if (connection.second->answered_alive == false) {
				std::cout << "Guess " << connection.first << " died..." << std::endl;
				connection.second->peer_connection = false;
			}
			else {
				connection.second->answered_alive = false;
				//std::cout << "Send alive msg to " << connection.first << std::endl;
				DataPackage alive_question = DataPackage(6, _id, _network_port, _network_id, _ip, (std::string)"u ded?");
				alive_question.int_data_1 = alive_question.sender_id * _id;
				tcp_send_data(connection.second->tcp_client, &alive_question);
			}

			if (connection.second->peer_connection == false) {
				std::cout << "closing connection" << std::endl;
				peers_to_delete.push_back(connection.first);
			}
		}
		for (auto const& id : peers_to_delete) {
			_peer_connections.at(id)->tcp_client->prepare_stop();
			// join thread
			_connection_threads.at(id)->join();
			// close client
			_peer_connections.at(id)->tcp_client->close();
			// delete client
			delete _peer_connections.at(id)->tcp_client;
			_peer_connections.at(id)->tcp_client = nullptr;
			delete _peer_connections.at(id);
			_peer_connections.erase(id);
			// delete thread
			delete _connection_threads.at(id);
			_connection_threads.erase(id);

			std::cout << "Deleted " << id << std::endl;
		}
	}

	void P2PNetworkManager::set_frequency(double freq)
	{
		_frequency = freq;
	}

	void P2PNetworkManager::broadcast_network_exists_loop()
	{
		//std::cout << "Sending Network signal to port" << _app_broadcaster->get_address()->get_port() <<std::endl;
		while (_connected) {
			DataPackage dp = DataPackage(1, _id, _network_port, _network_id, _ip, (std::string)"exists\0");
			app_broadcast_data(&dp);
			Sleep(200);
		}

	}

	void P2PNetworkManager::accept_network_connections_loop()
	{
		while (_connected) {
			TCPClient * client = _net_tcp_listener->accept();
			if (client != nullptr) {
				std::cout << "Sending pack 3" << std::endl;
				DataPackage accepted_pack = DataPackage(3, _id, _network_port, _network_id, _ip, (std::string)"accepted\0");
				tcp_send_data(client, &accepted_pack);
				char sender_ip[INET_ADDRSTRLEN];
				DataPackage accepted_response = receive_tcp_data(client, sender_ip);
				if (accepted_response.type == 4) {
					int sender_id = accepted_response.sender_id;
					// send that you got established message
					std::cout << "sending pack 4 " << client << std::endl;
					DataPackage established = DataPackage(4, _id, _network_port, _network_id, _ip, (std::string)"established\0");
					established.int_data_1 = sender_id * _id;
					//client->send(&established, sizeof(DataPackage));
					tcp_send_data(client, &established);

					// add connection
					PeerPartner * peer_partner = new PeerPartner(sender_id, _network_id, client);
					_peer_connections.insert(std::pair<int, PeerPartner*>(sender_id, peer_partner));
					peer_partner->peer_connection = true;
					peer_partner->answered_alive = true;
					// add thread
					std::thread * t = new std::thread(&P2PNetworkManager::receive_tcp_data_loop, this, client);// &_peer_connections.at(sender_id)->peer_connection);
						//&peer_partner->peer_connection);//_peer_connections.at(sender_id)->peer_connection);
					_connection_threads.insert(std::pair<int, std::thread*>(accepted_response.sender_id, t));
					int quiz = accepted_response.int_data_1 / accepted_response.sender_id;

					std::cout << "Connection successfully established! QUIZ: " << quiz << " = " << _id << std::endl;
				}
			}
			else {
				//std::cout << "Error accepting client!" << std::endl;
			}
			Sleep(200);
		}

	}

	void P2PNetworkManager::setup_tcp_listener(std::string ip, int port)
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

	void P2PNetworkManager::setup_udp_listener(std::string ip, int port)
	{
		_udp_port = port;
		int binding_result = _net_udp_listener->bind(ip, _udp_port, true);
		if (binding_result == SOCKET_ERROR) {
			std::cout << "Error binding udp listener on port " << _udp_port << std::endl;
			throw std::invalid_argument("Error binding");
		}
		else {
			std::cout << "UDP setup on port " << _net_udp_listener->get_address()->get_port() << std::endl;
		}
	}

	void P2PNetworkManager::setup_app_broadcaster(std::string ip, int port)
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

	void P2PNetworkManager::setup_net_broadcaster(std::string ip, int port)
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

	TCPClient * P2PNetworkManager::setup_connection(std::string & ip, int port, bool blocking)
	{
		TCPClient * tcp_client = new TCPClient(new Socket_WIN());
		int res = tcp_client->connect(ip, port, blocking);
		if (res != SOCKET_ERROR) {
			std::cout << "Connected TCPClient to " << tcp_client->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting TCPClient to " << tcp_client->get_address()->get_port() << std::endl;
		}
		return tcp_client;
	}
}
