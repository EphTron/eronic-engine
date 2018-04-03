#include "Address.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

Address::Address() :
	_ip("127.0.0.1"),
	_port(8000)
{
	_socket_address.sin_family = AF_INET;
	_socket_address.sin_port = htons(_port);
	_socket_address.sin_addr.S_un.S_addr = inet_pton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);
}

Address::Address(std::string &ip, int port) :
	_ip(ip),
	_port(port)
{
	_socket_address.sin_family = AF_INET;
	_socket_address.sin_port = htons(_port);
	if (_ip == "ANY") {
		_socket_address.sin_addr.s_addr = htonl( INADDR_ANY );
	}
	else {
		_socket_address.sin_addr.S_un.S_addr = inet_pton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);
	}
}

//Address::Address(Address& address) :
//	_socket_address(*address.get_socket_address()),
//	_port(address.get_port()),
//	_ip(address.get_ip())
//{
//	/*_socket_address.sin_family = AF_INET;
//	_socket_address.sin_port = htons(_port);
//	InetPton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);*/
//}


Address::~Address()
{
}

sockaddr_in const* Address::get_socket_address() const
{
	return &_socket_address;
}

int Address::get_port() const
{
	return _port;
}

std::string const& Address::get_ip() const
{
	return _ip;
}
