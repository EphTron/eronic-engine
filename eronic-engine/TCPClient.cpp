#include <iostream>

#include "TCPClient.h"

namespace eronic {
	TCPClient::TCPClient() :
		_socket(),
		_is_connected(false)
	{
	}

	TCPClient::TCPClient(Socket * _socket):
		_socket(_socket),
		_is_connected(false)
	{
	}

	TCPClient::~TCPClient()
	{}

	bool TCPClient::connect(std::string & ip, int port)
	{
		int TCP = 1;
		int connect_result = _socket->connect_to(TCP, ip, port);
		if (connect_result == 0) {
			_is_connected = true;
			return connect_result;
		}
		else {
			return connect_result;
		}
	}

	int TCPClient::send(const void * data, size_t data_size)
	{
		return _socket->send_data(data, data_size);
	}

	bool TCPClient::receive(void * data, size_t data_size)
	{
		return _socket->receive_data(data, data_size);
	}

	int TCPClient::stop(int how)
	{
		int result = _socket->stop(how);
		if (result == 0) {
			return result;
			_is_connected = true;
		}
		else {
			return result;
		}
	}

	int TCPClient::close()
	{
		int result = _socket->close();
		if (result == 0) {
			_is_connected = true;
			return result;
		}
		else {
			return result;
		}
	}
	
} // namespace eronic