#pragma once

#include <iostream>

#include "Socket.h"
#include "TransferClient.h"

namespace eronic {

	class ListeningServer
	{
	public:
		ListeningServer();
		ListeningServer(Socket * listening_socket, int max_connections);
		~ListeningServer();

		bool bind(std::string& ip, int port);
		TransferClient* accept();
	private:
		Socket * _listening_socket;
		int _max_connections;
	};

} // namespace eronic