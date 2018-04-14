#pragma once
#include "GameObjectComponent.h"

class LifeTimerComponent
	: public GameObjectComponent
{
	// Data
protected:
	double		_startingLifeTime;		// Default starting lifetime (s)
	double		_timeLeft;				// Lifespan left (s)


	// Structors
public:
	LifeTimerComponent(GameObject* gob, double lifeTime);
	virtual ~LifeTimerComponent();

	// Gets/Sets
public:
	double GetStartingLifeTime()		const	{ return _startingLifeTime; }
	void SetStartingLifeTime(double v)			{ _startingLifeTime = v; }
	
	double GetLifeTimeLeft()			const	{ return _timeLeft; }
	void SetLifeTimeLeft(double v)				{ _timeLeft = v; }
	void DecreaseLifeTime(double v)				{ _timeLeft -= v; }
	void ResetLifeTime()						{ _timeLeft = _startingLifeTime; }

	double GetPercentLifeLeft()			const	{ return _startingLifeTime > 0 ? _timeLeft / _startingLifeTime : 0; }

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
};