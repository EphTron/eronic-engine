#pragma once

#include "Socket.h"
#include "UDPClient.h"

namespace eronic {

	class UDPListener
	{
	public:
		UDPListener();
		UDPListener(Socket * listening_socket);
		~UDPListener();

		int bind(std::string& ip, int port, bool blocking);

		//int send(void * data, size_t data_size);
		int receive(void * data, size_t data_size);
		int receivefrom(void * data, size_t data_size, char * sender_ip);
		int set_blocking(bool flag);

		int stop(int how);
		int close();

		Address const * get_address() const;
	private:
		Socket * _listening_socket;
		bool _is_bound;
		bool _is_listening;
	};

} // namespace eronic