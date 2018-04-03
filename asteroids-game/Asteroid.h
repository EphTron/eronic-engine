#pragma once
#include "GameObject.h"
#include <stdlib.h>

class Asteroid :
	public GameObject
{
	// Constants
public:
	static const float MAX_DRIFT;

	// Structors
public:
	Asteroid();
	virtual ~Asteroid();

	// Functions
public:
	// Reset the asteroid to a random velocity somewhere along the edge
	void Reset();

	// Reset the asteroid to a random velocity at a given position and a given scale
	void Reset(float scale, const Vector4& pos);
};

