#include "Socket_WIN.h"

namespace eronic {

	Socket_WIN::Socket_WIN() :
		_address(),
		_wsa_data()
	{
		// Create version identifier
		WORD w_version_requested = MAKEWORD(2, 0);

		// Startup windows sockets
		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}

	Socket_WIN::Socket_WIN(int socket_handle) :
		Socket(socket_handle),
		_address(),
		_wsa_data()
	{
		WORD w_version_requested = MAKEWORD(2, 0);

		if (WSAStartup(w_version_requested, &_wsa_data) != 0) {
			WSACleanup();
		}
	}


	Socket_WIN::~Socket_WIN()
	{
		WSACleanup();
	}


	int Socket_WIN::connect_to(int type, std::string& ip, int port)
	{
		_address = Address(ip, port);

		_socket_handle = socket(AF_INET, type, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return WSAGetLastError();
		}
		else if (connect(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
			std::cerr << "SOCKET_WIN.cpp: Connect to peer failed with " << WSAGetLastError() << std::endl;
			return WSAGetLastError();
		}
		else {
			std::cout << "Connected to port:  " << _address.get_ip() << ":" << _address.get_port() << std::endl;
			return 0;
		}
		
	}

	int Socket_WIN::bind_to(int type, std::string & ip, int port)
	{
		_address = Address(ip, port);
		_socket_handle = socket(AF_INET, type, 0);
		if (_socket_handle == INVALID_SOCKET) {
			std::cerr << "Create socket failed" << std::endl;
			return WSAGetLastError();
		}

		// check whether TCP OR UDP
		if (type == SOCK_DGRAM) {
			if (bind(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
				std::cerr << "SOCKET_WIN.cpp: UDP Bind failed at " << _address.get_port() << " with "  << WSAGetLastError() << std::endl;
				return WSAGetLastError();
			}
		}
		else if (type == SOCK_STREAM) {
			if (bind(_socket_handle, (sockaddr *)_address.get_socket_address(), sizeof(*_address.get_socket_address())) == SOCKET_ERROR) {
				std::cerr << "SOCKET_WIN.cpp: TCP Bind failed at " << _address.get_port() << " with " << WSAGetLastError() << std::endl;
				return WSAGetLastError();
			}
		}
		std::cout << "BIND to port:  " << _address.get_ip() << ":" << _address.get_port() << std::endl;
		return 0;
	}

	int Socket_WIN::start_listening(int max_connections) {
		if (listen(_socket_handle, max_connections) == SOCKET_ERROR) {
			std::cerr << "Listen failed with " << WSAGetLastError() << std::endl;
			return WSAGetLastError();
		}
		else {
			return 0;
		}
	}

	int Socket_WIN::set_blocking(bool flag) {
		u_long NonBlock;
		if (flag) {
			NonBlock = 0;
		}
		else {
			NonBlock = 1;
		}

		int set_result = ioctlsocket(_socket_handle, FIONBIO, &NonBlock);
		if (set_result == SOCKET_ERROR) {
			std::cout << "Setting blocking/non-blocking failed";
			return set_result;
		}
		else
		{
			_blocking = flag;
			std::cout << "Set socket blocking = " << flag << " socket now =" << _blocking<<std::endl;
			return set_result;
		}
	}

	Address const* Socket_WIN::get_address() const
	{
		return &_address;
	}

	int Socket_WIN::send_data(const void* data, size_t data_size)
	{
		if (!_blocking) {
			std::cout << "non blocking send" << _blocking << std::endl;
			int send_result = send(_socket_handle, (char*)data, data_size, 0);
			if (send_result == SOCKET_ERROR) {
				return WSAGetLastError();
			}
			else {
				return send_result;
			}
		}
		else {
			std::cout << "blocking send" << _blocking << std::endl;
			fd_set mySet;
			FD_ZERO(&mySet);
			FD_SET(_socket_handle, &mySet);
			timeval zero = { 1, 0 };
			int sel = select(0, NULL, &mySet, NULL, &zero);
			if (FD_ISSET(_socket_handle, &mySet)) {
				int send_result = send(_socket_handle, (char*)data, data_size, 0);
				if ( send_result == SOCKET_ERROR) {
					return WSAGetLastError();
				}
				else {
					return send_result;
				}
			}
			else {
				std::cout << "Timed out" << std::endl;
				return -1;
			}
		}
		
	}

	int Socket_WIN::receive_data(void* data, size_t data_size)
	{
		if (!_blocking) {
			int recv_result = recv(_socket_handle, (char*)data, data_size, 0);
			if (recv_result == SOCKET_ERROR) {
				return WSAGetLastError();
			}
			else {
				return recv_result;
			}
		}
		else {
			fd_set mySet;
			FD_ZERO(&mySet);
			FD_SET(_socket_handle, &mySet);
			timeval zero = { 1, 0 };
			int sel = select(0, &mySet, NULL, NULL, &zero);
			if (FD_ISSET(_socket_handle, &mySet)) {
				int recv_result = recv(_socket_handle, (char*)data, data_size, 0);
				if (recv_result == SOCKET_ERROR) {
					return WSAGetLastError();
				}
				else {
					return recv_result;
				}
			}
			else {
				std::cout << "Timed out" << std::endl;
				return -1;
			}
		}
	}

	int Socket_WIN::receive_data_from(void * data, size_t data_size, char *sender_ip)
	{
		if (!_blocking) {
			sockaddr_in client; // Use to hold the client information (port / ip address)
			int clientLength = sizeof(client);
			int recv_result = recvfrom(_socket_handle, (char*)data, data_size, 0, (sockaddr*)&client, &clientLength);
			if (recv_result == SOCKET_ERROR) {
				return WSAGetLastError();
			}
			else {
				ZeroMemory(sender_ip, INET_ADDRSTRLEN); // to string of characters
				inet_ntop(AF_INET, &client.sin_addr, sender_ip, INET_ADDRSTRLEN);// Convert from byte array to chars
				return recv_result;
			}
		}
		else {
			fd_set mySet;
			FD_ZERO(&mySet);
			FD_SET(_socket_handle, &mySet);
			timeval zero = { 1, 0 };
			int sel = select(0, &mySet, NULL, NULL, &zero);
			if (FD_ISSET(_socket_handle, &mySet)) {
				sockaddr_in client; // Use to hold the client information (port / ip address)
				int clientLength = sizeof(client);
				int recv_result = recvfrom(_socket_handle, (char*)data, data_size, 0, (sockaddr*)&client, &clientLength);
				if (recv_result == SOCKET_ERROR) {
					return WSAGetLastError();
				}
				else {
					ZeroMemory(sender_ip, INET_ADDRSTRLEN); // to string of characters
					// Convert from byte array to chars
					inet_ntop(AF_INET, &client.sin_addr, sender_ip, INET_ADDRSTRLEN);
					return recv_result;
				}
			}
			else {
				std::cout << "Timed out" << std::endl;
				return -1;
			}
		}
	}

	Socket* Socket_WIN::accept_connection()
	{
		// Non Blocking accept with the help of select()
		// source: https://stackoverflow.com/questions/2843277/c-winsock-p2p
		fd_set mySet;
		FD_ZERO(&mySet);
		FD_SET(_socket_handle, &mySet);
		timeval zero = { 1, 0};
		// std::cout << "Socket starts select" << std::endl;
		int sel = select(0, &mySet, NULL, NULL, &zero);
		std::cout << "select" << sel << std::endl;
		if (FD_ISSET(_socket_handle, &mySet)) {
			std::cout << "Acception attempt" << std::endl;
			int new_socket_handle = accept(_socket_handle, NULL, NULL);
			Socket* connected_socket = new Socket_WIN(new_socket_handle);
			return connected_socket;
		}
		else {
			std::cout << "No connection " << WSAGetLastError() << std::endl;
			return nullptr;
		}
	}

	int Socket_WIN::close()
	{
		return closesocket(_socket_handle);
	}

	int Socket_WIN::stop(int how) 
	{
		return shutdown(_socket_handle, how);
	}

	std::string get_external_ip(std::string &url)
	{
		// url: "api.ipify.org"
		
		WSADATA wsaData;		
		std::string get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "WSAStartup failed.\n";
			return "";
		}

		SOCKET Socket;
		
		Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		struct addrinfo hints, *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		getaddrinfo(url.c_str(), "80", &hints, &res);
		struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
		addr = (struct sockaddr_in *)res->ai_addr;
		char address_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr->sin_addr, address_ip, INET_ADDRSTRLEN);

		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		inet_pton(AF_INET, address_ip, &(SockAddr.sin_addr));

		if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
			std::cout << "Could not connect";
			return "";
		}
		send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

		std::string website_HTML;
		std::locale local;
		char lineBuffer[200][80] = { ' ' };
		char buffer[10000];
		int i = 0, bufLen = 0, j = 0, lineCount = 0;
		int lineIndex = 0, posIndex = 0;
		int nDataLength;
		while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
			int i = 0;
			while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
				website_HTML += buffer[i];
				i += 1;
			}
		}

		closesocket(Socket);
		WSACleanup();

		for (size_t i = 0; i<website_HTML.length(); ++i) website_HTML[i] = tolower(website_HTML[i], local);

		std::istringstream ss(website_HTML);
		std::string stoken;
		char ip_address[INET_ADDRSTRLEN];

		while (getline(ss, stoken, '\n')) {

			strcpy_s(lineBuffer[lineIndex], stoken.c_str());
			int dot = 0;
			for (int ii = 0; ii< strlen(lineBuffer[lineIndex]); ii++) {

				if (lineBuffer[lineIndex][ii] == '.') dot++;
				if (dot >= 3) {
					dot = 0;
					strcpy_s(ip_address, lineBuffer[lineIndex]);
				}
			}

			lineIndex++;
		}
		std::cout << "Your IP Address is  " << ip_address << " \n\n";

		return ip_address;
	}

} // namespace eronic