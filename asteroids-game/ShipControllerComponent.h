#pragma once
#include "UserInputComponent.h"


class Bullet;


class ShipControllerComponent
	: public UserInputComponent
{
	// Constants
public:
	static const float ACCELERATION;
	static const int NUM_BULLETS = 5;


	// Data
protected:
	bool		_turnLeftPressed;
	bool		_turnRightPressed;
	bool		_acceleratePressed;

	Bullet*		_bullets[NUM_BULLETS];

	// Structors
public:
	ShipControllerComponent(GameObject* gob);
	virtual ~ShipControllerComponent();

	// Gets/Sets
public:


	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start()						;

	// Main update function (called every frame)
	virtual void Update(double deltaTime)		;

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg)		;

	// Shutdown function -- called when parent object is destroyed
	virtual void End()							;


	// New Functions
public:
	void TurnLeft(float angle)					;
	void TurnRight(float angle)					;
	void Accelerate(float amt)					;
	void FireBullet()							;
	void Reset()								;
};