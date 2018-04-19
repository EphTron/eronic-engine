#pragma once
#include "System.h"
#include "AIController.h"
class AISystem :
	public System
{


	// Data
protected:
	std::vector<AIController*> _ai_controllers;

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

