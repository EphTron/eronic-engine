#include "NetworkManager.h"

namespace eronic {

	NetworkManager::NetworkManager()
	{
	}


	NetworkManager::~NetworkManager()
	{
	}

	std::vector<Network*> NetworkManager::find_networks()
	{
		_peer->find_networks(7500, false);
		return std::vector<Network*>();
	}

	void NetworkManager::run_network_loop()
	{
		_peer->run_peer_network();
	}

} // namespace eronic