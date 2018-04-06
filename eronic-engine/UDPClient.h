#pragma once

#include <iostream>

#include "Socket.h"


namespace eronic{

	class UDPClient
	{
	public:
		UDPClient();
		UDPClient(Socket * _socket);
		~UDPClient();

		int connect(std::string& ip, int port);

		int send(const void* data, size_t data_size);
		int receive(void* data, size_t data_size);
	private:
		Socket * _socket;
		bool _is_connected;
	};

}