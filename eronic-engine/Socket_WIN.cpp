#include "Socket_WIN.h"

namespace eronic {

	Socket_WIN::Socket_WIN() :
		_address(),
		_wsa_data()
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
		_address(),
		_wsa_data()
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

	int Socket_WIN::connect_to(int type, std::string& ip, int port)
	{
		_address = Address(ip, port);
		_socket_handle = socket(AF_INET, type, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return WSAGetLastError();
		}
		else if (connect(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
			std::cerr << "SOCKET_WIN.cpp: Connect to peer failed with " << WSAGetLastError() << std::endl;
			return WSAGetLastError();
		}
		else {

			return 0;
		}
	}

	int Socket_WIN::bind_to(int type, std::string & ip, int port)
	{
		_address = Address(ip, port);
		std::cout << "new port ?" <<_address.get_port() << std::endl;
		_socket_handle = socket(AF_INET, type, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return WSAGetLastError();
		}

		// check whether TCP OR UDP
		if (type == SOCK_DGRAM) {
			if (bind(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
				std::cerr << "SOCKET_WIN.cpp: UDP Bind failed at " << _address.get_port() << " with "  << WSAGetLastError() << std::endl;
				return WSAGetLastError();
			}
		}
		else if (type == SOCK_STREAM) {
			if (bind(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
				std::cerr << "SOCKET_WIN.cpp: TCP Bind failed at " << _address.get_port() << " with " << WSAGetLastError() << std::endl;
				return WSAGetLastError();
			}
		}
		std::cout << "BIND to port:  " << _address.get_port() << std::endl;
		return 0;
	}

	int Socket_WIN::start_listening(int max_connections) {
		if (listen(_socket_handle, max_connections) == SOCKET_ERROR) {
			std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
			return WSAGetLastError();
		}
		else {
			return 0;
		}
	}

	Address const* Socket_WIN::get_address() const
	{
		return &_address;
	}

	int Socket_WIN::send_data(const void* data, size_t data_size)
	{
		if (send(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return WSAGetLastError();
		}
		else {
			return 0;
		}
	}

	int Socket_WIN::receive_data(void* data, size_t data_size)
	{
		if (recv(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return WSAGetLastError();
		}
		else {
			return 0;
		}
	}

	Socket* Socket_WIN::accept_connection()
	{
		// Non Blocking accept with the help of select()
		// source: https://stackoverflow.com/questions/2843277/c-winsock-p2p
		fd_set mySet;
		FD_ZERO(&mySet);
		FD_SET(_socket_handle, &mySet);
		timeval zero = { 5, 0};
		// std::cout << "Socket starts select" << std::endl;
		int sel = select(0, &mySet, NULL, NULL, &zero);
		std::cout << "select" << sel << std::endl;
		if (FD_ISSET(_socket_handle, &mySet)) {
			std::cout << "Acception attempt" << std::endl;
			int new_socket_handle = accept(_socket_handle, NULL, NULL);
			Socket* connected_socket = new Socket_WIN(new_socket_handle);
			return connected_socket;
		}
		else {
			std::cout << "No connection " <<WSAGetLastError() << std::endl;
			return nullptr;
		}
	}

	int Socket_WIN::close()
	{
		return closesocket(_socket_handle);
	}

	int Socket_WIN::stop(int how) 
	{
		return shutdown(_socket_handle, how);
	}

} // namespace eronic