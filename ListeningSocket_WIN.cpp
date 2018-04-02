#include "ListeningSocket_WIN.h"

namespace eronic {

	ListeningSocket_WIN::ListeningSocket_WIN() :
		_wsa_data(),
		_socket_handle(),
		_address(),
		_backlog(5)
	{
		// Create version identifier
		WORD w_version_requested = MAKEWORD(2, 0);

		// Startup windows sockets
		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}

	ListeningSocket_WIN::ListeningSocket_WIN(int backlog) :
		_wsa_data(),
		_socket_handle(),
		_address(),
		_backlog(backlog)
	{
		// Create version identifier
		WORD w_version_requested = MAKEWORD(2, 0);

		// Startup windows sockets
		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}


	ListeningSocket_WIN::~ListeningSocket_WIN()
	{
		WSACleanup();
	}

	bool ListeningSocket_WIN::bind_to(std::string & ip, int port)
	{
		
		_address = Address(ip, port);
		_socket_handle = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return false;
		}
		else if (bind(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
			std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
			return false;
		}
		else if (listen(_socket_handle, _backlog) == SOCKET_ERROR) {
			std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
			return false;
		}
		else {
			return true;
		}
	}

	TransferSocket * ListeningSocket_WIN::accept_connection()
	{
		// receiving data
		fd_set mySet;
		FD_ZERO(&mySet);
		FD_SET(_socket_handle, &mySet);
		// loop all your sockets and add to the mySet like the call above
		timeval zero = { 0, 0 };
		int sel = select(0, &mySet, NULL, NULL, &zero);
		if (FD_ISSET(_socket_handle, &mySet)) {
			// you have a new caller
			sockaddr_in remote;
			SOCKET newSocket = accept(_socket_handle, (SOCKADDR*)&remote, sizeof(remote));
			// TO DO
			return newSocket;
		}
		
	}

	/*int ListeningSocket_WIN::
		select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, const timeval * timeout)
	{
		return 0;
	}*/
} // namespace eronic