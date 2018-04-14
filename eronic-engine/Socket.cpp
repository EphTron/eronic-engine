#include "Socket.h"

namespace eronic {

	Socket::Socket() :
		_socket_handle(-1),
		_blocking(true)
	{
	}


	Socket::Socket(int socket_handle) :
		_socket_handle(socket_handle),
		_blocking(true)
	{
	}


	Socket::~Socket()
	{
	}
} // namespace eronic