#pragma once

#include <iostream>

namespace eronic {

	class TransferSocket
	{
	public:
		TransferSocket();
		virtual ~TransferSocket();

		virtual bool connect_to(std::string& ip, int port) = 0;

		virtual bool send_to(const void* data, size_t data_size) = 0;
		virtual bool receive_from(void* data, size_t data_size) = 0;
	};

} // namespace eronic