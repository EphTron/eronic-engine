#pragma once
#include "GameObject.h"
#include "Colour.h"

class Mesh;
class Message;

class Ship :
	public GameObject
{
	// Constants
public:
	static const float MAX_SPEED;
	static const float ACCELERATION;

	// Data
protected:

	// Structors
public:
	Ship(Mesh* mesh);
	virtual ~Ship();

	// Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

