#pragma once
#include "System.h"
#include "NetworkManager.h"

class NetworkSystem :
	public System
{

	// Data
protected:
	eronic::NetworkManager* _network_manager;

	// Structors
public:
	NetworkSystem();
	virtual ~NetworkSystem();

	// Get / sets
public:
	eronic::NetworkManager* GetNetworkManager()		const { return _network_manager; }
	void SetRenderer(eronic::NetworkManager* nwm) { _network_manager = nwm; }

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

