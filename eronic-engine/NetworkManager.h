#pragma once


#include <vector>
#include "PeerNode.h"

namespace eronic {

	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		std::vector<Network*> find_networks();
		void open_network();
		void join_network();

		void run_network_loop();

		std::vector<DataPackage*> get_network_data();
		void send_data_to_network();







	private:
		PeerNode * _peer;
		//std::vector<DataPackage*> _data_buffer;

		void collect_network_data();
	};

} // namespace eronic

