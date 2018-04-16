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

		bool connect(std::string& ip, int port, bool blocking);

		int send(const void* data, size_t data_size);
		int receive(void* data, size_t data_size);
		int receivefrom(void* data, size_t data_size, char* sender_ip);

		int set_blocking(bool flag);

		int stop(int how);
		int close();
		int prepare_stop();
		
		Address const * get_address() const;
		bool const is_connected() const;

	private:
		Socket * _socket;
		bool _is_connected;
	};

} // namespace eronic