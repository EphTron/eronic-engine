#pragma once

#include <WinSock2.h>

#include "Address.h"
#include "ListeningSocket.h"

namespace eronic {

	class ListeningSocket_WIN :
		public ListeningSocket
	{
	public:
		ListeningSocket_WIN();
		ListeningSocket_WIN(int backlog);
		~ListeningSocket_WIN();
		 
		bool bind_to(std::string& ip, int port);
		TransferSocket* accept_connection();
		//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);

	private:
		WSADATA _wsa_data;
		SOCKET _socket_handle;
		Address _address;
		int _backlog;
	};

} // namespace eronic