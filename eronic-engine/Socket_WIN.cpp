#include "Socket_WIN.h"

namespace eronic {

	Socket_WIN::Socket_WIN() :
		_wsa_data(),
		_address()
	{
		// Create version identifier
		WORD w_version_requested = MAKEWORD(2, 0);

		// Startup windows sockets
		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}

	Socket_WIN::Socket_WIN(int socket_handle) :
		Socket(socket_handle),
		_wsa_data(),
		_address()
	{
		WORD w_version_requested = MAKEWORD(2, 0);

		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}


	Socket_WIN::~Socket_WIN()
	{
		WSACleanup();
	}

	bool Socket_WIN::connect_to(std::string& ip, int port)
	{
		_address = Address(ip, port);
		_socket_handle = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return false;
		}
		else if (connect(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
			std::cerr << "Connect to peer failed with " << WSAGetLastError() << std::endl;
			return false;
		}
		else {
			return true;
		}
	}

	bool Socket_WIN::bind_to(std::string & ip, int port, int max_connections)
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
		else if (listen(_socket_handle, max_connections) == SOCKET_ERROR) {
			std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
			return false;
		}
		else {
			return true;
		}
	}

	bool Socket_WIN::send_data(const void* data, size_t data_size)
	{
		if (send(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return false;
		}
		else {
			return true;
		}
	}

	bool Socket_WIN::receive_data(void* data, size_t data_size)
	{
		if (recv(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return false;
		}
		else {
			return true;
		}
	}

	Socket* Socket_WIN::accept_connection()
	{
		// Non Blocking accept with the help of select()
		// source: https://stackoverflow.com/questions/2843277/c-winsock-p2p
		fd_set mySet;
		FD_ZERO(&mySet);
		FD_SET(_socket_handle, &mySet);
		timeval zero = { 0, 0 };
		int sel = select(0, &mySet, NULL, NULL, &zero);
		std::cout << "Selecting connections" << std::endl;
		if (FD_ISSET(_socket_handle, &mySet)) {
			std::cout << "Acception attempt" << std::endl;
			int new_socket_handle = accept(_socket_handle, NULL, NULL);
			Socket* connected_socket = new Socket_WIN(new_socket_handle);
			return connected_socket;
		}
		else {
			std::cout << "No connection attempt" << std::endl;
			return nullptr;
		}
	}

} // namespace eronic