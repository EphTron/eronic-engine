#include "TCPListener.h"

namespace eronic {
	TCPListener::TCPListener() :
		_listening_socket(),
		_is_bound(false),
		_is_listening(false)
	{
	}

	TCPListener::TCPListener(Socket * listening_socket) :
		_listening_socket(listening_socket),
		_is_bound(false),
		_is_listening(false)
	{
	}

	TCPListener::~TCPListener()
	{
	}

	int TCPListener::bind(std::string & ip, int port)
	{
		int TCP = 1;
		int bind_result = _listening_socket->bind_to(TCP, ip, port);
		if (bind_result == 0) {
			_is_bound = true;
			return bind_result;
		}
		else {
			return bind_result;
		}
	}

	int TCPListener::start(int max_connections)
	{
		int listen_result = _listening_socket->start_listening(max_connections);
		if (listen_result == 0) {
			_is_listening = true;
			return listen_result;
		}
		else {
			return listen_result;
		}
	}

	int TCPListener::stop(int how)
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

	int TCPListener::close()
	{
		return 0;
	}

	TCPClient * TCPListener::accept()
	{
		Socket * accepted_socket = _listening_socket->accept_connection();
		if (accepted_socket == nullptr)
		{
			return nullptr;
		}
		else {
			TCPClient * accepted_client = new TCPClient(accepted_socket);
			std::cout << "Accepted" << std::endl;
			return accepted_client;
		}
	}

	Address const* TCPListener::get_address() const
	{
		return _listening_socket->get_address();
	}


} // namespace eronic