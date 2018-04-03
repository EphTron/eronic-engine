#pragma once
#include "GameObject.h"

class Explosion :
	public GameObject
{
	// Constants
private:
	static const float LIFETIME;	// In seconds
	static const float MAX_SIZE;	// Max scale of the explosion

	// Data
private:
	float _life;

	// Structors
public:
	Explosion();
	virtual ~Explosion();

	// Functions
public:
	void Reset(const Vector4& pos);
	virtual void Update(double deltaTime);
};

