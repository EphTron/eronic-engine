#pragma once

#include <string>


namespace eronic {

	typedef struct Network {
		Network() :
			network_id(-1),
			network_port(-1),
			network_ip("")
		{}

		Network(Network* n) :
			network_id(n->network_id),
			network_port(n->network_port),
			network_ip(network_ip)
		{}
		Network(const Network&) = delete;

		Network(Network&& source)
			: network_id(std::move(source.network_id))
			, network_port(std::move(source.network_port))
			, network_ip(std::move(source.network_ip))
		{  }

		int network_id;
		int network_port;
		std::string network_ip;
	}Network;
}