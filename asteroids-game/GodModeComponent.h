#pragma once
#include "GameObjectComponent.h"
#include "Colour.h"

class GodModeComponent
	: public GameObjectComponent
{

	// Data
protected:
	int		_collisionID;
	float	_fader;
	float	_faderInc;
	bool	_godMode;
	Colour	_objectColour;

	// Structors
public:
	GodModeComponent(GameObject* gob);
	virtual ~GodModeComponent();


	// Gets/sets
public:
	bool GetGodMode()				const	{ return _godMode; }
	void SetGodMode(bool v)					{ _godMode = v; }
	void ToggleGodMode()					{ _godMode = !_godMode; }


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

};