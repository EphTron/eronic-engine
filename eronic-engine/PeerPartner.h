#pragma once

#include "TCPClient.h"

namespace eronic {

	typedef struct PeerPartner {
		PeerPartner() :
			peer_id(-1),
			network_id(-1),
			peer_connection(false),
			answered_alive(false),
			tcp_client(nullptr),
			delta_t_to_self_t(0.0)
		{}

		PeerPartner(int p_id, int n_id, TCPClient * client) :
			peer_id(p_id),
			network_id(n_id),
			peer_connection(true),
			answered_alive(false),
			tcp_client(client),
			delta_t_to_self_t(0.0)
		{}

		int peer_id;
		int network_id;
		bool peer_connection;
		bool answered_alive;
		TCPClient * tcp_client;
		double delta_t_to_self_t;
	}PeerPartner;

}