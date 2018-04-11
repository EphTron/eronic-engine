#include "Address.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

Address::Address() :
	_ip("127.0.0.1"),
	_port(8000)
{
	_socket_address.sin_family = AF_INET;
	_socket_address.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &(_socket_address.sin_addr));
	//_socket_address.sin_addr.S_un.S_addr = inet_pton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);
	//InetPton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);
}

Address::Address(std::string &ip, int port) :
	_ip(ip),
	_port(port)
{
	_socket_address.sin_family = AF_INET;
	_socket_address.sin_port = htons(_port);
	if (_ip == "ADDR_ANY") {
		//InetPton(AF_INET, INADDR_ANY, &_socket_address.sin_addr.S_un.S_addr);
		//inet_pton(AF_INET, INADDR_ANY, &(_socket_address.sin_addr));
		_socket_address.sin_addr.S_un.S_addr = INADDR_ANY;
		char address_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(_socket_address.sin_addr), address_ip, INET_ADDRSTRLEN);
		_ip = address_ip;
	}
	else {
		inet_pton(AF_INET, _ip.c_str(), &(_socket_address.sin_addr));
		//_socket_address.sin_addr.S_un.S_addr = inet_pton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);
	}
}


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
	char address_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(_socket_address.sin_addr), address_ip, INET_ADDRSTRLEN);
	return _ip;
}

//Address::Address(sockaddr * addr) :
//	_ip(),
//	_port()
//{
//	_socket_address = sockaddr_in(& sockaddr(*addr));
//	
//}

//Address::Address(Address& address) :
//	_socket_address(*address.get_socket_address()),
//	_port(address.get_port()),
//	_ip(address.get_ip())
//{
//	/*_socket_address.sin_family = AF_INET;
//	_socket_address.sin_port = htons(_port);
//	InetPton(AF_INET, _ip.c_str(), &_socket_address.sin_addr.S_un.S_addr);*/
//}
//

////inet_ntoa(struct in_addr in);
//struct sockaddr_in {
//u_short     sin_family;
//u_short     sin_port;
//struct      in_addr sin_addr;
//char        sin_zero[8];
//};
////
//char adress_ip[INET_ADDRSTRLEN];
//
//// now get it back and print it
//inet_ntop(AF_INET, &(_socket_address.sin_addr), adress_ip, INET_ADDRSTRLEN);
//std::cout << "ADRESS IP" << adress_ip << std::endl;

// printf("%s\n", str); // prints "192.0.2.33"
//InetNtop()
