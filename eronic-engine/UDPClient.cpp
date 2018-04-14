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

	int UDPClient::connect(std::string & ip, int port, bool blocking)
	{
		int UDP = 2;
		int connect_result = _socket->connect_to(UDP, ip, port);
		if (connect_result == 0) {
			if (!blocking) {
				_socket->set_blocking(false);
			}
			_is_connected = true;
			return connect_result;
		}
		else {
			return connect_result;
		}
	}

	int UDPClient::send(const void * data, size_t data_size)
	{
		int i = _socket->send_data(data, data_size);
		std::cout << "i :" << i << std::endl;
		return i;
	}

	int UDPClient::receive(void * data, size_t data_size)
	{
		return _socket->receive_data(data, data_size);
	}

	int UDPClient::receivefrom(void * data, size_t data_size, char * sender_ip)
	{
		return _socket->receive_data_from(data, data_size, sender_ip);
	}

	int UDPClient::set_blocking(bool flag)
	{
		return _socket->set_blocking(flag);
	}


	int UDPClient::stop(int how)
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

	int UDPClient::close()
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
	Address const * UDPClient::get_address() const
	{
		 return _socket->get_address();;
	}
} // namespace eronic