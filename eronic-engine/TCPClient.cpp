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

	bool TCPClient::connect(std::string & ip, int port, bool blocking)
	{
		int TCP = 1;
		int connect_result = _socket->connect_to(TCP, ip, port);
		if (connect_result != SOCKET_ERROR) {
			if (!blocking) {
				_socket->set_blocking(false);
			}
			_is_connected = true;
			std::cout << "Connected TCP " << std::endl;
			return connect_result;
		}
		else {
			std::cout << "Error TCP client connect " << std::endl;
			return WSAGetLastError();
		}
	}

	int TCPClient::send(const void * data, size_t data_size)
	{
		//std::cout << "tcp sending " << _socket->get_address()->get_ip() << ":" << _socket->get_address()->get_port() << std::endl;
		return _socket->send_data(data, data_size);
	}

	int TCPClient::receive(void * data, size_t data_size)
	{
		return _socket->receive_data(data, data_size);
	}

	int TCPClient::receivefrom(void * data, size_t data_size, char * sender_ip)
	{
		return _socket->receive_data_from(data, data_size, sender_ip);
	}

	int TCPClient::set_blocking(bool flag)
	{
		return _socket->set_blocking(flag);
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

	int TCPClient::prepare_stop()
	{
		_is_connected = false;
		return 0;
	}

	Address const * TCPClient::get_address() const
	{
		return _socket->get_address();
	}

	bool TCPClient::is_connected()
	{
		return is_connected;
	}
	
} // namespace eronic