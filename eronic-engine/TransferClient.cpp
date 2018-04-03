#include <iostream>

#include "TransferClient.h"

namespace eronic {
	TransferClient::TransferClient() :
		_transfer_socket()
	{
	}

	TransferClient::TransferClient(Socket * transfer_socket):
		_transfer_socket(transfer_socket)
	{
	}

	TransferClient::~TransferClient()
	{}

	bool TransferClient::connect(std::string & ip, int port)
	{
		return _transfer_socket->connect_to(ip, port);
	}

	bool TransferClient::send(const void * data, size_t data_size)
	{
		return _transfer_socket->send_data(data, data_size);
	}

	bool TransferClient::receive(void * data, size_t data_size)
	{
		return _transfer_socket->receive_data(data, data_size);
	}
	
} // namespace eronic