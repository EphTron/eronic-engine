#pragma once
#include "GameObject.h"
#include "CollisionMatrix.h"
#include <string>

class Mesh;


class Missile :
	public GameObject
{
	// Constants
private:
	static const float	MISSILE_SPEED;		// Speed in half screens/sec
	static const float	MAX_LIFE;			// How long the Missile lasts (in seconds)
	static const float	TURN_RATE;			// Deg/sec

	// Data
private:
	GameObject*			_target;			// Target to home in on
	std::string			_validTargetType;	// Type of object we can target

	// Structors
public:
	Missile(Mesh* mesh);
	virtual ~Missile();


	// Gets/sets
public:
	GameObject* GetTarget()				const	{ return _target;  }
	void SetTarget(GameObject* o)				{ _target = o; }

	// Valid Targets
	void SetValidTargetType(std::string s)		{ _validTargetType = s; }
	std::string GetValidTargetType()	const	{ return _validTargetType; }

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
	GameObject* FindValidTarget();
	void Spawn(const Vector4& pos);
};

