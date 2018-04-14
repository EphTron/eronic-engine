#pragma once
#include "System.h"
#include <vector>

class PhysicsComponent;

// Moves objects around based on PhysicsComponents
class PhysicsSystem :
	public System
{
	// Data
protected:


	// Structors
public:
	PhysicsSystem();
	virtual ~PhysicsSystem();

	// Get / sets
public:

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

