#pragma once

#include <string>


namespace eronic {

	typedef struct Network {
		Network() :
			network_id(-1),
			network_port(-1),
			network_ip("")
		{}

		int network_id;
		int network_port;
		std::string network_ip;
	}Network;
}