#pragma once
#include "Message.h"

class GameObject;

class DamageMessage
	: public Message
{
	// Data
protected:
	GameObject* _target;
	int _damage;

	// Structors
public:
	DamageMessage(GameObject* target, int damage=1);
	virtual ~DamageMessage();


	// Gets/Sets
public:
	GameObject* GetTarget()		const	{ return _target; }
	int GetDamage()				const	{ return _damage; }


	// Functions
public:


};