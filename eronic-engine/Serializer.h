#pragma once

#include <iostream>

#define MSG_SIZE 128

namespace eronic {

	typedef struct Package
	{
		int type;
		int sender;
		char message[MSG_SIZE];
	}Package;

	// concept from http://amscata.blogspot.co.uk/2013/12/serialize-structure-in-c-for-socket.html

	void serialize(Package * package, char *serialized_data);
	void deserialize(char *serialized_data, Package* package);
	void print_package(Package* p)

} // namespace eronic