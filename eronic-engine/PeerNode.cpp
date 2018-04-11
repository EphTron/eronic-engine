#include "PeerNode.h"

namespace eronic {

	PeerNode::PeerNode()
	{
	}

	PeerNode::PeerNode(bool same_machine, int app_port, int max_connections) :
		_running(false),
		_connected(false),
		_same_machine(same_machine),
		_id(),
		_ip(),
		_udp_port(-1),
		_tcp_port(-1),
		_network_id(-1),
		_network_port(-1),
		_app_udp_port(app_port),
		_app_tcp_port(app_port),
		_max_connections(max_connections),
		_app_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN())),
		_net_udp_listener(new eronic::UDPListener(new eronic::Socket_WIN())),
		_net_broadcaster(new eronic::UDPClient(new eronic::Socket_WIN()))

		//_net_tcp_listener(new eronic::TCPListener(new eronic::Socket_WIN())),
		//_net_tcp_client(new eronic::TCPClient(new eronic::Socket_WIN())),
		//_net_connections(std::list<eronic::TCPClient *>())
	{
		_id = std::rand();
		std::cout << "PeerNode id = " << _id << std::endl;
		if (_same_machine) {
			// get external ip address;
			_ip = "127.0.0.1";

			// setup udp listener to listen for networks in the app
			int binding_result = -1;
			_app_udp_port = app_port;
			while (_app_udp_port <= app_port + 1 && binding_result != 0) {
				binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port);
				if (binding_result == 10048) {
					_app_udp_port++;
				}
				else if (binding_result != 0) {
					std::cout << "Error binding listener on port " << _app_udp_port << std::endl;
					throw std::invalid_argument("Error binding");
				}
				else {
					std::cout << "Peer listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
				}
			}

			// connect UDP Client (for same machine)
			// TODO: use this ip for broadcast in network later 192.168.1.255
			int connect_result = -1;
			if (_app_udp_port == app_port) {
				connect_result = _app_broadcaster->connect((std::string)"127.0.0.1", app_port + 1);
			}
			else {
				connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", app_port);
			}
			if (connect_result == 0) {
				std::cout << "Peer connected to " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
			else {
				std::cout << "Error connecting udp client to " << _net_udp_listener->get_address()->get_port() << std::endl;
				throw std::invalid_argument("Error connecting");
			}
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

			// setup udp listener
			_net_udp_listener->close();
			int binding_result = -1;
			while (_udp_port <= _network_port + 1 && binding_result != 0) {
				binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _udp_port);
				if (binding_result == 10048) {
					_udp_port++;
				}
				else if (binding_result != 0) {
					std::cout << "Error binding listener on port " << _udp_port << std::endl;
				}
				else {
					std::cout << "Peer listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
				}
			}

			// connect UDP Client (for same machine)
			// TODO: use this ip for broadcast in network later 192.168.1.255
			_net_broadcaster->stop(0);
			int connect_result = -1;
			if (_udp_port == _network_port)
				connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", _network_port + 1);
			else
				connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", _network_port);

			if (connect_result == 0) {
				std::cout << "Peer connected to " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
			else {
				std::cout << "Error connecting udp client to " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
			_connected = true;
		}
		else {
			// get external ip address;
			//_ip = eronic::get_external_ip((std::string)"api.ipify.org");
		}
		_network_broadcast_thread = std::thread(&PeerNode::broadcast_network_exists, this);
		
	}

	void PeerNode::join_network(int network_id, int network_port)
	{
		_network_id = network_id;
		_network_port = network_port;
		_udp_port = _network_port;
		if (_same_machine) {
			// get external ip address;
			_ip = "127.0.0.1";

			// setup udp listener
			_net_udp_listener->close();
			int binding_result = -1;
			while (_udp_port <= _network_port + 1 && binding_result != 0) {
				binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _udp_port);
				if (binding_result == 10048) {
					_udp_port++;
				}
				else if (binding_result != 0) {
					std::cout << "Error binding listener on port " << _udp_port << std::endl;
				}
				else {
					std::cout << "Peer listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
				}
			}

			// connect UDP Client (for same machine)
			// as we are on the same machine the app wont run on the same port
			// TODO: use this ip for broadcast in network later 192.168.1.255
			_net_broadcaster->stop(0);
			int connect_result = -1;
			if (_udp_port == _network_port)
				connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", _network_port + 1);
			else
				connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", _network_port);

			if (connect_result == 0) {
				std::cout << "Peer connected to " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
			else {
				std::cout << "Error connecting udp client to " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
		}
		else {
			// get external ip address;
			//_ip = eronic::get_external_ip((std::string)"api.ipify.org");
		}
	}

	void PeerNode::leave_network()
	{
		_connected = false;
	}

	std::vector<Network*>& PeerNode::find_networks()
	{
		std::vector<Network*> network_results = std::vector<Network*>();
		char data[sizeof(DataPackage)];
		int receive_result = _net_udp_listener->receive(data, sizeof(DataPackage));
		if (receive_result != SOCKET_ERROR) {
			std::cout << "recv result:" << receive_result << std::endl;
			DataPackage dp = DataPackage(data);
			Network* new_network = new Network();
			new_network->network_port = dp.int_data_1;
			new_network->ip = dp.sender_ip;
			new_network->network_id = dp.network_id;
			network_results.push_back(new_network);
			std::cout << "Found network " << new_network->network_id << " Sender was " << dp.sender_id << std::endl;
			return network_results;
		}
		else
		{
			return network_results;
		}
	}

	bool PeerNode::app_broadcast_data(eronic::DataPackage * data)
	{
		int send_result = _app_broadcaster->send(data->serialize(), sizeof(*data));
		if (send_result == 0) {
			return true;
		}
		else {
			return false;
		}

	}

	bool PeerNode::net_broadcast_data(eronic::DataPackage * data)
	{
		int send_result = _net_broadcaster->send(data->serialize(), sizeof(*data));
		if (send_result == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	eronic::DataPackage * PeerNode::receive_data()
	{
		char data[sizeof(eronic::DataPackage)];
		int recv_result = _net_udp_listener->receive(data, sizeof(eronic::DataPackage));
		if (recv_result == 0) {
			return new eronic::DataPackage(data);
		}
		else {
			return nullptr;
		}
	}

	void PeerNode::run()
	{
		_running = true;
		while (_running) {
			
		}
	}
	void PeerNode::broadcast_network_exists()
	{
		while (_connected) {
			std::cout << "network " << _network_id << " exists" << std::endl;
			//DataPackage dp = DataPackage(1, 2, _network_port, _ip, std::to_string(_network_id));
			DataPackage dp = DataPackage();
			dp.type = 1;
			dp.network_id = _network_id;
			dp.int_data_1 = _network_port;
			dp.set_ip(_ip);
			app_broadcast_data(&dp);

			Sleep(1500);
		}

	}
	/*
	if (same_machine) {
		// get external ip address;
		_ip = "127.0.0.1";

		// setup udp listener
		int binding_result = -1;
		while (_app_udp_port <= _app_udp_port + 1 && binding_result != 0) {
			binding_result = _net_udp_listener->bind((std::string)"ADDR_ANY", _app_udp_port);
			if (binding_result == 10048) {
				_app_udp_port++;
			}
			else if (binding_result != 0) {
				std::cout << "Error binding listener on port " << _app_udp_port << std::endl;
			}
			else {
				std::cout << "Peer listening on " << _net_udp_listener->get_address()->get_port() << std::endl;
			}
		}

		// connect UDP Client (for same machine)
		int connect_result = -1;
		// as we are on the same machine the app wont run on the same port
		// TODO: use this ip for broadcast in network later 192.168.1.255
		if (app_port == _app_udp_port)
			connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", _app_udp_port + 1);
		else
			connect_result = _net_broadcaster->connect((std::string)"127.0.0.1", app_port);

		if (connect_result == 0) {
			std::cout << "Peer connected to " << _net_udp_listener->get_address()->get_port() << std::endl;
		}
		else {
			std::cout << "Error connecting udp client to " << _net_udp_listener->get_address()->get_port() << std::endl;
		}

	}
	else {
		// get external ip address;
		_ip = eronic::get_external_ip((std::string)"api.ipify.org");
	}
	*/

}