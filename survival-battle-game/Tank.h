#pragma once
#include "GameObject.h"
#include "Colour.h"

class Mesh;
class Message;

class Tank :
	public GameObject
{
	// Constants
public:
	static const float MAX_SPEED;
	static const float ACCELERATION;

	// Data
protected:
	float				_aim_angle;			// Angle of cannon in degrees

	// Structors
public:
	Tank(Mesh* mesh, Mesh* cannon);
	virtual ~Tank();

	// Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

