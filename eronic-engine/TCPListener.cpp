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
		_listening_socket->close();
	}

	int TCPListener::bind(std::string & ip, int port, bool blocking)
	{
		int TCP = 1;
		int bind_result = _listening_socket->bind_to(TCP, ip, port);

		if (bind_result == SOCKET_ERROR) {
			return WSAGetLastError();
		}
		else {
			std::cout << "Bind TCPListener to " << _listening_socket->get_address()->get_ip() << ":" << _listening_socket->get_address()->get_port() << std::endl;
			_is_bound = true; 
			_is_listening = true;
			if (!blocking) {
				_listening_socket->set_blocking(false);
			}
			return bind_result;
		}
		//u_long iMode = 0;
		//WSAAsyncSelect(_listening_socket, nullptr, MY_MESSAGE_NOTIFICATION, FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT); //Set
		//int result = ioctlsocket(_listening_socket, FIONBIO, &iMode);
		
	}

	int TCPListener::start(int max_connections)
	{
		std::cout << "START TCP LISTENING!!!!!!!!!!!!!!!!!!!!" << _listening_socket->get_address()->get_ip();
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

	int TCPListener::set_blocking(bool flag)
	{
		return _listening_socket->set_blocking(flag);
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