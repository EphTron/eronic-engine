#include "GameObject.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GameObject::GameObject()
	:	_angle(0),
		_scale(1),
		_position(0,0),
		_velocity(0,0),
		_colour(1,1,1,1),
		_alive(true),
		_mesh(NULL)
{
}

/******************************************************************************************************************/

GameObject::~GameObject()
{
}

/******************************************************************************************************************/

GameObject::GameObject(const GameObject& other)
{
	_angle = other.GetAngle();
	_scale = other.GetScale();
	_position = other.GetPosition();
	_velocity = other.GetVelocity();
	_colour = other.GetColour();
	_mesh = other.GetMesh();
}

/******************************************************************************************************************/

GameObject& GameObject::operator=(const GameObject& other)
{
	_angle = other.GetAngle();
	_scale = other.GetScale();
	_position = other.GetPosition();
	_velocity = other.GetVelocity();
	_colour = other.GetColour();
	_mesh = other.GetMesh();
	return *this;
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

bool GameObject::CollideWith(const GameObject* other, float collisionRange)
{
	Vector4 diff = other->GetPosition();
	diff -= this->GetPosition();

	return (diff.length() < collisionRange);
}

/******************************************************************************************************************/

void GameObject::Update(double deltaTime)
{
	Vector4 vel = _velocity;
	vel *= (float)deltaTime;
	_position += vel;

	// Wrap around
	if (_position.x() < -1) _position.x(_position.x() + 2);
	else if (_position.x() > 1) _position.x(_position.x() - 2);
	if (_position.y() < -1) _position.y(_position.y() + 2);
	else if (_position.y() > 1) _position.y(_position.y() - 2);
}

/******************************************************************************************************************/
