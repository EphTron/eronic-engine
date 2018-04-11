#pragma once


#include <vector>
#include "PeerNode.h"

namespace eronic {

	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		std::vector<DataPackage*> get_network_data();




	private:
		PeerNode * _peer;
		std::vector<DataPackage*> _data_buffer;

		void collect_network_data();
	};

} // namespace eronic

