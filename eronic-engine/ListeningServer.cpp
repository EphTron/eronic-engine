#include "ListeningServer.h"

namespace eronic {
	ListeningServer::ListeningServer() :
		_listening_socket(),
		_max_connections(1)
	{
	}

	ListeningServer::ListeningServer(Socket * listening_socket, int max_connections) :
		_listening_socket(listening_socket),
		_max_connections(max_connections)
	{
	}

	ListeningServer::~ListeningServer()
	{
	}

	bool ListeningServer::bind(std::string & ip, int port)
	{
		return _listening_socket->bind_to(ip, port, _max_connections);
	}

	TransferClient * ListeningServer::accept()
	{
		Socket * accepted_socket = _listening_socket->accept_connection();
		if (accepted_socket == nullptr)
		{
			std::cout << "Error in accept" << std::endl;
			return nullptr;
		}
		else {
			TransferClient * accepted_client = new TransferClient(accepted_socket);
			std::cout << "Accepted" << std::endl;
			return accepted_client;
		}
	}


} // namespace eronic