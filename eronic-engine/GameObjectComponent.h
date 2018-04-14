#pragma once
#include <string>
#include "Observer.h"

// Forward references
class GameObject;
class Message;


class GameObjectComponent
	: public Observer // Implement observer pattern
{
	// Data
protected:
	GameObject* _parent;
	std::string _componentType;

	// Structors
public:
	GameObjectComponent(std::string type, GameObject* parent);
	virtual ~GameObjectComponent();

	// Copy/assignment
private:
	GameObjectComponent(const GameObjectComponent&);
	GameObjectComponent& operator=(const GameObjectComponent&);

	// Gets/Sets
public:
	GameObject* GetGameObject()			const	{ return _parent; }
	std::string GetComponentType()		const	{ return _componentType; }


	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start()					= 0;

	// Main update function (called every frame)
	virtual void Update(double deltaTime)	= 0;

	// Shutdown function -- called when parent object is destroyed
	virtual void End()						= 0;

	// Broadcast a message to all objects
	void BroadcastMessage(Message* msg)		;
};