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

	int UDPListener::bind(std::string & ip, int port)
	{
		int UDP = 2;
		int bind_result = _listening_socket->bind_to(UDP, ip, port);
		if (bind_result == 0) {
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
		if (recv_result == 0) {
			return 0;
		}
		else {
			return WSAGetLastError();
		}
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