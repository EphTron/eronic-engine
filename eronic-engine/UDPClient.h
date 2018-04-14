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

		int connect(std::string& ip, int port, bool blocking);

		int send(const void* data, size_t data_size);
		int receive(void* data, size_t data_size);
		int receivefrom(void * data, size_t data_size, char * sender_ip);

		int set_blocking(bool flag);

		int stop(int how);
		int close();

		Address const * get_address() const;
	private:
		Socket * _socket;
		bool _is_connected;
	};

}