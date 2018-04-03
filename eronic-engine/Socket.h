#pragma once

#include <iostream>

namespace eronic {
	class Socket
	{
	public:
		Socket();
		Socket(int socket_handle);
		virtual ~Socket();

		virtual bool connect_to(std::string& ip, int port) = 0;
		virtual bool bind_to(std::string& ip, int port, int max_connections) = 0;

		virtual bool send_data(const void* data, size_t data_size) = 0;
		virtual bool receive_data(void* data, size_t data_size) = 0;

		virtual Socket* accept_connection() = 0;

	protected:
		int _socket_handle;
	};

} // namespace eronic