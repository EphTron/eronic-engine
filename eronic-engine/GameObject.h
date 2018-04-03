#pragma once
#include "Mesh.h"
#include "Vector4.h"
#include "Colour.h"
#include "MathsHelper.h"

class GameObject
{
	// Constants
public:

	// Data
protected:
	float	_angle;		// Angle of object in degrees
	float	_scale;		// Scale of the object (1 = normal)
	Mesh*	_mesh;		// Vertex info
	Colour	_colour;	// Colour of object
	Vector4 _position;	// Position of object's origin
	Vector4 _velocity;	// Velocity of object
	bool	_alive;

	// Constructors
public:
	GameObject();
	virtual ~GameObject();

	GameObject(const GameObject&);
	GameObject& operator=(const GameObject&);

	// Gets/sets
public:
	Mesh* GetMesh()			const	{ return _mesh;  }
	void SetMesh(Mesh* m)			{ _mesh = m; }

	float GetAngle()		const	{ return _angle; }
	void SetAngle(float v)			{ _angle = v;  }

	float GetScale()		const	{ return _scale; }
	void SetScale(float v)			{ _scale = v; }

	Colour GetColour()		const	{ return _colour; }
	void SetColour(Colour c)		{ _colour = c; }

	Vector4 GetPosition()	const	{ return _position; }
	void SetPosition(Vector4 v)		{ _position = v; }

	Vector4 GetVelocity()	const	{ return _velocity; }
	void SetVelocity(Vector4 v)		{ _velocity = v; }

	bool IsAlive()			const	{ return _alive; }
	void SetAlive(bool v)			{ _alive = v; }

	// Functions
public:
	virtual bool CollideWith(const GameObject* other, float collisionRange);
	virtual void Update(double deltaTime);
};

