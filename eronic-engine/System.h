#pragma once
#include "GameObjectComponent.h"
#include <vector>

// System class 
// This provides the behaviours that act on the components
class System
{
	// Data
protected:


	// Structors
public:
	System();
	virtual ~System();

	// Get / sets
public:

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime) = 0;

};

