#pragma once
#include "GameObject.h"


class Mesh;


class Bullet :
	public GameObject
{
	// Constants
private:
	static const float BULLET_SPEED;
	static const float MAX_LIFE;	// How long the bullet lasts (in seconds)

	// Data
private:


	// Structors
public:
	Bullet(Mesh* mesh);
	virtual ~Bullet();


	// Gets/sets
public:


	// Functions
public:
	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);
	
	// Reset to start state
	virtual void Reset();


	// Extra Functions
public:
	void Spawn(const Vector4& pos, float angle);
};

