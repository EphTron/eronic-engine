#pragma once
#include "GameObject.h"
#include "Colour.h"
#include <stdlib.h>

class Mesh;
class Message;

class UFO :
	public GameObject
{
	// Constants
public:
	static const float MAX_SPEED;
	static const float FIRE_DELAY;	// In seconds

	// Data
protected:
	double	_fireTimer;		// How long before we fire another missile

	// Structors
public:
	UFO(Mesh* mesh);
	virtual ~UFO();

	// Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

