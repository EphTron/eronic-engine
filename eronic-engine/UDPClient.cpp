#include "UDPClient.h"

namespace eronic {

	UDPClient::UDPClient() :
		_socket(),
		_is_connected(false)
	{
	}

	UDPClient::UDPClient(Socket * _socket) :
		_socket(_socket),
		_is_connected(false)
	{
	}

	UDPClient::~UDPClient()
	{}

	int UDPClient::connect(std::string & ip, int port)
	{
		int UDP = 2;
		int connect_result = _socket->connect_to(UDP, ip, port);
		if (connect_result == 0) {
			_is_connected = true;
			return connect_result;
		}
		else {
			return connect_result;
		}
	}

	int UDPClient::send(const void * data, size_t data_size)
	{
		return _socket->send_data(data, data_size);
	}

	int UDPClient::receive(void * data, size_t data_size)
	{
		return _socket->receive_data(data, data_size);
	}
} // namespace eronic