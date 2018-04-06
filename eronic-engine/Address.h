#pragma once

#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>


class Address
{
public:
	Address();
	Address(std::string &ip, int port);
	//Address(struct sockaddr *);
	//Address(Address& address) = delete;
	~Address();

	sockaddr_in const* get_socket_address() const;
	int get_port() const;
	std::string const& get_ip() const;

private:
	sockaddr_in _socket_address;
	std::string _ip;
	int _port;
};

