#pragma once

#include <WinSock2.h>

#include "Address.h"
#include "TransferSocket.h"

namespace eronic {
	class TransferSocket_WIN :
		public TransferSocket
	{
	public:
		TransferSocket_WIN();
		~TransferSocket_WIN();

		bool connect_to(std::string& ip, int port);

		bool send_to(const void* data, size_t data_size);
		bool receive_from(void* data, size_t data_size);

	private:
		WSADATA _wsa_data;
		SOCKET _socket_handle;
		Address _address;
	};
} // namespace eronic

