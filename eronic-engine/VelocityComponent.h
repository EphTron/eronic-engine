#pragma once
#include "GameObjectComponent.h"
#include "Vector4.h"

class VelocityComponent
	: public GameObjectComponent
{
	// Data
protected:
	Vector4			_velocity;	// Velocity of object


	// Structors
public:
	VelocityComponent(GameObject* gob);
	virtual ~VelocityComponent();

	// Gets/Sets
public:
	Vector4 GetVelocity()	const	{ return _velocity; }
	void SetVelocity(Vector4 v)		{ _velocity = v; }

	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();

	// Extra Functions
public:
	void LimitToMaximumSpeed(float max);
};