#pragma once

#include <iostream>

#include "Address.h"

namespace eronic {
	class Socket
	{
	public:
		Socket();
		Socket(int socket_handle);
		virtual ~Socket();

		virtual int connect_to(int type, std::string& ip, int port) = 0;
		virtual int bind_to(int type, std::string& ip, int port) = 0;
		virtual int start_listening(int max_connections) = 0;

		virtual Address const* get_address() const = 0;

		virtual int send_data(const void* data, size_t data_size) = 0;
		virtual int receive_data(void* data, size_t data_size) = 0;

		virtual Socket* accept_connection() = 0;

		virtual int close() = 0;
		virtual int stop(int how) = 0;

	protected:
		Address _address;
		int _socket_handle;
		
	};

} // namespace eronic