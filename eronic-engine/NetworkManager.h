#pragma once


#include <vector>
#include "Message.h"
#include "PeerNode.h"

namespace eronic {

	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		std::vector<Network*> find_networks();
		void open_network(int net_id, int net_port);
		void join_network(int net_id, int net_port);
		void leave_network();
		void retry_network();

		void start();
		void stop();
		void close();

		std::vector<DataPackage*> get_network_messages();
		void broadcast_message();
		void send_message();

		/// Respond to messages
		void OnMessage(Message* msg);


	private:
		bool _connected;
		bool _fully_connected;
		P2PNetworkManager * _peer;
		std::vector<Network*> networks;
		std::map<int, PeerPartner*> participants;
		//std::vector<DataPackage*> _data_buffer;

		void collect_network_data();
	};

} // namespace eronic

