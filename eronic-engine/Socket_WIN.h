#pragma once

#include <WinSock2.h>

#include "Address.h"
#include "Socket.h"

namespace eronic {

	class Socket_WIN :
		public Socket
	{
	public:
		Socket_WIN();
		Socket_WIN(int socket_handle);
		~Socket_WIN();

		bool connect_to(std::string& ip, int port);
		bool bind_to(std::string& ip, int port, int max_connections = 5);

		bool send_data(const void* data, size_t data_size);
		bool receive_data(void* data, size_t data_size);

		Socket* accept_connection();
	private:
		WSADATA _wsa_data;
		Address _address;
	};

} // namespace eronic