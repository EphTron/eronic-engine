#include "Socket.h"

namespace eronic {

	Socket::Socket() :
		_socket_handle(-1)
	{
	}


	Socket::Socket(int socket_handle) :
		_socket_handle(socket_handle)
	{
	}


	Socket::~Socket()
	{
	}
} // namespace eronic