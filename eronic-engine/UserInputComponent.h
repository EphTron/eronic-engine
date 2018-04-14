#pragma once
#include "GameObjectComponent.h"
#include "Vector4.h"

class UserInputComponent
	: public GameObjectComponent
{
	// Data
protected:


	// Structors
public:
	UserInputComponent(GameObject* gob);
	virtual ~UserInputComponent();

	// Gets/Sets
public:


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