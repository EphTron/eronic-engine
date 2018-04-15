#include "UDPListener.h"

namespace eronic {

	UDPListener::UDPListener() :
		_listening_socket(),
		_is_bound(false),
		_is_listening(false)
	{
	}

	eronic::UDPListener::UDPListener(Socket * listening_socket) :
		_listening_socket(listening_socket),
		_is_bound(false),
		_is_listening(false)
	{
	}

	UDPListener::~UDPListener()
	{
		_listening_socket->close();
	}

	int UDPListener::bind(std::string & ip, int port, bool blocking)
	{
		int UDP = 2;
		int bind_result = _listening_socket->bind_to(UDP, ip, port);
		if (bind_result == 0) {
			if (!blocking) {
				_listening_socket->set_blocking(false);
			}
			_is_bound = true;
			return bind_result;
		}
		else {
			return bind_result;
		}
	}

	/*int UDPListener::send(void * data, size_t data_size)
	{
		int send_result = _listening_socket->send_data(data, data_size);
		if (send_result == 0) {
			return 0;
		}
		else {
			return WSAGetLastError();
		}
	}*/

	int UDPListener::receive(void * data, size_t data_size)
	{
		int recv_result = _listening_socket->receive_data(data, data_size);
		if (recv_result != SOCKET_ERROR) {
			return recv_result;
		}
		else {
			return WSAGetLastError();
		}
	}

	int UDPListener::receivefrom(void * data, size_t data_size, char * sender_ip)
	{
		std::cout << "recving " << _listening_socket->get_address()->get_ip() << ":" << _listening_socket->get_address()->get_port() << std::endl;
		return _listening_socket->receive_data_from(data, data_size, sender_ip);
	}

	int UDPListener::set_blocking(bool flag)
	{
		return _listening_socket->set_blocking(flag);
	}

	int UDPListener::stop(int how)
	{
		int result = _listening_socket->stop(how);
		if (result == 0) {
			_is_bound = false;
			_is_listening = false;
			return result;
		}
		else {
			return result;
		}
	}

	int UDPListener::close()
	{
		int result = _listening_socket->close();
		if (result == 0) {
			_is_bound = false;
			_is_listening = false;
			return result;
		}
		else {
			return result;
		}
	}

	Address const * UDPListener::get_address() const
	{
		return _listening_socket->get_address();
	}

}