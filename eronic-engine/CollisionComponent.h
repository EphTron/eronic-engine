#pragma once
#include "GameObjectComponent.h"
#include "Vector4.h"
#include "GameObject.h"

class CollisionComponent
	: public GameObjectComponent
{
	// Data
protected:
	float	_radius;			// Collision radius of object
	int		_collisionMatrix;	// Who to collide with (bitfields)
	int		_collisionID;		// This object's collision type

	// Structors
public:
	CollisionComponent(GameObject* gob);
	virtual ~CollisionComponent();


	// Gets/Sets
public:
	float GetCollisionRadius()				const	{ return _radius * _parent->GetScale(); }
	void SetCollisionRadius(float r)				{ _radius = r; }

	// Collision matrix
	void SetCollisionMatrixFlag(int flag)			{ _collisionMatrix |= flag; }
	void UnsetCollisionMatrixFlag(int flag)			{ _collisionMatrix &= ~flag; }
	bool CheckCollisionMatrixFlag(int flag)	const	{ return (_collisionMatrix & flag) != 0; }
	int GetCollisionMatrix()				const	{ return _collisionMatrix; }

	// Collision ID
	int GetCollisionID()					const	{ return _collisionID; }
	void SetCollisionID(int id)						{ _collisionID = id; }

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