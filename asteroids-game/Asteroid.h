#pragma once
#include "GameObject.h"
#include <stdlib.h>


class Mesh;


class Asteroid :
	public GameObject
{
	// Constants & statics
public:
	static const float MAX_DRIFT;
	static int count;	// Counts how many asteroids there are

	// Structors
public:
	Asteroid(Mesh* mesh);
	virtual ~Asteroid();


	// Gets/sets
public:


	// Functions
public:
	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Reset the asteroid to a random velocity somewhere along the edge
	virtual void Reset();

	// Extra Functions
public:
	// Reset the asteroid to a random velocity at a given position and a given scale
	void Spawn(float scale, const Vector4& pos);
};

