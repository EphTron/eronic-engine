#pragma once
#include "GameObject.h"

class Ship :
	public GameObject
{
	// Constants
public:
	static const float MAX_SPEED;
	static const float ACCELERATION;

	// Data
public:

	// Structors
public:
	Ship();
	virtual ~Ship();

	// Functions
public:
	void TurnLeft(float angle)					{ _angle -= angle; if (_angle < 0) _angle += 360; }
	void TurnRight(float angle)					{ _angle += angle; if (_angle > 360) _angle -= 360; }
	void Accelerate(float amt = ACCELERATION)	;

	virtual void Update(double deltaTime)		;
};

