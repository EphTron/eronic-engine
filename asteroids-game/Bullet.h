#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
	// Constants
private:
	static const float BULLET_SPEED;
	static const float MAX_LIFE;	// How long the bullet lasts (in seconds)

	// Data
private:
	float _life;

	// Structors
public:
	Bullet();
	virtual ~Bullet();

	// Gets/sets
public:

	// Functions
public:
	void Reset(const Vector4& pos, float angle);
	virtual void Update(double deltaTime);
};

