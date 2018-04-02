#pragma once

#include <iostream>

#include "TransferSocket.h"

namespace eronic {

	class ListeningSocket
	{
	public:
		ListeningSocket();
		virtual ~ListeningSocket();

		virtual bool bind_to(std::string& ip, int port) = 0;
		virtual TransferSocket* accept_connection() = 0;
		//virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout) = 0;
	};

} // namespace eronic