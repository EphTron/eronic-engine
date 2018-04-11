#include "Serializer.h"

namespace eronic {

	void serialize(Package * package, char *serialized_data)
	{
		int *q = (int*)serialized_data;
		*q = package->type;
		q++;
		*q = package->sender;
		q++;

		char *p = (char*)q;
		int i = 0;
		while (i < MSG_SIZE)
		{
			*p = package->message[i];
			p++;
			i++;
		}
		q = nullptr;
		delete q;
		p = nullptr;
		delete p;
	}

	void deserialize(char *serialized_data, Package* package)
	{
		int *q = (int*)serialized_data;
		package->type = *q;
		q++;
		package->sender = *q;
		q++;

		char *p = (char*)q;
		int i = 0;
		while (i < MSG_SIZE)
		{
			package->message[i] = *p;
			p++;
			i++;
		}
		q = nullptr;
		delete q;
		p = nullptr;
		delete p;
	}

	void print_package(Package * p)
	{
		std::cout << p->type << std::endl;
		std::cout << p->sender << std::endl;
		std::cout << p->message << std::endl;
	}

} // namespace eronic