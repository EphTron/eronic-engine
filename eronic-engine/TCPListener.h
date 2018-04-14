#pragma once

#include <iostream>

#include "Socket.h"
#include "TCPClient.h"

namespace eronic {

	class TCPListener
	{
	public:
		TCPListener();
		TCPListener(Socket * listening_socket);
		~TCPListener();

		int bind(std::string& ip, int port, bool blocking);
		int start(int max_connections);

		int stop(int how);
		int close();

		int set_blocking(bool flag);

		TCPClient* accept();

		Address const * get_address() const;
	private:
		Socket * _listening_socket;
		bool _is_bound;
		bool _is_listening;
	};

} // namespace eronic