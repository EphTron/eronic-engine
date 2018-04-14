#pragma once
#include "Message.h"

class GameObject;

class DeadObjectMessage
	: public Message
{
	// Data
protected:
	GameObject* _deadObject;

	// Structors
public:
	DeadObjectMessage(GameObject* deadObject);
	virtual ~DeadObjectMessage();


	// Gets/Sets
public:
	GameObject* GetDeadObject()		const	{ return _deadObject; }



	// Functions
public:


};