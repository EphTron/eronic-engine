#pragma once

#include <iostream>

#include "Socket.h"


namespace eronic {

	class TCPClient
	{
	public:
		TCPClient();
		TCPClient(Socket * _socket);
		~TCPClient();

		bool connect(std::string& ip, int port);

		int send(const void* data, size_t data_size);
		bool receive(void* data, size_t data_size);
	private:
		Socket * _socket;
		bool _is_connected;
	};

} // namespace eronic