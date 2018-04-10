#pragma once

#include <iostream>
#include <WS2tcpip.h>

#define MSG_SIZE 128


namespace eronic {

	struct DataPackage
	{
		DataPackage();
		DataPackage(int type, int id, int port, int net_id, 
					int d1, int d2, int d3, 
					float x, float y, float z, 
					std::string & ip, std::string & message);
		DataPackage(char* serialized_data);
		~DataPackage();

		char * serialize();
		void set_ip(std::string & ip);
		void set_msg(std::string & msg);

		int type;
		int sender_id;
		int sender_port;
		int network_id;
		int int_data_1;
		int int_data_2;
		int int_data_3;
		float x;
		float y;
		float z;
		char sender_ip[INET_ADDRSTRLEN];
		char message[MSG_SIZE];

	};

	// concept from http://amscata.blogspot.co.uk/2013/12/serialize-structure-in-c-for-socket.html

	// void serialize(Package * package, char *serialized_data);
	// void deserialize(char *serialized_data, Package* package);
	// void print_package(Package* p);

} // namespace eronic