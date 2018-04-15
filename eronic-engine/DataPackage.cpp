#include "DataPackage.h"

#define MSG_SIZE 128
#define DATA_PACK_SIZE sizeof(eronic::DataPackage)


namespace eronic {

	DataPackage::DataPackage():
		type(-1),
		sender_id(-1),
		sender_port(-1),
		network_id(-1),
		int_data_1(-1),
		int_data_2(-1),
		int_data_3(-1),
		x(-1.0),
		y(-1.0),
		z(-1.0),
		sender_ip("\0"),
		message("\0")
	{
	}

	DataPackage::DataPackage(int type, int id, int port, int net_id, 
							 int d1, int d2, int d3, 
							 float fx, float fy, float fz, 
							std::string & ip, std::string & msg):
		type(type),
		sender_id(id),
		sender_port(port),
		network_id(net_id),
		int_data_1(d1),
		int_data_2(d2),
		int_data_3(d3),
		x(fx),
		y(fy),
		z(fz),
		sender_ip(),
		message()
	{
		strcpy_s(sender_ip, INET_ADDRSTRLEN, ip.c_str() );
		strcpy_s(message, MSG_SIZE, msg.c_str() );
	}

	DataPackage::DataPackage(char * serialized_data) :
		type(-1),
		sender_id(-1),
		sender_port(-1),
		network_id(-1),
		int_data_1(-1),
		int_data_2(-1),
		int_data_3(-1),
		x(-1.0),
		y(-1.0),
		z(-1.0),
		sender_ip("\0"),
		message("\0")
	{
		int *q = (int*)serialized_data;
		type = *q; q++;
		sender_id = *q; q++;
		sender_port = *q; q++;
		network_id = *q; q++;
		int_data_1 = *q; q++;
		int_data_2 = *q; q++;
		int_data_3 = *q; q++;

		float *f = (float*)q;
		x = *f; f++;
		y = *f; f++;
		z = *f; f++;

		char *p = (char*)q;
		int i = 0;
		while (i < INET_ADDRSTRLEN)
		{
			sender_ip[i] = *p;
			p++;
			i++;
		}

		i = 0;
		while (i < INET_ADDRSTRLEN)
		{
			message[i] = *p;
			p++;
			i++;
		}
	}


	DataPackage::~DataPackage()
	{
	}

	void DataPackage::serialize(char* out_data)
	{
		int *q = (int*)out_data;
		*q = type; q++;
		*q = sender_id; q++;
		*q = sender_port; q++;
		*q = network_id; q++;
		*q = int_data_1; q++;
		*q = int_data_2; q++;
		*q = int_data_3; q++;

		float *f = (float*)q;
		*f = x; f++;
		*f = y; f++;
		*f = z; f++;

		char *p = (char*)q;
		int i = 0;
		while (i < INET_ADDRSTRLEN)
		{
			*p = sender_ip[i];
			p++; i++;
		}

		i = 0;
		while (i < MSG_SIZE)
		{
			*p = message[i];
			p++; i++;
		}

		//q = nullptr;
		//delete q;
		//p = nullptr;
		//delete p;
	}

	void DataPackage::set_ip(std::string & ip)
	{
		strcpy_s(sender_ip, INET_ADDRSTRLEN, ip.c_str());
		
	}

	void DataPackage::set_msg(std::string & msg)
	{
		strcpy_s(message, MSG_SIZE, msg.c_str());
	}

} // namespace eronic