#pragma once
#include "System.h"
class AISystem :
	public System
{


	// Data
protected:


	// Structors
public:
	AISystem();
	virtual ~AISystem();

	// Get / sets
public:

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

