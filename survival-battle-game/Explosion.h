#pragma once
#include "GameObject.h"


class Mesh;


class Explosion :
	public GameObject
{
	// Constants
private:
	static const float LIFETIME;	// In seconds
	static const float MAX_SIZE;	// Max scale of the explosion

	// Structors
public:
	Explosion(Mesh* mesh);
	virtual ~Explosion();

	// Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();

	// Extra Functions
public:
	void Spawn(const Vector4& pos);
};

