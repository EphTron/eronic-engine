#pragma once
#include "System.h"
#include "P2PNetworkManager.h"

class NetworkSystem :
	public System
{

	// Data
protected:
	eronic::P2PNetworkManager* _network_manager;

	// Structors
public:
	NetworkSystem();
	virtual ~NetworkSystem();

	// Get / sets
public:
	eronic::P2PNetworkManager* GetNetworkManager()		const { return _network_manager; }
	void SetNetworkManager(eronic::P2PNetworkManager* nwm) { _network_manager = nwm; }

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

