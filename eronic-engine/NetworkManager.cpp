#include "NetworkManager.h"

namespace eronic {

	NetworkManager::NetworkManager():
		_connected(false),
		_fully_connected(false),
		_peer(),
		networks(std::vector<Network*>()),
		participants(std::map<int, PeerPartner*>())
	{

	}


	NetworkManager::~NetworkManager()
	{
	}

	std::vector<Network*> NetworkManager::find_networks()
	{
		std::vector<Network*> res = std::vector<Network*>();
		res = _peer->find_networks(7500, false);
		networks = res;
		return res;
	}

	void NetworkManager::open_network(int net_id, int net_port)
	{
		_peer->open_network(net_id, net_port);
		_connected = true;
	}

	void NetworkManager::join_network(int net_id, int net_port)
	{
		_peer->join_network(net_id, net_port);
		_connected = true;
	}

	void NetworkManager::leave_network()
	{
	}

	void NetworkManager::retry_network()
	{
	}

	void NetworkManager::start()
	{
		_peer->run_peer_network();
	}

	void NetworkManager::stop()
	{
	}

	void NetworkManager::close()
	{
	}

	std::vector<DataPackage*> NetworkManager::get_network_messages()
	{
		return std::vector<DataPackage*>();
	}

	void NetworkManager::broadcast_message()
	{
	}

	void NetworkManager::send_message()
	{
	}

	void NetworkManager::OnMessage(Message * msg)
	{
	}


} // namespace eronic