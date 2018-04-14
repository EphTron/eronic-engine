#pragma once
#include "Message.h"

class GameObject;

class CollisionMessage
	: public Message
{
	// Data
protected:
	GameObject* _collider;
	GameObject* _collidee;

	// Structors
public:
	CollisionMessage(GameObject* collider, GameObject* collidee);
	virtual ~CollisionMessage();


	// Gets/Sets
public:
	GameObject* GetCollider()								const	{ return _collider; }
	GameObject* GetCollidee()								const	{ return _collidee; }
	GameObject* GetOtherCollisionObject(GameObject* obj)	const	{ return _collider == obj ? _collidee : _collider; }

	// Functions
public:


};