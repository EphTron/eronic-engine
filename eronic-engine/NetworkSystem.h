#pragma once
#include "System.h"
#include "P2PNetworkManager.h"
#include "NetworkManager.h"

class NetworkSystem :
	public System
{

	// Data
protected:
	//eronic::NetworkManager* _network_manager;
	eronic::P2PNetworkManager*	_network_manager;
	std::string					_tag;

	// Structors
public:
	NetworkSystem(std::string tag);
	virtual ~NetworkSystem();

	// Get / sets
public:
	eronic::P2PNetworkManager* GetNetworkManager()		const { return _network_manager; }
	void SetNetworkManager(eronic::P2PNetworkManager* nwm) { _network_manager = nwm; }

	void set_tag(std::string tag) { _tag = tag; };
	std::string const& get_tag() { return _tag; };

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

