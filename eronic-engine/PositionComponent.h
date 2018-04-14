#pragma once
#include "GameObjectComponent.h"
#include "Vector4.h"

class PositionComponent
	: public GameObjectComponent
{
	// Data
protected:
	Vector4			_position;	// Position of object


	// Structors
public:
	PositionComponent(GameObject* gob);
	virtual ~PositionComponent();

	// Gets/Sets
public:
	Vector4 GetPosition()	const	{ return _position; }
	void SetPosition(Vector4 v)		{ _position = v; }

	// Functions
public:
};