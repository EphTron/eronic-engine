#pragma once

#include <iostream>
#include <WS2tcpip.h>

#define MSG_SIZE 128


namespace eronic {

	struct DataPackage
	{
		DataPackage();
		DataPackage(int type, int id, int port, int net_id, std::string & ip, std::string & msg);
		DataPackage(int type, int id, int port, int net_id, 
					int d1, int d2, int d3, 
					float x, float y, float z,
					float a, float f_1, float f_2, float f_3,
					std::string & ip, std::string & msg);
		DataPackage(char* serialized_data);
		~DataPackage();

		void serialize(char * out_data);
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
		float angle;
		float f1;
		float f2;
		float f3;
		char sender_ip[INET_ADDRSTRLEN];
		char message[MSG_SIZE];

	};

	// concept from http://amscata.blogspot.co.uk/2013/12/serialize-structure-in-c-for-socket.html

	// void serialize(Package * package, char *serialized_data);
	// void deserialize(char *serialized_data, Package* package);
	// void print_package(Package* p);

} // namespace eronic