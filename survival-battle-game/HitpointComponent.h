#pragma once
#include "GameObjectComponent.h"


class HitpointComponent :
	public GameObjectComponent
{
	// Data
protected:
	int		_maxHitPoints;
	int		_hitPoints;

	// Structors
public:
	HitpointComponent(GameObject* gob);
	virtual ~HitpointComponent();

	// Gets/Sets
public:
	int GetMaxHitPoints()			const	{ return _maxHitPoints; }
	int GetHitPoints()				const	{ return _hitPoints; }

	void SetMaxHitPoints(int v)				{ _maxHitPoints = v; }
	void SetHitPoints(int v)				{ _hitPoints = v; }

	void IncrememtHitPoints(int v = 1)		{ _hitPoints += v; }
	void DecrememtHitPoints(int v = 1)		{ _hitPoints -= v; }

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

