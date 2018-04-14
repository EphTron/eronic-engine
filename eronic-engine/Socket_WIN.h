#pragma once

#include <WinSock2.h>
#include <sstream>
#include <locale>

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

		int connect_to(int type, std::string& ip, int port);
		int bind_to(int type, std::string& ip, int port);
		int start_listening(int max_connections);
		int set_blocking(bool flag);

		Address const* get_address() const;

		int send_data(const void* data, size_t data_size);
		int receive_data(void* data, size_t data_size);
		int receive_data_from(void* data, size_t data_size, char *sender_ip);

		Socket* accept_connection();

		int close();
		int stop(int how);

	private:
		Address _address;
		WSADATA _wsa_data;
	};

	std::string get_external_ip(std::string &url);
} // namespace eronic