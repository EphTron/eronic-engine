#pragma once

#include <iostream>

#include "Socket.h"

namespace eronic {

	class TransferClient
	{
	public:
		TransferClient();
		TransferClient(Socket * transfer_socket);
		~TransferClient();

		bool connect(std::string& ip, int port);

		bool send(const void* data, size_t data_size);
		bool receive(void* data, size_t data_size);
	private:
		Socket * _transfer_socket;
	};

} // namespace eronic