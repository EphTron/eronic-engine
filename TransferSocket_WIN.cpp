#include "TransferSocket_WIN.h"

#include <iostream>


namespace eronic {

	TransferSocket_WIN::TransferSocket_WIN() :
		_wsa_data(),
		_socket_handle(),
		_address()
	{
		// Create version identifier
		WORD w_version_requested = MAKEWORD(2, 0);

		// Startup windows sockets
		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}

	TransferSocket_WIN::~TransferSocket_WIN()
	{
		WSACleanup();
	}

	bool TransferSocket_WIN::connect_to(std::string& ip, int port)
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

	bool TransferSocket_WIN::send_to(const void* data, size_t data_size)
	{
		if (send(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return false;
		}
		else {
			return true;
		}
	}

	bool TransferSocket_WIN::receive_from(void* data, size_t data_size)
	{
		if (recv(_socket_handle, (char*)data, data_size, 0) == SOCKET_ERROR) {
			return false;
		}
		else {
			return true;
		}
	}
} // namespace eronic