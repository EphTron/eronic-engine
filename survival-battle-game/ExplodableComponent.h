#pragma once
#include "GameObjectComponent.h"


class Explosion;


class ExplodableComponent
	: public GameObjectComponent
{
	// Data
protected:
	Explosion*	_explosion;
	bool		_exploding;
	bool		_exploded;

	// Structors
public:
	ExplodableComponent(GameObject* gob);
	virtual ~ExplodableComponent();

	// Gets/Sets
public:
	Explosion* GetExplosion()		const	{ return _explosion; }

	bool IsExploding()				const	{ return _exploding; }
	bool HasExploded()				const	{ return _exploded; }

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